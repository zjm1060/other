package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.sql.Date;

public class EstimateIncomeObj extends TableBean implements Serializable {

	private static final long serialVersionUID = -838845837521156076L;
	private String id;
	private String projectId;
	private Date incomeDate;
	private Double amt;
	private boolean isPayed;
	private int registerId;

	public EstimateIncomeObj() {
		super();
	}

	public EstimateIncomeObj(String id, String projectId, Date incomeDate,
			Double amt, boolean isPayed, int registerId) {
		super();
		this.id = id;
		this.projectId = projectId;
		this.incomeDate = incomeDate;
		this.amt = amt;
		this.isPayed = isPayed;
		this.registerId = registerId;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getProjectId() {
		return projectId;
	}

	public void setProjectId(String projectId) {
		this.projectId = projectId;
	}

	public Date getIncomeDate() {
		return incomeDate;
	}

	public void setIncomeDate(Date incomeDate) {
		this.incomeDate = incomeDate;
	}

	public Double getAmt() {
		return amt;
	}

	public void setAmt(Double amt) {
		this.amt = amt;
	}

	public boolean isPayed() {
		return isPayed;
	}

	public void setPayed(boolean isPayed) {
		this.isPayed = isPayed;
	}

	public int getRegisterId() {
		return registerId;
	}

	public void setRegisterId(int registerId) {
		this.registerId = registerId;
	}

}
