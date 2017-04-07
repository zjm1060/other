package com.bitc.jk.xmis.applymanage.web;

import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.math.BigDecimal;
import java.util.*;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.xml.crypto.Data;

import com.bitc.jk.xmis.util.*;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;
import org.aspectj.apache.bcel.generic.NEW;
import org.fireflow.engine.EngineException;
import org.omg.CORBA.DATA_CONVERSION;

import com.bitc.jk.xmis.applymanage.model.AbsentForm;
import com.bitc.jk.xmis.applymanage.model.ApplyCarForm;
import com.bitc.jk.xmis.applymanage.model.ApplyMeetingRoomForm;
import com.bitc.jk.xmis.applymanage.model.ApplyOfficialSuppliesForm;
import com.bitc.jk.xmis.applymanage.model.ApplyOfficialSuppliesRepairForm;
import com.bitc.jk.xmis.applymanage.model.BusinessEntertainment;
import com.bitc.jk.xmis.applymanage.model.CeyaInfo;
import com.bitc.jk.xmis.applymanage.model.DocumentAppFormDetail;
import com.bitc.jk.xmis.applymanage.model.DocumentCiculatedInstances;
import com.bitc.jk.xmis.applymanage.model.EarlyPlans;
import com.bitc.jk.xmis.applymanage.model.GczlInfo;
import com.bitc.jk.xmis.applymanage.model.TheEndOfThePlanned;
import com.bitc.jk.xmis.applymanage.service.ApplyManageService;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;
import com.bitc.jk.xmis.workflow.service.WorkflowService;

public class ApplyManageAction extends DispatchAction {
	private static Logger logger = Logger.getLogger(ApplyManageAction.class);

	private ApplyManageService applyMangeService;
	public WorkflowService workflowService;

	public WorkflowService getWorkflowService() {
		return workflowService;
	}

	public void setWorkflowService(WorkflowService workflowService) {
		this.workflowService = workflowService;
	}

	public ApplyManageService getApplyMangeService() {
		return applyMangeService;
	}

	public void setApplyMangeService(ApplyManageService applyMangeService) {
		this.applyMangeService = applyMangeService;
	}

