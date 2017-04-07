package com.bitc.exchange;

import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.cfg.ConfigurationAware;
import com.bitc.exchange.model.Table;
import org.apache.commons.io.FileUtils;

import java.io.File;
import java.sql.Connection;
import java.util.List;

/**
 * Created by BomWu on 2015/5/18 0018.
 */
public class Importer implements ConfigurationAware {

    private ExchangeFileParser parser;
    private Configuration configuration;

    public Importer(Configuration configuration, File file) {
        this.configuration = configuration;
        this.parser = new ExchangeFileParser(configuration, file);
    }

    public void start() throws Exception {
        Log.LOGGER.info("start import");
        File dir = parser.unZip();
        Connection connection = configuration.getConnection();
        connection.setAutoCommit(false);
        try {
            for (File f : dir.listFiles()) {
                if (!f.isDirectory()) {
                    new TableImport(configuration, f, dir).inport();
                }
            }
            connection.commit();
        } catch (Exception e) {
            e.printStackTrace();
            connection.rollback();
            throw e;
        } finally {
            configuration.closeConnection();
        }
        FileUtils.deleteDirectory(dir);
    }

    public List<Table> listTables() throws Exception {
        return parser.listTables();
    }

    @Override
    public Configuration getConfiguration() {
        return configuration;
    }

    @Override
    public void setConfiguration(Configuration configuration) {
        this.configuration = configuration;
    }
}
