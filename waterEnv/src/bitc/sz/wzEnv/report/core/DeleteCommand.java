package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.report.db.Executable;
import bitc.sz.wzEnv.report.db.SQLExecutor;
import bitc.sz.wzEnv.report.model.FormField;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.List;

/**
 * Created by BomWu on 2015/11/24 0024.
 */
public class DeleteCommand implements Command, Executable {
    private String dbName;
    private List<FormField> formFields;

    public String getDbName() {
        return dbName;
    }

    public void setDbName(String dbName) {
        this.dbName = dbName;
    }

    public List<FormField> getFormFields() {
        return formFields;
    }

    public void setFormFields(List<FormField> formFields) {
        this.formFields = formFields;
    }

    @Override
    public void execute() throws Exception {
        SQLExecutor executor = new SQLExecutor(this.dbName);
        executor.execute(this);
    }

    @Override
    public Object execute(Connection connection) throws SQLException {
        for (FormField field : this.getFormFields()) {
            StringBuilder sql = new StringBuilder();
            sql.append("delete from ").append(field.getTableName())
                    .append(" where ").append(field.getRowKey()).append("=?");
            PreparedStatement ps = connection.prepareStatement(sql.toString());
            ps.setString(1, field.getRowKeyValue());
            ps.execute();
            ps.close();
        }
        return null;
    }
}
