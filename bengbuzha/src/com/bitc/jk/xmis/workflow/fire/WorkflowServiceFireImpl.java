package com.bitc.jk.xmis.workflow.fire;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.IWorkItem;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.RuntimeContext;
import org.fireflow.engine.definition.WorkflowDefinition;
import org.fireflow.engine.impl.TaskInstance;
import org.fireflow.engine.impl.WorkItem;
import org.fireflow.engine.persistence.IPersistenceService;
import org.fireflow.kernel.KernelException;
import org.wltea.expression.ExpressionEvaluator;

import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.service.impl.MissAttendanceServiceImpl;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.ExpressionFunctions;
import com.bitc.jk.xmis.util.JSONUtils;
import com.bitc.jk.xmis.util.TemplateUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.SpringInit;
import com.bitc.jk.xmis.workflow.dao.TodoDAO;
import com.bitc.jk.xmis.workflow.dao.WfCalendarDAO;
import com.bitc.jk.xmis.workflow.dao.WorkflowDAO;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;
import com.bitc.jk.xmis.workflow.model.AuthorizeInfo;
import com.bitc.jk.xmis.workflow.model.Todo;
import com.bitc.jk.xmis.workflow.model.Week;
import com.bitc.jk.xmis.workflow.model.WfCalendar;
import com.bitc.jk.xmis.workflow.model.WorkItemHistory;
import com.bitc.jk.xmis.workflow.service.WorkflowService;
import com.bitc.jk.xmis.workflow.util.Constants;
import com.bitc.jk.xmis.workflow.util.WorkflowUtil;
import com.bitc.jk.xmis.workflow.web.WorkflowAction;

public class WorkflowServiceFireImpl implements WorkflowService {

	private static final Logger logger = Logger.getLogger(WorkflowAction.class);

	private RuntimeContext runtimeContext;

	private WfCalendarDAO wfCalendarDAO;

	private TodoDAO todoDAO;

	private WorkflowDAO workflowDAO;

	private OrganizationDAO organizationDAO;

	public void doClaimTask(String workItemId, String userId)
			throws WorkflowException {
		IWorkflowSession wflsession = runtimeContext.getWorkflowSession();
		IWorkItem wi = wflsession.findWorkItemById(workItemId);
		if (wi == null) {
			throw new WorkflowException("签收任务出错，不存在该任务（" + workItemId + "）");
		}
		// 对于已经签收未完成任务的处理：20140117
		if (wi.getState() == IWorkItem.INITIALIZED) {
			// if (!userId.equals(wi.getActorId())) {
			// throw new WorkflowException("签收任务出错，该任务（" + workItemId
			// + "）的任务人不是当前用户");
			// }
			try {
				Todo todo = todoDAO.findTodoByAppId(Constants.TODO_MODEL_ID,
						workItemId);
				Todo todoList = todoDAO.findTodoByAppId(
						Constants.TODO_MODEL_LISTS, workItemId);
				// 多人任务时，删除其他任务人的待办，必须在WorkItem.claim()之前调用
				if ("ANY".equals(wi.getTaskInstance().getAssignmentStrategy())) {
					List<WorkItem> workitems = workflowDAO
							.getWorkItemsInInitializedState(wi
									.getTaskInstance().getId());
					for (WorkItem item : workitems) {
						if (!workItemId.equals(item.getId())) {
							todoDAO.deleteTodoByAppId(Constants.TODO_MODEL_ID,
									item.getId());
							todoDAO.deleteTodoByAppId(
									Constants.TODO_MODEL_LISTS, item.getId());
							workflowDAO.deleteObject(new WorkItemHistory(item
									.getId()));
						}
					}
				}
				wi.claim();// 签收任务
				if (todo != null) {
					todo.setCreateTime(DateUtil.getCurrentDateTime());
					todo.setState(Todo.STATE_RUNNING);
					todo.setUrl("/workflow.do?method=openForm&workItemId="
							+ wi.getId());
					todo.setScript("doTask");
				}
				if (todoList != null) {
					todoList.setCreateTime(DateUtil.getCurrentDateTime());
					todoList.setState(Todo.STATE_RUNNING);
					todoList.setScript("doTaskLists");
				}
				WorkItemHistory history = workflowDAO.load(
						WorkItemHistory.class, wi.getId());
				history.setClaimedTime(wi.getClaimedTime());
				history.setState(wi.getState());
				history.setComments(wi.getComments());
				workflowDAO.doUpdateWorkItemHistory(history);
				if (logger.isDebugEnabled()) {
					logger.debug("签收任务项成功//" + history.toString());
				}
			} catch (EngineException e) {
				throw new WorkflowException(e);
			} catch (KernelException e) {
				throw new WorkflowException(e);
			}

		}
	}

