package com.bitc.jk.xmis.workflow.model;

import java.io.Serializable;
import java.util.Date;

import com.bitc.jk.xmis.model.SimpleEmployee;

public class WorkflowApprovalInfo implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private String ID;
	private String workItemID;
	private int isApproved;
	private Date approvedTime;
	private String advice;
	private SimpleEmployee approver;
	public String getID() {
		return ID;
	}
	public void setID(String iD) {
		ID = iD;
	}
	public String getWorkItemID() {
		return workItemID;
	}
	public void setWorkItemID(String workItemID) {
		this.workItemID = workItemID;
	}
	public int getIsApproved() {
		return isApproved;
	}
	public void setIsApproved(int isApproved) {
		this.isApproved = isApproved;
	}
	public Date getApprovedTime() {
		return approvedTime;
	}
	public void setApprovedTime(Date approvedTime) {
		this.approvedTime = approvedTime;
	}
	public String getAdvice() {
		return advice;
	}
	public void setAdvice(String advice) {
		this.advice = advice;
	}
	public SimpleEmployee getApprover() {
		return approver;
	}
	public void setApprover(SimpleEmployee approver) {
		this.approver = approver;
	}
	
}
