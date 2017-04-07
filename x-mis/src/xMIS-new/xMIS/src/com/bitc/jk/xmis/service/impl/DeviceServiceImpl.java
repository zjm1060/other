package com.bitc.jk.xmis.service.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import net.sf.ezmorph.object.DateMorpher;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;
import net.sf.json.util.JSONUtils;

import org.apache.commons.lang.StringUtils;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.RuntimeContext;
import org.springframework.beans.BeanUtils;

import com.bitc.jk.xmis.dao.DeviceDAO;
import com.bitc.jk.xmis.dao.ProjectDAO;
import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.ProjectDevicePurchaseStatus;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.device.DeviceInObj;
import com.bitc.jk.xmis.model.device.DeviceInfo;
import com.bitc.jk.xmis.model.device.PurchaseRequisition;
import com.bitc.jk.xmis.model.device.PurchaseRequisitionDetail;
import com.bitc.jk.xmis.model.device.PurchaseRequisitionDetailStatus;
import com.bitc.jk.xmis.model.device.PurchasingDetail;
import com.bitc.jk.xmis.model.device.PurchasingList;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.Power;
import com.bitc.jk.xmis.model.sys.Weight;
import com.bitc.jk.xmis.service.DeviceService;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.WorkflowConstants;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.util.WorkflowUtil;
import com.bitc.jk.xmis.workflow.web.WorkItemParam;

public class DeviceServiceImpl implements DeviceService {

	private ProjectDAO projectDAO;
	private DeviceDAO deviceDAO;
	private RuntimeContext workflowContext;

	public RuntimeContext getWorkflowContext() {
		return workflowContext;
	}

	public void setWorkflowContext(RuntimeContext aWorkflowContext) {
		this.workflowContext = aWorkflowContext;
	}

	public PurchaseRequisition getGeneratePurchaseRequisition(String projectId,
			String seldeviceids) {
		PurchaseRequisition purchaseRequisition = new PurchaseRequisition();
		ProjectObj projectObj = projectDAO.getProjectInfoByID(projectId);
		purchaseRequisition.setProjectId(projectId);
		purchaseRequisition.setProjectCode(projectObj.getProjectCode());
		purchaseRequisition.setProjectName(projectObj.getProjectName());
		List<DeviceObj> projectDevices = projectDAO
				.getDeviceListByID(projectId);
		purchaseRequisition.generateDetails(projectDevices, seldeviceids);
		return purchaseRequisition;
	}

	public ProjectDAO getProjectDAO() {
		return projectDAO;
	}

	public void setProjectDAO(ProjectDAO projectDAO) {
		this.projectDAO = projectDAO;
	}

	public String getDeviceInfoListStoreByQuery(StandardQuery query,
			PageInfo pageInfo) throws Exception {
		List listStore = deviceDAO.getDeviceInfoListStoreByQuery(query, pageInfo);
		return WebUtil.buildStoreForJson(listStore, "");
	}

	public DeviceDAO getDeviceDAO() {
		return deviceDAO;
	}

	public void setDeviceDAO(DeviceDAO deviceDAO) {
		this.deviceDAO = deviceDAO;
	}

