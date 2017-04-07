package com.bitc.jk.xmis.workflow.model;

import java.io.Serializable;
import java.util.Date;

import com.bitc.jk.xmis.model.SimpleEmployee;

public class AuthorizeInfo  implements Serializable{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	public String id;
	public String processId;
	public SimpleEmployee authorizee;//被授权人
	public SimpleEmployee authorizer;//授权人
	public Date createDate;
	public Date availableDate;
	public Date cancelDate;
	public String result;
	public String comment;
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getProcessId() {
		return processId;
	}
	public void setProcessId(String processId) {
		this.processId = processId;
	}
	public SimpleEmployee getAuthorizee() {
		return authorizee;
	}
	public void setAuthorizee(SimpleEmployee authorizee) {
		this.authorizee = authorizee;
	}
	public SimpleEmployee getAuthorizer() {
		return authorizer;
	}
	public void setAuthorizer(SimpleEmployee authorizer) {
		this.authorizer = authorizer;
	}
	public Date getCreateDate() {
		return createDate;
	}
	public void setCreateDate(Date createDate) {
		this.createDate = createDate;
	}
	public Date getAvailableDate() {
		return availableDate;
	}
	public void setAvailableDate(Date availableDate) {
		this.availableDate = availableDate;
	}
	public Date getCancelDate() {
		return cancelDate;
	}
	public void setCancelDate(Date cancelDate) {
		this.cancelDate = cancelDate;
	}
	public String getResult() {
		return result;
	}
	public void setResult(String result) {
		this.result = result;
	}
	public String getComment() {
		return comment;
	}
	public void setComment(String comment) {
		this.comment = comment;
	}
	
	
}
