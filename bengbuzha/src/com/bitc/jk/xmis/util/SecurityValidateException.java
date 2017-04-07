package com.bitc.jk.xmis.util;

public class SecurityValidateException extends Exception {

	public SecurityValidateException(String message) {
		super(message);
	}
	
	public static String securityValidateMessage = WebKeys.Ext_userValidateMsg_Message ;
	public static String securityValidateMesTitle = WebKeys.Ext_errMsg_For_Get_Store_Title;
}
