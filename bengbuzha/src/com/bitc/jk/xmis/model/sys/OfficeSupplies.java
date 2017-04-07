package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;

public class OfficeSupplies implements Serializable{
	
	private static final long serialVersionUID = 8399193481008317647L;
	
	private String supplies_type_id;
	
	private String supplies_type_Name;
	 private String remark;
	 
	 
	public String getSupplies_type_id() {
		return supplies_type_id;
	}
	public void setSupplies_type_id(String supplies_type_id) {
		this.supplies_type_id = supplies_type_id;
	}
	public String getSupplies_type_Name() {
		return supplies_type_Name;
	}
	public void setSupplies_type_Name(String supplies_type_Name) {
		this.supplies_type_Name = supplies_type_Name;
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
	 

}
