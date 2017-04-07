package com.bitc.jk.xmis.web;

import javax.servlet.ServletContext;
import javax.servlet.http.HttpSession;
import javax.servlet.http.HttpSessionEvent;
import javax.servlet.http.HttpSessionListener;

import org.apache.log4j.Logger;
import org.springframework.context.ApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.sys.SysLog;
import com.bitc.jk.xmis.service.LoginService;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

/**
 * Application Lifecycle Listener implementation class SessionListener
 * 
 */
public class SessionListener implements HttpSessionListener {

	public SessionListener() {
	}

	/**
	 * @see HttpSessionListener#sessionCreated(HttpSessionEvent)
	 */
	public void sessionCreated(HttpSessionEvent event) {
	}

	/**
	 * @see HttpSessionListener#sessionDestroyed(HttpSessionEvent)
	 */
	public void sessionDestroyed(HttpSessionEvent event) {
		HttpSession session = event.getSession();
		EmployeeObj emp = WebUtil.getCurrentEmployee(session);
		if (emp == null) {
			return;
		}
		ServletContext servletContext = session.getServletContext();
		ApplicationContext context = WebApplicationContextUtils
				.getWebApplicationContext(servletContext);
		LoginService service = (LoginService) context
				.getBean("loginServiceImpl");
		String userID = String.valueOf(emp.getUserID());
		String opIP = (String) session
				.getAttribute(WebKeys.Session_Attribute_Name_LogoutIP);
		SysLog sysLog = new SysLog(userID, null, opIP);
		if (opIP == null) {
			sysLog.setOpType(SysLog.OP_TYPE_TIMEOUT);
		} else {
			sysLog.setOpType(SysLog.OP_TYPE_LOGOUT);
		}
		try {
			service.doSaveSysLog(sysLog);
		} catch (Exception e) {
			Logger.getLogger(this.getClass()).error(e.getMessage());
		}
		Integer count = (Integer) servletContext
				.getAttribute(WebKeys.Session_Application_Attr_CountOfOnline);
		if (count != null) {
			synchronized (count) {
				count = count - 1;
			}
		}
	}

}
