//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\LoginAction.java

package com.bitc.jk.xmis.web.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.util.WebKeys;

public class LogoutAction extends DispatchAction {

	private static Logger logger = Logger.getLogger(LogoutAction.class);

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

		logger.info("---> logout");
		
		request.getSession().invalidate();
		String forwordStr = WebKeys.Forword_to_ReLogin;

		return (mapping.findForward(forwordStr));
	}

}