	/**
	 * 批量签收
	 * 
	 * @param workItemId
	 * @param userId
	 * @throws WorkflowException
	 */
	public void doClaimTaskList(String workItemId, String userId)
			throws WorkflowException {
		if (workItemId.indexOf(",") > -1) { // 批量签收
			String[] strs = workItemId.split(",");
			strs = TemplateUtil.getUniqueList(strs);// 去除数组中重复数据
			for (int i = 0; i < strs.length; i++) {
				workItemId = strs[i];
				this.doClaimTask(workItemId, userId);
			}// for循环结束标签
		} else {
			doClaimTask(workItemId, userId);
		}

	}

	public void doRejectTask(String workItemId, String userId, String comments)
			throws WorkflowException {
		IWorkflowSession wflsession = runtimeContext.getWorkflowSession();
		IWorkItem wi = wflsession.findWorkItemById(workItemId);
		if (wi == null) {
			throw new WorkflowException("拒收任务出错，不存在该任务（" + workItemId + "）");
		}

		// if (!userId.equals(wi.getActorId())) {
		// throw new WorkflowException("拒收任务出错，该任务（" + workItemId
		// + "）的任务人不是当前用户");
		// }
		try {
			wi.setComments(comments);
			if ("ANY".equals(wi.getTaskInstance().getAssignmentStrategy())) {
				List<WorkItem> workitems = workflowDAO
						.getWorkItemsInInitializedState(wi.getTaskInstance()
								.getId());
				for (WorkItem item : workitems) {
					todoDAO.deleteTodoByAppId(Constants.TODO_MODEL_ID,
							item.getId());
				}
			}
			wi.reject();
			WorkItemHistory history = workflowDAO.load(WorkItemHistory.class,
					wi.getId());
			history.setEndTime(wi.getEndTime());
			history.setComments(wi.getComments());
			history.setState(wi.getState());
			workflowDAO.doUpdateWorkItemHistory(history);
		} catch (Exception e) {
			throw new WorkflowException(e);
		}

	}

	/**
	 * 取回 未使用
	 * 
	 */
	public void doWithdrawTask(String workItemId, String userId, String comments)
			throws WorkflowException {
		IWorkflowSession wflsession = runtimeContext.getWorkflowSession();
		IWorkItem wi = wflsession.findWorkItemById(workItemId);
		if (wi == null) {
			throw new WorkflowException("取回任务出错，不存在该任务（" + workItemId + "）");
		}

		IPersistenceService persistenceService = this.runtimeContext
				.getPersistenceService();
		List<IWorkItem> workItems = persistenceService.findTodoWorkItems(wi
				.getActorId(), wi.getTaskInstance().getProcessId(), wi
				.getTaskInstance().getTaskId());
		for (int i = 0; i < workItems.size(); i++) {
			System.out.println("------more work item-------"
					+ workItems.get(i).getId());
		}

		try {
			wi.withdraw();
			wi.setComments(comments);
			IWorkItem new_wi = wi.withdraw(); // 取回创建一个新的

			WorkItemHistory history = workflowDAO.load(WorkItemHistory.class,
					wi.getId());
			history.setComments(wi.getComments());// 注释 申请取回

			// 新任务 在待办和历史里修改状态
			System.out.println(new_wi.getId() + "***********");
			Todo newTodo = todoDAO.findTodoByAppId(Constants.TODO_MODEL_ID,
					new_wi.getId());
			if (newTodo != null) {
				newTodo.setCreateTime(DateUtil.getCurrentDateTime());
				newTodo.setState(Todo.STATE_RUNNING);
				newTodo.setUrl("/workflow.do?method=openForm&workItemId="
						+ new_wi.getId());
				newTodo.setScript("doTask");
			}
			WorkItemHistory newHistory = workflowDAO.load(
					WorkItemHistory.class, new_wi.getId());
			newHistory.setClaimedTime(new_wi.getClaimedTime());
			newHistory.setState(new_wi.getState());
			newHistory.setComments(new_wi.getComments());
		} catch (Exception e) {
			throw new WorkflowException(e);
		}
	}

