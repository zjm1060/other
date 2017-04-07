package com.bitc.jk.xmis.dao;
import java.util.List;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.device.DeviceInObj;
import com.bitc.jk.xmis.model.device.DeviceInfo;
import com.bitc.jk.xmis.model.device.PurchaseRequisition;
import com.bitc.jk.xmis.model.device.PurchaseRequisitionDetail;
import com.bitc.jk.xmis.model.device.PurchasingDetail;
import com.bitc.jk.xmis.model.device.PurchasingList;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface DeviceDAO {

	List<DeviceInfo> getDeviceInfoListByQuery(StandardQuery query,
			PageInfo pageInfo);

	int getDeviceInfoCountByQuery(StandardQuery query, PageInfo pageInfo);

	void savePurchaseRequisition(PurchaseRequisition requisition);

	PurchaseRequisition getPurchaseRequisition(String requestId);

	List<PurchaseRequisition> getPurchaseRequisitionByQuery(
			StandardQuery query, PageInfo pageInfo);

	/**
	 * 通过项目设备保存设备信息库信息 。 按设备名称与型号判断是否已经存在
	 * 
	 * @param device
	 */
	void saveDeviceInfo(DeviceObj device);

	List<PurchaseRequisitionDetail> getTobePurchaseList(StandardQuery query,
			PageInfo info);

	int getTobePurchaseListCount(StandardQuery query);

	PurchasingList getPurchasingList(String purchasingId);

	void savePurchasingList(PurchasingList purchasingList);

	PurchaseRequisitionDetail getPurchaseRequisitionDetail(
			String requisitionDetailId);

	void savePurchaseRequisitionDetail(PurchaseRequisitionDetail detail);

	List<PurchasingList> getPurchasingListList(StandardQuery query,
			PageInfo pageInfo);

	int getPurchasingListListCount(StandardQuery query);

	List<Object[]> getPurchasingListByOutContract(Integer contractId);

	List<Object[]> getRelationProjectID(Integer contractId);

	void addMaterialsPurchasingList(PurchasingList purchasingList);

	String getMaterialsPurchasingList(Integer contractId);

	void addMaterialsPurchasingDetail(String message_id, String s,
			PurchasingDetail pl, DeviceInfo dfo, ContractObj cop,
			SimpleEmployee see);

	void doDeletePurchasingDetail(String purdelID);

	List getPurchasingDetaildouble(String contractID, String materialID,
			Integer contractId);

	List getPurchasingDe(String projectID, Integer deviceID);

	List getcontractSum(String valueField);

	void doUpdatePurchasingDetail(PurchasingDetail pl, DeviceInfo dfo,
			SimpleEmployee see);

	List getPurchasingDetailTotal(String contractID);

	List getRecordpurdelID(String purdelID);

	List getMaterialsInfomation(String id);

	void seveMaterialsInformation(DeviceInfo dfo);

	DeviceInfo getMaterialnfoObjById(String id);

	List getMateerials(String id);

	List getDeviceInfoListStoreByQuery(StandardQuery query, PageInfo pageInfo) throws Exception;

	List getMaximumSupplier(String materialsID);

	List getUseProject(String materialsID);
	
	/** 到库信息的保存
	 */ 
	void doSaveDeviceIn(DeviceInObj deviceIn);

	List getWareHouseCombo(String fid);

	List<?> getPorjectDeviceInfoList(String projectId);

	List getDeviceInList(StandardQuery query, PageInfo pageInfo);

	List getShelfCombo(String fid);

	List getPositionCombo(String fid);

	void doAddWareHouseCombo(String fid, String name, String managerID);

	String doAddShelfCombo(String fid, String name, String code);

	void doAddPositionCombo(String fid, String name, String code);

	List getArrivalDeviceInfos(String contractId, String deviceInfoId);

	List getDeviceInfoList(StandardQuery query, PageInfo pageInfo);

	List getUseStypeList();

	List getDeviceInListByContractId(StandardQuery query, PageInfo pageInfo);

	List getDeviceInInfoByID(String id);

	void delDeviceInObjByID(String id);

	/** 到库信息的编辑 
	 */ 
	void doUpdateDeviceIn(DeviceInObj deviceIn);

	List getDeviceAddressList(StandardQuery query, PageInfo pageInfo);

	List getDeviceInfoListWithPurchaseList(String contractId);

	List getContactListByProjectIdCombo(String fid);

	List getPurchaseSumNum(String deviceInfoId,String purchaseListId);

	List getImportDeviceInfoList(StandardQuery query, PageInfo pageInfo);


}
