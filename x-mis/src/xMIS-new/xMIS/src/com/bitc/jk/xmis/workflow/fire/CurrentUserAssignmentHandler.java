package com.bitc.jk.xmis.workflow.fire;

import org.fireflow.engine.EngineException;
import org.fireflow.engine.taskinstance.IAssignable;
import org.fireflow.engine.taskinstance.IAssignmentHandler;
import org.fireflow.kernel.KernelException;

import com.bitc.jk.xmis.util.WebUtil;

public class CurrentUserAssignmentHandler implements IAssignmentHandler {

	public void assign(IAssignable iassignable, String s)
			throws EngineException, KernelException {
		String userId = String
				.valueOf(WebUtil.getCurrentEmployee().getUserID());
		iassignable.assignToActor(userId);
	}

}
