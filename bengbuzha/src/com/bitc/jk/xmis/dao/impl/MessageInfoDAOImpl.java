package com.bitc.jk.xmis.dao.impl;

import java.util.List;

import com.bitc.jk.xmis.dao.MessageInfoDAO;
import com.bitc.jk.xmis.model.MessageInfo;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class MessageInfoDAOImpl  extends BaseDAOImpl  implements MessageInfoDAO {

	public void doSaveOrUpdate(MessageInfo messageInfo) {
		if("".equals(messageInfo.getId())){
			ht.save(messageInfo);
		} else {
			ht.saveOrUpdate(messageInfo);
		}
	}


	public List<MessageInfo> getMessageListByQuery(StandardQuery stateQuery,
			PageInfo pageInfo) {
		return getListStore(stateQuery, pageInfo, MessageInfo.class);
	}


	public MessageInfo getMessageInfoById(String id) {
		return (MessageInfo) ht.get(MessageInfo.class, id);
	}


}
