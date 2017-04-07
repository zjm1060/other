package com.bitc.jk.xmis.web.response;

import java.util.HashMap;
import java.util.Map;

/**
 * Extjs ajax请求响应，通常用于Form提交响应或者一般的Ajax.request请求响应
 * 
 * @author Bom Wu
 * 
 */
public class ExtjsAjaxResponse extends JsonResponse {

	private static final long serialVersionUID = 8686723336266273137L;

	private boolean success;

	private String message;

	private String messageTitle;

	private Map<String, Object> attributes;

	public ExtjsAjaxResponse() {
		attributes = new HashMap<String, Object>();
	}

	public ExtjsAjaxResponse(boolean success, String messageTitle,
			String message) {
		this();
		this.success = success;
		this.messageTitle = messageTitle;
		this.message = message;
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

	public String getMessageTitle() {
		return messageTitle;
	}

	public void setMessageTitle(String messageTitle) {
		this.messageTitle = messageTitle;
	}

	public void addAttribute(String name, Object value) {
		this.attributes.put(name, value);
	}

	public Map<String, Object> getAttributes() {
		return attributes;
	}

	public void setAttributes(Map<String, Object> attributes) {
		this.attributes = attributes;
	}

}
