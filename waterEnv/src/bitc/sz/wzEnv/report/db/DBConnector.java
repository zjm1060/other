package bitc.sz.wzEnv.report.db;

import bitc.sz.wzEnv.report.core.ReportService;
import bitc.sz.wzEnv.report.core.ReportServiceImpl;
import bitc.sz.wzEnv.report.model.DBConnect;
import org.apache.log4j.Logger;
import org.logicalcobwebs.proxool.ProxoolException;
import org.logicalcobwebs.proxool.ProxoolFacade;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.ApplicationListener;
import org.springframework.context.event.ContextRefreshedEvent;
import org.springframework.stereotype.Service;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.List;
import java.util.Properties;

@Service
public class DBConnector implements ApplicationListener<ContextRefreshedEvent> {

    @Autowired
    private ReportService reportService;

    private static Logger logger = Logger.getLogger(DBConnector.class);

    public static Connection getConnection(String connectName)
            throws SQLException {
        Connection con = DriverManager.getConnection("proxool." + connectName);
        if (logger.isInfoEnabled()) {
            logger.info("getConnection-->" + connectName);
        }
        return con;
    }

    public void init() {
        try {
            Class.forName("org.logicalcobwebs.proxool.ProxoolDriver");
            List<DBConnect> list = reportService.getDbConnects();
            for (DBConnect connect : list) {
                registerConnectionPool(connect);
            }
            if (logger.isInfoEnabled()) {
                logger.info("DBConnector-->init compeleted!");
            }
        } catch (ClassNotFoundException e) {
            logger.error(e.getMessage());
        }
    }

    public void registerConnectionPool(DBConnect connect) {
        Properties info = new Properties();
        info.setProperty("proxool.maximum-connection-count", "10");
        info.setProperty("user", connect.getUsername());
        info.setProperty("password", connect.getPassword());
        String url = "proxool." + connect.getName() + ":" + connect.getDriver()
                + ":" + connect.getUrl();
        try {
            ProxoolFacade.registerConnectionPool(url, info);
        } catch (ProxoolException e) {
            logger.error(e.getMessage());
        }
    }

    @Override
    public void onApplicationEvent(ContextRefreshedEvent contextRefreshedEvent) {
        if (contextRefreshedEvent.getApplicationContext().getParent() == null) {
            init();
        }
    }
}
