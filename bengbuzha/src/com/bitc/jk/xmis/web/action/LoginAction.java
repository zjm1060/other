//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\LoginAction.java

package com.bitc.jk.xmis.web.action;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.Calendar;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import net.sf.json.JSONObject;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.sys.UserHabit;
import com.bitc.jk.xmis.service.LoginService;
import com.bitc.jk.xmis.service.OrganizationService;
import com.bitc.jk.xmis.util.CryptUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UserHabitUtil;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.exception.TimeOutException;
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
	
	public ActionForward updatePwd(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String message;
		try {
				if (organizationService.doChangePassword(Integer
						.parseInt(request.getParameter("userid")), request
						.getParameter("pwd"), Integer
						.parseInt(request.getParameter("userid"))) > 0){
					message = "密码保存成功！";
				}
				else
					message = "密码保存异常，请重新操作或联系管理员，谢谢！";

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
			message = "密码保存失败，请重新登录后再操作，谢谢！";
		}
		try {
			response.setContentType("text/xml; charset=UTF-8");
			response.setHeader("Cache-Control", "no-cache");
			PrintWriter out = response.getWriter();
			out.append(message);
			out.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	/**
	 * @return ActionForward
	 * @throws IOException 
	 * @roseuid 4C199C8D0157
	 */
	public ActionForward login(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) throws IOException {

		String uri = request.getParameter("uri");
		String queryStr = request.getParameter("queryStr");
		String forwordStr;
		if (uri == null || "".equals(uri) || "null".equals(uri)) {
			forwordStr = "";
		} else {
			forwordStr = uri;
			if (queryStr == null || "".equals(queryStr)
					|| "null".equals(queryStr)) {
			} else {
				forwordStr = forwordStr + "?" + queryStr;
			}
		}
		logger.info(forwordStr + "uri=" + request.getParameter("uri")
				+ "queryStr=" + request.getParameter("queryStr"));
		try {
			LoginForm loginForm = (LoginForm) form;
			String loginName = loginForm.getLoginName();
			String password = loginForm.getPassword();
			if ("".equals(loginName)) {
				forwordStr = WebKeys.Forword_to_LoginFailed;
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed, "请填写用户名！");
			} else if ("".equals(password)) {
				forwordStr = WebKeys.Forword_to_LoginFailed;
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed, "请填写口令！");
				loginForm.setLoginName(loginName);
			} else {
				EmployeeObj userObj = loginService.doUserValidete(loginName,
						password, request);
				if (userObj.isLocked()) {
					forwordStr = WebKeys.Forword_to_LoginFailed;
					request.setAttribute(
							WebKeys.Request_Attribute_Name_LoginFailed,
							"您的账户被锁定，请联系系统管理员");
				} else {
					if("000000".equals(password.trim())){
						forwordStr = WebKeys.Forword_to_LoginModifyPassword;
						request.setAttribute("uri", uri);
						request.setAttribute("queryStr", queryStr);
						request.setAttribute("userID", userObj.getUserID()+"");
						request.setAttribute("userName", userObj.getUserName());
						request.getSession().removeAttribute(WebKeys.Session_Attribute_Name_UserDNA);
						forwordStr = "".equals(forwordStr) ? WebKeys.Forword_to_LoginSuccessfully
								: forwordStr;
						if (loginForm.isRemember()) {
							saveLoginStateCookie(loginName, password, response);
						}
					}else{
						forwordStr = "".equals(forwordStr) ? WebKeys.Forword_to_LoginSuccessfully
								: forwordStr;
						if (loginForm.isRemember()) {
							saveLoginStateCookie(loginName, password, response);
						}
					}
				}
			}

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_LoginFailed_Value);
			forwordStr = WebKeys.Forword_to_LoginFailed;
		}

		logger.info(forwordStr);
		ActionForward gotourl;

		if(Type.GetInt(request.getParameter("isApp"))== 1 && WebKeys.Forword_to_LoginSuccessfully.equals(forwordStr)){
			EmployeeObj emp = (EmployeeObj) request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			JSONObject json=new JSONObject();
			json.element("userID", emp.getUserID());
			json.element("userName", emp.getUserName());
			json.element("departmentID", emp.getDepartmentID());
			json.element("departmentName", emp.getDepartmentName());
			json.element("isApplyPerson",false);
			//json.element("roles", emp.getRoles());
			response.setContentType("application/json;charset=UTF-8");
			response.getWriter().write(json.toString());
			return null;
		}else{
			
			if (WebKeys.Forword_to_LoginSuccessfully.equals(forwordStr)) {
				return mapping.findForward(WebKeys.Forword_to_LoginSuccessfully);

			} else if (WebKeys.Forword_to_LoginFailed.equals(forwordStr)) {
				return mapping.findForward(WebKeys.Forword_to_LoginFailed);
			} else if (WebKeys.Forword_to_LoginModifyPassword.equals(forwordStr)){
				
				return mapping.findForward(WebKeys.Forword_to_LoginModifyPassword);
			} else {
				gotourl = new ActionForward(forwordStr, true);
				return gotourl;
			}
		}
	}

	private void saveLoginStateCookie(String loginName, String password,
			HttpServletResponse response) {
		String loginstate;
		try {
			loginstate = CryptUtil.encryptBase64(loginName + "|" + password);
			Cookie cookie = new Cookie(WebKeys.Session_Attribute_Name_UserDNA,
					loginstate);
			long now = Calendar.getInstance().getTimeInMillis();
			Calendar cal = Calendar.getInstance();
			cal.setTimeInMillis(now);
			cal.set(Calendar.HOUR_OF_DAY, 23);
			cal.set(Calendar.MINUTE, 59);
			cal.set(Calendar.SECOND, 59);
			cal.set(Calendar.MILLISECOND, 999);
			long l = cal.getTimeInMillis();
			cookie.setMaxAge((int) ((l - now) / 1000));
			response.addCookie(cookie);
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}

	}
	
	public void saveUH(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String messages = "用户习惯保存成功！";
		String messageTitle = "成功";
		boolean flag = true;
		String jsonStr = "";
		try {
			HttpSession session = request.getSession();
			EmployeeObj employee = WebUtil.getCurrentEmployee(session);
			loginService.doSaveUserHabit(request, employee);
		} catch (Exception e) {
			messages = "用户习惯保存失败！";
			messageTitle = "失败";
			flag = false;
		}finally{
			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
			+ messages + "',messageTitle:'" + messageTitle + "'}";
			WebUtil.returnResponse(response, jsonStr);
		}
		
	}

	public ActionForward saveUserHabit(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String state = request.getParameter("extjsstate");
		EmployeeObj user = WebUtil.getCurrentEmployee(request.getSession());
		UserHabit habit = UserHabitUtil.buildUserHabit(state, user.getUserID());
		request.getSession().setAttribute(
				WebKeys.Session_Attribute_Name_UserHabit, habit);
		return null;
	}

	public ActionForward getUserHabit(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		UserHabit habit = (UserHabit) request.getSession().getAttribute(
				WebKeys.Session_Attribute_Name_UserHabit);
		String habitString;
		if (habit != null) {
			try {
				habitString = UserHabitUtil.getHabitString(habit);
			} catch (Exception e) {
				logger.error(e.getMessage());
				habitString = "{}";
			}
		} else {
			habitString = "{}";
		}
		WebUtil.returnResponse(response, habitString);
		return null;
	}

	public ActionForward logout(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> logout");
		HttpSession session = request.getSession();
		EmployeeObj employeeObj = WebUtil.getCurrentEmployee(session);
		loginService.doSaveUserHabit(request, employeeObj);
		session.setMaxInactiveInterval(1);
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
			HttpServletRequest request, HttpServletResponse response) {

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
			WebUtil.returnResponse(
					response,
					"{'success':'false','messageTitle':'提示','message':'"
							+ te.getMessage() + "'}");

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}

	}
	
	public String getRemortIP(HttpServletRequest request) { 
	    if (request.getHeader("x-forwarded-for") == null) { 
	        return request.getRemoteAddr(); 
	    } 
	    return request.getHeader("x-forwarded-for"); 
	} 
	
	public String getMACAddress(String ip){ 
        String str = ""; 
        String macAddress = ""; 
        try { 
            Process p = Runtime.getRuntime().exec("nbtstat -A " + ip); 
            InputStreamReader ir = new InputStreamReader(p.getInputStream()); 
            LineNumberReader input = new LineNumberReader(ir); 
            for (int i = 1; i < 100; i++) { 
                str = input.readLine(); 
                if (str != null) { 
                    if (str.indexOf("MAC Address") > 1) { 
                        macAddress = str.substring(str.indexOf("MAC Address") + 14, str.length()); 
                        break; 
                    } 
                } 
            } 
        } catch (IOException e) { 
            e.printStackTrace(System.out); 
        } 
        return macAddress; 
    }
	
}
