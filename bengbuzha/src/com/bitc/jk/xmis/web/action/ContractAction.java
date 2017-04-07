//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\ContractAction.java

package com.bitc.jk.xmis.web.action;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;
import org.apache.commons.lang.math.NumberUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;

import com.bitc.jk.xmis.model.ContractInfoObj;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.DelayReason;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.EstimatedPayment;
import com.bitc.jk.xmis.model.GContractSourceObj;
import com.bitc.jk.xmis.model.InvoiceObj;
import com.bitc.jk.xmis.model.InvoiceSplitObj;
import com.bitc.jk.xmis.model.MoneySplitObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PaymentConditionObj;
import com.bitc.jk.xmis.model.PaymentDetailObj;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.SimpleDeptObj;
import com.bitc.jk.xmis.model.Stencil;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.service.ContractService;
import com.bitc.jk.xmis.service.LoginService;
import com.bitc.jk.xmis.service.SysService;
import com.bitc.jk.xmis.util.Argument;
import com.bitc.jk.xmis.util.DBSchema.Contract;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;
import com.bitc.jk.xmis.web.response.ExtjsFormLoadResponse;

public class ContractAction extends BaseAction {

	private static Logger logger = Logger.getLogger(ContractAction.class);

	private ContractService contractService;

	private SysService sysService;

	private LoginService loginService;

	/**
	 * @roseuid 4C1B2D240157
	 */
	public ContractAction() {

	}

	/**
	 * Access method for the contractService property.
	 * 
	 * @return the current value of the contractService property
	 */
	public ContractService getContractService() {
		return contractService;
	}

	/**
	 * Sets the value of the contractService property.
	 * 
	 * @param aContractService
	 *            the new value of the contractService property
	 */
	public void setContractService(ContractService aContractService) {
		contractService = aContractService;
	}

	public ActionForward searchContractList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {

		logger.info("---> Go to getContractListByType ! ");

		return getContractList(mapping, form, request, response);

	}

	protected ActionForward unspecified(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {

		logger.info("---> Go to getContractListByType ! ");

		return getContractList(mapping, form, request, response);

	}

	public ActionForward contract(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> contract ?showType="
				+ request.getParameter("showType"));

		String forwordStr = request.getSession()
				.getAttribute(WebKeys.Session_Attribute_Name_Layout)
				.equals(WebKeys.Type_Layout_Ext) ? WebKeys.Forword_to_constract_ext
				: WebKeys.Forword_to_constract;

		if (Type.getString(request.getParameter("showType")).equals("readonly")) {
			forwordStr = WebKeys.Forword_to_constractReadonly;
		} else if (Type.getString(request.getParameter("showType")).equals(
				"paymentOption")) {
			forwordStr = WebKeys.Forword_to_PaymentOption;
		}

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			logger.info(userObj.getCompanyName());
			request.setAttribute(WebKeys.Request_Attribute_Name_Type,
					WebKeys.Request_Attribute_Name_Type_Contract);
			request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
					WebKeys.Request_Attribute_Name_SubType_contract);

			ContractObj contract = null;
			if (request.getParameter("contractID") == null
					|| "".equals(request.getParameter("contractID"))) {
				contract = contractService.getContractInfoNull();
				String contractType = request.getParameter("contractType");
				if ((!forwordStr.equals(WebKeys.Forword_to_constractReadonly))
						&& WebKeys.Type_ContractType_Payout == Type
								.GetInt(contractType)) {
					forwordStr = "contractOut";
				}
				contract.setContractOpType(5);

			} else {
				contract = contractService.getContractInfoByID(new Integer(
						request.getParameter("contractID")).intValue());

				// 得到摘要模版
				// Stencil stencil = contractService.getStencilByDeptId(userObj
				// .getDepartmentID());
				Stencil stencil = contractService.getStencilByDeptId(contract
						.getDeptIDOfVendor());
				contract.setStencil(stencil);
				if ((!forwordStr.equals(WebKeys.Forword_to_constractReadonly))
						&& contract.getContractTypeID() == WebKeys.Type_ContractType_Payout) {
					forwordStr = "contractOut";
				}

				contract.setContractOpType(loginService.getContractOpType(
						userObj, contract));
			}

			request.setAttribute(WebKeys.Request_Attribute_Name_ContractObject,
					contract);

