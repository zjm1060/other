package com.bitc.jk.xmis.report.util;


public class SQLReportUtil {

	public static String getNoDataSQL(String sql) {
		return "select nodatasql.* from ( " + sql + " ) as nodatasql where 1=2";
	}
}
