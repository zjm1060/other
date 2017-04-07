package bitc.sz.wzEnv.report.model;

import bitc.sz.wzEnv.util.JsonUtil;
import org.apache.log4j.Logger;
import org.codehaus.jackson.type.TypeReference;
import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;
import javax.persistence.Column;
import java.util.List;
import java.util.Map;

/**
 * Created by BomWu on 2015/11/16 0016.
 */
@Entity
@Table(name = "rep_form")
public class Form {
    @Id
    @GenericGenerator(name = "idGenerator", strategy = "org.hibernate.id.UUIDGenerator")
    @GeneratedValue(generator = "idGenerator")
    private String id;
    @Column(unique = true)
    private String name;
    @Column
    private String title;
    @Column
    private String dbName;
    @Column(length = 4000)
    private String config;

    @OneToMany(cascade = CascadeType.ALL)
    @JoinColumn(name = "formId")
    @OrderBy(value = "sort asc")
    private List<Field> fields;

    @Column(length = 1000)
    private String listener;


    public String getConfig() {
        return config;
    }

    public void setConfig(String config) {
        this.config = config;
    }

    public List<Field> getFields() {
        return fields;
    }

    public void setFields(List<Field> fields) {
        this.fields = fields;
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

    public String getDbName() {
        return dbName;
    }

    public void setDbName(String dbName) {
        this.dbName = dbName;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getListener() {
        return listener;
    }

    public void setListener(String listener) {
        this.listener = listener;
    }
}
