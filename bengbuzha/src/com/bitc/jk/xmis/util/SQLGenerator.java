package com.bitc.jk.xmis.util;

import java.util.ArrayList;
import java.util.List;

import org.apache.commons.lang.StringUtils;

/**
 * 生成SQL语句的工具类
 * 
 * @author Bom Wu
 */
public class SQLGenerator {
	/**
	 * 源SQL其结果集必须最终存入#result临时表
	 */
	private String original;

	private List<String> appendSQL = new ArrayList<String>();

	public SQLGenerator(String original) {
		this.original = original;
	}

	public void addAppendSQL(String sql) {
		this.appendSQL.add(sql);
	}

	public String getSQL() {
		String sql = original;
		for (int i = 0; i < appendSQL.size(); i++) {
			sql = generateSQL(sql, appendSQL.get(i), i);
		}
		return sql;
	}

	public static final String prex = "\n select #prev.* into #result from #prev \n";

	public static String generateSQL(String origSQL, String appendSQL, int count) {
		if (StringUtils.isEmpty(appendSQL)) {
			return origSQL;
		}
		if (count > 0) {
			origSQL = StringUtils.replace(origSQL, "#prev", "#prev" + count);
		}
		origSQL = StringUtils.replace(origSQL, "#result", "#prev");

		StringBuilder builder = new StringBuilder(origSQL);
		builder.append(prex).append(dealWhere(appendSQL));
		return builder.toString();
	}

	private static String dealWhere(String sql) {
		if (!StringUtils.startsWith(StringUtils.trim(sql), "where ")) {
			if (StringUtils.startsWith(StringUtils.trim(sql), "full")
					|| StringUtils.startsWith(StringUtils.trim(sql), "left")
					|| StringUtils.startsWith(StringUtils.trim(sql), "right")
					|| StringUtils.startsWith(StringUtils.trim(sql), "inner")
					|| StringUtils.startsWith(StringUtils.trim(sql), "join")) {
				return sql;
			}
			return " where " + sql;
		}
		return sql;
	}

	@Override
	public String toString() {
		return getSQL();
	}

}
