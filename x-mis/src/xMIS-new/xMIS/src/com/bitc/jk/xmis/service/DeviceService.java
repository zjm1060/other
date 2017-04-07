package com.bitc.jk.xmis.service;

import java.util.Date;
import java.util.List;
import java.util.Map;

import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.device.DeviceInObj;
import com.bitc.jk.xmis.model.device.DeviceInfo;
import com.bitc.jk.xmis.model.device.PurchaseRequisition;
import com.bitc.jk.xmis.model.device.PurchasingDetail;
import com.bitc.jk.xmis.model.device.PurchasingList;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.Power;
import com.bitc.jk.xmis.model.sys.Weight;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.web.WorkItemParam;

public interface DeviceService {
	PurchaseRequisition getGeneratePurchaseRequisition(String projectId,
			String seldeviceids);

	/**
	 * 查询设备库
	 * 
	 * @param query
	 * @param pageInfo
	 * @return
	 * @throws Exception 
	 */
	String getDeviceInfoListStoreByQuery(StandardQuery query, PageInfo pageInfo) throws Exception;

	/**
	 * 保存采购申请，同时启动采购申请流程
	 * @param param TODO
	 * @param requisition
	 *            申请信息
	 * @param details
	 *            申请明细 json
	 * 
	 * @throws Exception
	 */
	String doSavePurchaseRequisition(WorkItemParam param,
			PurchaseRequisition requisition, String details) throws Exception;

	/**
	 * 
	 * 获取采购申请
	 * 
	 * @param requestId
	 *            申请ID
	 * @return
	 */
	PurchaseRequisition getPurchaseRequisition(String requestId);

	/**
	 * 查询采购申请列表
	 * 
	 * @param query
	 * @param pageInfo
	 * @return
	 */
	String getPurchaseRequisitionList(StandardQuery query, PageInfo pageInfo);

	/**
	 * 查询待采购列表
	 * 
	 * @param query
	 * @param info
	 * @return
	 */
	String getTobePurchaseList(StandardQuery query, PageInfo info);

	/**
	 * 获取采购清单
	 * 
	 * @param purchasingId
	 *            清单ID
	 * @return
	 */
	PurchasingList getPurchasingList(String purchasingId);

	/**
	 * 保存采购清单
	 * 
	 * @param purchasingList
	 *            采购清单
	 * @param details
	 *            物资明细JSON
	 * @return
	 * @throws Exception
	 */
	void doSavePurchasingList(PurchasingList purchasingList, String details) throws Exception;

	/**
	 * 运行采购申请流程
	 * @param workItemParam TODO
	 * @param requisition
	 *            采购申请对象
	 * @param details
	 *            申请明细
	 * 
	 * @throws Exception
	 */
	void doPurchaseRequisition(WorkItemParam workItemParam, PurchaseRequisition requisition,
			String details) throws Exception;

	String getPurchasingListList(StandardQuery query, PageInfo pageInfo);

	/** 到库信息的保存、编辑 
	 */ 
	void doSaveDeviceIn(DeviceInObj deviceIn);

	/**库存、货架、存储位置的下拉列表
	 * @param fid
	 * @param type
	 * @return
	 */
	String getDeviceAddressCombo(String fid, String type);

	/**根据项目ID得到项目物资清单
	 * @param projectId
	 * @return
	 */
	String getPorjectDeviceInfoList(String projectId);

	String getDeviceInList(StandardQuery query, PageInfo pageInfo);

	/**库存、货架、存储位置的新建
	 * @param fid
	 * @param name
	 * @param code
	 * @param type
	 */
	void doAddDeviceAddress(String fid, String name, String code, String type);

	String getArrivalDeviceInfos(String contractId, String deviceInfoId);


	public String getPurchasingListByOutContract(Integer contractId);


	public String getRelationProjectID(Integer contractId);

	void doAddMaterialsPurchasingDetail(PurchasingDetail pl, DeviceInfo dfo,
			ContractObj cop,Integer contractId,SimpleEmployee see);

	void doDeletePurchasingDetail(String purdelID);

	String getPurchasingDetaildouble(String contractID, String materialID,Integer contractId);

	String getPurchasingDe(String projectID, Integer deviceID);

	String getcontractSum(String valueField);

	void doUpdatePurchasingDetail(PurchasingDetail pl, DeviceInfo dfo,
			SimpleEmployee see);


	String getPurchasingDetailTotal(String contractID);

	String getRecordpurdelID(String purdelID);

	void doSaveMaterialsInformation(DeviceInfo dfo);

	Map<String, String> getMaterialsInfomation(String id);

	DeviceInfo getMaterialnfoObjById(String id);

	String getDeviceInfoListStoreByQueryto(StandardQuery query,
			PageInfo pageInfo);

	String getMaximumSupplier(String materialsID);

	String getUseProject(String materialsID);

	
	/**  物资信息列表（到货信息保存页面 ）
	 * @param query
	 * @param pageInfo
	 * @return
	 */
	String getDeviceInfoList(StandardQuery query, PageInfo pageInfo);

	/**领用级别下拉框
	 * @return
	 */
	String getUseStypeList();

	/**到库清单、到货登记列表（第二版）
	 * @param query
	 * @param pageInfo
	 * @return
	 */
	String getDeviceInListByContractId(StandardQuery query, PageInfo pageInfo);

	/**根据ID得到一条到库信息
	 * @param id
	 * @return
	 */
	String getDeviceInInfoByID(String id);

	/**到库清单的删除
	 * @param id
	 */
	void delDeviceInObjByID(String id);

	/** 物资存放地址信息列表、项目物资清单上的存放地址列表
	 * @param query
	 * @param pageInfo
	 * @return
	 */
	String getDeviceAddressList(StandardQuery query, PageInfo pageInfo);

	/**根据合同ID得到其子合同的列表 含有采购清单ID、收入合同ID
	 * @param contractId
	 * @return
	 */
	String getDeviceInfoListWithPurchaseList(String contractId);

	/** 根据收入合同ID 得到对应的子合同列表
	 * @param fid
	 * @return
	 */
	String getContactListByProjectIdCombo(String fid);

	/**已采购的数量，用于到货校验
	 * @param fid
	 * @param purchaseListId
	 * @return
	 */
	int getPurchaseSumNum(String fid, String purchaseListId);
	String getImportDeviceInfoList(StandardQuery query, PageInfo pageInfo);


	
	
}
