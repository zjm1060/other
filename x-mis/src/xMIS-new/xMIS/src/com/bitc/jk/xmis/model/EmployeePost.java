package com.bitc.jk.xmis.model;

import java.io.Serializable;

import com.bitc.jk.xmis.model.sys.Post;

public class EmployeePost implements Serializable {

	private static final long serialVersionUID = 5691206303136027415L;

	private String id;
	private DepartmentObj department;
	private SimpleEmployee employee;
	private Post post;

	public Post getPost() {
		return post;
	}

	public void setPost(Post post) {
		this.post = post;
	}

	public EmployeePost() {
	}

	public SimpleEmployee getEmployee() {
		return employee;
	}

	public void setEmployee(SimpleEmployee employee) {
		this.employee = employee;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public DepartmentObj getDepartment() {
		return department;
	}

	public void setDepartment(DepartmentObj department) {
		this.department = department;
	}

}
