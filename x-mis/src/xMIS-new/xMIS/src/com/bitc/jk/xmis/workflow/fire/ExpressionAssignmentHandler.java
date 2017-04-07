package com.bitc.jk.xmis.workflow.fire;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.impl.TaskInstance;
import org.fireflow.engine.taskinstance.IAssignable;
import org.fireflow.engine.taskinstance.IAssignmentHandler;
import org.fireflow.kernel.KernelException;
import org.wltea.expression.ExpressionEvaluator;
import org.wltea.expression.datameta.Variable;

public class ExpressionAssignmentHandler implements IAssignmentHandler {
	private static Logger logger = Logger
			.getLogger(ExpressionAssignmentHandler.class);

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public void assign(IAssignable iassignable, String performerName)
			throws EngineException, KernelException {
		TaskInstance taskInstance = (TaskInstance) iassignable;
		IProcessInstance pi = taskInstance.getAliveProcessInstance();
		Map vars = pi.getProcessInstanceVariables();
		List variables = new ArrayList();
		Object keys[] = vars.keySet().toArray();
		for (int i = 0; keys != null && i < keys.length; i++) {
			Object key = keys[i];
			variables
					.add(Variable.createVariable(key.toString(), vars.get(key)));
		}
		List<String> result = (List<String>) ExpressionEvaluator.evaluate(
				performerName, variables);
		if (result == null || result.isEmpty()) {
			String message = "执行表达式未找到人员：[" + performerName + "]";
			logger.error(message);
			throw new EngineException(pi, taskInstance.getTask(), message);
		}
		iassignable.assignToActors(result);
	}
}
