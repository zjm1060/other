package com.bitc.jk.xmis.model;

/**
 * @author
 * 
 */
public class UserSecrityInfo {
	/*
	 * return boolean[0] 是否允许该请求 
	 */
	private boolean allowAccessRequest;
	
	//return boolean[1] 是否对该请求记录至系统日志 
	private boolean addRequestIntoLog;
	
	//return boolean[2] 是否对该请求只读属性 
	private boolean readOnlyForRequest;
	
	//return boolean[3] 是否需要显示提示信息
	private boolean allowShowMessage;

	public boolean isAllowAccessRequest() {
		return allowAccessRequest;
	}

	public void setAllowAccessRequest(boolean allowAccessRequest) {
		this.allowAccessRequest = allowAccessRequest;
	}

	public boolean isAddRequestIntoLog() {
		return addRequestIntoLog;
	}

	public void setAddRequestIntoLog(boolean addRequestIntoLog) {
		this.addRequestIntoLog = addRequestIntoLog;
	}

	public boolean isReadOnlyForRequest() {
		return readOnlyForRequest;
	}

	public void setReadOnlyForRequest(boolean readOnlyForRequest) {
		this.readOnlyForRequest = readOnlyForRequest;
	}

	public boolean isAllowShowMessage() {
		return allowShowMessage;
	}

	public void setAllowShowMessage(boolean allowShowMessage) {
		this.allowShowMessage = allowShowMessage;
	}
	
	
}
