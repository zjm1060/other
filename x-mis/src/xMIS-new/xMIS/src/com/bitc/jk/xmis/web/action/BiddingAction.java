package com.bitc.jk.xmis.web.action;

import java.util.Calendar;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.bid.BidCompany;
import com.bitc.jk.xmis.model.bid.Bidding;
import com.bitc.jk.xmis.service.BidService;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;
import com.bitc.jk.xmis.web.response.StoreResponse;

public class BiddingAction extends BaseAction {
	private BidService bidService;

	public ActionForward addBidCompany(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String id = bidService.doAddBidCompany(null);
		ajaxResponse.addAttribute("id", id);
		ajaxResponse.setSuccess(true);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward getBidCompanies(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		StoreResponse storeResponse = new StoreResponse();
		List<BidCompany> companies = bidService.getBidCompanies();
		storeResponse.setRows(companies);
		storeResponse.setSuccess(true);
		WebUtil.returnResponse(response, storeResponse.toJsonString());
		return null;
	}

	public ActionForward getBiddingDisplay(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		List<Bidding> biddings = bidService.findBiddings(query);
		List<Map<String, Object>> result = bidService.getBiddingDisplay(biddings);
		StoreResponse storeResponse = new StoreResponse();
		storeResponse.setSuccess(true);
		storeResponse.setRows(result);
		storeResponse.setTotalCount(result.size());
		WebUtil.returnResponse(response, storeResponse.toJsonString());
		return null;
	}

	public ActionForward addBidding(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String id = bidService.doAddBidding(null);
		ajaxResponse.addAttribute("id", id);
		ajaxResponse.setSuccess(true);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward deleteBidding(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String id = request.getParameter("id");
		bidService.doDeleteBidding(id);
		ajaxResponse.addAttribute("id", id);
		ajaxResponse.setSuccess(true);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward updateObject(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String clazz = request.getParameter("clazz");
		String id = request.getParameter("id");
		String field = request.getParameter("field");
		String value = request.getParameter("value");
		String valueType = request.getParameter("valueType");
		bidService.doUpdateObject(clazz, id, field, value, valueType);
		ajaxResponse.setSuccess(true);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward updateAssociation(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String clazz = request.getParameter("clazz");
		String id = request.getParameter("id");
		String field = request.getParameter("field");
		String value = request.getParameter("value");
		String valueType = request.getParameter("valueType");
		bidService.doUpdateAssociation(clazz, id, field, value, valueType);
		ajaxResponse.setSuccess(true);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward updateBidPrice(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String clazz = request.getParameter("clazz");
		String id = request.getParameter("id");
		String field = request.getParameter("field");
		String value = request.getParameter("value");
		String companyId = request.getParameter("company");
		bidService.doUpdateBidPrice(clazz, id, companyId, field, value);
		ajaxResponse.setSuccess(true);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public BidService getBidService() {
		return bidService;
	}

	public void setBidService(BidService bidService) {
		this.bidService = bidService;
	}
	public void getBidAnalysisList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String bidID = Type.getString(request.getParameter("bidID"));
		String query = Type.getString(request.getParameter("query"));
		SimpleEmployee employee = WebUtil.getCurrentEmployee(request
				.getSession(false));
		String result = bidService.getBidAnalysisList(pageInfo,bidID,query,employee.getUserID());
		WebUtil.returnResponse(response, result);
	}
	public ActionForward getBidAnalysisDetail(ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		
		String bidID = Type.getString(request.getParameter("bidID"));
		List result = bidService.getBidAnalysisDetail(bidID);
		StoreResponse storeResponse = new StoreResponse();
		storeResponse.setSuccess(true);
		storeResponse.setRows(result);
		storeResponse.setTotalCount(result.size());
		WebUtil.returnResponse(response, storeResponse.toJsonString());
		return null;
	}
	public void doSaveBidPlan(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String bidID = Type.getString(request.getParameter("bidID"));
		String content = Type.getString(request.getParameter("content"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			bidService.doSaveBidPlan(bidID, content);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>");
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
}
