package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.util.Date;

public class InvoiceSplitObj implements Serializable {

	private static final long serialVersionUID = -7374245290824937691L;

	private int id;
	private int contractID;
	private int invoiceNumber;
	private double splitSum;
	private Boolean payFlag;
	private Date anticipatedTime;
	private String remark;
	private Date registrationTime;
	private int registrant;

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public int getContractID() {
		return contractID;
	}

	public void setContractID(int contractID) {
		this.contractID = contractID;
	}

	public int getInvoiceNumber() {
		return invoiceNumber;
	}

	public void setInvoiceNumber(int invoiceNumber) {
		this.invoiceNumber = invoiceNumber;
	}

	public double getSplitSum() {
		return splitSum;
	}

	public void setSplitSum(double splitSum) {
		this.splitSum = splitSum;
	}

	public Boolean getPayFlag() {
		return payFlag;
	}

	public void setPayFlag(Boolean payFlag) {
		this.payFlag = payFlag;
	}

	public Date getAnticipatedTime() {
		return anticipatedTime;
	}

	public void setAnticipatedTime(Date anticipatedTime) {
		this.anticipatedTime = anticipatedTime;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public Date getRegistrationTime() {
		return registrationTime;
	}

	public void setRegistrationTime(Date registrationTime) {
		this.registrationTime = registrationTime;
	}

	public int getRegistrant() {
		return registrant;
	}

	public void setRegistrant(int registrant) {
		this.registrant = registrant;
	}

}
