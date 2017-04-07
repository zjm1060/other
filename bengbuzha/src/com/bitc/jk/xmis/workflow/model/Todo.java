package com.bitc.jk.xmis.workflow.model;

import java.io.Serializable;
import java.util.Date;

import com.bitc.jk.xmis.model.SimpleEmployee;

/**
 * 待办事项
 * 
 * @author Bom Wu
 * 
 */
public class Todo implements Serializable {
	private static final long serialVersionUID = 6222091939454117635L;

	public static final int DEFAULT_PRIORITY = 5;

	/**
	 * 初始化状态，即未接收状态
	 */
	public static final int STATE_INIT = 0;

	/**
	 * 正在进行状态，已接收状态
	 */
	public static final int STATE_RUNNING = 1;

	/**
	 * 删除状态
	 */
	public static final int STATE_DEL = 10;

	private String id;
	/**
	 * 模块名，用于区别待办的模块
	 */
	private String model;
	/**
	 * 应用ID，用于各模块来标识待办，便于以后删除
	 */
	private String appId;
	private String text;
	private String url;
	// 创建时间或接收时间
	private Date createTime;
	// 用户ID
	private String userID;
	// 点击待办时要执行的脚本
	private String script;
	// 发送人
	private SimpleEmployee sender;
	// 发送时间
	private Date sendTime;
	// 任务描述
	private String description;
	/**
	 * 优先级 0-10，数字越小优先级越高
	 */
	private int priority = DEFAULT_PRIORITY;
	/**
	 * 状态
	 */
	private int state = STATE_RUNNING;
	
	private int isVisible;//是否可见

	public Todo() {
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public int getIsVisible() {
		return isVisible;
	}

	public void setIsVisible(int isVisible) {
		this.isVisible = isVisible;
	}

	public String getText() {
		return text;
	}

	public void setText(String text) {
		this.text = text;
	}

	public String getUrl() {
		return url;
	}

	public void setUrl(String url) {
		this.url = url;
	}

	public int getPriority() {
		return priority;
	}

	public void setPriority(int priority) {
		if (priority > 10) {
			this.priority = 10;
		} else if (priority < 0) {
			this.priority = 0;
		} else {
			this.priority = priority;
		}
	}

	public Date getCreateTime() {
		return createTime;
	}

	public void setCreateTime(Date createTime) {
		this.createTime = createTime;
	}

	public String getUserID() {
		return userID;
	}

	public void setUserID(String userID) {
		this.userID = userID;
	}

	public String getAppId() {
		return appId;
	}

	public void setAppId(String appId) {
		this.appId = appId;
	}

	public String getScript() {
		return script;
	}

	public void setScript(String script) {
		this.script = script;
	}

	public int getState() {
		return state;
	}

	public void setState(int state) {
		this.state = state;
	}

	public String getModel() {
		return model;
	}

	public void setModel(String model) {
		this.model = model;
	}

	public Date getSendTime() {
		return sendTime;
	}

	public void setSendTime(Date sendTime) {
		this.sendTime = sendTime;
	}

	public SimpleEmployee getSender() {
		return sender;
	}

	public void setSender(SimpleEmployee sender) {
		this.sender = sender;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

}
