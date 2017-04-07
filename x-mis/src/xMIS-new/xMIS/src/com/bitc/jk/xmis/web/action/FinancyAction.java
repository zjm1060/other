package com.bitc.jk.xmis.web.action;

import java.io.File;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Iterator;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.service.FinancyService;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;
import com.bitc.jk.xmis.web.response.StoreResponse;

public class FinancyAction extends BaseAction {

	private static Logger logger = Logger.getLogger(FinancyAction.class);

	private FinancyService financyService;

	public ActionForward toFinancyRunningAccountList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {

		logger.info("---> Go to toFinancyRunningAccountList ! ");
		request.setAttribute(WebKeys.Forword_Param_Name,
				"financyRunningAccountList");
		request.setAttribute("type", request.getParameter("searchType"));
		return super.forward(mapping, form, request, response);
	}

	public ActionForward toProjectBalanceStore(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {

		request.setAttribute(WebKeys.Forword_Param_Name, "projectBalanceStore");
		return super.forward(mapping, form, request, response);
	}
	
	public ActionForward toFundsRevenueAndExpenditure(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {

		request.setAttribute(WebKeys.Forword_Param_Name, "fundsRevenueAndExpenditureStore");
		return super.forward(mapping, form, request, response);
	}
	
	public ActionForward toManagementFee(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {

		request.setAttribute(WebKeys.Forword_Param_Name, "managementFeeStore");
		return super.forward(mapping, form, request, response);
	}

	public void getFinancyRunningAccountList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {

		logger.info("---> Go to getFinancyRunningAccountList");

		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		int searchWay = Type.GetInt(request.getParameter("searchWay"));
		String begDate = request.getParameter("begDate");
		String endDate = request.getParameter("endDate");
		List<StandardQuery> querys = (List<StandardQuery>) request
				.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);
		CompositParam query = CompositParam.buildAndCompositParam();
		if (searchWay == 1) { // 按输入查询
			String searchContent = request.getParameter("searchContent");
			if (searchContent != null && !searchContent.trim().equals("")) {
				String searchColumn = request.getParameter("searchColumn");
				if (searchColumn != null) {
					String[] columns = searchColumn.split(",");
					CompositParam queryOr = CompositParam
							.buildOrCompositParam();
					for (int i = 0; i < columns.length; i++) {
						queryOr.addStandardQuery(SimpleParam.buildSimpleParam(
								columns[i], searchContent, Type.STRING,
								RelationalOperator.LIKE));
					}
					query.addStandardQuery(queryOr);
				}
			}
		} else { // 按部门查询
			String deptID = request.getParameter("deptID");
			if (deptID != null && !"".equals(deptID.trim())
					&& !"-1".equals(deptID.trim())) {
				SimpleParam param = SimpleParam.buildSimpleParam(
						RelationalOperator.JDBCSQL, "deptID in (" + deptID
								+ ")", "");
				query.addStandardQuery(param);
			}

		}
		if (!"".equals(begDate.trim()))
			query.addStandardQuery(SimpleParam.buildSimpleParam("dbill_date",
					begDate, Type.STRING, RelationalOperator.GE));
		if (!"".equals(endDate.trim()))
			query.addStandardQuery(SimpleParam.buildSimpleParam("dbill_date",
					endDate, Type.STRING, RelationalOperator.LE));
		String searchType = request.getParameter("searchType");
		querys.add(query);
		String store = financyService.getFinancyIncomeRunningAccountList(
				searchType, querys, pageInfo);
		WebUtil.returnResponse(response, store);
	}

	public void getProjectBalanceDynamicColumn(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		SAXReader reader = new SAXReader();
		try {
			String path = request.getSession().getServletContext()
					.getRealPath("/")
					+ "\\projectBalanceDynamicColumn.xml";
			logger.info("projectBalanceDynamicColumnPath:" + path);
			Document doc = reader.read(new File(path));
			Element root = doc.getRootElement();

			Iterator columns = root.elementIterator();// columns

			while (columns.hasNext()) {
				Element elmt = (Element) columns.next();// column-one

				Iterator OneColumn = elmt.elementIterator();

				String columnString = "";

				while (OneColumn.hasNext()) {
					Element elm = (Element) OneColumn.next();
					Iterator column = elm.elementIterator();
					String str = "";
					while (column.hasNext()) {

						Element el = (Element) column.next();
						str = str + el.getName() + ":" + el.getText() + ",";
					}
					str = "{" + str.substring(0, str.length() - 1) + "}";
					columnString = columnString + str + ",";
				}
				columnString = "["
						+ columnString.substring(0, columnString.length() - 1)
						+ "]";
				ajaxResponse.addAttribute(elmt.getName(), columnString);
			}
			boolean bflag = financyService.getBFlagForProjectBalance(Calendar
					.getInstance().get(Calendar.YEAR) - 1);
			ajaxResponse.addAttribute("bflag", bflag);
			ajaxResponse.setSuccess(true);
		} catch (DocumentException e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("解析XML获取动态列头失败");
			ajaxResponse.setMessageTitle("失败");
			e.printStackTrace();
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public void getProjectBalanceStore(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		logger.info("-------getProjectBalance");
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String executeString = request.getParameter("executeString");
		int type = Type.GetInt(request.getParameter("type"));
		String endDate = request.getParameter("endDate");
		String deptName = request.getParameter("deptName");
		WebUtil.returnResponse(response, financyService.getProjectBalance(
				executeString, type, endDate, deptName, pageInfo));
	}

	public void getProjectDetailsForProjectBalance(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		logger.info("-------getProjectDetailsForProjectBalance");
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String executeString = request.getParameter("executeString");
		String type = request.getParameter("type");
		String startDate = request.getParameter("startDate");
		String endDate = request.getParameter("endDate");
		String param = request.getParameter("param");
		String store = financyService.getProjectDetailsForProjectBalance(
				executeString, type, startDate, endDate, param, pageInfo);
		WebUtil.returnResponse(response, store);
	}

	public void getFundsRevenueAndExpenditureStore(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String executeString = request.getParameter("executeString");
		int type = Type.GetInt(request.getParameter("type"));
		String store = financyService.getFundsRevenueAndExpenditureStore(
				executeString, type, pageInfo);
		WebUtil.returnResponse(response, store);
	}

	public void getManagementFeeStore(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String executeString = request.getParameter("executeString");
		String store = financyService.getManagementFeeStore(executeString,
				pageInfo);
		WebUtil.returnResponse(response, store);
	}

	public FinancyService getFinancyService() {
		return financyService;
	}

	public void setFinancyService(FinancyService financyService) {
		this.financyService = financyService;
	}
}
