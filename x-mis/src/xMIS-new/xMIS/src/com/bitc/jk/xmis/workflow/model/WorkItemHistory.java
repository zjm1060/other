package com.bitc.jk.xmis.workflow.model;

import java.io.Serializable;
import java.util.Date;

import com.bitc.jk.xmis.model.SimpleEmployee;

public class WorkItemHistory implements Serializable {

	private static final long serialVersionUID = -6257359971759323527L;

	private String id;
	private Integer state;
	private Date createdTime;
	private Date claimedTime;
	private Date endTime;
	private SimpleEmployee actor;
	private String comments;
	private String processId;
	private String processInstanceId;
	private String processName;
	private String activityId;
	private String activityName;
	private String taskInstanceId;
	private String taskId;
	private String taskName;
	private SimpleEmployee sender;//发送人
	private SimpleEmployee agent;
	
	private String authorizeeName;//用函数得到的被授权人姓名
	
	
	public String getAuthorizeeName() {
		return authorizeeName;
	}

	public void setAuthorizeeName(String authorizeeName) {
		this.authorizeeName = authorizeeName;
	}

	public SimpleEmployee getAgent() {
		return agent;
	}

	public void setAgent(SimpleEmployee agent) {
		this.agent = agent;
	}

	public SimpleEmployee getSender() {
		return sender;
	}

	public void setSender(SimpleEmployee sender) {
		this.sender = sender;
	}

	public WorkItemHistory() {
	}

	public WorkItemHistory(String id) {
		this.id = id;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public Integer getState() {
		return state;
	}

	public void setState(Integer state) {
		this.state = state;
	}

	public Date getCreatedTime() {
		return createdTime;
	}

	public void setCreatedTime(Date createdTime) {
		this.createdTime = createdTime;
	}

	public Date getClaimedTime() {
		return claimedTime;
	}

	public void setClaimedTime(Date claimedTime) {
		this.claimedTime = claimedTime;
	}

	public Date getEndTime() {
		return endTime;
	}

	public void setEndTime(Date endTime) {
		this.endTime = endTime;
	}

	public SimpleEmployee getActor() {
		return actor;
	}

	public void setActor(SimpleEmployee actor) {
		this.actor = actor;
	}

	public String getComments() {
		return comments;
	}

	public void setComments(String comments) {
		this.comments = comments;
	}

	public String getTaskName() {
		return taskName;
	}

	public void setTaskName(String taskName) {
		this.taskName = taskName;
	}

	public String getProcessName() {
		return processName;
	}

	public void setProcessName(String processName) {
		this.processName = processName;
	}

	public String getActivityName() {
		return activityName;
	}

	public void setActivityName(String activityName) {
		this.activityName = activityName;
	}

	public String getProcessInstanceId() {
		return processInstanceId;
	}

	public void setProcessInstanceId(String processInstanceId) {
		this.processInstanceId = processInstanceId;
	}

	public String getTaskInstanceId() {
		return taskInstanceId;
	}

	public void setTaskInstanceId(String taskInstanceId) {
		this.taskInstanceId = taskInstanceId;
	}

	public String getActivityId() {
		return activityId;
	}

	public void setActivityId(String activityId) {
		this.activityId = activityId;
	}

	public String getProcessId() {
		return processId;
	}

	public void setProcessId(String processId) {
		this.processId = processId;
	}

	public String getTaskId() {
		return taskId;
	}

	public void setTaskId(String taskId) {
		this.taskId = taskId;
	}

	@Override
	public String toString() {
		return "WorkItemHistory [id=" + id + ", state=" + state
				+ ", createdTime=" + createdTime + ", claimedTime="
				+ claimedTime + ", endTime=" + endTime + ", actor=" + actor
				+ ", comments=" + comments + ", processId=" + processId
				+ ", processInstanceId=" + processInstanceId + ", processName="
				+ processName + ", activityId=" + activityId
				+ ", activityName=" + activityName + ", taskInstanceId="
				+ taskInstanceId + ", taskId=" + taskId + ", taskName="
				+ taskName + "]";
	}
}
