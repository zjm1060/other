package bitc.sz.wzEnv.report.model;

import bitc.sz.wzEnv.report.sql.Types;
import com.fasterxml.jackson.annotation.JsonIgnore;

/**
 * Created by zsz on 2015/11/18.
 */
public class FormField {
    private String tableName;
    private String name;
    private String value;
    private String rowKey;
    private String rowKeyValue;
    private String valueType;
    @JsonIgnore
    private Field field;

    public FormField() {
    }

    public FormField(String tableName, String name, String value, String rowKey, String rowKeyValue, String valueType) {
        this.tableName = tableName;
        this.name = name;
        this.value = value;
        this.rowKey = rowKey;
        this.rowKeyValue = rowKeyValue;
        this.valueType = valueType;
    }

    public FormField(String tableName, String name) {
        this.tableName = tableName;
        this.name = name;
    }

    public String getTableName() {
        return tableName;
    }

    public void setTableName(String tableName) {
        this.tableName = tableName;
    }

    public String getRowKey() {
        return rowKey;
    }

    public void setRowKey(String rowKey) {
        this.rowKey = rowKey;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    public String getRowKeyValue() {
        return rowKeyValue;
    }

    public void setRowKeyValue(String rowKeyValue) {
        this.rowKeyValue = rowKeyValue;
    }

    public String getValueType() {
        return valueType;
    }

    public void setValueType(String valueType) {
        this.valueType = valueType;
    }

    public Object getObjectValue() throws Exception {
        return Types.getValue(this.valueType, this.value);
    }

    public Field getField() {
        return field;
    }

    public void setField(Field field) {
        this.field = field;
    }
}
