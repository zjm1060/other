package com.bitc.jk.xmis.msg;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

import org.apache.log4j.Logger;

import com.bitc.jk.xmis.model.EmailMessageInfo;
import com.bitc.jk.xmis.service.OrganizationService;
import com.bitc.jk.xmis.util.EmailUtils;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.web.action.LoginAction;

/**
 * 用于工作流中 消息平台、email发送消息
 * 
 */
public class EmailAndPlatformMessageInfoSender implements
		MessageSender<EmailMessageInfo> {
	private static Logger logger = Logger
			.getLogger(EmailAndPlatformMessageInfoSender.class);
	public OrganizationService organizationService;

	public OrganizationService getOrganizationService() {
		return organizationService;
	}

	public void setOrganizationService(OrganizationService organizationService) {
		this.organizationService = organizationService;
	}

	public void doSendMessage(EmailMessageInfo message) {
		// 环境变量
		InputStream cf = LoginAction.class.getClassLoader()
				.getResourceAsStream("properties/config.properties");
		Properties properties = new Properties();
		try {
			properties.load(cf);
		} catch (IOException e) {
			logger.info(e);
		}
		String environment = properties.getProperty("environment");

		// 消息平台
//		if (message.getReceiverList() != null) {
//			organizationService.sendAnnouncementMessage(-1, message.getTitle(),
//					"home");
//		} else if (message.getReceiver() != null) {
			organizationService.sendPersonalMessage(-1, message.getTitle(),
					"home", Type.getString(message.getReceiver().getUserID()));
//		}
		if ("production".equals(environment)) {
			logger.info("production***正式环境*****发emai通知！");
		} else {
//			String list = "";
//			if (message.getReceiverList() != null) {
//				for (int i = 0; i < message.getReceiverList().size(); i++) {
//					list += Type.getString(message.getReceiverList().get(0));
//				}
//			}
			message.setContent(message.getContent()
					+ "——————————————————————————开发环境追加信息：接收人邮箱是"
					+ message.getReceiverAddress());
			message.setReceiverAddress("2081860410@qq.com");
		}
		// 邮件
//		if ((message.getReceiverAddress() != null
//				&& !message.getReceiverAddress().equals("") && !message
//				.getReceiverAddress().trim().equals(""))
//				|| message.getReceiverList() != null) {
		if(message.getReceiverAddress()!=null&&!message.getReceiverAddress().equals("") && !message.getReceiverAddress().trim().equals("")){
			EmailUtils.sendEmail(message, false);// 异步
		}
	}

}
