//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\Impl\\TaskServiceImpl.java

package com.bitc.jk.xmis.service.impl;

import java.util.ArrayList;
import java.util.List;

import org.apache.log4j.Logger;
import org.codehaus.jackson.type.TypeReference;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.RuntimeContext;

import com.bitc.jk.xmis.dao.TaskDAO;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.TaskObj;
import com.bitc.jk.xmis.service.TaskService;
import com.bitc.jk.xmis.util.JacksonUtil;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.WorkflowConstants;
import com.bitc.jk.xmis.workflow.util.WorkflowUtil;

public class TaskServiceImpl implements TaskService {
	private static Logger logger = Logger.getLogger(TaskServiceImpl.class);
	private TaskDAO taskDAO;
	private RuntimeContext workflowContext;

	public void doAssignTask(String tasks) throws Exception {
		List<TaskObj> taskList = JacksonUtil.readValue(tasks,
				new TypeReference<List<TaskObj>>() {
				});
		doSaveTasks(taskList);
		//TODO 还未完成，不能使用
		EmployeeObj emp = WebUtil.getCurrentEmployee();
		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		IProcessInstance processInstance = WorkflowUtil.startProcess(
				workflowSession, WorkflowConstants.WF_TASK_ASSIGN,
				String.valueOf(emp.getUserID()));
		List<String> actorIds = getTaskEmpIds(taskList);
		WorkflowUtil.specifyActor(workflowSession, actorIds, "__assignee");
		WorkflowUtil.completeFirstTask(workflowSession, processInstance,
				String.valueOf(emp.getUserID()), null);
		if (logger.isInfoEnabled()) {
			logger.info("启动任务分配流程：" + processInstance.getId());
		}
	}

	private void doSaveTasks(List<TaskObj> taskList) {
		for(TaskObj task:taskList){
			taskDAO.saveTask(task);
		}
	}

	private List<String> getTaskEmpIds(List<TaskObj> taskList) {
		List<String> ids = new ArrayList<String>();
		for (TaskObj task : taskList) {
			ids.add(String.valueOf(task.getOperatorID()));
		}
		return ids;
	}

	public TaskDAO getTaskDAO() {
		return taskDAO;
	}

	public void setTaskDAO(TaskDAO taskDAO) {
		this.taskDAO = taskDAO;
	}

	public RuntimeContext getWorkflowContext() {
		return workflowContext;
	}

	public void setWorkflowContext(RuntimeContext workflowContext) {
		this.workflowContext = workflowContext;
	}

}
