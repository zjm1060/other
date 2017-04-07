package com.bitc.jk.xmis.msg;

import com.bitc.jk.xmis.dao.MessageInfoDAO;
import com.bitc.jk.xmis.model.MessageInfo;

/**
 * 普通消息的发送
 *
 */
public class MessageInfoSender implements MessageSender<MessageInfo>{
	
	private MessageInfoDAO messageInfoDAO;

	public MessageInfoDAO getMessageInfoDAO() {
		return messageInfoDAO;
	}
	public void setMessageInfoDAO(MessageInfoDAO messageInfoDAO) {
		this.messageInfoDAO = messageInfoDAO;
	}
	public void doSendMessage(MessageInfo messageInfo) {
		messageInfoDAO.doSaveOrUpdate(messageInfo);
	}

}
