package com.bitc.jk.xmis.model;

import com.bitc.jk.xmis.model.sys.RelationalOperator;

public class ConditionMap {
	private int id;
	private ConditionItem conditionItem;
	private ConditionType conditionType;
	private RelationalOperator operator;
	private String typeConfig;
	private String sql;

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public ConditionItem getConditionItem() {
		return conditionItem;
	}

	public void setConditionItem(ConditionItem conditionItem) {
		this.conditionItem = conditionItem;
	}

	public ConditionType getConditionType() {
		return conditionType;
	}

	public void setConditionType(ConditionType conditionType) {
		this.conditionType = conditionType;
	}

	public String getTypeConfig() {
		return typeConfig;
	}

	public void setTypeConfig(String typeConfig) {
		this.typeConfig = typeConfig;
	}

	public String getSql() {
		return sql;
	}

	public void setSql(String sql) {
		this.sql = sql;
	}

	public RelationalOperator getOperator() {
		return operator;
	}

	public void setOperator(RelationalOperator operator) {
		this.operator = operator;
	}

}
