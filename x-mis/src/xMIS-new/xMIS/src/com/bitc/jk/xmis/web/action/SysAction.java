package com.bitc.jk.xmis.web.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.sf.json.JSONObject;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.Condition;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.sys.Draft;
import com.bitc.jk.xmis.service.SysService;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;

public class SysAction extends DispatchAction {

	private SysService sysService;
	private static Logger logger = Logger.getLogger(SysAction.class);

	public SysAction() {
	}

	public ActionForward getCondition(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String sid = request.getParameter("id");
		if (StringUtils.isEmpty(sid)) {
			return null;
		}
		int id = Integer.parseInt(sid);
		Condition condition = sysService.getCondition(id);
		JSONObject object = JSONObject.fromObject(condition);
		object.write(response.getWriter());
		return null;
	}

	public ActionForward getLeftMenu(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		EmployeeObj employeeObj = (EmployeeObj) request.getSession()
				.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
		int moduleId = Type.GetInt(request.getParameter("moduleId"));
		String str = employeeObj.buildMenuJsonStr(moduleId);
		WebUtil.returnResponse(response, str);
		return null;
	}
/**
 * 币种
 * */
	public ActionForward getCurrency(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String store = sysService.getCurrencyStore();
		WebUtil.returnResponse(response, store);
		return null;
	}
/**
 * 功率单位
 * */
	public ActionForward getPower(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String store = sysService.getPowerStore();
		WebUtil.returnResponse(response, store);
		return null;
	}
/**
 * 重量单位
 * */
	public ActionForward getWeight(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String store = sysService.getWeightStore();
		WebUtil.returnResponse(response, store);
		return null;
	}
/**
 * 物资自身属性
 * */	
	public ActionForward getMaterialsProperty(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String store = sysService.getMaterialsProperty();
		WebUtil.returnResponse(response, store);
		return null;
	}
/**
 * 物资业务属性
 * */
	public ActionForward getBusiness(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String store = sysService.getBusiness();
		WebUtil.returnResponse(response, store);
		return null;
	}
/**
 * 物资用途
 * */
	public ActionForward getProduceStatus(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String store = sysService.getProduceStatus();
		WebUtil.returnResponse(response, store);
		return null;
	}
/**
 * 物资生产状态
 * */	
	public ActionForward getMaterialsUse(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String store = sysService.getMaterialsUse();
		WebUtil.returnResponse(response, store);
		return null;
	}
	public ActionForward saveDraft(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		Draft draft = new Draft();
		BeanUtils.populate(draft, request.getParameterMap());
		draft.setLastModified(DateUtil.getCurrentDateTime());
		draft.setLastModifier(WebUtil.getCurrentEmployee().clone());
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			sysService.doSaveDraft(draft);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("draftId", draft.getId());
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存草稿失败\n" + e.getMessage());
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
		return null;
	}

	public ActionForward getDrafts(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		Draft draft = new Draft();
		BeanUtils.populate(draft, request.getParameterMap());
		PageInfo info = WebUtil.generatePageInfo(request);
		String store = sysService.getDraftsByExample(draft, info);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getDraft(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String draftId = request.getParameter("id");
		Draft draft = null;
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		if (StringUtils.isEmpty(draftId)) {
			String model = request.getParameter("model");
			String appId = request.getParameter("appId");
			draft = sysService.getDraftByModelAndAppId(model, appId);
		} else {
			draft = sysService.getDraftsById(draftId);
		}
		if (draft == null) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("打开草搞失败");
			ajaxResponse.setMessage("未找到草稿，草稿不存在或已删除。请刷新首页后重新打开链接。");
		} else {
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("draft", draft);
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward delDraft(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String draftId = request.getParameter("id");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			sysService.doDeleteDraft(draftId);
		} catch (Exception e) {
			logger.error("删草稿出错，DraftId:" + draftId, e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("删除草稿出错");
		}
		ajaxResponse.setSuccess(true);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public SysService getSysService() {
		return sysService;
	}

	public void setSysService(SysService sysService) {
		this.sysService = sysService;
	}
}
