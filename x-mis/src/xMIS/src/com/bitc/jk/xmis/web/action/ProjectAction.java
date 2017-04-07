//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\ProjectAction.java

package com.bitc.jk.xmis.web.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.service.ProjectService;
import com.bitc.jk.xmis.util.TimeOutException;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

public class ProjectAction extends DispatchAction {
	private static Logger logger = Logger.getLogger(ProjectAction.class);

	private ProjectService projectService;

	/**
	 * @roseuid 4C1F042200DA
	 */
	public ProjectAction() {

	}

	/**
	 * Access method for the projectService property.
	 * 
	 * @return   the current value of the projectService property
	 */
	public ProjectService getProjectService() {
		return projectService;
	}

	/**
	 * Sets the value of the projectService property.
	 * 
	 * @param aProjectService the new value of the projectService property
	 */
	public void setProjectService(ProjectService aProjectService) {
		projectService = aProjectService;
	}

	/**
	 * @roseuid 4C19D05D0232
	 */
	public void getProjectListByUser() {

	}
	
	public void getProjectStatusStore(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		logger.info("---> getProjectStatusStore //"+ request.getParameter("searchName"));
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

				jsonStr = projectService.getProjectStatusStore(request.getParameter("searchName").trim());

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
}
