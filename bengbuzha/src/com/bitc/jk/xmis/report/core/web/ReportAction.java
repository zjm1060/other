package com.bitc.jk.xmis.report.core.web;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.report.core.ReportException;
import com.bitc.jk.xmis.report.core.ReportService;
import com.bitc.jk.xmis.report.model.GridData;
import com.bitc.jk.xmis.report.model.GridMetaData;
import com.bitc.jk.xmis.report.model.Report;
import com.bitc.jk.xmis.util.EscapeUtils;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;

public class ReportAction extends DispatchAction {

	private ReportService reportService;

	public ActionForward getRegisterDB(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String store = reportService.getDBConnectsStore();
		response.getWriter().write(store);
		return null;
	}

	public ActionForward getGridMeta(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String token = request.getParameter("reportToken");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			Report report = reportService.getReportByName(token);
			GridMetaData data = report.buildGridMetaData();
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("meta", data);
		} catch (ReportException e) {
			Logger.getLogger(ReportAction.class).error(e.getMessage());
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage(e.getMessage());
			ajaxResponse.setMessageTitle("错误");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
		return null;
	}

	public ActionForward newReport(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String jsonStr;
		try {
			Report report = new Report();
			BeanUtils.populate(report, request.getParameterMap());
			String reportId = reportService.doInitSQLReport(report);
			ExtjsAjaxResponse formSubmitResponse = new ExtjsAjaxResponse(true,
					"成功", "处理成功");
			formSubmitResponse.addAttribute("reportId", reportId);
			formSubmitResponse.addAttribute("reportName", report.getName());
			jsonStr = formSubmitResponse.toJsonString();

		} catch (Exception e) {
			ExtjsAjaxResponse formSubmitResponse = new ExtjsAjaxResponse(true,
					"失败", "SQL处理失败<br/>" + e.getMessage());
			jsonStr = formSubmitResponse.toJsonString();
		}
		response.getWriter().write(jsonStr);
		return null;
	}

	public ActionForward gridReport(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String reportName = request.getParameter("reportToken");
		if (StringUtils.isEmpty(reportName)) {
			return null;
		}
		request.setAttribute("reportToken", reportName);
		return mapping.findForward("gridReport");

	}

	@Override
	protected ActionForward unspecified(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		return gridReport(mapping, form, request, response);
	}

	public ActionForward getGridData(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {

		String token = request.getParameter("reportToken");

		try {
			Report report = reportService.getReportByName(token);
			GridData data = report.selectGridData();
			EscapeUtils.escape(response.getWriter(), data.buildJsonString());
		} catch (ReportException e) {
			Logger.getLogger(ReportAction.class).error(e.getMessage());
			ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse(false,
					"加载失败", e.getMessage());
			WebUtil.returnResponse(response, ajaxResponse.toString());
		}
		return null;
	}

	public ReportService getReportService() {
		return reportService;
	}

	public void setReportService(ReportService reportService) {
		this.reportService = reportService;
	}

	public static void main(String[] args) throws Exception {
		GridMetaData data = new GridMetaData();
		System.out.println(data.buildJsonString());

	}
}
