package com.bitc.jk.xmis.model;

import java.io.Serializable;

public class ContractInfoObj implements Serializable{
	
	private int contractID;
	private int projectResourceParentID;//项目来源父ID
	private int projectResourceID;//项目来源ID 
	private int projectOperationTypeParentID;//项目合作形式父ID
	private int projectOperationTypeID;//项目合作形式ID
	private int projectPropertyID;//项目性质ID
	private int accountingAgencyID;//核算机构ID
	private int executiveOrgID;//执行单位ID
	
	public int getContractID() {
		return contractID;
	}
	public void setContractID(int contractID) {
		this.contractID = contractID;
	}
	public int getProjectResourceParentID() {
		return projectResourceParentID;
	}
	public void setProjectResourceParentID(int projectResourceParentID) {
		this.projectResourceParentID = projectResourceParentID;
	}
	public int getProjectResourceID() {
		return projectResourceID;
	}
	public void setProjectResourceID(int projectResourceID) {
		this.projectResourceID = projectResourceID;
	}
	public int getProjectOperationTypeParentID() {
		return projectOperationTypeParentID;
	}
	public void setProjectOperationTypeParentID(int projectOperationTypeParentID) {
		this.projectOperationTypeParentID = projectOperationTypeParentID;
	}
	public int getProjectOperationTypeID() {
		return projectOperationTypeID;
	}
	public void setProjectOperationTypeID(int projectOperationTypeID) {
		this.projectOperationTypeID = projectOperationTypeID;
	}
	public int getProjectPropertyID() {
		return projectPropertyID;
	}
	public void setProjectPropertyID(int projectPropertyID) {
		this.projectPropertyID = projectPropertyID;
	}
	public int getAccountingAgencyID() {
		return accountingAgencyID;
	}
	public void setAccountingAgencyID(int accountingAgencyID) {
		this.accountingAgencyID = accountingAgencyID;
	}
	public int getExecutiveOrgID() {
		return executiveOrgID;
	}
	public void setExecutiveOrgID(int executiveOrgID) {
		this.executiveOrgID = executiveOrgID;
	}
	
	
}