	public RuntimeContext getRuntimeContext() {
		return runtimeContext;
	}

	public void setRuntimeContext(RuntimeContext runtimeContext) {
		this.runtimeContext = runtimeContext;
	}

	public String doGetWfCalendarStore(int year, int month) {
		List<WfCalendar> list = wfCalendarDAO.getWfCalendar(year, month);
		List<Week> weeks = new ArrayList<Week>();
		Week week = new Week();
		int j = 1;
		for (int i = 0; i < list.size(); i++) {
			WfCalendar wfCalendar = list.get(i);
			if (wfCalendar.getDayOfWeek() == Calendar.SUNDAY) {
				weeks.add(week);
				week = new Week();
				j++;
			}
			week.setDay(wfCalendar);
			week.setId(j);
		}
		weeks.add(week);
		return WebUtil.buildJsonStoreByList(weeks);
	}

	public WfCalendarDAO getWfCalendarDAO() {
		return wfCalendarDAO;
	}

	public void setWfCalendarDAO(WfCalendarDAO wfCalendarDAO) {
		this.wfCalendarDAO = wfCalendarDAO;
	}

	public void doSaveHoliday(String years, String months, String[] holiday) {
		int year = Integer.parseInt(years);
		int month = Integer.parseInt(months);
		List<String> holidays = Arrays.asList(holiday);
		List<WfCalendar> list = wfCalendarDAO.getWfCalendar(year, month);
		for (int i = 0; i < list.size(); i++) {
			WfCalendar calendar = list.get(i);
			if (holidays.contains(calendar.getDate())) {
				calendar.setType(WfCalendar.TYPE_WEEKEDNS);
			} else {
				calendar.setType(WfCalendar.TYPE_WEEKDAYS);
			}
		}
	}

