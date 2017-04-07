package com.bitc.exchange.cfg;

import com.bitc.exchange.model.ExistsStrategy;
import com.bitc.exchange.model.Table;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.OutputStream;
import java.io.Serializable;
import java.sql.Connection;
import java.util.List;

/**
 * Created by BomWu on 2015/5/18 0018.
 */
public interface Configuration {
    public Connection getConnection() throws ConfigurationException;

    public List<Table> getTables();

    public OutputStream getOutputStream() throws IOException;
    public final static String BlobsDirectory = "BlobsDirectory";

    public String getTempDirectoryPath();

    void closeConnection();

    public ExistsStrategy getExistsStrategy();
}
