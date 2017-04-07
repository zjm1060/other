package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.util.Date;
import java.util.List;

public class MissAttendanceObj implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public String id;
	public SimpleEmployee applicant;
	public Date registdate;
	public int isAgent;
	public int isOfficialTravel;
	public String processInstanceId;
	public SimpleEmployee agent;
	public String content;
	
	List<MissAttendanceInfos> members ;
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getContent() {
		return content;
	}
	public void setContent(String content) {
		this.content = content;
	}
	public SimpleEmployee getApplicant() {
		return applicant;
	}
	public void setApplicant(SimpleEmployee applicant) {
		this.applicant = applicant;
	}
	public Date getRegistdate() {
		return registdate;
	}
	public void setRegistdate(Date registdate) {
		this.registdate = registdate;
	}
	public int getIsAgent() {
		return isAgent;
	}
	public void setIsAgent(int isAgent) {
		this.isAgent = isAgent;
	}
	public List<MissAttendanceInfos> getMembers() {
		return members;
	}
	public void setMembers(List<MissAttendanceInfos> members) {
		this.members = members;
	}
	public int getIsOfficialTravel() {
		return isOfficialTravel;
	}
	public void setIsOfficialTravel(int isOfficialTravel) {
		this.isOfficialTravel = isOfficialTravel;
	}
	public String getProcessInstanceId() {
		return processInstanceId;
	}
	public void setProcessInstanceId(String processInstanceId) {
		this.processInstanceId = processInstanceId;
	}
	public SimpleEmployee getAgent() {
		return agent;
	}
	public void setAgent(SimpleEmployee agent) {
		this.agent = agent;
	}
	
}
