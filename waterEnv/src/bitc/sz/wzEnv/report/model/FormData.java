package bitc.sz.wzEnv.report.model;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by BomWu on 2015/11/24 0024.
 */
public class FormData {

    private Map<String, FormField> fieldMap;

    public FormData() {
        fieldMap = new HashMap<String, FormField>();
    }

    public Map<String, FormField> getFieldMap() {
        return fieldMap;
    }

    public void setFormFields(List<FormField> formFields) {
        for (FormField formField : formFields) {
            fieldMap.put(formField.getTableName() + "-" + formField.getName(), formField);
        }
    }

    public FormField getFormField(String tableName, String name) {
        return fieldMap.get(tableName + "" + name);
    }

    public void setFieldMap(Map<String, FormField> formFields) {
        this.fieldMap = formFields;
    }
}
