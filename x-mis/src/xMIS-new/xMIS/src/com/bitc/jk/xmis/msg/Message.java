package com.bitc.jk.xmis.msg;

import java.util.Date;

import com.bitc.jk.xmis.model.SimpleEmployee;

/**
 *  消息
 *
 */
public interface Message {
	String getTitle(); //标题

	SimpleEmployee getReceiver();//接收人

	SimpleEmployee getSender();//发送人

	String getContent();//正文
	
	Date getSendTime();//发送时间
	
	int getState(); //是否已读的状态
}
