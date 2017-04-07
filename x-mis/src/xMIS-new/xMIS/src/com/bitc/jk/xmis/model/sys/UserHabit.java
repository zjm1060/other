package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;

public class UserHabit implements Serializable {

	private static final long serialVersionUID = 8051691813937890212L;

	private String id;
	private Integer employeeID;
	private String habit;

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public Integer getEmployeeID() {
		return employeeID;
	}

	public void setEmployeeID(Integer employeeID) {
		this.employeeID = employeeID;
	}

	public String getHabit() {
		return habit;
	}

	public void setHabit(String habit) {
		this.habit = habit;
	}

}
