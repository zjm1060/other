package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.sql.Date;

public class EstimatedPayment implements Serializable {

	private static final long serialVersionUID = -6371889764472000441L;

	private String id;
	private String projectId;
	private Date estimatedDate;
	private double estimatedAmt;
	private double estimatedPercent;
	private int isPayed;
	private int registerId;
	private String registerName;
	private Date registerDate;
	private int lastModifiedId;
	private int lastModifiedName;
	private Date lastModifiedDate;

	public EstimatedPayment() {
		super();
	}

	public EstimatedPayment(String id, String projectId, Date estimatedDate,
			double estimatedAmt, double estimatedPercent, int isPayed,
			int registerId, String registerName, Date registerDate,
			int lastModifiedId, int lastModifiedName, Date lastModifiedDate) {
		super();
		this.id = id;
		this.projectId = projectId;
		this.estimatedDate = estimatedDate;
		this.estimatedAmt = estimatedAmt;
		this.estimatedPercent = estimatedPercent;
		this.isPayed = isPayed;
		this.registerId = registerId;
		this.registerName = registerName;
		this.registerDate = registerDate;
		this.lastModifiedId = lastModifiedId;
		this.lastModifiedName = lastModifiedName;
		this.lastModifiedDate = lastModifiedDate;
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

	public Date getEstimatedDate() {
		return estimatedDate;
	}

	public void setEstimatedDate(Date estimatedDate) {
		this.estimatedDate = estimatedDate;
	}

	public double getEstimatedAmt() {
		return estimatedAmt;
	}

	public void setEstimatedAmt(double estimatedAmt) {
		this.estimatedAmt = estimatedAmt;
	}

	public double getEstimatedPercent() {
		return estimatedPercent;
	}

	public void setEstimatedPercent(double estimatedPercent) {
		this.estimatedPercent = estimatedPercent;
	}

	public int getIsPayed() {
		return isPayed;
	}

	public void setIsPayed(int isPayed) {
		this.isPayed = isPayed;
	}

	public int getRegisterId() {
		return registerId;
	}

	public void setRegisterId(int registerId) {
		this.registerId = registerId;
	}

	public String getRegisterName() {
		return registerName;
	}

	public void setRegisterName(String registerName) {
		this.registerName = registerName;
	}

	public Date getRegisterDate() {
		return registerDate;
	}

	public void setRegisterDate(Date registerDate) {
		this.registerDate = registerDate;
	}

	public int getLastModifiedId() {
		return lastModifiedId;
	}

	public void setLastModifiedId(int lastModifiedId) {
		this.lastModifiedId = lastModifiedId;
	}

	public int getLastModifiedName() {
		return lastModifiedName;
	}

	public void setLastModifiedName(int lastModifiedName) {
		this.lastModifiedName = lastModifiedName;
	}

	public Date getLastModifiedDate() {
		return lastModifiedDate;
	}

	public void setLastModifiedDate(Date lastModifiedDate) {
		this.lastModifiedDate = lastModifiedDate;
	}

}
