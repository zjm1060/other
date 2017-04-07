//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\ContractAction.java

package com.bitc.jk.xmis.web.action;

import java.sql.Date;
import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.GContractSourceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PaymentConditionObj;
import com.bitc.jk.xmis.model.PaymentDetailObj;

import com.bitc.jk.xmis.model.InvoiceObj ;//tianXh add 

import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.service.ContractService;
import com.bitc.jk.xmis.util.Argument;
import com.bitc.jk.xmis.util.TimeOutException;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

public class ContractAction extends DispatchAction {

	private static Logger logger = Logger.getLogger(ContractAction.class);

	private ContractService contractService;

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

		String forwordStr = request.getSession().getAttribute(
				WebKeys.Session_Attribute_Name_Layout).equals(
				WebKeys.Type_Layout_Ext) ? WebKeys.Forword_to_constract_ext
				: WebKeys.Forword_to_constract;

		if (Type.getString(request.getParameter("showType")).equals("readonly"))
			forwordStr = WebKeys.Forword_to_constractReadonly;
		else if (Type.getString(request.getParameter("showType")).equals(
				"paymentOption"))
			forwordStr = WebKeys.Forword_to_PaymentOption;

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			logger.info(userObj.getCompanyName());

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {
				request.setAttribute(WebKeys.Request_Attribute_Name_Type,
						WebKeys.Request_Attribute_Name_Type_Contract);
				request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
						WebKeys.Request_Attribute_Name_SubType_contract);

				request
						.setAttribute(
								WebKeys.Request_Attribute_Name_ContractObject,

								(request.getParameter("contractID") == null || ""
										.equals(request
												.getParameter("contractID"))) ? contractService
										.getContractInfoNull()
										: contractService
												.getContractInfoByID(new Integer(
														request
																.getParameter("contractID"))
														.intValue()));
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
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}

		return (mapping.findForward(forwordStr));
	}

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

			WebUtil.returnResponse(response, contractService.getContractList(
					userObj, args));

		} catch (Exception e) {
			e.printStackTrace();
		}
		return (mapping.findForward(forwordStr));
	}

	/*
	 * public void getContractList(ActionMapping mapping, ActionForm form,
	 * HttpServletRequest request, HttpServletResponse response) { try {
	 * 
	 * EmployeeObj userObj = (EmployeeObj) request.getSession()
	 * .getAttribute(WebKeys.Session_Attribute_Name_UserDNA); Argument arg = new
	 * Argument(); List args = new ArrayList(); args.add(arg); Document
	 * contractDocument = contractService.getContractListXML( userObj, args);
	 * 
	 * request.setAttribute("success", "true");
	 * response.setContentType("text/xml; charset=utf-8");
	 * org.dom4j.io.OutputFormat format = org.dom4j.io.OutputFormat
	 * .createCompactFormat(); format.setEncoding("utf-8"); java.io.PrintWriter
	 * out = response.getWriter(); org.dom4j.io.XMLWriter writer = new
	 * org.dom4j.io.XMLWriter(out, format);
	 * 
	 * writer.write(contractDocument); writer.flush(); writer.close(); } catch
	 * (Exception e) { request.setAttribute("failure", "true");
	 * e.printStackTrace(); } }
	 */

	public ActionForward getContractList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getContractList //"
				+ Type.getString(request.getParameter("layout")));

		
		String forwordStr = request.getSession().getAttribute(
				WebKeys.Session_Attribute_Name_Layout).equals(
				WebKeys.Type_Layout_Ext) ? WebKeys.Forword_to_constractList_Ext
				: WebKeys.Forword_to_constractList;
		
