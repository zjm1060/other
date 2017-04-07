package bitc.sz.wzEnv.report.model;

import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.report.core.ReportException;
import bitc.sz.wzEnv.report.db.SQLExecutor;
import bitc.sz.wzEnv.report.util.SQLReportUtil;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import org.hibernate.annotations.GenericGenerator;

import javax.persistence.*;
import java.io.Serializable;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Entity
@Table(name = "rep_Report")
@JsonIgnoreProperties(value = {"hibernateLazyInitializer", "handler", "fieldHandler"})
public class Report implements Serializable {
    private static final long serialVersionUID = -4864109563428911073L;
    private static final Logger logger = Logger.getLogger(Report.class);
    @Id
    @GenericGenerator(name = "idGenerator", strategy = "org.hibernate.id.UUIDGenerator")
    @GeneratedValue(generator = "idGenerator")
    private String id;
    @javax.persistence.Column(unique = true)
    private String name;
    @javax.persistence.Column(length = 4000)
    private String sql;
    @javax.persistence.Column
    private SQLType sqlType;
    @javax.persistence.Column
    private String dbName;
    @javax.persistence.Column
    private String title;

    @OneToMany(cascade = CascadeType.ALL)
    @JoinColumn(name = "reportId")
    @OrderBy(value = "sort asc")
    private List<Column> columns;

    @OneToMany(cascade = CascadeType.ALL)
    @JoinColumn(name = "reportId")
    @OrderBy(value = "sort asc")
    private List<Parameter> parameters = new ArrayList<Parameter>();

    @javax.persistence.Column
    private String plugins;

    @javax.persistence.Column
    private String dbType;
    @javax.persistence.Column
    private boolean isPaging = false;

    public String getId() {
        return id;
    }

    public void setId(String id) {
        if (StringUtils.isEmpty(id)) {
            this.id = null;
        } else {
            this.id = id;
        }
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSql() {
        return sql;
    }

    public void setSql(String sql) {
        this.sql = sql;
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

    public List<Column> getColumns() {
        return columns;
    }

    public void setColumns(List<Column> columns) {
        this.columns = columns;
    }

    public GridData selectGridData(PagingInfo info, String reportCondition) throws ReportException {
        GridData data = new GridData();
        SQLExecutor executor = null;
        ResultSet rs = null;
        try {
            executor = new SQLExecutor(getDbName());
            ResultSet rs1 = executor.executeQuery(SQLReportUtil.getTotalCount(getSql(), reportCondition));
            if (rs1.next()) {
                data.setTotal(rs1.getInt("totalcount"));
            }
            String sql = SQLReportUtil.getSQL(getIsPaging(), getDbType(), getSql(), info, reportCondition);
            if (logger.isDebugEnabled()) {
                logger.debug("SQL:" + sql);
            }
            rs = executor.executeQuery(sql);
            int columnCount = getColumns().size();
            List<Map<String, String>> rows = new ArrayList<Map<String, String>>();
            Map<String, String> map;
            while (rs.next()) {
                map = new HashMap<String, String>();
                for (int i = 1; i <= columnCount; i++) {
                    String columnName = getColumns().get(i - 1).getName();
                    map.put(columnName, rs.getString(columnName));
                }
                rows.add(map);
            }
            data.setRows(rows);
            return data;
        } catch (Exception e) {
            throw new ReportException(e.getMessage());
        } finally {
            executor.close();
        }
    }

    public String getPlugins() {
        return plugins;
    }

    public void setPlugins(String plugins) {
        this.plugins = plugins;
    }

    public String getDbType() {
        return dbType;
    }

    public void setDbType(String dbType) {
        this.dbType = dbType;
    }

    public boolean getIsPaging() {
        return isPaging;
    }

    public void setIsPaging(boolean isPaging) {
        this.isPaging = isPaging;
    }

    public SQLType getSqlType() {
        return sqlType;
    }

    public void setSqlType(SQLType sqlType) {
        this.sqlType = sqlType;
    }

    public List<Parameter> getParameters() {
        return parameters;
    }

    public void setParameters(List<Parameter> parameters) {
        this.parameters = parameters;
    }
}
