package com.bitc.jk.xmis.model;

import java.util.List;

/**
 * 条件
 * 
 * @author Bom Wu
 * 
 */
public class Condition {
	private int id;

	private String desc;

	private List<ConditionMap> items;

	public Condition() {
	}

	public void addItem(ConditionMap item) {
		items.add(item);
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getDesc() {
		return desc;
	}

	public void setDesc(String desc) {
		this.desc = desc;
	}

	public List<ConditionMap> getItems() {
		return items;
	}

	public void setItems(List<ConditionMap> items) {
		this.items = items;
	}

}
