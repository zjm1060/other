package bitc.sz.wzEnv.report.model;

/**
 * Created by zsz on 2015/11/25.
 */
public class FormSetValueField {
    private String name;
    private String value;
    private String rowKeyValue;

    public FormSetValueField(String name, String value, String rowKeyValue) {
        this.name = name;
        this.value = value;
        this.rowKeyValue = rowKeyValue;
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
}
