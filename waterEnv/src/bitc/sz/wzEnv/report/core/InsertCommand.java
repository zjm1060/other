package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.report.db.SQLExecutor;
import bitc.sz.wzEnv.report.model.Column;
import bitc.sz.wzEnv.report.model.FormField;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by BomWu on 2015/11/24 0024.
 */
public class InsertCommand implements Command {
    private SQLExecutor executor;
    private String tableName;
    private List<String> columns;
    private List<Object> values;

    public InsertCommand(SQLExecutor executor, String tableName) {
        this.executor = executor;
        this.tableName = tableName;
        columns = new ArrayList<String>();
        values = new ArrayList<Object>();
    }


    public void addColumn(String column, Object value) {
        columns.add(column);
        values.add(value);
    }

    @Override
    public void execute() throws Exception {
        String sql = this.buildSQL();
        this.executor.execute(sql, this.values.toArray());
    }

    private String buildSQL() {
        StringBuilder sql = new StringBuilder();
        sql.append("insert into ").append(getTableName())
                .append("(");
        StringBuilder sql2 = new StringBuilder("values(");
        for (String column : columns) {
            sql.append(column).append(",");
            sql2.append("?,");
        }
        sql.deleteCharAt(sql.length() - 1);
        sql2.deleteCharAt(sql2.length() - 1);
        sql.append(") ").append(sql2).append(")");
        return sql.toString();
    }

    public String getTableName() {
        return tableName;
    }

    public void setTableName(String tableName) {
        this.tableName = tableName;
    }
}
