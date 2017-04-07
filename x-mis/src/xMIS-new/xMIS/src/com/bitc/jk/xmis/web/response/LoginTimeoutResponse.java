package com.bitc.jk.xmis.web.response;

/**
 * 对于Ajax异步请求，当Session失效后在响应中返回此Json
 * 
 * @author Bom Wu
 * 
 */
public class LoginTimeoutResponse extends ExtjsAjaxResponse {

	private static final long serialVersionUID = -1568119325243658749L;

	/**
	 * 是否超时，始终为true
	 */
	private boolean timeout;

	/**
	 * 重定向地址
	 */
	private String redirectUri;

	public LoginTimeoutResponse() {
		super();
		this.setSuccess(false);
		timeout = true;
	}

	public LoginTimeoutResponse(String redirectUri) {
		this();
		this.redirectUri = redirectUri;
	}

	public boolean isTimeout() {
		return timeout;
	}

	public void setTimeout(boolean timeout) {
	}

	public String getRedirectUri() {
		return redirectUri;
	}

	public void setRedirectUri(String redirectUri) {
		this.redirectUri = redirectUri;
	}

}
