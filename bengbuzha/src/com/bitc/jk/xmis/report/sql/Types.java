package com.bitc.jk.xmis.report.sql;

import java.util.HashMap;
import java.util.Map;

public class Types {
	private static Map<Integer, String> typeMapping = new HashMap<Integer, String>();
	static {
		typeMapping.put(null, "auto");
		typeMapping.put(java.sql.Types.BIGINT, "int");
		typeMapping.put(java.sql.Types.BOOLEAN, "boolean");
		typeMapping.put(java.sql.Types.CHAR, "string");
		typeMapping.put(java.sql.Types.DATE, "date");
		typeMapping.put(java.sql.Types.DECIMAL, "float");
		typeMapping.put(java.sql.Types.DOUBLE, "float");
		typeMapping.put(java.sql.Types.FLOAT, "float");
		typeMapping.put(java.sql.Types.INTEGER, "int");
		typeMapping.put(java.sql.Types.LONGVARCHAR, "string");
		typeMapping.put(java.sql.Types.NUMERIC, "float");
		typeMapping.put(java.sql.Types.SMALLINT, "int");
		typeMapping.put(java.sql.Types.TIME, "date");
		typeMapping.put(java.sql.Types.TIMESTAMP, "date");
		typeMapping.put(java.sql.Types.TINYINT, "int");
		typeMapping.put(java.sql.Types.VARCHAR, "string");
	}

	public static String getExtjsType(Integer javaSqlType) {
		// return typeMapping.get(javaSqlType);
		return "auto";
	}
}
