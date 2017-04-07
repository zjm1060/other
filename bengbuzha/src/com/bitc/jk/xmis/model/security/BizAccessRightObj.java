package com.bitc.jk.xmis.model.security;

import java.io.Serializable;
import java.util.List;

public class BizAccessRightObj implements Serializable{

	private int accessRightID; 

	private int userRoleID;

	private int operationID;

	private boolean isRole;

	private List<BizDataScopeObj> dataScopeList;

	public int getAccessRightID() {
		return accessRightID;
	}

	public void setAccessRightID(int accessRightID) {
		this.accessRightID = accessRightID;
	}

	public List<BizDataScopeObj> getDataScopeList() {
		return dataScopeList;
	}

	public void setDataScopeList(List<BizDataScopeObj> dataScopeList) {
		this.dataScopeList = dataScopeList;
	}

	public boolean isRole() {
		return isRole;
	}

	public void setRole(boolean isRole) {
		this.isRole = isRole;
	}

	public int getOperationID() {
		return operationID;
	}

	public void setOperationID(int operationID) {
		this.operationID = operationID;
	}

	public int getUserRoleID() {
		return userRoleID;
	}

	public void setUserRoleID(int userRoleID) {
		this.userRoleID = userRoleID;
	}
}
