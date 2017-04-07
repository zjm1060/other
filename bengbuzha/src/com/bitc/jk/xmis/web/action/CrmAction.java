package com.bitc.jk.xmis.web.action;

import java.sql.Date;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.beanutils.BeanUtils;

import org.apache.commons.beanutils.PropertyUtils;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.MeetingRoomInformation;
import com.bitc.jk.xmis.model.OfficeSuppliesDetails;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PatentInformation;
import com.bitc.jk.xmis.model.ProjectQualification;
import com.bitc.jk.xmis.model.QualificationObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.SupplierInformation;
import com.bitc.jk.xmis.model.VehicleManagement;
import com.bitc.jk.xmis.model.crm.Address;
import com.bitc.jk.xmis.model.crm.BusinessRelation;
import com.bitc.jk.xmis.model.crm.City;
import com.bitc.jk.xmis.model.crm.CompanyScale;
import com.bitc.jk.xmis.model.crm.CreditLevel;
import com.bitc.jk.xmis.model.crm.CustomerLevel;
import com.bitc.jk.xmis.model.crm.RelativeAddress;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.service.CrmService;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.exception.DuplicatedException;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;
import com.bitc.jk.xmis.web.response.ExtjsFormLoadResponse;

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

		logger.info("---> createNewCompanySimple  empName-->"
				+ request.getParameter("empName"));

		logger.info("---> createNewCompanySimple  deptID-->"
				+ request.getParameter("deptID"));
		logger.info("---> createNewCompanySimple  compID-->"
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

				if (msgStr != null && !"".equals(msgStr))
					flag = false;

				logger.info(msgStr);
				jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
						+ msgStr + "',messageTitle:'" + "提示" + "'}";

			}

		} catch (DuplicatedException de) {
			de.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'" + de.getMessage()
					+ "',messageTitle:'失败'}";

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

		logger.info("---> createNewCompanySimple --》compID="
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

				CompanySimpleObj comp = new CompanySimpleObj();
				comp.setType(request.getParameter("type"));

				String titleType = "";

				// 新建联系人，临时添加
				String empType = request.getParameter("empType");

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

					// 新建联系人，临时添加
					if (empType != null && "linkMan".equals(empType)) {
						comp.setEmpAddr(request.getParameter("empAddr"));
						comp.setEmpPhone(request.getParameter("empPhone"));
						comp.setEmpPostCode(request.getParameter("empPostCode"));
						comp.setEmpPost(request.getParameter("empPost"));
						comp.setEmpMobilePhone(request
								.getParameter("empMobilePhone"));
						comp.setEmpEmail(request.getParameter("empEmail"));
						comp.setEmpType(empType);
					}

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
		} catch (DuplicatedException de) {
			de.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'" + de.getMessage()
					+ "',messageTitle:'失败'}";

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

				jsonStr = crmService.getPeasonListByCompany(
						Type.GetInt(request.getParameter("compID")), userObj);

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

				jsonStr = crmService.getContactPersonBySearchName(
						Type.GetInt(request.getParameter("compID")),
						request.getParameter("searchName"), userObj);

			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	public void getEmpInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("---> getEmpInfoById   empID="+request.getParameter("empID"));
		ExtjsFormLoadResponse ajaxResponse = new ExtjsFormLoadResponse();
		try {
			String empID = request.getParameter("empID");
			EmployeeObj emp = crmService.getEmpInfoById(Type.GetInt(empID));
			Map<String, Object> map = new HashMap<String, Object>();
			String properties = request.getParameter("properties");
			//公司ID
			String pathId = emp.getPathId();
			String compID = pathId.substring(pathId.lastIndexOf(",")+1,pathId.length());
			
			String[] ps = StringUtils.split(properties, ',');

			for (String p : ps) {
				map.put(p, PropertyUtils.getProperty(emp, p));
			}
			ajaxResponse.setSuccess(true);
			ajaxResponse.setData(map);
			ajaxResponse.addAttribute("compID", compID);
			ajaxResponse.addAttribute("positionEdit", PropertyUtils.getProperty(emp, "positionEdit"));
		} catch (Exception e) {
			logger.error(e.getMessage());
			ajaxResponse.setSuccess(false);

			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("获取联系人信息失败");

		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void updateEmpInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("---> updateUserInfo");
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
				int empID = Type.GetInt(request.getParameter("empID"));
				String empName = request.getParameter("empName");
				String empAddr = request.getParameter("empAddr");
				String empPost = request.getParameter("empPost");
				String empPhone = request.getParameter("empPhone");
				String empEmail = request.getParameter("empEmail");
				String empMobilePhone = request.getParameter("empMobilePhone");
				String empPostCode = request.getParameter("empPostCode");

				EmployeeObj emp = new EmployeeObj();
				emp.setUserID(empID);
				emp.setUserName(empName);
				emp.setHomeAddress(empAddr);
				emp.setPosition(empPost);
				emp.setFaxNo(empPhone);
				emp.setEmailAddress(empEmail);
				emp.setMobileNo(empMobilePhone);
				emp.setZipCode(empPostCode);
				crmService.updateEmpInfo(emp, userObj);
				jsonStr = "{success:true,message:'操作成功！',messageTitle:'成功'}";
			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	// 得到联系人信息
	public void getEmpInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("---> getEmpInfo   empID=" + request.getParameter("empID"));
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
				String empID = request.getParameter("empID");
				List list = new ArrayList();
				EmployeeObj emp = crmService.getEmpInfoById(Type.GetInt(empID));
				list.add(emp);
				jsonStr = WebUtil.buildJsonStoreByList(list);
			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		WebUtil.returnResponse(response, jsonStr);
	}

	/**
	 * 根据公司ID 的到公司的信息
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 * @throws Exception
	 */
	public ActionForward getCompanyInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String companyID = request.getParameter("companyID");
		String id = request.getParameter("id");
		String sjid = request.getParameter("sjid");
		
		// logger.info("getCompanyInfo//companyId:" + companyID);
		CompanyObj company = null;
//		String sition = null;

		if (companyID != null && !"".equals(companyID.trim())) {
			try {
				company = crmService.getCompanyObjById(Type
						.getInteger(companyID));
//				Map<String, String> map = crmService.getSupplierInformation(Type.getInteger(companyID));
				request.setAttribute("sition", null);
				
//				Map<String,String> qualifica = crmService.getQualificationAssignmentr(id);
				request.setAttribute("qualificamap", null);
				
//				Map<String,String> patent = crmService.getPatentAssignmentr(sjid);
				request.setAttribute("patentmap", null);
			} catch (Exception e) {
				e.printStackTrace();
			}

		}
		request.setAttribute("company", company);
		return mapping.findForward("companyInfo");
	}

	/**
	 * 编辑、保存公司信息
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 * @throws Exception
	 */
	public void doSaveCompanyInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		CompanyObj cmpObj = new CompanyObj();

		cmpObj.setCompanyID(Type.GetInt(request.getParameter("companyID")));
		cmpObj.setCompanyName(request.getParameter("companyName"));
		cmpObj.setAbbreviation(request.getParameter("abbreviation"));

		cmpObj.setTelephoneNo1(request.getParameter("telephoneNo1").trim());
		cmpObj.setTelephoneNo2(request.getParameter("telephoneNo2").trim());
		cmpObj.setFaxNo1(request.getParameter("faxNo1").trim());
		cmpObj.setFaxNo2(request.getParameter("faxNo2").trim());

		cmpObj.setWebSite(request.getParameter("webSite").trim());
		cmpObj.setEmailAddress(request.getParameter("emailAddress").trim());
		cmpObj.setBankName(request.getParameter("bankName").trim());
		cmpObj.setAccountNo(request.getParameter("accountNo"));
		cmpObj.setTaxNo(request.getParameter("taxNo").trim());

		// 法人
		String legalRepresentativeID = request
				.getParameter("legalRepresentativeID");
		String legalRepresentativeEdit = request.getParameter("legalRepresentativeEdit");
		if (!StringUtils.isEmpty(legalRepresentativeID)) {
			SimpleEmployee legalRepresentative = new SimpleEmployee();
			legalRepresentative.setUserID(Type
					.getInteger(legalRepresentativeID));
			cmpObj.setLegalRepresentative(legalRepresentative);
		}
		// 联系人
		String contactEdit = request.getParameter("contactEdit");
		String contactPersonID = request.getParameter("contactPersonID");
		if (!StringUtils.isEmpty(contactPersonID)) {
			SimpleEmployee contactPerson = new SimpleEmployee();
			contactPerson.setUserID(Type.getInteger(contactPersonID));
			cmpObj.setContactPerson(contactPerson);
		} 

		// 业务关系
		String businessRelationID = request.getParameter("businessRelationID");
		if (!StringUtils.isEmpty(businessRelationID)) {
			BusinessRelation businessRelation = new BusinessRelation();
			BeanUtils.populate(businessRelation, request.getParameterMap());
			cmpObj.setBusinessRelation(businessRelation);
		}
		// 公司规模
		String companyScaleID = request.getParameter("companyScaleID");
		if (!StringUtils.isEmpty(companyScaleID)
				&& !companyScaleID.equals("-1")) {
			CompanyScale companyScale = new CompanyScale();
			BeanUtils.populate(companyScale, request.getParameterMap());
			cmpObj.setCompanyScale(companyScale);
		}
		// 客户级别
		String customerLevelID = request.getParameter("customerLevelID");
		if (!StringUtils.isEmpty(customerLevelID)
				&& !customerLevelID.equals("-1")) {
			CustomerLevel customerLevel = new CustomerLevel();
			customerLevel.setCustomerLevelID(Type.getInteger(customerLevelID));
			cmpObj.setCustomerLevel(customerLevel);
		}
		// 信用等级
		String creditLevelID = request.getParameter("creditLevelID");
		if (!StringUtils.isEmpty(creditLevelID) && !creditLevelID.equals("-1")) {
			CreditLevel creditLevel = new CreditLevel();
			creditLevel.setCreditLevelID(Type.getInteger(creditLevelID));
			cmpObj.setCreditLevel(creditLevel);
		}

		// 货币类型
		String currencyTypeID = request.getParameter("currencyTypeID");
		if (!StringUtils.isEmpty(currencyTypeID)) {
			Currency currency = new Currency();
			BeanUtils.populate(currency, request.getParameterMap());
			currency.setId(Type.GetInt(currencyTypeID));
			cmpObj.setCurrency(currency);
		}
		cmpObj.setOrganizationCode(request.getParameter("organizationCode")
				.trim());
		cmpObj.setRegisteredCapital((request.getParameter("registeredCapital")
				.trim()));

		cmpObj.setRemark(request.getParameter("remark").trim());

		cmpObj.setCustomerSourceID(0);
		// 客户来源
		cmpObj.setSecurityLevelID(0);
		// 保密级别

		// 创建人、创建日期
		SimpleEmployee emp = WebUtil.getCurrentEmployee();
		Date currentDateTime = DateUtil.getCurrentDateTime();
		if (cmpObj.getCompanyID() == 0) {
			cmpObj.setUpdateDate(currentDateTime);
			cmpObj.setUpdatePerson(emp);
		} else {
			cmpObj.setUpdateDate(DateUtil.parseDate(
					request.getParameter("updateDateId"), "yyyy-MM-dd"));
			if (!StringUtils.isEmpty(request.getParameter("updateID"))) {
				int updateID = Integer.parseInt(request
						.getParameter("updateID"));
				SimpleEmployee updatePerson = crmService
						.getEmpInfoById(updateID);
				cmpObj.setUpdatePerson(updatePerson);
			}
		}
		// 最后修改人和日期
		cmpObj.setLastModifier(emp);
		cmpObj.setLastModified(currentDateTime);

		// 更新地址表
		Address address = new Address();
		String cityID = request.getParameter("cityID");
		City city = new City();
		city.setCityID(Type.getInteger(cityID));
		address.setCity(city);
		BeanUtils.populate(address, request.getParameterMap());
		if (StringUtils.isEmpty(address.getAddressID())) {
			address.setAddressID(null);
		}
		cmpObj.setAddress(address);
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {

			crmService.doSaveCompanyInfo(cmpObj,contactEdit,legalRepresentativeEdit);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功！");
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.addAttribute("id", cmpObj.getCompanyID());
			ajaxResponse.addAttribute("addressid", address.getAddressID());
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 获取下拉框信息
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	@SuppressWarnings("rawtypes")
	public void getMultiData(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		try {
			int fid = Type.getInteger(request.getParameter("fid"));
			String category = request.getParameter("category");
			logger.info("--------->getMultiData//category：" + category
					+ "   fid:" + fid);
			List list = crmService.getMultiData(category, fid);
			String store = WebUtil.buildJsonStoreByList(list);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 删除一条或者多条公司信息 【 未用】
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void dodelCompanyInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String ids = request.getParameter("ids");
			crmService.dodelCompanyInfoById(ids);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
			ajaxResponse.setMessageTitle("提示");
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 获取公司信息列表 sql query
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return 
	 */
	@SuppressWarnings({ "unused", "unchecked" })
	public ActionForward getCompanyListStoreByQuery(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String show = request.getParameter("show");
		if (show != null) {
			return mapping.findForward("companyList");
		}
		
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		CompositParam query = CompositParam.buildAndCompositParam();
		int businessID = Type.GetInt(request.getParameter("businessID"));
		String searchContent = request.getParameter("searchContent");
		String searchColumn = request.getParameter("searchColumn");
		String personName = request.getParameter("personName");
		
		String sql_Search=SQLScript.sql_Search;
		
		String sql_BusinessID ="";
		if("".equals(searchContent)|| searchContent==null){
			sql_Search="\n"+SQLScript.sql_Search+"\n";
			//模糊查询为空  ( 业务有数据  )
			if(businessID!=-1 && businessID!=0){
				sql_BusinessID=" where  c.业务关系ID = "+businessID+"\n"; 
				}
			sql_Search = sql_Search+ sql_BusinessID;
		} else {
			List<StandardQuery> querysSearch = (List<StandardQuery>) request
			.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);
			// 模糊查询 (业务有数据 )
			if (searchContent != null && !searchContent.trim().equals("")) {
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
					if(businessID!=-1 && businessID!=0){
						sql_BusinessID=" and  c.业务关系ID = "+businessID+"\n"; 
						}
				}
			}
			sql_Search = sql_Search+ " where "+ query+sql_BusinessID;
		}
		String sql_PersonSearch = SQLScript.sql_PersonSearch
			 + 
			"WHERE    RTRIM(h.姓氏) + RTRIM(h.名字) LIKE '%"+personName+"%'\n";
		
		if("".equals(personName)||personName == null){
			sql_PersonSearch="SELECT 公司ID into #person FROM xmis.org_公司";
		}
		//根据公司ID 查列表    in 交集的公司ID 
		String sql_Str = sql_Search+sql_PersonSearch+SQLScript.sql_Search_temp;
		String store = crmService.getCompanyListStoreByQuerys(sql_Str, pageInfo);
		WebUtil.returnResponse(response, store);
		return null;
	}

	/**
	 * 获取地址信息列表 相关地址列表、可包含公司的地址信息
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getAllAddressList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String companyID = request.getParameter("companyId");
		String isNeedCompID = request.getParameter("isNeedCompID");

		String store = crmService.getAllAddressList(query, companyID, isNeedCompID);
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 获取地址信息列表 相关地址列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getRelativeAddressList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = "";
		try {
			StandardQuery query = WebUtil
					.generateStandardQueryByInkling(request);
			String companyID = request.getParameter("companyId");
			PageInfo pageInfo = WebUtil.generatePageInfo(request);
			
			store = crmService.getRelativeAddressListM(query, companyID,
					pageInfo);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	/**
	 * 根据id得到地址
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getAddress(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String id = request.getParameter("id");
		ExtjsFormLoadResponse formLoadResponse = new ExtjsFormLoadResponse();
		if (id != null) {
			try {
				Address rAddress = crmService.getAddress(id);
				formLoadResponse.setSuccess(true);
				formLoadResponse.setData(rAddress);
			} catch (Exception e) {
				e.printStackTrace();
				formLoadResponse.setSuccess(false);
				formLoadResponse.setMessageTitle("提示");
				formLoadResponse.setMessage("载入失败");
			}
		}
		WebUtil.returnResponse(response, formLoadResponse.toJsonString());
	}

	/**
	 * 根据id得到地址
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getRelativeAddress(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String id = request.getParameter("id");
		ExtjsFormLoadResponse formLoadResponse = new ExtjsFormLoadResponse();
		if (id != null) {
			try {
				RelativeAddress rAddress = crmService.getRelativeAddress(id);
				formLoadResponse.setSuccess(true);
				formLoadResponse.setData(rAddress);
			} catch (Exception e) {
				e.printStackTrace();
				formLoadResponse.setSuccess(false);
				formLoadResponse.setMessageTitle("提示");
				formLoadResponse.setMessage("载入失败");
			}
		}
		WebUtil.returnResponse(response, formLoadResponse.toJsonString());
	}

	/**
	 * 相关地址信息的编辑、保存
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void doSaveRelativeAddress(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		RelativeAddress relativeAddress = new RelativeAddress();

		SimpleEmployee emp = WebUtil.getCurrentEmployee();
		Date currentDateTime = DateUtil.getCurrentDateTime();
		// 最后修改人和日期
		relativeAddress.setLastModifier(emp);
		relativeAddress.setLastModified(currentDateTime);
		// 更新地址表
		Address address = new Address();
		String cityID = request.getParameter("cityID");
		City city = new City();
		city.setCityID(Type.getInteger(cityID));
		address.setCity(city);
		BeanUtils.populate(address, request.getParameterMap());
		if (StringUtils.isEmpty(address.getAddressID())) {
			address.setAddressID(null);
		}
		relativeAddress.setAddress(address);
		// 联系人
		String contactPersonID = request.getParameter("contactPersonID");
		if (!StringUtils.isEmpty(contactPersonID)) {
			SimpleEmployee contactPersonA = new SimpleEmployee();
			contactPersonA.setUserID(Type.getInteger(contactPersonID));
			relativeAddress.setContactPersonA(contactPersonA);
		}

		BeanUtils.populate(relativeAddress, request.getParameterMap());
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {

			crmService.doSaveRelativeAddress(relativeAddress);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功！");
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.addAttribute("id", relativeAddress.getId());
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 根据ID删除一条相关地址信息
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void dodelRelativeAddressById(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String id = request.getParameter("id");
			crmService.dodelRelativeAddressById(id);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
			ajaxResponse.setMessageTitle("提示");
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	
	/** 根据名称模糊查询城市选择框的ID
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getAddressIDListByName(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response){
		
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String name = request.getParameter("name");
			if(!StringUtils.isEmpty(name)){
				name = name.trim();
			}
			String selectIds = crmService.getAddressIDListByName(name);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("selectIds", selectIds);
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
		
	}
	
	
	/** 公司地址及相关地址的下拉框
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getRelationAddressCombo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String companyID =request.getParameter("companyID");
		String store = crmService.getRelationAddressCombo(companyID);
		WebUtil.returnResponse(response, store);
	}
	/**
	 * 获取公司地址
	 * */
	public void getRelationAddress(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String companyID =request.getParameter("selectCompanyID");
		System.out.println("companyID="+companyID);
		String store = crmService.getRelationAddress(companyID);
		WebUtil.returnResponse(response, store);
	}
	/**
	 * 供应商信息
	 * */
	public void doSupplierInformation(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try{
			Integer companyID = Integer.valueOf(request.getParameter("selectCompanyID"));
			Integer managerID = Integer.valueOf(request.getParameter("managerID"));
			
			String addressID = request.getParameter("officeAddressID");
			String headOfUnit = request.getParameter("headOfUnit");
			String industryAssessment = request.getParameter("industryAssessment");
			String qualification = request.getParameter("qualification");
			String productsProduction = request.getParameter("productsProduction");
			String standard = request.getParameter("standard");
			Integer generalTaxpayer = Integer.valueOf(request.getParameter("generalTaxpayer"));
			Integer introductionLine = Integer.valueOf(request.getParameter("introductionLine"));
			String ancillaryProducts = request.getParameter("ancillaryProducts");
			String Yield = request.getParameter("Yield");
			String qualityAssurance = request.getParameter("qualityAssurance");
			
			SupplierInformation si = new SupplierInformation();
			CompanyObj cob = new CompanyObj();
			SimpleEmployee se = new SimpleEmployee();
			
			si.setID(companyID);
			si.setAddressID(addressID);
			si.setManagerID(managerID);
			si.setHeadOfUnit(headOfUnit);
			si.setIndustryAssessment(industryAssessment);
			si.setQualification(qualification);
			si.setGeneralTaxpayer(generalTaxpayer);
			si.setProductsProduction(productsProduction);
			si.setStandard(standard);
			si.setAncillaryProducts(ancillaryProducts);
			si.setIntroductionLine(introductionLine);
			si.setYield(Yield);
			si.setQualityAssurance(qualityAssurance);
			si.setNewregisterID(WebUtil.getCurrentEmployee());
			si.setLastModifier(WebUtil.getCurrentEmployee());
			si.setNewupdateDate(DateUtil.getCurrentDateTime());
			si.setLastModified(DateUtil.getCurrentDateTime());
			
			crmService.doSupplierInformation(si);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功！");
			
		}catch(Exception e){
			logger.error("保存供方信息出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	
	/**
	 * 查询资质字典表
	 * */
	public void getQualificationDictionary(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		
		String store = "";
		store = crmService.getQualificationDictionary();
		WebUtil.returnResponse(response, store);
	}
	
	/**
	 * 保存资质信息
	 * */
	public void doQualificationInformation(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try{
			String id = request.getParameter("id");
			Integer companyID = Integer.valueOf(request.getParameter("selectCompanyID"));
			String qualificationName = request.getParameter("qualificationName");
			String departmentName = request.getParameter("departmentName");
			Integer contactID = Integer.valueOf(request.getParameter("qualificationContactID"));
			String contactMode = request.getParameter("qualificationContactMode");
			Date certificateValidQi = Date.valueOf(request.getParameter("certificateValidQi"));
			Date certificateValidZhi = Date.valueOf(request.getParameter("certificateValidZhi"));
			String certificateNo = request.getParameter("certificateNo");
			Integer examined = Integer.valueOf(request.getParameter("examined"));
			String remark = request.getParameter("qualificationRemark");
			
			QualificationObj qfo = new QualificationObj();
			SimpleEmployee se = new SimpleEmployee();
			
			qfo.setId(id);
			qfo.setCompanyID(companyID);
			qfo.setQualificationName(qualificationName);
			qfo.setDepartmentName(departmentName);
			qfo.setContactID(contactID);
			qfo.setContactMode(contactMode);
			qfo.setCertificateValidQi(certificateValidQi);
			qfo.setCertificateValidZhi(certificateValidZhi);
			qfo.setCertificateNo(certificateNo);
			qfo.setExamined(examined);
			qfo.setRemark(remark);
			qfo.setSubscriberID(WebUtil.getCurrentEmployee());
			qfo.setSubscriberDate(DateUtil.getCurrentDateTime());
			qfo.setLastModifier(WebUtil.getCurrentEmployee());
			qfo.setLastModified(DateUtil.getCurrentDateTime());
			
			
			crmService.doQualificationInformation(qfo);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功！");
			
		}catch(Exception e){
			logger.error("保存资质信息出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		
	}
	public void getQualificationList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String companyID = request.getParameter("contractId");
		String store = crmService.getQualificationList(companyID);
		WebUtil.returnResponse(response, store);
	}
	
	/**
	 * 资质使用频次列表查询
	 * */
	public void getUseProjectQualification(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String companyID = request.getParameter("contractId");
		String store = crmService.getUseProjectQualification(companyID);
		WebUtil.returnResponse(response, store);
	}
	
	public void getQualificationAssignment(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String id = request.getParameter("ID");
		String store = crmService.getQualificationAssignment(id);
		WebUtil.returnResponse(response, store);
	}
	
	public void getProjectQualification(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String companyID =request.getParameter("selectCompanyID");
		String store = crmService.getProjectQualification(companyID);
		WebUtil.returnResponse(response, store);
	}
	
	public void doProcurementInformation(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
			ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
			ajaxResponse.setMessageTitle("提示");
			try{
				String id = request.getParameter("id");
				Integer applicationSectorsID = Integer.valueOf(request.getParameter("applicationSectorsID"));
				String ziID = request.getParameter("ziID");
				String projectID = request.getParameter("projectID");
				Integer applicantID = Integer.valueOf(request.getParameter("applicantID"));
				Date applicationDate = Date.valueOf(request.getParameter("applicationDate"));
				String applicationReasons = request.getParameter("applicationReasons");
				
				ProjectQualification rq = new ProjectQualification();
				SimpleEmployee se = new SimpleEmployee();
				rq.setId(id);
				rq.setApplicationSectorsID(applicationSectorsID);
				rq.setZiID(ziID);
				rq.setProjectID(projectID);
				rq.setApplicantID(applicantID);
				rq.setApplicationDate(applicationDate);
				rq.setApplicationReasons(applicationReasons);
				rq.setApplicationLastModifierName(WebUtil.getCurrentEmployee());
				rq.setApplicationNewregisterName(WebUtil.getCurrentEmployee());
				rq.setApplicationLastModified(DateUtil.getCurrentDateTime());
				rq.setApplicationNewupdateDate(DateUtil.getCurrentDateTime());
				
				crmService.doProcurementInformation(rq);
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessage("保存成功！");
				
			}catch(Exception e){
				logger.error("保存资质信息出错。", e);
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
			}
			WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	public void getProjectQualificationAssignment(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String id = request.getParameter("id");
		String store = crmService.getProjectQualificationAssignment(id);
		WebUtil.returnResponse(response, store);
	}
	public void getDecisionMakersInformation(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		Integer hrID = Integer.valueOf(request.getParameter("hrID"));
		String store = crmService.getDecisionMakersInformation(hrID);
		WebUtil.returnResponse(response, store);
	}
	
	/**
	 * 专利
	 * */
	public void doPatentInformation(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
			ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
			ajaxResponse.setMessageTitle("提示");
			try{
				PatentInformation pf = new PatentInformation();
				String sjid = request.getParameter("sjid");
				Integer companyID = Integer.valueOf(request.getParameter("selectCompanyID"));
				String patentName =request.getParameter("patentName");
				String patentsCategory = request.getParameter("patentsCategory");
				Integer patentedDesignPeopleID1 = Integer.valueOf(request.getParameter("patentedDesignPeopleID1"));
				if("".equals(request.getParameter("patentedDesignPeopleID2"))){
					Integer patentedDesignPeopleID2 = null;
//					pf.setPatentedDesignPeopleID2(patentedDesignPeopleID2);
				}else{
					Integer patentedDesignPeopleID2 = Integer.valueOf(request.getParameter("patentedDesignPeopleID2"));
					pf.setPatentedDesignPeopleID2(patentedDesignPeopleID2);
				}
//				if(request.getParameter("patentedDesignPeopleID2") == null || request.getParameter("patentedDesignPeopleID2").length() <= 0){
//					Integer patentedDesignPeopleID2 = Integer.valueOf(request.getParameter("patentedDesignPeopleID2"));
//					
//				}else{
//					
//				}
				if("".equals(request.getParameter("patentedDesignPeopleID3"))){
					Integer patentedDesignPeopleID3 = null;
				}else{
					Integer patentedDesignPeopleID3 = Integer.valueOf(request.getParameter("patentedDesignPeopleID3"));
					pf.setPatentedDesignPeopleID3(patentedDesignPeopleID3);
				}
				Integer patentRightPeopleID1 = Integer.valueOf(request.getParameter("patentRightPeopleID1"));
				if("".equals(request.getParameter("patentRightPeopleID2"))){
					Integer patentRightPeopleID2 = null;
				}else{
					Integer patentRightPeopleID2 = Integer.valueOf(request.getParameter("patentRightPeopleID2"));
					pf.setPatentRightPeopleID2(patentRightPeopleID2);
				}
				if("".equals(request.getParameter("patentRightPeopleID3"))){
					Integer patentRightPeopleID3 = null;
				}else{
					Integer patentRightPeopleID3 = Integer.valueOf(request.getParameter("patentRightPeopleID3"));
					pf.setPatentRightPeopleID3(patentRightPeopleID3);
				}
				if("".equals(request.getParameter("patentRightPeopleID4"))){
					Integer patentRightPeopleID4 = null;
				}else{
					Integer patentRightPeopleID4 = Integer.valueOf(request.getParameter("patentRightPeopleID4"));
					pf.setPatentRightPeopleID4(patentRightPeopleID4);
				}
				if("".equals(request.getParameter("patentRightPeopleID5"))){
					Integer patentRightPeopleID5 = null;
				}else{
					Integer patentRightPeopleID5 = Integer.valueOf(request.getParameter("patentRightPeopleID5"));
					pf.setPatentRightPeopleID5(patentRightPeopleID5);
				}
				
				String patentNumber = request.getParameter("patentNumber");
				String patentCertificateNo = request.getParameter("patentCertificateNo");
				Date applicationTime = Date.valueOf(request.getParameter("applicationTime"));
				Date authorizeTime =Date.valueOf(request.getParameter("authorizeTime"));
				Date validFrom = Date.valueOf(request.getParameter("validFrom"));
				Date validOnly = Date.valueOf(request.getParameter("validOnly"));
				
				
				
				pf.setSjid(sjid);
				pf.setCompanyID(companyID);
				pf.setPatentName(patentName);
				pf.setPatentsCategory(patentsCategory);
				pf.setPatentedDesignPeopleID1(patentedDesignPeopleID1);
				
				
				
				pf.setPatentRightPeopleID1(patentRightPeopleID1);
				
				
				
				
				pf.setPatentNumber(patentNumber);
				pf.setApplicationTime(applicationTime);
				pf.setPatentCertificateNo(patentCertificateNo);
				pf.setAuthorizeTime(authorizeTime);
				pf.setValidFrom(validFrom);
				pf.setValidOnly(validOnly);
				pf.setApplicationTime(applicationTime);
				pf.setPatentregisterName(WebUtil.getCurrentEmployee());
				pf.setPatentupdateDate(DateUtil.getCurrentDateTime());
				pf.setPatentlastModifierName(WebUtil.getCurrentEmployee());
				pf.setPatentlastModified(DateUtil.getCurrentDateTime());
				
				crmService.doPatentInformation(pf);
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessage("保存成功！");
			
			}catch(Exception e){
				logger.error("保存专利信息出错。", e);
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
			}
			WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	
	/**
	 * 专利列表
	 * */
	public void getPatentInformationList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String companyID =request.getParameter("selectCompanyID");
		String store = crmService.getPatentInformationList(companyID);
		WebUtil.returnResponse(response, store);
		
	}
	
	/**
	 * 专利赋值
	 * */
	
	public void getPatentAssignment(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String sjid = request.getParameter("sjid");
		String store = crmService.getPatentAssignment(sjid);
		WebUtil.returnResponse(response, store);
	}
	/**
	 * 客户关系下资质列表
	 * */
	public ActionForward getQualificationListC(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String show = request.getParameter("show");
		if (show != null) {
			return mapping.findForward("qualificationLISTC");
		}
		
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = "";
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		store = crmService.getQualificationListC(pageInfo,query);
		WebUtil.returnResponse(response, store);
		return null;
	}
	/**
	 * 客户关系下专利列表
	 * */
	public ActionForward getPatentInfoListC(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String show = request.getParameter("show");
		if (show != null) {
			return mapping.findForward("pateneInfoLISTC");
		}
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = "";
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		store = crmService.getPatentInfoListC(pageInfo,query);
		WebUtil.returnResponse(response, store);
		return null;
	}
	
	/**
	 * 会议室基本信息维护
	 * */
	public void doMeetingRoomInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try{
			MeetingRoomInformation mr = new MeetingRoomInformation();
			String room_id = request.getParameter("room_id");
			String room_No = request.getParameter("room_No");
			String room_name = request.getParameter("room_name");
			String floor = request.getParameter("floor");
			String address = request.getParameter("address");
			String remark = request.getParameter("remark");
			if("".equals(request.getParameter("number_people"))){
				Integer number_people = null;
//				pf.setPatentedDesignPeopleID2(patentedDesignPeopleID2);
			}else{
				Integer number_people = Integer.valueOf(request.getParameter("number_people"));
				mr.setNumber_people(number_people);
			}
			mr.setRoom_id(room_id);
			mr.setRoom_No(room_No);
			mr.setRoom_name(room_name);
			mr.setFloor(floor);
			mr.setAddress(address);
			mr.setRemark(remark);
			crmService.doMeetingRoomInfo(mr);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功！");
		
		}catch(Exception e){
			logger.error("保存会议室信息出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	/**
	 * 会议室查询
	 * */
	public void getMeetingRoomInfor(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String store = crmService.getMeetingRoomInfor();
		WebUtil.returnResponse(response, store);
	}
	/**
	 * 会议室查询赋值
	 * */
	public void getMeetingRoomInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String room_id = request.getParameter("room_id");
		String store = crmService.getMeetingRoomInfo(room_id);
		WebUtil.returnResponse(response, store);
		
	}
	/**
	 * 会议室信息删除
	 * */
	public void dodelMeetingRoomInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		 

			ExtjsAjaxResponse jsonStr = new ExtjsAjaxResponse();
			String room_id = request.getParameter("room_id");

			try {
				crmService.dodelMeetingRoomInfoById(room_id);
				jsonStr.setSuccess(true);
				jsonStr.setMessageTitle("成功");
				jsonStr.setMessage("删除成功!");
			} catch (Exception e) {
				jsonStr.setSuccess(false);
				jsonStr.setMessageTitle("失败");
				jsonStr.setMessage("删除失败!");
			}
			WebUtil.returnResponse(response, jsonStr.toJsonString());
	}
	
	/**
	 * 车辆管理
	 * */
	public void doVehicleManagement(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try{
			VehicleManagement vm = new VehicleManagement();
			String car_id = request.getParameter("car_id");
			String car_No = request.getParameter("car_No");
			String car_brand = request.getParameter("car_brand");
			String car_type = request.getParameter("car_type");
		
			String remark = request.getParameter("remark");
			if("".equals(request.getParameter("capacity"))){
				Integer capacity = null;
//				pf.setPatentedDesignPeopleID2(patentedDesignPeopleID2);
			}else{
				Integer capacity = Integer.valueOf(request.getParameter("capacity"));
				vm.setCapacity(capacity);
			}
			if(request.getParameter("purchase_date") == ""){
				
			}else{
				Date purchase_date = Date.valueOf(request.getParameter("purchase_date"));
				vm.setPurchase_date(purchase_date);
			}
			
			vm.setCar_id(car_id);
			vm.setCar_No(car_No);
			vm.setCar_brand(car_brand);
			vm.setCar_type(car_type);
			
			vm.setRemark(remark);
			crmService.doVehicleManagement(vm);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功！");
		
		}catch(Exception e){
			logger.error("保存车辆信息出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	
	/**
	 * 公务车辆查询
	 * */
	public void getVehicleManagement(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String store = crmService.getVehicleManagement();
		WebUtil.returnResponse(response, store);
	}
	
	/**
	 * 公务车辆赋值
	 * */
	public void getVehicleManagemenid(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String car_id = request.getParameter("car_id");
		String store = crmService.getVehicleManagemenid(car_id);
		WebUtil.returnResponse(response, store);
		
	}
	
	/**
	 * 车辆信息删除
	 * */
	public void dodelVehicleManagemenById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		 

			ExtjsAjaxResponse jsonStr = new ExtjsAjaxResponse();
			String car_id = request.getParameter("car_id");

			try {
				crmService.dodelVehicleManagemenById(car_id);
				jsonStr.setSuccess(true);
				jsonStr.setMessageTitle("成功");
				jsonStr.setMessage("删除成功!");
			} catch (Exception e) {
				jsonStr.setSuccess(false);
				jsonStr.setMessageTitle("失败");
				jsonStr.setMessage("删除失败!");
			}
			WebUtil.returnResponse(response, jsonStr.toJsonString());
	}
	
	/**
	 * 办公用品
	 * */
	public void doOfficeSupplies(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try{
			OfficeSuppliesDetails osds = new OfficeSuppliesDetails();
			String supplies_id = request.getParameter("supplies_id");
			String supplies_type_id = request.getParameter("supplies_type_id");
			String supplies_name = request.getParameter("supplies_name");
			String supplies_brand = request.getParameter("supplies_brand");
			String supplies_model = request.getParameter("supplies_model");
//			Date  purchase_date = Date.valueOf(request.getParameter("purchase_date"));	
			String remark = request.getParameter("remark");
			
			
			if("".equals(request.getParameter("qty"))){
				Integer qty = null;
//				pf.setPatentedDesignPeopleID2(patentedDesignPeopleID2);
			}else{
				Integer qty = Integer.valueOf(request.getParameter("qty"));
				osds.setQty(qty);
			}
			
			if(request.getParameter("purchase_date") == ""){
				
			}else{
				Date  purchase_date = Date.valueOf(request.getParameter("purchase_date"));
				osds.setPurchase_date(purchase_date);
			}
			
			osds.setSupplies_id(supplies_id);
			osds.setSupplies_type_id(supplies_type_id);
			osds.setSupplies_name(supplies_name);
			osds.setSupplies_model(supplies_model);
			osds.setSupplies_brand(supplies_brand);
			osds.setRemark(remark);
			
			
			
			
			crmService.doOfficeSupplies(osds);
			
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功！");
		
		}catch(Exception e){
			logger.error("保存办公用品出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	/**
	 * 办公用品查询
	 * */
	public void getOfficeSupplies(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");
		String store = "";
		store = crmService.getOfficeSupplies(pageInfo,queryStr);
		WebUtil.returnResponse(response, store);
	}
	/**
	 * 办公用品赋值
	 * */
	public void getOfficeSuppliesid(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String supplies_id = request.getParameter("supplies_id");
		String store = crmService.getOfficeSuppliesid(supplies_id);
		WebUtil.returnResponse(response, store);
		
	}
	
	/**
	 * 办公用品删除
	 * */
	
	public void dodelOfficeSuppliesById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		 

			ExtjsAjaxResponse jsonStr = new ExtjsAjaxResponse();
			String supplies_id = request.getParameter("supplies_id");

			try {
				crmService.dodelOfficeSuppliesById(supplies_id);
				jsonStr.setSuccess(true);
				jsonStr.setMessageTitle("成功");
				jsonStr.setMessage("删除成功!");
			} catch (Exception e) {
				jsonStr.setSuccess(false);
				jsonStr.setMessageTitle("失败");
				jsonStr.setMessage("删除失败!");
			}
			WebUtil.returnResponse(response, jsonStr.toJsonString());
	}
	
	/**
	 * 人员角色
	 * */
	public void doStaffRoles(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try{
			
			String mycarsID = request.getParameter("mycars");
			String[] emps = mycarsID.split(",");
			Integer userID = Integer.valueOf(request.getParameter("userID"));
			crmService.dodelSataffRoles(userID);
			
			List<Map> list = new ArrayList<Map>();
			for (int i = 0; i < emps.length; i++) {
				String id = "bbz-"+userID+"-"+emps[i];
				String empID = emps[i];
				crmService.doStaffRoles(id,userID,empID);
			}
			
			
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功！");
		
		}catch(Exception e){
			logger.error("保存人员角色出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	
	public void getStaffRoles(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		
		Integer userID = Integer.valueOf(request.getParameter("userID"));
		
		String store=	crmService.getStaffRoles(userID);
		
		WebUtil.returnResponse(response, store);
	}
	
	public void getDepartment(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		
		String store=	crmService.getDepartment();
		
		WebUtil.returnResponse(response, store);
	}
	
	public void getDepartmentID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		Integer userID = Integer.valueOf(request.getParameter("userID"));
		String store = crmService.getDepartmentID(userID);
		WebUtil.returnResponse(response, store);
	}
	
	public void getOfficeSuppliesCombot(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		
		String store = crmService.getOfficeSuppliesCombot();
		WebUtil.returnResponse(response, store);
		
	}
	
	public void doStaffRolestty(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try{
			
			String displayID = request.getParameter("displayText");
			String[] emjs = displayID.split(",");
			Integer userID = Integer.valueOf(request.getParameter("userID"));
			
			
			String mycarsID = request.getParameter("mycars");
			String[] emps = mycarsID.split(",");
			String roet = request.getParameter("roet");
			String[] roe = roet.split(",");
		
			crmService.dodelSataffRolestty(userID);
			crmService.dodelSataffRolesteqw(userID);
			List<Map> list = new ArrayList<Map>();
			for (int i = 0; i < emps.length; i++) {
				
				String empID = emps[i];
				String enjsID = emjs[i];
				String roeID = roe[i];
				crmService.doStaffRolestty(enjsID,userID,empID);
				
				crmService.doStaffRoles(userID,roeID);
			}
			
			
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功！");
		
		}catch(Exception e){
			logger.error("保存人员职务出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		
	}
	
	public void doStaffRolesttyryyu(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		ExtjsAjaxResponse jsonStr = new ExtjsAjaxResponse();
		Integer userID = Integer.valueOf(request.getParameter("userID"));

		try {
			crmService.dodelSataffRolestty(userID);
			crmService.dodelSataffRolesteqw(userID);
			jsonStr.setSuccess(true);
			jsonStr.setMessageTitle("成功");
			jsonStr.setMessage("职责变更成功!");
		} catch (Exception e) {
			jsonStr.setSuccess(false);
			jsonStr.setMessageTitle("失败");
			jsonStr.setMessage("职责变更失败!");
		}
		WebUtil.returnResponse(response, jsonStr.toJsonString());
	}
}
