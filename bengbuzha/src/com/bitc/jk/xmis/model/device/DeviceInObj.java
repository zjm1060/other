package com.bitc.jk.xmis.model.device;

import java.util.Date;

import com.bitc.jk.xmis.model.TableBean;

public class DeviceInObj extends TableBean {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	private String id;
	private String projectId;
	private Integer outContractId;
	private String purchasingListId;
	private String deviceInfoId;
	private int inNum;
	private Date inDate;
	private String positionId;
	private int useStypeId;
	private Date outFactoryDate;
	private Integer backDays;
	private Integer changeDays;
	private Integer repairDays;
	private int registerId;
	private Date registerDate;
	private String remarks;
	
	public Integer getBackDays() {
		return backDays;
	}
	public void setBackDays(Integer backDays) {
		this.backDays = backDays;
	}
	public Integer getChangeDays() {
		return changeDays;
	}
	public void setChangeDays(Integer changeDays) {
		this.changeDays = changeDays;
	}
	public Integer getRepairDays() {
		return repairDays;
	}
	public void setRepairDays(Integer repairDays) {
		this.repairDays = repairDays;
	}
	public String getProjectId() {
		return projectId;
	}
	public void setProjectId(String projectId) {
		this.projectId = projectId;
	}
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public Integer getOutContractId() {
		return outContractId;
	}
	public void setOutContractId(Integer outContractId) {
		this.outContractId = outContractId;
	}
	public String getPurchasingListId() {
		return purchasingListId;
	}
	public void setPurchasingListId(String purchasingListId) {
		this.purchasingListId = purchasingListId;
	}
	public String getDeviceInfoId() {
		return deviceInfoId;
	}
	public void setDeviceInfoId(String deviceInfoId) {
		this.deviceInfoId = deviceInfoId;
	}
	public int getInNum() {
		return inNum;
	}
	public void setInNum(int inNum) {
		this.inNum = inNum;
	}
	public Date getInDate() {
		return inDate;
	}
	public void setInDate(Date inDate) {
		this.inDate = inDate;
	}
	public String getPositionId() {
		return positionId;
	}
	public void setPositionId(String positionId) {
		this.positionId = positionId;
	}
	public int getUseStypeId() {
		return useStypeId;
	}
	public void setUseStypeId(int useStypeId) {
		this.useStypeId = useStypeId;
	}
	public Date getOutFactoryDate() {
		return outFactoryDate;
	}
	public void setOutFactoryDate(Date outFactoryDate) {
		this.outFactoryDate = outFactoryDate;
	}
	public int getRegisterId() {
		return registerId;
	}
	public void setRegisterId(int registerId) {
		this.registerId = registerId;
	}
	public Date getRegisterDate() {
		return registerDate;
	}
	public void setRegisterDate(Date registerDate) {
		this.registerDate = registerDate;
	}
	public String getRemarks() {
		return remarks;
	}
	public void setRemarks(String remarks) {
		this.remarks = remarks;
	}

	
}
