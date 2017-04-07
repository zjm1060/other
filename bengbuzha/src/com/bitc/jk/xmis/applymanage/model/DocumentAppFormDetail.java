package com.bitc.jk.xmis.applymanage.model;

import java.io.Serializable;
import java.util.Date;

public class DocumentAppFormDetail implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = -5208338235362592721L;
	private String form_id;
	private int document_type_id; // 公文类别ID（ID=2时，字段10~18不显示在页面表单）
	private int priority;// 优先等级，0普通1紧急，缺省0
	private int file_no;
	private String file_flag;
	private String title;
	private String external_name;
	private String document_code;
	
	private String summary;// 摘要说明
	private Date app_datetime;// 申请时间
	private int app_user_id;// 申请人ID
	private int app_dept_id;// 申请人ID
	private int app_dept_leader_id;// 部门领导ID
	private Date app_dept_approved_datetime;// 部门领导审批日期
	private String app_dept_approved_reason;// 部门领导审批意见
	private int app_spec_leader_id;// 分管领导ID
	private Date app_spec_approved_datetime;// 分管领导审批日期
	private String app_spec_approved_reason;// 分管领导审批意见
	private int app_direc_leader_id;// 主管领导ID
	private Date app_direc_approved_datetime;// 主管领导审批日期
	private String app_direc_approved_reason;// 主管领导审批意见
	
	public String getForm_id() {
		return form_id;
	}
	
	public int getFile_no() {
		return file_no;
	}

	public void setFile_no(int file_no) {
		this.file_no = file_no;
	}

	public void setForm_id(String form_id) {
		this.form_id = form_id;
	}
	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}
	public String getExternal_name() {
		return external_name;
	}
	public void setExternal_name(String external_name) {
		this.external_name = external_name;
	}
	public String getDocument_code() {
		return document_code;
	}
	public void setDocument_code(String document_code) {
		this.document_code = document_code;
	}
	public int getDocument_type_id() {
		return document_type_id;
	}
	public void setDocument_type_id(int document_type_id) {
		this.document_type_id = document_type_id;
	}
	public int getPriority() {
		return priority;
	}
	public void setPriority(int priority) {
		this.priority = priority;
	}
	public String getSummary() {
		return summary;
	}
	public void setSummary(String summary) {
		this.summary = summary;
	}
	public Date getApp_datetime() {
		return app_datetime;
	}
	public void setApp_datetime(Date app_datetime) {
		this.app_datetime = app_datetime;
	}
	public int getApp_user_id() {
		return app_user_id;
	}
	public void setApp_user_id(int app_user_id) {
		this.app_user_id = app_user_id;
	}
	public int getApp_dept_leader_id() {
		return app_dept_leader_id;
	}
	public void setApp_dept_leader_id(int app_dept_leader_id) {
		this.app_dept_leader_id = app_dept_leader_id;
	}
	public Date getApp_dept_approved_datetime() {
		return app_dept_approved_datetime;
	}
	public void setApp_dept_approved_datetime(Date app_dept_approved_datetime) {
		this.app_dept_approved_datetime = app_dept_approved_datetime;
	}
	public String getApp_dept_approved_reason() {
		return app_dept_approved_reason;
	}
	public void setApp_dept_approved_reason(String app_dept_approved_reason) {
		this.app_dept_approved_reason = app_dept_approved_reason;
	}
	public int getApp_spec_leader_id() {
		return app_spec_leader_id;
	}
	public void setApp_spec_leader_id(int app_spec_leader_id) {
		this.app_spec_leader_id = app_spec_leader_id;
	}
	public Date getApp_spec_approved_datetime() {
		return app_spec_approved_datetime;
	}
	public void setApp_spec_approved_datetime(Date app_spec_approved_datetime) {
		this.app_spec_approved_datetime = app_spec_approved_datetime;
	}
	public String getApp_spec_approved_reason() {
		return app_spec_approved_reason;
	}
	public void setApp_spec_approved_reason(String app_spec_approved_reason) {
		this.app_spec_approved_reason = app_spec_approved_reason;
	}
	public int getApp_direc_leader_id() {
		return app_direc_leader_id;
	}
	public void setApp_direc_leader_id(int app_direc_leader_id) {
		this.app_direc_leader_id = app_direc_leader_id;
	}
	public Date getApp_direc_approved_datetime() {
		return app_direc_approved_datetime;
	}
	public void setApp_direc_approved_datetime(Date app_direc_approved_datetime) {
		this.app_direc_approved_datetime = app_direc_approved_datetime;
	}
	public String getApp_direc_approved_reason() {
		return app_direc_approved_reason;
	}
	public void setApp_direc_approved_reason(String app_direc_approved_reason) {
		this.app_direc_approved_reason = app_direc_approved_reason;
	}
	public int getApp_dept_id() {
		return app_dept_id;
	}
	public void setApp_dept_id(int app_dept_id) {
		this.app_dept_id = app_dept_id;
	}

	public String getFile_flag() {
		return file_flag;
	}

	public void setFile_flag(String file_flag) {
		this.file_flag = file_flag;
	}

	
}
