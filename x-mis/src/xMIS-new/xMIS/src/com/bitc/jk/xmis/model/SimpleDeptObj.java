package com.bitc.jk.xmis.model;

import java.io.Serializable;

import com.bitc.jk.xmis.util.JSONUtils;

public class SimpleDeptObj implements Serializable {

	private static final long serialVersionUID = 2526050589769301763L;
	private int departmentID;
	private String departmentName;
	private Boolean disabled;

	public SimpleDeptObj() {
	}

	public SimpleDeptObj(int departmentID) {
		super();
		this.departmentID = departmentID;
	}

	public int getDepartmentID() {
		return departmentID;
	}

	public void setDepartmentID(int departmentID) {
		this.departmentID = departmentID;
	}

	public String getDepartmentName() {
		return departmentName;
	}

	public void setDepartmentName(String departmentName) {
		this.departmentName = departmentName;
	}

	public Boolean getDisabled() {
		return disabled;
	}

	public void setDisabled(Boolean disabled) {
		this.disabled = disabled;
	}
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
}
