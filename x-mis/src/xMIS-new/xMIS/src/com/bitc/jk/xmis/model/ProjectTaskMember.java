package com.bitc.jk.xmis.model;


public class ProjectTaskMember extends TableBean {

	private static final long serialVersionUID = 4404296633774302905L;
	private String id;
	private String projectTaskId;
	private SimpleEmployee member;
	private String taskDesc;
	private int role = 3;
	private String remark;

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getProjectTaskId() {
		return projectTaskId;
	}

	public void setProjectTaskId(String projectTaskId) {
		this.projectTaskId = projectTaskId;
	}

	public String getTaskDesc() {
		return taskDesc;
	}

	public void setTaskDesc(String taskDesc) {
		this.taskDesc = taskDesc;
	}

	public SimpleEmployee getMember() {
		return member;
	}

	public void setMember(SimpleEmployee member) {
		this.member = member;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public int getRole() {
		return role;
	}

	public void setRole(int role) {
		this.role = role;
	}

}
