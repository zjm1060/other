package com.bitc.jk.xmis.web.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;

public class BaseAction extends DispatchAction {

	private static Logger logger = Logger.getLogger(BaseAction.class);

	public ActionForward forward(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		return mapping.findForward(Type.getString(request
				.getAttribute(WebKeys.Forword_Param_Name)));

	}
}
