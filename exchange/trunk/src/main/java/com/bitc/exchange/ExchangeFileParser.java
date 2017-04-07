package com.bitc.exchange;

import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.cfg.ConfigurationAware;
import com.bitc.exchange.model.Table;
import com.bitc.exchange.util.UUIDUtils;
import com.bitc.exchange.util.Zip;
import org.apache.commons.io.FileUtils;
import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by BomWu on 2015/6/11 0011.
 */
public class ExchangeFileParser implements ConfigurationAware {
    private File file;
    private Configuration configuration;

    public ExchangeFileParser(Configuration configuration, File file) {
        this.configuration = configuration;
        this.file = file;
    }

    public List<Table> listTables() throws Exception {
        List<Table> tables = new ArrayList<Table>();
        File dir = unZip();
        for (File file : dir.listFiles()) {
            if (!file.isDirectory()) {
                ObjectInputStream ois = new ObjectInputStream(new FileInputStream(file));
                Table table = (Table) ois.readObject();
                tables.add(table);
                IOUtils.closeQuietly(ois);
            }
        }
        FileUtils.deleteDirectory(dir);
        return tables;
    }

    @Override
    public Configuration getConfiguration() {
        return this.configuration;
    }

    @Override
    public void setConfiguration(Configuration config) {
        this.configuration = config;
    }

    public File unZip() throws IOException {
        String directory = configuration.getTempDirectoryPath() + File.separator + UUIDUtils.nextCode();
        Zip.unZip(file, directory);
        Log.LOGGER.info("unzip file to[" + directory + "]");
        return new File(directory);
    }
}
