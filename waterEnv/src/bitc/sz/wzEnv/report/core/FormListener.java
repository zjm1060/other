package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.report.db.SQLExecutor;
import bitc.sz.wzEnv.report.model.Form;
import bitc.sz.wzEnv.report.model.FormField;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by BomWu on 2015/11/23 0023.
 */
public interface FormListener {
    boolean beforeSave(Form form, Map<String, List<FormField>> fieldsMap, Map<String, String> generateKeys);

    void afterSave(Form form, SQLExecutor executor, List<FormField> allFields, Map<String, List<FormField>> fieldsMap, Map<String, String> generateKeys) throws SQLException;

    void afterCommit();
}
