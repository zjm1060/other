package com.bitc.jk.xmis.msg;

public interface MessageSender<T extends Message> {
	 void doSendMessage(T message);
}
