package com.riversql.task.impl;
import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.cfg.ConfigurationException;
import com.bitc.exchange.model.ExistsStrategy;
import com.bitc.exchange.model.Table;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

import java.io.*;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * Created by BomWu on 2015/5/18 0018.
 */
public class XmlConfigurationAdapter implements Configuration {


    private Element root;
    private String driver;
    private String url;
    private String user;
    private String password;
    private List<Table> tables;
    private String outputFilePath;
    private Lock lock = new ReentrantLock();
    private Connection connection;
    private ExistsStrategy strategy;

    public XmlConfigurationAdapter() throws ConfigurationException {
        this(null);
    }

    public XmlConfigurationAdapter(byte[] xmlConfigInputStream) throws ConfigurationException {
        SAXReader reader = new SAXReader();
        try {
            if (xmlConfigInputStream == null) {
               return;
            }
            Document document = reader.read(new ByteArrayInputStream(xmlConfigInputStream));
            root = document.getRootElement();
            Element dataSource = root.element("datasource");
            url = dataSource.elementText("url");
            driver = dataSource.elementText("driver");
            user = dataSource.elementText("user");
            password = dataSource.elementText("password");

            outputFilePath = root.elementText("output").trim();
            this.strategy = ExistsStrategy.valueOf(root.elementText("exists").toUpperCase());
            File file = new File(outputFilePath);
            if (!file.exists()) {
                file.createNewFile();
            }

            tables = new ArrayList<Table>();
            Element tablesElement = root.element("tables");
            for (Iterator<Element> it = tablesElement.elements("table").iterator(); it.hasNext(); ) {
                Element tableElement = it.next();
                Table table = new Table(tableElement.attributeValue("name"));
                table.setPk(tableElement.element("pk").attributeValue("name"));
                for (Iterator<Element> itt = tableElement.elements("column").iterator(); itt.hasNext(); ) {
                    Element columnElement = itt.next();
                    table.addColumn(columnElement.attributeValue("name"), columnElement.attributeValue("type"));
                }
                table.setWhere(tableElement.elementText("where"));
                tables.add(table);
            }
        } catch (DocumentException e) {
            throw new ConfigurationException("parse xml exception", e);
        } catch (IOException e) {
            throw new ConfigurationException("create output file exception", e);
        }
    }


    @Override
    public Connection getConnection() throws ConfigurationException {
        lock.lock();
        if (this.connection == null) {
            try {
                Class.forName(driver);
                this.connection = DriverManager.getConnection(url, user, password);
            } catch (ClassNotFoundException e) {
                throw new ConfigurationException(e);
            } catch (SQLException e) {
                throw new ConfigurationException(e);
            }
        }
        lock.unlock();
        return this.connection;
    }

    @Override
    public List<Table> getTables() {
        return tables;
    }

    @Override
    public OutputStream getOutputStream() {
        try {
            return new FileOutputStream(outputFilePath);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return null;
        }
    }

    @Override
    public String getTempDirectoryPath() {
        return System.getProperty("java.io.tmpdir");
    }

    @Override
    public void closeConnection() {
        try {
            this.connection.close();
        } catch (Exception e) {
        }
    }

    @Override
    public ExistsStrategy getExistsStrategy() {
        return this.strategy;
    }
}
