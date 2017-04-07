package com.bitc.jk.xmis.model.crm;

import java.sql.Date;

/**
 * @author liying
		TableName = "xmis.org_客户级别";
		customerLevelID = "客户级别ID";
		customerLevelName = "名称";
		customerLevelDefinition = "描述";
 */
public class CustomerLevel {

	private int customerLevelID;
	private String customerLevelName;
	private String customerLevelDefinition;
	private Date updateDate;
	private int updateID;
	
	public int getCustomerLevelID() {
		return customerLevelID;
	}
	public void setCustomerLevelID(int customerLevelID) {
		this.customerLevelID = customerLevelID;
	}
	public String getCustomerLevelName() {
		return customerLevelName;
	}
	public void setCustomerLevelName(String customerLevelName) {
		this.customerLevelName = customerLevelName;
	}
	public String getCustomerLevelDefinition() {
		return customerLevelDefinition;
	}
	public void setCustomerLevelDefinition(String customerLevelDefinition) {
		this.customerLevelDefinition = customerLevelDefinition;
	}
	public Date getUpdateDate() {
		return updateDate;
	}
	public void setUpdateDate(Date updateDate) {
		this.updateDate = updateDate;
	}
	public int getUpdateID() {
		return updateID;
	}
	public void setUpdateID(int updateID) {
		this.updateID = updateID;
	}
	
	
}
