package com.bitc.jk.xmis.workflow.web;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

public class WorkItemParam implements Serializable {
	private static final long serialVersionUID = -5086156481821302032L;
	private String step;
	private String workItemId;
	private String comments;
	private Map<String, Object> variables;

	public WorkItemParam() {
		variables = new HashMap<String, Object>();
	}

	public void addVariable(String key, Object value) {
		variables.put(key, value);
	}

	public Object getVariable(String key) {
		return variables.get(key);
	}

	public String getStep() {
		return step;
	}

	public void setStep(String step) {
		this.step = step;
	}

	public String getWorkItemId() {
		return workItemId;
	}

	public void setWorkItemId(String workItemId) {
		this.workItemId = workItemId;
	}

	public String getComments() {
		return comments;
	}

	public void setComments(String comments) {
		this.comments = comments;
	}

	public Map<String, Object> getVariables() {
		return variables;
	}

	public void setVriables(Map<String, Object> variables) {
		this.variables = variables;
	}
}
