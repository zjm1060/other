package com.bitc.woix.pojo;

import java.util.HashMap;
import java.util.Map;

import net.sf.json.JSONObject;

public class AjaxResponse {
	private boolean success;
	private String message;
	private String title;
	private Map<String, Object> attributes;

	public AjaxResponse() {
	}

	public AjaxResponse(boolean success) {
		this();
		this.success = success;
	}

	public AjaxResponse(boolean success, String message, String title) {
		this();
		this.success = success;
		this.message = message;
		this.title = title;
	}

	public AjaxResponse addAttribute(String key, Object value) {
		if (attributes == null) {
			attributes = new HashMap<String, Object>();
		}
		attributes.put(key, value);
		return this;
	}

	public boolean isSuccess() {
		return success;
	}

	public void setSuccess(boolean success) {
		this.success = success;
	}

	public String getMessage() {
		return message;
	}

	public void setMessage(String message) {
		this.message = message;
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public Map<String, Object> getAttributes() {
		return attributes;
	}

	public void setAttributes(Map<String, Object> attributes) {
		this.attributes = attributes;
	}

	@Override
	public String toString() {
		return JSONObject.fromObject(this).toString();
	}
}
