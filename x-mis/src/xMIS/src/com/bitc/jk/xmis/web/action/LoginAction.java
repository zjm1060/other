//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\LoginAction.java

package com.bitc.jk.xmis.web.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.service.LoginService;
import com.bitc.jk.xmis.service.OrganizationService;
import com.bitc.jk.xmis.util.TimeOutException;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.web.form.LoginForm;

public class LoginAction extends DispatchAction {

	private static Logger logger = Logger.getLogger(LoginAction.class);

	private OrganizationService organizationService;

	private LoginService loginService;

	/**
	 * @roseuid 4C1F04F902BF
	 */
	public LoginAction() {

	}

	/**
	 * Access method for the loginService property.
	 * 
	 * @return the current value of the loginService property
	 */
	public OrganizationService getOrganizationService() {
		return organizationService;
	}

	/**
	 * Sets the value of the loginService property.
	 * 
	 * @param aLoginService
	 *            the new value of the loginService property
	 */
	public void setOrganizationService(OrganizationService aLoginService) {
		organizationService = aLoginService;
	}

	protected ActionForward unspecified(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {

		logger.info("---> Go to login ! ");

		return login(mapping, form, request, response);

	}

	/**
	 * @return ActionForward
	 * @roseuid 4C199C8D0157
	 */
	public ActionForward login(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String forwordStr = "null".equals(request.getParameter("uri")) ? ""
				: "null".equals(request.getParameter("queryStr")) ? request
						.getParameter("uri") : request.getParameter("uri")
						+ "?" + request.getParameter("queryStr");

		// String forwordStr = "null".equals(request.getParameter("uri")) ?
		// "":request.getParameter("uri");

		logger.info(forwordStr + "uri=" + request.getParameter("uri")
				+ "queryStr=" + request.getParameter("queryStr"));

		// .equals(request.getParameter("forwordStr")) ? ""
		// : request.getParameter("forwordStr");
		// String gotoURL = "".equals(request.getParameter("forwordStr")) ? ""
		// : request.getParameter("forwordStr");

		try {

			// response.setContentType("text/html;charset=UTF-8");
			// request.setCharacterEncoding("UTF-8");
			// EmployeeObj userObj = (EmployeeObj) request.getSession()
			// .getAttribute("useObj");
			// List aList = organizationService.getDepartmentTree(userObj);

			String loginName = ((LoginForm) form).getLoginName();
			String password = ((LoginForm) form).getPassword();

			if ("".equals(loginName)) {
				forwordStr = WebKeys.Forword_to_LoginFailed;
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed, "请填写用户名！");
			} else if ("".equals(password)) {
				forwordStr = WebKeys.Forword_to_LoginFailed;
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed, "请填写口令！");
				((LoginForm) form).setLoginName(loginName);

			} else {

				EmployeeObj userObj = loginService.userValidete(loginName,
						password);

				if (userObj == null) {
					forwordStr = WebKeys.Forword_to_LoginFailed;
					request.setAttribute(
							WebKeys.Request_Attribute_Name_LoginFailed,
							WebKeys.Request_Attribute_Name_LoginFailed_Value);
				} else {
					forwordStr = "".equals(forwordStr) ? WebKeys.Forword_to_LoginSuccessfully
							: forwordStr;

					request.getSession().setAttribute(
							WebKeys.Session_Attribute_Name_UserDNA, userObj);
				}
			}

		} catch (Exception e) {
			e.getStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_LoginFailed_Value);
			forwordStr = WebKeys.Forword_to_LoginFailed;
		}

		logger.info(forwordStr);
		ActionForward gotourl;

		if (WebKeys.Forword_to_LoginSuccessfully.equals(forwordStr)) {

			return request.getSession().getAttribute(
					WebKeys.Session_Attribute_Name_Layout).equals(
					WebKeys.Type_Layout_Ext) ? (mapping
					.findForward(WebKeys.Forword_to_LoginSuccessfully_Ext))
					: (mapping
							.findForward(WebKeys.Forword_to_LoginSuccessfully));

			// return (mapping.findForward(forwordStr));
			// gotourl = new ActionForward("/xMIS/orgMaint.do");
			// return gotourl;
		} else {
			gotourl = new ActionForward(forwordStr);
			return gotourl;
		}
	}

	public ActionForward logout(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> logout");

		request.getSession().setMaxInactiveInterval(1);
		String forwordStr = WebKeys.Forword_to_ReLogin;

		return (mapping.findForward(forwordStr));
	}

	public LoginService getLoginService() {
		return loginService;
	}

	public void setLoginService(LoginService loginService) {
		this.loginService = loginService;
	}
	
	public void buildLeftMenu(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		
		String treeName = Type.getString(request.getParameter("treeName"));
		
		logger.info(treeName);
		
		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {
				String store = loginService.buildLeftMenu(treeName, userObj);

				logger.info(store);
				
				WebUtil.returnResponse(response, store);

			}

		} catch (TimeOutException te) {
			te.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
			WebUtil.returnResponse(response,
					"{'success':'false','messageTitle':'提示','message':'"
							+ te.getMessage() + "'}");

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
		
	}
}
