//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\TaskAction.java

package com.bitc.jk.xmis.web.action;

import org.apache.log4j.Logger;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.service.TaskService;

public class TaskAction extends DispatchAction {
	private static Logger logger = Logger.getLogger(TaskAction.class);

	private TaskService taskService;

	/**
	 * @roseuid 4C19C97A02AF
	 */
	public TaskAction() {

	}

	/**
	 * Access method for the taskService property.
	 * 
	 * @return   the current value of the taskService property
	 */
	public TaskService getTaskService() {
		return taskService;
	}

	/**
	 * Sets the value of the taskService property.
	 * 
	 * @param aTaskService the new value of the taskService property
	 */
	public void setTaskService(TaskService aTaskService) {
		taskService = aTaskService;
	}

	/**
	 * @roseuid 4C19C12B03C8
	 */
	public void getNewTaskList() {

	}
}
