package com.bitc.jk.xmis.service;

import com.bitc.jk.xmis.model.MessageInfo;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface MessageInfoService {

	public String getMessageListByQuery(StandardQuery stateQuery,
			PageInfo pageInfo);

	public MessageInfo getMessageInfoById(String id);

	public void doUpdateMessageInfoState(String id);

}
