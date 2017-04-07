package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.MessageInfo;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.StandardQuery;


public interface MessageInfoDAO {

	void doSaveOrUpdate(MessageInfo messageInfo);

	List<MessageInfo> getMessageListByQuery(StandardQuery stateQuery,
			PageInfo pageInfo);

	MessageInfo getMessageInfoById(String id);


	
}