			request.setAttribute("delimiter", contract.getDelimiter());
			request.setAttribute("prefix", contract.getPrefix());
			String parentID = request.getParameter("parentID");
			if (!StringUtils.isEmpty(parentID)) {
				request.setAttribute("parentID", parentID);
				request.setAttribute("parentNo",
						request.getParameter("parentNo"));
				request.setAttribute("parentName",
						request.getParameter("parentName"));
				request.setAttribute("parentAbb",
						request.getParameter("parentAbb"));
			}

		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}

		logger.info("readonly == "
				+ request
						.getAttribute(WebKeys.Request_Attribute_Name_ReadonlyLimit));
		request.setAttribute(WebKeys.Forword_Param_Name, forwordStr);
		return super.forward(mapping, form, request, response);
	}

	// //根据userDNA,权限，判定该用户是否拥有该合同的修改权限
	// //5-完全控制，
	// private int getContractOpType(EmployeeObj userDNA, ContractObj contract){
	//
	//
	// String url = "";
	//
	// // 按合同类型采用的相应合同列表的数据权限 url为改合同的权限
	// if (contract.getContractTypeID() == 1)
	// url = "/contractMain.do?method=getContractListByType&contractTypeID=1";
	// else
	// url = "/contractMain.do?method=getContractListByType&contractTypeID=2";
	//
	//
	// Map<String, List> dataScopeListMap = loginService.getSecrityDataScope(
	// url, userDNA, false);
	//
	// logger.info(dataScopeListMap
	// .get(WebKeys.Request_Attribute_Name_DataScopeOpTypeList));
	//
	// List<QueryParam> paramList = (List) dataScopeListMap
	// .get(WebKeys.Request_Attribute_Name_DataScopeOpTypeList);
	//
	// List<StandardQuery> querys = WebUtil.buildStandardQuery(paramList);
	//
	// boolean hasFullRights = contractService.hasFullRightsOnContract(
	// contract.getContractID(), contract.getContractTypeID(), querys);
	//
	// logger.info(hasFullRights);
	//
	// return hasFullRights==true?5:0;
	//
	// }

	public ActionForward contractList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("---> contractList ");

		String forwordStr = WebKeys.Forword_to_constractList;
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			Argument arg = new Argument();
			List args = new ArrayList();
			args.add(arg);
			request.setAttribute(WebKeys.Request_Attribute_Name_Type,
					WebKeys.Request_Attribute_Name_Type_Contract);
			request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
					WebKeys.Request_Attribute_Name_SubType_contractList);

			WebUtil.returnResponse(response,
					contractService.getContractList(userObj, args));
		} catch (Exception e) {
			e.printStackTrace();
		}

		// return (mapping.findForward(forwordStr));
		request.setAttribute(WebKeys.Forword_Param_Name, forwordStr);
		return super.forward(mapping, form, request, response);

	}

	public ActionForward getContractList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		logger.info("---> getContractList //"
				+ Type.getString(request.getParameter("layout")));
		String forwordStr = request.getSession()
				.getAttribute(WebKeys.Session_Attribute_Name_Layout)
				.equals(WebKeys.Type_Layout_Ext) ? WebKeys.Forword_to_constractList_Ext
				: WebKeys.Forword_to_constractList;
		logger.info(forwordStr);
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			request.setAttribute(WebKeys.Request_Attribute_Name_Type,
					WebKeys.Request_Attribute_Name_Type_Contract);
			request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
					WebKeys.Request_Attribute_Name_SubType_contractList);
			request.setAttribute(
					WebKeys.Request_Attribute_Name_SearchType_For_ContractList,
					Type.getString(request.getParameter("searchtype")).equals(
							WebKeys.Type_Search_For_ContractList_Integration) ? WebKeys.Type_Search_For_ContractList_Integration
							: WebKeys.Type_Search_For_ContractList_Normal);
		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
		// return (mapping.findForward(forwordStr));
		request.setAttribute(WebKeys.Forword_Param_Name, forwordStr);
		return super.forward(mapping, form, request, response);
	}

	public ActionForward getContractInfo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		logger.info("---> getContractInfo //contractID = "
				+ request.getParameter("contractIDnew"));
		String forwordStr = WebKeys.Forword_to_constract;
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
					WebKeys.Request_Attribute_Name_SubType_contract);
			request.setAttribute(WebKeys.Request_Attribute_Name_ContractObject,
					contractService.getContractInfoByID(new Integer(request
							.getParameter("contractIDnew")).intValue()));

		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}

		// return (mapping.findForward(forwordStr));

		request.setAttribute(WebKeys.Forword_Param_Name, forwordStr);
		return super.forward(mapping, form, request, response);

	}

	public ActionForward paymentListEdit(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		logger.info("---> paymentListEdit ");
		String forwordStr = WebKeys.Forword_to_paymentListEdit;
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			request.setAttribute(WebKeys.Request_Attribute_Name_Type,
					WebKeys.Request_Attribute_Name_Type_Contract);
			request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
					WebKeys.Request_Attribute_Name_SubType_paymentListEdit);
		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
		// return (mapping.findForward(forwordStr));

		request.setAttribute(WebKeys.Forword_Param_Name, forwordStr);
		return super.forward(mapping, form, request, response);

	}

	public ActionForward invoiceEdit(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("---> invoiceEdit ");
		String forwordStr = WebKeys.Forword_to_invoiceEdit;
		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			request.setAttribute(WebKeys.Request_Attribute_Name_Type,
					WebKeys.Request_Attribute_Name_Type_Contract);
			request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
					WebKeys.Request_Attribute_Name_SubType_invoiceEdit);
		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
		// return (mapping.findForward(forwordStr));

		request.setAttribute(WebKeys.Forword_Param_Name, forwordStr);
		return super.forward(mapping, form, request, response);

	}

	public void getGovernmentContractSourceList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getGovernmentContractSourceList"
				+ request.getParameter("selectedID"));
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			int selectedID = Integer.parseInt(request
					.getParameter("selectedID"));
			WebUtil.returnResponse(response, contractService
					.getGovernmentContractSourceList(selectedID,
							WebUtil.generatePageInfo(request)));
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
	}

	public void saveGovernmentContractSource(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		logger.info("---> saveGovernmentContractSource //"
				+ request.getParameter("selected"));
		String jsonStr = "";
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			GContractSourceObj st = new GContractSourceObj();
			st.setContractID(Type.GetInt(request.getParameter("contractID")));
			st.setGovermentContractSourceID(Type.GetInt(request
					.getParameter("stID")));
			st.setGovermentContractSourceName(Type.getString(request
					.getParameter("stName")));
			st.setCompanyID(Type.GetInt(request.getParameter("companyID")));
			st.setDepartmentID(Type.GetInt(request.getParameter("deptID")));
			st.setAmt(Type.GetDouble(request.getParameter("amt")));
			st.setCurrencyTypeID(Type.GetInt(request
					.getParameter("currencyType")));
			st.setSpecialty(Type.getString(request.getParameter("remark")));
			st.setSelected(Type.getString(request.getParameter("selected")));

			st.setGovermentContractSourceID(contractService
					.doSaveGovernmentContractSource(st, userObj));

			String messages = "";
			String messageTitle = "";
			boolean flag = false;

			if (st.getGovermentContractSourceID() > 0) {

				messages = "纵向来源信息选择成功";
				messageTitle = "成功";
				flag = true;
			} else {
				messages = "纵向来源信息选择失败！";
				messageTitle = "失败";
			}
			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "',stID:'"
					+ Type.getString(st.getGovermentContractSourceID())
					+ "',stName:'" + st.getGovermentContractSourceName() + "'}";
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'设置失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	public void delGovernmentContractSource(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		logger.info("---> delGovernmentContractSource //"
				+ request.getParameter("stID"));
		String jsonStr = "";
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			GContractSourceObj st = new GContractSourceObj();
			st.setGovermentContractSourceID(Type.GetInt(request
					.getParameter("stID")));
			st.setGovermentContractSourceName(Type.getString(request
					.getParameter("stName")));

			String contractInfo = contractService
					.doDelGovernmentContractSource(st, userObj);

			String messages = "";
			String messageTitle = "";
			boolean flag = false;

			if ("".equals(contractInfo)) {

				messages = "横向来源信息删除成功";
				messageTitle = "成功";
				flag = true;

			} else {
				messages = "横向来源信息删除失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle
					+ "',contractInfo:'" + contractInfo + "'}";
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'删除失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	public void getContractListForParentStroe(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getContractListForParentStroe // "
				+ request.getParameter("contractTypeID") + "//"
				+ request.getParameter("searchName"));

		logger.info(request.getParameter("start") + "//"
				+ request.getParameter("limit") + "//"
				+ request.getParameter("sort") + "//"
				+ request.getParameter("dir"));
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			PageInfo pageInfo = WebUtil.generatePageInfo(request);
			String store = "";
			int contractTypeID = Type.GetInt(request
					.getParameter("contractTypeID"));
			List<StandardQuery> querys = (List<StandardQuery>) request
					.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);
			if (querys == null) {
				querys = new ArrayList<StandardQuery>();
			}

			if (Type.GetInt(request.getParameter("parentID")) == 0)
				store = contractService.getContractListForParent(
						contractTypeID, request.getParameter("searchName")
								.trim(), querys, pageInfo);
			else
				store = contractService.getParentContractListPositionByID(
						contractTypeID, Type.GetInt(request
								.getParameter("parentID")), request
								.getParameter("searchName").trim(), querys,
						pageInfo);

			WebUtil.returnResponse(response, store);

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	@SuppressWarnings("unchecked")
	public void getContractListByType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		try {
			List<QueryParam> params = WebUtil.generateQueryCondition(request,
					sysService);

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			logger.info("请求开始:" + System.currentTimeMillis());
			logger.info("---> getContractListByQuery // ");
			PageInfo pageInfo = WebUtil.generatePageInfo(request);
			logger.info(pageInfo.getStart() + "//" + pageInfo.getLimit() + "//"
					+ pageInfo.getSort() + "//" + pageInfo.getDir());
			String store = "";

			int contractTypeID = Type.GetInt(request
					.getParameter("contractTypeID"));
			int warningType = Type.GetInt(request.getParameter("warningType"));

			String searchName = request.getParameter("searchName");

			if (!Type.getString(request.getParameter("currencyGraph")).equals(
					"")) {

				logger.info("getContractListByTypeForCurrency");
				store = contractService.getContractListByTypeForCurrency(
						contractTypeID, request.getParameter("searchName")
								.trim(), Type.getString(request
								.getParameter("currencyGraph")), pageInfo,
						userObj);
			} else if (warningType != 0) {
				store = contractService.getContractListByTypeFilterWarning(
						contractTypeID, warningType, pageInfo, userObj, params);

			} else if (!StringUtils.isEmpty(searchName)) {
				searchName = searchName.trim();
				store = contractService.getContractListByType(contractTypeID,
						searchName, pageInfo);
			} else {

				// 获取权限定义
				List<StandardQuery> querys = (List<StandardQuery>) request
						.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);// 权限查询

				if (querys == null
				// || Type.GetInt(request.getParameter("contractTypeID")) ==
				// WebKeys.Type_ContractType_Payout
				) {
					querys = new ArrayList<StandardQuery>();
				}
				if (!StringUtils.isEmpty(request.getParameter("inkling"))
						&& !StringUtils.isEmpty(request
								.getParameter("inklings"))) {
					String inkling = request.getParameter("inkling");
					String inklings = request.getParameter("inklings");
					StandardQuery query = WebUtil
							.generateStandardQueryByInkling(inklings, inkling);
					querys.add(query);
				}
				List<StandardQuery> query2 = WebUtil
						.generateStandardQuerysByMulit(request, sysService);// 条件查询
				if (query2 != null) {
					// querys = query2;
					for (int i = 0; i < query2.size(); i++) {
						querys.add(query2.get(i));
					}
				}
				// if (!StringUtils.isEmpty(request.getParameter("sourceID"))) {
				// querys.add(SimpleParam.buildSimpleParam(
				// RelationalOperator.JDBCSQL, "where 合同来源类型ID = ?",
				// request.getParameter("sourceID")));
				// }
				if (!StringUtils.isEmpty(request.getParameter("sourceID"))) {
					querys.add(SimpleParam.buildSimpleParam(
							RelationalOperator.JDBCSQL,
							"where 合同来源类型ID like ?",
							request.getParameter("sourceID") + '%'));
				}
				
				String deptID = request.getParameter("deptID");
				if (!StringUtils.isEmpty(deptID)) {
					if (contractTypeID == 1) {
						if (deptID.indexOf(",") == -1) {
							querys.add(SimpleParam.buildSimpleParam(
									RelationalOperator.JDBCSQL,
									"where 乙方部门ID = ? ", deptID));
						} else {
							querys.add(SimpleParam.buildSimpleParam(
									RelationalOperator.JDBCSQL,
									" where 乙方部门ID in (" + deptID + ")", ""));
						}
					} else {
						if (deptID.indexOf(",") == -1) {
							querys.add(SimpleParam.buildSimpleParam(
									RelationalOperator.JDBCSQL,
									"where 对应收入合同部门ID = ? ", deptID));
						} else {
							querys.add(SimpleParam
									.buildSimpleParam(
											RelationalOperator.JDBCSQL,
											" where 对应收入合同部门ID in (" + deptID
													+ ")", ""));
						}
					}

				}
				
				String excuteDeptID = request.getParameter("excuteDeptID");
				if (!StringUtils.isEmpty(excuteDeptID)) {
					querys.add(SimpleParam.buildSimpleParam(
							RelationalOperator.JDBCSQL, "where execu_id = ? ",
							excuteDeptID));
				}
				
				store = contractService.getContractListByQuerys(contractTypeID,
						querys, pageInfo);
			}
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public ActionForward getContractByProjectID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String projectID = request.getParameter("projectID");
		if (StringUtils.isEmpty(projectID)) {
			throw new Exception("查询合同列表时提供的项目ID为空");
		}
		String store = contractService.getContractListByProjectID(projectID);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getOutContractListByProjectID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String projectID = request.getParameter("projectID");
		if (StringUtils.isEmpty(projectID)) {
			throw new Exception("查询合同列表时提供的项目ID为空");
		}
		String store = "";
		store = contractService.getOutContractListByProjectID(projectID);
		response.getWriter().print(store);
		return null;
	}

	public void integrationSearchPayout(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		this.integrationSearch(mapping, form, request, response);
	}

	@SuppressWarnings("unchecked")
	public void integrationSearch(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> integrationSearch // contractTypeID="
				+ request.getParameter("contractTypeID") + "//"
				+ request.getParameter("currencyGraph"));

		logger.info(request.getParameter("start") + "//"
				+ request.getParameter("limit") + "//"
				+ request.getParameter("sort") + "//"
				+ request.getParameter("dir"));

		try {

			// List<QueryParam> params = WebUtil.generateQueryCondition(request,
			// sysService);

			if (WebKeys.Type_Contract_Integration_Simple == Type.GetInt(request
					.getParameter("searchType"))) {
				this.getContractListByType(mapping, form, request, response);
				return;
			}
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			PageInfo pageInfo = WebUtil.generatePageInfo(request);
			String store = "";
			int contractTypeID = Type.GetInt(request
					.getParameter("contractTypeID"));
			int warningType = Type.GetInt(request.getParameter("warningType"));
			String begDate = Type.getString(request.getParameter("begDate"));
			String endDate = Type.getString(request.getParameter("endDate"));

			int integrationType = Type.GetInt(request
					.getParameter("searchType"));

			List<StandardQuery> querys = (List<StandardQuery>) request
					.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);
			if (querys == null
			// || contractTypeID == 2
			) {
				querys = new ArrayList<StandardQuery>();
			}
			if (!StringUtils.isEmpty(request.getParameter("inkling"))
					&& !StringUtils.isEmpty(request.getParameter("inklings"))) {
				String inkling = request.getParameter("inkling");
				String inklings = request.getParameter("inklings");
				StandardQuery query = WebUtil.generateStandardQueryByInkling(
						inklings, inkling);
				querys.add(query);
			}
			List<StandardQuery> query2 = WebUtil.generateStandardQuerysByMulit(
					request, sysService);
			if (query2 != null) {
				// querys = query2;
				for (int i = 0; i < query2.size(); i++) {
					querys.add(query2.get(i));
				}
			}
			
			if (!StringUtils.isEmpty(request.getParameter("sourceID"))) {
				querys.add(SimpleParam.buildSimpleParam(
						RelationalOperator.JDBCSQL,
						"where 合同来源类型ID like ?",
						request.getParameter("sourceID") + '%'));
			}
			
			String deptID = request.getParameter("deptID");
			if (!StringUtils.isEmpty(deptID)) {
				if (contractTypeID == 1) {
					if (deptID.indexOf(",") == -1) {
						querys.add(SimpleParam.buildSimpleParam(
								RelationalOperator.JDBCSQL,
								"where 乙方部门ID = ? ", deptID));
					} else {
						querys.add(SimpleParam.buildSimpleParam(
								RelationalOperator.JDBCSQL,
								" where 乙方部门ID in (" + deptID + ")", ""));
					}
				} else {
					if (deptID.indexOf(",") == -1) {
						querys.add(SimpleParam.buildSimpleParam(
								RelationalOperator.JDBCSQL,
								"where 对应收入合同部门ID = ? ", deptID));
					} else {
						querys.add(SimpleParam.buildSimpleParam(
								RelationalOperator.JDBCSQL,
								" where 对应收入合同部门ID in (" + deptID + ")", ""));
					}
				}
			}
			
			String excuteDeptID = request.getParameter("excuteDeptID");
			if (!StringUtils.isEmpty(excuteDeptID)) {
				querys.add(SimpleParam.buildSimpleParam(
						RelationalOperator.JDBCSQL, "where execu_id = ? ",
						excuteDeptID));
			}

			store = Type.getString(request.getParameter("currencyGraph"))
					.equals("") ? contractService.getContractListByIntegration(
					contractTypeID, begDate, endDate, integrationType,
					pageInfo, userObj, querys) : contractService
					.getContractListByIntegrationForCurrency(contractTypeID,
							begDate, endDate, integrationType, Type
									.getString(request
											.getParameter("currencyGraph")),
							Type.getString(request
									.getParameter("subIntegrationType")),
							pageInfo, userObj, querys);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
	}

	public void integrationSearchForSumPayout(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		this.integrationSearchForSum(mapping, form, request, response);
	}

	public void integrationSearchForSum(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> integrationSearchForSum ");

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			PageInfo pageInfo = WebUtil.generatePageInfo(request);
			String store = "";
			int contractTypeID = Type.GetInt(request
					.getParameter("contractTypeID"));

			int warningType = Type.GetInt(request.getParameter("warningType"));

			String begDate = Type.getString(request.getParameter("begDate"));
			String endDate = Type.getString(request.getParameter("endDate"));
			int integrationType = Type.GetInt(request
					.getParameter("searchType"));

			@SuppressWarnings("unchecked")
			List<StandardQuery> querys = (List<StandardQuery>) request
					.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);
			if (querys == null
			// || contractTypeID == 2
			) {
				querys = new ArrayList<StandardQuery>();
			}
			if (!StringUtils.isEmpty(request.getParameter("inkling"))
					&& !StringUtils.isEmpty(request.getParameter("inklings"))) {
				String inkling = request.getParameter("inkling");
				String inklings = request.getParameter("inklings");
				StandardQuery query = WebUtil.generateStandardQueryByInkling(
						inklings, inkling);
				querys.add(query);
			}

			List<StandardQuery> query2 = WebUtil.generateStandardQuerysByMulit(
					request, sysService);
			if (query2 != null) {
				// querys = query2;
				for (int i = 0; i < query2.size(); i++) {
					querys.add(query2.get(i));
				}
			}

			String deptID = request.getParameter("deptID");
			if (!StringUtils.isEmpty(deptID)) {
				if (contractTypeID == 1) {
					if (deptID.indexOf(",") == -1) {
						querys.add(SimpleParam.buildSimpleParam(
								RelationalOperator.JDBCSQL,
								"where 乙方部门ID = ? ", deptID));
					} else {
						querys.add(SimpleParam.buildSimpleParam(
								RelationalOperator.JDBCSQL,
								" where 乙方部门ID in (" + deptID + ")", ""));
					}
				} else {
					if (deptID.indexOf(",") == -1) {
						querys.add(SimpleParam.buildSimpleParam(
								RelationalOperator.JDBCSQL,
								"where 对应收入合同部门ID = ? ", deptID));
					} else {
						querys.add(SimpleParam.buildSimpleParam(
								RelationalOperator.JDBCSQL,
								" where 对应收入合同部门ID in (" + deptID + ")", ""));
					}
				}
			}
			if (Type.GetInt(request.getParameter("searchType")) == WebKeys.Type_Contract_Integration_Simple) {

				if (warningType != 0) {
					store = contractService.getContractListByIntegrationSum(
							contractTypeID, warningType,
							WebKeys.Type_Contract_Integration_Simple, pageInfo,
							userObj, querys);
				} else {
					// List<QueryParam> params = WebUtil.generateQueryCondition(
					// request, sysService);
					store = contractService.getContractListByIntegrationSum(
							contractTypeID, querys,
							WebKeys.Type_Contract_Integration_Simple, pageInfo,
							userObj);
				}
			} else {
				store = contractService.getContractListByIntegrationSum(
						contractTypeID, begDate, endDate, integrationType,
						pageInfo, userObj, querys);
			}
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	// getAttachedListStroe
	public void getAttachedListStroe(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getAttachedListStroe // "
				+ request.getParameter("contractID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			int contractID = Type.GetInt(request.getParameter("contractID"));

			String store = contractService.getAttachedListStroe(contractID,
					WebUtil.generatePageInfo(request));
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	// getAttachedListStroe
	public void getAttachedListStroeByApplicationType(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getAttachedListStroeByApplicationType // "
				+ request.getParameter("applicationType") + "=="
				+ request.getParameter("refID"));

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			int refID = Type.GetInt(request.getParameter("refID"));

			String store = contractService.getAttachedListStroe(refID,
					Type.GetInt(request.getParameter("applicationType")),
					WebUtil.generatePageInfo(request));

			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public void getCompanyListStroe(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("---> getCompanyListStroe // "
				+ request.getParameter("compID"));
		logger.info(request.getParameter("start") + "//"
				+ request.getParameter("limit") + "//"
				+ request.getParameter("sort") + "//"
				+ request.getParameter("dir"));
		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			String store = "";
			PageInfo pageInfo = WebUtil.generatePageInfo(request);
			if (Type.GetInt(request.getParameter("compID")) == 0) {
				StandardQuery query = WebUtil
						.generateStandardQueryByInkling(request);
				String businessRelation = request
						.getParameter("businessRelation");
				if (!StringUtils.isEmpty(businessRelation)) {
					query = CompositParam
							.buildAndCompositParam()
							.addStandardQuery(
									SimpleParam.buildSimpleParam(
											RelationalOperator.JDBCSQL,
											"业务关系ID & ? = 业务关系ID",
											businessRelation))
							.addStandardQuery(query);
				}
				if (query != null) {
					store = contractService.getCompanyListStroeByInkling(query,
							pageInfo);
				} else {
					store = contractService.getCompanyListStroe(
							userObj.getCompanyID(), pageInfo);
				}
			} else {
				store = contractService.getCompanyListPositionByID(
						userObj.getCompanyID(),
						Type.GetInt(request.getParameter("compID")), pageInfo);
			}
			WebUtil.returnResponse(response, store);

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public void getDeptListStroeByCompany(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getDeptListStroeByCompany // ?companyID="
				+ request.getParameter("compID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			String store = contractService.getDeptListStroeByCompany(
					Type.GetInt(request.getParameter("compID")), userObj,
					WebUtil.generatePageInfo(request));

			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public ActionForward saveContractInfo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> saveContractInfo //"
				+ request.getParameter("delivery_id"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			ContractObj con = new ContractObj();
			ProjectObj pro = new ProjectObj();

			con.setContractTypeID(Type.GetInt(request
					.getParameter("contractTypeID")));
			con.setSourceTypeID(Type.GetInt(request
					.getParameter("contractSourceType")));
			con.setGovermentContractSourceID(Type.GetInt(request
					.getParameter("gSourceTypeID")));
			con.setContractID(Type.GetInt(request.getParameter("contractID")));

			String contractCode = "";
			boolean existFlag = false;
			String jsonString = null;
			if (con.getContractID() == 0) {
				contractCode = request.getParameter("contractCode") + "/Q"
						+ request.getParameter("newContractCodeNumber");
				existFlag = contractService.existContractCode(contractCode);
				if (existFlag) {
					jsonString = contractService
							.getContractInfoByContractCode(contractCode);
					contractCode = request.getParameter("contractCode")
							+ "/Q"
							+ contractService
									.getContractCodeNumber(contractCode);// 新生成一个
				}
			} else {
				contractCode = request.getParameter("contractCodeExist");
			}
			con.setContractNo(contractCode);

			pro.setProjectID(request.getParameter("projectID"));
			pro.setProjectCode(request.getParameter("projectCode"));
			con.setProjectOBJ(pro);

			con.setContractName(request.getParameter("contractName"));
			con.setAbbreviation(request.getParameter("abb"));
			con.setContractSummary(request.getParameter("summary"));
			if (request.getParameter("stencilContent") != null
					&& !"".equals(request.getParameter("stencilContent")))
				con.setContractSummary(request.getParameter("stencilContent"));

			con.setParentID(Type.GetInt(request.getParameter("parentID")));
			// 质保期
			con.setWarrantyDefinition(request
					.getParameter("warrantyDefinition"));
			con.setWarrantyStartDate(Type.getDate(request
					.getParameter("warrantyStartDate")));
			con.setWarrantyEndDate(Type.getDate(request
					.getParameter("warrantyEndDate")));
			con.setWarrantyLimit(Type.GetFloat(request
					.getParameter("warrantyLimit")));
			con.setWarrantyUnit(Type.GetInt(request
					.getParameter("warrantyUnit")));
			// 有效期
			con.setValidityStartDate(Type.getDate(request
					.getParameter("validityStartDate")));
			con.setValidityEndDate(Type.getDate(request
					.getParameter("validityEndDate")));
			con.setValidityLimit(Type.GetFloat(request
					.getParameter("validityLimit")));
			con.setValidityUnit(Type.GetInt(request
					.getParameter("validityUnit")));

			con.setContractAmt(Type.GetDouble(request
					.getParameter("contractAmt")));
			con.setCurrencyID(Type.GetInt(request.getParameter("currencyType")));
			con.setSignatureDate(Type.getDate(request
					.getParameter("signatureDate")));
			con.setBuyerID(Type.GetInt(request.getParameter("aCompany_id")));
			con.setDeptIDOfBuyer(Type.GetInt(request.getParameter("aDept_id")));
			con.setBuyerRepresentativeID(Type.GetInt(request
					.getParameter("aEmp_id")));
			con.setVendorID(Type.GetInt(request.getParameter("bCompany_id")));
			con.setDeptIDOfVendor(Type.GetInt(request.getParameter("bDept_id")));
			con.setVendorRepresentativeID(Type.GetInt(request
					.getParameter("bEmp_id")));
			con.setRemark(request.getParameter("remark"));
			con.setDeliveryID(Type.GetInt(request.getParameter("delivery_id")));
			con.setRegisterID(userObj.getUserID());
			con.setDirectorID(Type.GetInt(request.getParameter("directorID")));

			// 最后修改人
			con.setLastModificator(userObj.getUserID());
			boolean isNew = con.getContractID() == 0;
			contractService.doSaveContractInfo(con, userObj);

			ExtjsAjaxResponse formSubmitResponse = new ExtjsAjaxResponse(true,
					"成功", "保存成功");

			formSubmitResponse.addAttribute("existflag", existFlag);
			if (existFlag) {
				formSubmitResponse.addAttribute("json", jsonString);
			}
			formSubmitResponse.addAttribute("contractCode", contractCode);

			formSubmitResponse.addAttribute("contractID", con.getContractID());
			// 检索可能关联的收入合同
			if (isNew
					&& con.getContractTypeID() == WebKeys.Type_ContractType_Payout
					&& !StringUtils.isEmpty(con.getContractNo())
					&& StringUtils.indexOf(con.getContractNo(), "/Q") != -1) {
				String relatedNo = StringUtils.substringBefore(
						con.getContractNo(), "/Q");
				ContractObj relatedCon = contractService
						.getContractInfoByNo(relatedNo);
				if (relatedCon != null) {
					JSONObject related = new JSONObject();
					related.element("contract1", relatedCon.getContractID());
					related.element("contract2", con.getContractID());
					related.element("contract2Name", relatedCon.getProjectOBJ()
							.getProjectCode());
					related.element("contractNo", con.getContractNo());

					related.element("contractNoCode", relatedNo);
					related.element("contractNoNumber", StringUtils
							.substringAfter(con.getContractNo(), "/Q"));

					related.element("relatedAmt", con.getContractAmt());
					related.element("remark", "");
					formSubmitResponse.addAttribute("related", related);
					contractService.doSaveRelatedContract(
							relatedCon.getContractID(), con.getContractID(),
							con.getContractAmt(), "", con.getContractNo(),
							userObj.getUserID());
				}

			}
			jsonStr = formSubmitResponse.toJsonString();
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'设置失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
		return null;
	}

	public ActionForward saveContractInfoNew(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> saveContractInfo //delivery_id="
				+ request.getParameter("delivery_id"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			ContractObj con = new ContractObj();
			ProjectObj pro = new ProjectObj();
			ContractInfoObj conInfoObj = new ContractInfoObj();

			con.setContractTypeID(Type.GetInt(request
					.getParameter("contractTypeID")));
			con.setSourceTypeID(Type.GetInt(request
					.getParameter("contractSourceType")));
			con.setGovermentContractSourceID(Type.GetInt(request
					.getParameter("gSourceTypeID")));
			con.setContractID(Type.GetInt(request.getParameter("contractID")));

			if (con.getContractTypeID() == 1) {
				// 合同基本信息
				conInfoObj.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				conInfoObj.setProjectResourceParentID(Type.GetInt(request
						.getParameter("projectResource")));
				conInfoObj.setProjectResourceID(Type.GetInt(request
						.getParameter("projectResourceInfoCon")));
				conInfoObj.setProjectOperationTypeParentID(Type.GetInt(request
						.getParameter("projectOperationType")));
				conInfoObj.setProjectOperationTypeID(Type.GetInt(request
						.getParameter("projectOperation")));
				conInfoObj.setProjectPropertyID(Type.GetInt(request
						.getParameter("projectProperty")));
				conInfoObj.setAccountingAgencyID(Type.GetInt(request
						.getParameter("accountingAgency")));
				if (Type.GetInt(request.getParameter("accountingAgency")) == 12) {
					conInfoObj.setExecutiveOrgID(Type.GetInt(request
							.getParameter("executiveOrgID")));
				}
				if (Type.GetInt(request.getParameter("accountingAgency")) == 13) {
					conInfoObj.setExecutiveOrgID(Type.GetInt(request
							.getParameter("bDept_id")));
				}
				if (Type.GetInt(request.getParameter("accountingAgency")) == -1) {
					conInfoObj.setExecutiveOrgID(-1);
				}
				con.setContractInfoObj(conInfoObj);

				// 按科质部金额分解
				List<MoneySplitObj> moneyList = new ArrayList<MoneySplitObj>();

				MoneySplitObj selfHard = new MoneySplitObj();// 自制硬件费
				selfHard.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				selfHard.setMoney(Type.GetDouble(request
						.getParameter("selfHard")));
				selfHard.setSplitTypeID(selfHard.selfHard);
				selfHard.setLastModificatorID(userObj.getUserID());
				moneyList.add(selfHard);

				MoneySplitObj homemadeHard = new MoneySplitObj();// 国产硬件费
				homemadeHard.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				homemadeHard.setMoney(Type.GetDouble(request
						.getParameter("homemadeHard")));
				homemadeHard.setSplitTypeID(homemadeHard.homemadeHard);
				homemadeHard.setLastModificatorID(userObj.getUserID());
				moneyList.add(homemadeHard);

				MoneySplitObj importHard = new MoneySplitObj();// 进口硬件费
				importHard.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				importHard.setMoney(Type.GetDouble(request
						.getParameter("importHard")));
				importHard.setSplitTypeID(importHard.importHard);
				importHard.setLastModificatorID(userObj.getUserID());
				moneyList.add(importHard);

				MoneySplitObj hardwarePurchase = new MoneySplitObj();// 硬件购置费
				hardwarePurchase.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				hardwarePurchase.setMoney(selfHard.getMoney()
						+ homemadeHard.getMoney() + importHard.getMoney());
				hardwarePurchase
						.setSplitTypeID(hardwarePurchase.hardwarePurCost);
				hardwarePurchase.setLastModificatorID(userObj.getUserID());
				moneyList.add(hardwarePurchase);

				MoneySplitObj homemadeSoft = new MoneySplitObj();// 国产软件费
				homemadeSoft.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				homemadeSoft.setMoney(Type.GetDouble(request
						.getParameter("homemadeSoft")));
				homemadeSoft.setSplitTypeID(homemadeSoft.homemadeSoft);
				homemadeSoft.setLastModificatorID(userObj.getUserID());
				moneyList.add(homemadeSoft);

				MoneySplitObj importSoft = new MoneySplitObj();// 进口软件费
				importSoft.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				importSoft.setMoney(Type.GetDouble(request
						.getParameter("importSoft")));
				importSoft.setSplitTypeID(importSoft.importSoft);
				importSoft.setLastModificatorID(userObj.getUserID());
				moneyList.add(importSoft);

				MoneySplitObj softwarePurchase = new MoneySplitObj();// 软件购置费
				softwarePurchase.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				softwarePurchase.setMoney(homemadeSoft.getMoney()
						+ importSoft.getMoney());
				softwarePurchase
						.setSplitTypeID(softwarePurchase.softwarePurCost);
				softwarePurchase.setLastModificatorID(userObj.getUserID());
				moneyList.add(softwarePurchase);

				MoneySplitObj softwareDevCost = new MoneySplitObj();// 软件开发费用
				softwareDevCost.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				softwareDevCost.setMoney(Type.GetDouble(request
						.getParameter("softwareCost")));
				softwareDevCost.setSplitTypeID(softwareDevCost.softwareDevCost);
				softwareDevCost.setLastModificatorID(userObj.getUserID());
				moneyList.add(softwareDevCost);

				MoneySplitObj systemDesCost = new MoneySplitObj();// 咨询设计费
				systemDesCost.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				systemDesCost.setMoney(Type.GetDouble(request
						.getParameter("systemdesignCost")));
				systemDesCost.setSplitTypeID(systemDesCost.systemDesCost);
				systemDesCost.setLastModificatorID(userObj.getUserID());
				moneyList.add(systemDesCost);

				MoneySplitObj systemIntCost = new MoneySplitObj();// 系统集成费
				systemIntCost.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				systemIntCost.setMoney(Type.GetDouble(request
						.getParameter("systemintegrateCost")));
				systemIntCost.setSplitTypeID(systemIntCost.systemIntCost);
				systemIntCost.setLastModificatorID(userObj.getUserID());
				moneyList.add(systemIntCost);

				MoneySplitObj technicalCost = new MoneySplitObj();// 运营维护费
				technicalCost.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				technicalCost.setMoney(Type.GetDouble(request
						.getParameter("technical")));
				technicalCost.setSplitTypeID(technicalCost.technicalCost);
				technicalCost.setLastModificatorID(userObj.getUserID());
				moneyList.add(technicalCost);

				MoneySplitObj softwareSerCost = new MoneySplitObj();// 软件服务费
				softwareSerCost.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				softwareSerCost.setMoney(softwareDevCost.getMoney()
						+ systemDesCost.getMoney() + systemIntCost.getMoney()
						+ technicalCost.getMoney());
				softwareSerCost.setSplitTypeID(softwareSerCost.softwareSerCost);
				softwareSerCost.setLastModificatorID(userObj.getUserID());
				moneyList.add(softwareSerCost);

				MoneySplitObj constructCost = new MoneySplitObj();// 建筑工程费
				constructCost.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				constructCost.setMoney(Type.GetDouble(request
						.getParameter("constructCost")));
				constructCost.setSplitTypeID(constructCost.constructCost);
				constructCost.setLastModificatorID(userObj.getUserID());
				moneyList.add(constructCost);

				MoneySplitObj managementCost = new MoneySplitObj();// 管理费
				managementCost.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				managementCost.setMoney(Type.GetDouble(request
						.getParameter("managementCost")));
				managementCost.setSplitTypeID(managementCost.managementCost);
				managementCost.setLastModificatorID(userObj.getUserID());
				moneyList.add(managementCost);

				MoneySplitObj transportCost = new MoneySplitObj();// 运费
				transportCost.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				transportCost.setMoney(Type.GetDouble(request
						.getParameter("transportCost")));
				transportCost.setSplitTypeID(transportCost.transportCost);
				transportCost.setLastModificatorID(userObj.getUserID());
				moneyList.add(transportCost);

				MoneySplitObj taxes = new MoneySplitObj();// 税金
				taxes.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				taxes.setMoney(Type.GetDouble(request.getParameter("taxes")));
				taxes.setSplitTypeID(taxes.taxes);
				taxes.setLastModificatorID(userObj.getUserID());
				moneyList.add(taxes);

				MoneySplitObj elseCost = new MoneySplitObj();// 其他费用
				elseCost.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				elseCost.setMoney(constructCost.getMoney()
						+ managementCost.getMoney() + transportCost.getMoney()
						+ taxes.getMoney());
				elseCost.setSplitTypeID(elseCost.elseCost);
				elseCost.setLastModificatorID(userObj.getUserID());
				moneyList.add(elseCost);
				con.setMoneyList(moneyList);

				// 按合同文本分解金额
				String moneySplitStore = request
						.getParameter("splitMoneyRecords");
				con.setMoneySplitStore(moneySplitStore);
			}

			con.setContractName(request.getParameter("contractName"));
			con.setAbbreviation(request.getParameter("abb"));
			con.setContractSummary(request.getParameter("summary"));
			if (request.getParameter("stencilContent") != null
					&& !"".equals(request.getParameter("stencilContent")))
				con.setContractSummary(request.getParameter("stencilContent"));

			con.setParentID(Type.GetInt(request.getParameter("parentID")));
			// 质保期
			con.setWarrantyDefinition(request
					.getParameter("warrantyDefinition"));
			con.setWarrantyStartDate(Type.getDate(request
					.getParameter("warrantyStartDate")));
			con.setWarrantyEndDate(Type.getDate(request
					.getParameter("warrantyEndDate")));
			con.setWarrantyLimit(Type.GetFloat(request
					.getParameter("warrantyLimit")));
			con.setWarrantyUnit(Type.GetInt(request
					.getParameter("warrantyUnit")));
			// 有效期
			con.setValidityStartDate(Type.getDate(request
					.getParameter("validityStartDate")));
			con.setValidityEndDate(Type.getDate(request
					.getParameter("validityEndDate")));
			con.setValidityLimit(Type.GetFloat(request
					.getParameter("validityLimit")));
			con.setValidityUnit(Type.GetInt(request
					.getParameter("validityUnit")));

			con.setContractAmt(Type.GetDouble(request
					.getParameter("contractAmt")));
			con.setCurrencyID(Type.GetInt(request.getParameter("currencyType")));
			con.setSignatureDate(Type.getDate(request
					.getParameter("signatureDate")));
			con.setBuyerID(Type.GetInt(request.getParameter("aCompany_id")));
			con.setDeptIDOfBuyer(Type.GetInt(request.getParameter("aDept_id")));
			con.setBuyerRepresentativeID(Type.GetInt(request
					.getParameter("aEmp_id")));
			con.setVendorID(Type.GetInt(request.getParameter("bCompany_id")));
			con.setDeptIDOfVendor(Type.GetInt(request.getParameter("bDept_id")));
			con.setVendorRepresentativeID(Type.GetInt(request
					.getParameter("bEmp_id")));
			con.setRemark(request.getParameter("remark"));
			// con.setDeliveryID(Type.GetInt(request.getParameter("delivery_id")));
			if (con.getContractTypeID() == 1) {
				con.setDeliveryID(Type.GetInt(request
						.getParameter("contact_id")));
			} else {
				con.setDeliveryID(Type.GetInt(request
						.getParameter("delivery_id")));
			}

			con.setRegisterID(userObj.getUserID());
			String directorID = request.getParameter("directorID");
			con.setDirectorID(NumberUtils.isNumber(directorID) ? Integer
					.valueOf(directorID) : null);
			// 最后修改人
			con.setLastModificator(userObj.getUserID());

			boolean isNew = con.getContractID() == 0;

			boolean existFlag = false;// 收入合同
			String jsonString = null;// 收入合同
			String projectNumber = "";// 收入合同
			String contractCode = "";// 支出合同

			String delimiter = Type.getString(request
					.getParameter("contractDelimiter"));
			String prefix = Type.getString(request
					.getParameter("contractPrefix"));
			if (con.getContractTypeID() == 1) {
				// 新建合同标记
				int newflag = 0;
				if (request.getParameter("newcontractflag") == null
						|| request.getParameter("newcontractflag") == "") {
					newflag = 1;
				}
				if (Type.GetInt(request.getParameter("newcontractflag")) == 1) {
					if (contractService.isEmpBelongDept(
							Type.GetInt(request.getParameter("bDept_id")),
							userObj.getUserID())) {
						newflag = 2;
					} else {
						newflag = 1;
					}
				}
				if (Type.GetInt(request.getParameter("newcontractflag")) == 2) {
					newflag = 2;
				}
				con.setNewflag(newflag);
				pro.setProjectID(request.getParameter("projectID"));

				if (isNew) {
					projectNumber = prefix
							+ "-"
							+ Type.getString(request
									.getParameter("projectDept"))
							+ "-"
							+ Type.getString(request
									.getParameter("projectYear"))
							+ "-"
							+ Type.getString(request
									.getParameter("projectCodeType"))
							+ "-"
							+ Type.getString(request
									.getParameter("projectNumber"));
					existFlag = contractService.existProjectCode(projectNumber);
					if (existFlag) {// 如果后三位序列码存在，获取一个
						jsonString = contractService
								.getConInfoByProjectCode(projectNumber);
						projectNumber = prefix
								+ "-"
								+ Type.getString(request
										.getParameter("projectDept"))
								+ "-"
								+ Type.getString(request
										.getParameter("projectYear"))
								+ "-"
								+ Type.getString(request
										.getParameter("projectCodeType"))
								+ "-"
								+ contractService
										.getProjectNumber(projectNumber);
					}
					pro.setExecutiveDept(new SimpleDeptObj(Type.GetInt(request
							.getParameter("bDept_id"))));
				} else {
					projectNumber = request.getParameter("projectNumberExist");
				}
				pro.setProjectCode(projectNumber);

				con.setContractNo(request.getParameter("contractCode"));

			} else {
				// 新建合同标记
				int newflag = 0;
				if (request.getParameter("newcontractflag") == null
						|| request.getParameter("newcontractflag") == "") {// contractID==0
					newflag = 1;
				} else { // contractID != 0
					newflag = Type.GetInt(request
							.getParameter("newcontractflag"));
					if (Type.GetInt(request.getParameter("newcontractflag")) == 0// 1.科质部添加的新合同，但是金额未分配完
																					// 2.原有的老合同
					) {
						if (Type.GetDouble(contractService.getAmtByConID(
								con.getContractID(), 2)) == Type.GetDouble(con
								.getContractAmt())) {
							newflag = 1;
						}
						if (Type.GetDouble(contractService.getAmtByConID(
								con.getContractID(), 2)) == Type.GetDouble(con
								.getContractAmt())
								&& (contractService.isEmpBelongDept(Type
										.GetInt(request
												.getParameter("aDept_id")),
										userObj.getUserID()) || contractService
										.isEmpDeptSpecial(Type.GetInt(request
												.getParameter("aDept_id")),
												userObj.getUserID()))) {
							newflag = 2;
						}
					}
					if (Type.GetInt(request.getParameter("newcontractflag")) == 1) {
						if (contractService.isEmpBelongDept(
								Type.GetInt(request.getParameter("aDept_id")),
								userObj.getUserID())
								|| contractService.isEmpDeptSpecial(Type
										.GetInt(request
												.getParameter("aDept_id")),
										userObj.getUserID())) {
							newflag = 2;
						}
					}
				}

				con.setNewflag(newflag);

				pro.setProjectID(request.getParameter("projectID"));
				pro.setProjectCode(request.getParameter("projectCode"));

				if (con.getContractID() == 0) {
					contractCode = request.getParameter("contractCode")
							+ delimiter
							+ request.getParameter("newContractCodeNumber");
					existFlag = contractService.existContractCode(contractCode);
					if (existFlag) {
						jsonString = contractService
								.getContractInfoByContractCode(contractCode);
						contractCode = request.getParameter("contractCode")
								+ delimiter
								+ contractService
										.getContractCodeNumber(contractCode);// 新生成一个
					}
				} else {
					contractCode = request.getParameter("contractCodeExist");
				}
				con.setContractNo(contractCode);

			}
			con.setProjectOBJ(pro);

			contractService.doSaveContractInfo(con, userObj);

			ExtjsAjaxResponse formSubmitResponse = new ExtjsAjaxResponse(true,
					"成功", "保存成功");
			formSubmitResponse.addAttribute("existflag", existFlag);
			if (existFlag) {
				formSubmitResponse.addAttribute("json", jsonString);
			}
			if (con.getContractTypeID() == 1) {
				formSubmitResponse.addAttribute("projectNumber", projectNumber);
			} else {
				formSubmitResponse.addAttribute("contractCode", contractCode);
			}
			formSubmitResponse.addAttribute("contractID", con.getContractID());
			// 检索可能关联的收入合同
			if (isNew
					&& con.getContractTypeID() == WebKeys.Type_ContractType_Payout
					&& !StringUtils.isEmpty(con.getContractNo())
					&& StringUtils.indexOf(con.getContractNo(), delimiter) != -1) {
				String relatedNo = StringUtils.substringBefore(
						con.getContractNo(), delimiter);
				ContractObj relatedCon = contractService
						.getContractInfoByNo(relatedNo);
				if (relatedCon != null) {
					JSONObject related = new JSONObject();
					related.element("contract1", relatedCon.getContractID());
					related.element("contract2", con.getContractID());
					related.element("contract2Name", relatedCon.getProjectOBJ()
							.getProjectCode());
					related.element("contractNo", con.getContractNo());

					related.element("contractNoCode", relatedNo);
					related.element("contractNoNumber", StringUtils
							.substringAfter(con.getContractNo(), delimiter));

					related.element("relatedAmt", con.getContractAmt());
					related.element("remark", "");
					formSubmitResponse.addAttribute("related", related);
					contractService.doSaveRelatedContract(
							relatedCon.getContractID(), con.getContractID(),
							con.getContractAmt(), "", con.getContractNo(),
							userObj.getUserID());

				}

			}
			jsonStr = formSubmitResponse.toJsonString();
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'设置失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
		return null;
	}

	public void getPaymentConditionList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---==========================> getPaymentConditionList"
				+ request.getParameter("contractID"));
//		Interger contractID = request.getParameter("contractID")
		try {
			if("0".equals(request.getParameter("contractID"))){
				WebUtil.returnResponse(response,null);
			}else{
				EmployeeObj userObj = (EmployeeObj) request.getSession()
				.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
				WebUtil.returnResponse(response, contractService
						.getPaymentConditionList(Type.GetInt(request
								.getParameter("contractID"))));
			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public void getPaymentConditionListByProjectID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getPaymentConditionList"
				+ request.getParameter("contractID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			WebUtil.returnResponse(response, contractService
					.getPaymentConditionListByProjectID(request
							.getParameter("projectID")));
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public void savePaymentCondition1(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("---> savePaymentCondition1");
		ExtjsAjaxResponse jsonStr = new ExtjsAjaxResponse();
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			String store = request.getParameter("records");
			int contractID = Type.GetInt(request.getParameter("contractID"));
			contractService.doSavePaymentCondition1(contractID, store, userObj);
			jsonStr.setSuccess(true);
			jsonStr.setMessage("付款计划设置成功！");
			jsonStr.setMessageTitle("成功");
		} catch (Exception e) {
			e.printStackTrace();
			jsonStr.setSuccess(false);
			jsonStr.setMessage("付款计划设置失败！");
			jsonStr.setMessageTitle("失败");
		} finally {
			WebUtil.returnResponse(response, jsonStr.toJsonString());
		}
	}

	public void savePaymentCondition(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> savePaymentCondition ?contractID=//");

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			PaymentConditionObj pay = new PaymentConditionObj();
			pay.setContractID(Type.GetInt(request.getParameter("contractID")));
			// pay.setProjectId(request.getParameter("projectId"));
			pay.setPaymentConditionID(Type.GetInt(request
					.getParameter("conditionID")));
			pay.setDescription(request.getParameter("description"));
			pay.setProjectStatusMapping(Type.GetInt(request
					.getParameter("projectStatusID")));
			logger.info("ggg" + pay.getProjectStatusMapping());
			pay.setProjectStatusName(request.getParameter("projectStatusName"));
			pay.setDelayDays(Type.GetInt(request.getParameter("delayDays")));
			pay.setPaymentPercent(Type.GetDouble(request
					.getParameter("percent")));
			// pay.setPaymentDate(Type.getDate(request.getParameter("paymentDate")));
			// //预计到款日期
			pay.setPamymentAmt(Type.GetDouble(request.getParameter("amt")));
			pay.setLastModificatorID(userObj.getUserID());
			pay.setPaymentNodeName(request.getParameter("paymentNodeName"));
			// pay.setShouldPaymentDate(Type.getDate(request.getParameter("shouldPaymentDate")));

			pay.setPaymentConditionID(contractService.doSavePaymentCondition(
					pay, userObj));

			String messages = "";
			String messageTitle = "";
			boolean flag = false;

			if (pay.getPaymentConditionID() > 0) {

				messages = "付款条件设置成功";
				messageTitle = "成功";
				flag = true;

			} else {
				messages = "付款条件设置失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "'}";

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'设置失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	public void delPaymentCondition(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> delPaymentCondition? contractID=//"
				+ request.getParameter("contractID") + ",conditionID="
				+ request.getParameter("conditionID"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			PaymentConditionObj payC = new PaymentConditionObj();

			payC.setContractID(Type.GetInt(request.getParameter("contractID")));
			payC.setPaymentConditionID(Type.GetInt(request
					.getParameter("conditionID")));

			int delFlag = contractService.doDelPaymentCondition(payC, userObj);

			String messages = "";
			String messageTitle = "";
			boolean flag = false;

			if (delFlag > 0) {

				messages = "付款条件删除成功";
				messageTitle = "成功";
				flag = true;

			} else {
				messages = "付款条件删除失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "'}";

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'删除失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	public void getGLDetailList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getGLDetailList //" + request.getParameter("sort"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			WebUtil.returnResponse(
					response,
					contractService.getGLDetailListByID(
							request.getParameter("glType"),
							request.getParameter("contractID"),
							WebUtil.generatePageInfo(request)));
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public void getPaymentList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getPaymentList //" + request.getParameter("sort"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			WebUtil.returnResponse(response, contractService
					.getPaymentListByID(
							Type.GetInt(request.getParameter("contractID")),
							WebUtil.generatePageInfo(request)));
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public void getInvoiceList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getInvoicetList" + request.getParameter("contractID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			String type = request.getParameter("type");
			if ("2".equals(type)) {
				WebUtil.returnResponse(
						response,
						contractService.getReceiptListByID(
								Type.GetInt(request.getParameter("contractID")),
								WebUtil.generatePageInfo(request)));
			} else {
				WebUtil.returnResponse(
						response,
						contractService.getInvoiceListByID(
								Type.GetInt(request.getParameter("contractID")),
								WebUtil.generatePageInfo(request)));
			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public void getDelayReasonListByContractID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getDelayReasonListByContractID"
				+ request.getParameter("contractID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			WebUtil.returnResponse(response, contractService
					.getDelayReasonListByContractID(
							Type.GetInt(request.getParameter("contractID")),
							WebUtil.generatePageInfo(request)));
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	// getChildContractListByID
	public void getChildContractListByID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getChildContractListByID"
				+ request.getParameter("parentID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			WebUtil.returnResponse(response, contractService
					.getChildContractListByID(
							Type.GetInt(request.getParameter("parentID")),
							WebUtil.generatePageInfo(request)));
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public void savePaymentDetail(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> savePaymentDetail ?contractID=//"
				+ request.getParameter("contractType"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			PaymentDetailObj pay = new PaymentDetailObj();
			pay.setContractID(Type.GetInt(request.getParameter("contractID")));
			pay.setPaymentID(Type.GetInt(request
					.getParameter("paymentDetailID")));
			pay.setPaymentType(Type.GetInt(request.getParameter("contractType")) == WebKeys.Type_ContractType_Income ? WebKeys.Type_PaymentType_Income
					: WebKeys.Type_ContractType_Payout);

			pay.setPaymentDate(Type.getDate(request
					.getParameter("paymentDetailDate")));
			pay.setPaymentAmt(Type.GetDouble(request
					.getParameter("paymentDetailAmt")));

			pay.setRemark(request.getParameter("paymentDetailRemark"));

			pay.setTransactorID(Type.GetInt(request.getParameter("transatorID")));

			boolean hasFakeDate = Type.getBoolean(request
					.getParameter("hasFakeDate"));
			if (hasFakeDate) {
				PaymentDetailObj fakePay = new PaymentDetailObj();
				fakePay.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				fakePay.setPaymentID(0);
				fakePay.setPaymentType(1);
				fakePay.setPaymentDate(Type.getDate(request
						.getParameter("fakePaymentDetailDate")));
				fakePay.setPaymentAmt(Type.GetDouble(request
						.getParameter("fakePaymentDetailAmt")));
				fakePay.setRemark(request
						.getParameter("fakePaymentDetailRemark"));
				fakePay.setTransactorID(Type.GetInt(request
						.getParameter("fakeTransatorID")));
				fakePay.setPaymentID(contractService.doSavePaymentDetail(
						fakePay, userObj));
				if (fakePay.getPaymentID() > 0) {
					pay.setPaymentID(contractService.doSavePaymentDetail(pay,
							userObj));
				}
			} else {
				logger.info("hhhhhhhh//");
				pay.setPaymentID(contractService.doSavePaymentDetail(pay,
						userObj));
			}

			String messages = pay.getPaymentType() == WebKeys.Type_ContractType_Income ? WebKeys.Message_PaymentType_Income
					: WebKeys.Message_PaymentType_Payout;
			String messageTitle = "";
			boolean flag = false;

			if (pay.getPaymentID() > 0) {

				messages += "保存成功";
				messageTitle = "成功";
				flag = true;

			} else {
				messages += "保存失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "'}";
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	public void delPaymentDetail(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> delPaymentCondition? contractID=//"
				+ request.getParameter("contractID") + ",conditionID="
				+ request.getParameter("paymentDetailID"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			PaymentDetailObj payC = new PaymentDetailObj();

			payC.setContractID(Type.GetInt(request.getParameter("contractID")));
			payC.setPaymentID(Type.GetInt(request
					.getParameter("paymentDetailID")));

			int delFlag = contractService.doDelPaymentDetail(payC, userObj);

			String messages = "";
			String messageTitle = "";
			boolean flag = false;

			if (delFlag > 0) {

				messages = "删除成功";
				messageTitle = "成功";
				flag = true;

			} else {
				messages = "删除失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "'}";
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	// invoice
	public void saveInvoiceDetail(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> saveInvoiceDetail ?contractID=//"
				+ request.getParameter("transatorID"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			String type = request.getParameter("type");
			InvoiceObj invo = new InvoiceObj();
			invo.setContractID(Type.GetInt(request.getParameter("contractID")));
			invo.setInvoiceID(Type.GetInt(request.getParameter("invoiceID")));

			invo.setInvoiceCode(Type.getString(request
					.getParameter("invoiceCode")));

			invo.setContractType(Type.GetInt(request
					.getParameter("contractType")) == WebKeys.Type_ContractType_Income ? WebKeys.Type_PaymentType_Income
					: WebKeys.Type_ContractType_Payout);

			invo.setInvoiceDate(Type.getDate(request
					.getParameter("invoiceDetailDate")));
			invo.setInvoiceAMT(Type.GetDouble(request
					.getParameter("invoiceDetailAmt")));
			invo.setInvoiecRemark(Type.getString(request
					.getParameter("invoiceRemark")));

			invo.setTransactorID(Type.GetInt(request
					.getParameter("transavtorID")));
			invo.setReceiverId(Type.GetInt(request.getParameter("receiverID")));
			invo.setMailCode(Type.getString(request.getParameter("mailCode")));
			if ("2".equals(type)) {
				invo.setInvoiceID(contractService.doSaveReceiptDetail(invo,
						userObj));
			} else {
				invo.setInvoiceID(contractService.doSaveInvoiceDetail(invo,
						userObj));
			}
			String messages = invo.getContractType() == WebKeys.Type_ContractType_Income ? WebKeys.Message_InvoiceType_Income
					: WebKeys.Message_InvoiceType_Payout;

			String messageTitle = "";
			boolean flag = false;

			if (invo.getInvoiceID() > 0) {

				messages += "保存成功";
				messageTitle = "成功";
				flag = true;

			} else {
				messages += "保存失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "'}";
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	public void delInvoiceDetail(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> delInvoiceDetail? contractID=//"
				+ request.getParameter("contractID") + ",invoiceID="
				+ request.getParameter("invoiceID"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			String type = request.getParameter("type");
			InvoiceObj invoice = new InvoiceObj();

			invoice.setContractID(Type.GetInt(request
					.getParameter("contractID")));
			invoice.setInvoiceID(Type.GetInt(request.getParameter("invoiceID")));

			int delFlag;
			if ("2".equals(type)) {
				delFlag = contractService.doDelReceiptDetail(invoice, userObj);
			} else {
				delFlag = contractService.doDelInvoiceDetail(invoice, userObj);
			}
			String messages = "";
			String messageTitle = "";
			boolean flag = false;

			if (delFlag > 0) {

				messages = "删除成功";
				messageTitle = "成功";
				flag = true;

			} else {
				messages = "删除失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "'}";
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	// warningTotalCountForSum
	public void warningTotalCountForSum(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> warningTotalCountForSum ");

		try {

			List<QueryParam> params = WebUtil.generateQueryCondition(request,
					sysService);

			logger.info(params.size());

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			PageInfo pageInfo = WebUtil.generatePageInfo(request);

			String store = "";
			int contractTypeID = Type.GetInt(request
					.getParameter("contractTypeID"));

			int warningType = Type.GetInt(request.getParameter("warningType"));

			String begDate = Type.getString(request.getParameter("begDate"));
			String endDate = Type.getString(request.getParameter("endDate"));
			int integrationType = Type.GetInt(request
					.getParameter("searchType"));

			store = contractService.getWarningTotalCountForSum(contractTypeID,
					request.getParameter("searchName").trim(), integrationType,
					begDate, endDate, pageInfo, userObj, params);

			WebUtil.returnResponse(response, store);

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public ActionForward getRelatedContractList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		int contractID = Type.getInteger(request.getParameter("contractID"))
				.intValue();
		int contractType = Type
				.getInteger(request.getParameter("contractType")).intValue();
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = contractService.getRelatedContractList(contractID,
				contractType, pageInfo);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getRelated(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		return super.execute(mapping, form, request, response);
	}

	public ActionForward saveRelatedContract(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String jsonStr;
		String demiliter = Type.getString(request
				.getParameter("relatedDelimiter"));
		try {
			EmployeeObj employeeObj = WebUtil.getCurrentEmployee(request
					.getSession());
			int contract1 = Type.GetInt(request.getParameter("contract1"));
			int contract2 = Type.GetInt(request.getParameter("contract2"));
			double amt = Type.GetDouble(request.getParameter("relatedAmt"));
			String remark = Type.getString(request.getParameter("remark"));
			// String contractNo = Type.getString(request
			// .getParameter("contractNo"));
			String contractNo = Type.getString(request
					.getParameter("contractNoCode"))
					+ demiliter
					+ Type.getString(request.getParameter("contractNoNumber"));
			contractService.doSaveRelatedContract(contract1, contract2, amt,
					remark, contractNo, employeeObj.getUserID());
			ExtjsAjaxResponse formSubmitResponse = new ExtjsAjaxResponse(true,
					"成功", "保存成功");
			jsonStr = formSubmitResponse.toJsonString();
		} catch (Exception e) {
			e.printStackTrace();
			jsonStr = "{success:false,message:'设置失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}
		WebUtil.returnResponse(response, jsonStr);
		return null;
	}

	public void getDeptCombo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		// String url = (String) request.getParameter("qxurl").substring(5);
		String url = (String) request.getParameter("qxurl");

		// 域名访问时，url里没有/xMIS/字符，ip地址访问时有，需要分别处理
		url = url.indexOf("/xMIS/") == 0 ? url.substring(5) : url;

		EmployeeObj userDNA = WebUtil.getCurrentEmployee();

		// 采用的收入合同列表的数据权限 url为合同列表的url

		Map<String, List> map = loginService.getSecrityDataScope(url, userDNA,
				false);

		logger.info(map.get(WebKeys.Request_Attribute_Name_DataScopeDeptList));

		String store = WebUtil.buildJsonStoreByList(map
				.get(WebKeys.Request_Attribute_Name_DataScopeDeptList));

		WebUtil.returnResponse(response, store);
	}

	/**
	 * 保存推迟付款原因
	 */
	public void saveDelayInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> saveDelayInfo ?contractID=//"
				+ request.getParameter("contractID"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			DelayReason dr = new DelayReason();
			dr.setId(Type.GetInt("".equals(request.getParameter("ID")) ? 0
					: request.getParameter("ID"))); // ID=0 : 新数据
			dr.setContractID(Type.GetInt(request.getParameter("contractID")));
			dr.setReason(request.getParameter("reason"));
			dr.setOperator(Type.GetInt(request.getParameter("operator")));
			dr.setRegistrant(userObj.getUserID());

			String messages = "";
			String messageTitle = "";
			boolean flag = false;

			if (contractService.doUpdateDelayInfo(dr)) {
				messages = "推迟付款原因设置成功！";
				messageTitle = "成功";
				flag = true;
			} else {
				messages = "推迟付款原因设置失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "'}";

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
			jsonStr = "{success:false,message:'设置失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	/**
	 * 删除推迟付款原因
	 */
	public void deleteDelayReasonInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> delDelayReasonInfo? ID=//"
				+ request.getParameter("ID"));

		String jsonStr = "";

		try {
			DelayReason dr = new DelayReason();
			dr.setId(Type.GetInt(request.getParameter("ID")));

			String messages = "";
			String messageTitle = "";
			boolean flag = false;

			if (contractService.doDeleteDelayReasonInfo(dr)) {

				messages = "推迟付款原因删除成功！";
				messageTitle = "成功";
				flag = true;

			} else {
				messages = "推迟付款原因删除失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "'}";

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'删除失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	/**
	 * 查询发票拆分信息
	 */
	public void getInvoiceSplitDetailBySimpleInvoice(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		try {
			InvoiceSplitObj iso = new InvoiceSplitObj();
			iso.setContractID(Type.GetInt(request.getParameter("contractID")));
			iso.setInvoiceNumber(Type.GetInt(request
					.getParameter("invoiceNumber")));
			String store = contractService
					.getInvoiceSplitDetailBySimpleInvoice(iso);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 查询发票基本拆分信息
	 */
	public void getInvoiceSplitInfoBySimpleInvoice(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		try {
			InvoiceSplitObj iso = new InvoiceSplitObj();
			iso.setContractID(Type.GetInt(request.getParameter("contractID")));
			iso.setInvoiceNumber(Type.GetInt(request
					.getParameter("invoiceNumber")));
			String store = contractService
					.getInvoiceSplitInfoBySimpleInvoice(iso);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 删除发票拆分信息
	 */
	public void deleteInvoiceSplitInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> deleteInvoiceSplitMsg? ID=//"
				+ request.getParameter("ID"));

		String jsonStr = "";

		try {
			InvoiceSplitObj iso = new InvoiceSplitObj();
			iso.setId(Type.GetInt(request.getParameter("ID")));

			String messages = "";
			String messageTitle = "";
			boolean flag = false;

			if (contractService.doDeleteInvoiceSplitInfo(iso)) {

				messages = "信息删除成功！";
				messageTitle = "成功";
				flag = true;

			} else {
				messages = "信息删除失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "'}";

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'删除失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	/**
	 * 新建发票拆分信息
	 */
	public void saveInvoiceSplitInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> saveInvoiceSplitInfo ");

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			InvoiceSplitObj iso = new InvoiceSplitObj();
			iso.setContractID(Type.GetInt(request.getParameter("contractID")));
			iso.setInvoiceNumber(Type.GetInt(request
					.getParameter("invoiceNumber")));
			iso.setAnticipatedTime(Type.getDate(request
					.getParameter("anticipatedTime")));
			iso.setSplitSum(Type.GetDouble(request.getParameter("splitSum")));
			iso.setPayFlag(Type.getBoolean(request.getParameter("payFlag")));
			iso.setRegistrant(userObj.getUserID());
			iso.setRemark(request.getParameter("remark"));

			String messages = "";
			String messageTitle = "";
			boolean flag = false;

			if (contractService.doSaveInvoiceSplitInfo(iso)) {
				messages = "发票拆分设置成功！";
				messageTitle = "成功";
				flag = true;
			} else {
				messages = "发票拆分设置失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "'}";

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
			jsonStr = "{success:false,message:'设置失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	/**
	 * 更新发票拆分信息
	 */
	public void updateInvoiceSplitInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> saveInvoiceSplitInfo ");

		String jsonStr = "";

		try {
			InvoiceSplitObj iso = new InvoiceSplitObj();
			iso.setId(Type.GetInt(request.getParameter("ID")));
			iso.setAnticipatedTime(Type.getDate(request
					.getParameter("anticipatedTime")));
			iso.setSplitSum(Type.GetDouble(request.getParameter("splitSum")));
			iso.setPayFlag(Type.getBoolean(request.getParameter("payFlag")));
			iso.setRemark(request.getParameter("remark"));

			String messages = "";
			String messageTitle = "";
			boolean flag = false;

			if (contractService.doUpdateInvoiceSplitInfo(iso)) {
				messages = "发票拆分更新成功！";
				messageTitle = "成功";
				flag = true;
			} else {
				messages = "发票拆分更新失败！";
				messageTitle = "失败";
			}

			jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
					+ messages + "',messageTitle:'" + messageTitle + "'}";

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
			jsonStr = "{success:false,message:'设置失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	/**
	 * 得到辅助查询列表
	 */
	public void getIncomeAssistList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		try {
			logger.info("---> getIncomeAssistList // ");
			PageInfo pageInfo = WebUtil.generatePageInfo(request);
			logger.info(pageInfo.getStart() + "//" + pageInfo.getLimit() + "//"
					+ pageInfo.getSort() + "//" + pageInfo.getDir());

			String store = "";
			// 验证权限
			List<StandardQuery> querys = (List<StandardQuery>) request
					.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);
			if (querys == null) {
				querys = new ArrayList<StandardQuery>();
			}
			// INKLINGS为模糊查询时传回值
			if (!StringUtils.isEmpty(request.getParameter("inkling"))
					&& !StringUtils.isEmpty(request.getParameter("inklings"))) {
				String inkling = request.getParameter("inkling");
				String inklings = request.getParameter("inklings");
				StandardQuery query = WebUtil.generateStandardQueryByInkling(
						inklings, inkling);
				querys.add(query);
			}
			List<StandardQuery> query2 = WebUtil.generateStandardQuerysByMulit(
					request, sysService);
			if (query2 != null) {
				for (int i = 0; i < query2.size(); i++) {
					querys.add(query2.get(i));
				}
			}
			store = contractService.getIncomeAssistList(querys, pageInfo);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	/**
	 * 到款辅助查询页面跳转
	 */
	public ActionForward incomeAssistSearch(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		request.setAttribute(WebKeys.Forword_Param_Name,
				WebKeys.Forword_to_IncomeAssistSearch);
		return super.forward(mapping, form, request, response);
	}

	/**
	 * 修改付款信息
	 */
	public void savePaymentDetailQuickly(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> savePaymentDetailQuickly ");

		ExtjsFormLoadResponse elfr = new ExtjsFormLoadResponse();
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			// 拆分数据
			String sourceType = null;
			EstimatedPayment ep = null;
			PaymentConditionObj pc = null;
			InvoiceObj iv = null;
			Object obj = null;
			if ("2".equals(request.getParameter("sourceType").trim())) {
				sourceType = "estimated";
				ep = new EstimatedPayment();
				ep.setId(request.getParameter("receiptID"));
				ep.setLastModifiedId(userObj.getUserID());
				obj = ep;
			}
			if ("1".equals(request.getParameter("sourceType").trim())) {
				sourceType = "paymentCondition";
				pc = new PaymentConditionObj();
				pc.setContractID(Type.GetInt(request.getParameter("contractID")));
				pc.setPaymentConditionID(Type.GetInt(request
						.getParameter("receiptID")));
				pc.setLastModificatorID(userObj.getUserID());
				obj = pc;
			}
			if ("3".equals(request.getParameter("sourceType").trim())) {
				sourceType = "invoice";
				iv = new InvoiceObj();
				iv.setContractID(Type.GetInt(request.getParameter("contractID")));
				iv.setInvoiceID(Type.GetInt(request.getParameter("receiptID")));
				iv.setLastModificatorID(userObj.getUserID());
				obj = iv;
			}
			if ("4".equals(request.getParameter("sourceType").trim())) {
				sourceType = "receipt";
				iv = new InvoiceObj();
				iv.setContractID(Type.GetInt(request.getParameter("contractID")));
				iv.setReceiverId(Type.GetInt(request.getParameter("receiptID")));
				iv.setLastModificatorID(userObj.getUserID());
				obj = iv;
			}
			// 付款数据
			PaymentDetailObj pay = new PaymentDetailObj();
			pay.setContractID(Type.GetInt(request.getParameter("contractID")));
			pay.setPaymentID(Type.GetInt(request
					.getParameter("paymentDetailID")));
			pay.setPaymentType(WebKeys.Type_PaymentType_Income);
			pay.setPaymentDate(Type.getDate(request
					.getParameter("paymentDetailDate")));
			pay.setPaymentAmt(Type.GetDouble(request
					.getParameter("paymentDetailAmt")));
			pay.setRemark(request.getParameter("paymentDetailRemark"));
			pay.setTransactorID(Type.GetInt(request.getParameter("transatorID")));
			pay.setPaymentID(contractService.doSavePaymentDetailQuickly(pay,
					userObj, sourceType, obj));

			if (pay.getPaymentID() > 0) {
				elfr.setSuccess(true);
				elfr.setMessage("保存成功");
				elfr.setMessageTitle("成功");
			} else {
				elfr.setSuccess(false);
				elfr.setMessage("保存失败！");
				elfr.setMessageTitle("失败");
			}
		} catch (Exception e) {
			e.printStackTrace();
			elfr.setSuccess(false);
			elfr.setMessage("保存失败！");
			elfr.setMessageTitle("失败");
		}

		WebUtil.returnResponse(response, elfr.toJsonString());
	}

	public void getPaymentConditionTotalAmtById(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		logger.info("---> getPaymentConditionTotalAmtById ");
		ExtjsAjaxResponse ajaxresponse = new ExtjsAjaxResponse();
		try {
			int id = Type.GetInt(request.getParameter("contractID"));
			Map<String, String> result = contractService
					.getPaymentConditionTotalAmtById(id);
			if (result != null) {
				Map valueResult = new HashMap<String, Object>();
				valueResult.put("amt", result.get("amt"));
				valueResult.put("percent", result.get("percent"));
				ajaxresponse.setAttributes(valueResult);
			} else {
				Map nullResult = new HashMap<String, Object>();
				nullResult.put("amt", 0.00);
				nullResult.put("percent", 0.00);
				ajaxresponse.setAttributes(nullResult);
			}
			ajaxresponse.setSuccess(true);
			logger.info("asasas=" + ajaxresponse.toJsonString());
		} catch (Exception e) {
			e.printStackTrace();
			ajaxresponse.setSuccess(false);
		}
		WebUtil.returnResponse(response, ajaxresponse.toJsonString());
	}

	public void getContractRelationInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getContractRelationInfo ");
		String jsonStr = "";

		try {
			int relateID = Type.GetInt(request.getParameter("relateID"));
			int pCID = Type.GetInt(request.getParameter("pContractID"));
			jsonStr = contractService.getContractRelationInfo(pCID, relateID);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	public void updateRelatedContract(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String jsonStr;
		try {
			EmployeeObj employeeObj = WebUtil.getCurrentEmployee(request
					.getSession());
			if (employeeObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new Exception("Timeout!");

			} else {
				int inContractID = Type.GetInt(request
						.getParameter("inContractID"));
				String contractNo = request.getParameter("contractNo");
				double relateAmt = Type.GetDouble(request
						.getParameter("relateAmt"));
				String remark = Type.getString(request.getParameter("remark"));
				int relatedID = Type.GetInt(request.getParameter("relatedID"));
				int outContractID = Type.GetInt(request
						.getParameter("outContractID"));
				contractService.updateRelatedContract(inContractID, contractNo,
						relateAmt, remark, relatedID, outContractID);
				ExtjsAjaxResponse formSubmitResponse = new ExtjsAjaxResponse(
						true, "成功", "保存成功");
				jsonStr = formSubmitResponse.toJsonString();
			}
		} catch (Exception e) {
			e.printStackTrace();
			jsonStr = "{success:false,message:'设置失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	public void getContractSummaryStencilByDeptID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String jsonStr;
		try {
			EmployeeObj employeeObj = WebUtil.getCurrentEmployee(request
					.getSession());
			if (employeeObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new Exception("Timeout!");
			} else {
				int deptID = Type.GetInt(request.getParameter("deptID"));

				Map<String, String> map = contractService
						.getContractSummaryByConId(deptID);
				jsonStr = "{success:true,stencilContent:'"
						+ Type.getString(map.get("stencilContent"))
						+ "',stencilKeyWords:'"
						+ Type.getString(map.get("stencilKeyWords"))
						+ "',summaryResult:'"
						+ Type.getString(map.get("summaryResult"))
						+ "',existStencil:'"
						+ Type.getString(map.get("existStencil")) + "'}";
			}
		} catch (Exception e) {
			e.printStackTrace();
			jsonStr = "{success:false,message:'设置失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	public void getParentDeptIdByDeptId(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getParentDeptIdByDeptId ");
		String jsonStr = "";

		try {
			int id = Type.GetInt(request.getParameter("deptID"));
			int deptID = contractService.getParentDeptIdByDeptId(id);
			jsonStr = "{success:true,deptID:" + deptID + "}";
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	public void getIncomAmtTotalByGl(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getIncomAmtTotalByGl ");
		String jsonStr = "";

		try {
			// double mb = contractService.getIncomAmtTotalByGl(request
			// .getParameter("proID"));
			// int conID = contractService.getContractIDByProjectID(request
			// .getParameter("proID"));
			// jsonStr = "{success:true,mb:" + mb + ",conID:" + conID + "}";
			double mb = contractService.getIncomAmtTotalByGl(request
					.getParameter("proID"));
			int conID = contractService.getContractIDByProjectID(request
					.getParameter("proID"));
			double invoiceAmt = contractService.getTotalSumByID(conID,
					WebKeys.Type_Total_Sum_Invoiced);
			jsonStr = "{success:true,mb:" + mb + ",conID:" + conID
					+ ",invoiceAmt:" + invoiceAmt + "}";
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	// 合同资质管理基本信息
	public void getContractInfoByParentID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		logger.info("getContractInfoByParentID--->parentID="
				+ request.getParameter("parentID"));
		try {
			int parentID = Type.GetInt(request.getParameter("parentID"));
			String store = contractService.getContractInfoByParentID(parentID);
			logger.info("getContractInfoByParentID" + store);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
	}

	public void getContractInfoByType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("getContractInfoByType--->type="
				+ request.getParameter("type"));
		try {
			int type = Type.GetInt(request.getParameter("type"));
			String store = contractService.getContractInfoByType(type);
			logger.info("getContractInfoByType" + store);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
	}
/**
 * 查询项目名称
 * */
	public void getProjectCodeTypeByParentID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		logger.info("getProjectCodeTypeByParentID--->parentID="
				+ request.getParameter("parentID"));
		try {
			int parentID = Type.GetInt(request.getParameter("parentID"));
			String store = contractService
					.getProjectCodeTypeByParentID(parentID);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
	}

	public void getDeptCode(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		try {
			String store = contractService.getDeptCode();
			logger.info("getDeptCode" + store);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
	}

	public void getProjectNumber(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String projectNumber = "BITC-"
					+ Type.getString(request.getParameter("projectDept")) + "-"
					+ Type.getString(request.getParameter("projectYear")) + "-"
					+ Type.getString(request.getParameter("projectCodeType"));
			logger.info(projectNumber);
			String number = contractService.getProjectNumber(projectNumber);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("number", number);
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("自动获取项目编码失败");
			ajaxResponse.setMessageTitle("失败");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	// 删除按合同文本规定金额分解
	public void deleteConTextRecord(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("deleteConTextRecord--->contractID = "
				+ Type.GetInt(request.getParameter("contractID")) + "typeID="
				+ Type.GetInt(request.getParameter("costID")));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			int contractID = Type.GetInt(request.getParameter("contractID"));
			// int typeID = Type.GetInt(request.getParameter("costID"));
			String typeIDs = Type.getString(request.getParameter("costIDs"));
			String[] typeIDArr = typeIDs.split(",");
			int typeID;
			boolean deleteSuccess = false;
			for (int i = 0; i < typeIDArr.length; i++) {
				typeID = Type.GetInt(typeIDArr[i]);
				int result = contractService.doDeleteConMoneySplit(contractID,
						typeID);
				if (result > 0) {
					deleteSuccess = true;
				} else {
					deleteSuccess = false;
					return;
				}
			}
			if (deleteSuccess) {
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.setMessage("删除成功");
			} else {
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessageTitle("失败");
				ajaxResponse.setMessage("删除失败");
			}
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除失败，请重新登录后再操作，谢谢");
			ajaxResponse.setMessageTitle("失败");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public void getConTextSplitList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("getConTextSplitList---->contractID="
				+ Type.GetInt(request.getParameter("contractID"))
				+ contractService.getConTextSplitList(
						Type.GetInt(request.getParameter("contractID")), 10000));
		try {
			WebUtil.returnResponse(response, contractService
					.getConTextSplitList(
							Type.GetInt(request.getParameter("contractID")),
							10000));
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
	}

	public void getContractStateList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		List<StandardQuery> querys = (List<StandardQuery>) request
				.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);

		if (querys == null) {
			querys = new ArrayList<StandardQuery>();
		}
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		if (query != null) {
			querys.add(query);
		}
		String store = contractService.getContractStateList(querys,
				WebUtil.generatePageInfo(request));
		WebUtil.returnResponse(response, store);

	}

	public void getPaymentPromptList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = "";
		try {
			store = contractService.getPaymentPromptList();
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void getScientificName(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int deptId = Type.GetInt(request.getParameter("deptId"));
		String store = "";
		try {
			store = contractService.getScientificName(deptId);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void getContractCodeNumber(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String contractCode = Type.getString(request
					.getParameter("contractCode"));
			logger.info("contractCode" + contractCode);
			String number = contractService.getContractCodeNumber(contractCode);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("number", number);
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("自动获取甲方合同编号失败");
			ajaxResponse.setMessageTitle("失败");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public void saveEstimatedInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> saveEstimatedInfo ");
		ExtjsAjaxResponse jsonStr = new ExtjsAjaxResponse();
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			String projectID = request.getParameter("projectID");
			String store = request.getParameter("records");
			contractService.doSaveEstimatedInfo(projectID, store, userObj);
			jsonStr.setSuccess(true);
			jsonStr.setMessage("保存成功！");
			jsonStr.setMessageTitle("成功");
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
		WebUtil.returnResponse(response, jsonStr.toJsonString());
	}

	public void getProjectEstimatedPaymentList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getProjectEstimatedPaymentList ");
		String jsonStr = "";
		try {
			String projectID = request.getParameter("projectID");
			jsonStr = contractService.getProjectEstimatedPaymentList(projectID);
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	public void deleteEstimatedInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> deleteEstimatedInfo");
		ExtjsAjaxResponse jsonStr = new ExtjsAjaxResponse();
		try {
			String estimatedIds = request.getParameter("estimatedIDS");

			if (estimatedIds != null && !"".equals(estimatedIds)) {
				contractService.doDeleteEstimatedInfo(estimatedIds);
			} else {
				throw new NullPointerException();
			}
			jsonStr.setSuccess(true);
			jsonStr.setMessageTitle("成功");
			jsonStr.setMessage("删除成功!");
		} catch (NullPointerException e) {
			jsonStr.setSuccess(false);
			jsonStr.setMessageTitle("未选择");
			jsonStr.setMessage("请先选择!");
		} catch (Exception e) {
			jsonStr.setSuccess(false);
			jsonStr.setMessageTitle("失败");
			jsonStr.setMessage("删除失败!");
		}
		WebUtil.returnResponse(response, jsonStr.toJsonString());
	}

	public void updateEstimatedPayedFlag(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> updateEstimatedPayedFlag");
		ExtjsAjaxResponse jsonStr = new ExtjsAjaxResponse();
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			String estimatedID = request.getParameter("estimatedID");
			EstimatedPayment ep = new EstimatedPayment();
			ep.setId(estimatedID);
			ep.setLastModifiedId(userObj.getUserID());
			contractService.doUpdateEstimatedPayedFlag(ep);
			jsonStr.setSuccess(true);
			jsonStr.setMessageTitle("成功");
			jsonStr.setMessage("修改成功!");
		} catch (Exception e) {
			jsonStr.setSuccess(false);
			jsonStr.setMessageTitle("失败");
			jsonStr.setMessage("修改失败!");
		}
		WebUtil.returnResponse(response, jsonStr.toJsonString());
	}

	public void getTotalSumByID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getTotalSumByID ");
		String jsonStr = "";

		try {
			double mb = contractService.getTotalSumByID(
					Type.GetInt(request.getParameter("conID")),
					WebKeys.Type_Total_Sum_Invoiced);
			jsonStr = "{success:true,invoiceAmt:" + mb + "}";
		} catch (Exception e) {
			e.printStackTrace();
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	public void getContractInfoByProjectCode(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String projectCode = Type.getString(request.getParameter("code"));
		String store = "";
		try {
			store = contractService.getConInfoByProjectCode(projectCode);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void getContractAmtSplitList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {

		String store = "";
		PageInfo pageInfo = WebUtil.generatePageInfo(request);

		List<StandardQuery> querys = (List<StandardQuery>) request
				.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);
		if (querys == null) {
			querys = new ArrayList<StandardQuery>();
		}
		if (!StringUtils.isEmpty(request.getParameter("inkling"))
				&& !StringUtils.isEmpty(request.getParameter("inklings"))) {
			String inkling = request.getParameter("inkling");
			String inklings = request.getParameter("inklings");
			StandardQuery query = WebUtil.generateStandardQueryByInkling(
					inklings, inkling);
			querys.add(query);
		}
		if (!StringUtils.isEmpty(request.getParameter("sourceID"))) {
			querys.add(SimpleParam.buildSimpleParam(RelationalOperator.JDBCSQL,
					"where 合同来源类型ID = ?", request.getParameter("sourceID")));
		}

		String deptID = request.getParameter("deptID");
		if (!StringUtils.isEmpty(deptID)) {
			if (deptID.indexOf(",") == -1) {
				querys.add(SimpleParam
						.buildSimpleParam(RelationalOperator.JDBCSQL,
								"where 乙方部门ID = ? ", deptID));
			} else {
				querys.add(SimpleParam.buildSimpleParam(
						RelationalOperator.JDBCSQL, " where 乙方部门ID in ("
								+ deptID + ")", ""));
			}

		}

		store = contractService.getContractAmtSplitList(querys, pageInfo);
		WebUtil.returnResponse(response, store);
	}

	public void getContractSourceTypeID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		try {
			int level = Type.GetInt(request.getParameter("level"));
			String store = contractService.getContractSourceTypeID(level);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void getContractOpType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			int contractID = Type.GetInt(request.getParameter("contractID"));
			ContractObj contract = contractService
					.getContractInfoByID(contractID);
			int contractOpType = loginService.getContractOpType(userObj,
					contract);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("contractOpType",contractOpType);
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("自动获取合同操作类型失败");
			ajaxResponse.setMessageTitle("失败");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	
	public void getExcuteDeptInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		try{
			String deptIds = request.getParameter("deptIds");
			String deptInfo = contractService.getExcuteDeptInfo(deptIds);
			WebUtil.returnResponse(response, deptInfo);
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public void getOutContractDelimiterStore(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		try {
			WebUtil.returnResponse(response,
					contractService.getOutContractDelimiterStroe());
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public SysService getSysService() {
		return sysService;
	}

	public void setSysService(SysService sysService) {
		this.sysService = sysService;
	}

	public LoginService getLoginService() {
		return loginService;
	}

	public void setLoginService(LoginService loginService) {
		this.loginService = loginService;
	}

}