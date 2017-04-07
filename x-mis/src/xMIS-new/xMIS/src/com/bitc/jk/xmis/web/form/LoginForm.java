package com.bitc.jk.xmis.web.form;

import javax.servlet.ServletRequest;
import javax.servlet.http.HttpServletRequest;

import org.apache.struts.action.ActionError;
import org.apache.struts.action.ActionErrors;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionMapping;

import com.bitc.jk.xmis.util.WebKeys;

public class LoginForm extends ActionForm {
	private String loginName;

	private String password;

	private String forwordStr;

	private boolean remember;

	@Override
	public void reset(ActionMapping mapping, ServletRequest request) {
		this.remember = false;
	}

	public String getLoginName() {
		return loginName;
	}

	public void setLoginName(String loginName) {
		this.loginName = loginName;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public void reset(ActionMapping mapping, HttpServletRequest request) {
		this.loginName = null;
		this.password = null;
	}

	public ActionErrors validate(ActionMapping mapping,
			HttpServletRequest request) {

		ActionErrors errors = new ActionErrors();

		if (loginName == null) {
			errors.add("loginName", new ActionError("adf"));

		}
		return (errors);

	}

	public String getForwordStr() {
		return forwordStr;
	}

	public void setForwordStr(HttpServletRequest request) {
		this.forwordStr = (String) request
				.getAttribute(WebKeys.RequestKey_Original_QueryString);
		System.out.println("loginFrm------------> forwordStr =" + forwordStr);
	}

	public boolean isRemember() {
		return remember;
	}

	public void setRemember(boolean remember) {
		this.remember = remember;
	}

	public void setForwordStr(String forwordStr) {
		this.forwordStr = forwordStr;
	}

}
