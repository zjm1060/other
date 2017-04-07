//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\ProjectAction.java

package com.bitc.jk.xmis.web.action;

import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.ConditionMap;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.service.FileUploadDownloadService;
import com.bitc.jk.xmis.service.KZHBService;
import com.bitc.jk.xmis.service.SysService;
import com.bitc.jk.xmis.service.impl.FileUploadDownloadServiceImpl;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.SpringInit;

public class KZHBAction extends BaseAction {
	private static Logger logger = Logger.getLogger(KZHBAction.class);

	private KZHBService kzhbService;

	private SysService sysService;

	/**
	 * @roseuid 4C1F042200DA
	 */
	public KZHBAction() {

	}

	protected ActionForward unspecified(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {

		logger.info("---> Go to Page ! ");

		return gotoAttchedFilesListPage(mapping, form, request, response);

	}

	public ActionForward gotoAttchedFilesListPage(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {

		String forword = WebKeys.Forword_to_KZHB_AttatchedFileList_Page;

		request.setAttribute(WebKeys.Forword_Param_Name, forword);
		return super.forward(mapping, form, request, response);
	}

	public ActionForward doDeleteAttachment(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {

		logger.info("----> do delete attachment file");

		int attID = Type.GetInt(request.getParameter("attID"));

		kzhbService.doDeleteAttachment(attID);

		return gotoAttchedFilesListPage(mapping, form, request, response);
	}

	public void getAttchedFilesList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {

//		logger.info("----> getAttchedFilesKZHList");
//
//		request.setAttribute("queryType", "Herb");
//
//		PageInfo pageInfo = WebUtil.generatePageInfo(request);
//
//		List<QueryParam> params = WebUtil.generateQueryCondition(request,
//				sysService);
//
//		params.add(QueryParam.buildQueryParam("attachmentTypeID",
//				Type.getString(WebKeys.Upload_Application_KzhbFile_Type), "=",
//				Type.INTEGER));
//
//		params.add(QueryParam.buildQueryParam("deletedFlag", "1", "=",
//				Type.INTEGER));
//
//		String store = kzhbService.getAttchedFilesListStore(params, pageInfo);
//		response.getWriter().print(store);
		
		String store = "";
		try {
			FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
					.getBean("fileUploadDownloadServiceImpl",
							FileUploadDownloadServiceImpl.class);
			PageInfo pageInfo = WebUtil.generatePageInfo(request);

			List<StandardQuery> querys = (List<StandardQuery>) request
					.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);
			if (querys == null) {
				querys = new ArrayList<StandardQuery>();
			}
			List<StandardQuery> query2 = WebUtil.generateStandardQuerysByMulit(
					request, sysService);
			if (query2 != null) {
				for (int i = 0; i < query2.size(); i++) {
					querys.add(query2.get(i));
				}
			}
			store = service.getAttachmentListForKZHB(querys, pageInfo);
			WebUtil.returnResponse(response, store);

		} catch (Exception e) {
			e.printStackTrace();
			request.setAttribute(WebKeys.Request_Attribute_Name_LoginFailed,
					WebKeys.Request_Attribute_Name_ErrMessage_Value);
		}

	}
	/**
	 * 查询合格供应商列表
	 * */
	public ActionForward getQualifiedSuppliers(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String show = request.getParameter("show");
		if (show != null) {
			return mapping.findForward("deviceInfoListr");
		}
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = "";
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		store = kzhbService.getQualifiedSuppliers(pageInfo,query);
		WebUtil.returnResponse(response, store);
		return null;
		
	}
/**
 * 资质列表
 * */
	public ActionForward getQualificationList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)throws Exception{
		String show = request.getParameter("show");
		if (show != null) {
			return mapping.findForward("qualificationLIST");
		}
		
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = "";
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		store = kzhbService.getQualificationList(pageInfo,query);
		WebUtil.returnResponse(response, store);
				return null;
		
	}
	
	/**
	 * 专利列表
	 * */
	public ActionForward getPatentInfoList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response){
		String show = request.getParameter("show");
		if (show != null) {
			return mapping.findForward("patentInfoLIST");
		}
		
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = "";
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		store = kzhbService.getPatentInfoList(pageInfo,query);
		WebUtil.returnResponse(response, store);
		return null;
	}
	public SysService getSysService() {
		return sysService;
	}

	public void setSysService(SysService sysService) {
		this.sysService = sysService;
	}

	public KZHBService getKzhbService() {
		return kzhbService;
	}

	public void setKzhbService(KZHBService kzhbService) {
		this.kzhbService = kzhbService;
	}
}