	public String doSavePurchaseRequisition(WorkItemParam param,
			PurchaseRequisition requisition, String details) throws Exception {

		JSONArray detailArray = JSONArray.fromObject(details);
		for (int i = 0; i < detailArray.size(); i++) {
			JSONObject object = detailArray.getJSONObject(i);
			JSONUtils.getMorpherRegistry().registerMorpher(
					new DateMorpher(new String[] { "yyyy-MM-dd HH:mm:ss" }));
			PurchaseRequisitionDetail detail = (PurchaseRequisitionDetail) JSONObject
					.toBean(object, PurchaseRequisitionDetail.class);
			requisition.addDetail(detail);
		}
		deviceDAO.savePurchaseRequisition(requisition);
		double amt = 0;// 预计采购总额
		for (Iterator<PurchaseRequisitionDetail> it = requisition.getDetails()
				.iterator(); it.hasNext();) {
			PurchaseRequisitionDetail d = it.next();
			ProjectDevicePurchaseStatus purchaseStatus = projectDAO
					.getProjectDevicePurchaseStatusByPrdId(d.getId());
			if (purchaseStatus == null) {
				purchaseStatus = new ProjectDevicePurchaseStatus();
			}
			purchaseStatus.setNumber(d.getNumber());
			purchaseStatus.setProjectDeviceId(d.getProjectDeviceId());
			purchaseStatus.setPurchaseRequisitionDetailId(d.getId());
			purchaseStatus
					.setStatus(ProjectDevicePurchaseStatus.STATUS_REQUEST);
			amt += d.getReferencePrice() * d.getNumber();
			projectDAO.saveProjectDevicePurchaseStatus(purchaseStatus);
		}

		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		SimpleEmployee emp = WebUtil.getCurrentEmployee();
		String empId = String.valueOf(emp.getUserID());
		IProcessInstance processInstance = WorkflowUtil.startProcess(
				workflowSession, WorkflowConstants.WF_PURCHASE_REQUISITION,
				empId);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "requestId",
				requisition.getId());
		WorkflowUtil.addProcessInstanceVariable(processInstance, "deptId",
				requisition.getDept().getDepartmentID());
		WorkflowUtil.addProcessInstanceVariable(processInstance, "creatId",
				empId);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "amt", amt);
		ProjectObj project = projectDAO.getProjectInfoByID(requisition
				.getProjectId());
		int managerId = project.getManager().getUserID();
		WorkflowUtil.addProcessInstanceVariable(processInstance, "managerId",
				String.valueOf(managerId));
		WorkflowUtil.completeFirstTask(workflowSession, processInstance, empId,
				param.getComments());
		return processInstance.getId();
	}

	public void doSavePurchasingList(PurchasingList purchasingList,
			String details) throws Exception {
		if (StringUtils.isNotEmpty(purchasingList.getId())) {
			PurchasingList purchasingList2 = deviceDAO
					.getPurchasingList(purchasingList.getId());
			com.bitc.jk.xmis.util.BeanUtils.copyNotNullProperties(
					purchasingList2, purchasingList);
			purchasingList = purchasingList2;
		}
		JSONArray detailArray = JSONArray.fromObject(details);
		for (int i = 0; i < detailArray.size(); i++) {
			JSONObject object = detailArray.getJSONObject(i);
			JSONUtils.getMorpherRegistry().registerMorpher(
					new DateMorpher(new String[] { "yyyy-MM-dd HH:mm:ss" }));
			PurchasingDetail detail = (PurchasingDetail) JSONObject.toBean(
					object, PurchasingDetail.class);
			PurchaseRequisitionDetail prDetail = deviceDAO
					.getPurchaseRequisitionDetail(detail
							.getRequisitionDetailId());
			prDetail.setStatus(PurchaseRequisitionDetailStatus.PURCHASING);
			PurchaseRequisition pr = prDetail.getPurchaseRequisition();
			detail.setDept(pr.getDept());
			detail.setApplicant(pr.getEntryEmp());
			purchasingList.addDetail(detail);
			detail.setPurchasing(purchasingList);
		}
		deviceDAO.savePurchasingList(purchasingList);
	}

	public PurchaseRequisition getPurchaseRequisition(String requestId) {
		PurchaseRequisition purchaseRequisition = deviceDAO
				.getPurchaseRequisition(requestId);
		if (!StringUtils.isEmpty(purchaseRequisition.getProjectId())) {
			ProjectObj project = projectDAO
					.getProjectInfoByID(purchaseRequisition.getProjectId());
			if (project != null) {
				purchaseRequisition.setProjectCode(project.getProjectCode());
				purchaseRequisition.setProjectName(project.getProjectName());
			}
		}
		return purchaseRequisition;
	}

	public String getPurchaseRequisitionList(StandardQuery query,
			PageInfo pageInfo) {
		List<PurchaseRequisition> list = deviceDAO
				.getPurchaseRequisitionByQuery(query, pageInfo);
		for (Iterator<PurchaseRequisition> it = list.iterator(); it.hasNext();) {
			PurchaseRequisition pr = it.next();
			if (StringUtils.isEmpty(pr.getProjectId())) {
				continue;
			}
			ProjectObj project = projectDAO.getProjectInfoByID(pr
					.getProjectId());
			if (project != null) {
				pr.setProjectCode(project.getProjectCode());
				pr.setProjectName(project.getProjectName());
			}
		}
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getTobePurchaseList(StandardQuery query, PageInfo info) {
		List<PurchaseRequisitionDetail> list = deviceDAO.getTobePurchaseList(
				query, info);
		int count = deviceDAO.getTobePurchaseListCount(query);
		List<PurchasingDetail> list2 = new ArrayList<PurchasingDetail>();
		for (int i = 0; i < list.size(); i++) {
			PurchaseRequisitionDetail prd = list.get(i);
			PurchasingDetail detail = new PurchasingDetail();
			try {
				BeanUtils.copyProperties(prd, detail);
				detail.setId(null);
				PurchaseRequisition pr = prd.getPurchaseRequisition();
				ProjectObj project = projectDAO.getProjectInfoByID(pr
						.getProjectId());
				if (project != null) {
					// detail.setProjectObj(project);
					// detail.setProjectCode(project.getProjectCode());
					// detail.setProjectName(project.getProjectName());
				}
				detail.setRequisitionDetailId(prd.getId());
				detail.setRequisitionId(pr.getId());
				detail.setDept(pr.getDept());
				detail.setApplicant(pr.getEntryEmp());
				list2.add(detail);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		return WebUtil.buildJsonStoreByList(list2, count);
	}

	public PurchasingList getPurchasingList(String purchasingId) {
		return deviceDAO.getPurchasingList(purchasingId);
	}

	public void doPurchaseRequisition(WorkItemParam workItemParam,
			PurchaseRequisition requisition, String details) throws Exception {
		IWorkflowSession wflsession = workflowContext.getWorkflowSession();
		EmployeeObj emp = WebUtil.getCurrentEmployee();
		IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
				wflsession, workItemParam.getWorkItemId());
		WorkflowUtil.addProcessInstanceVariables(processInstance,
				workItemParam.getVariables());
		if ("PurchaseRequisition.ProjectManagerApproval.Approve"
				.equals(workItemParam.getStep())
				|| "PurchaseRequisition.SupervisorApproval.Task"
						.equals(workItemParam.getStep())
				|| "PurchaseRequisition.Requition.Task".equals(workItemParam
						.getStep())) {
			PurchaseRequisition purchaseRequisition = deviceDAO
					.getPurchaseRequisition(requisition.getId());
			JSONArray detailArray = JSONArray.fromObject(details);
			double amt = 0;// 预计采购总额
			for (int i = 0; i < detailArray.size(); i++) {
				JSONObject object = detailArray.getJSONObject(i);
				JSONUtils
						.getMorpherRegistry()
						.registerMorpher(
								new DateMorpher(
										new String[] { "yyyy-MM-dd HH:mm:ss" }));
				PurchaseRequisitionDetail detail = (PurchaseRequisitionDetail) JSONObject
						.toBean(object, PurchaseRequisitionDetail.class);
				amt += detail.getReferencePrice() * detail.getNumber();
				detail.setPurchaseRequisition(purchaseRequisition);
				deviceDAO.savePurchaseRequisitionDetail(detail);
			}
			purchaseRequisition.setRemark(requisition.getRemark());
			deviceDAO.savePurchaseRequisition(purchaseRequisition);
			WorkflowUtil
					.addProcessInstanceVariable(processInstance, "amt", amt);
		} else if ("PurchaseRequisition.CompPurchaseAssign.Task"
				.equals(workItemParam.getStep())) {
		} else if ("PurchaseRequisition.DeptPurchaseAssign.Task"
				.equals(workItemParam.getStep())) {
		}
		WorkflowUtil.completeTask(wflsession, workItemParam.getWorkItemId(),
				String.valueOf(emp.getUserID()), workItemParam.getComments());
	}

	public String getPurchasingListList(StandardQuery query, PageInfo pageInfo) {
		List<PurchasingList> purchasingList = deviceDAO.getPurchasingListList(
				query, pageInfo);
		int count = 0;
		if (pageInfo.getLimit() == 0) {
			count = purchasingList.size();
		} else {
			count = deviceDAO.getPurchasingListListCount(query);
		}
		return WebUtil.buildJsonStoreByList(purchasingList, count);
	}

	
	public void doSaveDeviceIn(DeviceInObj deviceIn)  {
		if (deviceIn.getId() == null || "".equals(deviceIn.getId())) {
			try {
				deviceDAO.doSaveDeviceIn(deviceIn);
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else {
			deviceDAO.doUpdateDeviceIn(deviceIn);
		}
	}

	
	public String getDeviceAddressCombo(String fid, String type) {
		List list = null;
		if ("house".equals(type)) {
			list = deviceDAO.getWareHouseCombo(fid);
		}
		if ("shelf".equals(type)) {
			list = deviceDAO.getShelfCombo(fid);
		}
		if ("position".equals(type)) {
			list = deviceDAO.getPositionCombo(fid);
		}
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	
	public String getPorjectDeviceInfoList(String projectId) {
		List list = deviceDAO.getPorjectDeviceInfoList(projectId);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	
	public String getDeviceInList(StandardQuery query, PageInfo pageInfo) {
		List list = deviceDAO.getDeviceInList(query, pageInfo);
		return WebUtil.buildStoreForJson(list, "");
	}

	
	public void doAddDeviceAddress(String type, String fid, String name,
			String code) {
		if ("house".equals(type)) {
			deviceDAO.doAddWareHouseCombo(fid, name, code);
		}
		if ("shelf".equals(type)) {
			String uid = deviceDAO.doAddShelfCombo(fid, name, code);
			deviceDAO.doAddPositionCombo(uid, WebKeys.POSITION_CODE,WebKeys.POSITION_NAME);
		}
		if ("position".equals(type)) {
			deviceDAO.doAddPositionCombo(fid, name, code);
		}

	}

	
	public String getArrivalDeviceInfos(String contractId, String deviceInfoId) {
		List list = deviceDAO.getArrivalDeviceInfos(contractId, deviceInfoId);

		return WebUtil.buildStoreForJsonWithTotalInfo(list, "");
	}

	public String getPurchasingListByOutContract(Integer contractId) {
		List list = deviceDAO.getPurchasingListByOutContract(contractId);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getRelationProjectID(Integer contractId) {
		List list = deviceDAO.getRelationProjectID(contractId);
		return WebUtil.buildJsonStoreByList(list);
	}

	/**
 * 
 * */
	public void doAddMaterialsPurchasingDetail(PurchasingDetail pl,
			DeviceInfo dfo, ContractObj cop, Integer contractId,
			SimpleEmployee see) {

		String message_id = UUIDUtils.nextCode();
		PurchasingList purchasingList = new PurchasingList();

		purchasingList.setId(message_id);
		purchasingList.setContractId(contractId);
		String s = deviceDAO.getMaterialsPurchasingList(contractId);
		if (s == null) {
			deviceDAO.addMaterialsPurchasingList(purchasingList);
			s = deviceDAO.getMaterialsPurchasingList(contractId);
		} else {
		}
		deviceDAO
				.addMaterialsPurchasingDetail(message_id, s, pl, dfo, cop, see);

	}

	public void doDeletePurchasingDetail(String purdelID) {
		// TODO Auto-generated method stub
		deviceDAO.doDeletePurchasingDetail(purdelID);
	}

	/**
	 * 查询是否有重复购买
	 * */
	public String getPurchasingDetaildouble(String contractID,
			String materialID, Integer contractId) {
		// TODO Auto-generated method stub
		List list = deviceDAO.getPurchasingDetaildouble(contractID, materialID,
				contractId);

		return WebUtil.buildJsonStoreByList(list);
	}

	public String getPurchasingDe(String projectID, Integer deviceID) {
		// TODO Auto-generated method stub
		List list = deviceDAO.getPurchasingDe(projectID, deviceID);
		return WebUtil.buildJsonStoreByList(list);
	}

	/**
	 * 查询选定项目已消费金额
	 * */
	public String getcontractSum(String valueField) {
		// TODO Auto-generated method stub
		List list = deviceDAO.getcontractSum(valueField);
		return WebUtil.buildJsonStoreByList(list);
	}

	/**
	 * 修改采购清单明细
	 * */
	public void doUpdatePurchasingDetail(PurchasingDetail pl, DeviceInfo dfo,
			SimpleEmployee see) {
		// TODO Auto-generated method stub
		deviceDAO.doUpdatePurchasingDetail(pl, dfo, see);
	}

	/**
	 * 查询项目单价数量
	 * */
	public String getPurchasingDetailTotal(String contractID) {
		List list = deviceDAO.getPurchasingDetailTotal(contractID);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getRecordpurdelID(String purdelID) {
		// TODO Auto-generated method stub
		List list = deviceDAO.getRecordpurdelID(purdelID);
		return WebUtil.buildJsonStoreByList(list);
	}

	public void doSaveMaterialsInformation(DeviceInfo dfo) {
		if ("".equals(dfo.getId())) {
			dfo.setId(null);
		}

		deviceDAO.seveMaterialsInformation(dfo);
	}

	public Map<String, String> getMaterialsInfomation(String id) {
		List l =  deviceDAO.getMateerials(id);
		Map<String,String> map=new HashMap<String,String>();
		for(int i=0;i<l.size();i++){
			Map row=(Map)l.get(i);
			if(row.get("t").equals("newest")){
				map.put("lastPrice",row.get("单价").toString());
				map.put("lastPriceTime",row.get("登记日期").toString());
				map.put("lastPricePurchaseID", row.get("支出合同ID").toString());
				map.put("lastPricePurchaseCode", row.get("合同编号").toString());
			}else if(row.get("t").equals("lowest")){
				map.put("lowestPrices",row.get("单价").toString());
				map.put("lowestPricesTime",row.get("登记日期").toString());
				map.put("lowestPricesPurchaseID", row.get("支出合同ID").toString());
				map.put("lowestPricesPurchaseCode", row.get("合同编号").toString());
			}
		}
		return map;
	}



	public DeviceInfo getMaterialnfoObjById(String id) {
		DeviceInfo dfo = deviceDAO.getMaterialnfoObjById(id);
		return dfo;
	}

	public void seveMaterialsInformation(DeviceInfo dfo, Currency crr,
			CompanyObj cob, Power pr, Weight wt) {

	}

	public String getDeviceInfoListStoreByQueryto(StandardQuery query,
			PageInfo pageInfo) {
		List<DeviceInfo> devices = deviceDAO.getDeviceInfoListByQuery(query,
				pageInfo);
		int totalCount = deviceDAO.getDeviceInfoCountByQuery(query, pageInfo);
		return WebUtil.buildJsonStoreByList(devices, totalCount);
	}

	public String getMaximumSupplier(String materialsID) {
		List list = deviceDAO.getMaximumSupplier(materialsID);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getUseProject(String materialsID) {
		List list = deviceDAO.getUseProject(materialsID);
		return WebUtil.buildJsonStoreByList(list);
	}

	
	public String getDeviceInfoList(StandardQuery query, PageInfo pageInfo) {
		List list = deviceDAO.getDeviceInfoList(query, pageInfo);

		return WebUtil.buildStoreForJson(list, "");
	}
	public String getImportDeviceInfoList(StandardQuery query, PageInfo pageInfo) {
		List list = deviceDAO.getImportDeviceInfoList(query, pageInfo);
		
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	
	public String getUseStypeList() {
		List list = deviceDAO.getUseStypeList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	public String getDeviceInListByContractId(StandardQuery query, PageInfo pageInfo) {
		List list = deviceDAO.getDeviceInListByContractId(query,pageInfo);
		return WebUtil.buildStoreForJson(list, "");
	}

	public String getDeviceInInfoByID(String id) {
		List list = deviceDAO.getDeviceInInfoByID(id);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	public void delDeviceInObjByID(String id) {
		deviceDAO.delDeviceInObjByID(id);
	}

	public String getDeviceAddressList(StandardQuery query, PageInfo pageInfo) {
		List list = deviceDAO.getDeviceAddressList(query,pageInfo);
		return WebUtil.buildStoreForJson(list, "");
	}

	
	public String getDeviceInfoListWithPurchaseList(String contractId) {
		List list = deviceDAO.getDeviceInfoListWithPurchaseList(contractId);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	
	public String getContactListByProjectIdCombo(String fid) {
		List list = deviceDAO.getContactListByProjectIdCombo(fid);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	
	public int getPurchaseSumNum(String fid, String purchaseListId) {
		List list = deviceDAO.getPurchaseSumNum(fid,purchaseListId);
		int num = 0;
		if(list !=null && list.size()>0&&((Map) list.get(0)).get("num")!=null){
			num = Type.GetInt(((Map) list.get(0)).get("num").toString());
		}
		return num;
	}


}
