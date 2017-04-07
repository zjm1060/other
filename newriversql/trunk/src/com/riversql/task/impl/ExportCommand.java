package com.riversql.task.impl;

import com.bitc.exchange.Exporter;
import com.bitc.exchange.cfg.Configuration;
import com.bitc.exchange.cfg.ConfigurationAware;
import com.bitc.exchange.cfg.DefaultXmlConfiguration;
import com.riversql.task.Command;

import javax.persistence.EntityManager;

/**
 * Created by zsz on 2015/8/18.
 */
public class ExportCommand implements Command {
    private byte[] config;
    @Override
    public void execute() {
        try {
            Configuration configuration = new XmlConfigurationAdapter(this.config);
            Exporter exportor = new Exporter(configuration);
            exportor.start();
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public byte[] getConfig() {
        return config;
    }

    @Override
    public void setConfig(byte[] config) {
        this.config = config;
    }

    @Override
    public void rollback() {

    }
}
