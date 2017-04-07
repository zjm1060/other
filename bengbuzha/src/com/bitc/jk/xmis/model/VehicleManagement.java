package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.sql.Date;

import com.bitc.jk.xmis.util.JSONUtils;

public class VehicleManagement implements Serializable {
	private static final long serialVersionUID = 949846564180205754L;
	
	/**
	 * 车ID
	 * */
	private String car_id;
	/**
	 * 车编号
	 * */
	private String car_No;
	/**
	 * 品牌名称
	 * */
	private String car_brand;
	/**
	 * 车辆型号
	 * */
	private String car_type;
	/**
	 * 购买日期
	 * */
	private Date purchase_date;
	/**
	 * 准乘人数
	 * */
	private int capacity;
	/**
	 * 备注
	 * */
	private String remark;
	public String getCar_id() {
		return car_id;
	}
	public void setCar_id(String car_id) {
		this.car_id = car_id;
	}
	public String getCar_No() {
		return car_No;
	}
	public void setCar_No(String car_No) {
		this.car_No = car_No;
	}
	public String getCar_brand() {
		return car_brand;
	}
	public void setCar_brand(String car_brand) {
		this.car_brand = car_brand;
	}
	public String getCar_type() {
		return car_type;
	}
	public void setCar_type(String car_type) {
		this.car_type = car_type;
	}
	public Date getPurchase_date() {
		return purchase_date;
	}
	public void setPurchase_date(Date purchase_date) {
		this.purchase_date = purchase_date;
	}
	public int getCapacity() {
		return capacity;
	}
	public void setCapacity(int capacity) {
		this.capacity = capacity;
	}
	public String getRemark() {
		return remark;
	}
	public void setRemark(String remark) {
		this.remark = remark;
	}
	public static long getSerialversionuid() {
		return serialVersionUID;
	}
	
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
	
}
