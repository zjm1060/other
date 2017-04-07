package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.report.db.Executable;
import bitc.sz.wzEnv.report.db.SQLExecutor;
import bitc.sz.wzEnv.report.model.*;
import bitc.sz.wzEnv.report.sql.ResultSetAdapter;
import bitc.sz.wzEnv.report.sql.Types;
import bitc.sz.wzEnv.report.util.ReportSqlParser;
import bitc.sz.wzEnv.report.util.SQLReportUtil;
import bitc.sz.wzEnv.util.JsonUtil;
import net.sf.jsqlparser.JSQLParserException;
import org.apache.commons.lang.StringUtils;
import org.springframework.stereotype.Component;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.apache.log4j.Logger;
/**
 * Created by BomWu on 2016/3/14 0014.
 */
@Component
public class NormalReportExecutor implements ReportExecutor {
    private static final Logger logger = Logger.getLogger(NormalReportExecutor.class);
//    @Override
//    public List<Parameter> initParameter(Report report, String defaultValues) throws ReportException, JSQLParserException {
//        List<Parameter> parameters = null;
//        if(StringUtils.isNotEmpty(defaultValues)){
//            parameters = JsonUtil.readJson(defaultValues, ArrayList.class, Parameter.class);
//        }
//        String sql = report.getSql();
//        List<Parameter> list = new ArrayList<Parameter>();
//        ReportSqlParser.sqlParser(list,sql);
//        Parameter parameter = null;
//        for(int i=0;i<list.size();i++){
//            parameter = list.get(i);
//            parameter.setParameterType(ParameterType.IN);
//            parameter.setType(Types.STRING);
//            parameter.setReportId(report.getId());
//            parameter.setDefaultValue(parameters != null?parameters.get(i):null);
//        }
//        return parameters;
//    }

    @Override
    public List<Column> initColumns(final Report report) throws ReportException {

        try {
            SQLExecutor executor = new SQLExecutor(report.getDbName());
//            final String sql = SQLReportUtil.getNoDataSQL(report.getSql());
            List<Column> cols = executor.execute(new Executable<List<Column>>() {
                public List<Column> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(report.getSql());
                    List<Parameter> list = report.getParameters();
                    if(list!= null && list.size()>0){
                        for (int i = 1; i <= list.size(); i++) {
                            ps.setObject(i,list.get(i-1).getDefaultValue());
                        }
                    }
                    ResultSet rs = ps.executeQuery();
                    ResultSetAdapter rsa = new ResultSetAdapter(rs);
                    return rsa.getColumns();
                }
            });
            return cols;
        } catch (SQLException e) {
            throw new ReportException("init columns error!", e);
        }
    }

    @Override
    public GridData selectGridData(Report report, PagingInfo info, Object[] parameterValues, String condition) throws ReportException {
        GridData data = new GridData();
        SQLExecutor executor = null;
        ResultSet rs = null;
        try {
            executor = new SQLExecutor(report.getDbName());
            List<Parameter> parameters = null;
            ResultSet rs1 = executor.executeQuery(SQLReportUtil.getTotalCount(report.getSql(), condition),parameterValues);
            if (rs1.next()) {
                data.setTotal(rs1.getInt("totalcount"));
            }
            String sql = SQLReportUtil.getSQL(report.getIsPaging(), report.getDbType(), report.getSql(), info, condition);
            if (logger.isDebugEnabled()) {
                logger.debug("SQL:" + sql);
            }
            rs = executor.executeQuery(sql,parameterValues);
            int columnCount = report.getColumns().size();
            List<Map<String, String>> rows = new ArrayList<Map<String, String>>();
            Map<String, String> map;
            while (rs.next()) {
                map = new HashMap<String, String>();
                for (int i = 1; i <= columnCount; i++) {
                    String columnName = report.getColumns().get(i - 1).getName();
                    map.put(columnName, rs.getString(columnName));
                }
                rows.add(map);
            }
            data.setRows(rows);
            return data;
        } catch (Exception e) {
            throw new ReportException(e.getMessage());
        } finally {
            executor.close();
        }
    }

}
