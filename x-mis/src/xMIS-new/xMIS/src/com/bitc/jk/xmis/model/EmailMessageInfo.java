package com.bitc.jk.xmis.model;

import java.util.List;

public class EmailMessageInfo extends MessageInfo {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	public List<String> receiverList;//用与群发
	public String receiverAddress;
	public String senderAddress;

	public String getSenderAddress() {
		return senderAddress;
	}

	public void setSenderAddress(String senderAddress) {
		this.senderAddress = senderAddress;
	}

	public String getReceiverAddress() {
		return receiverAddress;
	}

	public void setReceiverAddress(String receiverAddress) {
		this.receiverAddress = receiverAddress;
	}

	public List<String> getReceiverList() {
		return receiverList;
	}

	public void setReceiverList(List<String> receiverList) {
		this.receiverList = receiverList;
	}


}
