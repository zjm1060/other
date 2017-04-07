package com.bitc.jk.xmis.applymanage.model;

import java.io.Serializable;
import java.util.Date;

public class AbsentForm implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = -3319337351563727441L;
	private String form_id;
	private int absent_type_id;
	private Date plan_begin_date;
	private Date plan_end_date;
	private Date actual_begin_date;
	private Date actual_end_date;
	private Date leavetime;
	private String reason;
	private Date app_datetime;
	private int app_user_id;
	private String plan_sum;
	private String actual_sum;
	
	public String getForm_id() {
		return form_id;
	}
	public void setForm_id(String form_id) {
		this.form_id = form_id;
	}
	public int getAbsent_type_id() {
		return absent_type_id;
	}
	public void setAbsent_type_id(int absent_type_id) {
		this.absent_type_id = absent_type_id;
	}
	public Date getPlan_begin_date() {
		return plan_begin_date;
	}
	public void setPlan_begin_date(Date plan_begin_date) {
		this.plan_begin_date = plan_begin_date;
	}
	public Date getPlan_end_date() {
		return plan_end_date;
	}
	public void setPlan_end_date(Date plan_end_date) {
		this.plan_end_date = plan_end_date;
	}
	public Date getActual_begin_date() {
		return actual_begin_date;
	}
	public void setActual_begin_date(Date actual_begin_date) {
		this.actual_begin_date = actual_begin_date;
	}
	public Date getActual_end_date() {
		return actual_end_date;
	}
	public void setActual_end_date(Date actual_end_date) {
		this.actual_end_date = actual_end_date;
	}
	public Date getLeavetime() {
		return leavetime;
	}
	public void setLeavetime(Date leavetime) {
		this.leavetime = leavetime;
	}
	public String getReason() {
		return reason;
	}
	public void setReason(String reason) {
		this.reason = reason;
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
	public String getPlan_sum() {
		return plan_sum;
	}
	public void setPlan_sum(String plan_sum) {
		this.plan_sum = plan_sum;
	}
	public String getActual_sum() {
		return actual_sum;
	}
	public void setActual_sum(String actual_sum) {
		this.actual_sum = actual_sum;
	}

	
}
