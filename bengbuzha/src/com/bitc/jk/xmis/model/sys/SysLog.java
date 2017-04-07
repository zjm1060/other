package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;
import java.sql.Date;

import com.bitc.jk.xmis.util.DateUtil;

public class SysLog implements Serializable {

	private static final long serialVersionUID = -6892951702134711405L;

	public static final String OP_TYPE_LOGIN = "login";
	public static final String OP_TYPE_LOGOUT = "logout";
	public static final String OP_TYPE_TIMEOUT = "timeout";
	public static final String OP_TYPE_BROWSE = "browse";

	private String id;
	private String userID;
	private Date opTime;
	private String opType;
	private String opUri;
	private String opIP;

	public SysLog() {
	}

	public SysLog(String userID, String opUri, String opIP) {
		super();
		this.userID = userID;
		this.opTime = DateUtil.getCurrentDateTime();
		this.opType = OP_TYPE_BROWSE;
		this.opUri = opUri;
		this.opIP = opIP;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getUserID() {
		return userID;
	}

	public void setUserID(String userID) {
		this.userID = userID;
	}

	public Date getOpTime() {
		return opTime;
	}

	public void setOpTime(Date opTime) {
		this.opTime = opTime;
	}

	public String getOpType() {
		return opType;
	}

	public void setOpType(String opType) {
		this.opType = opType;
	}

	public String getOpUri() {
		return opUri;
	}

	public void setOpUri(String opUri) {
		this.opUri = opUri;
	}

	public String getOpIP() {
		return opIP;
	}

	public void setOpIP(String opIP) {
		this.opIP = opIP;
	}
}
