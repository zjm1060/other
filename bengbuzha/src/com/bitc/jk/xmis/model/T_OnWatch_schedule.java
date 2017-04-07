package com.bitc.jk.xmis.model;

import java.sql.Date;

public class T_OnWatch_schedule{

	private String id;
	
	private int jobTypeId;
	
	private int onWatchId;
	
	private String onWatchDate;
	
	private String beginTime;
	
	private String endDate;
	
	private String endTime;
	
	private String Reason;
	
	private String scheduledDatetime;
	
	private int scheduledId;
	
	private int lastUpdatedId;
	
	private String lastUpdatedDatetime;

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public int getJobTypeId() {
		return jobTypeId;
	}

	public void setJobTypeId(int jobTypeId) {
		this.jobTypeId = jobTypeId;
	}

	public int getOnWatchId() {
		return onWatchId;
	}

	public void setOnWatchId(int onWatchId) {
		this.onWatchId = onWatchId;
	}

	public String getOnWatchDate() {
		return onWatchDate;
	}

	public void setOnWatchDate(String onWatchDate) {
		this.onWatchDate = onWatchDate;
	}

	public String getBeginTime() {
		return beginTime;
	}

	public void setBeginTime(String beginTime) {
		this.beginTime = beginTime;
	}

	public String getEndDate() {
		return endDate;
	}

	public void setEndDate(String endDate) {
		this.endDate = endDate;
	}

	public String getEndTime() {
		return endTime;
	}
	
	public void setEndTime(String endTime) {
		this.endTime = endTime;
	}

	public String getReason() {
		return Reason;
	}

	public void setReason(String reason) {
		Reason = reason;
	}

	public String getScheduledDatetime() {
		return scheduledDatetime;
	}

	public void setScheduledDatetime(String scheduledDatetime) {
		this.scheduledDatetime = scheduledDatetime;
	}

	public int getScheduledId() {
		return scheduledId;
	}

	public void setScheduledId(int scheduledId) {
		this.scheduledId = scheduledId;
	}

	public int getLastUpdatedId() {
		return lastUpdatedId;
	}

	public void setLastUpdatedId(int lastUpdatedId) {
		this.lastUpdatedId = lastUpdatedId;
	}

	public String getLastUpdatedDatetime() {
		return lastUpdatedDatetime;
	}

	public void setLastUpdatedDatetime(String lastUpdatedDatetime) {
		this.lastUpdatedDatetime = lastUpdatedDatetime;
	}
	
	
	
}
