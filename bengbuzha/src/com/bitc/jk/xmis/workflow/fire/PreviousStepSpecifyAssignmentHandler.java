package com.bitc.jk.xmis.workflow.fire;

import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.impl.TaskInstance;
import org.fireflow.engine.taskinstance.DynamicAssignmentHandler;
import org.fireflow.engine.taskinstance.IAssignable;
import org.fireflow.engine.taskinstance.IAssignmentHandler;
import org.fireflow.kernel.KernelException;

public class PreviousStepSpecifyAssignmentHandler implements IAssignmentHandler {

	public void assign(IAssignable iassignable, String performerName)
			throws EngineException, KernelException {
		TaskInstance taskInstance = (TaskInstance) iassignable;
		IWorkflowSession workflowSession = taskInstance
				.getCurrentWorkflowSession();
		DynamicAssignmentHandler dynamicHandler = (DynamicAssignmentHandler) workflowSession
				.getAttribute(performerName);
		if (dynamicHandler == null) {
			IProcessInstance processInstance = taskInstance
					.getAliveProcessInstance();
			throw new EngineException(processInstance, taskInstance.getTask(),
					"没有找到上一步指定的用户！指定变量名：" + performerName);
		}
		dynamicHandler.assign(iassignable, performerName);
	}

}
