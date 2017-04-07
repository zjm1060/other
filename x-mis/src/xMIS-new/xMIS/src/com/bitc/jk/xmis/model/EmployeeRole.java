package com.bitc.jk.xmis.model;

public class EmployeeRole extends TableBean {

	private static final long serialVersionUID = -146492138785025627L;
	private String id;
	private SimpleEmployee employee;
	private Role role;

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public SimpleEmployee getEmployee() {
		return employee;
	}

	public void setEmployee(SimpleEmployee employee) {
		this.employee = employee;
	}

	public Role getRole() {
		return role;
	}

	public void setRole(Role role) {
		this.role = role;
	}

}
