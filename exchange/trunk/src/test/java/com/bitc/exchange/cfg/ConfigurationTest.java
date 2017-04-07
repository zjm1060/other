package com.bitc.exchange.cfg;

import org.junit.Assert;
import org.junit.Test;

/**
 * Created by BomWu on 2015/5/18 0018.
 */
public class ConfigurationTest {
    @Test
    public void testDefaultXmlConfiguration() throws ConfigurationException {
        Configuration configuration = new DefaultXmlConfiguration();
        Assert.assertNotNull(configuration);
        Assert.assertTrue(configuration.getTables().size() > 0);
        System.out.println(configuration.getTables().get(0).getSelectSql());
    }
}
