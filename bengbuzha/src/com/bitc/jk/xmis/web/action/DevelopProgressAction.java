package com.bitc.jk.xmis.web.action;

import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.bitc.jk.xmis.service.DevelopProgressService;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;

public class DevelopProgressAction extends BaseAction {

	private DevelopProgressService developProgressService;

	public ActionForward getDevelopProgressList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		List list = developProgressService.getDevelopProgressList();
		List<Map<Integer, Integer>> nums = developProgressService.getCounts(list);
		request.setAttribute("resultList", list);
		request.setAttribute("allModuleMapping", nums.get(0));
		request.setAttribute("allSubModuleMapping", nums.get(1));
		request.setAttribute(WebKeys.Forword_Param_Name, "ProgressList");
		return super.forward(mapping, form, request, response);
	}
	
	public ActionForward getMonthAccounting(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		//String json = developProgressService.getMonthAccounting("2012-01-01", "2012-12-31");
		//WebUtil.returnResponse(response, json);
		request.setAttribute(WebKeys.Forword_Param_Name, "getMonthAccounting");
		return super.forward(mapping, form, request, response);
	}
	
	public void getMonthAccounting1(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String json = developProgressService.getMonthAccounting(request);
		WebUtil.returnResponse(response, json);
	}
	
	public void getWeekAccounting(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String json = developProgressService.getWeekAccounting(request);
		WebUtil.returnResponse(response, json);
	}
	
	public void getSubDeptCount(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		ExtjsAjaxResponse ajaxresponse = new ExtjsAjaxResponse();
		int result = developProgressService.getSubDeptCount(request.getParameter("deptName"));
		ajaxresponse.setSuccess(true);
		ajaxresponse.setMessage(result + "");
		WebUtil.returnResponse(response, ajaxresponse.toJsonString());
	}
	
	
	

	public DevelopProgressService getDevelopProgressService() {
		return developProgressService;
	}

	public void setDevelopProgressService(
			DevelopProgressService developProgressService) {
		this.developProgressService = developProgressService;
	}
	
}
