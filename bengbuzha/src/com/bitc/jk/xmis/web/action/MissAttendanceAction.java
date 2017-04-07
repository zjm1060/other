package com.bitc.jk.xmis.web.action;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.MissAttendanceInfos;
import com.bitc.jk.xmis.model.MissAttendanceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.service.MissAttendanceService;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;
import com.bitc.jk.xmis.workflow.service.WorkflowService;
import com.bitc.jk.xmis.workflow.util.Constants;
import org.apache.commons.lang.StringUtils;

public class MissAttendanceAction extends DispatchAction {
	private static Logger logger = Logger.getLogger(MissAttendanceAction.class);

	public MissAttendanceService missAttenceService;
	public WorkflowService workflowService;

	public WorkflowService getWorkflowService() {
		return workflowService;
	}

	public void setWorkflowService(WorkflowService workflowService) {
		this.workflowService = workflowService;
	}

	public MissAttendanceService getMissAttenceService() {
		return missAttenceService;
	}

	public void setMissAttenceService(MissAttendanceService missAttenceService) {
		this.missAttenceService = missAttenceService;
	}

	public void startMissAttence(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String applicantID = request.getParameter("applicantID");
		String isAgent = request.getParameter("isAgent");
		String isOfficialTravel = request.getParameter("isOfficialTravel");// 非出差

		MissAttendanceObj missAttendanceObj = new MissAttendanceObj();
		missAttendanceObj.setApplicant(new SimpleEmployee(Type
				.GetInt(applicantID)));
		missAttendanceObj.setIsAgent(Type.GetInt(isAgent));
		missAttendanceObj.setIsOfficialTravel(Type.GetInt(isOfficialTravel));
		missAttendanceObj.setRegistdate(DateUtil.getCurrentDateTime());

		String memberJson = request.getParameter("members");

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String processInstanceId = missAttenceService.doStartMissAttence(
					missAttendanceObj, memberJson);
			String infoMsg = workflowService
					.getCurrentActorName(processInstanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("提交成功。<br/>" + infoMsg);
			ajaxResponse.setMessageTitle("提示");
		} catch (Exception e) {
			logger.error("提交报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>" + e.getMessage());
			ajaxResponse.setMessageTitle("提示");
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doSaveMissAttence(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String missAttendanceId = request.getParameter("missAttendanceId");
		String applicantID = request.getParameter("applicantID");
		String isAgent = request.getParameter("isAgent");
		String isOfficialTravel = request.getParameter("isOfficialTravel");// 非出差

		String amorpm = request.getParameter("amorpm");
		String missDate = request.getParameter("missDate");
		String missDesc = request.getParameter("missDesc");
		String startDate = request.getParameter("startDate");
		String stopDate = request.getParameter("stopDate");

		MissAttendanceObj missAttendanceObj = new MissAttendanceObj();
		missAttendanceObj.setApplicant(new SimpleEmployee(Type
				.GetInt(applicantID)));
		missAttendanceObj.setIsAgent(Type.GetInt(isAgent));
		missAttendanceObj.setIsOfficialTravel(Type.GetInt(isOfficialTravel));
		missAttendanceObj.setRegistdate(DateUtil.getCurrentDateTime());

		MissAttendanceInfos info = new MissAttendanceInfos();
		info.setAmorpm(Type.GetInt(amorpm));
		info.setMissAttendanceId(missAttendanceId);
		info.setMissDesc(missDesc);
		if (!StringUtils.isBlank(missDate)) {
			info.setMissDate(DateUtil.parseDateTime(missDate));
		}
		if (!StringUtils.isBlank(startDate)) {
			info.setStartDate(DateUtil.parseDateTime(startDate));
			info.setStopDate(DateUtil.parseDateTime(stopDate));
		}

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			missAttendanceId = missAttenceService.doSaveMissAttence(
					missAttendanceId, missAttendanceObj, info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("保存报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存失败<br/>" + e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/**
	 * 被退回考勤单的再次提交
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void doGoOnSubmitMissAttence(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String workItemId = request.getParameter("workItemId");
		String missAttendanceId = request.getParameter("missAttendanceId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String processInstanceId = missAttenceService
					.doGoOnSubmitMissAttence(missAttendanceId, workItemId);

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

	public void doEndMissAttence(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String workItemId = request.getParameter("workItemId");
		String missAttendanceId = request.getParameter("missAttendanceId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			missAttenceService.doEndMissAttence(workItemId, missAttendanceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("操作成功");
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
	 * 考勤单内容的 新建
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void doSaveMissAttendanceInfo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String missAttendanceId = request.getParameter("missAttendanceId");
		String amorpm = request.getParameter("amorpm");
		String missDate = request.getParameter("missDate");
		String missDesc = request.getParameter("missDesc");
		String startDate = request.getParameter("startDate");
		String stopDate = request.getParameter("stopDate");
		MissAttendanceInfos info = new MissAttendanceInfos();
		info.setAmorpm(Type.GetInt(amorpm));
		info.setMissAttendanceId(missAttendanceId);
		info.setMissDesc(missDesc);
		if (!StringUtils.isBlank(startDate)) {
			info.setStartDate(DateUtil.parseDateTime(startDate));
			info.setStopDate(DateUtil.parseDateTime(stopDate));
		}
		if (!StringUtils.isBlank(missDate)) {
			info.setMissDate(DateUtil.parseDateTime(missDate));
		}
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String infoId = missAttenceService.doUpdateMissAttendanceInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("infoId", infoId);
			ajaxResponse.setMessage("操作成功");
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
	 * 考勤单内容的修改
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void doUpdateMissAttendanceInfo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String infoId = request.getParameter("infoId");
		String amorpm = request.getParameter("amorpm");
		String missDate = request.getParameter("missDate");
		String missDesc = request.getParameter("missDesc");
		String startDate = request.getParameter("startDate");
		String stopDate = request.getParameter("stopDate");
		MissAttendanceInfos info = missAttenceService
				.getMissAttendanceInfoById(infoId);
		info.setAmorpm(Type.GetInt(amorpm));
		info.setMissDesc(missDesc);
		if (!StringUtils.isBlank(startDate)) {
			info.setStartDate(DateUtil.parseDateTime(startDate));
			info.setStopDate(DateUtil.parseDateTime(stopDate));
		}
		if (!StringUtils.isBlank(missDate)) {
			info.setMissDate(DateUtil.parseDateTime(missDate));
		}
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			missAttenceService.doUpdateMissAttendanceInfo(info);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("操作成功");
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
	 * 考勤单内容的 修改
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void doDeleteMissAttendanceInfo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String infoId = request.getParameter("infoId");

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			missAttenceService.doDeleteMissAttendanceInfo(infoId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("操作成功");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败<br/>" + e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void doUpdateMissAttendanceRecordState(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String infoId = request.getParameter("infoId");
		int state = Type.GetInt(request.getParameter("state"));
		int year = Type.GetInt(request.getParameter("year"));
		int month = Type.GetInt(request.getParameter("month"));

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			missAttenceService.doUpdateMissAttendanceRecordState(infoId, year,
					month, state);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("操作成功");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("操作报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败<br/>" + e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void getMissAttendanceList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		SimpleEmployee emp = WebUtil.getCurrentEmployee();
		String todoIds = request.getParameter("todoIds");
		String isTodo = request.getParameter("isTodo");
		if (isTodo.equals("true")) {
			isTodo = "0";
		} else {
			isTodo = "1";
		}
		String isOfficialTravel = request.getParameter("isOfficialTravel");
		String todoIdsStr = todoIds.replaceAll(",", "','");
		todoIdsStr = "('" + todoIdsStr + "')";

		StandardQuery queryOr = CompositParam.buildOrCompositParam().addStandardQuery(
						SimpleParam.buildSimpleParam("状态", 0,
								Type.INTEGER, RelationalOperator.EQ)).addStandardQuery(
										SimpleParam.buildSimpleParam("状态", 1,
												Type.INTEGER, RelationalOperator.EQ));
		StandardQuery query = CompositParam
				.buildAndCompositParam()
				.addStandardQuery(queryOr)
				.addStandardQuery(
						SimpleParam.buildSimpleParam("出差单标识",
								Type.GetInt(isOfficialTravel), Type.INTEGER,
								RelationalOperator.EQ))
		// .addStandardQuery(
		// SimpleParam.buildSimpleParam("用户ID",
		// Type.getString(emp.getUserID()), Type.STRING,
		// RelationalOperator.EQ))
		;
		String todoIdsQuery = "";
		if (!StringUtils.isBlank(todoIds)) {
			todoIdsQuery = " and todo.ID in " + todoIdsStr;
		}
		// 查找统计列表
		String store = missAttenceService.getMissAttendanceList(query,
				pageInfo, todoIdsQuery);
		WebUtil.returnResponse(response, store);
	}

	public void getMissAttendanceListByEmpId(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String wokItemId = request.getParameter("workItemId");
		String isTodo = request.getParameter("isTodo");
		if (isTodo.equals("true")) {
			isTodo = "0";
		} else {
			isTodo = "1";
		}
		String isOfficialTravel = request.getParameter("isOfficialTravel");

		// 查找统计列表
		String store = missAttenceService.getMissAttendanceListByEmpId(
				isOfficialTravel, Constants.TODO_MODEL_ID, wokItemId, isTodo,
				pageInfo);
		WebUtil.returnResponse(response, store);
	}

	public void doApprovalTask(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String workItemIds = request.getParameter("ids");
		String isAgree = request.getParameter("isAgree");
		String comment = request.getParameter("comment");

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String processInstanceId = missAttenceService.doApprovalTaskList(
					workItemIds, isAgree, comment);
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

	// public void doResignTask(ActionMapping mapping, ActionForm form,
	// HttpServletRequest request, HttpServletResponse response) {
	// String workItemId = request.getParameter("workItemId");
	//
	// ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
	// try {
	// missAttenceService
	// .doResignTasks(workItemId);
	// ajaxResponse.setSuccess(true);
	// ajaxResponse.setMessageTitle("提示");
	// ajaxResponse.setMessage("操作成功");
	// } catch (Exception e) {
	// logger.error("提交报错", e);
	// ajaxResponse.setSuccess(false);
	// ajaxResponse.setMessageTitle("提示");
	// ajaxResponse.setMessage("提交失败<br/>" + e.getMessage());
	// }
	// WebUtil.returnResponse(response, ajaxResponse.toString());
	// }

	/**
	 * 删除统计待办
	 * 
	 * @param workItemId
	 */

	public void deleteTodoCountByTodoId(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String todoId = request.getParameter("todoId");

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			missAttenceService.deleteTodoCountByTodoId(todoId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("统计报表审批已完成！");
		} catch (Exception e) {
			logger.error("删除统计报表报错", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("删除统计报表失败<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void getMissAttendanceListByQuery(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String isOfficialTravel = request.getParameter("isOfficialTravel");
		String divisionID = request.getParameter("divisionID");
		// String begDate = request.getParameter("begDate");
		// String endDate = request.getParameter("endDate");
		String yearID = request.getParameter("yearID");
		if (StringUtils.isBlank(yearID)) {
			yearID = DateUtil.getYear(DateUtil.getCurrentDate());
		}
		String monthID = request.getParameter("monthID");
		if (StringUtils.isBlank(monthID)) {
			monthID = DateUtil.getMonth(DateUtil.getCurrentDate());
		}

		StandardQuery queryByInkling = WebUtil
				.generateStandardQueryByInkling(request);

		StandardQuery query = CompositParam
				.buildAndCompositParam()
				.addStandardQuery(
						SimpleParam.buildSimpleParam("出差单标识",
								Type.getInteger(isOfficialTravel),
								Type.INTEGER, RelationalOperator.EQ))
				.addStandardQuery(queryByInkling);
		if (!StringUtils.isBlank(divisionID) && !divisionID.equals("-1")) {

			SimpleParam divisionQuery = SimpleParam.buildSimpleParam("事业部ID",
					Type.getInteger(divisionID), Type.INTEGER,
					RelationalOperator.EQ);

			query = CompositParam.buildAndCompositParam()
					.addStandardQuery(divisionQuery).addStandardQuery(query);
		}
		// if(!StringUtils.isBlank(begDate)){
		//
		// SimpleParam begDateQuery = SimpleParam.buildSimpleParam("缺勤日期",
		// begDate , Type.DATE,
		// RelationalOperator.GE);
		//
		// query = CompositParam
		// .buildAndCompositParam()
		// .addStandardQuery(begDateQuery).addStandardQuery(query);
		// }
		// if(!StringUtils.isBlank(endDate)){
		//
		// SimpleParam endDateQuery = SimpleParam.buildSimpleParam("缺勤日期",
		// endDate , Type.DATE,
		// RelationalOperator.LE);
		//
		// query = CompositParam
		// .buildAndCompositParam()
		// .addStandardQuery(endDateQuery).addStandardQuery(query);
		// }
		String dateFirst = "";
		String dateLast = "";
		if (!StringUtils.isBlank(monthID)) {
			int year = Type.GetInt(yearID);
			int month = Type.GetInt(monthID);
			if (month < 10) {
				monthID = "0" + monthID;
			}
			if (isOfficialTravel.equals("1")) {
				SimpleParam monthQuery = SimpleParam.buildSimpleParam(
						"SUBSTRING(CONVERT(VARCHAR(10),缺勤日期,23),0,8)", yearID
								+ "-" + monthID, Type.DATE,
						RelationalOperator.EQ);

				query = CompositParam.buildAndCompositParam()
						.addStandardQuery(monthQuery).addStandardQuery(query);
			} else {

				dateLast = DateUtil.getLastDayOfMonth(year, month);// 当月的最后一天
				dateFirst = DateUtil.getFirstDayOfMonth(year, month);// 当月的第一天
			}

		}

		// 查找统计列表
		String store = missAttenceService.getMissAttendanceListByQuery(query,
				pageInfo, dateFirst, dateLast);
		WebUtil.returnResponse(response, store);
	}

	public void getMissAttendanceInfos(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String workItemId = request.getParameter("workItemId");

		String store = missAttenceService.getMissAttendanceInfos(workItemId);
		WebUtil.returnResponse(response, store);
	}

	public void getMissAttendanceObjByProcessInstanceId(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String processInstanceId = request.getParameter("processInstanceId");
		String applicantID = "";
		String isOfficialTravel = "";
		String isAgent = "";
		String missAttendanceId = "";
		String applicantdate = "";
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		MissAttendanceObj obj;
		try {
			obj = missAttenceService.getMissAttendanceObj(processInstanceId);
			if (obj != null) {
				applicantID = obj.getApplicant().getUserID() + "";
				isOfficialTravel = obj.getIsOfficialTravel() + "";
				isAgent = obj.getIsAgent() + "";
				missAttendanceId = obj.getId();
				applicantdate = DateUtil.formatDate(obj.getRegistdate(),
						"yyyy-MM-dd");
			}
			ajaxResponse.addAttribute("applicantID", applicantID);
			ajaxResponse.addAttribute("isOfficialTravel", isOfficialTravel);
			ajaxResponse.addAttribute("isAgent", isAgent);
			ajaxResponse.addAttribute("missAttendanceId", missAttendanceId);
			ajaxResponse.addAttribute("applicantdate", applicantdate);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("操作成功");
			ajaxResponse.setMessageTitle("提示");

		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("操作失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public ActionForward showPage(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String applicantID = request.getParameter("applicantID");
		String isOfficialTravel = request.getParameter("isOfficialTravel");
		String isAgent = request.getParameter("isAgent");
		String missAttendanceId = request.getParameter("missAttendanceId");
		String applicantdate = request.getParameter("applicantdate");

		request.setAttribute("applicantID", applicantID);
		request.setAttribute("isOfficialTravel", isOfficialTravel);
		request.setAttribute("isAgent", isAgent);
		request.setAttribute("missAttendanceId", missAttendanceId);
		request.setAttribute("applicantdate", applicantdate);
		return mapping.findForward("missAttendanceReturn");
	}

	public void getTaskList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		try {
			PageInfo pageInfo = WebUtil.generatePageInfo(request);
			String task_ID = request.getParameter("task_ID");
			SimpleEmployee employee = WebUtil.getCurrentEmployee(request
					.getSession(false));
			StandardQuery query = CompositParam
					.buildAndCompositParam()
					.addStandardQuery(
							SimpleParam.buildSimpleParam("用户ID",
									employee.getUserID() + "", Type.STRING,
									RelationalOperator.EQ))
					.addStandardQuery(
							SimpleParam.buildSimpleParam("task_ID", task_ID,
									Type.STRING, RelationalOperator.EQ));
			String store = missAttenceService.getApprovalTaskListByQuery(query,
					pageInfo);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
