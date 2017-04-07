package com.bitc.jk.xmis.applymanage.model;

import java.util.Date;

public class TheEndOfThePlanned {

	private String formId;
	private String excutedComments;
	private String unfinishedComments;
	private String excutedAssignedComments;
	private int excutedAssignedId;
	private Date excuteAssignedDate;
	public String getFormId() {
		return formId;
	}
	public void setFormId(String formId) {
		this.formId = formId;
	}
	public String getExcutedComments() {
		return excutedComments;
	}
	public void setExcutedComments(String excutedComments) {
		this.excutedComments = excutedComments;
	}
	public String getUnfinishedComments() {
		return unfinishedComments;
	}
	public void setUnfinishedComments(String unfinishedComments) {
		this.unfinishedComments = unfinishedComments;
	}
	public String getExcutedAssignedComments() {
		return excutedAssignedComments;
	}
	public void setExcutedAssignedComments(String excutedAssignedComments) {
		this.excutedAssignedComments = excutedAssignedComments;
	}
	public int getExcutedAssignedId() {
		return excutedAssignedId;
	}
	public void setExcutedAssignedId(int excutedAssignedId) {
		this.excutedAssignedId = excutedAssignedId;
	}
	public Date getExcuteAssignedDate() {
		return excuteAssignedDate;
	}
	public void setExcuteAssignedDate(Date excuteAssignedDate) {
		this.excuteAssignedDate = excuteAssignedDate;
	}
	
}
