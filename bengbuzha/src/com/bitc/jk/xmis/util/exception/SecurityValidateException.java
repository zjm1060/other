package com.bitc.jk.xmis.util.exception;

import com.bitc.jk.xmis.util.WebKeys;

public class SecurityValidateException extends Exception {

	public SecurityValidateException(String message) {
		super(message);

		this.setSecurityValidateMessage(WebKeys.Ext_userValidateMsg_Message
				.replaceFirst("该资源", " [" + message + "] 功能"));

	}

	// "您无权限访问该资源！如有疑问，请联系系统管理员。"

	public String securityValidateMessage; // =
											// WebKeys.Ext_userValidateMsg_Message
											// ;

	public static String securityValidateMesTitle = WebKeys.Ext_errMsg_For_Get_Store_Title;

	public String getSecurityValidateMessage() {
		return securityValidateMessage;
	}

	public void setSecurityValidateMessage(String securityValidateMessage) {
		this.securityValidateMessage = securityValidateMessage;
	}
}
