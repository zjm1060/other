package com.bitc.jk.xmis.workflow.fire;

import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

import org.fireflow.engine.EngineException;
import org.fireflow.engine.IWorkItem;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.IWorkflowSessionAware;
import org.fireflow.engine.event.TaskInstanceEvent;
import org.fireflow.engine.impl.ProcessInstanceTrace;
import org.fireflow.engine.impl.TaskInstance;
import org.fireflow.engine.impl.WorkItem;
import org.fireflow.engine.persistence.IPersistenceService;
import org.fireflow.engine.taskinstance.BasicTaskInstanceManager;
import org.fireflow.kernel.IActivityInstance;
import org.fireflow.kernel.INetInstance;
import org.fireflow.kernel.ISynchronizerInstance;
import org.fireflow.kernel.ITransitionInstance;
import org.fireflow.kernel.KernelException;
import org.fireflow.kernel.impl.Token;
import org.fireflow.model.Task;
import org.fireflow.model.WorkflowProcess;
import org.fireflow.model.net.Activity;

public class TaskInstanceManager extends BasicTaskInstanceManager {
	public void completeWorkItem(IWorkItem workItem,
			IActivityInstance targetActivityInstance, String comments)
			throws EngineException, KernelException {
		if (workItem.getState().intValue() != 1) {
			TaskInstance thisTaskInst = (TaskInstance) workItem
					.getTaskInstance();

			throw new EngineException(thisTaskInst.getProcessInstanceId(),
					thisTaskInst.getWorkflowProcess(),
					thisTaskInst.getTaskId(),
					"Complete work item failed . The state of the work item [id="
							+ workItem.getId() + "] is " + workItem.getState());
		}

		if (workItem.getTaskInstance().isSuspended().booleanValue()) {
			TaskInstance thisTaskInst = (TaskInstance) workItem
					.getTaskInstance();
			WorkflowProcess process = thisTaskInst.getWorkflowProcess();
			throw new EngineException(thisTaskInst.getProcessInstanceId(),
					process, thisTaskInst.getTaskId(),
					"Complete work item failed. The correspond task instance [id="
							+ thisTaskInst.getId() + "] is suspended");
		}

		IPersistenceService persistenceService = this.rtCtx
				.getPersistenceService();

		((WorkItem) workItem).setComments(comments);
		((WorkItem) workItem).setState(Integer.valueOf(7));
		((WorkItem) workItem).setEndTime(this.rtCtx.getCalendarService()
				.getSysDate());
		persistenceService.saveOrUpdateWorkItem(workItem);

		TaskInstanceEvent e = new TaskInstanceEvent();
		e.setSource(workItem.getTaskInstance());
		e.setWorkflowSession(((IWorkflowSessionAware) workItem)
				.getCurrentWorkflowSession());
		e.setProcessInstance(((TaskInstance) workItem.getTaskInstance())
				.getAliveProcessInstance());
		e.setWorkItem(workItem);
		e.setEventType(6);
		if (this.getDefaultTaskInstanceEventListener() != null) {
			this.getDefaultTaskInstanceEventListener()
					.onTaskInstanceEventFired(e);
		}

		fireTaskInstanceEvent(workItem.getTaskInstance(), e);

		((TaskInstance) workItem.getTaskInstance())
				.complete(targetActivityInstance);
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	@Override
	public void rejectWorkItem(IWorkItem workItem, String comments)
			throws EngineException, KernelException {
		Activity thisActivity = workItem.getTaskInstance().getActivity();
		TaskInstance thisTaskInstance = (TaskInstance) workItem
				.getTaskInstance();
		if ((workItem.getState().intValue() > 5)
				|| (workItem.getTaskInstance().isSuspended().booleanValue())) {
			throw new EngineException(thisTaskInstance.getProcessInstanceId(),
					thisTaskInstance.getWorkflowProcess(),
					thisTaskInstance.getTaskId(),
					// "Reject operation refused!Current work item is completed or the correspond task instance is suspended!!"
					"拒收操作被拒绝！此任务已完成或者已中止！");
		}

		if (thisActivity.getTasks().size() > 1) {
			throw new EngineException(thisTaskInstance.getProcessInstanceId(),
					thisTaskInstance.getWorkflowProcess(),
					thisTaskInstance.getTaskId(),
					// "Reject operation refused!The correspond activity has more than 1 tasks"
					"拒收操作被拒绝！当前步骤存在多个任务，不能拒收！");
		}

		if ("ALL".equals(thisTaskInstance.getAssignmentStrategy())) {
			throw new EngineException(thisTaskInstance.getProcessInstanceId(),
					thisTaskInstance.getWorkflowProcess(),
					thisTaskInstance.getTaskId(),
					// "Reject operation refused!The assignment strategy is 'ALL'"
					"拒收操作被拒绝，当前步骤为多人任务，不能拒收！");
		}

		if (thisTaskInstance.getFromActivityId().equals("FROM_START_NODE")) {
			throw new EngineException(thisTaskInstance.getProcessInstanceId(),
					thisTaskInstance.getWorkflowProcess(),
					thisTaskInstance.getTaskId(),
					"Reject operation refused!Because the from activityId equals FROM_START_NODE");
		}

		IPersistenceService persistenceService = this.rtCtx
				.getPersistenceService();
		List siblingTaskInstancesList = null;

		siblingTaskInstancesList = persistenceService
				.findTaskInstancesForProcessInstanceByStepNumber(workItem
						.getTaskInstance().getProcessInstanceId(),
						thisTaskInstance.getStepNumber());

		if (siblingTaskInstancesList.size() > 1) {
			throw new EngineException(thisTaskInstance.getProcessInstanceId(),
					thisTaskInstance.getWorkflowProcess(),
					thisTaskInstance.getTaskId(),
					// "Reject operation refused!Because the process instance has taken a split operation."
					"拒收操作被拒绝！流程中有分支结构，不能拒收任务！");
		}

		List fromActivityIdList = new ArrayList();
		StringTokenizer tokenizer = new StringTokenizer(
				thisTaskInstance.getFromActivityId(), "&");
		while (tokenizer.hasMoreTokens()) {
			fromActivityIdList.add(tokenizer.nextToken());
		}
		WorkflowProcess workflowProcess = workItem.getTaskInstance()
				.getWorkflowProcess();
		for (int i = 0; i < fromActivityIdList.size(); i++) {
			String fromActivityId = (String) fromActivityIdList.get(i);
			Activity fromActivity = (Activity) workflowProcess
					.findWFElementById(fromActivityId);
			List fromTaskList = fromActivity.getTasks();
			for (int j = 0; j < fromTaskList.size(); j++) {
				Task task = (Task) fromTaskList.get(j);
				if ((!"TOOL".equals(task.getType()))
						&& (!"SUBFLOW".equals(task.getType())))
					continue;
				throw new EngineException(
						thisTaskInstance.getProcessInstanceId(),
						thisTaskInstance.getWorkflowProcess(),
						thisTaskInstance.getTaskId(),
						"Reject operation refused!The previous activity contains tool-task or subflow-task");
			}

		}

		INetInstance netInstance = this.rtCtx.getKernelManager()
				.getNetInstance(workflowProcess.getId(),
						workItem.getTaskInstance().getVersion());
		if (netInstance == null) {
			throw new EngineException(thisTaskInstance.getProcessInstanceId(),
					thisTaskInstance.getWorkflowProcess(),
					thisTaskInstance.getTaskId(),
					"Not find the net instance for workflow process [id="
							+ workflowProcess.getId() + ", version="
							+ workItem.getTaskInstance().getVersion() + "]");
		}

		IWorkflowSession session = ((IWorkflowSessionAware) workItem)
				.getCurrentWorkflowSession();
		session.setWithdrawOrRejectOperationFlag(true);
		int newStepNumber = thisTaskInstance.getStepNumber().intValue() + 1;
		try {
			workItem.setComments(comments);
			((WorkItem) workItem).setState(Integer.valueOf(9));
			((WorkItem) workItem).setEndTime(this.rtCtx.getCalendarService()
					.getSysDate());
			this.rtCtx.getPersistenceService().saveOrUpdateWorkItem(workItem);

			persistenceService.abortTaskInstance(thisTaskInstance);

			persistenceService.deleteTokensForNode(
					thisTaskInstance.getProcessInstanceId(),
					thisTaskInstance.getActivityId());

			// add by Bom Wu 2012-10-16
			TaskInstanceEvent e = new TaskInstanceEvent();
			e.setSource(workItem.getTaskInstance());
			e.setWorkflowSession(session);
			e.setProcessInstance(((TaskInstance) workItem.getTaskInstance())
					.getAliveProcessInstance());
			e.setWorkItem(workItem);
			e.setEventType(11);// workItem abort
			if (this.getDefaultTaskInstanceEventListener() != null) {
				this.getDefaultTaskInstanceEventListener()
						.onTaskInstanceEventFired(e);
			}
			fireTaskInstanceEvent(thisTaskInstance, e);

			IActivityInstance fromActivityInstance = null;
			for (int i = 0; i < fromActivityIdList.size(); i++) {
				String fromActivityId = (String) fromActivityIdList.get(i);
				Object obj = netInstance.getWFElementInstance(fromActivityId);
				fromActivityInstance = (IActivityInstance) obj;
				Token newToken = new Token();
				newToken.setAlive(Boolean.valueOf(true));
				newToken.setNodeId(fromActivityId);
				newToken.setProcessInstanceId(thisTaskInstance
						.getProcessInstanceId());
				newToken.setProcessInstance(thisTaskInstance
						.getAliveProcessInstance());
				newToken.setFromActivityId(thisTaskInstance.getActivityId());
				newToken.setStepNumber(Integer.valueOf(newStepNumber));
				newToken.setValue(Integer.valueOf(0));
				persistenceService.saveOrUpdateToken(newToken);

				createTaskInstances(newToken, fromActivityInstance);

				if (this.rtCtx.isEnableTrace()) {
					ProcessInstanceTrace trace = new ProcessInstanceTrace();
					trace.setProcessInstanceId(thisTaskInstance
							.getProcessInstanceId());
					trace.setStepNumber(Integer.valueOf(newStepNumber));
					trace.setType("Reject");
					trace.setFromNodeId(thisActivity.getId());
					trace.setToNodeId(fromActivityId);
					trace.setEdgeId("");
					this.rtCtx.getPersistenceService()
							.saveOrUpdateProcessInstanceTrace(trace);
				}
			}

			ITransitionInstance theLeavingTransitionInstance = (ITransitionInstance) fromActivityInstance
					.getLeavingTransitionInstances().get(0);
			ISynchronizerInstance synchronizerInstance = (ISynchronizerInstance) theLeavingTransitionInstance
					.getLeavingNodeInstance();

			if (synchronizerInstance.getEnteringTransitionInstances().size() > fromActivityIdList
					.size()) {
				Token supplementToken = new Token();
				supplementToken.setAlive(Boolean.valueOf(false));
				supplementToken.setNodeId(synchronizerInstance
						.getSynchronizer().getId());
				supplementToken.setProcessInstanceId(thisTaskInstance
						.getProcessInstanceId());
				supplementToken.setProcessInstance(thisTaskInstance
						.getAliveProcessInstance());
				supplementToken.setFromActivityId("EMPTY(created by reject)");
				supplementToken.setStepNumber(Integer.valueOf(thisTaskInstance
						.getStepNumber().intValue() + 1));
				supplementToken.setValue(Integer.valueOf(synchronizerInstance
						.getVolume()
						- theLeavingTransitionInstance.getWeight()
						* fromActivityIdList.size()));
				persistenceService.saveOrUpdateToken(supplementToken);
			}
		} finally {
			session.setWithdrawOrRejectOperationFlag(false);
		}
	}
}
