package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.report.db.SQLExecutor;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by BomWu on 2015/11/24 0024.
 */
public class UpdateCommand implements Command {
    private String tableName;
    private String rowKey;
    private String rowKeyValue;
    private List<String> columns;
    private List<Object> values;
    private SQLExecutor executor;

    public UpdateCommand(SQLExecutor executor, String tableName, String rowKey, String rowKeyValue) {
        this.executor = executor;
        this.rowKey = rowKey;
        this.rowKeyValue = rowKeyValue;
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
        this.values.add(this.rowKeyValue);
        this.executor.execute(sql, this.values.toArray());
    }

    private String buildSQL() {
        StringBuilder sql = new StringBuilder();
        sql.append("update ").append(getTableName())
                .append(" set ");

        for (String column : columns) {
            sql.append(column).append("=?,");
        }
        sql.deleteCharAt(sql.length() - 1);
        sql.append(" where ").append(rowKey).append("=?");
        return sql.toString();
    }

    public String getRowKey() {
        return rowKey;
    }

    public void setRowKey(String rowKey) {
        this.rowKey = rowKey;
    }

    public String getRowKeyValue() {
        return rowKeyValue;
    }

    public void setRowKeyValue(String rowKeyValue) {
        this.rowKeyValue = rowKeyValue;
    }

    public String getTableName() {
        return tableName;
    }

    public void setTableName(String tableName) {
        this.tableName = tableName;
    }
}
