package com.bitc.jk.xmis.workflow.fire;

import org.fireflow.engine.EngineException;
import org.fireflow.engine.IWorkItem;
import org.fireflow.engine.taskinstance.IAssignable;
import org.fireflow.engine.taskinstance.IAssignmentHandler;
import org.fireflow.kernel.KernelException;

import com.bitc.jk.xmis.service.MissAttendanceService;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;
import com.bitc.jk.xmis.workflow.service.WorkflowService;
import com.bitc.jk.xmis.workflow.util.Constants;

import common.Logger;

public class SystemUserAssignmentHandler implements IAssignmentHandler {
	private static final Logger logger = Logger.getLogger(SystemUserAssignmentHandler.class);
	
	private WorkflowService workflowService;
	private MissAttendanceService missAttendanceService;
	
	public WorkflowService getWorkflowService() {
		return workflowService;
	}
	public void setWorkflowService(WorkflowService workflowService) {
		this.workflowService = workflowService;
	}
	public MissAttendanceService getMissAttendanceService() {
		return missAttendanceService;
	}
	public void setMissAttendanceService(MissAttendanceService missAttendanceService) {
		this.missAttendanceService = missAttendanceService;
	}
	
	public void assign(IAssignable iassignable, String s)
			throws EngineException, KernelException {
		String userId = Constants.SYSTEM_USER;
		IWorkItem workItem = iassignable.assignToActor(userId);
		String workItemId=workItem.getId();
		try {
			workflowService.doClaimTask(workItemId, userId);//自动签收
			missAttendanceService.doTimerTask(workItemId);//创建定时任务
		} catch (WorkflowException e) {
			logger.info("--SystemUserAssignmentHandler//", e);
		}
		
		
	}

}
