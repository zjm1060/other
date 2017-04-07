package com.bitc.jk.xmis.util;

import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

import com.bitc.jk.xmis.model.EmailMessageInfo;
import com.bitc.jk.xmis.msg.SendEmailThread;

public class EmailUtils {
	 private static Executor executor = Executors.newFixedThreadPool(10); 
	/**
	 * 
	 * @param messageObj
	 * @param sync 是否同步
	 */
	public static void sendEmail(EmailMessageInfo messageObj, boolean sync) {
		SendEmailThread s = new SendEmailThread(messageObj);
		if (sync) {
			s.send();
		} else {
			executor.execute(s);
		}
	}

}
