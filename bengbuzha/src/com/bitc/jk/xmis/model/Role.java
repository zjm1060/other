package com.bitc.jk.xmis.model;

/**
 * 
 * 系统角色
 * 
 * @author BomWu
 * 
 */
public class Role extends TableBean {

	private static final long serialVersionUID = -6638486814840719094L;
	private Integer id;
	private String name;
	private String description;

	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
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

}
