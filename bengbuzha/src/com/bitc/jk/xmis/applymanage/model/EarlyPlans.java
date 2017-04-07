package com.bitc.jk.xmis.applymanage.model;

import java.util.Date;

public class EarlyPlans {

	private String formId;
	private int typeId;
	private String workMonth;
	private String planAssignComments;
	private String planComments;
	private int planAssignedId;
	private Date planAssignedDate;
	private int appUserId;
	private int appDeptId;
	private Date appDate;
	public String getFormId() {
		return formId;
	}
	public void setFormId(String formId) {
		this.formId = formId;
	}
	public int getTypeId() {
		return typeId;
	}
	public void setTypeId(int typeId) {
		this.typeId = typeId;
	}
	public String getWorkMonth() {
		return workMonth;
	}
	public void setWorkMonth(String workMonth) {
		this.workMonth = workMonth;
	}
	public String getPlanAssignComments() {
		return planAssignComments;
	}
	public void setPlanAssignComments(String planAssignComments) {
		this.planAssignComments = planAssignComments;
	}
	public String getPlanComments() {
		return planComments;
	}
	public void setPlanComments(String planComments) {
		this.planComments = planComments;
	}
	public int getPlanAssignedId() {
		return planAssignedId;
	}
	public void setPlanAssignedId(int planAssignedId) {
		this.planAssignedId = planAssignedId;
	}
	public Date getPlanAssignedDate() {
		return planAssignedDate;
	}
	public void setPlanAssignedDate(Date planAssignedDate) {
		this.planAssignedDate = planAssignedDate;
	}
	public int getAppUserId() {
		return appUserId;
	}
	public void setAppUserId(int appUserId) {
		this.appUserId = appUserId;
	}
	public Date getAppDate() {
		return appDate;
	}
	public void setAppDate(Date appDate) {
		this.appDate = appDate;
	}
	public int getAppDeptId() {
		return appDeptId;
	}
	public void setAppDeptId(int appDeptId) {
		this.appDeptId = appDeptId;
	}
	
}
