package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.util.Date;

public class CorrespondContractObj implements Serializable{

	private int revenueContractID;//收入合同ID
	private ContractObj outlayContractID;//支出合同ID
	private int correspondID;//对应ID
	private String money;//金额
	private String remark;//备注
	private int outlaysonContractID;//支出子合同ID
	private Date lastModified;//最后修改日期
	private int lastModifier;//最后修改人
	
	
	
	public int getRevenueContractID() {
		return revenueContractID;
	}
	public void setRevenueContractID(int revenueContractID) {
		this.revenueContractID = revenueContractID;
	}

	public ContractObj getOutlayContractID() {
		return outlayContractID;
	}
	public void setOutlayContractID(ContractObj outlayContractID) {
		this.outlayContractID = outlayContractID;
	}
	public int getCorrespondID() {
		return correspondID;
	}
	public void setCorrespondID(int correspondID) {
		this.correspondID = correspondID;
	}
	public String getMoney() {
		return money;
	}
	public void setMoney(String money) {
		this.money = money;
	}
	public String getRemark() {
		return remark;
	}
	public void setRemark(String remark) {
		this.remark = remark;
	}
	public int getOutlaysonContractID() {
		return outlaysonContractID;
	}
	public void setOutlaysonContractID(int outlaysonContractID) {
		this.outlaysonContractID = outlaysonContractID;
	}
	public Date getLastModified() {
		return lastModified;
	}
	public void setLastModified(Date lastModified) {
		this.lastModified = lastModified;
	}
	public int getLastModifier() {
		return lastModifier;
	}
	public void setLastModifier(int lastModifier) {
		this.lastModifier = lastModifier;
	}

}
