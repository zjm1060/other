package com.bitc.jk.xmis.applymanage.model;

import java.io.Serializable;
import java.util.Date;

public class ApplyOfficialSuppliesRepairForm implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public String form_id;
	public String supplies_id;
	public int priority;
	public String reason;
	public int qty;
	public Date app_datetime;
	public int app_user_id;
	public String remark;
	
	public String getForm_id() {
		return form_id;
	}
	public void setForm_id(String form_id) {
		this.form_id = form_id;
	}
	public String getSupplies_id() {
		return supplies_id;
	}
	public void setSupplies_id(String supplies_id) {
		this.supplies_id = supplies_id;
	}
	public int getPriority() {
		return priority;
	}
	public void setPriority(int priority) {
		this.priority = priority;
	}
	public String getReason() {
		return reason;
	}
	public void setReason(String reason) {
		this.reason = reason;
	}
	public int getQty() {
		return qty;
	}
	public void setQty(int qty) {
		this.qty = qty;
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
	public String getRemark() {
		return remark;
	}
	public void setRemark(String remark) {
		this.remark = remark;
	}
	
}
