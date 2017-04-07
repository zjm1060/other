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
import java.io.Serializable;
import java.util.Map;

/**
 * @author Bom Wu
 */
@Entity
@Table(name = "rep_column")
public class Column implements Serializable {
    private static final long serialVersionUID = 1685175766290725981L;
    @Id
    @GenericGenerator(name = "idGenerator", strategy = "org.hibernate.id.UUIDGenerator")
    @GeneratedValue(generator = "idGenerator")
    private String id;
    @javax.persistence.Column
    private String name;
    @javax.persistence.Column
    private String dname;
    @javax.persistence.Column
    private String type = "auto";
    @javax.persistence.Column
    private boolean hidden = false;
    @javax.persistence.Column
    private String reportId;
    @javax.persistence.Column
    private int sort;
    @javax.persistence.Column(length = 4000)
    @JsonIgnore
    private String config;

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

    public String getDname() {
        return dname;
    }

    public void setDname(String dname) {
        this.dname = dname;
    }

    public boolean isHidden() {
        return hidden;
    }

    public void setHidden(boolean hidden) {
        this.hidden = hidden;
    }

    public String getReportId() {
        return reportId;
    }

    public void setReportId(String reportId) {
        this.reportId = reportId;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public int getSort() {
        return sort;
    }

    public void setSort(int sort) {
        this.sort = sort;
    }

    public String getConfig() {
        return config;
    }

    public void setConfig(String config) {
        this.config = config;
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
