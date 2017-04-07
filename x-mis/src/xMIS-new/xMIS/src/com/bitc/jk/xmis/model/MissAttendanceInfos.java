package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.util.Date;

public class MissAttendanceInfos implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public String id;
	public Date missDate;
	public int amorpm;
	public String missDesc;
	public String missAttendanceId;
	public Date startDate;
	public Date stopDate;
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public Date getMissDate() {
		return missDate;
	}
	public void setMissDate(Date missDate) {
		this.missDate = missDate;
	}
	public int getAmorpm() {
		return amorpm;
	}
	public void setAmorpm(int amorpm) {
		this.amorpm = amorpm;
	}
	public String getMissDesc() {
		return missDesc;
	}
	public void setMissDesc(String missDesc) {
		this.missDesc = missDesc;
	}
	public String getMissAttendanceId() {
		return missAttendanceId;
	}
	public void setMissAttendanceId(String missAttendanceId) {
		this.missAttendanceId = missAttendanceId;
	}
	public Date getStartDate() {
		return startDate;
	}
	public void setStartDate(Date startDate) {
		this.startDate = startDate;
	}
	public Date getStopDate() {
		return stopDate;
	}
	public void setStopDate(Date stopDate) {
		this.stopDate = stopDate;
	}
	
}
