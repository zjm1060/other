//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\OrganizationAction.java

package com.bitc.jk.xmis.web.action;

import java.io.IOException;
import java.io.PrintWriter;
import java.sql.Date;
import java.util.Calendar;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;
import org.dom4j.Document;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.service.OrganizationService;
import com.bitc.jk.xmis.util.TimeOutException;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

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
				
				
				request
						.setAttribute(
								WebKeys.Request_Attribute_Name_Department,
								request
										.getParameter(WebKeys.Request_Attribute_Name_Department));
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

				int deptID = new Integer(request
						.getParameter(WebKeys.Type_Department)).intValue();

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
				
				request.setAttribute(WebKeys.Request_Attribute_Name_CurrentDATE, String.valueOf(dateOfMonth));
			}

		} catch (Exception e) {
			e.printStackTrace();
			forwordStr = WebKeys.Forword_to_LoginFailed;
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}

		return (mapping.findForward(forwordStr));

	}

	public void updatePwd(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> updatePwd //" + request.getParameter("userid") + "//"
				+ request.getParameter("pwd"));
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
				if (organizationService.doChangePassword(Integer
						.parseInt(request.getParameter("userid")), request
						.getParameter("pwd"), userObj.getUserID()) > 0)
					message = "密码保存成功！";
				else
					message = "密码保存失败，请重新操作，谢谢！";

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
				emp.setEducationHighestLevel(Integer.parseInt(request
						.getParameter("eduID")));
				emp.setEducationLevelName(request.getParameter("eduLevel").trim());
				emp.setSchoolName(request.getParameter("schoolName").trim());
				emp.setGraduationDate("".equals(request
						.getParameter("graduationDate").trim()) ? null : Date
						.valueOf(request.getParameter("graduationDate").trim()));
				emp.setMajor(request.getParameter("major").trim());
				emp.setEducationRemark(request.getParameter("eduRemark").trim());

				emp.setEducationHighestLevel(organizationService
						.doEditEducation(request.getParameter("oldEduName").trim(),
								emp, userObj.getUserID()));

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
						+ emp.getGraduationDate().toString().trim() + "',eduMajor:'"
						+ emp.getMajor() + "',eduRemark:'"
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

	public ActionForward createNewUser(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> createNewUser //" + request.getParameter("newDept") + "//"
				+ request.getParameter("newEmp") + "//" + request.getParameter("empDeptMapping"));

		String message= "";
		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("TimeOut！请重新登录。");
			} else {
				
				Map deptMap = WebUtil.getNewPart(request.getParameter("newDept"));
				Map empMap = WebUtil.getNewPart(request.getParameter("newEmp"));
				Map empDeptMapping = WebUtil.getNewPart(request.getParameter("empDeptMapping"));
				
				//logger.debug(String.valueOf(empMap.size()));
				
				if(organizationService.doCreateNewBatch(deptMap, empMap, empDeptMapping, userObj)>0)
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
			
			message = "创建不成功\n" + e.getMessage() ;
			
		}
		
		try{
			response.getWriter().write(message);
		}catch(Exception e){
			e.printStackTrace();
		}
		return null;

	}
	
	public ActionForward saveEmpInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> saveEmpInfo //"+ request.getParameter("joinDate").trim()+"//");

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
				
				EmployeeObj empObj = new EmployeeObj();
				
				empObj.setUserID(Integer.parseInt(request.getParameter("userid")));
				empObj.setFirstName(request.getParameter("firstName"));
				empObj.setLastName(request.getParameter("lastName"));
				empObj.setLoginName(request.getParameter("loginName"));
				empObj.setSex(Integer.parseInt(request.getParameter("sex")));
				
				empObj.setJoinDate( (request.getParameter("joinDate")==null || "".equals(request
						.getParameter("joinDate").trim()) ) ? null : Date
						.valueOf(request.getParameter("joinDate").trim()));
				
				empObj.setBirthday("".equals(request
						.getParameter("birthday")) ? null : Date
						.valueOf(request.getParameter("birthday")));
				empObj.setIDCard(request.getParameter("IDCard"));
				empObj.setMobileNo(request.getParameter("mobileNo"));
				empObj.setEmailAddress(request.getParameter("email"));
				empObj.setWebPage(request.getParameter("webPage"));
				empObj.setPosition(request.getParameter("position"));
				empObj.setSwitchboardNo(request.getParameter("officeTelNo"));
				empObj.setExtNo(request.getParameter("extNo"));
				empObj.setFaxNo(request.getParameter("faxNo"));
				empObj.setHomeAddress(request.getParameter("homeAddress"));
				empObj.setZipCode(request.getParameter("zipCode"));
				empObj.setHomeTelNo(request.getParameter("homeTelNo"));
				empObj.setPoliticalStatus(Integer.parseInt(request.getParameter("politicalStatus")));
				empObj.setNativePlace(request.getParameter("nativePlace"));
				empObj.setMaritalStatus(Integer.parseInt(request.getParameter("MaritalStatus")));
				empObj.setSpouseName(request.getParameter("spouseName"));
				empObj.setIssueName(request.getParameter("issueName"));
				empObj.setIssueSex(Integer.parseInt(request.getParameter("issueSex")));
				empObj.setHobby(request.getParameter("hobby"));
				empObj.setSpecialty(request.getParameter("specialty"));
				
				empObj.setDisabledFlag(Integer.parseInt(request.getParameter("disabledFlag")));
				
				empObj.setRemark(request.getParameter("remark"));

				organizationService.doSaveEmployeeInfo(empObj, userObj);
				response.getWriter().write(String.valueOf("保存成功！"));				
			}

		} catch (TimeOutException te) {
			te.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_ErrMessage,
					e.getMessage());
			
			message = "保存不成功\n" + e.getMessage() ;
			
		}
		
		try{
			response.getWriter().write(message);
		}catch(Exception e){
			e.printStackTrace();
		}
		return null;

	}
	
	public ActionForward disableTreeNode(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> disableTreeNode //" + request.getParameter("item") );

		String message= "";
		try {

			EmployeeObj userObj = (EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

			if (userObj == null) {
				request.setAttribute(
						WebKeys.Request_Attribute_Name_LoginFailed,
						WebKeys.Request_Attribute_Name_ReLogin_Value);
				throw new TimeOutException("TimeOut！请重新登录。");
			} else {
				
				
				if(organizationService.doDisableTreeNode(request.getParameter("item"), userObj)>0)
					response.getWriter().write(String.valueOf("禁用成功！如需重新启用，请联系管理员！"));
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
			
			message = "禁用操作失败\n" + e.getMessage() ;
			
		}
		
		try{
			response.getWriter().write(message);
		}catch(Exception e){
			e.printStackTrace();
		}
		return null;

	}	
	
	public void changeLunchAddress(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		logger.info("---> changeLunchAddress //" + request.getParameter("userid")
				+ "//" + request.getParameter("newAddress"));

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

				if (organizationService.doChangeLunchAddress(Integer
						.parseInt(request.getParameter("newAddress")), Integer
						.parseInt(request.getParameter("userid")), userObj
						.getUserID()) > 0) {

					messages = "午餐地址保存成功";
					messageTitle = "成功";
					flag = true;

				} else {
					messages = "午餐地址保存失败！";
					messageTitle = "失败";
				}

				jsonStr = "{success:" + String.valueOf(flag) + ",message:'"
						+ messages + "',messageTitle:'" + messageTitle
						+ "'}";

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

	public void getLunchAddressList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

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

				dataStore = organizationService.getLunchAddressListByDiv(userObj.getDivisionID());
				
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

		logger.info("---> lunchAddressJournal //" );

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
						+ messages + "',messageTitle:'" + messageTitle
						+ "'}";

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

}
