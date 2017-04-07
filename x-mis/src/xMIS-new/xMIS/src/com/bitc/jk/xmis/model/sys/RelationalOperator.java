package com.bitc.jk.xmis.model.sys;

public class RelationalOperator {

	/**
	 * 等于 =
	 */
	public static final String EQ = "=";
	/**
	 * 大于 >
	 */
	public static final String GT = ">";
	/**
	 * 小于 <
	 */
	public static final String LT = "<";
	/**
	 * 大于等于 >=
	 */
	public static final String GE = ">=";
	/**
	 * 小于等于 <=
	 */
	public static final String LE = "<=";
	/**
	 * 不等于 <>
	 */
	public static final String NE = "<>";
	/**
	 * IS NULL
	 */
	public static final String ISNULL = "is null";
	/**
	 * IS NOT NULL
	 */
	public static final String ISNOTNULL = "is not null";
	/**
	 * LIKE like
	 */
	public static final String LIKE = "like";

	public static final String SQL = "sql";

	public static final String JDBCSQL = "jdbcsql";

	private int id;
	private String operator = EQ;

	public RelationalOperator() {
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getOperator() {
		return operator;
	}

	public void setOperator(String operator) {
		this.operator = operator;
	}
}
