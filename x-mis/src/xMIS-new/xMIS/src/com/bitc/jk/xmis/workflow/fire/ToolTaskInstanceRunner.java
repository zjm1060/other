package com.bitc.jk.xmis.workflow.fire;

import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.ITaskInstance;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.RuntimeContext;
import org.fireflow.engine.taskinstance.IApplicationHandler;
import org.fireflow.engine.taskinstance.ITaskInstanceManager;
import org.fireflow.engine.taskinstance.ITaskInstanceRunner;
import org.fireflow.kernel.KernelException;
import org.fireflow.model.ToolTask;

public class ToolTaskInstanceRunner implements ITaskInstanceRunner {

	public ToolTaskInstanceRunner() {
	}

	public void run(IWorkflowSession currentSession,
			RuntimeContext runtimeContext, IProcessInstance processInstance,
			ITaskInstance taskInstance) throws EngineException, KernelException {
		if (!"TOOL".equals(taskInstance.getTaskType()))
			throw new EngineException(
					processInstance,
					taskInstance.getActivity(),
					"DefaultToolTaskInstanceRunner\uFF1ATaskInstance\u7684\u4EFB\u52A1\u7C7B\u578B\u9519\u8BEF\uFF0C\u53EA\u80FD\u4E3ATOOL\u7C7B\u578B");
		org.fireflow.model.Task task = taskInstance.getTask();
		if (task == null) {
			org.fireflow.model.WorkflowProcess process = taskInstance
					.getWorkflowProcess();
			throw new EngineException(taskInstance.getProcessInstanceId(),
					process, taskInstance.getTaskId(),
					"The Task is null,can NOT start the taskinstance,");
		}
		if (((ToolTask) task).getApplication() == null
				|| ((ToolTask) task).getApplication().getHandler() == null) {
			org.fireflow.model.WorkflowProcess process = taskInstance
					.getWorkflowProcess();
			throw new EngineException(
					taskInstance.getProcessInstanceId(),
					process,
					taskInstance.getTaskId(),
					"The task.getApplication() is null or task.getApplication().getHandler() is null,can NOT start the taskinstance,");
		}
		Object obj = runtimeContext.getBeanByName(((ToolTask) task)
				.getApplication().getHandler());
		if (obj == null || !(obj instanceof IApplicationHandler)) {
			org.fireflow.model.WorkflowProcess process = taskInstance
					.getWorkflowProcess();
			throw new EngineException(
					taskInstance.getProcessInstanceId(),
					process,
					taskInstance.getTaskId(),
					(new StringBuilder(
							"Run tool task instance error! Not found the instance of "))
							.append(((ToolTask) task).getApplication()
									.getHandler())
							.append(" or the instance not implements IApplicationHandler")
							.toString());
		}
		((IApplicationHandler) obj).execute(taskInstance);
		ITaskInstanceManager taskInstanceManager = runtimeContext
				.getTaskInstanceManager();
		taskInstanceManager.completeTaskInstance(currentSession,
				processInstance, taskInstance, null);
	}
}