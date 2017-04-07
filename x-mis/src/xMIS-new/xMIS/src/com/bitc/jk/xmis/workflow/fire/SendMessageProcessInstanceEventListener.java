package com.bitc.jk.xmis.workflow.fire;

import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.event.IProcessInstanceEventListener;
import org.fireflow.engine.event.ProcessInstanceEvent;

import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.EmailMessageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.msg.EmailAndPlatformMessageInfoSender;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.workflow.dao.WorkflowDAO;

public class SendMessageProcessInstanceEventListener implements IProcessInstanceEventListener {
	private static Logger logger = Logger
			.getLogger(SendMessageProcessInstanceEventListener.class);
	private OrganizationDAO organizationDAO;
	private EmailAndPlatformMessageInfoSender emailAndPlatformMessageInfoSender;
	private WorkflowDAO workflowDAO;
	
	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}
	public void setOrganizationDAO(OrganizationDAO organizationDAO) {
		this.organizationDAO = organizationDAO;
	}
	public EmailAndPlatformMessageInfoSender getEmailAndPlatformMessageInfoSender() {
		return emailAndPlatformMessageInfoSender;
	}
	public void setEmailAndPlatformMessageInfoSender(
			EmailAndPlatformMessageInfoSender emailAndPlatformMessageInfoSender) {
		this.emailAndPlatformMessageInfoSender = emailAndPlatformMessageInfoSender;
	}
	public WorkflowDAO getWorkflowDAO() {
		return workflowDAO;
	}
	public void setWorkflowDAO(WorkflowDAO workflowDAO) {
		this.workflowDAO = workflowDAO;
	}
	
	
	public void onProcessInstanceEventFired(
			ProcessInstanceEvent e) throws EngineException {
		IProcessInstance proceInst = e.getSource();
		   if(e.getEventType() == 7)
	            afterProcessInstanceCompleted(proceInst);
	}
	
	
	@SuppressWarnings("rawtypes")
	private void afterProcessInstanceCompleted(IProcessInstance proceInst) {
		String creatorId = proceInst.getCreatorId();
		
		SimpleEmployee receiver = organizationDAO.getSimpleEmployee(Type.GetInt(creatorId));
		List list = organizationDAO.getSendMessageAfterProcessInstance(proceInst.getId());
		
		if (list != null && list.size()>0) {
			Map map = (Map) list.get(0);
			String name = (String) map.get("关键字")+" 流程结束！";
			String email = (String) map.get("电子邮件");
				//根据流程ID 给流程创建人发消息
				EmailMessageInfo messageObj = new EmailMessageInfo();
				messageObj.setTitle(name);
				messageObj.setContent("您发起的流程“"+name+"”已经完成！");
				messageObj.setReceiverAddress(email);
				messageObj.setReceiver(receiver);
				try {
					emailAndPlatformMessageInfoSender.doSendMessage(messageObj);
				} catch (Exception e) {
					logger.info("SendMessageProcessInstanceEventListener//afterProcessInstanceCompleted----//"+e);
				}
				
		}
		
		workflowDAO.updateLastActorToNull(proceInst.getId());
		
	}


}