	/**
	 * 会议室信息保存
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doSaveAppMeetingRoom(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String id = Type.getString(request.getParameter("id"));
		String room_id = Type.getString(request.getParameter("room_id"));
		int priority = Type.GetInt(request.getParameter("priority"));
		int sponsor_id = Type.GetInt(request.getParameter("sponsor_id"));
		String reason = Type.getString(request.getParameter("reason"));
		Date begin_datetime = Type.getDateTime(request
				.getParameter("begin_datetime"));
		Date end_datetime = Type.getDateTime(request
				.getParameter("end_datetime"));
		Date app_datetime = Type.getDateTime(request
				.getParameter("app_datetime"));
		int number_people = Type.GetInt(request.getParameter("number_people"));
		int app_user_id = Type.GetInt(request.getParameter("app_user_id"));
		String remark = Type.getString(request.getParameter("remark"));
		int assigned_id = Type.GetInt(request.getParameter("assigned_id"));
		Date assigned_datetime = Type.getDateTime(request
				.getParameter("assigned_datetime"));
		int status_id = Type.GetInt(request.getParameter("status_id"));

		ApplyMeetingRoomForm applyMeetingRoom = new ApplyMeetingRoomForm();

		applyMeetingRoom.setId(id);
		applyMeetingRoom.setRoom_id(room_id);
		applyMeetingRoom.setPriority(priority);
		applyMeetingRoom.setSponsor_id(sponsor_id);
		applyMeetingRoom.setReason(reason);
		applyMeetingRoom.setBegin_datetime(begin_datetime);
		applyMeetingRoom.setEnd_datetime(end_datetime);
		applyMeetingRoom.setApp_datetime(app_datetime);
		applyMeetingRoom.setNumber_people(number_people);
		applyMeetingRoom.setApp_user_id(app_user_id);
		applyMeetingRoom.setRemark(remark);
		applyMeetingRoom.setAssigned_id(assigned_id);
		applyMeetingRoom.setAssigned_datetime(assigned_datetime);
		applyMeetingRoom.setStatus_id(status_id);

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			String return_id = applyMangeService
					.doSaveAppMeetingRoom(applyMeetingRoom);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("预约申请信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 根据ID得到会议室信息
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getApplyMeetingRoom(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String id = Type.getString(request.getParameter("id"));

		String jsonstr = applyMangeService.getApplyMeetingRoomFormById(id);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 得到会议室列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getAppMeetingRoomFormList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		String jsonstr = applyMangeService.getAppMeetingRoomFormList(pageInfo,
				queryStr);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getMeetingRoomCombo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String jsonstr = applyMangeService.getMeetingRoomCombo();
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 会议室状态变更
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doHandleAppMeetingRoomStatus(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		String id = Type.getString(request.getParameter("id"));
		int status_id = Type.GetInt(request.getParameter("status_id"));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int n = applyMangeService.doHandleAppMeetingRoomStatus(id,
					status_id);
			if (n != 0) {
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessage("处理成功！");
			} else {
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessage("处理错误！\n错误信息：0条数据被处理。");
			}
		} catch (Exception e) {
			logger.error("处理出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("处理错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 得到正在使用的会议室列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getMeetingRoomUsingTimeList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String param_id = Type.getString(request.getParameter("param_id"));
		String form_id = Type.getString(request.getParameter("form_id"));
		String type = Type.getString(request.getParameter("type"));
		String now_begin_datetime = Type.getString(request
				.getParameter("begin_datetime"));
		String now_end_datetime = Type.getString(request
				.getParameter("end_datetime"));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			boolean isInuse = applyMangeService.getMeetingRoomUsingTimeList(
					type, param_id, now_begin_datetime, now_end_datetime,
					form_id);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("isInuse", isInuse);
		} catch (Exception e) {
			logger.error("验证出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("验证错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 首页 定时器
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getApplyMeetingFormTipsList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String jsonstr = applyMangeService.getApplyMeetingFormTipsList();
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 得到车的列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getAppCarList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		String jsonstr = applyMangeService.getAppCarList(pageInfo, queryStr);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 得到被选中的车辆列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getVehicleManagementListToSelectGrid(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String store = applyMangeService.getVehicleManagementListToSelectGrid();
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 公务车的保存
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doSaveAppCars(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String caridStrs = request.getParameter("caridStrs");

		String form_id = Type.getString(request.getParameter("form_id"));
		int priority = Type.GetInt(request.getParameter("priority"));
		String reason = Type.getString(request.getParameter("reason"));
		Date begin_datetime = Type.getDateTime(request
				.getParameter("begin_datetime"));
		Date end_datetime = Type.getDateTime(request
				.getParameter("end_datetime"));
		Date app_datetime = Type.getDateTime(request
				.getParameter("app_datetime"));
		int app_user_id = Type.GetInt(request.getParameter("app_user_id"));
		String remark = Type.getString(request.getParameter("remark"));
		int assigned_id = Type.GetInt(request.getParameter("assigned_id"));
		int status_id = Type.GetInt(request.getParameter("status_id"));
		Date assigned_datetime =null;
		if(status_id == 3){
			Calendar calendar = Calendar.getInstance();
			 assigned_datetime = new Date(
					calendar.getTimeInMillis());
		}
		ApplyCarForm applyCarForm = new ApplyCarForm();
		applyCarForm.setForm_id(form_id);
		applyCarForm.setPriority(priority);
		applyCarForm.setReason(reason);
		applyCarForm.setBegin_datetime(begin_datetime);
		applyCarForm.setEnd_datetime(end_datetime);
		applyCarForm.setApp_datetime(app_datetime);
		applyCarForm.setApp_user_id(app_user_id);
		applyCarForm.setRemark(remark);
		applyCarForm.setAssigned_id(assigned_id);
		applyCarForm.setAssigned_datetime(assigned_datetime);
		applyCarForm.setStatus_id(status_id);

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			String return_id = applyMangeService.doSaveAppCars(applyCarForm,
					caridStrs);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("预约申请信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 根据id 得到公务车申请列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getApplyCarByFormId(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String form_id = Type.getString(request.getParameter("form_id"));
		EmployeeObj emp = (EmployeeObj) request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
		String jsonstr = applyMangeService.getApplyCarByFormId(form_id,emp);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 根据id 获取已选定的车辆列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getApplyCarIdsDetalsByFormId(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id = Type.getString(request.getParameter("form_id"));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			String car_idstr = applyMangeService
					.getApplyCarIdsDetalsByFormId(form_id);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("car_idstr", car_idstr);
			ajaxResponse.setMessage("操作成功！");
		} catch (Exception e) {
			logger.error("获取已选定的车辆列表错误", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("获取已选定的车辆列表错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 时间的验证
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void validBeginEndTime(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String now_begin_datetime = Type.getString(request
				.getParameter("begin_datetime"));
		String now_end_datetime = Type.getString(request
				.getParameter("end_datetime"));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			boolean isInuse = applyMangeService.validBeginEndTime(
					now_begin_datetime, now_end_datetime);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("isInuse", isInuse);
		} catch (Exception e) {
			logger.error("验证出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("验证错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 得到办公用品的列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getApplyOfficialSuppliesList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		String form_id = Type.getString(request.getParameter("form_id"));
		String jsonstr = applyMangeService
				.getApplyOfficialSuppliesList(form_id);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 得到办公用品的汇总列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getApplyOfficialSuppliesListSum(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		String jsonstr = applyMangeService.getApplyOfficialSuppliesListSum(
				pageInfo, queryStr);
		WebUtil.returnResponse(response, jsonstr);
	}

	// 原办公清单申请的查询列表
	/*
	 * public void getApplyOfficialSuppliesListQuery(ActionMapping mapping,
	 * ActionForm form, HttpServletRequest request, HttpServletResponse
	 * response) { PageInfo pageInfo = WebUtil.generatePageInfo(request); String
	 * queryStr = request.getParameter("queryStr");// js 做trim String jsonstr =
	 * applyMangeService.getApplyOfficialSuppliesListQuery(pageInfo, queryStr);
	 * WebUtil.returnResponse(response, jsonstr); }
	 */
	/**
	 * 办公用品的类别下拉选择框
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getOfficialSuppliesTypeCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		String jsonstr = applyMangeService.getOfficialSuppliesTypeCombo();
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 办公用品的下拉框
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getOfficialSuppliesCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		String type = Type.getString(request.getParameter("type"));

		String jsonstr = applyMangeService.getOfficialSuppliesCombo(type);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 办公用品的保存
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doSaveApplyOfficialSuppliesForm(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id = Type.getString(request.getParameter("form_id"));
		String supplies_id = Type
				.getString(request.getParameter("supplies_id"));
		int priority = Type.GetInt(request.getParameter("priority"));
		int qty = Type.GetInt(request.getParameter("qty"));
		String reason = Type.getString(request.getParameter("reason"));
		Date app_datetime = DateUtil.getCurrentDateTime();
		int app_user_id = WebUtil.getCurrentEmployee().getUserID();
		String remark = Type.getString(request.getParameter("remark"));

		ApplyOfficialSuppliesForm applyForm = new ApplyOfficialSuppliesForm();
		applyForm.setForm_id(form_id);
		applyForm.setSupplies_id(supplies_id);
		applyForm.setPriority(priority);
		applyForm.setQty(qty);
		applyForm.setReason(reason);
		applyForm.setApp_user_id(app_user_id);
		applyForm.setApp_datetime(app_datetime);
		applyForm.setRemark(remark);

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			String return_id = applyMangeService
					.doSaveApplyOfficialSuppliesForm(applyForm);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("申请信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 办公用品的修改
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doUpdateApplyOfficialSuppliesForm(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id_param = Type.getString(request
				.getParameter("form_id_param"));
		String supplies_id_param = Type.getString(request
				.getParameter("supplies_id_param"));
		String form_id = Type.getString(request.getParameter("form_id"));
		String supplies_id = Type
				.getString(request.getParameter("supplies_id"));
		int priority = Type.GetInt(request.getParameter("priority"));
		int qty = Type.GetInt(request.getParameter("qty"));
		String reason = Type.getString(request.getParameter("reason"));
		Date app_datetime = DateUtil.getCurrentDateTime();
		int app_user_id = WebUtil.getCurrentEmployee().getUserID();
		String remark = Type.getString(request.getParameter("remark"));

		ApplyOfficialSuppliesForm applyForm = new ApplyOfficialSuppliesForm();
		applyForm.setForm_id(form_id);
		applyForm.setSupplies_id(supplies_id);
		applyForm.setPriority(priority);
		applyForm.setQty(qty);
		applyForm.setReason(reason);
		applyForm.setApp_user_id(app_user_id);
		applyForm.setApp_datetime(app_datetime);
		applyForm.setRemark(remark);

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			applyMangeService.doUpdateApplyOfficialSuppliesForm(applyForm,
					form_id_param, supplies_id_param);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", form_id_param);
			ajaxResponse.setMessage("修改成功！");
		} catch (Exception e) {
			logger.error("申请信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("修改错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 办公用品的删除
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doDeleteApplyOfficialSuppliesForm(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id = Type.getString(request.getParameter("form_id"));
		String supplies_id = Type
				.getString(request.getParameter("supplies_id"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			applyMangeService.doDeleteApplyOfficialSuppliesForm(form_id,
					supplies_id);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("申请信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 得到公用品表单
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getOfficalSuppliesByCompositeIds(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		String form_id = Type.getString(request.getParameter("form_id"));
		String supplies_id = Type
				.getString(request.getParameter("supplies_id"));

		String jsonstr = applyMangeService.getOfficalSuppliesByCompositeIds(
				form_id, supplies_id);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 办公用品的申请流程启动
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doStartApplyOfficialSupplies(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id = Type.getString(request.getParameter("form_id"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			String processInstanceId = applyMangeService
					.doStartApplyOfficialSupplies(form_id);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);
		} catch (EngineException e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>找不到指定角色的人，请联系管理员~");
			ajaxResponse.setMessageTitle("失败");
		} catch (Exception e) {
			logger.error("提交出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 审批
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doApprovalTask(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String workItemId = request.getParameter("workItemId");
		String isAgree = request.getParameter("isAgree");
		String comment = request.getParameter("comment");

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String processInstanceId = applyMangeService.doApprovalTask(
					workItemId, isAgree, comment);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);

		} catch (Exception e) {
			logger.error("提交报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("提交失败<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 销假领导的确认
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doApprovalTask2(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String workItemId = request.getParameter("workItemId");
		String isAgree = request.getParameter("isAgree");
		String comment = request.getParameter("comment");

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String processInstanceId = applyMangeService.doApprovalTask2(
					workItemId, isAgree, comment);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);

		} catch (Exception e) {
			logger.error("提交报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("提交失败<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 修改后再次提交
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void doGoOnSubmitApplyOfficialSupplies(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String workItemId = request.getParameter("workItemId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String processInstanceId = applyMangeService
					.doGoOnSubmitApplyOfficialSupplies(workItemId);

			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("提交成功！<br/>" + infoMsg);
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("提交报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>" + e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 流程结束
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void doEndApply(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String workItemId = request.getParameter("workItemId");
		String form_id = request.getParameter("form_id");
		String forceStop = request.getParameter("forceStop");
		String processInstanceId = request.getParameter("processInstanceId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			applyMangeService.doEndApply(workItemId, forceStop,
					processInstanceId);
			// applyMangeService.updateLastActorToNull(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("操作成功,流程结束");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败<br/>" + e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 办公用品领用的流程的结束，并做数量计算
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void doEndApplyOfficialSuppliesUpdateNum(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String workItemId = request.getParameter("workItemId");
		String form_id = request.getParameter("form_id");
		String processInstanceId = request.getParameter("processInstanceId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			applyMangeService.doEndApplyOfficialSuppliesUpdateNum(workItemId,
					form_id, processInstanceId);
			applyMangeService.updateLastActorToNull(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("操作成功,流程结束");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败<br/>" + e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	// 办公用品维修

	/**
	 * 办公用品维修表单保存
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doSaveApplyOfficialSuppliesRepairForm(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id = Type.getString(request.getParameter("form_id"));
		String supplies_id = Type
				.getString(request.getParameter("supplies_id"));
		int priority = Type.GetInt(request.getParameter("priority"));
		int qty = Type.GetInt(request.getParameter("qty"));
		String reason = Type.getString(request.getParameter("reason"));
		Date app_datetime = DateUtil.getCurrentDateTime();
		int app_user_id = WebUtil.getCurrentEmployee().getUserID();
		String remark = Type.getString(request.getParameter("remark"));

		ApplyOfficialSuppliesRepairForm applyForm = new ApplyOfficialSuppliesRepairForm();
		applyForm.setForm_id(form_id);
		applyForm.setSupplies_id(supplies_id);
		applyForm.setPriority(priority);
		applyForm.setQty(qty);
		applyForm.setReason(reason);
		applyForm.setApp_user_id(app_user_id);
		applyForm.setApp_datetime(app_datetime);
		applyForm.setRemark(remark);

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			String return_id = applyMangeService
					.doSaveApplyOfficialSuppliesRepairForm(applyForm);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("申请信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 办公用品维修表单修改
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doUpdateApplyOfficialSuppliesRepairForm(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id_param = Type.getString(request
				.getParameter("form_id_param"));
		String supplies_id_param = Type.getString(request
				.getParameter("supplies_id_param"));
		String form_id = Type.getString(request.getParameter("form_id"));
		String supplies_id = Type
				.getString(request.getParameter("supplies_id"));
		int priority = Type.GetInt(request.getParameter("priority"));
		int qty = Type.GetInt(request.getParameter("qty"));
		String reason = Type.getString(request.getParameter("reason"));
		Date app_datetime = DateUtil.getCurrentDateTime();
		int app_user_id = WebUtil.getCurrentEmployee().getUserID();
		String remark = Type.getString(request.getParameter("remark"));

		ApplyOfficialSuppliesRepairForm applyForm = new ApplyOfficialSuppliesRepairForm();
		applyForm.setForm_id(form_id);
		applyForm.setSupplies_id(supplies_id);
		applyForm.setPriority(priority);
		applyForm.setQty(qty);
		applyForm.setReason(reason);
		applyForm.setApp_user_id(app_user_id);
		applyForm.setApp_datetime(app_datetime);
		applyForm.setRemark(remark);

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			applyMangeService.doUpdateApplyOfficialSuppliesRepairForm(
					applyForm, form_id_param, supplies_id_param);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", form_id_param);
			ajaxResponse.setMessage("修改成功！");
		} catch (Exception e) {
			logger.error("申请信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("修改错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 办公用品维修的删除
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doDeleteApplyOfficialSuppliesRepairForm(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id = Type.getString(request.getParameter("form_id"));
		String supplies_id = Type
				.getString(request.getParameter("supplies_id"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			applyMangeService.doDeleteApplyOfficialSuppliesRepairForm(form_id,
					supplies_id);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("申请信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 办公用品维修流程的启动
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doStartApplyOfficialSuppliesRepair(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id = Type.getString(request.getParameter("form_id"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			String processInstanceId = applyMangeService
					.doStartApplyOfficialSuppliesRepair(form_id);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);
		} catch (EngineException e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>找不到指定角色的人，请联系管理员~");
			ajaxResponse.setMessageTitle("失败");
		} catch (Exception e) {
			logger.error("提交出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 办公用品维修列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getApplyOfficialSuppliesRepairList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		String form_id = Type.getString(request.getParameter("form_id"));
		String jsonstr = applyMangeService
				.getApplyOfficialSuppliesRepairList(form_id);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 办公用品维修汇总列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getApplyOfficialSuppliesRepairListSum(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		String jsonstr = applyMangeService
				.getApplyOfficialSuppliesRepairListSum(pageInfo, queryStr);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 业务招待申报
	 * 
	 * */

