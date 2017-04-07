package com.bitc.jk.xmis.service.impl;

import java.util.List;

import com.bitc.jk.xmis.dao.MessageInfoDAO;
import com.bitc.jk.xmis.model.MessageInfo;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.service.MessageInfoService;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class MessageInfoServiceImpl implements MessageInfoService {
	
	private MessageInfoDAO messageInfoDAO;

	public MessageInfoDAO getMessageInfoDAO() {
		return messageInfoDAO;
	}

	public void setMessageInfoDAO(MessageInfoDAO messageInfoDAO) {
		this.messageInfoDAO = messageInfoDAO;
	}


	public String getMessageListByQuery(StandardQuery stateQuery,
			PageInfo pageInfo) {
		List<MessageInfo> list = messageInfoDAO.getMessageListByQuery(stateQuery,pageInfo);
		return WebUtil.buildJsonStoreByList(list);
	}

	public MessageInfo getMessageInfoById(String id) {
		return messageInfoDAO.getMessageInfoById(id);
	}

	public void doUpdateMessageInfoState(String id) {
		MessageInfo messageInfo = messageInfoDAO.getMessageInfoById(id);
		messageInfo.setState(0);
		messageInfoDAO.doSaveOrUpdate(messageInfo);
	}

}
