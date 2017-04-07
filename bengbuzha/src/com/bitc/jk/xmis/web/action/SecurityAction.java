/**
 * 
 */
package com.bitc.jk.xmis.web.action;


import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.security.NodeInfo;
import com.bitc.jk.xmis.service.SecurityService;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.web.action.param.SecurityTreeParam;
import com.bitc.jk.xmis.web.response.TreeLoaderResponse;

/**
 * @author Ted Li
 * 
 *         2011-5-30
 */
public class SecurityAction extends DispatchAction {

	private static Logger logger = Logger.getLogger(SecurityAction.class);

	private SecurityService securityService;

	public SecurityAction() {
	}

	public SecurityService getSecurityService() {
		return securityService;
	}

	public void setSecurityService(SecurityService aSecurityService) {
		this.securityService = aSecurityService;
	}

	protected ActionForward unspecified(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {

		logger.info("---> Go to SecurityConfigPage ! ");

		return gotoSecurityListPage(mapping, form, request, response);

	}

	public ActionForward gotoSecurityListPage(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {

		logger.info("---> goto SecurityListPage ! ");

		String forwordStr = WebKeys.Forword_to_SecurityCfgPage;

		return mapping.findForward(forwordStr);
	}

	public ActionForward gotoSecurityConfigPage(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {

		logger.info("---> goto SecurityConfigPage ! ");

		String forwordStr = WebKeys.Forword_to_SecurityCfgPage;

		return mapping.findForward(forwordStr);
	}

	// public ActionForward getLeftMenu(ActionMapping mapping, ActionForm form,
	// HttpServletRequest request, HttpServletResponse response)
	// throws Exception {
	// EmployeeObj employeeObj = (EmployeeObj) request.getSession()
	// .getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
	// int moduleId = Type.GetInt(request.getParameter("moduleId"));
	//
	// String str = employeeObj.getMenuJsonStr(moduleId);
	//
	// System.out.println(str);
	//
	// response.getWriter().write(str);
	// return null;
	// }

	// temp
	public ActionForward gotoSecurityCfgPage(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {

		logger.info("---> goto SecurityConfigPage ! ");

		String forwordStr = WebKeys.Forword_to_SecurityListPage;

		return mapping.findForward(forwordStr);
	}

	public ActionForward securityTreeLoader(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		SecurityTreeParam param = new SecurityTreeParam();
		BeanUtils.populate(param, request.getParameterMap());
		String store = "";
		if (param.getNodeLevel() != 0) {
			store = securityService.getSecurityTree(param);
		} else {
			TreeLoaderResponse loaderResponse = new TreeLoaderResponse();
			loaderResponse.addTreeNode("权限管理", "1-1-1", false, "file", "");
			store = loaderResponse.toJsonString(param.getJsonConfig());
		}
		WebUtil.returnResponse(response, store);
		return null;
	}

	public void getBizObjById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String node = request.getParameter("bizID"); // 传入格式1-1-1
		String store = "";
		if (node.matches("\\d-\\d-\\d")) {
			store = securityService.getBizObjById(node);
		}
		WebUtil.returnResponse(response, store);
	}

	public void getAllStatusList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		WebUtil.returnResponse(response, securityService.getAllStatusList());
	}

	public void getOtherBizModuleList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int id = Type.GetInt(request.getParameter("bizModuleId"));
		WebUtil.returnResponse(response,
				securityService.getOtherBizModuleList(id));
	}

	public void saveAllOperation(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String[] allNode = request.getParameterValues("allNode");
		securityService.doSaveNodesMsg(allNode);

	}

	public void deleteOperation(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String nodeId = request.getParameter("nodeId");
		securityService.deleteOperation(nodeId);

	}

	/**
	 * 列出角色信息
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 */
	public ActionForward getSecurityInfoList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		NodeInfo node = null;
		try {
			node = securityService.getSecurityNodes();
		} catch (Exception e) {
			e.printStackTrace();
		}
		request.setAttribute("node", node);
		return mapping.findForward("secList");
	}
	
	public void getSecurityRoles(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response){
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");
		String store = "";
		store = securityService.getSecurityRoles(pageInfo,queryStr);
		WebUtil.returnResponse(response, store);
		
	}
}
