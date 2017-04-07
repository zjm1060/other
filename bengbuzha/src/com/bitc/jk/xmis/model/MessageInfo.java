package com.bitc.jk.xmis.model;

import java.util.Date;

import com.bitc.jk.xmis.msg.Message;

public class MessageInfo extends TableBean implements Message {

	private static final long serialVersionUID = -54310041027915676L;
	private String id;	
	private SimpleEmployee receiver;	//接收人
	private SimpleEmployee sender;  //发送人
	private String title;          //标题
	private String content;        //内容
	private Date sendTime; //发送时间
	private int state=1; // 是否已读
	//类型
	
	public int getState() {
		return state;
	}
	public SimpleEmployee getReceiver() {
		return receiver;
	}
	public void setReceiver(SimpleEmployee receiver) {
		this.receiver = receiver;
	}
	public SimpleEmployee getSender() {
		return sender;
	}
	public void setSender(SimpleEmployee sender) {
		this.sender = sender;
	}
	public void setState(int state) {
		this.state = state;
	}
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}
	public String getContent() {
		return content;
	}
	public void setContent(String content) {
		this.content = content;
	}
	public Date getSendTime() {
		return sendTime;
	}
	public void setSendTime(Date sendTime) {
		this.sendTime = sendTime;
	}
	
}
