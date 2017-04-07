/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.riversql.actions;

import com.bitc.exchange.Exporter;
import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.cfg.ConfigurationException;
import com.bitc.exchange.model.ExistsStrategy;
import com.bitc.exchange.model.Table;
import com.bitc.exchange.util.UUIDUtils;
import com.riversql.IDManager;
import com.riversql.JSONAction;
import com.riversql.dbtree.SQLSession;
import com.riversql.sql.SQLConnection;
import org.json.JSONObject;

import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.sql.Connection;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

/**
 * @author river.liao
 * @author BomWu
 */
public class Export implements JSONAction, Configuration {
    private String id; //connection id
    private String tablename;
    private String columns;
    private String blobcolumns;
    private String pk;
    private String type;
    private String where;
    private Connection connection;
    private String tempFile;

    public JSONObject execute(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws Exception {
        SQLSession sqlsession = (SQLSession) IDManager.get().get(id);
        SQLConnection conn = sqlsession.getConn();
        this.connection = conn.getConnection();
        String fileId = UUIDUtils.nextCode();
        this.tempFile = this.getTempDirectoryPath() + File.separator + fileId;
        JSONObject ret = new JSONObject();
        Exporter exporter = new Exporter(this);
        exporter.start();
        ret.put("success", true);
        ret.put("fileId", fileId);
        ret.put("fileName", this.tablename + "_" + (new Date().getTime()) + ".exchange");
        return ret;
    }


    @Override
    public Connection getConnection() throws ConfigurationException {
        return this.connection;
    }

    @Override
    public List<Table> getTables() {
        List<Table> tables = new ArrayList<Table>();
        Table table = new Table(this.tablename);
        table.setWhere(this.where);
        table.setPk(this.pk);
        for (String col : columns.trim().split(",")) {
            if (col == null || "".equals(col)) {
                continue;
            }
            if (this.pk.equals(col)) {
                continue;
            }
            table.getColumns().add(col);
        }
        for (String col : blobcolumns.trim().split(",")) {
            if (col == null || "".equals(col)) {
                continue;
            }
            if (col.equals(this.pk)) {
                continue;
            }
            table.getBlobs().add(col);
        }

        tables.add(table);
        return tables;
    }

    @Override
    public OutputStream getOutputStream() throws IOException {
        return new FileOutputStream(this.tempFile);
    }

    @Override
    public String getTempDirectoryPath() {
        return System.getProperty("java.io.tmpdir");
    }

    @Override
    public void closeConnection() {
        //DO NULL
    }

    @Override
    public ExistsStrategy getExistsStrategy() {
        return null;
    }

    public String getColumns() {
        return columns;
    }

    public void setColumns(String columns) {
        this.columns = columns;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getPk() {
        return pk;
    }

    public void setPk(String pk) {
        this.pk = pk;
    }

    public String getTablename() {
        return tablename;
    }

    public void setTablename(String tablename) {
        this.tablename = tablename;
    }

    public String getType() {
        return type;
    }

    public void setType(String type) {
        this.type = type;
    }

    public String getBlobcolumns() {
        return blobcolumns;
    }

    public void setBlobcolumns(String blobcolumns) {
        this.blobcolumns = blobcolumns;
    }

    public String getWhere() {
        return where;
    }

    public void setWhere(String where) {
        this.where = where;
    }
}
