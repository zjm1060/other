package com.bitc.jk.xmis.task;

import java.util.Date;

import com.bitc.jk.xmis.model.SimpleEmployee;


/**
 * 任务表：id、命令名字、所有参数、是否执行、执行时间、创建人、任务描述、创建时间
 *
 */
public class Task {
	
	private String id;
	
	private String commandBeanName;
	private String paramString;
	private Date executeTime;
	private int state;//未完成0  已完成1
	
	private SimpleEmployee creator;
	private String taskDesc;
	private Date createDateTime;
	
	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}
	
	public SimpleEmployee getCreator() {
		return creator;
	}

	public Date getCreateDateTime() {
		return createDateTime;
	}

	public void setCreateDateTime(Date createDateTime) {
		this.createDateTime = createDateTime;
	}

	public void setCreator(SimpleEmployee creator) {
		this.creator = creator;
	}

	public String getTaskDesc() {
		return taskDesc;
	}

	public void setTaskDesc(String taskDesc) {
		this.taskDesc = taskDesc;
	}

	public String getParamString() {
		return paramString;
	}
	
	public void setParamString(String paramString) {
		this.paramString = paramString;
	}
	public String getCommandBeanName() {
		return commandBeanName;
	}

	public void setCommandBeanName(String commandBeanName) {
		this.commandBeanName = commandBeanName;
	}

	public Date getExecuteTime() {
		return executeTime;
	}

	public void setExecuteTime(Date executeTime) {
		this.executeTime = executeTime;
	}

	public int getState() {
		return state;
	}

	public void setState(int state) {
		this.state = state;
	}

}
