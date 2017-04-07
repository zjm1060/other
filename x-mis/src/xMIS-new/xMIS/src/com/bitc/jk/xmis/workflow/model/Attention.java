package com.bitc.jk.xmis.workflow.model;

import java.io.Serializable;
import java.util.Date;

public class Attention implements Serializable {

	private static final long serialVersionUID = -2331129945441874956L;

	private String id;
	/**
	 * 模块名，用于区别待办的模块
	 */
	private String model;
	/**
	 * 应用ID，用于各模块来标识待办，便于以后删除
	 */
	private String appId;
	private String text;
	private String url;
	private Date createTime;
	private String userID;

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getModel() {
		return model;
	}

	public void setModel(String model) {
		this.model = model;
	}

	public String getAppId() {
		return appId;
	}

	public void setAppId(String appId) {
		this.appId = appId;
	}

	public String getText() {
		return text;
	}

	public void setText(String text) {
		this.text = text;
	}

	public String getUrl() {
		return url;
	}

	public void setUrl(String url) {
		this.url = url;
	}

	public Date getCreateTime() {
		return createTime;
	}

	public void setCreateTime(Date createTime) {
		this.createTime = createTime;
	}

	public String getUserID() {
		return userID;
	}

	public void setUserID(String userID) {
		this.userID = userID;
	}
}
