package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.util.Date;

public class IncomeAssistObj implements Serializable {

	private static final long serialVersionUID = 4886969856985369012L;
	private int contractID;
	private String projectNo;
	private String contractShortName;
	private String invoiceAmt;
	private String antipateAmt;
	private Date registrationTime;
	private String remark;
	private String payerAccounts;
	private int companyID;
	private String companyShortName;
	private Boolean isFlag;
	private String contractMsg;
	private Date createTime;

	public String getContractMsg() {
		return contractMsg;
	}

	public void setContractMsg(String contractMsg) {
		this.contractMsg = contractMsg;
	}

	public Date getCreateTime() {
		return createTime;
	}

	public void setCreateTime(Date createTime) {
		this.createTime = createTime;
	}

	public int getContractID() {
		return contractID;
	}

	public void setContractID(int contractID) {
		this.contractID = contractID;
	}

	public String getProjectNo() {
		return projectNo;
	}

	public void setProjectNo(String projectNo) {
		this.projectNo = projectNo;
	}

	public String getContractShortName() {
		return contractShortName;
	}

	public void setContractShortName(String contractShortName) {
		this.contractShortName = contractShortName;
	}

	public String getInvoiceAmt() {
		return invoiceAmt;
	}

	public void setInvoiceAmt(String invoiceAmt) {
		this.invoiceAmt = invoiceAmt;
	}

	public String getAntipateAmt() {
		return antipateAmt;
	}

	public void setAntipateAmt(String antipateAmt) {
		this.antipateAmt = antipateAmt;
	}

	public Date getRegistrationTime() {
		return registrationTime;
	}

	public void setRegistrationTime(Date registrationTime) {
		this.registrationTime = registrationTime;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public String getPayerAccounts() {
		return payerAccounts;
	}

	public void setPayerAccounts(String payerAccounts) {
		this.payerAccounts = payerAccounts;
	}

	public int getCompanyID() {
		return companyID;
	}

	public void setCompanyID(int companyID) {
		this.companyID = companyID;
	}

	public String getCompanyShortName() {
		return companyShortName;
	}

	public void setCompanyShortName(String companyShortName) {
		this.companyShortName = companyShortName;
	}

	public Boolean getIsFlag() {
		return isFlag;
	}

	public void setIsFlag(Boolean isFlag) {
		this.isFlag = isFlag;
	}

}
