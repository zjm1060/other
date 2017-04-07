//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\LoginAction.java

package com.bitc.jk.xmis.web.action;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.service.LoginService;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

public class LogoutAction extends DispatchAction {

	private static Logger logger = Logger.getLogger(LogoutAction.class);
	private LoginService loginService;

	/**
	 * @roseuid 4C1F04F902BF
	 */
	public LogoutAction() {

	}

	protected ActionForward unspecified(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {

		logger.info("---> Go to logout ! ");

		return logout(mapping, form, request, response);

	}

	public ActionForward logout(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		HttpSession session = request.getSession();
		EmployeeObj employee = WebUtil.getCurrentEmployee(session);
		logger.info("---> logout");
		try {
			//loginService.doSaveUserHabit(request, employee);
		} catch (Exception e) {
			logger.warn(e.getMessage());
		}
		String opIP = WebUtil.getIpAddr(request);
		session.setAttribute(WebKeys.Session_Attribute_Name_LogoutIP, opIP);
		session.invalidate();
		clearUserCookie(response);
		String forwordStr = WebKeys.Forword_to_ReLogin;
		return (mapping.findForward(forwordStr));
	}

	private void clearUserCookie(HttpServletResponse response) {
		Cookie cookie=new Cookie(WebKeys.Session_Attribute_Name_UserDNA, null);
		cookie.setMaxAge(0);
		response.addCookie(cookie);
	}

	public LoginService getLoginService() {
		return loginService;
	}

	public void setLoginService(LoginService loginService) {
		this.loginService = loginService;
	}

}
