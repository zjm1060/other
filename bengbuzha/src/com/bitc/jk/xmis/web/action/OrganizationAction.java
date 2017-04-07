//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\OrganizationAction.java

package com.bitc.jk.xmis.web.action;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.Date;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.servlet.ServletContext;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.sf.json.JSONObject;
import net.sf.json.JsonConfig;
import net.sf.json.util.CycleDetectionStrategy;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;
import org.dom4j.Document;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;
import org.wltea.expression.ExpressionEvaluator;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.T_OnWatch_schedule;
import com.bitc.jk.xmis.model.crm.Address;
import com.bitc.jk.xmis.model.crm.City;
import com.bitc.jk.xmis.model.sys.Degree;
import com.bitc.jk.xmis.model.sys.DutyLevel;
import com.bitc.jk.xmis.model.sys.Echnicalposition;
import com.bitc.jk.xmis.model.sys.IdentityObj;
import com.bitc.jk.xmis.model.sys.ProfessionalLevel;
import com.bitc.jk.xmis.model.sys.WorkType;
import com.bitc.jk.xmis.msg.fastmsg.FastMsg;
import com.bitc.jk.xmis.msg.fastmsg.FastMsgException;
import com.bitc.jk.xmis.service.OrganizationService;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.exception.TimeOutException;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.SpringInit;
import com.bitc.jk.xmis.web.action.param.OrgTreeParam;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;

public class OrganizationAction extends DispatchAction {
	private static Logger logger = Logger.getLogger(OrganizationAction.class);

	private OrganizationService organizationService;

	/**
	 * @roseuid 4C1F03AE002E
	 */
	public OrganizationAction() {

	}

	protected ActionForward unspecified(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {

		logger.info("---> Go to org_maint ! ");

		return org_maint(mapping, form, request, response);

	}

	/**
	 * Access method for the orgainzationService property.
	 * 
	 * @return the current value of the orgainzationService property
	 */
	public OrganizationService getOrganizationService() {
		return organizationService;
	}

	/**
	 * Sets the value of the orgainzationService property.
	 * 
	 * @param aOrgainzationService
	 *            the new value of the orgainzationService property
	 */
	public void setOrganizationService(OrganizationService aOrganizationService) {
		organizationService = aOrganizationService;
	}

	public ActionForward org_maint(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> org_maint ");

		String forwordStr = WebKeys.Forword_to_orgMaint;

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				forwordStr = WebKeys.Forword_to_LoginFailed;
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
			} else {
				request.setAttribute(WebKeys.Request_Attribute_Name_Type,
						WebKeys.Request_Attribute_Name_Type_Employee);
			}

		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
		return (mapping.findForward(forwordStr));
	}

