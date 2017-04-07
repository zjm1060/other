package com.bitc.jk.xmis.model.sys;

import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.model.TableBean;

public class Draft extends TableBean {

	private static final long serialVersionUID = -515992450757473222L;

	private String id;
	private String model;
	private String appId;
	private String userID;
	private String text;
	private String url;

	public String getUrl() {
		return url;
	}

	public void setUrl(String url) {
		this.url = url;
	}

	private String content;

	public String getUserID() {
		return userID;
	}

	public void setUserID(String userID) {
		this.userID = userID;
	}

	public String getId() {
		return id;
	}

	public String getText() {
		return text;
	}

	public void setText(String text) {
		this.text = text;
	}

	public void setId(String id) {
		this.id = StringUtils.isEmpty(id) ? null : id;
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

	public String getContent() {
		return content;
	}

	public void setContent(String content) {
		this.content = content;
	}

}
