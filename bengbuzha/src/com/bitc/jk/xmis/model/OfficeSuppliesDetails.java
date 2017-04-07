package com.bitc.jk.xmis.model;

import java.sql.Date;

import com.bitc.jk.xmis.util.JSONUtils;

public class OfficeSuppliesDetails extends TableBean{
	
	private static final long serialVersionUID = 588358338404132780L;
	
	private String supplies_id;
	
	private String supplies_type_id;
	
	private String supplies_name;
	
	private String supplies_brand;
	
	private String supplies_model;
	
	private Date purchase_date;
	
	private String remark;
	
	private int qty;

	public String getSupplies_id() {
		return supplies_id;
	}

	public void setSupplies_id(String supplies_id) {
		this.supplies_id = supplies_id;
	}

	public String getSupplies_type_id() {
		return supplies_type_id;
	}

	public void setSupplies_type_id(String supplies_type_id) {
		this.supplies_type_id = supplies_type_id;
	}

	public String getSupplies_name() {
		return supplies_name;
	}

	public void setSupplies_name(String supplies_name) {
		this.supplies_name = supplies_name;
	}

	public String getSupplies_brand() {
		return supplies_brand;
	}

	public void setSupplies_brand(String supplies_brand) {
		this.supplies_brand = supplies_brand;
	}

	public String getSupplies_model() {
		return supplies_model;
	}

	public void setSupplies_model(String supplies_model) {
		this.supplies_model = supplies_model;
	}

	public Date getPurchase_date() {
		return purchase_date;
	}

	public void setPurchase_date(Date purchase_date) {
		this.purchase_date = purchase_date;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public int getQty() {
		return qty;
	}

	public void setQty(int qty) {
		this.qty = qty;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
}
