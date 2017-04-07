package com.bitc.jk.xmis.service.impl;

import java.io.IOException;
import java.text.ParseException;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import org.codehaus.jackson.JsonParseException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.type.TypeReference;

import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.IWorkItem;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.RuntimeContext;
import org.fireflow.engine.impl.WorkItem;
import org.fireflow.kernel.KernelException;

import com.bitc.jk.xmis.dao.MissAttendanceDAO;
import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.EmailMessageInfo;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.MissAttendanceInfos;
import com.bitc.jk.xmis.model.MissAttendanceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.msg.EmailAndPlatformMessageInfoSender;
import com.bitc.jk.xmis.service.MissAttendanceService;
import com.bitc.jk.xmis.service.OrganizationService;
import com.bitc.jk.xmis.task.Task;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.ExpressionFunctions;
import com.bitc.jk.xmis.util.JacksonUtil;
import com.bitc.jk.xmis.util.TemplateUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.WorkflowConstants;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.dao.TodoDAO;
import com.bitc.jk.xmis.workflow.dao.WfCalendarDAO;
import com.bitc.jk.xmis.workflow.dao.WorkflowDAO;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;
import com.bitc.jk.xmis.workflow.model.WfCalendar;
import com.bitc.jk.xmis.workflow.model.WorkItemHistory;
import com.bitc.jk.xmis.workflow.model.WorkflowApprovalInfo;
import com.bitc.jk.xmis.workflow.model.ProcessWithKeyword;
import com.bitc.jk.xmis.workflow.service.WorkflowService;
import com.bitc.jk.xmis.workflow.util.WorkflowUtil;

import org.apache.commons.lang.ArrayUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

public class MissAttendanceServiceImpl implements MissAttendanceService {
	private static Logger logger = Logger
			.getLogger(MissAttendanceServiceImpl.class);
	public MissAttendanceDAO missAttendanceDAO;
	public OrganizationDAO organizationDAO;
	private RuntimeContext workflowContext;
	private WorkflowDAO workflowDAO;
	private com.bitc.jk.xmis.task.TaskDAO taskDAO;
	private TodoDAO todoDAO;
	private WfCalendarDAO wfCalendarDAO;
	private OrganizationService organizationService;
	private EmailAndPlatformMessageInfoSender emailAndPlatformMessageInfoSender;

	private WorkflowService workflowService;

	public WorkflowService getWorkflowService() {
		return workflowService;
	}

	public void setWorkflowService(WorkflowService workflowService) {
		this.workflowService = workflowService;
	}

	public EmailAndPlatformMessageInfoSender getEmailAndPlatformMessageInfoSender() {
		return emailAndPlatformMessageInfoSender;
	}

	public void setEmailAndPlatformMessageInfoSender(
			EmailAndPlatformMessageInfoSender emailAndPlatformMessageInfoSender) {
		this.emailAndPlatformMessageInfoSender = emailAndPlatformMessageInfoSender;
	}

	public OrganizationService getOrganizationService() {
		return organizationService;
	}

	public void setOrganizationService(OrganizationService organizationService) {
		this.organizationService = organizationService;
	}

	public WfCalendarDAO getWfCalendarDAO() {
		return wfCalendarDAO;
	}

	public void setWfCalendarDAO(WfCalendarDAO wfCalendarDAO) {
		this.wfCalendarDAO = wfCalendarDAO;
	}

	public MissAttendanceDAO getMissAttendanceDAO() {
		return missAttendanceDAO;
	}

	public com.bitc.jk.xmis.task.TaskDAO getTaskDAO() {
		return taskDAO;
	}

	public void setTaskDAO(com.bitc.jk.xmis.task.TaskDAO taskDAO) {
		this.taskDAO = taskDAO;
	}

