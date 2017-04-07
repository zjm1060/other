package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.model.T_log;
import bitc.sz.wzEnv.query.BaseDAO;
import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.report.db.Executable;
import bitc.sz.wzEnv.report.db.ReportDAO;
import bitc.sz.wzEnv.report.db.ReportDAOImpl;
import bitc.sz.wzEnv.report.db.SQLExecutor;
import bitc.sz.wzEnv.report.model.*;
import bitc.sz.wzEnv.report.sql.*;
import bitc.sz.wzEnv.report.sql.Types;
import bitc.sz.wzEnv.report.util.ReportUtil;
import bitc.sz.wzEnv.report.util.SQLReportUtil;
import bitc.sz.wzEnv.sys.spring.SpringContextHolder;
import bitc.sz.wzEnv.util.BeanUtils;
import bitc.sz.wzEnv.util.JsonUtil;
import net.sf.jsqlparser.JSQLParserException;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.sql.*;
import java.util.*;

@Service
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
public class ReportServiceImpl implements ReportService {
    private final static Logger logger = Logger.getLogger(ReportServiceImpl.class);
    @Autowired
    private ReportDAO reportDAO;
    @Autowired
    private BaseDAO baseDAO;

    @Autowired
    private ReportExecutorFactory reportExecutorFactory;

    @Override
    public List<DBConnect> getDbConnects() {
        return reportDAO.getDbConnects();
    }

    @Override
    public void setReportDAO(ReportDAOImpl reportDAO) {
        this.reportDAO = reportDAO;
    }

    @Override
    public String doInitSQLReport(Report report, List<Parameter> defaultValues) throws SQLException, ReportException, JSQLParserException {

        ReportExecutor executor=reportExecutorFactory.getReportExecutor(report);

//        List<Parameter> parameters=executor.initParameter(report, defaultValues);
        report.setParameters(defaultValues);

        List<Column> cols=executor.initColumns(report);
        report.setColumns(cols);

        report.setDbType(reportDAO.getDbConnectByName(report.getDbName()).getDbType());
        reportDAO.saveReport(report);
        return report.getId();
    }

    @Override
    public Report getReport(String reportId) {
        return baseDAO.load(Report.class, reportId);
    }

    @Override
    public Report getReportByName(String token) throws ReportException {
        Report report = reportDAO.findReportByName(token);
        if (report == null) {
            throw new ReportException("不存在名为" + token + "的报表");
        }
        return report;
    }

    @Override
    public void doInitForm(final Form form, final String tableName, final String rowKey) throws SQLException {
        SQLExecutor executor = new SQLExecutor(form.getDbName());
        final String sql = "select * from " + tableName + " where 1=2";
        List<Field> fields = executor.execute(new Executable<List<Field>>() {
            public List<Field> execute(Connection connection) throws SQLException {
                PreparedStatement ps = connection.prepareStatement(sql);
                ResultSet rs = ps.executeQuery();
                ResultSetMetaData meta = rs.getMetaData();
                List<Field> list = new ArrayList<Field>();
                for (int i = 0; i < meta.getColumnCount(); i++) {
                    Field field = new Field();
                    field.setName(meta.getColumnName(i + 1));
                    field.setSort(i);
                    field.setFieldLabel(field.getName());
                    field.setSqlType(Types.getFieldType(meta.getColumnType(i + 1)));
                    field.setRowKey(rowKey);
                    field.setTableName(tableName);
                    if (field.getName().equalsIgnoreCase(rowKey)) {
                        field.setInsertValueGenerator("uuid");
                    }
                    list.add(field);
                }
                return list;
            }
        });
        form.setFields(fields);
        baseDAO.saveOrUpdate(form);
    }

    @Override
    public Object getForm(String formId) {
        return baseDAO.load(Form.class, formId);
    }

    @Override
    public Form getFormByName(String reportToken) throws FormException {
        Form form = reportDAO.findFormByName(reportToken);
        if (form == null) {
            throw new FormException("不存在名为" + reportToken + "的表单");
        }
        return form;
    }

