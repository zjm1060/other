/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package com.riversql.actions;

import com.bitc.exchange.Importer;
import com.bitc.exchange.Log;
import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.cfg.ConfigurationException;
import com.bitc.exchange.model.ExistsStrategy;
import com.bitc.exchange.model.Table;
import com.riversql.IDManager;
import com.riversql.JSONAction;
import com.riversql.dbtree.SQLSession;
import com.riversql.entities.ImportResult;
import com.riversql.sql.SQLConnection;
import org.json.JSONObject;

import javax.persistence.EntityManager;
import javax.persistence.EntityTransaction;
import javax.persistence.Query;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.namespace.QName;
import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.sql.Connection;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * @author river.liao
 */
public class ImportStep2 implements JSONAction, Configuration {
    private String id;
    private String tableName;
    private String fileName;
    private String strategy;
    private Connection connection;
    public JSONObject execute(HttpServletRequest request, HttpServletResponse response, EntityManager em, EntityTransaction et) throws Exception {
        SQLSession sqlsession = (SQLSession) IDManager.get().get(id);
        SQLConnection conn = sqlsession.getConn();
        this.connection = conn.getConnection();
        File file = new File(this.getTempDirectoryPath() + File.separator + fileName);
        Importer importer = new Importer(this, file);
        importer.start();
        file.delete();
        Query query =em .createQuery("select o from ImportResult o where o.fileName = ?1");
        query.setParameter(1, fileName);
        ImportResult importResult = (ImportResult)  query.getSingleResult();
        importResult.setResult(1);
        em.merge(importResult);
        JSONObject results = new JSONObject();
        return results;
    }

    public String getStrategy() {
        return strategy;
    }

    public void setStrategy(String strategy) {
        this.strategy = strategy;
    }

    @Override
    public Connection getConnection() throws ConfigurationException {
        return this.connection;
    }

    @Override
    public List<Table> getTables() {
        return null;
    }

    @Override
    public OutputStream getOutputStream() throws IOException {
        //Import Do Null
        return null;
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
        return ExistsStrategy.valueOf(this.strategy.toUpperCase());
    }

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
    }

    public String getTableName() {
        return tableName;
    }

    public void setTableName(String tableName) {
        this.tableName = tableName;
    }
}
