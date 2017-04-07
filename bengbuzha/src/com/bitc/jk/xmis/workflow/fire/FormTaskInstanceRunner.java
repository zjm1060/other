package com.bitc.jk.xmis.workflow.fire;

import java.util.List;

import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.ITaskInstance;
import org.fireflow.engine.IWorkItem;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.RuntimeContext;
import org.fireflow.engine.beanfactory.IBeanFactory;
import org.fireflow.engine.event.TaskInstanceEvent;
import org.fireflow.engine.persistence.IPersistenceService;
import org.fireflow.engine.taskinstance.BasicTaskInstanceManager;
import org.fireflow.engine.taskinstance.DefaultFormTaskInstanceRunner;
import org.fireflow.engine.taskinstance.DynamicAssignmentHandler;
import org.fireflow.engine.taskinstance.IAssignable;
import org.fireflow.engine.taskinstance.IAssignmentHandler;
import org.fireflow.engine.taskinstance.ITaskInstanceManager;
import org.fireflow.kernel.KernelException;
import org.fireflow.model.FormTask;
import org.fireflow.model.resource.Participant;

import com.bitc.jk.xmis.workflow.dao.TodoDAO;
import com.bitc.jk.xmis.workflow.dao.WorkflowDAO;
import com.bitc.jk.xmis.workflow.model.Todo;
import com.bitc.jk.xmis.workflow.model.WorkItemHistory;
import com.bitc.jk.xmis.workflow.util.Constants;

public class FormTaskInstanceRunner extends DefaultFormTaskInstanceRunner {
	private TodoDAO todoDAO;

	private WorkflowDAO workflowDAO;

	@Override
	protected void assign(IWorkflowSession currentSession,
			IProcessInstance processInstance, RuntimeContext runtimeContext,
			ITaskInstance taskInstance, FormTask formTask, Participant part,
			DynamicAssignmentHandler dynamicAssignmentHandler)
			throws EngineException, KernelException {
		if (dynamicAssignmentHandler != null) {
			dynamicAssignmentHandler.assign((IAssignable) taskInstance,
					part.getName());
		} else {
			IPersistenceService persistenceService = runtimeContext
					.getPersistenceService();
			List<ITaskInstance> taskInstanceList = persistenceService
					.findTaskInstancesForProcessInstance(
							taskInstance.getProcessInstanceId(),
							taskInstance.getActivityId());
			ITaskInstance theLastCompletedTaskInstance = null;
			for (int i = 0; taskInstanceList != null
					&& i < taskInstanceList.size(); i++) {
				ITaskInstance tmp = (ITaskInstance) taskInstanceList.get(i);
				if (!tmp.getId().equals(taskInstance.getId())
						&& tmp.getTaskId().equals(taskInstance.getTaskId())
						&& tmp.getState().intValue() == 7)
					if (theLastCompletedTaskInstance == null)
						theLastCompletedTaskInstance = tmp;
					else if (theLastCompletedTaskInstance.getStepNumber()
							.intValue() < tmp.getStepNumber().intValue())
						theLastCompletedTaskInstance = tmp;
			}

			if (theLastCompletedTaskInstance != null
					&& ("REDO".equals(formTask.getLoopStrategy()) || currentSession
							.isInWithdrawOrRejectOperation())) {
				List<IWorkItem> workItemList = persistenceService
						.findCompletedWorkItemsForTaskInstance(theLastCompletedTaskInstance
								.getId());
				ITaskInstanceManager taskInstanceMgr = runtimeContext
						.getTaskInstanceManager();
				for (int k = 0; k < workItemList.size(); k++) {
					IWorkItem completedWorkItem = (IWorkItem) workItemList
							.get(k);
					IWorkItem newFromWorkItem = taskInstanceMgr.createWorkItem(
							currentSession, processInstance, taskInstance,
							completedWorkItem.getActorId());
					newFromWorkItem.claim();

					// 增加处理
					updateTodoAndHistory(newFromWorkItem);
				}

			} else {
				IBeanFactory beanFactory = runtimeContext.getBeanFactory();
				IAssignmentHandler assignmentHandler = (IAssignmentHandler) beanFactory
						.getBean(part.getAssignmentHandler());
				assignmentHandler.assign((IAssignable) taskInstance,
						part.getName());
			}
		}
//		BasicTaskInstanceManager taskInstanceMgr = (BasicTaskInstanceManager) runtimeContext
//				.getTaskInstanceManager();
//		if (taskInstanceMgr.getDefaultTaskInstanceEventListener() != null) {
//			TaskInstanceEvent e = new TaskInstanceEvent();
//			e.setSource(taskInstance);
//			e.setWorkflowSession(currentSession);
//			e.setProcessInstance(processInstance);
//			e.setEventType(100);// 为分配完所有任务后的发邮件用 100 以上为自定义
//			taskInstanceMgr.getDefaultTaskInstanceEventListener()
//					.onTaskInstanceEventFired(e);
//		}
	}

	private void updateTodoAndHistory(IWorkItem workItem) {
		Todo todo = todoDAO.findTodoByAppId(Constants.TODO_MODEL_ID,
				workItem.getId());
		if (todo != null) {
			todo.setCreateTime(workItem.getClaimedTime());
			todo.setState(Todo.STATE_RUNNING);
			todo.setUrl("/workflow.do?method=openForm&workItemId="
					+ workItem.getId());
			todo.setScript("doTask");
		}
		WorkItemHistory history = workflowDAO.load(WorkItemHistory.class,
				workItem.getId());
		history.setClaimedTime(workItem.getClaimedTime());
		history.setState(workItem.getState());
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

}