	public String getTodoListByUserID(int userID) {
		List<Todo> list = todoDAO.findTodoByUserId(String.valueOf(userID));
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getTodoListByUserID(int userID, int size) {
		List<Todo> list = todoDAO.findTodoByUserId(String.valueOf(userID), 0,
				size);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getTodoInfo(String todoId) {
		Todo todo = todoDAO.findTodoById(todoId);
		return JSONUtils.toJSON(todo, true).toString();
	}

	public TodoDAO getTodoDAO() {
		return todoDAO;
	}

	public void setTodoDAO(TodoDAO todoDAO) {
		this.todoDAO = todoDAO;
	}

	public String getFormUri(String workItemId, String userId)
			throws WorkflowException {
		IWorkflowSession wflsession = runtimeContext.getWorkflowSession();
		IWorkItem wi = wflsession.findWorkItemById(workItemId);
		if (wi == null) {
			throw new WorkflowException("不存在该任务（" + workItemId + "），请刷新待办列表");
		}
		// 自动签收
		if (!(wi.getState() == IWorkItem.INITIALIZED || wi.getState() == IWorkItem.RUNNING)) {
			// if (wi.getState() != IWorkItem.RUNNING) {
			String msg = "";
			switch (wi.getState()) {
			case IWorkItem.COMPLETED:
				msg = "已完成";
				break;
			case IWorkItem.CANCELED:
				msg = "已取消";
				break;
			default:
				break;
			}
			throw new WorkflowException("任务（" + workItemId + "）状态出错，该任务" + msg);
		}
		// if (!userId.equals(wi.getActorId())) {
		// throw new WorkflowException("任务（" + workItemId + "）的任务人不是当前用户");
		// }
		try {
			return WorkflowUtil.getDefaultFormUri(wi, ((TaskInstance) wi
					.getTaskInstance()).getAliveProcessInstance());
		} catch (Exception e) {
			throw new WorkflowException(e);
		}
	}

	public String getTodoListByQuery(StandardQuery query, PageInfo pageInfo) {
		List<Todo> list = todoDAO.findTodoByQuery(query, pageInfo);
		int count = workflowDAO.getTotalCount(query, Todo.class);
		return WebUtil.buildJsonStoreByList(list, count);
	}

	public String getWorkItemHistoryListStore(StandardQuery query,
			PageInfo pageInfo) throws Exception {
		List<WorkItemHistory> listStore = workflowDAO.getListStore(query,
				pageInfo, WorkItemHistory.class);
		// 如果是未执行的任务队列，修改备注
		for (int i = 0; i < listStore.size(); i++) {
			boolean isWait = workflowDAO.isWaitByWorkItemId(listStore.get(i)
					.getId());
			if (isWait) {
				String executeDateStr = workflowDAO
						.getContentByWorkItemId(listStore.get(i).getId());
				WorkItemHistory history = listStore.get(i);
				history.setComments(executeDateStr);
			}

		}

		int count = workflowDAO.getTotalCount(query, WorkItemHistory.class);
		return WebUtil.buildJsonStoreByList(listStore, count);
	}

	public String getHaveDoneHistoryListStore(StandardQuery query,
			PageInfo pageInfo) throws Exception {
		List listStore = workflowDAO.getHaveDoneHistoryListStore(query,
				pageInfo);

		return WebUtil.buildStoreForJson(listStore, "");
	}

	public String getProcessDefinitionXML(String processId,
			Integer versionNumber) {
		if (versionNumber == null) {
			return runtimeContext.getDefinitionService()
					.getTheLatestVersionOfWorkflowDefinition(processId)
					.getProcessContent();
		} else {
			return runtimeContext
					.getDefinitionService()
					.getWorkflowDefinitionByProcessIdAndVersionNumber(
							processId, versionNumber).getProcessContent();
		}
	}

	public String getProcessDefinitionXMLByInstanceId(String processInstanceId) {
		IProcessInstance processInstance = runtimeContext
				.getPersistenceService().findProcessInstanceById(
						processInstanceId);
		return getProcessDefinitionXML(processInstance.getProcessId(),
				processInstance.getVersion());
	}

	public String getHistoryTrace(String processInstanceId) {
		@SuppressWarnings("rawtypes")
		List list = runtimeContext.getPersistenceService()
				.findProcessInstanceTraces(processInstanceId);
		return WebUtil.buildJsonStoreByList(list);
	}

	@SuppressWarnings("unchecked")
	public List<SimpleEmployee> getCandidate(String expression) {
		List<SimpleEmployee> list = new ArrayList<SimpleEmployee>();
		List<String> ids = (List<String>) ExpressionEvaluator
				.evaluate(expression);
		for (Iterator<String> it = ids.iterator(); it.hasNext();) {
			list.add(organizationDAO.getSimpleEmployee(Integer.parseInt(it
					.next())));
		}
		return list;

	}

	public void doDeploy(WorkflowDefinition workflowDefinition) {
		IPersistenceService persistenceService = runtimeContext
				.getPersistenceService();
		persistenceService.saveOrUpdateWorkflowDefinition(workflowDefinition);
	}

	public WorkflowDAO getWorkflowDAO() {
		return workflowDAO;
	}

	public void setWorkflowDAO(WorkflowDAO workflowDAO) {
		this.workflowDAO = workflowDAO;
	}

	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}

	public void setOrganizationDAO(OrganizationDAO organizationDAO) {
		this.organizationDAO = organizationDAO;
	}

	public String getDeployedProcessList() {
		List<WorkflowDefinition> list = runtimeContext.getDefinitionService()
				.getAllLatestVersionsOfWorkflowDefinition();
		return WebUtil.buildJsonStoreByList(list);
	}

	/**
	 * 工作流分配策略
	 */
	public String getActorAndAssignmentStrategyList(String processInstanceId) {
		List<Object[]> list = workflowDAO
				.getActorAndAssignmentStrategyList(processInstanceId);
		List<String> strList = new ArrayList<String>();
		for (int i = 0; i < list.size(); i++) {
			strList.add(list.get(i)[0].toString());
		}
		String resultStr = strList.toString();
		int length = resultStr.length();
		if (length > 1) {
			resultStr = resultStr.substring(1, length - 1);
		}
		return resultStr;
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	public String getProcessInstanceList(StandardQuery query, PageInfo pageInfo) {
		List list = workflowDAO.getProcessInstanceList(query, pageInfo);
		if (list.size() <= 1) {
			list.clear();
		}
		for (int i = 0; i < list.size(); i++) {
			Map map = (Map) list.get(i);
			String processInstanceId = (String) map.get("流程实例ID");
			String actorNames = getActorAndAssignmentStrategyList(processInstanceId);
			map.put("接收人", actorNames);
		}

		return WebUtil.buildStoreForJson(list, "W");
	}

	public List getProcessInstanceComb() {
		return workflowDAO.getProcessInstanceComb();
	}

	@SuppressWarnings("rawtypes")
	public String getTaskListByQuery(StandardQuery query, PageInfo pageInfo) {
		List list = workflowDAO.getTaskListByQuery(query, pageInfo);
		if (list.size() <= 1) {
			list.clear();
		}
		return WebUtil.buildStoreForJson(list, "W");
	}

	@SuppressWarnings("rawtypes")
	public Map<String, String> getTileAndTaskIDByUserID(int userID) {
		List list = workflowDAO.getTileAndTaskIDByUserID(userID);
		int num = list.size();
		String[] tasks = null;
		String[] titles = null;
		Map<String, String> maps = new HashMap<String, String>();
		String titleStr = "";
		String taskStr = "";
		if (num > 0) {
			tasks = new String[num];
			titles = new String[num];
			for (int i = 0; i < num; i++) {
				Map map = (Map) list.get(i);
				String taskId = (String) map.get("task_ID");
				String title = (String) map.get("DISPLAY_NAME");
				tasks[i] = taskId;
				titles[i] = title;
			}
			for (int i = 0; i < num; i++) {
				titleStr += titles[i] + "#";
				taskStr += tasks[i] + "#";
			}
			titleStr = titleStr.substring(0, titleStr.length() - 1);
			taskStr = taskStr.substring(0, taskStr.length() - 1);
		}
		maps.put("titles", titleStr);
		maps.put("taskIds", taskStr);
		return maps;
	}

	@SuppressWarnings("rawtypes")
	public String getApprovalInfos(String processinstanceId) {
		List list = workflowDAO.getApprovalInfos(processinstanceId);
		return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
	}

	public String getTodoListSQL(StandardQuery query, PageInfo pageInfo)
			throws Exception {
		logger.info("now in service// getTodoListSQL+------------------------------------------");
		List listStore = workflowDAO.getTodoListSQL(query, pageInfo);

		return WebUtil.buildStoreForJson(listStore, "");
	}
	public String getTodoListSQLByApp(StandardQuery query, PageInfo pageInfo)
	throws Exception {
		logger.info("now in service// getTodoListSQLByApp");
		List listStore = workflowDAO.getTodoListSQLByApp(query, pageInfo);
		listStore.remove(0);
		return WebUtil.buildStoreForJsonWithoutPageInfo(listStore, "");
	}

	public String getAuthorizeList(StandardQuery querys, PageInfo pageInfo)
			throws Exception {
		List list = workflowDAO.getAuthorizeList(querys, pageInfo);
		return WebUtil.buildStoreForJson(list, "");
	}

	public String getCurrentActorName(String processInstanceId) {
		if(processInstanceId==null || "".equals(processInstanceId)){
			return "";
		}
		IProcessInstance processInstance = workflowDAO
				.getProcessInstanceByID(processInstanceId);
		int divisionID = Type.GetInt(processInstance
				.getProcessInstanceVariable("divisionID"));
		// boolean isAgree =
		// Type.getBoolean(processInstance.getProcessInstanceVariable("isAgree"));
		// boolean isSystemTask =
		// Type.getBoolean(processInstance.getProcessInstanceVariable("isSystemTask"));
		// boolean forceStop =
		// Type.getBoolean(processInstance.getProcessInstanceVariable("forceStop"));
		// boolean isDirectorLeader =
		// Type.getBoolean(processInstance.getProcessInstanceVariable("isDirectorLeader"));
		// boolean isDirect =
		// Type.getBoolean(processInstance.getProcessInstanceVariable("isDirect"));

		String currentActorName = workflowDAO
				.getCurrentActorName(processInstanceId);
		// 任务队列的情况
		// if (isAgree && isSystemTask &&
		// (isDirect||isDirectorLeader)&&!forceStop){
		if (currentActorName.equals("任务队列")) {
			ExpressionFunctions expressionFunctions = new ExpressionFunctions();
			List<String> receiverIDs = expressionFunctions
					.getEmpsByRoleAndDivisionID(-1, divisionID);
			if (receiverIDs != null && receiverIDs.size() > 0) {
				String idsStr = "";
				for (int i = 0; i < receiverIDs.size(); i++) {
					int id = Type.GetInt(receiverIDs.get(i));
					if (id != 0) {
						idsStr += organizationDAO.getSimpleEmployee(id)
								.getUserName() + ",";
					}
				}
				if (idsStr.length() > 1) {
					idsStr = idsStr.substring(0, idsStr.length() - 1);
				}

				MissAttendanceServiceImpl service = (MissAttendanceServiceImpl) SpringInit
						.getBean("missAttenceService");
				String dateStr = DateUtil.formatDate(service.getExecuteTime());
				return "预计<font color='green'>" + dateStr
						+ "</font>日到达任务人：<font color='green'>" + idsStr
						+ "</font>";
			} else {
				return "流程结束";
			}

		}
		if (currentActorName == null || currentActorName.equals("")) {
			return "流程结束";
		}
		return "下一步任务人是：<font color='green'>" + currentActorName + "</font>";
	}

	public boolean doCancelAuthorizeById(String id) {
		int n = workflowDAO.doCancelAuthorizeById(id);
		if (n > 0) {
			return true;
		} else {
			return false;
		}
	}

	public boolean doAddAuthorize(AuthorizeInfo info) {
		String id = "";
		if (info.getId() != null && !info.getId().equals("")) {
			id=info.getId();
		}
		boolean isValidate = workflowDAO.doValidate(id,info.getProcessId(),info.getAuthorizer().getUserID(),info.getAuthorizee().getUserID());
		if(!isValidate){
			return false;
		}
		
		if (info.getId() == null || info.getId().equals("")) {
			info.setCreateDate(DateUtil.getCurrentDateTime());
			workflowDAO.saveObject(info);
			return true;
		} else {
			workflowDAO.upldateObject(info);
			return true;
		}
	}

}
