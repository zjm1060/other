package com.bitc.jk.xmis.model.device;

import java.util.Date;

import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.model.sys.Currency;

public class PurchaseRequisitionDetail {

	private String id;
	private String name;
	private int number;
	private String modelType;
	private String standard;
	private String weight;
	private double referencePrice;
	private Currency currency;
	private PurchaseRequisition purchaseRequisition;
	/**
	 * 项目设备ID
	 */
	private String projectDeviceId;

	/**
	 * 技术要求
	 */
	private String technicalRequirement;

	/**
	 * 期望到货日期
	 */
	private Date expectArrivalDate;

	/**
	 * 备注
	 */
	private String remark;

	private PurchaseRequisitionDetailStatus status;

	public PurchaseRequisitionDetailStatus getStatus() {
		return status;
	}

	public void setStatus(PurchaseRequisitionDetailStatus status) {
		this.status = status;
	}

	private static final long serialVersionUID = 4168867064237923677L;

	public String getTechnicalRequirement() {
		return technicalRequirement;
	}

	public void setTechnicalRequirement(String technicalRequirement) {
		this.technicalRequirement = technicalRequirement;
	}

	public Date getExpectArrivalDate() {
		return expectArrivalDate;
	}

	public void setExpectArrivalDate(Date expectArrivalDate) {
		this.expectArrivalDate = expectArrivalDate;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public String getProjectDeviceId() {
		return projectDeviceId;
	}

	public void setProjectDeviceId(String projectDeviceId) {
		this.projectDeviceId = projectDeviceId;
	}

	public PurchaseRequisition getPurchaseRequisition() {
		return purchaseRequisition;
	}

	public void setPurchaseRequisition(PurchaseRequisition purchaseRequisition) {
		this.purchaseRequisition = purchaseRequisition;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		if (StringUtils.isBlank(id)) {
			this.id = null;
		} else {
			this.id = id;
		}
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getNumber() {
		return number;
	}

	public void setNumber(int number) {
		this.number = number;
	}

	public String getModelType() {
		return modelType;
	}

	public void setModelType(String modelType) {
		this.modelType = modelType;
	}

	public String getStandard() {
		return standard;
	}

	public void setStandard(String standard) {
		this.standard = standard;
	}

	public String getWeight() {
		return weight;
	}

	public void setWeight(String weight) {
		this.weight = weight;
	}

	public double getReferencePrice() {
		return referencePrice;
	}

	public void setReferencePrice(double referencePrice) {
		this.referencePrice = referencePrice;
	}

	public Currency getCurrency() {
		return currency;
	}

	public void setCurrency(Currency currency) {
		this.currency = currency;
	}
}