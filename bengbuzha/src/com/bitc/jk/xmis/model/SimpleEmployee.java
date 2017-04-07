package com.bitc.jk.xmis.model;

import java.io.Serializable;

import com.bitc.jk.xmis.util.JSONUtils;

public class SimpleEmployee implements Serializable {

	private static final long serialVersionUID = 5892722920648773954L;
	protected int userID;
	protected String userName;
	protected Boolean flag;

	public SimpleEmployee() {
		super();
	}

	public SimpleEmployee(int userID) {
		this.userID = userID;
	}

	public SimpleEmployee(int userID, String userName, Boolean flag) {
		super();
		this.userID = userID;
		this.userName = userName;
		this.flag = flag;
	}

	public int getUserID() {
		return userID;
	}

	public void setUserID(int userID) {
		this.userID = userID;
	}

	public String getUserName() {
		return userName;
	}

	public void setUserName(String userName) {
		this.userName = userName;
	}

	public Boolean getFlag() {
		return flag;
	}

	public void setFlag(Boolean flag) {
		this.flag = flag;
	}

	@Override
	public SimpleEmployee clone() throws CloneNotSupportedException {
		return new SimpleEmployee(this.userID, this.userName, this.flag);
	}
	public String toJsonString(){
		return JSONUtils.toJSON(this, true).toString();
	}
	

}
