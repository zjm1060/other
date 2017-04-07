package com.bitc.jk.xmis.workflow.model;

import java.util.Calendar;

public class Week {
	private int id;
	private WfCalendar monday;
	private WfCalendar sunday;
	private WfCalendar tuesday;
	private WfCalendar wednesday;
	private WfCalendar thursday;
	private WfCalendar friday;
	private WfCalendar saturday;

	public void setDay(WfCalendar wfCalendar) {
		switch (wfCalendar.getDayOfWeek()) {
		case Calendar.SUNDAY:
			this.sunday = wfCalendar;
			break;
		case Calendar.MONDAY:
			this.monday = wfCalendar;
			break;
		case Calendar.TUESDAY:
			this.tuesday = wfCalendar;
			break;
		case Calendar.WEDNESDAY:
			this.wednesday = wfCalendar;
			break;
		case Calendar.THURSDAY:
			this.thursday = wfCalendar;
			break;
		case Calendar.FRIDAY:
			this.friday = wfCalendar;
			break;
		case Calendar.SATURDAY:
			this.saturday = wfCalendar;
			break;
		default:
			break;
		}
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public WfCalendar getMonday() {
		return monday;
	}

	public void setMonday(WfCalendar monday) {
		this.monday = monday;
	}

	public WfCalendar getSunday() {
		return sunday;
	}

	public void setSunday(WfCalendar sunday) {
		this.sunday = sunday;
	}

	public WfCalendar getTuesday() {
		return tuesday;
	}

	public void setTuesday(WfCalendar tuesday) {
		this.tuesday = tuesday;
	}

	public WfCalendar getWednesday() {
		return wednesday;
	}

	public void setWednesday(WfCalendar wednesday) {
		this.wednesday = wednesday;
	}

	public WfCalendar getThursday() {
		return thursday;
	}

	public void setThursday(WfCalendar thursday) {
		this.thursday = thursday;
	}

	public WfCalendar getFriday() {
		return friday;
	}

	public void setFriday(WfCalendar friday) {
		this.friday = friday;
	}

	public WfCalendar getSaturday() {
		return saturday;
	}

	public void setSaturday(WfCalendar saturday) {
		this.saturday = saturday;
	}

}
