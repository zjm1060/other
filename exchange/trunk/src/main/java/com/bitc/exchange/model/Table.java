package com.bitc.exchange.model;

import com.bitc.exchange.util.JsonUtil;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by BomWu on 2015/5/18 0018.
 */
public class Table implements Serializable {
    private static final long serialVersionUID = 1L;
    private String name;
    private String pk;
    private List<String> columns;
    private List<String> blobs;
    private Integer rowCount;
    private String where;

    public Table(String name) {
        this.name = name;
        this.columns = new ArrayList<String>();
        this.blobs = new ArrayList<String>();
    }


    public void addColumn(String column, String type) {
        if (type == null) {
            this.columns.add(column);
        } else if ("blob".equalsIgnoreCase(type)) {
            this.blobs.add(column);
        }
    }

    public int getColumnSize() {
        return this.columns.size() + 1;
    }

    public String getSelectSql() {
        StringBuilder sb = new StringBuilder();
        sb.append("select ").append(this.pk).append(",");
        for (int i = 0; i < this.columns.size(); i++) {
            sb.append(this.columns.get(i));
            sb.append(",");
        }
        sb.deleteCharAt(sb.length() - 1);
        sb.append(" from ").append(this.name);
        if (this.where != null && !"".equals(this.where)) {
            sb.append(" where ").append(this.where);
        }
        return sb.toString();
    }

    public String getCountSql() {
        StringBuilder sb = new StringBuilder();
        sb.append("select count(*) ");
        sb.append(" from ").append(this.name);
        if (this.where != null && !"".equals(this.where)) {
            sb.append(" where ").append(this.where);
        }
        return sb.toString();
    }


    public String getInsertSql() {
        StringBuilder sb = new StringBuilder();
        sb.append("insert into ").append(this.getName()).append("(");
        sb.append(this.pk).append(",");
        for (int i = 0; i < this.columns.size(); i++) {
            sb.append(this.columns.get(i)).append(",");
        }
        sb.deleteCharAt(sb.length() - 1);
        sb.append(")");
        sb.append(" values(");
        for (int i = 0; i < this.getColumnSize(); i++) {
            sb.append("?,");
        }
        sb.deleteCharAt(sb.length() - 1);
        sb.append(")");
        return sb.toString();
    }

    public String getDeleteSql() {
        StringBuilder sb = new StringBuilder();
        sb.append("delete from ").append(this.name).append(" where ").append(this.pk).append("=?");
        return sb.toString();
    }

    public String getExistsCheckSql() {
        StringBuilder sb = new StringBuilder();
        sb.append("select count(*) from ").append(this.name).append(" where ").append(this.pk).append("=?");
        return sb.toString();
    }

    public List<String> getColumns() {
        return columns;
    }

    public void setColumns(List<String> columns) {
        this.columns = columns;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getPk() {
        return pk;
    }

    public void setPk(String pk) {
        this.pk = pk;
    }

    public String getWhere() {
        return where;
    }

    public void setWhere(String where) {
        this.where = where;
    }

    public Integer getRowCount() {
        return rowCount;
    }

    public void setRowCount(Integer rowCount) {
        this.rowCount = rowCount;
    }

    public List<String> getBlobs() {
        return blobs;
    }

    public void setBlobs(List<String> blobs) {
        this.blobs = blobs;
    }

    @Override
    public String toString() {
        return JsonUtil.getJsonString(this);
    }


}
