package com.bitc.jk.xmis.model.crm;

import java.sql.Date;

/**
 * @author liying
 *		TableName = "xmis.org_信用等级";
		creditLevelID = "信用等级ID";
		creditLevelName = "名称";
		creditLevelDefinition = "描述";
 */
public class CreditLevel {
	private int creditLevelID;
	private String creditLevelName;
	private String creditLevelDefinition;
	private Date updateDate;
	private int updateID;
	
	public int getCreditLevelID() {
		return creditLevelID;
	}
	public void setCreditLevelID(int creditLevelID) {
		this.creditLevelID = creditLevelID;
	}
	public String getCreditLevelName() {
		return creditLevelName;
	}
	public void setCreditLevelName(String creditLevelName) {
		this.creditLevelName = creditLevelName;
	}
	public String getCreditLevelDefinition() {
		return creditLevelDefinition;
	}
	public void setCreditLevelDefinition(String creditLevelDefinition) {
		this.creditLevelDefinition = creditLevelDefinition;
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