    @Override
    public void saveOrUpdate(Form form, List<FormField> formFields) throws Exception {
        //按表名-列名分组
        Map<String, List<FormField>> map = new HashMap<String, List<FormField>>();
        List<String> keys = new ArrayList<String>();
        for (FormField formField : formFields) {
            String key = formField.getTableName() + (StringUtils.isEmpty(formField.getRowKeyValue()) ? "-insert-" : "-update-" + formField.getRowKeyValue());
            List<FormField> newFields = map.get(key);
            keys.add(key);
            if (newFields == null) {
                newFields = new ArrayList<FormField>();
                map.put(key, newFields);
            }
            newFields.add(formField);
        }
        //值生成
        Map<String, String> generateKeys = new HashMap<String, String>();
        for (String key : keys) {
            List<FormField> list = map.get(key);
            if (key.endsWith("-insert-")) {
                for (FormField formField : list) {
                    String value = ReportUtil.generateInsertValue(formField, formFields);
                    if (value != null) {
                        formField.setValue(value);
                        generateKeys.put(formField.getTableName() + "-" + formField.getName(), value);
                    }
                }
            } else {
                for (FormField formField : list) {
                    String value = ReportUtil.generateUpdateValue(formField, formFields);
                    if (value != null) {
                        formField.setValue(value);
                        generateKeys.put(formField.getTableName() + "-" + formField.getName(), value);
                    }
                }
            }
        }
        //调用监听
        FormListener listener = null;
        if (StringUtils.isNotEmpty(form.getListener())) {
            listener = (FormListener) SpringContextHolder.getBean(form.getListener());
        }
        if (listener != null) {
            if (!listener.beforeSave(form, map, generateKeys)) {
                return;
            }
        }

        SQLExecutor executor = new SQLExecutor(form.getDbName());

        try {
            for (String key : map.keySet()) {
                List<FormField> list = map.get(key);
                if (key.endsWith("-insert-")) {
                    InsertCommand insertCommand = new InsertCommand(executor, list.get(0).getTableName());
                    for (FormField field : list) {
                        insertCommand.addColumn(field.getName(), field.getObjectValue());
                    }
                    insertCommand.execute();
                } else {
                    UpdateCommand updateCommand = new UpdateCommand(executor, list.get(0).getTableName(),
                            list.get(0).getRowKey(), list.get(0).getRowKeyValue());

                    for (FormField field : list) {
                        updateCommand.addColumn(field.getName(), field.getObjectValue());
                    }
                    updateCommand.execute();
                }
            }
            if (listener != null) {
                listener.afterSave(form, executor, formFields, map, generateKeys);
            }
            executor.commit();
            if (listener != null) {
                listener.afterCommit();
            }
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw  e;
        } finally {
            executor.close();
        }
    }
    @Override
    public List<CombField> getCombos(CombField field) throws Exception {
        SQLExecutor sqlExecutor = new SQLExecutor(field.getDbName());
        final String sql = "select " + field.getValueColumn() + "," + field.getDisplayColumn() + " from " + field.getTableName() + (StringUtils.isEmpty(field.getCondition()) ? "" : " where " + field.getCondition());
        return sqlExecutor.execute(new Executable<List<CombField>>() {
            @Override
            public List<CombField> execute(Connection connection) throws SQLException {
                PreparedStatement ps = connection.prepareStatement(sql);
                ResultSet rs = ps.executeQuery();
                List<CombField> list = new ArrayList<CombField>();
                while (rs.next()) {
                    CombField combField = new CombField();
                    combField.setValueColumn(rs.getString(1));
                    combField.setDisplayColumn(rs.getString(2));
                    list.add(combField);
                }
                return list;

            }
        });
    }

    @Override
    public void updateReport(Report report, List<Parameter> list) throws SQLException, ReportException {

        ReportExecutor executor=reportExecutorFactory.getReportExecutor(report);

//        List<Parameter> parameters=executor.initParameter(report, defaultValues);
        report.setParameters(list);

        List<Column> cols=executor.initColumns(report);

        Report oldReport = baseDAO.load(Report.class, report.getId());
        if (oldReport.getSql().equals(report.getSql())) {//sql语句未改变
            BeanUtils.copyNotNullProperties(oldReport, report);
            oldReport.setDbType(reportDAO.getDbConnectByName(report.getDbName()).getDbType());
            baseDAO.saveOrUpdate(oldReport);
            return;
        }
        BeanUtils.copyNotNullProperties(oldReport, report);
//        SQLExecutor executor = new SQLExecutor(report.getDbName());
//        final String sql = SQLReportUtil.getNoDataSQL(report.getSql());
//        List<Column> cols = executor.execute(new Executable<List<Column>>() {
//            public List<Column> execute(Connection connection) throws SQLException {
//                PreparedStatement ps = connection.prepareStatement(sql);
//                ResultSet rs = ps.executeQuery();
//                ResultSetAdapter rsa = new ResultSetAdapter(rs);
//                return rsa.getColumns();
//            }
//        });

        List<Column> newList = new ArrayList<Column>();
        for (Column column : cols) {
            for (Column oldColumn : oldReport.getColumns()) {
                if (column.getName().equals(oldColumn.getName())) {
                    column = oldColumn;
                    break;
                }
            }
            newList.add(column);
        }
        oldReport.setColumns(newList);
        oldReport.setDbType(reportDAO.getDbConnectByName(report.getDbName()).getDbType());
        reportDAO.saveReport(oldReport);
    }

    @Override
    public void deleteReport(String reportId) {
        reportDAO.delete(reportId);
    }

    @Override
    public void deleteRows(DeleteCommand deleteCommand) throws Exception {
        deleteCommand.execute();
    }

    @Override
    public void saveLog(T_log log) {
        reportDAO.saveLog(log);
    }

    @Override
    public GridData selectGridData(String reportToken, PagingInfo info, String parameterValues, String reportCondition) throws ReportException {
        List<String> parameters = null;
        if(StringUtils.isNotEmpty(parameterValues)){
            parameters = JsonUtil.readJson(parameterValues, ArrayList.class, String.class);
        }
        Report report  = getReportByName(reportToken);
        ReportExecutor executor=reportExecutorFactory.getReportExecutor(report);
        GridData data = executor.selectGridData(report,info,parameters!= null?parameters.toArray():null,reportCondition);
        return data;
    }
}
