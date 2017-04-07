package com.bitc.jk.xmis.model;

import java.io.Serializable;

/**
 * 项目设备采购状态
 * 
 * @author Bom Wu
 * 
 */
public class ProjectDevicePurchaseStatus implements Serializable {

	private static final long serialVersionUID = -1918707394479076969L;

	private String id;
	private String projectDeviceId;
	private String purchaseRequisitionDetailId;
	private int number;
	private String status;
	public static String STATUS_REQUEST = "已申请";
	public static String STATUS_PURCHASING = "正在采购";
	public static String STATUS_PURCHASE = "已采购";

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getProjectDeviceId() {
		return projectDeviceId;
	}

	public void setProjectDeviceId(String projectDeviceId) {
		this.projectDeviceId = projectDeviceId;
	}

	public String getPurchaseRequisitionDetailId() {
		return purchaseRequisitionDetailId;
	}

	public void setPurchaseRequisitionDetailId(
			String purchaseRequisitionDetailId) {
		this.purchaseRequisitionDetailId = purchaseRequisitionDetailId;
	}

	public int getNumber() {
		return number;
	}

	public void setNumber(int number) {
		this.number = number;
	}

	public String getStatus() {
		return status;
	}

	public void setStatus(String status) {
		this.status = status;
	}

}
