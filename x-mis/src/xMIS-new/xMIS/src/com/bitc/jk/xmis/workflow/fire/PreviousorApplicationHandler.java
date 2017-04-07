package com.bitc.jk.xmis.workflow.fire;

import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.ITaskInstance;
import org.fireflow.engine.impl.TaskInstance;
import org.fireflow.engine.taskinstance.IApplicationHandler;

import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.MessageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.msg.MessageSender;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.workflow.dao.WorkflowDAO;

public class PreviousorApplicationHandler implements IApplicationHandler {
	private MessageSender<MessageInfo> messageSender;
	private WorkflowDAO workflowDAO;
	private OrganizationDAO organizationDAO;

	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}

	public void setOrganizationDAO(OrganizationDAO organizationDAO) {
		this.organizationDAO = organizationDAO;
	}

	public WorkflowDAO getWorkflowDAO() {
		return workflowDAO;
	}

	public void setWorkflowDAO(WorkflowDAO workflowDAO) {
		this.workflowDAO = workflowDAO;
	}

	public MessageSender<MessageInfo> getMessageSender() {
		return messageSender;
	}

	public void setMessageSender(MessageSender<MessageInfo> messageSender) {
		this.messageSender = messageSender;
	}

	public void execute(ITaskInstance iTaskInstance) throws EngineException {
		TaskInstance taskInstance = (TaskInstance) iTaskInstance;
//		IProcessInstance processInstance = taskInstance.getAliveProcessInstance();
		IProcessInstance processInstance = workflowDAO.getProcessInstanceByID(taskInstance.getProcessInstanceId());

		// 验收部门主任分配任务时，可知事业部主任ID，将事业部主任ID 放入流程参数 “divisionManagerID”
		int divisionManagerID = Type.GetInt(processInstance.getProcessInstanceVariable("divisionManagerID"));
		int checkManagerID = Type.GetInt(processInstance.getProcessInstanceVariable("checkManagerID"));
		String projectCode = Type.getString(processInstance.getProcessInstanceVariable("projectCode"));
		try{
			String isApproved = (String) processInstance
			.getProcessInstanceVariable("isApproved");
			String content = "";
			if (isApproved.equals("yes")) {
				content = "通过";
			} else {
				content = "未通过";
			}
			
			MessageInfo messageInfo = new MessageInfo();
			messageInfo.setTitle("项目"+projectCode+"审批结果");// 标题
			messageInfo.setContent(content);
			// 发送人 在验收部门主任审批流程中，将验收部门的主任放入流程参数“checkManagerID”
			SimpleEmployee sender = organizationDAO.getSimpleEmployee(checkManagerID);
			messageInfo.setSender(sender);
			// 接收人
			SimpleEmployee receiver = organizationDAO.getSimpleEmployee(divisionManagerID);
			messageInfo.setReceiver(receiver);
			messageInfo.setSendTime(DateUtil.getCurrentDateTime());
			messageSender.doSendMessage(messageInfo); // 发送消息
			
		} catch( Exception e) {
			e.printStackTrace();
		}

	}

}