	public ActionForward deptTree(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> deptTree ");

		String forwordStr = WebKeys.Forword_to_deptTree;

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				forwordStr = WebKeys.Forword_to_LoginFailed;
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
			} else {
				organizationService.setWebDir(request.getSession()
						.getServletContext().getRealPath(""));

				organizationService.setWebPath(request.getScheme() + "://"
						+ request.getServerName() + ":"
						+ request.getServerPort() + request.getContextPath()
						+ "/");

				organizationService.buildDepartmentTree(userObj);
				request.setAttribute(WebKeys.Request_Attribute_Name_Type,
						WebKeys.Request_Attribute_Name_Type_Employee);
				request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
						WebKeys.Request_Attribute_Name_SubType_departmentTree);
			}

		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}
		return (mapping.findForward(forwordStr));
	}

	public ActionForward divisionTotalSummary(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> divisionTotalSummary");

		String forwordStr = WebKeys.Forword_to_DivisionTotalSummary;

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				forwordStr = WebKeys.Forword_to_LoginFailed;
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
			} else {

				String divTotalSumData = organizationService
						.getProjectListByDiv(userObj.getDivisionID());

				request.setAttribute(
						WebKeys.Request_Attribute_Name_ProjListByDiv,
						divTotalSumData);
				request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
						WebKeys.Request_Attribute_Name_SubType_departmentTree);
				request.setAttribute(
						WebKeys.Request_Attribute_Name_Grid_Title_Prefix,
						userObj.getDivisionName());
			}

		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}

		return (mapping.findForward(forwordStr));
	}

	public ActionForward deptTotalSummary(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> deptTotalSummary" + "//"
				+ request.getParameter(WebKeys.Type_Department_Name));

		String forwordStr = WebKeys.Forword_to_DepartmentTotalSummary;

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				forwordStr = WebKeys.Forword_to_LoginFailed;
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
			} else {

				request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
						WebKeys.Request_Attribute_Name_SubType_departmentTree);
				request.setAttribute(
						WebKeys.Request_Attribute_Name_Grid_Title_Prefix,
						request.getParameter(WebKeys.Type_Department_Name));

				request.setAttribute(
						WebKeys.Request_Attribute_Name_Department,
						request.getParameter(WebKeys.Request_Attribute_Name_Department));
			}

		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}

		return (mapping.findForward(forwordStr));
	}

	public void getDeptTotalSummaryStore(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getDeptTotalSummaryStore");

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
				throw new Exception("Timeout!");
			} else {

				int start = new Integer(request.getParameter("start"))
						.intValue();
				int limit = new Integer(request.getParameter("limit"))
						.intValue();
				String sort = request.getParameter("sort");
				String dir = request.getParameter("dir");

				PageInfo pageInfo = new PageInfo();
				pageInfo.setStart(new Integer(request.getParameter("start"))
						.intValue());
				pageInfo.setLimit(new Integer(request.getParameter("limit"))
						.intValue());
				pageInfo.setSort(request.getParameter("sort"));
				pageInfo.setDir(request.getParameter("dir"));

				int deptID = new Integer(
						request.getParameter(WebKeys.Type_Department))
						.intValue();

				Document doc = organizationService.getEmpTotalSummary(deptID,
						pageInfo);

				request.setAttribute("success", "true");
				response.setContentType("text/xml; charset=utf-8");
				OutputFormat format = OutputFormat.createCompactFormat();
				format.setEncoding("utf-8");
				PrintWriter out = response.getWriter();
				XMLWriter writer = new XMLWriter(out, format);

				writer.write(doc);
				writer.flush();
				writer.close();

			}

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}
	}

	public ActionForward getEmployee(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getEmployee");

		String forwordStr = WebKeys.Forword_to_Employee;

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new Exception("Timeout!");
			} else {

				request.setAttribute(WebKeys.Request_Attribute_Name_SubType,
						WebKeys.Request_Attribute_Name_SubType_departmentTree);

				request.setAttribute(
						WebKeys.Request_Attribute_Name_EmployeeObjert,
						organizationService.getEmployeeInfo(new Integer(request
								.getParameter("emp")).intValue()));

				Calendar c = Calendar.getInstance();
				c.setTime(new Date(System.currentTimeMillis()));
				int dateOfMonth = c.get(Calendar.DATE);

				request.setAttribute(
						WebKeys.Request_Attribute_Name_CurrentDATE,
						String.valueOf(dateOfMonth));
			}

		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}

		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		JSONObject array = JSONObject.fromObject(organizationService
				.getEmployeeInfo(new Integer(request.getParameter("emp"))
						.intValue()), config);
		WebUtil.returnResponse(response, array.toString());
		return null;

	}

	public ActionForward getCompanySimple(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String companyIDs = request.getParameter("companyID");
		if (StringUtils.isEmpty(companyIDs)) {
			logger.error("查询公司时提供的公司ID为空");
			throw new Exception("查询公司时提供的公司ID为空");
		}
		int companyID = Type.GetInt(companyIDs);
		String store = organizationService.getCompanyStore(companyID);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getOrgTree(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		logger.info("---> getOrgTree (action)");

		String node = request.getParameter("node");
		int compID = Type.GetInt(request.getParameter("compID"));
		String store = organizationService.getOrgTree(node, compID);

		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getPoliticalStatusTotalSummary(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		logger.info("---> getPoliticalStatusTotalSummary (action)");

		String store = organizationService.getPoliticsStatusTotalSummary();
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getPositionTotalSummary(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		logger.info("---> getPositionTotalSummary (action)");

		String store = organizationService.getPositionTotalSummary();
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getOneEmployee(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		logger.info("---> getOneEmployee (action)");
		logger.info("---> getOneEmployee employeeId="
				+ request.getParameter("employeeId"));

		String employeeId = request.getParameter("employeeId");
		if (employeeId != null && !"".equals(employeeId.trim())) {
			String jsonstr = organizationService
			.getEmployee(Type.GetInt(employeeId.substring(9)));
			WebUtil.returnResponse(response, jsonstr);
			logger.info(jsonstr);
			Calendar c = Calendar.getInstance();
			c.setTime(new Date(System.currentTimeMillis()));
			int dateOfMonth = c.get(Calendar.DATE);
			request.setAttribute(WebKeys.Request_Attribute_Name_CurrentDATE,
					String.valueOf(dateOfMonth));
		}
		return null;
	}

	// public String getEmployeeList(String ID)
	public ActionForward getEmployeeList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String ID = request.getParameter("id");
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = organizationService.getEmployeeList(ID,
				pageInfo.getStart(), pageInfo.getLimit());
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward checkLoginNameExist(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		logger.info("-->checkLoginNameExist loginName="
				+ request.getParameter("loginName") + " userid="
				+ request.getParameter("userid"));
		String loginName = request.getParameter("loginName");
		int userid = Type.GetInt(request.getParameter("userid"));
		boolean flag = organizationService.checkLoginNameExist(loginName,
				userid);
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse(flag, "提示",
				"登录名已存在,请更换！");
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	/**
	 * 人员树右键保存全部 返回最后一个nodepath
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 */
	public ActionForward saveAllOperation(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		logger.info("---> saveAllOperation (action)");
		logger.info("---> newDept=" + request.getParameter("newDept"));
		logger.info("---> newEmpl=" + request.getParameter("newEmpl"));
		logger.info("---> newMapping=" + request.getParameter("newMapping"));
		logger.info("---> dragRelation=" + request.getParameter("dragRelation"));
		logger.info("---> changeEmplName="
				+ request.getParameter("changeEmplName"));
		logger.info("---> changeDeptName="
				+ request.getParameter("changeDeptName"));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			Map<String, String> deptMap = WebUtil.strToMap(request
					.getParameter("newDept"));
			Map<String, String> emplMap = WebUtil.strToMap(request
					.getParameter("newEmpl"));
			List<List<String>> mappingList = WebUtil.strToList(request
					.getParameter("newMapping"));
			List<List<String>> dragList = WebUtil.strToList(request
					.getParameter("dragRelation"));
			Map<String, String> changeEmplName = WebUtil.strToMap(request
					.getParameter("changeEmplName"));
			Map<String, String> changeDeptName = WebUtil.strToMap(request
					.getParameter("changeDeptName"));

			EmployeeObj userObj = WebUtil.getCurrentEmployee(request
					.getSession());
			String nodepath = organizationService.doSaveAllOperation(deptMap,
					emplMap, mappingList, dragList, changeEmplName,
					changeDeptName, userObj);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("nodepath", nodepath);
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败\n" + e.getMessage());
			ajaxResponse.setMessageTitle("提示");
			request.setAttribute(WebKeys.Request_Attribute_Name_ErrMessage,
					e.getMessage());

		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward updatePost(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		logger.info("---> updatePost (action)");

		int id = Type.GetInt(request.getParameter("id"));
		int postId = Type.GetInt(request.getParameter("postID"));
		String postName = organizationService.updatePost(id, postId);
		ExtjsAjaxResponse ajaxResponse = null;
		if ("error".equals(postName)) {
			ajaxResponse = new ExtjsAjaxResponse(false, "提示", "更改职务失败！");
		} else {
			ajaxResponse = new ExtjsAjaxResponse(true, "提示", "更改职务成功！");
			Map<String, Object> attributes = new HashMap<String, Object>();
			attributes.put("postName", postName);
			ajaxResponse.setAttributes(attributes);
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public void updatePwd(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		final String password = request.getParameter("pwd");
		logger.info("---> updatePwd //" + request.getParameter("userid") + "//"
				+ password);
		String message;

		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new Exception("Timeout!");
			} else {
				final int userId = Integer.parseInt(request
						.getParameter("userid"));
				if (organizationService.doChangePassword(userId, password,
						userObj.getUserID()) > 0) {
					ServletContext servletContext = request.getSession()
							.getServletContext();
					/*
					 *20140214 fastMsg密码修改同步*/
					 String syncfastmsg = (String) servletContext
							.getAttribute("syncfastmsg");
					if ("true".equals(syncfastmsg)) {
						final FastMsg fm = SpringInit.getBean("fastMsg",
								FastMsg.class);
						new Thread(new Runnable() {
							public void run() {
								try {
									fm.changePassword(userId, password);
								} catch (FastMsgException e) {
									logger.error(e);
								}
							}
						}).start();
					}
					message = "密码保存成功！";
				} else {
					message = "密码保存异常，请重新操作或联系管理员，谢谢！";
				}

			}

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
			message = "密码保存失败，请重新登录后再操作，谢谢！";
		}

		try {
			response.setContentType("text/xml; charset=UTF-8");
			response.setHeader("Cache-Control", "no-cache");
			PrintWriter out = response.getWriter();
			out.append(message);
			out.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void saveEducation(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> saveEducation //" + request.getParameter("userid")
				+ "//" + request.getParameter("graduationDate"));

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
				EmployeeObj emp = new EmployeeObj();
				emp.setUserID(Integer.parseInt(request.getParameter("userid")));
				if (!"".equals(request.getParameter("eduID").trim())) {
					emp.setEducationHighestLevel(Integer.parseInt(request
							.getParameter("eduID")));
				}
				emp.setEducationLevelName(request.getParameter("eduLevel")
						.trim());
				emp.setSchoolName(request.getParameter("schoolName").trim());
				emp.setGraduationDate("".equals(request.getParameter(
						"graduationDate").trim()) ? null : Date.valueOf(request
						.getParameter("graduationDate").trim()));
				emp.setMajor(request.getParameter("major").trim());
				emp.setEducationRemark(request.getParameter("eduRemark").trim());

				emp.setEducationHighestLevel(organizationService
						.doEditEducation(request.getParameter("oldEduName")
								.trim(), emp, userObj.getUserID()));

				String messages = "";
				String messageTitle = "";
				boolean flag = false;

				if (emp.getEducationHighestLevel() > 0) {

					messages = "学历记录保存成功";
					messageTitle = "成功";
					flag = true;

				} else {
					messages = "学历记录保存失败！";
					messageTitle = "失败";
				}

				jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
						+ messages + "',messageTitle:'" + messageTitle
						+ "',eduID:'"
						+ String.valueOf(emp.getEducationHighestLevel())
						+ "',eduName:'" + emp.getEducationLevelName()
						+ "',eduSchool:'" + emp.getSchoolName() + "',eduDate:'"
						+ emp.getGraduationDate().toString().trim()
						+ "',eduMajor:'" + emp.getMajor() + "',eduRemark:'"
						+ emp.getEducationRemark() + "'}";

			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'保存失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		try {
			// 返回成功标识
			response.setContentType("text/html; charset=utf-8");
			response.getWriter().write(jsonStr);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				response.getWriter().close();

			} catch (IOException e) {
				e.printStackTrace();
			}
		}

	}

	public ActionForward saveEmpInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> saveEmpInfo //"
				+ request.getParameter("joinDate").trim() + "//");

		String messages = "";
		String messageTitle = "";
		boolean flag = true;
		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("TimeOut！请重新登录。");
			} else {

				EmployeeObj empObj = new EmployeeObj();
				
				empObj.setFirstName(request.getParameter("firstName"));

				empObj.setUserID(Integer.parseInt(request
						.getParameter("userid")));
				empObj.setLastName(request.getParameter("lastName"));
				empObj.setLoginName(request.getParameter("loginName"));
				empObj.setSexID(Integer.parseInt(request.getParameter("sex")));
				empObj.setJoinDate((request.getParameter("joinDate") == null || ""
						.equals(request.getParameter("joinDate").trim())) ? null
						: Date.valueOf(request.getParameter("joinDate").trim()));
				empObj.setBirthday("".equals(request.getParameter("birthday")) ? null
						: Date.valueOf(request.getParameter("birthday")));
				empObj.setIDCard(request.getParameter("IDCard"));
				empObj.setMobileNo(request.getParameter("mobileNo"));
				empObj.setEmailAddress(request.getParameter("email"));
				empObj.setWebPage(request.getParameter("webPage"));
				empObj.setPosition(request.getParameter("position"));
				empObj.setSwitchboardNo(request.getParameter("switchboardNo"));
				empObj.setExtNo(request.getParameter("extNo"));
				empObj.setFaxNo(request.getParameter("faxNo"));
				empObj.setHomeAddress(request.getParameter("homeAddress"));
				empObj.setZipCode(request.getParameter("zipCode"));
				empObj.setHomeTelNo(request.getParameter("homeTelNo"));
				empObj.setOfficeAddress(request.getParameter("officeAddress"));
				// 非中水的职务保存
				empObj.setPositionEdit(request.getParameter("positionEdit"));
				if (request.getParameter("politicalStatus") != null
						&& !"".equals(request.getParameter("politicalStatus")
								.trim())) {
					empObj.setPoliticalStatus(Integer.parseInt(request
							.getParameter("politicalStatus")));
				}

				empObj.setNativePlace(request.getParameter("nativePlace"));
				if (request.getParameter("maritalStatus") != null
						&& !"".equals(request.getParameter("maritalStatus")
								.trim())) {
					empObj.setMaritalStatus(Integer.parseInt(request
							.getParameter("maritalStatus")));
				}

				empObj.setSpouseName(request.getParameter("spouseName"));
				empObj.setIssueName(request.getParameter("issueName"));
				if (request.getParameter("issueSex") != null
						&& !"".equals(request.getParameter("issueSex").trim())) {
					empObj.setIssueSex(Integer.parseInt(request
							.getParameter("issueSex")));
				}

				empObj.setHobby(request.getParameter("hobby"));
				empObj.setSpecialty(request.getParameter("specialty"));
				empObj.setUrgentinfo(request.getParameter("urgentinfo"));// 禁止联系方式

				empObj.setDisabledFlag(Integer.parseInt(request
						.getParameter("disabledFlag")));

				if (Integer.parseInt(request.getParameter("disabledFlag")) == 1) {
					empObj.setLocked(true);
				} else {
					empObj.setLocked(false);
				}
				empObj.setRemark(request.getParameter("remark"));

				Address address = new Address();
				BeanUtils.populate(address, request.getParameterMap());
				if (StringUtils.isEmpty(address.getAddressID())) {
					address.setAddressID(null);
				}
				String zipCodeAddress = request.getParameter("zipCodeAddress");
				address.setZipCode(Type.getInteger(zipCodeAddress));
				String cityID = request.getParameter("cityID");
				City city = new City();
				city.setCityID(Type.getInteger(cityID));
				address.setCity(city);
				empObj.setAddress(address);
				
				// 职务级别
				String dutylevelID = request.getParameter("dutylevelCombo");
				if (!StringUtils.isEmpty(dutylevelID)
						&& !dutylevelID.equals("-1")) {
					DutyLevel dutylevel = new DutyLevel();
					dutylevel.setId(Type.getIntegerWithNull(dutylevelID));
					empObj.setDutylevel(dutylevel);
				}
				
				// 技术职务
				String echnicalpositionID = request.getParameter("echnicalpositionCombo");
				if (!StringUtils.isEmpty(echnicalpositionID)
						&& !echnicalpositionID.equals("-1")) {
					Echnicalposition echnicalposition = new Echnicalposition();
					echnicalposition.setId(Type.getIntegerWithNull(echnicalpositionID));
					empObj.setEchnicalposition(echnicalposition);
				}
				
				// 专业技术级别
				String professionallevelID = request.getParameter("professionallevelCombo");
				if (!StringUtils.isEmpty(professionallevelID)
						&& !professionallevelID.equals("-1")) {
					ProfessionalLevel professionallevel = new ProfessionalLevel();
					professionallevel.setId(Type.getIntegerWithNull(professionallevelID));
					empObj.setProfessionallevel(professionallevel);
				}
				
				// 现有文化程度
				String degreeID = request.getParameter("DegreeCombo");
				if (!StringUtils.isEmpty(degreeID)
						&& !degreeID.equals("-1")) {
					Degree degree = new Degree();
					degree.setId(Type.getIntegerWithNull(degreeID));
					empObj.setDegree(degree);
				}
				
				// 工种
				String worktypeID = request.getParameter("WorkTypeCombo");
				if (!StringUtils.isEmpty(worktypeID)
						&& !worktypeID.equals("-1")) {
					WorkType worktype = new WorkType();
					worktype.setId(Type.getIntegerWithNull(worktypeID));
					empObj.setWorktype(worktype);
				}
				
				// 身份
				String identityID = request.getParameter("identityCombo");
				if (!StringUtils.isEmpty(identityID)
						&& !identityID.equals("-1")) {
				IdentityObj identity = new IdentityObj();
				identity.setId(Type.getIntegerWithNull(identityID));
					empObj.setIdentity(identity);
				}
				empObj.setNation(request.getParameter("nation"));
				empObj.setGetqualificationtime(Type.getDate(request.getParameter("getqualificationtime")));
				empObj.setText1(Type.getDate(request.getParameter("text1")));
				empObj.setWagelevel(Type.getInteger(request.getParameter("wagelevel")));
				empObj.setJoinworkdate(Type.getDate(request.getParameter("joinworkdate")));
				empObj.setJoinpartydate(Type.getDate(request.getParameter("joinpartydate")));
				empObj.setJoingroupdate(Type.getDate(request.getParameter("joingroupdate")));
				empObj.setRetirementdate(Type.getDate(request.getParameter("retirementdate")));
				empObj.setThetimeandschoolandprofessional(request.getParameter("thetimeandschoolandprofessional"));
				empObj.setRemark1(request.getParameter("remark1"));
				empObj.setRemark2(request.getParameter("remark2"));
				empObj.setRemark3(request.getParameter("remark3"));
				
				organizationService.doSaveEmployeeInfo(empObj, userObj);
				response.setContentType("text/html; charset=utf-8");
				messages = "用户信息保存成功！";
				messageTitle = "成功";
			}

		} catch (TimeOutException te) {
			te.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_ErrMessage,
					e.getMessage());
			messages = "用户信息保存异常，请重新操作或联系管理员！";
			messageTitle = "失败";
			flag = false;
		}
		try {
			response.getWriter().write(
					"{success:" + String.valueOf(flag) + ",message:'"
							+ messages + "',messageTitle:'" + messageTitle
							+ "'}");
		} catch (Exception e) {
			e.printStackTrace();
		}

		return null;
	}

	public ActionForward disableTreeNode(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> disableTreeNode //" + request.getParameter("item"));

		String message = "";
		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("TimeOut！请重新登录。");
			} else {

				if (organizationService.doDisableTreeNode(
						request.getParameter("item"), userObj) > 0)
					response.getWriter().write(
							String.valueOf("禁用成功！如需重新启用，请联系管理员！"));
			}

		} catch (TimeOutException te) {
			te.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			return (mapping.findForward(WebKeys.Forword_to_LoginFailed));

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_ErrMessage,
					e.getMessage());

			message = "禁用操作失败\n" + e.getMessage();

		}

		try {
			response.getWriter().write(message);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;

	}

	public void changeLunchAddress(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> changeLunchAddress //"
				+ request.getParameter("userid") + "//"
				+ request.getParameter("newAddress"));

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

				String messages = "";
				String messageTitle = "";
				boolean flag = false;

				if (organizationService.doChangeLunchAddress(
						Integer.parseInt(request.getParameter("newAddress")),
						Integer.parseInt(request.getParameter("userid")),
						userObj.getUserID()) > 0) {

					messages = "午餐地址保存成功";
					messageTitle = "成功";
					flag = true;

				} else {
					messages = "午餐地址保存失败！";
					messageTitle = "失败";
				}

				jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
						+ messages + "',messageTitle:'" + messageTitle + "'}";

			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'保存失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		try {
			// 返回成功标识
			response.setContentType("text/html; charset=utf-8");
			response.getWriter().write(jsonStr);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				response.getWriter().close();

			} catch (IOException e) {
				e.printStackTrace();
			}
		}

	}

	public void getLunchAddressList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> getLunchAddressList");

		String dataStore = "";

		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new Exception("Timeout!");
			} else {

				dataStore = organizationService
						.getLunchAddressListByDiv(userObj.getDivisionID());

				logger.info(dataStore);

			}

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		}

		try {
			// 返回成功标识
			response.setContentType("text/html; charset=utf-8");
			response.getWriter().write(dataStore);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				response.getWriter().close();

			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public void lunchAddressJournal(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> lunchAddressJournal //");

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

				String messages = "";
				String messageTitle = "";
				boolean flag = false;

				if (organizationService.doBookLunchAddress() > 0) {

					messages = "午餐地址 Journal 成功";
					messageTitle = "成功";
					flag = true;

				} else {
					messages = "午餐地址 Journal 失败！";
					messageTitle = "失败";
				}

				jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
						+ messages + "',messageTitle:'" + messageTitle + "'}";

			}
		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			jsonStr = "{success:false,message:'Journal失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}

		try {
			// 返回成功标识
			response.setContentType("text/html; charset=utf-8");
			response.getWriter().write(jsonStr);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				response.getWriter().close();

			} catch (IOException e) {
				e.printStackTrace();
			}
		}

	}

	public ActionForward createNewUser(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> createNewUser //" + request.getParameter("newDept")
				+ "//" + request.getParameter("newEmp") + "//"
				+ request.getParameter("empDeptMapping"));

		String message = "";
		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("TimeOut！请重新登录。");
			} else {

				Map deptMap = WebUtil.getNewPart(request
						.getParameter("newDept"));
				Map empMap = WebUtil.getNewPart(request.getParameter("newEmp"));
				Map empDeptMapping = WebUtil.getNewPart(request
						.getParameter("empDeptMapping"));

				// logger.debug(String.valueOf(empMap.size()));

				if (organizationService.doCreateNewBatch(deptMap, empMap,
						empDeptMapping, userObj) > 0)
					response.getWriter().write(String.valueOf("创建成功！"));

			}

		} catch (TimeOutException te) {
			te.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

			return (mapping.findForward(WebKeys.Forword_to_LoginFailed));

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_ErrMessage,
					e.getMessage());

			message = "创建不成功\n" + e.getMessage();

		}

		try {
			response.getWriter().write(message);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	// public void uploadPhoto(ActionMapping mapping, ActionForm form,
	// HttpServletRequest request, HttpServletResponse response) {
	//
	// logger.info("---> uploadPhoto //" + request.getParameter("userfile"));
	//
	// String filePath = "D:\\front\\photo\\pic\\";
	// String messages = "";
	// String forward = "";
	//
	// SmartUpload su = new SmartUpload();
	// long maxsize = 2 * 1024 * 1024; // 设置每个上传文件的大小，为2MB
	// String allowedFilesList = "jpg,gif,bmp";
	// String denidFilesList = "exe,bat,jsp,htm,html,,";
	//
	// try {
	// ApplicationContext appContext = WebApplicationContextUtils.
	// 18. getWebApplicationContext(this.getServlet().getServletContext());
	// 19. return (FileService) appContext.getBean("fileService");
	//
	// ApplicationContext ctx = WebApplicationContextUtils
	// .getRequiredWebApplicationContext(this.getServlet().getServletConfig()
	// .getServletContext());
	// ctx.getBean(arg0)
	// su.initialize(this.getServlet().getServletConfig(), request,
	// response); // 初始化
	// su.setMaxFileSize(maxsize); // 限制上传文件的大小
	// su.setAllowedFilesList(allowedFilesList); // 设置允许上传的文件类型
	// su.setDeniedFilesList(denidFilesList);
	// su.upload(); // 上传文件
	//
	// File file = su.getFiles().getFile(0); // 获取上传的文件，因为只上传了一个文件，所以可直接获取
	//
	// if (!file.isMissing()) { // 如果选择了文件
	// String now = new Date().getTime() + ""; // 获取当前时间并格式化为字符串
	// String photoAddr = filePath + now + "." + file.getFileExt(); // filePath值
	//
	// file.saveAs(photoAddr, File.SAVEAS_VIRTUAL);
	//
	// } else {
	// messages = "请选择要上传的文件！";
	// }
	//
	// } catch (Exception e) {
	// e.printStackTrace();
	// }
	//
	// }

	public ActionForward test(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		logger.info("-->test action");
		List<String> list = (List<String>) ExpressionEvaluator
				.evaluate("$joinList($getEmpIdsByPost(507,8),$getEmpIdsByPost(508,6))");
		logger.info(list);
		return null;
	}

	public ActionForward getOrganizationTree(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		logger.info("---> getOrganizationTree (action)");

		String node = request.getParameter("node");
		logger.info("---> getOrganizationTree (action)" + node);
		int type = Type.GetInt(request.getParameter("type"));
		String store = organizationService.getOrganizationTree(node, type);

		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward orgTreeLoader(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		OrgTreeParam param = new OrgTreeParam();
		BeanUtils.populate(param, request.getParameterMap());
		String store = organizationService.getOrganizationTree(param)
				.toJsonString(param.getJsonConfig());
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getEmpInfoByID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		logger.info("---->addressBook(action--getEmpInfoByID)");

		int deptNo = Type.GetInt(request.getParameter("deptNo"));

		// List<EmployeeObj> list = organizationService
		// .getEmployeeInfoByDeptID(deptNo);
		List list = organizationService.getEmployeeInfoByDeptID(deptNo);

		String jsStr = WebUtil.buildJsonStoreByList(list);
		logger.info(jsStr);
		WebUtil.returnResponse(response, jsStr);
		return null;
	}

	public ActionForward getEmps(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		StandardQuery inklingQuery = WebUtil
				.generateStandardQueryByInkling(request);
		StandardQuery query;
		query = CompositParam.buildAndCompositParam().addStandardQuery(
				inklingQuery);
		List list = organizationService.getEmployeeInfoByQuery(query);
		String jsStr = WebUtil.buildJsonStoreByList(list);
		WebUtil.returnResponse(response, jsStr);
		return null;
	}

	public ActionForward getLeaderInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		logger.info("---->addressBook(action--getLeaderInfo)");
		// List<EmployeeObj> list = organizationService.getLeaderInfo();
		List list = organizationService.getLeaderInfo();
		String jsStr = WebUtil.buildJsonStoreByList(list);
		// logger.info(jsStr);
		WebUtil.returnResponse(response, jsStr);
		return null;
	}

	// 根据人名查询
	public void getEmpListByName(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String name = request.getParameter("name");
		if (!StringUtils.isEmpty(name)) {
			name = name.trim();
		}
		String compID = request.getParameter("compID");
		String isAdmin = request.getParameter("isAdmin");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String nodePath = organizationService.getEmpListByName(compID,
					name, isAdmin);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("nodepath", nodePath);
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public ActionForward getCustomGroupTree(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		logger.info("---> getCustomGroupTree (action)");
		String node = request.getParameter("node");
		String groupID = Type.getString(request.getParameter("groupID"));
		String store = organizationService.getCustomGroupTree(node, groupID);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public void doDeleteCustomGroup(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String node = Type.getString(request.getParameter("node"));
			String father_groupid = node.split("__")[1].split("_")[1];
			String groupid = node.split("__")[0].split("_")[1];
			String type = Type.getString(request.getParameter("type"));
			boolean deleteSuccess = organizationService.doDeleteCustomGroup(
					node, groupid, father_groupid, type);
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
			ajaxResponse.setMessage("删除失败，请重新登录后再操作，谢谢！");
			ajaxResponse.setMessageTitle("失败");

		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public void doSaveAllOfCustomGroup(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		logger.info("---> changeGrouName="
				+ request.getParameter("changeGroupName"));
		ExtjsAjaxResponse jsonStr = new ExtjsAjaxResponse();
		try {
			EmployeeObj userObj = WebUtil.getCurrentEmployee(request
					.getSession());

			Map<String, String> changeGroupName = WebUtil.strToMap(request
					.getParameter("changeGroupName"));
			Map<String, String> newGroup = WebUtil.strToMap(request
					.getParameter("newGroup"));
			List<List<String>> relationList = WebUtil.strToList(request
					.getParameter("relationMapping"));
			List<List<String>> ddRelationList = WebUtil.strToList(request
					.getParameter("ddRelationMapping"));

			organizationService.doSaveAllOfCustomGroup(changeGroupName,
					newGroup, relationList, ddRelationList, userObj);
			jsonStr.setSuccess(true);
			jsonStr.setMessageTitle("成功");
			jsonStr.setMessage("保存成功");
		} catch (Exception e) {
			e.printStackTrace();
			jsonStr.setSuccess(false);
			jsonStr.setMessageTitle("失败");
			jsonStr.setMessage("保存失败");
		}
		WebUtil.returnResponse(response, jsonStr.toJsonString());
	}

	public void getEmpsInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String jsonStr = "";
		try {
			String empsID = request.getParameter("emps");
			String[] emps = empsID.split(",");
			List<Map> list = new ArrayList<Map>();
			for (int i = 0; i < emps.length; i++) {
				logger.info("emps[i]:" + emps[i]);
				Map emp = organizationService.getEmpInfo(Type.GetInt(emps[i]));
				list.add(emp);
			}
			jsonStr = WebUtil.buildJsonStoreByList(list);

		} catch (Exception e) {
			e.printStackTrace();
			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	public void getDeptsInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String jsonStr = "";
		try {
			String fdeptsID = request.getParameter("depts");
			String[] fdepts = fdeptsID.split(",");
			String deptsID = "";
			for (int j = 0; j < fdepts.length; j++) {
				String ds = getDeptsByFatherDeptID(Type.GetInt(fdepts[j]));
				if (deptsID == "") {
					deptsID = ds;
				} else {
					deptsID = deptsID + "," + ds;
				}
			}

			String[] depts = deptsID.split(",");
			List list = new ArrayList();
			for (int i = 0; i < depts.length; i++) {
				List emps = organizationService.getEmployeeInfoByDeptID(Type
						.GetInt(depts[i]));
				for (int v = 0; v < emps.size(); v++) {
					list.add(emps.get(v));

				}
				logger.info("list:" + list);
			}
			logger.info("deptsID:" + deptsID);
			jsonStr = WebUtil.buildJsonStoreByList(list);

		} catch (Exception e) {
			e.printStackTrace();
			jsonStr = "{success:false,message:'操作失败！请重新登录后再操作，谢谢',messageTitle:'失败'}";
		}
		WebUtil.returnResponse(response, jsonStr);
	}

	public String getDeptsByFatherDeptID(int deptID) {
		String depts = "";
		List list = organizationService.getDeptsByFatherDeptID(deptID);
		if (list.size() == 0) {
			if (depts == "") {
				depts = Type.getString(deptID);
			} else {
				depts = depts + "," + Type.getString(deptID);
			}
		} else {
			for (int i = 0; i < list.size(); i++) {
				String str = Type.getString(list.get(i));
				String regex = "\\d*";
				Pattern p = Pattern.compile(regex);
				Matcher m = p.matcher(str);
				while (m.find()) {
					if (!"".equals(m.group())) {
						if (depts == "") {
							depts = m.group();
						} else {
							depts = depts + "," + m.group();
						}
						getDeptsByFatherDeptID(Type.GetInt(m.group()));
					}
				}
			}
		}
		return depts;
	}

	public void getCompStoreByEmpName(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String empName = request.getParameter("empName");
		String store = "";
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		try {
			store = organizationService
					.getCompStoreByEmpName(empName, pageInfo);
			WebUtil.returnResponse(response, store);

		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void getCustomGroupList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = "";
		String empName = request.getParameter("empName");
		EmployeeObj userObj = (EmployeeObj) request.getSession().getAttribute(
				WebKeys.Session_Attribute_Name_UserDNA);
		if (!StringUtils.isEmpty(empName)) {
			store = organizationService.getCustomGroupListByEmpname(userObj,
					empName);
		} else {
			store = organizationService.getCustomGroupList(userObj);
		}
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 根据人员ID 得到此此人的节点路径
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getEmpPathByEmpId(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String id = request.getParameter("id");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String nodePath = organizationService.getEmpNodePath(Type
					.GetInt(id));
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("nodepath", nodePath);
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	/**
	 * 设置
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 */
	public void updateDisableReasonAndDate(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		int empId = Type.GetInt(request.getParameter("empId"));// 人员ID
		String disable = request.getParameter("disable");// 职务名字
		String reason = request.getParameter("reason");// 职务名字
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String result = organizationService.doUpdateDisableReasonAndDate(
					empId, reason, disable);// 原因+&+选择禁用的ID
			String reasonID = result.substring(result.lastIndexOf("&") + 1,
					result.length());
			String reasonName = result.substring(0, result.lastIndexOf("&"));
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功");
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.addAttribute("reasonDate", DateUtil.getCurrentDate());
			ajaxResponse.addAttribute("reasonID", reasonID);
			ajaxResponse.addAttribute("reasonName", reasonName);
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	/**
	 * 设置
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 */
	public void updatePostCard(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int empId = Type.GetInt(request.getParameter("empId"));// 人员ID
		Boolean isFront = Type.getBoolean(request.getParameter("isFront"));// 人员ID
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			organizationService.doUpdatePostCard(empId, isFront);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("操作成功");
			ajaxResponse.setMessageTitle("提示");
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
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
	public void getDisabledReasonComb(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		try {
			List list = organizationService.getDisabledReasonComb();
			String store = WebUtil.buildJsonStoreByList(list);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 根据人员ID 得到此节点的路径
	 * 
	 * @param empID
	 * @return
	 */
	public void getEmpNodePathByEmpId(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int empId = Type.GetInt(request.getParameter("empId"));// 人员ID
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String path = organizationService.getEmpNodePath(empId);
			String nodepath = path.substring(5);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("操作成功");
			ajaxResponse.addAttribute("nodepath", nodepath);
			ajaxResponse.setMessageTitle("提示");
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public void sendMessage(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			int userid = userObj.getUserID();
			String empIDs = request.getParameter("empids");
			String text = request.getParameter("text");
			boolean success = organizationService.sendMessage(userid, empIDs,
					text);
			if (success) {
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.setMessage("发送成功");
			} else {
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessageTitle("失败");
				ajaxResponse.setMessage("发送失败");
			}
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("发送失败，请重新登录后再操作，谢谢！");
			ajaxResponse.setMessageTitle("失败");

		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public void saveUserTopicMessageHabit(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String tipID = request.getParameter("tipID");
			int userID = Type.GetInt(request.getParameter("userID"));
			String messageids = request.getParameter("messageids");
			String[] messageIDs = messageids.split(",");

			organizationService.saveUserTopicMessageHabit(tipID, userID,
					messageIDs);

			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功");
			ajaxResponse.setMessage("保存成功");
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("保存失败");
			e.printStackTrace();
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public void getMessageByUserIDAndTopID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String topicID = request.getParameter("topicID");
		int userID = Type.GetInt(request.getParameter("userID"));
		String store = organizationService.getMessageByUserIDAndTopID(topicID,
				userID);
		WebUtil.returnResponse(response, store);
	}

	public void sendAnnouncementMessage(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			int userid = userObj.getUserID();
			// String empIDs = request.getParameter("empids");
			String text = request.getParameter("text");
			String topic_type = request.getParameter("topic_type");
			boolean success = organizationService.sendAnnouncementMessage(
					userid, text, topic_type);
			if (success) {
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.setMessage("发送成功");
			} else {
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessageTitle("失败");
				ajaxResponse.setMessage("发送失败");
			}
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("发送失败，请重新登录后再操作，谢谢！");
			ajaxResponse.setMessageTitle("失败");

		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public void sendPersonalMessage(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
			int userid = userObj.getUserID();
			String empIDs = request.getParameter("empids");
			String text = request.getParameter("text");
			String topic_type = request.getParameter("topic_type");
			organizationService.sendPersonalMessage(userid, text, topic_type,
					empIDs);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功");
			ajaxResponse.setMessage("发送成功");
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("发送失败，请重新登录后再操作，谢谢！");
			ajaxResponse.setMessageTitle("失败");

		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());

	}

	/**
	 * 获取用户已接收消息列表
	 * */
	// public void getHaveReceivedMessageByUseId(ActionMapping mapping,
	// ActionForm form, HttpServletRequest request,
	// HttpServletResponse response) {
	// int userid = Type.GetInt(request.getParameter("userid"));
	// String sore = organizationService.getHaveReceivedMessageByUseId(userid);
	// WebUtil.returnResponse(response, sore);
	// }
	/**
	 * 获取部门下的成员
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	@SuppressWarnings("rawtypes")
	public void getPersonByDeptID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String detpId = request.getParameter("deptID");
		List list = organizationService.getPersonByDeptID(detpId);
		String jsStr = WebUtil.buildJsonStoreByList(list);
		WebUtil.returnResponse(response, jsStr);
	}

	/**
	 * 获取中水的所有的事业部
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	@SuppressWarnings("rawtypes")
	public void getAllDivisionDept(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String companyID = request.getParameter("companyID");
		List list = organizationService.getAllDivisionDept(Type
				.GetInt(companyID));
		String jsStr = WebUtil.buildJsonStoreByList(list);
		WebUtil.returnResponse(response, jsStr);
	}
	
	public void saveDutyPlan(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try{
			T_OnWatch_schedule tOsc = new T_OnWatch_schedule();
			String id = request.getParameter("id");
			
			if(id.equals("") || id == null){
				id = UUIDUtils.nextCode();
			}
			int scheduledId =Type.GetInt( request.getParameter("scheduled_id"));
			//String name = request.getParameter("on_watch_name");
			int jobTypeId =Type.GetInt(request.getParameter("job_type_id"));
			int onWatchId =Type.GetInt(request.getParameter("on_watch_id"));
			String onWatchDate = request.getParameter("on_watch_date");
			String beginTime = request.getParameter("begin_time");
			String endDate = request.getParameter("end_date");
			String endTime = request.getParameter("end_time");
			String scheduledDatetime = request.getParameter("scheduled_datetime");
			int lastUpdatedId = Type.GetInt( request.getParameter("last_updated_id"));
			String reason = request.getParameter("reason");
			
			if(onWatchId != 0 && scheduledId!=0){
				tOsc.setId(id);
				tOsc.setJobTypeId(2);
				tOsc.setOnWatchId(onWatchId);
				tOsc.setOnWatchDate(onWatchDate);
				tOsc.setBeginTime(beginTime);
				tOsc.setEndDate(endDate);
				tOsc.setEndTime(endTime);
				tOsc.setScheduledId(scheduledId);
				tOsc.setScheduledDatetime(scheduledDatetime);
				Date today = new Date(System.currentTimeMillis());
				tOsc.setLastUpdatedDatetime(today.toString());
				tOsc.setLastUpdatedId(scheduledId);
				tOsc.setReason(reason);
				organizationService.doSaveDutyPlan(tOsc);
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.setMessage("保存成功");
			}else{
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessageTitle("提示");
				ajaxResponse.setMessage("值班人和制定人不能为空");
			}
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存失败，请重新登录后再操作，谢谢！");
			ajaxResponse.setMessageTitle("失败");

		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	public void getDutyPlanList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String department = request.getParameter("department");
		String onWatchDate = request.getParameter("on_watch_date");
		String jsStr =  organizationService.getDutyPlanList(pageInfo,department,onWatchDate);
		WebUtil.returnResponse(response,jsStr);
	}
	public void deleteDutyPlan(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String id = request.getParameter("id");
		try{
			organizationService.deleteDutyPlan(id);
			
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功");
			ajaxResponse.setMessage("删除成功");
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除失败，请重新登录后再操作，谢谢！");
			ajaxResponse.setMessageTitle("失败");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	/**
	 * 性别统计图
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getSexCount(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String jsStr =  organizationService.getSexCount();
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 性别统计表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getSexList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String sex = request.getParameter("sex");
		String jsStr =  organizationService.getSexList(sex,pageInfo);
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 政治面貌统计图
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getZzmmCount(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String jsStr =  organizationService.getZzmmCount();
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 政治面貌统计表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getZzmmList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String zzmm = request.getParameter("zzmm");
		String jsStr =  organizationService.getZzmmList(zzmm,pageInfo);
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 年龄统计图
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getAgeCount(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String jsStr =  organizationService.getAgeCount();
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 年龄统计表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getAgeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String age = request.getParameter("age");
		String jsStr =  organizationService.getAgeList(age,pageInfo);
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 职务统计图
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getDutyCount(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String jsStr =  organizationService.getDutyCount();
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 职务统计表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getDutyList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String duty = request.getParameter("duty");
		String jsStr =  organizationService.getDutyList(duty,pageInfo);
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 技术职称统计图
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getTitlesCount(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String jsStr =  organizationService.getTitlesCount();
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 技术职称统计表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getTitlesList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String titles = request.getParameter("titles");
		String jsStr =  organizationService.getTitlesList(titles,pageInfo);
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 技术级别统计图
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getJsjbCount(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String jsStr =  organizationService.getJsjbCount();
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 技术级别统计表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getJsjbList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String jsjb = request.getParameter("jsjb");
		String jsStr =  organizationService.getJsjbList(jsjb,pageInfo);
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 文化程度统计图
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getWhcdCount(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String jsStr =  organizationService.getWhcdCount();
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 文化程度统计表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getWhcdList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String whcd = request.getParameter("whcd");
		String jsStr =  organizationService.getWhcdList(whcd,pageInfo);
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 文化程度统计图
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getZgsfCount(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String jsStr =  organizationService.getZgsfCount();
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 文化程度统计表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getZgsfList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String zgsf = request.getParameter("zgsf");
		String jsStr =  organizationService.getZgsfList(zgsf,pageInfo);
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 在职统计图
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getZzygCount(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String jsStr =  organizationService.getZzygCount();
		WebUtil.returnResponse(response,jsStr);
	}
	/**
	 * 在职统计表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getZzygList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String zzyg = request.getParameter("zzyg");
		String jsStr =  organizationService.getZzygList(zzyg,pageInfo);
		WebUtil.returnResponse(response,jsStr);
	}
	public void  getDeptManagerList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String jsStr =  organizationService.getDeptManagerList();
		WebUtil.returnResponse(response,jsStr);
	}
}
