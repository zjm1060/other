package com.bitc.jk.xmis.model.device;

public enum PurchaseRequisitionDetailStatus {
	/**
	 * 新采购申请，待采购
	 */
	NEW,
	/**
	 * 采购中，已生成采购清单
	 */
	PURCHASING,
	/**
	 * 采购完成，设备已入库
	 */
	PURCHASED,
	/**
	 * 已分配，已出库
	 */
	ASSIGNED

}