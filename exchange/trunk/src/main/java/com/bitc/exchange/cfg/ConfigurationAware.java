package com.bitc.exchange.cfg;

/**
 * Created by BomWu on 2015/6/11 0011.
 */
public interface ConfigurationAware {
    public Configuration getConfiguration();

    public void setConfiguration(Configuration config);
}
