package com.bitc.jk.xmis.workflow.model;

import java.io.Serializable;

public class WfCalendar implements Serializable {

	private static final long serialVersionUID = -1337513644326558025L;

	/**
	 * 工作日，一般是周一到周五
	 */
	public static final int TYPE_WEEKDAYS = 0;
	/**
	 * 周末，周六和周日
	 */
	public static final int TYPE_WEEKEDNS = 1;

	/**
	 * 国家法定假日
	 */
	public static final int TYPE_LEGAL_HOLIDAYS = 2;

	private String id;
	private String date;
	private int type;
	private int dayOfWeek;
	private String amS;
	private String amE;
	private String pmS;
	private String pmE;

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public int getType() {
		return type;
	}

	public void setType(int type) {
		this.type = type;
	}

	public String getAmS() {
		return amS;
	}

	public void setAmS(String amS) {
		this.amS = amS;
	}

	public String getAmE() {
		return amE;
	}

	public void setAmE(String amE) {
		this.amE = amE;
	}

	public String getPmS() {
		return pmS;
	}

	public void setPmS(String pmS) {
		this.pmS = pmS;
	}

	public String getPmE() {
		return pmE;
	}

	public void setPmE(String pmE) {
		this.pmE = pmE;
	}

	public String getDate() {
		return date;
	}

	public void setDate(String date) {
		this.date = date;
	}

	public int getDayOfWeek() {
		return dayOfWeek;
	}

	public void setDayOfWeek(int dayOfWeek) {
		this.dayOfWeek = dayOfWeek;
	}

}