	public void setMissAttendanceDAO(MissAttendanceDAO missAttendanceDAO) {
		this.missAttendanceDAO = missAttendanceDAO;
	}

	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}

	public void setOrganizationDAO(OrganizationDAO organizationDAO) {
		this.organizationDAO = organizationDAO;
	}

	public RuntimeContext getWorkflowContext() {
		return workflowContext;
	}

	public void setWorkflowContext(RuntimeContext workflowContext) {
		this.workflowContext = workflowContext;
	}

	public WorkflowDAO getWorkflowDAO() {
		return workflowDAO;
	}

	public void setWorkflowDAO(WorkflowDAO workflowDAO) {
		this.workflowDAO = workflowDAO;
	}

	public TodoDAO getTodoDAO() {
		return todoDAO;
	}

	public void setTodoDAO(TodoDAO todoDAO) {
		this.todoDAO = todoDAO;
	}

	public void doUpdateProcessName(IProcessInstance processInstance,
			String appendName) {
		try {
			String processInstanceId = processInstance.getId();
			ProcessWithKeyword processWithKeyword = new ProcessWithKeyword();
			processWithKeyword.setProcessInstanceId(processInstanceId);
			processWithKeyword.setKeyword(processInstance.getDisplayName()
					+ " " + appendName);
			workflowDAO.doCreateProcessWithKeyword(processWithKeyword);
			int userID = WebUtil.getCurrentEmployee().getUserID();
			workflowDAO.doUpdateWorkHistoryByProcessInstanceId(
					processInstanceId, processInstance.getDisplayName() + "||"
							+ appendName, userID);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public String doStartMissAttence(MissAttendanceObj missAttendanceObj,
			String memberJson) throws WorkflowException, JsonParseException,
			JsonMappingException, IOException, EngineException, KernelException {
		EmployeeObj agent = WebUtil.getCurrentEmployee();// 操作人
		SimpleEmployee applicant = organizationDAO
				.getSimpleEmployee(missAttendanceObj.getApplicant().getUserID());
		missAttendanceObj.setApplicant(applicant);
		missAttendanceObj.setAgent(agent);

		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		// 流程启动
		IProcessInstance processInstance = WorkflowUtil.startProcess(
				workflowSession, WorkflowConstants.WF_MISS_ATTENDANCE,
				Type.getString(agent.getUserID()));

		missAttendanceObj.setProcessInstanceId(processInstance.getId());
		missAttendanceDAO.doUpdateMissAttendanceObj(missAttendanceObj);

		// 表单信息保存
		this.doSaveMissAttenceInfo(missAttendanceObj, memberJson);

		int agentId = agent.getUserID();
		int divisionID = Type.GetInt(this.getDivisionIDByEmpID(agentId));// 根据人员ID
																			// 得到此人的事业部
		int deptID = agent.getDepartmentID();

		// List<MissAttendanceInfos> members = JacksonUtil.readValue(memberJson,
		// new TypeReference<List<MissAttendanceInfos>>() {
		// });
		// // 流程的名字+补报的具体日期
		// String mdStr = "";
		// Date md;
		// for (int i = 0; i < members.size(); i++) {
		// md = members.get(i).getMissDate();
		// mdStr += DateUtil.formatDate(md) + "/";
		// }
		// mdStr = mdStr.substring(0, mdStr.length() - 1);
		int roleId = -1;
		Boolean isDirect = true;
		if (organizationDAO.getIsExistByDeptId(deptID)) {
			isDirect = organizationDAO.getIsDirectByDeptId(deptID);// 是否直属
		} else {
			isDirect = false;
			roleId = -2;
		}
		// 2013、12、26 判断代理人是佛是事业部领导
		Boolean isDirectorLeader = organizationDAO.getIsExistRole(
				WebKeys.DIVISION_LEADER_ROLE, applicant.getUserID());// 判断此人是否是事业部领导

		WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
				true);// 第一次提交 审批意见默认是 通过
		// 流程参数
		WorkflowUtil.addProcessInstanceVariable(processInstance, "__agent",
				agentId);// 通知申请人或代理人
		WorkflowUtil.addProcessInstanceVariable(processInstance, "isDirect",
				isDirect);// 条件
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"isDirectorLeader", isDirectorLeader);// 条件
		WorkflowUtil.addProcessInstanceVariable(processInstance, "roleId",
				roleId);// 用于审批人表达式
		WorkflowUtil.addProcessInstanceVariable(processInstance, "deptID",
				deptID);// 用于审批人表达式
		WorkflowUtil.addProcessInstanceVariable(processInstance, "divisionID",
				divisionID);// 用于审批人表达式
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"applicantname", applicant.getUserName());// 用于__title
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"applicantdate", Type.getString(DateUtil.getCurrentDate()));// 用于__title

		boolean isSystemTask = !isInDuration();
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"isSystemTask", isSystemTask);

		WorkflowUtil.addProcessInstanceVariable(processInstance, "applicantID",
				applicant.getUserID());// 用于被退回的链接
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"isOfficialTravel", missAttendanceObj.getIsOfficialTravel());// 用于被退回的链接
		WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgent",
				missAttendanceObj.getIsAgent());// 用于被退回的链接
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"missAttendanceId", missAttendanceObj.getId());// 用于被退回的链接
		// 流程关键字
		String appendName = applicant.getUserName()
				+ Type.getString(DateUtil.getCurrentDate()) + "补报考勤申请";
		WorkflowUtil.completeFirstTask(workflowSession, processInstance,
				Type.getString(agentId), "");
		this.doUpdateProcessName(processInstance, appendName);
		return processInstance.getId();
	}

	public void doSaveMissAttenceInfo(MissAttendanceObj missAttendanceObj,
			String memberJson) throws JsonParseException, JsonMappingException,
			IOException {
		List<MissAttendanceInfos> members = JacksonUtil.readValue(memberJson,
				new TypeReference<List<MissAttendanceInfos>>() {
				});
		String missAttendanceId = missAttendanceDAO
				.doCreateMissAttendanceObj(missAttendanceObj);
		for (MissAttendanceInfos infos : members) {
			infos.setMissAttendanceId(missAttendanceId);
			missAttendanceDAO.doCreateMissAttendanceInfos(infos);
		}
	}

	/**
	 * 根据人员ID 得到他所在的事业部ID
	 * 
	 * @param empID
	 * @return
	 */
	public String getDivisionIDByEmpID(int empID) {
		String pathId = organizationDAO.getEmpPathId(empID); // 根据人员id得到 此人的路径
																// 人ID-部门ID-上级部门ID-...-上级部门ID-部门ID-公司ID
		if (pathId.indexOf(",") > -1) {
			pathId = pathId.substring(0, pathId.lastIndexOf(","));
		}
		return pathId.substring(pathId.lastIndexOf(",") + 1, pathId.length());
	}

	// 求前后的几天
	@SuppressWarnings("unused")
	private static Calendar addDate(Calendar c, int day) {
		c.add(Calendar.DATE, day);
		return c;
	}

	public String getMissAttendanceList(StandardQuery query, PageInfo pageInfo,
			String todoIdsQuery) throws Exception {
		@SuppressWarnings("rawtypes")
		List list = missAttendanceDAO.getMissAttendanceList(query, pageInfo,
				todoIdsQuery);

		return WebUtil.buildStoreForJson(list, "M");
	}

	public void doCreateTimerTask(String needId, String commandBeanName) {
		SimpleEmployee emp = WebUtil.getCurrentEmployee();

		Task task = new Task();// 任务队列
		task.setCommandBeanName(commandBeanName);
		task.setParamString(needId);
		task.setExecuteTime(this.getExecuteTime());
		task.setState(0);// 未完成0 已完成1
		task.setCreator(emp);
		task.setCreateDateTime(DateUtil.getCurrentDateTime());
		taskDAO.addTask(task);
	}

	public void doCreateTimerTaskSystem(String workItemId,
			String commandBeanName) {
		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
				workflowSession, workItemId);
		String needId = workItemId + "," + processInstance.getId();
		Task task = new Task();// 任务队列
		task.setCommandBeanName(commandBeanName);
		task.setParamString(needId);
		task.setExecuteTime(this.getExecuteTime());
		task.setState(0);// 未完成0 已完成1
		task.setCreator(new SimpleEmployee(-10000));
		task.setCreateDateTime(DateUtil.getCurrentDateTime());
		task.setTaskDesc("参数描述workItemId,processInstanceId");
		taskDAO.addTask(task);
	}

	public int getLastDay() {
		String lastday = missAttendanceDAO
				.getValueFromParamTableByName(WebKeys.P_LASTDAY);
		return Type.GetInt(lastday);
	}

	public int getDurationNum() {
		String num = missAttendanceDAO
				.getValueFromParamTableByName(WebKeys.P_DURATION);
		return Type.GetInt(num);
	}

	/**
	 * 根据截止日期判断是否是截止日当天-未启动
	 * 
	 * @param lastday
	 * @return
	 */
	public Boolean isEqualLastday(int lastday) {
		Date currentDate = new Date();
		String nowday = DateUtil.getDay(currentDate);
		boolean result = false;
		if (Type.GetInt(nowday) == lastday) {
			result = true;
		}
		return result;
	}

	/**
	 * 判断是否在截止日期前规定的期限内
	 * 
	 * @return
	 */
	public Boolean isInDuration() {

		int num = this.getLastDay();
		String nownum = "";
		if (num < 10) {
			nownum = "0" + num;
		} else {
			nownum = "" + num;
		}
		String lastDate = Type.getString(DateUtil.getCurrentDate()).substring(
				0, 8)
				+ nownum;

		String durationStrs = getSomeWfCalendarBeforeDate(
				this.getDurationNum(), lastDate);
		String[] arrays = durationStrs.split(",");
		String nowDate = DateUtil.formatDate(DateUtil.getCurrentDate(),
				"yyyyMMdd");
		boolean result = false;
		result = ArrayUtils.contains(arrays, nowDate);
		return result;
	}

	/**
	 * 相对应申请时间的截止日期
	 */
	public Date getLastDate() {
		int lastday = this.getLastDay();
		Date nowDate = DateUtil.getCurrentDate();
		Calendar c = Calendar.getInstance();
		c.set(Calendar.DAY_OF_MONTH, lastday);
		Date lastDate = new Date(c.getTimeInMillis());
		if (lastDate.before(nowDate)) {
			c.add(Calendar.MONTH, 1);
		}
		lastDate = new Date(c.getTimeInMillis());
		return lastDate;
	}

	/**
	 * 得到生成统计表的执行时间
	 */
	public Date getExecuteTime() {
		int duration = this.getDurationNum();
		String lastDate = DateUtil.formatDate(this.getLastDate());

		String dateStrs = this.getSomeWfCalendarBeforeDate(duration, lastDate);
		String[] arrays = dateStrs.split(",");
		String dateStr = arrays[arrays.length - 1];
		String y = dateStr.substring(0, 4);
		String m = dateStr.substring(4, 6);
		String d = dateStr.substring(6);
		Date result = DateUtil.getCurrentDate();
		try {
			result = DateUtil.parseDate(y + "-" + m + "-" + d, "yyyy-MM-dd");
			result = DateUtil.clearHMS(result);
		} catch (ParseException e) {
			e.printStackTrace();
		}
		return result;
	}

	/**
	 * 得到截止日期的前四天的一天
	 */
	public Date getExecuteTime4() {
		int duration = this.getDurationNum();
		int lastday = this.getLastDay();

		Date nowDate = DateUtil.getCurrentDate();
		Calendar c = Calendar.getInstance();
		c.set(Calendar.DAY_OF_MONTH, lastday);
		c.add(Calendar.DAY_OF_MONTH, -(duration));
		Date executeDate = new Date(c.getTimeInMillis());
		if (executeDate.before(nowDate)) {
			c.add(Calendar.MONTH, 1);
		}
		Date result = DateUtil.clearHMS(executeDate);
		return result;
	}

	// 系统任务队列用
	public void doTimerTask(String workItemId) throws WorkflowException {
		// 为任务队列表增加数据
		this.doCreateTimerTaskSystem(workItemId, WebKeys.MISSATTENDANCE_TASK);
	}

	@SuppressWarnings("rawtypes")
	public String getMissAttendanceListByEmpId(String isOfficialTravel,
			String todoModelList, String wokItemId, String isTodo,
			PageInfo pageInfo) throws Exception {
		List list = missAttendanceDAO.getMissAttendanceListByEmpId(
				isOfficialTravel, todoModelList, wokItemId, isTodo, pageInfo);
		if (list == null || list.size() == 0) {
			return "";
		}
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, todoModelList);
	}

	/**
	 * 审批单条
	 * 
	 * @param workItemId
	 * @param isAgree
	 * @param comment
	 * @throws WorkflowException
	 */
	public String doApprovalTask(String workItemId, String isAgree,
			String comment) throws WorkflowException {

		SimpleEmployee emp = WebUtil.getCurrentEmployee();
		WorkflowApprovalInfo approvalInfo = new WorkflowApprovalInfo();
		approvalInfo.setApprover(emp);
		approvalInfo.setApprovedTime(DateUtil.getCurrentDateTime());
		approvalInfo.setIsApproved(Type.GetInt(isAgree));
		approvalInfo.setAdvice(comment);
		approvalInfo.setWorkItemID(workItemId);
		workflowDAO.doSaveWorkflowApprovalInfo(approvalInfo);

		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
				workflowSession, workItemId);

		boolean isSystemTask = !isInDuration();
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"isSystemTask", isSystemTask);

		WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
				"0".equals(isAgree) ? true : false);
		WorkflowUtil.completeTask(workflowSession, workItemId, Type
				.getString(emp.getUserID()),
				"0".equals(isAgree) ? ("[审批通过] " + comment)
						: ("[审批未通过] " + comment));
		// WorkflowUtil.jumpToTask(workflowSession, workItemId,
		// Type.getString(emp.getUserID()),
		// "0".equals(isAgree)?("[审批通过] "+comment):("[审批未通过] "+comment));

		// 任务人不是当前用户的时候，设置代理人2014-01-07
		WorkItem wi = workflowDAO.load(WorkItem.class, workItemId);
		logger.info("-----------doApprovalTask-----------" + wi.getState());
		WorkItemHistory history = workflowDAO.load(WorkItemHistory.class,
				wi.getId());
		if (!Type.getString(emp.getUserID()).equals(wi.getActorId())) {
			history.setAgent(new SimpleEmployee(emp.getUserID()));
		} else {
			history.setAgent(null);
		}
		workflowDAO.doUpdateWorkItemHistory(history);

		return processInstance.getId();
	}

	/**
	 * 审批多条
	 * 
	 */
	public String doApprovalTaskList(String workItemIds, String isAgree,
			String comment) throws WorkflowException {
		// 自动签收
		workflowService.doClaimTaskList(workItemIds,
				Type.getString(WebUtil.getCurrentEmployee().getUserID()));

		String infoMsg = "";
		if (workItemIds.indexOf(",") > -1) {
			String[] array = workItemIds.split(",");
			array = TemplateUtil.getUniqueList(array);// 去除重复的数据
			for (int i = 0; i < array.length; i++) {
				doApprovalTask(array[i], isAgree, comment);
			}
		} else {
			infoMsg = doApprovalTask(workItemIds, isAgree, comment);
		}
		return infoMsg;
	}

	/**
	 * 删除统计待办
	 * 
	 * @param workItemId
	 */
	public void deleteTodoCountByTodoId(String deleteTodo) {
		todoDAO.deleteTodo(deleteTodo);// 删除统计待办
	}

	@SuppressWarnings("rawtypes")
	public String getMissAttendanceListByQuery(StandardQuery query,
			PageInfo pageInfo, String dateFirst, String dateLast) {

		List list = missAttendanceDAO.getMissAttendanceListByQuery(query,
				pageInfo, dateFirst, dateLast);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	public void doWarnInfo() {
		String msg = WebKeys.MSG;
		String lastDate = DateUtil.formatDate(this.getLastDate());
		String warndate = getSomeWfCalendarBeforeDate(2, lastDate);
		String w1 = warndate.split(",")[0];
		String w2 = warndate.split(",")[1];
		String nowdate = DateUtil.formatDate(DateUtil.getCurrentDate(),
				"yyyyMMdd");
		try {
			if (nowdate.equals(w1) || nowdate.equals(w2)) {
				boolean isHasExist = organizationDAO
						.isHasExistAttendanceWarnInfo(msg, nowdate);
				if (!isHasExist) {
					String message_id = UUIDUtils.nextCode();
					organizationDAO.doInsertMessageDefinition(message_id, -1,
							msg, "home");
					organizationDAO.doInsertMessageRecord(message_id);

					List<String> receiverList_p = missAttendanceDAO
							.getAllMyCompEmailList();

					String receverAddress = "";
					if (receiverList_p != null && receiverList_p.size() > 0) {
						receverAddress = receiverList_p.get(0);
					}

					EmailMessageInfo messageObj = new EmailMessageInfo();
					messageObj.setTitle(msg);
					messageObj.setContent("xMis中有一条新的消息： " + msg
							+ " http://xmis.bitc.net.cn/");
					messageObj.setReceiverAddress(receverAddress);
					// messageObj.setReceiverList(receiverList_p);
					try {
						emailAndPlatformMessageInfoSender
								.doSendMessage(messageObj);
					} catch (Exception e) {
						logger.info("MissAttendanceServiceImpl//doWarnInfo----//"
								+ e);
					}
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 某一个日期前的指定个数工作日
	 */
	public String getSomeWfCalendarBeforeDate(int num, String onedate) {
		List<WfCalendar> list = wfCalendarDAO.getWfCalendarBeforeDate(onedate);
		String result = "";
		for (int i = 0; i < num; i++) {
			result += list.get(i).getDate() + ",";
		}
		return result;

	}

	@SuppressWarnings("rawtypes")
	public String getMissAttendanceInfos(String workItemId) {
		List list = missAttendanceDAO.getMissAttendanceInfos(workItemId);
		if (list == null || list.size() == 0) {
			return null;
		}
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "M");
	}

	public String doUpdateMissAttendanceInfo(MissAttendanceInfos info) {
		missAttendanceDAO.doUpdateMissAttendanceInfos(info);
		return info.getId();
	}

	public void doDeleteMissAttendanceInfo(String infoId) {
		MissAttendanceInfos info = missAttendanceDAO
				.getMissAttendanceInfoById(infoId);
		missAttendanceDAO.doDeleteMissAttendanceInfos(info);
	}

	public MissAttendanceInfos getMissAttendanceInfoById(String infoId) {
		return missAttendanceDAO.getMissAttendanceInfoById(infoId);
	}

	@SuppressWarnings("unused")
	public String doGoOnSubmitMissAttence(String missAttendanceId,
			String workItemId) throws WorkflowException {
		MissAttendanceObj missAttendanceObj = missAttendanceDAO
				.getMissAttendanceObjById(missAttendanceId);

		missAttendanceDAO.doUpdateMissAttendanceObj(missAttendanceObj);

		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();

		IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
				workflowSession, workItemId);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
				true);// 再次提交被默认同意
		IWorkItem wi = workflowSession.findWorkItemById(workItemId);
		// 自动签收
		workflowService.doClaimTaskList(workItemId,
				Type.getString(WebUtil.getCurrentEmployee().getUserID()));
		WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
				.getCurrentEmployee().getUserID() + "", "");
		return processInstance.getId();

	}

	public void doEndMissAttence(String workItemId, String missAttendanceId)
			throws WorkflowException, EngineException {

		// MissAttendanceObj missAttendanceObj = missAttendanceDAO
		// .getMissAttendanceObjById(missAttendanceId);
		// missAttendanceDAO.doDeleteMissAttendanceObj(missAttendanceObj);

		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
				workflowSession, workItemId);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "forceStop",
				true);// 用于被退回的链接
		// 自动签收
		workflowService.doClaimTaskList(workItemId,
				Type.getString(WebUtil.getCurrentEmployee().getUserID()));
		WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
				.getCurrentEmployee().getUserID() + "", "");

	}

	public String doSaveMissAttence(String missAttendanceId,
			MissAttendanceObj missAttendanceObj, MissAttendanceInfos info) {

		if (StringUtils.isBlank(missAttendanceId)) {
			missAttendanceDAO.doUpdateMissAttendanceObj(missAttendanceObj);
			return missAttendanceObj.getId();
		} else {
			info.setMissAttendanceId(missAttendanceId);
			missAttendanceDAO.doUpdateMissAttendanceInfos(info);
			return missAttendanceId;
		}
	}

	public void doUpdateMissAttendanceRecordState(String infoId, int year,
			int month, int state) {
		boolean isExist = missAttendanceDAO.isExistMissAttendanceRecord(infoId,
				year, month);
		if (isExist) {
			missAttendanceDAO.doUpdateMissAttendanceRecordState(infoId, year,
					month, state);
		} else {
			missAttendanceDAO.doCreateMissAttendanceRecord(infoId, year, month,
					1);
		}

	}

	@SuppressWarnings("rawtypes")
	public String getApprovalTaskListByQuery(StandardQuery query,
			PageInfo pageInfo) {
		List list = missAttendanceDAO.getApprovalTaskListByQuery(query,
				pageInfo);
		if (list.size() <= 1) {
			list.clear();
		}
		return WebUtil.buildStoreForJson(list, "miss");
	}

	// public void doResignTask(String workItemId) throws WorkflowException {
	// SimpleEmployee emp = WebUtil.getCurrentEmployee();
	// IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
	//
	// WorkflowUtil.reasignTo(workflowSession, workItemId,
	// Type.getString(emp.getUserID()), "");
	// }
	//

	/**
	 * 多条
	 * 
	 * @throws WorkflowException
	 * 
	 */
	// public void doResignTasks(String workItemIds) throws WorkflowException {
	// if (workItemIds.indexOf(",") > -1) {
	// String[] array = workItemIds.split(",");
	// array = TemplateUtil.getUniqueList(array);
	// for (int i = 0; i < array.length; i++) {
	// doResignTask(array[i]);
	// }
	// }
	// }
	public MissAttendanceObj getMissAttendanceObj(String processInstanceId) {
		List<MissAttendanceObj> list = missAttendanceDAO
				.getMissAttendanceId(processInstanceId);
		if (list != null && list.size() > 0) {
			return list.get(0);
		}
		return null;
	}
}
