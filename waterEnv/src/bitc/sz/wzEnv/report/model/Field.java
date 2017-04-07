package bitc.sz.wzEnv.report.model;

import bitc.sz.wzEnv.util.JsonUtil;
import com.fasterxml.jackson.annotation.JsonIgnore;
import org.apache.log4j.Logger;
import org.codehaus.jackson.type.TypeReference;
import org.hibernate.annotations.GenericGenerator;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.Id;
import javax.persistence.Table;
import java.util.Map;

/**
 * Created by BomWu on 2015/11/16 0016.
 */
@Entity
@Table(name = "rep_field")
public class Field {
    @Id
    @GenericGenerator(name = "idGenerator", strategy = "org.hibernate.id.UUIDGenerator")
    @GeneratedValue(generator = "idGenerator")
    private String id;
    @javax.persistence.Column
    private String name;
    @javax.persistence.Column
    private String tableName;
    @javax.persistence.Column
    private String rowKey;
    @javax.persistence.Column
    private String xtype = "textfield";
    @javax.persistence.Column
    private String fieldLabel;
    @javax.persistence.Column
    private int sort;
    @javax.persistence.Column
    private String formId;
    @javax.persistence.Column(length = 4000)
    @JsonIgnore
    private String config;
    @javax.persistence.Column(length = 50)
    private String sqlType;
    @javax.persistence.Column(length = 500)
    private String insertValueGenerator;
    @javax.persistence.Column(length = 500)
    private String updateValueGenerator;

    public String getFieldLabel() {
        return fieldLabel;
    }

    public void setFieldLabel(String fieldLabel) {
        this.fieldLabel = fieldLabel;
    }

    public String getFormId() {
        return formId;
    }

    public void setFormId(String formId) {
        this.formId = formId;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getRowKey() {
        return rowKey;
    }

    public void setRowKey(String rowKey) {
        this.rowKey = rowKey;
    }

    public int getSort() {
        return sort;
    }

    public void setSort(int sort) {
        this.sort = sort;
    }

    public String getTableName() {
        return tableName;
    }

    public void setTableName(String tableName) {
        this.tableName = tableName;
    }

    public String getXtype() {
        return xtype;
    }

    public void setXtype(String xtype) {
        this.xtype = xtype;
    }

    public String getConfig() {
        return config;
    }

    public void setConfig(String config) {
        this.config = config;
    }

    public String getSqlType() {
        return sqlType;
    }

    public void setSqlType(String sqlType) {
        this.sqlType = sqlType;
    }


    public String getInsertValueGenerator() {
        return insertValueGenerator;
    }

    public void setInsertValueGenerator(String insertValueGenerator) {
        this.insertValueGenerator = insertValueGenerator;
    }

    public String getUpdateValueGenerator() {
        return updateValueGenerator;
    }

    public void setUpdateValueGenerator(String updateValueGenerator) {
        this.updateValueGenerator = updateValueGenerator;
    }

    public Map<String, Object> getExtraConfig() {
        if (config != null) {
            try {
                return JsonUtil.readValue(config, new TypeReference<Map<String, Object>>() {
                });
            } catch (Exception e) {
                Logger.getLogger(this.getClass()).error("解析Config出错", e);
                return null;
            }
        }
        return null;
    }
}
