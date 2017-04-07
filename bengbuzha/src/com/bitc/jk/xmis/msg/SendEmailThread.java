package com.bitc.jk.xmis.msg;

import java.util.Date;
import java.util.List;
import java.util.Properties;

import javax.mail.Authenticator;
import javax.mail.Message;
import javax.mail.MessagingException;
import javax.mail.PasswordAuthentication;
import javax.mail.Session;
import javax.mail.Transport;
import javax.mail.internet.AddressException;
import javax.mail.internet.InternetAddress;
import javax.mail.internet.MimeMessage;

import org.apache.log4j.Logger;

import com.bitc.jk.xmis.model.EmailMessageInfo;

public class SendEmailThread implements Runnable {
	private static Logger logger = Logger.getLogger(SendEmailThread.class);
	private static Properties props;
	private static Session session;

	static {
		// 从配置文件里读取这些配置-待修改
		props = System.getProperties();
		props.put("mail.transport.protocol", "smtp");// 设置邮件服务器的协议
		props.put("mail.host", "smtp.qq.com");// 设置邮件发送服务器地址，该地址由邮件服务供应商提供，这里以QQ邮箱为例
		props.put("mail.smtp.auth", "true");// 设置邮件发送服务器需要用户验证，即需要账号密码才能登录邮箱
		session = Session.getInstance(props, new Authenticator() {

			@Override
			protected PasswordAuthentication getPasswordAuthentication() {
				return new PasswordAuthentication("2081860410@qq.com",
						"ADMINISTRATOR");
			}
		});
	}

	public EmailMessageInfo message;

	public SendEmailThread(EmailMessageInfo message) {
		this.message = message;
	}

	public EmailMessageInfo getMessage() {
		return message;
	}

	public void setMessage(EmailMessageInfo message) {
		this.message = message;
	}

	public boolean send() {
		if (this.message == null) {

		}
		Message msg = new MimeMessage(session);
		try {
			msg.setFrom(new InternetAddress("2081860410@qq.com"));
//			if (message.getReceiverAddress() != null
//					&& !message.getReceiverAddress().equals("")
//					&& !message.getReceiverAddress().trim().equals("")) {
				msg.addRecipient(Message.RecipientType.TO, new InternetAddress(
						message.getReceiverAddress()));
//			} else if (message.getReceiverList() != null) {
//				msg.setRecipients(Message.RecipientType.TO,
//						strListToInternetAddresses(message.getReceiverList()));// 群发
//			}
			msg.setSubject(message.getTitle());
			msg.setText(message.getContent());
			msg.setSentDate(new Date());
			Transport.send(msg);
			return true;
		} catch (AddressException e) {
			logger.info(e);
			return false;
		} catch (MessagingException e) {
			logger.info(e);
			return false;
		}
	}

	public void run() {
		if (this.send()) {
			System.out.println("email success");
		} else {
			System.out.println("emai failure");
		}
	}

	/**
	 * 将列表中的字符串转换成InternetAddress对象
	 * 
	 * @param list
	 *            邮件字符串地址列表
	 * @return InternetAddress对象数组
	 */
	private static InternetAddress[] strListToInternetAddresses(
			List<String> list) {
		if (list == null || list.isEmpty()) {
			return null;
		}
		int size = list.size();
		InternetAddress[] arr = new InternetAddress[size];
		for (int i = 0; i < size; i++) {
			try {
				arr[i] = new InternetAddress(list.get(i));
			} catch (AddressException e) {
				e.printStackTrace();
			}
		}
		return arr;
	}

}
