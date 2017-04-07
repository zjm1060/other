package com.bitc.jk.xmis.workflow.model;

import java.io.Serializable;

public class ProcessWithKeyword  implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public String id;
	public String processInstanceId;
	public String keyword;
	public String getId() {
		return id;
	}
	public String getProcessInstanceId() {
		return processInstanceId;
	}
	public void setProcessInstanceId(String processInstanceId) {
		this.processInstanceId = processInstanceId;
	}
	public String getKeyword() {
		return keyword;
	}
	public void setKeyword(String keyword) {
		this.keyword = keyword;
	}
	public void setId(String id) {
		this.id = id;
	}
	
}
