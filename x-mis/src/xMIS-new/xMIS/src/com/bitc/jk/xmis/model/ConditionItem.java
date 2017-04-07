package com.bitc.jk.xmis.model;

/**
 * 条件
 * 
 * @author Bom Wu
 * 
 */
public class ConditionItem {

	private int id;
	/**
	 * 条件的名称，用于Field的 name属性
	 */
	private String name;
	/**
	 * 条件选项类型，可用的类型如下： string,integer,float,double,date
	 */
	private String type;

	/**
	 * 描述，用于显示的名称，用于Field的label
	 */
	private String description;

	/**
	 * 对于某些Field类型的额外的配置，如为datefield指定format等
	 */

	public ConditionItem() {
	}

	public ConditionItem(int id, String name, String description) {
		super();
		this.id = id;
		this.name = name;
		this.description = description;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}

}
