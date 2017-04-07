/**
 * 
 */
package com.bitc.jk.xmis.web.response;

/**
 * @author Ted Li
 * 
 * 2011-5-26
 */
public class MessageResponse extends ExtjsAjaxResponse {

	/**
	 * 
	 */
	private static final long serialVersionUID = -1102927321235873195L;

	private boolean showMessage;

	public MessageResponse() {
		super();
		this.setSuccess(false);
		showMessage = true;
	}

	public MessageResponse(String msgTitle, String message) {
		this();
		this.setMessageTitle(msgTitle);
		this.setMessage(message);
	}

	public boolean isShowMessage() {
		return showMessage;
	}

	public void setShowMessage(boolean showMessage) {
		this.showMessage = showMessage;
	}

}
