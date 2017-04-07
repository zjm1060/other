package com.bitc.jk.xmis.workflow.tasklistener;

import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.EmailMessageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.msg.EmailAndPlatformMessageInfoSender;

import java.util.List;

import org.apache.log4j.Logger;
import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.ITaskInstance;
import org.fireflow.engine.IWorkItem;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.event.ITaskInstanceEventListener;
import org.fireflow.engine.event.TaskInstanceEvent;

import com.bitc.jk.xmis.service.MissAttendanceService;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.workflow.dao.TodoDAO;
import com.bitc.jk.xmis.workflow.dao.WorkflowDAO;
import com.bitc.jk.xmis.workflow.model.Todo;
import com.bitc.jk.xmis.workflow.util.Constants;

public class MissAttendanceDirectorAssignTaskListener implements
		ITaskInstanceEventListener {
	private static Logger logger = Logger
			.getLogger(MissAttendanceDirectorAssignTaskListener.class);

	private TodoDAO todoDAO;
	private WorkflowDAO workflowDAO;
	private MissAttendanceService missAttendanceService;
	private OrganizationDAO organizationDAO;
	private EmailAndPlatformMessageInfoSender emailAndPlatformMessageInfoSender;

	public EmailAndPlatformMessageInfoSender getEmailAndPlatformMessageInfoSender() {
		return emailAndPlatformMessageInfoSender;
	}

	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}

	public void setOrganizationDAO(OrganizationDAO organizationDAO) {
		this.organizationDAO = organizationDAO;
	}

	public void setEmailAndPlatformMessageInfoSender(
			EmailAndPlatformMessageInfoSender emailAndPlatformMessageInfoSender) {
		this.emailAndPlatformMessageInfoSender = emailAndPlatformMessageInfoSender;
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

	public MissAttendanceService getMissAttendanceService() {
		return missAttendanceService;
	}

	public void setMissAttendanceService(
			MissAttendanceService missAttendanceService) {
		this.missAttendanceService = missAttendanceService;
	}

	public void onTaskInstanceEventFired(TaskInstanceEvent e)
			throws EngineException {
		IWorkflowSession session = e.getWorkflowSession();
		IProcessInstance proceInst = e.getProcessInstance();
		ITaskInstance taskInst = e.getSource();
		IWorkItem wi = e.getWorkItem();
		if (e.getEventType() == 5) {
			afterWorkItemCreated(session, proceInst, taskInst, wi);
		}
	}

	private void afterWorkItemCreated(IWorkflowSession session,
			IProcessInstance processInstance, ITaskInstance taskInst,
			IWorkItem workItem) throws EngineException {

		/**
		 * 如果是任务队列，没有统计待办，生成一个统计；有的话修改url;
		 */
		boolean isSystemTask = Type.getBoolean(processInstance
				.getProcessInstanceVariable("isSystemTask"));
		if (isSystemTask) {
			Todo todoData = todoDAO.findTodoByAppId(Constants.TODO_MODEL_ID,
					workItem.getId());

			// 把已经创建的待办根据APPID，修改显示标识，设置为隐藏
			todoData.setIsVisible(0);
			todoDAO.saveOrUpdateObject(todoData);

			String displayName = DateUtil.getCurrentDate() + "补报考勤统计";
			String text = todoData.getText().substring(0,
					todoData.getText().indexOf(']') + 1)
					+ displayName;
			List<Todo> todoListHasexistList = todoDAO
					.getTodoListByTextAndUserID(text, todoData.getUserID());
			String todoIds = todoData.getId();
			if (todoListHasexistList.size() > 0) {
				Todo todoListHasexist = todoListHasexistList.get(0);
				String oldUrl = todoListHasexist.getUrl();
				String newUrl = oldUrl + "," + todoIds;
				todoListHasexist.setUrl(newUrl);
				todoDAO.saveOrUpadateTodo(todoListHasexist);
			} else {
				Todo todo = new Todo();
				todo.setModel(Constants.TODO_MODEL_LISTS);
				todo.setAppId(todoData.getAppId());
				todo.setUserID(todoData.getUserID());
				todo.setCreateTime(DateUtil.getCurrentDateTime());
				todo.setText(text);
				todo.setUrl("/attendance/missAttendanceLists.jsp?params="
						+ todoIds);// 未签收考勤单、已签收未完成的考勤单
				todo.setScript("showTaskLists");
				todo.setState(todoData.getState());
				todo.setDescription(todoData.getDescription());
				todo.setSendTime(todoData.getSendTime());
				todo.setIsVisible(1);
				todoDAO.saveTodo(todo);

				// 统计的邮件
				// 消息
				SimpleEmployee receiver = organizationDAO
						.getSimpleEmployee(Type.GetInt(todoData.getUserID()));
				if ("ANY".equals(taskInst.getAssignmentStrategy())) {
					text += "（此任务为多个接收人中有一人处理即可的任务；如果新的任务中查不到此任务，可能已被其他任务人处理）";
				}

				List<String> receiverList_p = workflowDAO
						.getActorsEmailByActorId(todoData.getUserID());
				if (receiverList_p != null && receiverList_p.size() > 0
						&& !receiverList_p.get(0).equals("")
						&& !receiverList_p.get(0).trim().equals("")) {
					String receverAddress = receiverList_p.get(0);

					EmailMessageInfo messageObj = new EmailMessageInfo();
					messageObj.setTitle(text);
					messageObj.setContent("xMis中有一条新的任务： " + text 
							+ " http://xmis.bitc.net.cn/");
					messageObj.setReceiverAddress(receverAddress);

					messageObj.setReceiver(receiver);
					try {
						emailAndPlatformMessageInfoSender
								.doSendMessage(messageObj);
					} catch (Exception e) {
						logger.info("SendMessageProcessInstanceEventListener//afterProcessInstanceCompleted----//"
								+ e);
					}
				}

			}

		}

	}
}
