//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\ProjectTaskObj.java

package com.bitc.jk.xmis.model;

import java.sql.Date;
import java.util.List;

public class ProjectTaskObj extends TableBean {
	private static final long serialVersionUID = -338686454955982322L;
	/**
	 * 任务ID
	 */
	private String taskID;
	/**
	 * 任务名称
	 */
	private String taskName;
	/**
	 * 项目ID
	 */
	private String projectID;
	/**
	 * 任务部门
	 */
	private DepartmentObj assigedToDept;
	/**
	 * 授予人
	 */
	private SimpleEmployee grantor;
	private int taskSponsorID;
	private String taskSponsorName;
	private String taskScheme;
	private String taskDifficulty;
	private Date taskETADate;
	/**
	 * 任务状态
	 */
	private ProjectTaskStatus status;
	private int regularTaskID;
	private String regularTaskName;

	private List<ProjectTaskMember> members;

	public ProjectTaskObj() {

	}

	public String getTaskID() {
		return taskID;
	}

	public void setTaskID(String aTaskID) {
		taskID = aTaskID;
	}

	public String getTaskName() {
		return taskName;
	}

	public void setTaskName(String aTaskName) {
		taskName = aTaskName;
	}

	public String getProjectID() {
		return projectID;
	}

	public void setProjectID(String aProjectID) {
		projectID = aProjectID;
	}

	public int getTaskSponsorID() {
		return taskSponsorID;
	}

	public void setTaskSponsorID(int aTaskSponsorID) {
		taskSponsorID = aTaskSponsorID;
	}

	public String getTaskSponsorName() {
		return taskSponsorName;
	}

	public void setTaskSponsorName(String aTaskSponsorName) {
		taskSponsorName = aTaskSponsorName;
	}

	public String getTaskScheme() {
		return taskScheme;
	}

	public void setTaskScheme(String aTaskScheme) {
		taskScheme = aTaskScheme;
	}

	public String getTaskDifficulty() {
		return taskDifficulty;
	}

	public void setTaskDifficulty(String aTaskDifficulty) {
		taskDifficulty = aTaskDifficulty;
	}

	public Date getTaskETADate() {
		return taskETADate;
	}

	public void setTaskETADate(Date aTaskETADate) {
		taskETADate = aTaskETADate;
	}

	public int getRegularTaskID() {
		return regularTaskID;
	}

	public void setRegularTaskID(int aRegularTaskID) {
		regularTaskID = aRegularTaskID;
	}

	public String getRegularTaskName() {
		return regularTaskName;
	}

	public void setRegularTaskName(String aRegularTaskName) {
		regularTaskName = aRegularTaskName;
	}

	public DepartmentObj getAssigedToDept() {
		return assigedToDept;
	}

	public void setAssigedToDept(DepartmentObj assigedToDept) {
		this.assigedToDept = assigedToDept;
	}

	public SimpleEmployee getGrantor() {
		return grantor;
	}

	public void setGrantor(SimpleEmployee grantor) {
		this.grantor = grantor;
	}

	public ProjectTaskStatus getStatus() {
		return status;
	}

	public void setStatus(ProjectTaskStatus status) {
		this.status = status;
	}

	public List<ProjectTaskMember> getMembers() {
		return members;
	}

	public void setMembers(List<ProjectTaskMember> members) {
		this.members = members;
	}
}
