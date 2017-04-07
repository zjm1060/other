package com.bitc.jk.xmis.workflow.fire;

import java.util.List;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.EmailMessageInfo;

import org.apache.log4j.Logger;
import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.ITaskInstance;
import org.fireflow.engine.IWorkItem;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.RuntimeContext;
import org.fireflow.engine.event.TaskInstanceEvent;
import org.fireflow.engine.impl.TaskInstance;
import org.fireflow.engine.impl.WorkItem;
import org.fireflow.engine.taskinstance.DefaultTaskInstanceEventListener;
import org.fireflow.model.net.Activity;
import org.springframework.beans.BeanUtils;

import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.msg.EmailAndPlatformMessageInfoSender;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.workflow.dao.TodoDAO;
import com.bitc.jk.xmis.workflow.dao.WorkflowDAO;
import com.bitc.jk.xmis.workflow.model.Todo;
import com.bitc.jk.xmis.workflow.model.WorkItemHistory;
import com.bitc.jk.xmis.workflow.util.Constants;
import com.bitc.jk.xmis.workflow.util.WorkflowUtil;

public class TaskInstanceEventListener extends DefaultTaskInstanceEventListener {
	private static Logger logger = Logger
			.getLogger(TaskInstanceEventListener.class);

	private TodoDAO todoDAO;
	private WorkflowDAO workflowDAO;
	private OrganizationDAO organizationDAO;
	private EmailAndPlatformMessageInfoSender emailAndPlatformMessageInfoSender;

	public void onTaskInstanceEventFired(TaskInstanceEvent e)
			throws EngineException {
		IWorkflowSession session = e.getWorkflowSession();
		IProcessInstance proceInst = e.getProcessInstance();
		ITaskInstance taskInst = e.getSource();
		IWorkItem wi = e.getWorkItem();
		if (e.getEventType() == 2)
			beforeTaskInstanceStart(session, proceInst, taskInst);
		else if (e.getEventType() == 7)
			afterTaskInstanceCompleted(session, proceInst, taskInst);
		else if (e.getEventType() == 5)
			afterWorkItemCreated(session, proceInst, taskInst, wi);
		else if (e.getEventType() == 6)
			afterWorkItemComplete(session, proceInst, taskInst, wi);
		else if (e.getEventType() == 11) {
			afterAbortTaskInstance(session, proceInst, taskInst, wi);
		}
	}

	protected void afterAbortTaskInstance(IWorkflowSession session,
			IProcessInstance proceInst, ITaskInstance taskInst, IWorkItem wi) {
		workflowDAO.abortTaskInstance(taskInst.getId());
	}

	@Override
	protected void afterWorkItemCreated(IWorkflowSession iworkflowsession,
			IProcessInstance iprocessinstance, ITaskInstance itaskinstance,
			IWorkItem iworkitem) throws EngineException {
		TaskInstance thisTaskInstance = (TaskInstance) iworkitem
				.getTaskInstance();
		if (!thisTaskInstance.getFromActivityId().equals("FROM_START_NODE")) {
			createTodo(iprocessinstance, thisTaskInstance, iworkitem);
		}
		createWorkItemHistory(thisTaskInstance, iworkitem);

		//替代触发器
//		todoDAO.updateCurrentPersonReplayWfTrigger(iprocessinstance.getId());
	}

	@Override
	protected void afterWorkItemComplete(IWorkflowSession iworkflowsession,
			IProcessInstance iprocessinstance, ITaskInstance itaskinstance,
			IWorkItem iworkitem) throws EngineException {
		todoDAO.deleteTodoByAppId(Constants.TODO_MODEL_ID, iworkitem.getId());
		updateWorkItemHistory(iworkitem, itaskinstance);
	}

