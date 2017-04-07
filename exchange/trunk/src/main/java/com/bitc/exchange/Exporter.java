package com.bitc.exchange;

import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.cfg.ConfigurationAware;
import com.bitc.exchange.cfg.ConfigurationException;
import com.bitc.exchange.model.Table;
import com.bitc.exchange.util.UUIDUtils;
import com.bitc.exchange.util.Zip;
import org.apache.commons.dbutils.QueryRunner;
import org.apache.commons.io.FileUtils;

import java.io.File;
import java.io.OutputStream;
import java.sql.Connection;
import java.util.List;

/**
 * Created by BomWu on 2015/5/18 0018.
 */
public class Exporter implements ConfigurationAware {
    private Configuration configuration;
    private List<Table> tables;
    private QueryRunner queryRunner = new QueryRunner();
    private Connection connection;

    public Exporter(Configuration configuration) throws Exception {
        this.configuration = configuration;
        this.tables = configuration.getTables();
        try {
            this.connection = configuration.getConnection();
        } catch (ConfigurationException e) {
            throw new Exception(e);
        }
        Log.LOGGER.info("Init Exporter");
    }


    public void start() throws Exception {
        Log.LOGGER.info("start export.");
        File tmpDirectory = new File(configuration.getTempDirectoryPath() + File.separator + UUIDUtils.nextCode());
        tmpDirectory.mkdir();
        Log.LOGGER.info("make temp dir:" + tmpDirectory.getPath());
//        File blobs = new File(tmpDirectory.getPath() + File.separator + Configuration.BlobsDirectory);
//        blobs.mkdir();
//        Log.LOGGER.info("make temp blob files dir:" + blobs.getPath());
        for (Table table : tables) {
            Log.LOGGER.info("start export table:" + table.getName());
            Log.LOGGER.info("start export table  Blobs------------------------>size:" + table.getBlobs().size());
            new TableExport(configuration, table, tmpDirectory).export();
            Log.LOGGER.info("export table " + table.getName() + " success!");
        }
        Log.LOGGER.info("start zip file.");
        OutputStream outputStream = configuration.getOutputStream();
        Zip.zip(tmpDirectory.listFiles(), outputStream);
        Log.LOGGER.info("zip file success!");
        FileUtils.deleteDirectory(tmpDirectory);
        Log.LOGGER.info("delete temp file.");
        configuration.closeConnection();
    }

    @Override
    public Configuration getConfiguration() {
        return this.configuration;
    }

    @Override
    public void setConfiguration(Configuration config) {
        this.configuration = config;
    }
}
