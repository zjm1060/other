package com.bitc.jk.xmis.report.db;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.List;
import java.util.Properties;

import org.apache.log4j.Logger;
import org.logicalcobwebs.proxool.ProxoolException;
import org.logicalcobwebs.proxool.ProxoolFacade;

import com.bitc.jk.xmis.report.core.ReportService;
import com.bitc.jk.xmis.report.model.DBConnect;

public class DBConnector {

	private static DBConnector connector;
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

	public static void init(ReportService service) {
		connector = new DBConnector();
		connector.setReportService(service);
		try {
			Class.forName("org.logicalcobwebs.proxool.ProxoolDriver");
			List<DBConnect> list = service.getDbConnects();
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

	public static void registerConnectionPool(DBConnect connect) {
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

	public ReportService getReportService() {
		return reportService;
	}

	public void setReportService(ReportService reportService) {
		this.reportService = reportService;
	}

}
