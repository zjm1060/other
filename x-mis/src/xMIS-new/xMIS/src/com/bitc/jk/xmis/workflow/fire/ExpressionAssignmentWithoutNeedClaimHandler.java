//2013-11-06 未使用
package com.bitc.jk.xmis.workflow.fire;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.IWorkItem;
import org.fireflow.engine.impl.TaskInstance;
import org.fireflow.engine.taskinstance.IAssignable;
import org.fireflow.engine.taskinstance.IAssignmentHandler;
import org.fireflow.kernel.KernelException;
import org.fireflow.model.FormTask;
import org.wltea.expression.ExpressionEvaluator;
import org.wltea.expression.datameta.Variable;

import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.workflow.dao.TodoDAO;
import com.bitc.jk.xmis.workflow.dao.WorkflowDAO;
import com.bitc.jk.xmis.workflow.model.Todo;
import com.bitc.jk.xmis.workflow.model.WorkItemHistory;
import com.bitc.jk.xmis.workflow.util.Constants;
public class ExpressionAssignmentWithoutNeedClaimHandler implements IAssignmentHandler {
	private static Logger logger = Logger
			.getLogger(ExpressionAssignmentWithoutNeedClaimHandler.class);
	
	private TodoDAO todoDAO;

	private WorkflowDAO workflowDAO;

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
		List<IWorkItem> workItems = iassignable.assignToActors(result);
		
		// 默认是不需要签收的
		boolean needClaim = false;
        //如果不需要签收，这里自动进行签收，（FormTask的strategy="all"或者=any并且工作项数量为1）
        if (!needClaim){
	        if (FormTask.ALL.equals(taskInstance.getAssignmentStrategy()) ||
	        		(FormTask.ANY.equals(taskInstance.getAssignmentStrategy()) && result.size()==1)){
	        	for (int i=0;i<workItems.size();i++){
	        		IWorkItem wi = workItems.get(i);
	        		wi.claim();
	        		//wf_待办、历史表处理
					String workItemId = wi.getId();
					Todo todo = todoDAO.findTodoByAppId(Constants.TODO_MODEL_ID,
							workItemId);
					if (todo != null) {
						todo.setCreateTime(DateUtil.getCurrentDateTime());
						todo.setState(Todo.STATE_RUNNING);
						todo.setUrl("/workflow.do?method=openForm&workItemId="
								+ wi.getId());
						todo.setScript("doTask");
					}
					WorkItemHistory history = workflowDAO.load(WorkItemHistory.class,
							wi.getId());
					history.setClaimedTime(wi.getClaimedTime());
					history.setState(wi.getState());
					history.setComments(wi.getComments());
					workflowDAO.saveOrUpdateObject(history);
	        	}
	        }
        }
	}
}
