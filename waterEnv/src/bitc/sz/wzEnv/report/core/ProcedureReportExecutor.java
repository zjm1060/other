package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.report.db.Executable;
import bitc.sz.wzEnv.report.db.SQLExecutor;
import bitc.sz.wzEnv.report.model.*;
import bitc.sz.wzEnv.report.sql.ResultSetAdapter;
import bitc.sz.wzEnv.report.sql.Types;
import bitc.sz.wzEnv.report.util.SQLReportUtil;
import bitc.sz.wzEnv.util.JsonUtil;
import oracle.jdbc.OracleTypes;
import org.apache.commons.lang.StringUtils;
import org.hibernate.dialect.SQLServer2005Dialect;
import org.hibernate.loader.custom.Return;
import org.springframework.stereotype.Component;

import java.sql.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by BomWu on 2016/3/14 0014.
 */
@Component
public class ProcedureReportExecutor implements ReportExecutor {
//    @Override
//    public List<Parameter> initParameter(Report report, String defaultValues) {
//        List<Parameter> defaultValuesList = null;
//        if(StringUtils.isNotEmpty(defaultValues)){
//            defaultValuesList = JsonUtil.readJson(defaultValues, ArrayList.class, Parameter.class);
//        }
//        List<Parameter> parameters = new ArrayList<Parameter>();
//        String sql = report.getSql();
//        int count = 0;
//        if (sql != null) {
//            Pattern p = Pattern.compile("\\(.*?\\)");// 查找规则公式中小括号以内的字符
//            Matcher m = p.matcher(sql);
//            String param = null;
//            while (m.find()) {// 遍历找到的所有小括号
//                param = m.group().replaceAll("\\(", "").replaceAll("\\)", "");// 去掉括号
//            }
//            count = param.split(",").length;
//        }
//        Parameter parameter = null;
//        for (int i = 0; i < count; i++) {
//            parameter = new Parameter();
//            if (i == count - 1) {
//                parameter.setParameterType(ParameterType.OUT);
//            } else {
//                parameter.setParameterType(ParameterType.IN);
//            }
//            parameter.setType(Types.STRING);
//            parameter.setReportId(report.getId());
//            parameter.setDefaultValue("");
//            parameters.add(parameter);
//        }
//        return defaultValuesList;
//    }

    @Override
    public List<Column> initColumns(final Report report) throws ReportException {
        try {
            SQLExecutor executor = new SQLExecutor(report.getDbName());
            final String sql = SQLReportUtil.getNoDataSQL(report.getSql());
            int cursorIndex = -1;
            List<Parameter> list = report.getParameters();
            if (list != null && list.size() > 0) {
                for (int i = 1; i <= list.size(); i++) {
                    if (list.get(i - 1).getParameterType() == ParameterType.OUT) {
                        cursorIndex = i;
                    }
                }
            }
            final int finalCursorIndex = cursorIndex;
            List<Column> cols = executor.execute(new Executable<List<Column>>() {
                public List<Column> execute(Connection connection) throws SQLException {
                    CallableStatement call = connection.prepareCall(report.getSql());
                    List<Parameter> list = report.getParameters();
                    for (int i = 1; i <= list.size(); i++) {
                        Parameter parameter = list.get(i - 1);
                        if (parameter.getParameterType() == ParameterType.OUT) {
                            call.registerOutParameter(i, OracleTypes.CURSOR);
                        } else {
                            call.setObject(i, parameter.getDefaultValue());
                        }
                    }
                    call.execute();
                    if (finalCursorIndex > -1) {
                        ResultSet rs = (ResultSet) call.getObject(finalCursorIndex);
                        ResultSetAdapter rsa = new ResultSetAdapter(rs);
                        return rsa.getColumns();
                    }
                    return null;
                }
            });
            return cols;
        } catch (SQLException e) {
            throw new ReportException("init columns error!", e);
        }
    }

    @Override
    public GridData selectGridData(final Report report, final PagingInfo info, final Object[] parameterValues, String condition) throws ReportException {
        SQLExecutor executor = null;
        try {
            executor = new SQLExecutor(report.getDbName());
            final GridData cols = executor.execute(new Executable<GridData>() {
                public GridData execute(Connection connection) throws SQLException {
                    CallableStatement call = connection.prepareCall(report.getSql());
                    int CursorIndex = -1;
                    List<Parameter> list = report.getParameters();
                    int rowCount = 0;
                    if (list != null && list.size() > 0) {
                        for (int i = 1; i <= list.size(); i++) {
                            Parameter parameter = list.get(i - 1);
                            if (parameter.getParameterType() == ParameterType.OUT) {
                                call.registerOutParameter(i, OracleTypes.CURSOR);
                                CursorIndex = i;
                            } else {
                                call.setObject(i, parameterValues[i-1]);
                            }
                        }
                        call.execute();
                        if (CursorIndex > -1) {
                            GridData data = new GridData();
                            ResultSet rs = (ResultSet) call.getObject(CursorIndex);
                            int columnCount = report.getColumns().size();
                            List<Map<String, String>> rows = new ArrayList<Map<String, String>>();
                            Map<String, String> map = null;
                            while (rs.next()) {
                                rowCount++;
                                map = new HashMap<String, String>();
                                for (int i = 1; i <= columnCount; i++) {
                                    String columnName = report.getColumns().get(i - 1).getName();
                                    map.put(columnName, rs.getString(columnName));
                                }
                                rows.add(map);
                            }
                            if(report.getIsPaging()){
                                if(info.getLimit() == 0){
                                    data.setRows(rows);
                                }else if(rows.size()>info.getStart() && rows.size()>info.getStart()+info.getLimit()){
                                    data.setRows(rows.subList(info.getStart(), info.getStart() + info.getLimit()));
                                }else if(rows.size()>info.getStart() && rows.size()<= info.getStart()+info.getLimit()){
                                    data.setRows(rows.subList(info.getStart(),rows.size()));
                                }
                            }else{
                                data.setRows(rows);
                            }
                            data.setTotal(rowCount);
                            return data;
                        }
                    }
                    call.execute();
                    return null;
                }
            });
            return cols;
        } catch (Exception e) {
            throw new ReportException(e.getMessage());
        } finally {
            executor.close();
        }
    }
}
