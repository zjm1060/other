package com.bitc.jk.xmis.web.action;

import java.sql.Date;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.service.CrmService;
import com.bitc.jk.xmis.util.DuplicatedException;
import com.bitc.jk.xmis.util.TimeOutException;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

public class CrmAction extends DispatchAction {

	private static Logger logger = Logger.getLogger(CrmAction.class);

	private CrmService crmService;

	/**
	 * @roseuid 4C1B2D240157
	 */
	public CrmAction() {

	}

	/**
	 * @return the crmService
	 */
	public CrmService getCrmService() {
		return crmService;
	}

	/**
	 * @param crmService
	 *            the crmService to set
	 */
	public void setCrmService(CrmService crmService) {
		this.crmService = crmService;
	}
	
	public void dataValidate(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		
		logger.info("---> createNewCompanySimple //"
				+ request.getParameter("type"));
		
		String jsonStr = "";
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {

				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new Exception("Timeout!");

			} else {
				CompanySimpleObj comp = new CompanySimpleObj();
				comp.setType(request.getParameter("type"));


				if (WebKeys.New_Company_Simple_Type_Company.equals(comp
						.getType())) {
					comp.setCompanyName(request.getParameter("compName"));
					comp.setCompanyAbb(request.getParameter("compApp"));
					comp.setDeptName(request.getParameter("deptName"));
					comp.setEmpName(request.getParameter("empName"));
				}

				else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
						.getType())) {
					comp.setCompanyID(Type.GetInt(request
							.getParameter("compID")));
					comp.setDeptName(request.getParameter("deptName"));
				}

				else if (WebKeys.New_Company_Simple_Type_ContactPerson
						.equals(comp.getType())) {
					comp.setCompanyID(Type.GetInt(request
							.getParameter("compID")));
					comp.setDeptID(Type.GetInt(request.getParameter("deptID")));
					comp.setEmpName(request.getParameter("empName"));
				}				
				
				String msgStr = crmService.dataValidate(comp);
				boolean flag = true;
				
				if(msgStr != null && !"".equals(msgStr))
					flag = false;
				
				logger.info(msgStr);
				jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
						+ msgStr + "',messageTitle:'" + "提示" + "'}";				

			}
			
		} catch (DuplicatedException de){
			de.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'"+de.getMessage()+"',messageTitle:'失败'}";
			
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		
		WebUtil.returnResponse(response, jsonStr);
	
	}

	public void createNewCompanySimple(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> createNewCompanySimple //"
				+ request.getParameter("compAbb"));
		String jsonStr = "";
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {

				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new Exception("Timeout!");

			} else {

				CompanySimpleObj comp = new CompanySimpleObj();
				comp.setType(request.getParameter("type"));

				String titleType = "";

				if (WebKeys.New_Company_Simple_Type_Company.equals(comp
						.getType())) {
					comp.setCompanyName(request.getParameter("compName"));
					comp.setCompanyAbb(request.getParameter("compAbb"));
					comp.setDeptName(request.getParameter("deptName"));
					comp.setEmpName(request.getParameter("empName"));
					titleType = "新公司";
					
				}

				else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
						.getType())) {
					comp.setCompanyID(Type.GetInt(request
							.getParameter("compID")));
					comp.setDeptName(request.getParameter("deptName"));
					titleType = "新部门";
				}

				else if (WebKeys.New_Company_Simple_Type_ContactPerson
						.equals(comp.getType())) {
					comp.setCompanyID(Type.GetInt(request
							.getParameter("compID")));
					comp.setDeptID(Type.GetInt(request.getParameter("deptID")));
					comp.setEmpName(request.getParameter("empName"));
					titleType = "新联系人";
				}

				String compStr = crmService.doCreateNewCompanySimple(comp,
						userObj);
				logger.info(compStr);
				String messages = "", messageTitle = "";
				boolean flag = false;

				if (!"".equals(compStr)) {

					messages = titleType + "创建成功";
					messageTitle = "成功";
					flag = true;

				} else {
					messages = titleType + "创建失败！";
					messageTitle = "失败";
				}

				jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
						+ messages + "',messageTitle:'" + messageTitle + "',"
						+ compStr + "}";

			}
		} catch (DuplicatedException de){
			de.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'"+de.getMessage()+"',messageTitle:'失败'}";
			
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	public void getContactPersonByCompany(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getContactPersonByCompany //"
				+ request.getParameter("compID"));
		String jsonStr = "";
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {

				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new Exception("Timeout!");

			} else {

				jsonStr = crmService.getPeasonListByCompany(Type.GetInt(request
						.getParameter("compID")), userObj);

			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	public void getContactPersonBySearchName(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getContactPersonBySearchName //"
				+ request.getParameter("compID") + "//"
				+ request.getParameter("searchName"));
		String jsonStr = "";
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {

				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new Exception("Timeout!");

			} else {

				jsonStr = crmService.getContactPersonBySearchName(Type
						.GetInt(request.getParameter("compID")), request
						.getParameter("searchName"), userObj);

			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}
	
}