	private void createTodo(IProcessInstance iprocessinstance,
			TaskInstance taskInstance, IWorkItem workItem)
			throws EngineException {
		Todo todo = new Todo();
		todo.setModel(Constants.TODO_MODEL_ID);
		todo.setAppId(workItem.getId());
		todo.setUserID(workItem.getActorId());
		todo.setCreateTime(DateUtil.getCurrentDateTime());
		todo.setText(WorkflowUtil.getTodoTitle(workItem));
		todo.setUrl("/workflow.do?method=claim&workItemId=" + workItem.getId());
		todo.setScript("showTaskInfo");
		todo.setState(workItem.getState());
		todo.setDescription(workItem.getComments());
		todo.setSender(WebUtil.getCurrentEmployee());
		todo.setSendTime(todo.getCreateTime());
		todo.setIsVisible(1);
		todoDAO.saveTodo(todo);

		String text = WorkflowUtil.getTodoTitle(workItem);
		if ("ANY".equals(taskInstance.getAssignmentStrategy())) {
			text += "（此任务为多个接收人中有一人处理即可的任务；如果新的任务中查不到此任务，可能已被其他任务人处理）";
		}
		// 发邮件 两处：还有一处在发统计的邮件
		boolean isSystemTask = Type.getBoolean(iprocessinstance
				.getProcessInstanceVariable("isSystemTask"));
		// boolean isDirect =
		// Type.getBoolean(iprocessinstance.getProcessInstanceVariable("isDirect"));
		// boolean isDirectorLeader =
		// Type.getBoolean(iprocessinstance.getProcessInstanceVariable("isDirectorLeader"));
		if (!isSystemTask) {
			// if (!(isSystemTask&&(isDirect||isDirectorLeader))) {
			List<String> receiverList_p = workflowDAO
					.getActorsEmailByActorId(workItem.getActorId());
			String receverAddress = receiverList_p.get(0);

			EmailMessageInfo messageObj = new EmailMessageInfo();
			messageObj.setTitle(text);
			messageObj.setContent("xMis中有一条新的任务： " + text
					+ " http://xmis.bitc.net.cn/");
			messageObj.setReceiverAddress(receverAddress);
			messageObj.setReceiver(organizationDAO.getSimpleEmployee(Type
					.GetInt(workItem.getActorId())));
			try {
				emailAndPlatformMessageInfoSender.doSendMessage(messageObj);
			} catch (Exception e) {
				logger.info("TaskInstanceEventListener//afterWorkItemCreated----//"
						+ e);
			}

		}
		// 消息、 邮件-stop
	}

	private void createWorkItemHistory(TaskInstance taskInstance,
			IWorkItem workItem) throws EngineException {
		RuntimeContext rtCtx = ((WorkItem) workItem).getRuntimeContext();
		IProcessInstance processInstance = rtCtx.getPersistenceService()
				.findProcessInstanceById(taskInstance.getProcessInstanceId());
		WorkItemHistory history = new WorkItemHistory();
		BeanUtils.copyProperties(workItem, history);
		history.setProcessInstanceId(processInstance.getId());
		history.setProcessId(taskInstance.getProcessId());
		history.setProcessName(processInstance.getDisplayName());
		Activity activity = taskInstance.getActivity();
		history.setActivityId(taskInstance.getActivityId());
		history.setActivityName(activity.getDisplayName());
		history.setTaskInstanceId(taskInstance.getId());
		history.setTaskId(taskInstance.getTask().getId());
		history.setTaskName(taskInstance.getDisplayName());
		String titleExp = taskInstance.getTask().getExtendedAttributes()
				.get(Constants.TODO_TITLE_EXP_KEY);
		if (titleExp != null) {
			history.setTaskName(history.getTaskName() + "||"
					+ WorkflowUtil.executeTitle(processInstance, titleExp));
		}
		history.setActor(new SimpleEmployee(Integer.parseInt(workItem
				.getActorId())));
		history.setSender(WebUtil.getCurrentEmployee());
		workflowDAO.saveObject(history);


	}

	private void updateWorkItemHistory(IWorkItem workItem,
			ITaskInstance taskInstance) {
		WorkItemHistory history = workflowDAO.get(WorkItemHistory.class,
				workItem.getId());
		history.setClaimedTime(workItem.getClaimedTime());
		history.setEndTime(workItem.getEndTime());
		history.setState(workItem.getState());
		history.setComments(workItem.getComments());

		// 任务人不是当前用户的时候，设置代理人2013-12-26
		if (WebUtil.getCurrentEmployee() != null) {
			int userId = WebUtil.getCurrentEmployee().getUserID();
			if (!Type.getString(userId).equals(workItem.getActorId())) {
				history.setAgent(new SimpleEmployee(Type.GetInt(userId)));
			}
		}
//		第一步的任务名字问题
		String titleExp = null;
		try {
			titleExp = taskInstance.getTask().getExtendedAttributes()
					.get(Constants.TODO_TITLE_EXP_KEY);
		} catch (EngineException e) {
			e.printStackTrace();
		}
		RuntimeContext rtCtx = ((WorkItem) workItem).getRuntimeContext();
		IProcessInstance processInstance = rtCtx.getPersistenceService()
				.findProcessInstanceById(taskInstance.getProcessInstanceId());
		if (titleExp != null) {
			history.setTaskName(history.getTaskName() + "||"
					+ WorkflowUtil.executeTitle(processInstance, titleExp));
		}

		workflowDAO.doUpdateWorkItemHistory(history);

	}

	public TodoDAO getTodoDAO() {
		return todoDAO;
	}

	public void setTodoDAO(TodoDAO todoDAO) {
		this.todoDAO = todoDAO;
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

	public EmailAndPlatformMessageInfoSender getEmailAndPlatformMessageInfoSender() {
		return emailAndPlatformMessageInfoSender;
	}

	public void setEmailAndPlatformMessageInfoSender(
			EmailAndPlatformMessageInfoSender emailAndPlatformMessageInfoSender) {
		this.emailAndPlatformMessageInfoSender = emailAndPlatformMessageInfoSender;
	}

}
