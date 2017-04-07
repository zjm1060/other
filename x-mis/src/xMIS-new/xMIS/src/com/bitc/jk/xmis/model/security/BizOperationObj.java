/**
 * 
 */
package com.bitc.jk.xmis.model.security;

import java.io.Serializable;

/**
 * @author Ted Li
 *
 * 2011-5-12
 */
public class BizOperationObj implements Serializable{
	
	private static final long serialVersionUID = 8381162606089858020L;
	private int operationID;
	private String operationName;
	private String operationDesc;
	
	
	public String getOperationDesc() {
		return operationDesc;
	}
	public void setOperationDesc(String operationDesc) {
		this.operationDesc = operationDesc;
	}
	public int getOperationID() {
		return operationID;
	}
	public void setOperationID(int operationID) {
		this.operationID = operationID;
	}
	public String getOperationName() {
		return operationName;
	}
	public void setOperationName(String operationName) {
		this.operationName = operationName;
	}
}
