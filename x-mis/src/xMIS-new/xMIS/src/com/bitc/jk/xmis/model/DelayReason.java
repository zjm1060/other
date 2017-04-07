package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.util.Date;

public class DelayReason implements Serializable  {
	private static final long serialVersionUID = 5177324236290061348L;
	
	private int id;
	private int contractID;
	private String reason;
	private int operator;
	private Date dateOfRegistration;
	private int Registrant;
	
	public DelayReason() {
		super();
	}

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

	public String getReason() {
		return reason;
	}

	public void setReason(String reason) {
		this.reason = reason;
	}

	public int getOperator() {
		return operator;
	}

	public void setOperator(int operator) {
		this.operator = operator;
	}

	public Date getDateOfRegistration() {
		return dateOfRegistration;
	}

	public void setDateOfRegistration(Date dateOfRegistration) {
		this.dateOfRegistration = dateOfRegistration;
	}

	public int getRegistrant() {
		return Registrant;
	}

	public void setRegistrant(int registrant) {
		Registrant = registrant;
	}

	
}
