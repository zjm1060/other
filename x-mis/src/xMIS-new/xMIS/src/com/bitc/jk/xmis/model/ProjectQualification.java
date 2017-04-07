package com.bitc.jk.xmis.model;

import java.sql.Date;

import com.bitc.jk.xmis.util.JSONUtils;

public class ProjectQualification extends TableBean{
	
	private static final long serialVersionUID = 588358338404132780L;
	
	/**
	 * ID主键
	 * */
	private String id;
	
	/**
	 * 申请部门ID 
	 * */
	private int applicationSectorsID;
	
	/**
	 * 资质名称对应ID
	 * */
	private String ziID;
	
	/**
	 * 项目ID
	 * */
	private String projectID;
	
	/**
	 * 申请人ID
	 * */
	private int applicantID;
	
	/**
	 * 申请时间
	 * */
	private Date applicationDate;
	
	/**
	 * 申请原因
	 * */
	private String applicationReasons;
	
	private SimpleEmployee applicationNewregisterName;
	private Date applicationNewupdateDate;
	private SimpleEmployee applicationLastModifierName;
	private Date applicationLastModified;
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public int getApplicationSectorsID() {
		return applicationSectorsID;
	}
	public void setApplicationSectorsID(int applicationSectorsID) {
		this.applicationSectorsID = applicationSectorsID;
	}
	public String getZiID() {
		return ziID;
	}
	public void setZiID(String ziID) {
		this.ziID = ziID;
	}

	public String getProjectID() {
		return projectID;
	}
	public void setProjectID(String projectID) {
		this.projectID = projectID;
	}
	public int getApplicantID() {
		return applicantID;
	}
	public void setApplicantID(int applicantID) {
		this.applicantID = applicantID;
	}
	public Date getApplicationDate() {
		return applicationDate;
	}
	public void setApplicationDate(Date applicationDate) {
		this.applicationDate = applicationDate;
	}
	public String getApplicationReasons() {
		return applicationReasons;
	}
	public void setApplicationReasons(String applicationReasons) {
		this.applicationReasons = applicationReasons;
	}
	
	public SimpleEmployee getApplicationNewregisterName() {
		return applicationNewregisterName;
	}
	public void setApplicationNewregisterName(
			SimpleEmployee applicationNewregisterName) {
		this.applicationNewregisterName = applicationNewregisterName;
	}
	public SimpleEmployee getApplicationLastModifierName() {
		return applicationLastModifierName;
	}
	public void setApplicationLastModifierName(
			SimpleEmployee applicationLastModifierName) {
		this.applicationLastModifierName = applicationLastModifierName;
	}
	public Date getApplicationNewupdateDate() {
		return applicationNewupdateDate;
	}
	public void setApplicationNewupdateDate(Date applicationNewupdateDate) {
		this.applicationNewupdateDate = applicationNewupdateDate;
	}

	public Date getApplicationLastModified() {
		return applicationLastModified;
	}
	public void setApplicationLastModified(Date applicationLastModified) {
		this.applicationLastModified = applicationLastModified;
	}
	public static long getSerialversionuid() {
		return serialVersionUID;
	}
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
}
