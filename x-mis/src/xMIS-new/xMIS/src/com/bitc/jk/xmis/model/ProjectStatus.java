package com.bitc.jk.xmis.model;

import java.io.Serializable;

public class ProjectStatus implements Serializable {

	private static final long serialVersionUID = -7394865232596484977L;

	public static final int NEW = 0;
	public static final int START = 191;

	private Integer id;
	private String name;
	private String description;
	private boolean custom;
	private String projectId;

	public ProjectStatus() {
	}

	public ProjectStatus(int id) {
		this.id = id;
	}

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

	public boolean isCustom() {
		return custom;
	}

	public void setCustom(boolean custom) {
		this.custom = custom;
	}

	public String getProjectId() {
		return projectId;
	}

	public void setProjectId(String projectId) {
		this.projectId = projectId;
	}

}
