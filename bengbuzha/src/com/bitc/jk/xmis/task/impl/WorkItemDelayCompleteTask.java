package com.bitc.jk.xmis.task.impl;

import org.fireflow.engine.IWorkItem;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.RuntimeContext;
import org.springframework.context.ApplicationContext;

import com.bitc.jk.xmis.task.Command;
import com.bitc.jk.xmis.task.Task;
import com.bitc.jk.xmis.workflow.dao.WorkflowDAO;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;
import com.bitc.jk.xmis.workflow.model.WorkItemHistory;
import com.bitc.jk.xmis.workflow.util.Constants;
import com.bitc.jk.xmis.workflow.util.WorkflowUtil;

public class WorkItemDelayCompleteTask implements Command {

	private IWorkflowSession workflowSession;
	public WorkflowDAO workflowDAO;

	public WorkflowDAO getWorkflowDAO() {
		return workflowDAO;
	}
	public void setWorkflowDAO(WorkflowDAO workflowDAO) {
		this.workflowDAO = workflowDAO;
	}

	// MissAttendanceWorkItemDelayCompleteTask任务队列专用
	public void execute(ApplicationContext context, Task task) {
		String param = task.getParamString();
		String[] params = param.split(",");

		String workItemId = params[0];
		workflowSession = ((RuntimeContext) context
				.getBean("fireRuntimeContext")).getWorkflowSession();
		try {
			WorkflowUtil.completeTask(workflowSession, workItemId,
					Constants.SYSTEM_USER, Constants.SYSTEM_USER_NAME);

			IWorkItem workItem = workflowDAO.getWorkItemById(workItemId);
			WorkItemHistory history = workflowDAO.get(WorkItemHistory.class,
					workItem.getId());
			history.setClaimedTime(workItem.getClaimedTime());
			history.setEndTime(workItem.getEndTime());
			history.setState(workItem.getState());
			history.setComments(workItem.getComments());
			workflowDAO.doUpdateWorkItemHistory(history);
		} catch (WorkflowException e) {
			e.printStackTrace();
		}
	}

}
