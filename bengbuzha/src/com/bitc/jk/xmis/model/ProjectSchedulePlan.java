package com.bitc.jk.xmis.model;

import java.util.Date;

import org.apache.commons.lang.StringUtils;

public class ProjectSchedulePlan extends TableBean {

	private static final long serialVersionUID = 1648425104195581090L;

	private String id;
	private ProjectObj project;
	private ProjectStatus status;
	//预计开始日期
	private Date startDate;
	//实际开始日期
	private Date actualStartDate;

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = StringUtils.isEmpty(id) ? null : id;
	}

	public ProjectObj getProject() {
		return project;
	}

	public void setProject(ProjectObj project) {
		this.project = project;
	}

	public ProjectStatus getStatus() {
		return status;
	}

	public void setStatus(ProjectStatus status) {
		this.status = status;
	}

	public Date getStartDate() {
		return startDate;
	}

	public void setStartDate(Date startDate) {
		this.startDate = startDate;
	}

	public Date getActualStartDate() {
		return actualStartDate;
	}

	public void setActualStartDate(Date actualStartDate) {
		this.actualStartDate = actualStartDate;
	}

}