	public void doStartApplyBusinessEntertainment(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String processInstanceId = request.getParameter("processInstanceId");
		String workItemId = request.getParameter("workItemId");

		String form_id = Type.getString(request.getParameter("form_id"));
		String visitor_company_name = request
				.getParameter("visitor_company_name");
		int visitor_qty = Type.GetInt(request.getParameter("visitor_qty"));
		Date meals_date = Type.getDateTime(request.getParameter("meals_date"));
		int meals_type = Type.GetInt(request.getParameter("meals_type"));
		BigDecimal meals_fee_budget = new BigDecimal(
				request.getParameter("meals_fee_budget"));
		int accompany_qty = Type.GetInt(request.getParameter("accompany_qty"));
		BigDecimal cost = new BigDecimal(request.getParameter("cost"));
		// BigDecimal profit = new BigDecimal(request.getParameter("profit"));
		BigDecimal cigarettes_wine_amt = new BigDecimal(
				request.getParameter("cigarettes_wine_amt"));
		BigDecimal total_amt = new BigDecimal(request.getParameter("total_amt"));
		Calendar calendar = Calendar.getInstance();
		java.util.Date app_datetime = new java.util.Date(
				calendar.getTimeInMillis());
		int app_user_id = Type.GetInt(request.getParameter("app_user_id"));
		int app_deptment = Type.GetInt(request.getParameter("app_deptment"));
		String sign_No = request.getParameter("sign_No");
		String remark = request.getParameter("remark");

		BusinessEntertainment businessEntertainment = new BusinessEntertainment();

		businessEntertainment.setForm_id(form_id);
		businessEntertainment.setVisitor_company_name(visitor_company_name);
		businessEntertainment.setVisitor_qty(visitor_qty);
		businessEntertainment.setMeals_date(meals_date);
		businessEntertainment.setMeals_type(meals_type);
		businessEntertainment.setMeals_fee_budget(meals_fee_budget);
		businessEntertainment.setAccompany_qty(accompany_qty);
		businessEntertainment.setCost(cost);
		// businessEntertainment.setProfit(profit);
		businessEntertainment.setCigarettes_wine_amt(cigarettes_wine_amt);
		businessEntertainment.setTotal_amt(total_amt);
		businessEntertainment.setApp_datetime(app_datetime);
		businessEntertainment.setApp_user_id(app_user_id);
		businessEntertainment.setApp_deptment(app_deptment);
		businessEntertainment.setSign_No(sign_No);
		businessEntertainment.setRemark(remark);
		businessEntertainment.setIs_worked(Type.GetInt(request
				.getParameter("is_worked")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			if ("".equals(form_id)) {
				processInstanceId = applyMangeService
						.doStartApplyBusinessEntertainment(businessEntertainment);
			} else {
				applyMangeService.doGoOnSubmitBusinessEntertainment(
						businessEntertainment, workItemId);
			}
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);

		} catch (EngineException e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>找不到指定角色的人，请联系管理员~");
			ajaxResponse.setMessageTitle("失败");

		} catch (Exception e) {
			logger.error("提交出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 业务招待的签单号处理
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doUpdateSignNoByFormId(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String form_id = Type.getString(request.getParameter("form_id"));
		String sign_No = request.getParameter("sign_No").trim();

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			if ("new_sign_no".equals(sign_No)) {
				String year = Calendar.getInstance().get(Calendar.YEAR) + "";
				int sign_No_Max = applyMangeService.getSign_No(year);
				sign_No = (sign_No_Max+1)+"";
				if (sign_No_Max == 0) {
					sign_No = year + "0001";
				}
			}
			applyMangeService.doUpdateSignNoByFormId(form_id, sign_No);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>");
		} catch (Exception e) {
			logger.error("提交出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 业务招待食堂人员填写后 要判断是否超额
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doUpdateApplyBusinessEntertainmentByCanteen(
			ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String processInstanceId = request.getParameter("processInstanceId");
		String workItemId = request.getParameter("workItemId");

		String form_id = Type.getString(request.getParameter("form_id"));
		String visitor_company_name = request
				.getParameter("visitor_company_name");
		int visitor_qty = Type.GetInt(request.getParameter("visitor_qty"));
		Date meals_date = Type.getDateTime(request.getParameter("meals_date"));
		int meals_type = Type.GetInt(request.getParameter("meals_type"));
		BigDecimal meals_fee_budget = new BigDecimal(
				request.getParameter("meals_fee_budget"));
		int accompany_qty = Type.GetInt(request.getParameter("accompany_qty"));
		BigDecimal cost = new BigDecimal(request.getParameter("cost"));
		// BigDecimal profit = new BigDecimal(request.getParameter("profit"));
		BigDecimal cigarettes_wine_amt = new BigDecimal(
				request.getParameter("cigarettes_wine_amt"));
		BigDecimal total_amt = new BigDecimal(request.getParameter("total_amt"));
		Calendar calendar = Calendar.getInstance();
		java.util.Date app_datetime = new java.util.Date(
				calendar.getTimeInMillis());
		int app_user_id = Type.GetInt(request.getParameter("app_user_id"));
		int app_deptment = Type.GetInt(request.getParameter("app_deptment"));
		String sign_No = request.getParameter("sign_No");
		String remark = request.getParameter("remark");

		BusinessEntertainment businessEntertainment = new BusinessEntertainment();

		businessEntertainment.setForm_id(form_id);
		businessEntertainment.setVisitor_company_name(visitor_company_name);
		businessEntertainment.setVisitor_qty(visitor_qty);
		businessEntertainment.setMeals_date(meals_date);
		businessEntertainment.setMeals_type(meals_type);
		businessEntertainment.setMeals_fee_budget(meals_fee_budget);
		businessEntertainment.setAccompany_qty(accompany_qty);
		businessEntertainment.setCost(cost);
		// businessEntertainment.setProfit(profit);
		businessEntertainment.setCigarettes_wine_amt(cigarettes_wine_amt);
		businessEntertainment.setTotal_amt(total_amt);
		businessEntertainment.setApp_datetime(app_datetime);
		businessEntertainment.setApp_user_id(app_user_id);
		businessEntertainment.setApp_deptment(app_deptment);
		businessEntertainment.setSign_No(sign_No);
		businessEntertainment.setRemark(remark);

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			applyMangeService.doUpdateApplyBusinessEntertainmentByCanteen(
					businessEntertainment, workItemId);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);
		} catch (Exception e) {
			logger.error("提交出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 获取业务招待表单信息
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getBusinessEntertainmentFormByFormId(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {

		String form_id = Type.getString(request.getParameter("form_id"));

		String jsonstr = applyMangeService
				.getBusinessEntertainmentFormByFormId(form_id);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 业务招待流程的强制结束
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void doForceStopApplyBusinessEntertainment(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String workItemId = request.getParameter("workItemId");
		String forceStop = request.getParameter("forceStop");
		String processInstanceId = request.getParameter("processInstanceId");
		String businessEntertentWorkedId = request
				.getParameter("businessEntertentWorkedId");
		String form_id = request.getParameter("form_id");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			applyMangeService.doForceStopApplyBusinessEntertainment(workItemId,
					forceStop, processInstanceId, businessEntertentWorkedId,
					form_id);
			applyMangeService.updateLastActorToNull(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("操作成功,流程结束");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败<br/>" + e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 业务招待查询列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void getBusinessEntertainmentFormList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String divisionID = request.getParameter("divisionID");
		String begDate = request.getParameter("begDate");
		String endDate = request.getParameter("endDate");
		String queryStr = " ";

		if (!StringUtils.isBlank(begDate) && !begDate.equals("")) {
			String end = endDate.substring(0, 10) + " 23:59:59";
			queryStr += " and  app_datetime between '" + begDate + "' and '"
					+ end + "'\n";
		}
		if (!StringUtils.isBlank(divisionID) && !divisionID.equals("-1")) {
			queryStr += " and  app_deptment=" + divisionID + "\n";
		}

		// 查找统计列表
		String store = applyMangeService.getBusinessEntertainmentFormList(
				queryStr, pageInfo);
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 请假申请
	 * 
	 * */

	public void doStartAskForLeave(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String processInstanceId = request.getParameter("processInstanceId");
		String workItemId = request.getParameter("workItemId");

		String form_id = Type.getString(request.getParameter("form_id"));
		Calendar calendar = Calendar.getInstance();
		java.util.Date app_datetime = new java.util.Date(
				calendar.getTimeInMillis());
		int app_user_id = Type.GetInt(request.getParameter("app_user_id"));

		AbsentForm absentForm = new AbsentForm();
		absentForm.setForm_id(form_id);
		absentForm.setAbsent_type_id(Type.GetInt(request
				.getParameter("absent_type_id")));
		absentForm.setPlan_sum(request.getParameter("plan_sum"));
		absentForm.setPlan_begin_date(Type.getDateTime(request
				.getParameter("plan_begin_date")));
		absentForm.setPlan_end_date(Type.getDateTime(request
				.getParameter("plan_end_date")));
		absentForm.setActual_begin_date(Type.getDateTime(request
				.getParameter("actual_begin_date")));
		absentForm.setActual_end_date(Type.getDateTime(request
				.getParameter("actual_end_date")));
		absentForm.setLeavetime(Type.getDateTime(request
				.getParameter("leavetime")));
		absentForm.setReason(request.getParameter("reason"));
		absentForm.setApp_datetime(app_datetime);
		absentForm.setApp_user_id(app_user_id);

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			if ("".equals(form_id)) {
				processInstanceId = applyMangeService
						.doStartAskForLeave(absentForm);
			} else {
				applyMangeService.doGoOnSubmitAskForLeave(absentForm,
						workItemId);
			}
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);
		} catch (EngineException e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>找不到指定角色的人，请联系管理员~");
			ajaxResponse.setMessageTitle("失败");
		} catch (Exception e) {
			logger.error("提交出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 获取请假类型列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getAbsentList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String store = applyMangeService.getAbsentList();
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 销假
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doUpdateAbsentForm(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String processInstanceId = request.getParameter("processInstanceId");
		String workItemId = request.getParameter("workItemId");

		String form_id = Type.getString(request.getParameter("form_id"));
		AbsentForm absentForm = new AbsentForm();
		absentForm.setForm_id(form_id);
		absentForm.setActual_begin_date(Type.getDateTime(request
				.getParameter("actual_begin_date")));
		absentForm.setActual_end_date(Type.getDateTime(request
				.getParameter("actual_end_date")));
		absentForm.setActual_sum(request.getParameter("actual_sum"));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			applyMangeService.doUpdateAbsentForm(absentForm, workItemId);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);
		} catch (Exception e) {
			logger.error("提交出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 请假单信息的获取
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getAbsentFormByFormId(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String form_id = Type.getString(request.getParameter("form_id"));

		String jsonstr = applyMangeService.getAbsentFormByFormId(form_id);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getAbsentDaysSum(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String plan_begin_date = request.getParameter("plan_begin_date");
		int plan_begin_hour = Type.getInteger(request
				.getParameter("plan_begin_hour"));
		String plan_end_date = request.getParameter("plan_end_date");
		int plan_end_hour = Type.getInteger(request
				.getParameter("plan_end_hour"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			String sumStr = applyMangeService.getAbsentDaysSum(plan_begin_date,
					plan_end_date, plan_begin_hour, plan_end_hour);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("sumStr", sumStr);
		} catch (Exception e) {
			logger.error("合计计算出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("合计计算出错！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 请假单信息列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getAbsentFormList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		String jsonstr = applyMangeService
				.getAbsentFormList(pageInfo, queryStr);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getCurrentAndNextAbsentInfo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String jsonstr = applyMangeService.getCurrentAndNextAbsentInfo();
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getAttachmentListByAbsentFormId(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id = Type.getString(request.getParameter("form_id"));

		String jsonstr = applyMangeService
				.getAttachmentListByAbsentFormId(form_id);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 公文类别下拉列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getDocumentTypeInfoCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String jsonstr = applyMangeService.getDocumentTypeInfoCombo();
		WebUtil.returnResponse(response, jsonstr);

	}

	/**
	 * 公文流转信息的保存
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void saveDocumentAppFormDetail(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		DocumentAppFormDetail applyForm = new DocumentAppFormDetail();
		String form_id_param = Type.getString(request.getParameter("form_id"));
		applyForm.setForm_id(form_id_param);
		applyForm.setDocument_type_id(Type.GetInt(request
				.getParameter("document_type_id")));
		applyForm.setPriority(Type.GetInt(request.getParameter("priority")));
		applyForm.setSummary(request.getParameter("summary"));

		applyForm.setTitle(Type.getString(request.getParameter("title")));
		applyForm.setExternal_name(request.getParameter("external_name"));
		applyForm.setDocument_code(request.getParameter("document_code"));
		applyForm.setFile_flag(request.getParameter("file_flag"));
		applyForm.setFile_no(Type.GetInt(request.getParameter("file_no")));
		applyForm.setApp_datetime(Type.getDateTime(request
				.getParameter("app_datetime")));
		applyForm.setApp_user_id(Type.GetInt(request
				.getParameter("app_user_id")));
		applyForm.setApp_dept_id(Type.GetInt(request
				.getParameter("app_dept_id")));
		applyForm.setApp_dept_leader_id(Type.GetInt(request
				.getParameter("app_dept_leader_id")));
		applyForm.setApp_dept_approved_reason(request
				.getParameter("app_dept_approved_reason"));
		applyForm.setApp_dept_approved_datetime(Type.getDateTime(request
				.getParameter("app_dept_approved_datetime")));

		applyForm.setApp_spec_leader_id(Type.GetInt(request
				.getParameter("app_spec_leader_id")));
		applyForm.setApp_spec_approved_reason(request
				.getParameter("app_spec_approved_reason"));
		applyForm.setApp_spec_approved_datetime(Type.getDateTime(request
				.getParameter("app_spec_approved_datetime")));

		applyForm.setApp_direc_leader_id(Type.GetInt(request
				.getParameter("app_direc_leader_id")));
		applyForm.setApp_direc_approved_reason(request
				.getParameter("app_direc_approved_reason"));
		applyForm.setApp_direc_approved_datetime(Type.getDateTime(request
				.getParameter("app_direc_approved_datetime")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			String form_id = applyMangeService
					.doSaveDocumentAppFormDetail(applyForm);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("form_id", form_id);
			ajaxResponse.setMessage("公文保存成功，请添加附件");
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存出错！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 公文流转信息的修改后提交
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doSubmitAfterUpdateApplyDocumentAppFormDetail(
			ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		DocumentAppFormDetail applyForm = new DocumentAppFormDetail();
		String form_id_param = Type.getString(request.getParameter("form_id"));
		applyForm.setForm_id(form_id_param);
		applyForm.setDocument_type_id(Type.GetInt(request
				.getParameter("document_type_id")));
		applyForm.setPriority(Type.GetInt(request.getParameter("priority")));

		applyForm.setTitle(request.getParameter("title"));
		applyForm.setExternal_name(request.getParameter("external_name"));
		applyForm.setDocument_code(request.getParameter("document_code"));

		applyForm.setSummary(request.getParameter("summary"));

		applyForm.setApp_datetime(Type.getDateTime(request
				.getParameter("app_datetime")));
		applyForm.setApp_user_id(Type.GetInt(request
				.getParameter("app_user_id")));
		applyForm.setApp_dept_id(Type.GetInt(request
				.getParameter("app_dept_id")));
		applyForm.setApp_dept_leader_id(Type.GetInt(request
				.getParameter("app_dept_leader_id")));
		applyForm.setApp_dept_approved_reason(request
				.getParameter("app_dept_approved_reason"));
		applyForm.setApp_dept_approved_datetime(Type.getDateTime(request
				.getParameter("app_dept_approved_datetime")));

		applyForm.setApp_spec_leader_id(Type.GetInt(request
				.getParameter("app_spec_leader_id")));
		applyForm.setApp_spec_approved_reason(request
				.getParameter("app_spec_approved_reason"));
		applyForm.setApp_spec_approved_datetime(Type.getDateTime(request
				.getParameter("app_spec_approved_datetime")));

		applyForm.setApp_direc_leader_id(Type.GetInt(request
				.getParameter("app_direc_leader_id")));
		applyForm.setApp_direc_approved_reason(request
				.getParameter("app_direc_approved_reason"));
		applyForm.setApp_direc_approved_datetime(Type.getDateTime(request
				.getParameter("app_direc_approved_datetime")));

		String workItemId = request.getParameter("workItemId");

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String processInstanceId = applyMangeService
					.doSubmitAfterUpdateApplyDocumentAppFormDetail(applyForm,
							workItemId);
			ajaxResponse.setSuccess(true);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作出错！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 根据form_id得到公文申请页面的信息
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws IOException
	 */
	public void getDocumentAppFormDetailByFormId(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws IOException {

		String form_id = request.getParameter("form_id");
		String jsonstr = applyMangeService
				.getDocumentAppFormDetailByFormId(form_id);
		if (Type.GetInt(request.getParameter("isApp")) == 1) {
			response.setContentType("application/json;charset=UTF-8");
			response.getWriter().write(jsonstr.toString());
		} else {
			WebUtil.returnResponse(response, jsonstr);
		}
	}

	/**
	 * 公文申请
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doStartApplyDocumentAppFormDetail(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id = Type.getString(request.getParameter("form_id"));
		int type = Type.GetInt(request.getParameter("type"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			String processInstanceId = applyMangeService
					.doStartApplyDocumentAppFormDetail(form_id, type);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);
		} catch (EngineException e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>找不到指定角色的人，请联系管理员~");
			ajaxResponse.setMessageTitle("失败");
		} catch (Exception e) {
			logger.error("提交出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doApprovalTaskWithFormInfo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String workItemId = request.getParameter("workItemId");
		String isAgree = request.getParameter("isAgree");
		String comment = request.getParameter("comment");
		String step = request.getParameter("step");
		String form_id = request.getParameter("form_id");

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String processInstanceId = applyMangeService
					.doApprovalTaskWithFormInfo(workItemId, isAgree, comment,
							step, form_id);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);
		} catch (Exception e) {
			logger.error("提交报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("提交失败<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 公文传阅的提交
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doUpdateDocumentCirculatedInstances(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String to_id_str = request.getParameter("to_id");

		DocumentCiculatedInstances instancesObj = new DocumentCiculatedInstances();
		instancesObj.setInstanc_id(request.getParameter("instanc_id"));
		instancesObj.setForm_id(request.getParameter("instanc_id"));
		instancesObj.setFrom_id(Type.GetInt(request.getParameter("from_id")));
		instancesObj.setFrom_datetime(Type.getDateTime(request
				.getParameter("from_datetime")));
		// instancesObj.setTo_id(Type.GetInt(request.getParameter("to_id")));
		instancesObj.setComments(request.getParameter("comments"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			applyMangeService.doInsertDocumentCirculatedInstances(instancesObj,
					to_id_str);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/> ");
		} catch (Exception e) {
			logger.error("提交报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("提交失败<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void getDocumentCirculatedInstancesList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String form_id = request.getParameter("form_id");
		if (form_id == null) {
			return;
		}
		String jsonstr = applyMangeService
				.getDocumentCirculatedInstancesList(form_id);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getDocumentAppFormDetailList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		String jsonstr = applyMangeService.getDocumentAppFormDetailList(
				pageInfo, queryStr);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 部门 月报计划
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doStartMonthPlan(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String processInstanceId = request.getParameter("processInstanceId");
		String workItemId = request.getParameter("workItemId");

		String form_id = Type.getString(request.getParameter("form_id"));
		int app_user_id = Type.GetInt(request.getParameter("app_user_id"));
		int app_dept_id = Type.GetInt(request.getParameter("app_dept_id"));
		Calendar calendar = Calendar.getInstance();
		java.util.Date app_date = new java.util.Date(calendar.getTimeInMillis());

		EarlyPlans earlyPlans = new EarlyPlans();

		earlyPlans.setFormId(form_id);
		earlyPlans.setAppDate(app_date);
		earlyPlans.setAppUserId(app_user_id);
		earlyPlans.setAppDeptId(app_dept_id);
		earlyPlans.setPlanAssignComments(Type.getString(request
				.getParameter("plan_assign_comments")));
		// earlyPlans.setPlanAssignedDate(Type.getDateTime(request.getParameter("plan_assigned_date")));
		// earlyPlans.setPlanAssignedId(Type.GetInt(request.getParameter("plan_assigned_id")));
		earlyPlans.setPlanComments(request.getParameter("plan_comments"));
		earlyPlans.setWorkMonth(request.getParameter("work_month"));
		earlyPlans.setTypeId(Type.GetInt(request.getParameter("type_id")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			if ("".equals(form_id)) {
				processInstanceId = applyMangeService
						.doStartMonthPlan(earlyPlans);
			} else {
				applyMangeService.doGoOnSubmitMonthPlan(earlyPlans, workItemId);
			}
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);
		} catch (EngineException e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>找不到指定角色的人，请联系管理员~");
			ajaxResponse.setMessageTitle("失败");
		} catch (Exception e) {
			logger.error("提交出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void getMonthPlanByFormId(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String form_id = Type.getString(request.getParameter("form_id"));

		String jsonstr = applyMangeService.getMonthPlanByFormId(form_id);
		WebUtil.returnResponse(response, jsonstr);
	}

	/**
	 * 月末计划
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void doStartEndOfMonthPlan(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		String processInstanceId = request.getParameter("processInstanceId");
		String workItemId = request.getParameter("workItemId");
		String form_id = Type.getString(request.getParameter("form_id"));
		TheEndOfThePlanned endOfThePlanned = new TheEndOfThePlanned();
		endOfThePlanned.setFormId(form_id);
		endOfThePlanned.setExcutedAssignedComments(request
				.getParameter("excuted_assigned_comments"));
		endOfThePlanned.setExcutedComments(request
				.getParameter("excuted_comments"));
		endOfThePlanned.setUnfinishedComments(request
				.getParameter("unfinished_comments"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			applyMangeService
					.doUpdateMonthPlanForm(endOfThePlanned, workItemId);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);
		} catch (Exception e) {
			logger.error("提交出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void updateMonthPlanComments(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {

		int EarlyOrEnd = Type.GetInt(request.getParameter("earlyOrEnd"));
		String workItemId = request.getParameter("workItemId");
		String isAgree = request.getParameter("isAgree");
		String comment = request.getParameter("comment");
		String form_id = Type.getString(request.getParameter("form_id"));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {

			int user_id = WebUtil.getCurrentEmployee().getUserID();
			if (EarlyOrEnd == 0) {
				EarlyPlans earlyPlans = new EarlyPlans();

				earlyPlans.setFormId(form_id);
				earlyPlans.setPlanAssignedId(user_id);
				earlyPlans.setPlanAssignComments(comment);
				Calendar calendar = Calendar.getInstance();
				java.util.Date planAssignedDate = new java.util.Date(
						calendar.getTimeInMillis());
				earlyPlans.setPlanAssignedDate(planAssignedDate);
				applyMangeService.doUpdateMonthPlanComments(earlyPlans);
			} else if (EarlyOrEnd == 1) {
				TheEndOfThePlanned endOfThePlanned = new TheEndOfThePlanned();
				endOfThePlanned.setFormId(form_id);
				endOfThePlanned.setExcutedAssignedId(user_id);
				endOfThePlanned.setExcutedAssignedComments(comment);
				Calendar calendar = Calendar.getInstance();
				java.util.Date excuteAssignedDate = new java.util.Date(
						calendar.getTimeInMillis());
				endOfThePlanned.setExcuteAssignedDate(excuteAssignedDate);
				;
				applyMangeService.doUpdateMonthPlanComments(endOfThePlanned);
			}
			String processInstanceId = applyMangeService.doApprovalTask(
					workItemId, isAgree, comment);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功<br/>" + infoMsg);
		} catch (Exception e) {
			logger.error("提交报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("提交失败<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void monthPlanQuery(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String deptId = request.getParameter("divisionID");
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String workMonth = request.getParameter("workMonth");
		String jsStr = applyMangeService.getMonthPlanList(pageInfo, deptId,
				workMonth);
		WebUtil.returnResponse(response, jsStr);
	}

	public void getGczlInfoList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		String jsonstr = applyMangeService.getGczlInfoList(pageInfo, queryStr);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getGczlInfoListWithGroupQuery(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String jsonstr = applyMangeService.getGczlInfoListWithGroupQuery(
				pageInfo, queryStr, query);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void doSaveGczlInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws IllegalAccessException, InvocationTargetException {
		GczlInfo info = new GczlInfo();

		info.setTab_seed(Type.GetInt(request.getParameter("tab_seed")));
		info.setUnit_code(request.getParameter("unit_code"));
		info.setMeasure_date(Type.getDateTime(request
				.getParameter("measure_date")));
		info.setAp_w_level(new BigDecimal(request.getParameter("ap_w_level")));
		info.setBp_w_level(new BigDecimal(request.getParameter("bp_w_level")));
		info.setGate_open_num_1(Type.GetInt(request
				.getParameter("gate_open_num_1")));
		info.setGate_open_num_2(Type.GetInt(request
				.getParameter("gate_open_num_2")));
		info.setGate_open_degree_1(request.getParameter("gate_open_degree_1"));
		info.setGate_open_degree_2(request.getParameter("gate_open_degree_2"));
		info.setElec_machine(Type.GetInt(request.getParameter("elec_machine")));
		info.setFlow_qty(Type.GetInt(request.getParameter("flow_qty")));
		info.setW_level_chan(new BigDecimal(request
				.getParameter("w_level_chan")));
		info.setMemo(request.getParameter("memo"));
		info.setMeasure_man(Type.GetInt(request.getParameter("measure_man")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int return_id = applyMangeService.doSaveGczlInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doDeleteGczlInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int tab_seed = Type.GetInt(request.getParameter("tab_seed"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			applyMangeService.doDeleteGczlInfoById(tab_seed);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void getGczlInfoListWithQuery(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String b_date = request.getParameter("b_date");
		String e_date = request.getParameter("e_date");
		String jsonstr = applyMangeService.getGczlInfoListWithQuery(b_date,
				e_date);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getAskForLeaveList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String year = request.getParameter("year");
		String month = request.getParameter("month");
		String hrName = request.getParameter("hrName");
		String date = "";
		if (month == "全年" || "全年".equals(month)) {
			date = year;
		} else {
			date = year + "-" + month;
		}

		String jsonstr = applyMangeService.getAskForLeaveList(pageInfo, hrName,
				date);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getColumnNameCombo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String jsonstr = applyMangeService.getColumnNameCombo();
		WebUtil.returnResponse(response, jsonstr);

	}

	public void getCeYaColumnNameCombo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String jsonstr = applyMangeService.getCeYaColumnNameCombo();
		WebUtil.returnResponse(response, jsonstr);

	}

	public void getGczlInfoListComparison(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String b_year = request.getParameter("b_year");
		String e_year = request.getParameter("e_year");
		String month = request.getParameter("month");
		String jsonstr = applyMangeService.getGczlInfoListComparison(b_year,
				e_year, month);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getGczlInfoExcretion(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String b_date = request.getParameter("b_date");
		String e_date = request.getParameter("e_date");
		String jsonstr = applyMangeService.getGczlInfoExcretion(b_date, e_date,
				pageInfo);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getExcretionSum(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String b_year = Type.getString(request.getParameter("b_year"));
		String e_year = Type.getString(request.getParameter("e_year"));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String sum = applyMangeService.getExcretionSum(b_year, e_year);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("sum", sum);
		} catch (Exception e) {
			logger.error("验证出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("验证错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void getGczlInfoListForFlood(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String year = request.getParameter("year");
		String flood = request.getParameter("flood");
		String jsonstr = applyMangeService.getGczlInfoListForFlood(year, flood);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getGczlInfoListForFloodGates(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String b_date = request.getParameter("b_date");
		String e_date = request.getParameter("e_date");
		String floodGates = request.getParameter("floodGates");
		String jsonstr = applyMangeService.getGczlInfoListForFloodGates(b_date,
				e_date, floodGates);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getGczlInfoEveryMonthLevel(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String b_date = request.getParameter("syear");
		String e_date = request.getParameter("eyear");
		String type = request.getParameter("type");
		String jsonstr = applyMangeService.getGczlInfoEveryMonthLevel(b_date,
				e_date, type);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getMptpCodeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = applyMangeService.getMptpCodeList();
		WebUtil.returnResponse(response, store);
	}

	public void getMptCodeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String mptp_code = request.getParameter("mptp_code");
		String store = applyMangeService.getMptCodeList(mptp_code);
		WebUtil.returnResponse(response, store);
	}

	public void doSavecyInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws IllegalAccessException, InvocationTargetException {
		CeyaInfo info = new CeyaInfo();

		info.setTab_seed(Type.GetInt(request.getParameter("tab_seed")));
		info.setUnit_code(request.getParameter("unit_code"));
		info.setMeasure_date(Type.getDateTime(request
				.getParameter("measure_date")));
		info.setAp_w_level(new BigDecimal(request.getParameter("ap_w_level")));
		info.setBp_w_level(new BigDecimal(request.getParameter("bp_w_level")));
		info.setTubo_w_level(new BigDecimal(request
				.getParameter("tubo_w_level")));
		info.setMptp_code(request.getParameter("mptp_code"));
		info.setMemo(request.getParameter("memo"));
		info.setMpt_name(request.getParameter("mpt_name"));
		info.setMeasure_man(Type.GetInt(request.getParameter("measure_man")));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			int return_id = applyMangeService.doSavecyInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("return_id", return_id);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("信息的保存出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doDeletecyInfoById(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		int tab_seed = Type.GetInt(request.getParameter("tab_seed"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			applyMangeService.doDeletecyInfoById(tab_seed);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
		} catch (Exception e) {
			logger.error("信息的删除出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void getCyInfoListWithGroupQuery(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String queryStr = request.getParameter("queryStr");// js 做trim
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String jsonstr = applyMangeService.getCyInfoListWithGroupQuery(
				pageInfo, queryStr, query);
		WebUtil.returnResponse(response, jsonstr);
	}

	public void getTubo_high_degree(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String mptp_code = request.getParameter("mptp_code");
		String mpt_name = request.getParameter("mpt_name");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			BigDecimal num = applyMangeService.getTubo_high_degree(mptp_code,
					mpt_name);
			// applyMangeService.updateLastActorToNull(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage(num + "");
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void getFileFlagList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = applyMangeService.getFileFlagList();
		WebUtil.returnResponse(response, store);
	}
	public void dosendHuiQianMessage(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String type = request.getParameter("type");
		String indexStr = request.getParameter("indexStr");
		String formId = request.getParameter("formId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			applyMangeService.dosendHuiQianMessage(indexStr,formId,type);
			// applyMangeService.updateLastActorToNull(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("发送成功！");
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void doupdateHuiQianMessage(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String type = request.getParameter("type");
		String isAgree = request.getParameter("isAgree");
		String formId = request.getParameter("formId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			applyMangeService.doupdateHuiQianMessage(isAgree, formId, type);
			// applyMangeService.updateLastActorToNull(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("发送成功！");
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getHQ_nameByFormId(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String type = request.getParameter("type");
		String formId = request.getParameter("formId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String HQ_name = applyMangeService.getHQ_nameByFormId(formId,type);
			// applyMangeService.updateLastActorToNull(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("HQ_name", HQ_name);
			ajaxResponse.setMessage("发送成功！");
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getDoDayReportData(ActionMapping mapping, ActionForm form,
								   HttpServletRequest request, HttpServletResponse response)throws Exception {
		String datetime = request.getParameter("datetime");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String data = applyMangeService.getDoDayReportData(datetime);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("data",data);
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("出现错误，请重试！");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void queryDoDayReportData(ActionMapping mapping, ActionForm form,
								   HttpServletRequest request, HttpServletResponse response)throws Exception {
		String startDate = request.getParameter("startDate");
		String endDate = request.getParameter("endDate");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String data = applyMangeService.queryDoDayReportData(startDate,endDate);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("data",data);
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("出现错误，请重试！");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void saveDoDayReportData(ActionMapping mapping, ActionForm form,
								   HttpServletRequest request, HttpServletResponse response)throws Exception {
		String data = request.getParameter("data");
		List<Map<String,String>> list = JacksonUtil.readValue(data,ArrayList.class);

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			applyMangeService.saveDoDayReportData(list);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("data",data);
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("出现错误，请重试！");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void deleteDoDayReportData(ActionMapping mapping, ActionForm form,
								   HttpServletRequest request, HttpServletResponse response)throws Exception {
		String queryDate = request.getParameter("queryDate");
		String queryPerson = request.getParameter("queryPerson");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			applyMangeService.deleteDoDayReportData(queryDate, queryPerson);
			ajaxResponse.setSuccess(true);
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("出现错误，请重试！");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void queryzmDayReportData(ActionMapping mapping, ActionForm form,
								   HttpServletRequest request, HttpServletResponse response)throws Exception {
		String currentDate = request.getParameter("currentDate");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String data = applyMangeService.queryzmDayReportData(currentDate);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("data",data);
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("出现错误，请重试！");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void getTopReportData(ActionMapping mapping, ActionForm form,
								   HttpServletRequest request, HttpServletResponse response)throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String startDate = request.getParameter("startDate");
			String endDate = request.getParameter("endDate");
			String data = applyMangeService.getTopReportData(startDate,endDate);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("data",data);
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("出现错误，请重试！");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void doCreateDdWord(ActionMapping mapping, ActionForm form,
								   HttpServletRequest request, HttpServletResponse response)throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String queryDate = request.getParameter("queryDate");
			String data = applyMangeService.doCreateDdWord(queryDate);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("data",data);
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("出现错误，请重试！");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	public void exportzmDayReportData(ActionMapping mapping, ActionForm form,
								   HttpServletRequest request, HttpServletResponse response)throws Exception {
		String currentDate = request.getParameter("currentDate");
		try {
			applyMangeService.exportzmDayReportData(response,currentDate);
		} catch (Exception e) {
			logger.error("操作报错", e);
			e.printStackTrace();
			throw e;
		}
	}
	public void exportdoDayReportData(ActionMapping mapping, ActionForm form,
								   HttpServletRequest request, HttpServletResponse response)throws Exception {
		String startDate = request.getParameter("startDate");
		String endDate = request.getParameter("endDate");
		try {
			applyMangeService.exportdoDayReportData(response, startDate, endDate);
		} catch (Exception e) {
			logger.error("操作报错", e);
			e.printStackTrace();
			throw e;
		}
	}
}