//		String forwordStr = Type.getString(request.getParameter("layout"))
//				.equals("ext") ? WebKeys.Forword_to_constractList_Ext
//				: WebKeys.Forword_to_constractList;
		
		logger.info(forwordStr);
		
		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {
				request.setAttribute(WebKeys.Request_Attribute_Name_Type,
						WebKeys.Request_Attribute_Name_Type_Contract);
				request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
						WebKeys.Request_Attribute_Name_SubType_contractList);

				request
						.setAttribute(
								WebKeys.Request_Attribute_Name_SearchType_For_ContractList,
								Type
										.getString(
												request
														.getParameter("searchtype"))
										.equals(
												WebKeys.Type_Search_For_ContractList_Integration) ? WebKeys.Type_Search_For_ContractList_Integration
										: WebKeys.Type_Search_For_ContractList_Normal);
			}

			// } catch (TimeOutException te) {
			// te.printStackTrace();
			// request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
			// WebKeys.Request_Attribute_Name_ErrMessage_Value);
			// WebUtil.returnResponse(response,
			// "{'success':'false','messageTitle':'提示','message':'"
			// + te.getMessage() + "'}");

		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}

		return (mapping.findForward(forwordStr));
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

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
						WebKeys.Request_Attribute_Name_SubType_contract);

				request.setAttribute(
						WebKeys.Request_Attribute_Name_ContractObject,
						contractService.getContractInfoByID(new Integer(request
								.getParameter("contractIDnew")).intValue()));
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
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}

		return (mapping.findForward(forwordStr));

	}

	public ActionForward paymentListEdit(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> paymentListEdit ");

		String forwordStr = WebKeys.Forword_to_paymentListEdit;
		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {
				request.setAttribute(WebKeys.Request_Attribute_Name_Type,
						WebKeys.Request_Attribute_Name_Type_Contract);
				request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
						WebKeys.Request_Attribute_Name_SubType_paymentListEdit);

			}

			// } catch (TimeOutException te) {
			// te.printStackTrace();
			// request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
			// WebKeys.Request_Attribute_Name_ErrMessage_Value);
			// WebUtil.returnResponse(response,
			// "{'success':'false','messageTitle':'提示','message':'"
			// + te.getMessage() + "'}");

		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}

		return (mapping.findForward(forwordStr));
	}
	
	public ActionForward invoiceEdit(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> invoiceEdit ");

		String forwordStr = WebKeys.Forword_to_invoiceEdit;
		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {
				request.setAttribute(WebKeys.Request_Attribute_Name_Type,
						WebKeys.Request_Attribute_Name_Type_Contract);
				request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
						WebKeys.Request_Attribute_Name_SubType_invoiceEdit);

			}

			// } catch (TimeOutException te) {
			// te.printStackTrace();
			// request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
			// WebKeys.Request_Attribute_Name_ErrMessage_Value);
			// WebUtil.returnResponse(response,
			// "{'success':'false','messageTitle':'提示','message':'"
			// + te.getMessage() + "'}");

		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}

		return (mapping.findForward(forwordStr));
	}	

	public void getGovernmentContractSourceList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getGovernmentContractSourceList"
				+ request.getParameter("selectedID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				int selectedID = Integer.parseInt(request
						.getParameter("selectedID"));
				WebUtil.returnResponse(response, contractService
						.getGovernmentContractSourceList(selectedID, WebUtil
								.generatePageInfo(request)));

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

	public void saveGovernmentContractSource(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> saveGovernmentContractSource //"
				+ request.getParameter("selected"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				GContractSourceObj st = new GContractSourceObj();
				st.setContractID(Type
						.GetInt(request.getParameter("contractID")));
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
				st
						.setSelected(Type.getString(request
								.getParameter("selected")));

				st.setGovermentContractSourceID(contractService
						.doSaveGovernmentContractSource(st, userObj));

				String messages = "";
				String messageTitle = "";
				boolean flag = false;

				if (st.getGovermentContractSourceID() > 0) {

					messages = "横向来源信息设置成功";
					messageTitle = "成功";
					flag = true;

				} else {
					messages = "横向来源信息设置失败！";
					messageTitle = "失败";
				}

				jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
						+ messages + "',messageTitle:'" + messageTitle
						+ "',stID:'"
						+ Type.getString(st.getGovermentContractSourceID())
						+ "',stName:'" + st.getGovermentContractSourceName()
						+ "'}";

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

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

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

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				PageInfo pageInfo = WebUtil.generatePageInfo(request);

				String store = "";
				int contractTypeID = Type.GetInt(request
						.getParameter("contractTypeID"));

				// String store =
				// contractService.getContractListForParentID(contractTypeID,
				// pageInfo);

				if (Type.GetInt(request.getParameter("parentID")) == 0)
					store = contractService.getContractListForParent(
							contractTypeID, request.getParameter("searchName")
									.trim(), pageInfo);
				else
					store = contractService.getParentContractListPositionByID(
							contractTypeID, Type.GetInt(request
									.getParameter("parentID")), request
									.getParameter("searchName").trim(),
							pageInfo);

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

	public void getContractListByType(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getContractListByType // "
				+ request.getParameter("contractTypeID") + "//"
				+ request.getParameter("searchName"));

		logger.info(request.getParameter("start") + "//"
				+ request.getParameter("limit") + "//"
				+ request.getParameter("sort") + "//"
				+ request.getParameter("dir"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				PageInfo pageInfo = WebUtil.generatePageInfo(request);

				String store = "";
				int contractTypeID = Type.GetInt(request
						.getParameter("contractTypeID"));

				int warningType = Type.GetInt(request
						.getParameter("warningType"));

				// if(Type.GetInt(request.getParameter("searchType")) ==
				// WebKeys.Type_Contract_Integration_Simple)
				// store = warningType == 0 ? contractService
				// .getContractListByIntegration(contractTypeID,
				// request.getParameter("searchName").trim(),
				// WebKeys.Type_Contract_Integration_Simple,
				// pageInfo, userObj) : contractService
				// .getContractListByIntegration(contractTypeID,
				// warningType,
				// WebKeys.Type_Contract_Integration_Simple,
				// pageInfo, userObj);
				// else
				
				if (Type.getString(request.getParameter("currencyGraph")).equals(""))
					store = warningType == 0 ? contractService
							.getContractListByType(contractTypeID, request
									.getParameter("searchName").trim(),
									pageInfo) : contractService
							.getContractListByTypeFilterWarning(contractTypeID,
									warningType, pageInfo, userObj);
				else{
					// store = contractService.getContractListByIntegration(
					// contractTypeID, begDate, endDate, integrationType,
					// pageInfo, userObj);
					logger.info("getContractListByTypeForCurrency");
					store = contractService.getContractListByTypeForCurrency(
							contractTypeID, request.getParameter("searchName")
									.trim(), Type.getString(request
									.getParameter("currencyGraph")), pageInfo,
							userObj);
				}

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

	public void integrationSearch(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> integrationSearch // "
				+ request.getParameter("searchType") + "//"
				+ request.getParameter("currencyGraph"));

		logger.info(request.getParameter("start") + "//"
				+ request.getParameter("limit") + "//"
				+ request.getParameter("sort") + "//"
				+ request.getParameter("dir"));

		try {

			if (WebKeys.Type_Contract_Integration_Simple == Type.GetInt(request
					.getParameter("searchType")))
				this.getContractListByType(mapping, form, request, response);

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				PageInfo pageInfo = WebUtil.generatePageInfo(request);

				String store = "";
				int contractTypeID = Type.GetInt(request
						.getParameter("contractTypeID"));

				int warningType = Type.GetInt(request
						.getParameter("warningType"));

				String begDate = Type
						.getString(request.getParameter("begDate"));
				String endDate = Type
						.getString(request.getParameter("endDate"));
				int integrationType = Type.GetInt(request
						.getParameter("searchType"));

				store = Type.getString(request.getParameter("currencyGraph"))
						.equals("") ? contractService
						.getContractListByIntegration(contractTypeID, begDate,
								endDate, integrationType, pageInfo, userObj)
						: contractService.getContractListByIntegrationForCurrency(
								contractTypeID, begDate, endDate,
								integrationType, Type.getString(request
										.getParameter("currencyGraph")),
								Type.getString(request.getParameter("subIntegrationType")),
								pageInfo, userObj);

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

	public void integrationSearchForSum(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> integrationSearchForSum ");

		try {

			// if(WebKeys.Type_Contract_Integration_Simple ==
			// Type.GetInt(request.getParameter("searchType")))
			// this.getContractListByType(mapping, form, request, response);
			//			

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				PageInfo pageInfo = WebUtil.generatePageInfo(request);

				String store = "";
				int contractTypeID = Type.GetInt(request
						.getParameter("contractTypeID"));

				int warningType = Type.GetInt(request
						.getParameter("warningType"));

				String begDate = Type
						.getString(request.getParameter("begDate"));
				String endDate = Type
						.getString(request.getParameter("endDate"));
				int integrationType = Type.GetInt(request
						.getParameter("searchType"));

				if (Type.GetInt(request.getParameter("searchType")) == WebKeys.Type_Contract_Integration_Simple)
					store = warningType == 0 ? contractService
							.getContractListByIntegrationSum(contractTypeID,
									request.getParameter("searchName").trim(),
									WebKeys.Type_Contract_Integration_Simple,
									pageInfo, userObj) : contractService
							.getContractListByIntegrationSum(contractTypeID,
									warningType,
									WebKeys.Type_Contract_Integration_Simple,
									pageInfo, userObj);
				else
					store = contractService.getContractListByIntegrationSum(
							contractTypeID, begDate, endDate, integrationType,
							pageInfo, userObj);

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

	// getAttachedListStroe
	public void getAttachedListStroe(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getAttachedListStroe // "
				+ request.getParameter("contractID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				int contractID = Type
						.GetInt(request.getParameter("contractID"));

				String store = contractService.getAttachedListStroe(contractID,
						WebUtil.generatePageInfo(request));

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

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				int refID = Type.GetInt(request.getParameter("refID"));

				String store = contractService.getAttachedListStroe(refID, Type
						.GetInt(request.getParameter("applicationType")),
						WebUtil.generatePageInfo(request));

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

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {
				String store = "";
				PageInfo pageInfo = WebUtil.generatePageInfo(request);

				if (Type.GetInt(request.getParameter("compID")) == 0)
					store = contractService.getCompanyListStroe(userObj
							.getCompanyID(), pageInfo);
				else
					store = contractService.getCompanyListPositionByID(userObj
							.getCompanyID(), Type.GetInt(request
							.getParameter("compID")), pageInfo);

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

	public void getDeptListStroeByCompany(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getDeptListStroeByCompany // ?companyID="
				+ request.getParameter("compID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				String store = contractService.getDeptListStroeByCompany(Type
						.GetInt(request.getParameter("compID")), userObj,
						WebUtil.generatePageInfo(request));

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

	public void saveContractInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> saveContractInfo //"
				+ request.getParameter("delivery_id"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("TimeOut！请重新登录。");
			} else {

				ContractObj con = new ContractObj();
				ProjectObj pro = new ProjectObj();

				con.setContractTypeID(Type.GetInt(request
						.getParameter("contractTypeID")));
				con.setSourceTypeID(Type.GetInt(request
						.getParameter("contractSourceType")));
				con.setGovermentContractSourceID(Type.GetInt(request
						.getParameter("gSourceTypeID")));
				con.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				con.setContractNo(request.getParameter("contractCode"));

				pro.setProjectID(Type.GetInt(request.getParameter("projID")));
				con.setProjectOBJ(pro);

				con.setContractName(request.getParameter("contractName"));
				con.setAbbreviation(request.getParameter("abb"));
				con.setContractSummary(request.getParameter("summary"));
				con.setParentID(Type.GetInt(request.getParameter("parentID")));

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

				con.setContractAmt(Type.GetDouble(request
						.getParameter("contractAmt")));
				con.setCurrencyID(Type.GetInt(request
						.getParameter("currencyType")));
				con.setSignatureDate(Type.getDate(request
						.getParameter("signatureDate")));
				con
						.setBuyerID(Type.GetInt(request
								.getParameter("aCompany_id")));
				con.setDeptIDOfBuyer(Type.GetInt(request
						.getParameter("aDept_id")));
				con.setBuyerRepresentativeID(Type.GetInt(request
						.getParameter("aEmp_id")));
				con.setVendorID(Type
						.GetInt(request.getParameter("bCompany_id")));
				con.setDeptIDOfVendor(Type.GetInt(request
						.getParameter("bDept_id")));
				con.setVendorRepresentativeID(Type.GetInt(request
						.getParameter("bEmp_id")));
				con.setRemark(request.getParameter("remark"));
				con.setDeliveryID(Type.GetInt(request
						.getParameter("delivery_id")));
				con.setRegisterID(userObj.getUserID());

				contractService.doSaveContractInfo(con, userObj);

				response.getWriter().write(
						String.valueOf("保存成功！" + "^"
								+ Type.getString(con.getContractID())));

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

			jsonStr = "{success:false,message:'设置失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	public void getPaymentConditionList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getPaymentConditionList"
				+ request.getParameter("contractID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				WebUtil.returnResponse(response, contractService
						.getPaymentConditionList(Type.GetInt(request
								.getParameter("contractID"))));

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

	public void savePaymentCondition(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> savePaymentCondition ?contractID=//"
				+ request.getParameter("manualFlag") + "//"
				+ request.getParameter("manualReason"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				PaymentConditionObj pay = new PaymentConditionObj();
				pay.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				pay.setPaymentConditionID(Type.GetInt(request
						.getParameter("conditionID")));
				pay.setDescription(request.getParameter("description"));
				pay.setProjectStatusMapping(Type.GetInt(request
						.getParameter("projectStatusID")));
				pay.setProjectStatusName(request
						.getParameter("projectStatusName"));
				pay
						.setDelayDays(Type.GetInt(request
								.getParameter("delayDays")));
				pay.setPaymentPercent(Type.GetInt(request
						.getParameter("percent")));
				pay.setPaymentDate(Type.getDate(request
						.getParameter("paymentDate")));
				pay.setPamymentAmt(Type.GetDouble(request.getParameter("amt")));
				pay.setManualFlag(request.getParameter("manualFlag"));

				pay.setManualReason(Type.getString(request
						.getParameter("manualReason")));

				pay.setPaymentConditionID(contractService
						.doSavePaymentCondition(pay, userObj));

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

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				PaymentConditionObj payC = new PaymentConditionObj();

				payC.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				payC.setPaymentConditionID(Type.GetInt(request
						.getParameter("conditionID")));

				int delFlag = contractService.doDelPaymentCondition(payC,
						userObj);

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

			jsonStr = "{success:false,message:'删除失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	public void getPaymentList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getPaymentList //" + request.getParameter("sort"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {
				WebUtil.returnResponse(response, contractService
						.getPaymentListByID(Type.GetInt(request
								.getParameter("contractID")), WebUtil
								.generatePageInfo(request)));

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

	public void getInvoiceList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger
				.info("---> getInvoicetList"
						+ request.getParameter("contractID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {
				WebUtil.returnResponse(response, contractService
						.getInvoiceListByID(Type.GetInt(request
								.getParameter("contractID")), WebUtil
								.generatePageInfo(request)));

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

	// getChildContractListByID
	public void getChildContractListByID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getChildContractListByID"
				+ request.getParameter("parentID"));

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				WebUtil.returnResponse(response, contractService
						.getChildContractListByID(Type.GetInt(request
								.getParameter("parentID")), WebUtil
								.generatePageInfo(request)));

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

	public void savePaymentDetail(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> savePaymentDetail ?contractID=//"
				+ request.getParameter("transatorID"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				PaymentDetailObj pay = new PaymentDetailObj();
				pay.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				pay.setPaymentID(Type.GetInt(request
						.getParameter("paymentDetailID")));
				pay
						.setPaymentType(Type.GetInt(request
								.getParameter("contractType")) == WebKeys.Type_ContractType_Income ? WebKeys.Type_PaymentType_Income
								: WebKeys.Type_ContractType_Payout);

				pay.setPaymentDate(Type.getDate(request
						.getParameter("paymentDetailDate")));
				pay.setPaymentAmt(Type.GetDouble(request
						.getParameter("paymentDetailAmt")));

				pay.setRemark(request.getParameter("paymentDetailRemark"));

				pay.setTransactorID(Type.GetInt(request
						.getParameter("transatorID")));

				pay.setPaymentID(contractService.doSavePaymentDetail(pay,
						userObj));

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

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				PaymentDetailObj payC = new PaymentDetailObj();

				payC.setContractID(Type.GetInt(request
						.getParameter("contractID")));
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

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}
	
//invoice	
	public void saveInvoiceDetail(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> saveInvoiceDetail ?contractID=//"
				+ request.getParameter("transatorID"));

		String jsonStr = "";

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				InvoiceObj invo = new InvoiceObj();
				invo.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				invo.setInvoiceID(Type.GetInt(request
						.getParameter("invoiceID")));
				
				invo.setInvoiceCode(Type.getString(request.getParameter("invoiceCode")));
				
				invo
				.setContractType(Type.GetInt(request
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
				

				invo.setInvoiceID(contractService.doSaveInvoiceDetail(invo,
						userObj));


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

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				InvoiceObj invoice = new InvoiceObj();

				invoice.setContractID(Type.GetInt(request
						.getParameter("contractID")));
				invoice.setInvoiceID(Type.GetInt(request
						.getParameter("invoiceID")));

				int delFlag = contractService.doDelInvoiceDetail(invoice, userObj);

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

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}
		

//	warningTotalCountForSum
	public void warningTotalCountForSum(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> warningTotalCountForSum ");

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("Timeout!");
			} else {

				PageInfo pageInfo = WebUtil.generatePageInfo(request);

				String store = "";
				int contractTypeID = Type.GetInt(request
						.getParameter("contractTypeID"));

				int warningType = Type.GetInt(request
						.getParameter("warningType"));

				String begDate = Type
						.getString(request.getParameter("begDate"));
				String endDate = Type
						.getString(request.getParameter("endDate"));
				int integrationType = Type.GetInt(request
						.getParameter("searchType"));

//				if (Type.GetInt(request.getParameter("searchType")) == WebKeys.Type_Contract_Integration_Simple)
//					store = warningType == 0 ? contractService
//							.getContractListByIntegrationSum(contractTypeID,
//									request.getParameter("searchName").trim(),
//									WebKeys.Type_Contract_Integration_Simple,
//									pageInfo, userObj) : contractService
//							.getContractListByIntegrationSum(contractTypeID,
//									warningType,
//									WebKeys.Type_Contract_Integration_Simple,
//									pageInfo, userObj);
//				else
//					store = contractService.getContractListByIntegrationSum(
//							contractTypeID, begDate, endDate, integrationType,
//							pageInfo, userObj);
				
				store = contractService.getWarningTotalCountForSum(contractTypeID,
						request.getParameter("searchName").trim(),
						integrationType,
						begDate,
						endDate,
						pageInfo, userObj);

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