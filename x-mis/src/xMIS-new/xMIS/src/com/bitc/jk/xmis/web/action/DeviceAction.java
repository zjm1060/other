package com.bitc.jk.xmis.web.action;

import java.math.BigDecimal;

import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;

import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.DeviceSheetImportInfo;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleDeptObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.device.DeviceInObj;
import com.bitc.jk.xmis.model.device.DeviceInfo;
import com.bitc.jk.xmis.model.device.PurchaseRequisition;
import com.bitc.jk.xmis.model.device.PurchaseRequisitionDetailStatus;
import com.bitc.jk.xmis.model.device.PurchasingDetail;
import com.bitc.jk.xmis.model.device.PurchasingList;
import com.bitc.jk.xmis.model.sys.Business;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.MaterialsProperty;
import com.bitc.jk.xmis.model.sys.MaterialsUse;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.service.DeviceService;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;
import com.bitc.jk.xmis.web.response.ExtjsFormLoadResponse;
import com.bitc.jk.xmis.workflow.web.WorkItemParam;

public class DeviceAction extends DispatchAction {
	private static Logger logger = Logger.getLogger(DeviceAction.class);

	private DeviceService deviceService;

	/**
	 * 为采购申请界面（purchaseRequisition.jsp）加载Form数据
	 */
	public ActionForward loadPurchaseRequisition(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String requestId = request.getParameter("requestId");
		String projectId = request.getParameter("projectId");
		PurchaseRequisition purchaseRequisition = null;
		ExtjsFormLoadResponse formLoadResponse = new ExtjsFormLoadResponse();
		try {
			if (!StringUtils.isEmpty(requestId)) {
				purchaseRequisition = deviceService
						.getPurchaseRequisition(requestId);
			} else if (!StringUtils.isEmpty(projectId)) {
				String seldeviceids = request.getParameter("seldeviceids");
				purchaseRequisition = deviceService
						.getGeneratePurchaseRequisition(projectId, seldeviceids);
				purchaseRequisition.setEntryDate(DateUtil.getCurrentDateTime());
				EmployeeObj currEmp = WebUtil.getCurrentEmployee();
				purchaseRequisition.setEntryEmp(currEmp.clone());
			}
			formLoadResponse.setSuccess(true);
			formLoadResponse.setData(purchaseRequisition);
		} catch (Exception e) {
			formLoadResponse.setSuccess(false);
			logger.error(e.getMessage(), e);
		}
		WebUtil.returnResponse(response, formLoadResponse.toJsonString());
		return null;
	}

	public ActionForward loadPurchasingList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String purchasingId = request.getParameter("purchasingId");
		ExtjsFormLoadResponse formLoadResponse = new ExtjsFormLoadResponse();
		PurchasingList purchasingList = new PurchasingList();
		SimpleEmployee employee = WebUtil.getCurrentEmployee();
		try {
			if (purchasingId != null) {
				purchasingList = deviceService.getPurchasingList(purchasingId);
			} else {
				purchasingList.setEntryEmp(employee.clone());
				purchasingList.setEntryDate(new Date());
			}
			formLoadResponse.setSuccess(true);
			formLoadResponse.setData(purchasingList);
		} catch (Exception e) {
			formLoadResponse.setSuccess(false);
			logger.error(e.getMessage(), e);
		}
		WebUtil.returnResponse(response, formLoadResponse.toJsonString());
		return null;
	}

	/**
	 * 保存采购申请
	 */
	public ActionForward savePurchaseRequisition(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		PurchaseRequisition requisition = null;
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		String details = request.getParameter("detailstore");
		EmployeeObj emp = WebUtil.getCurrentEmployee();
		try {
			requisition = new PurchaseRequisition();
			BeanUtils.populate(requisition, request.getParameterMap());
			WorkItemParam param = new WorkItemParam();
			BeanUtils.populate(param, request.getParameterMap());
			if (StringUtils.isEmpty(param.getStep())) {
				requisition.setEntryDate(DateUtil.getCurrentDateTime());
				requisition.setEntryEmp(emp.clone());
				requisition.setDept(new SimpleDeptObj(emp.getDivisionID()));// 一级部门
				String processInstanceId = deviceService
						.doSavePurchaseRequisition(param, requisition, details);
				ajaxResponse.addAttribute("processInstanceId",
						processInstanceId);
			} else {
				if (request.getParameter("purchaser") != null) {
					param.addVariable("purchaser",
							request.getParameter("purchaser"));
				}
				deviceService
						.doPurchaseRequisition(param, requisition, details);
			}
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("提交成功！");
			ajaxResponse.addAttribute("requestId", requisition.getId());
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	/**
	 * 保存采购清单（包含采购明细）
	 */
	public ActionForward savePurchasingList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		PurchasingList purchasingList = null;
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		String details = request.getParameter("detailstore");
		EmployeeObj emp = WebUtil.getCurrentEmployee();
		try {
			purchasingList = new PurchasingList();
			BeanUtils.populate(purchasingList, request.getParameterMap());
			java.sql.Date currentDateTime = DateUtil.getCurrentDateTime();
			if (StringUtils.isEmpty(purchasingList.getId())) {
				purchasingList.setEntryDate(currentDateTime);
				purchasingList.setEntryEmp(emp.clone());
			}
			purchasingList.setLastModified(currentDateTime);
			purchasingList.setLastModifier(emp.clone());
			int compId = Type.GetInt(request.getParameter("compId"));
			purchasingList.setSupplier(new CompanySimpleObj(compId));
			deviceService.doSavePurchasingList(purchasingList, details);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功！");
			ajaxResponse.addAttribute("id", purchasingList.getId());
		} catch (Exception e) {
			logger.error("保存采购清单出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward getPurchasingList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		EmployeeObj emp = WebUtil.getCurrentEmployee();
		StandardQuery query = null;
		StandardQuery query1 = WebUtil.generateStandardQueryByInkling(request);
		StandardQuery query2 = SimpleParam.buildSimpleParam("dept.id",
				emp.getDivisionID(), Type.INTEGER, RelationalOperator.EQ);
		if (query1 == null) {
			query = query2;
		} else {
			query = CompositParam.buildAndCompositParam()
					.addStandardQuery(query1).addStandardQuery(query2);
		}

		String store = deviceService.getPurchasingListList(query, pageInfo);
		WebUtil.returnResponse(response, store);
		return null;
	}

	/**
	 * 查询项目ID
	 * */
	public void getRelationProjectID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		Integer contractId = Integer
				.valueOf(request.getParameter("contractId"));
		String store = deviceService.getRelationProjectID(contractId);
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 查询设备库信息
	 */
	public ActionForward loadDeviceInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String store = "";
		String show = request.getParameter("show");
		if (show != null) {
			return mapping.findForward("deviceInfoList");
		}

		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		store = deviceService.getDeviceInfoListStoreByQueryto(query, pageInfo);
		WebUtil.returnResponse(response, store);

		return null;
	}

	/**
	 * 查询采购申请列表
	 */
	public ActionForward getPurchaseRequisition(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String store = "";
		String type = request.getParameter("type");
		StandardQuery query = null;
		PageInfo pageInfo = new PageInfo();
		if ("all".equals(type)) { // 查询所有采购申请
			// 无需操作
		} else { // 默认按申请人查询
			SimpleEmployee employee = WebUtil.getCurrentEmployee();
			query = SimpleParam.buildSimpleParam("entryEmp.userID",
					employee.getUserID(), Type.INTEGER, RelationalOperator.EQ);
		}
		store = deviceService.getPurchaseRequisitionList(query, pageInfo);
		WebUtil.returnResponse(response, store);
		return null;
	}

	/**
	 * 获取待采购列表
	 */
	public ActionForward getTobePurchaseList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		EmployeeObj emp = WebUtil.getCurrentEmployee();
		PageInfo info = WebUtil.generatePageInfo(request);
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		CompositParam query2 = CompositParam
				.buildAndCompositParam()
				.addStandardQuery(
						SimpleParam.buildSimpleParam("status",
								PurchaseRequisitionDetailStatus.NEW, null,
								RelationalOperator.EQ))
				.addStandardQuery(
						SimpleParam.buildSimpleParam(
								"purchaseRequisition.dept.id",
								emp.getDivisionID(), Type.INTEGER,
								RelationalOperator.EQ));
		if (query != null) {
			query2.addStandardQuery(query);
		}
		String store = deviceService.getTobePurchaseList(query2, info);
		WebUtil.returnResponse(response, store);
		return null;
	}

	/**
	 * 查询采购清单明细
	 */
	public void getPurchasingListByOutContract(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		Integer contractId = Integer
				.valueOf(request.getParameter("contractId"));
		if ("".equals(contractId)) {
			WebUtil.returnResponse(response, null);
		} else {

			String store = deviceService
					.getPurchasingListByOutContract(contractId);
			WebUtil.returnResponse(response, store);
		}
	}

	/**
	 * 添加采购清单明细
	 * */
	public ActionForward addMaterialsPurchasingDetail(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String store = "";
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			Integer contractId = Integer.valueOf(request
					.getParameter("contractId"));// 支出合同ID
			Integer contractID = Integer.valueOf(request
					.getParameter("contractID"));// //项目ID
			String materialID = request.getParameter("materialID");// 物资ID
			Integer Purchasequantity = Integer.valueOf(request
					.getParameter("Purchasequantity"));// 采购数量
			float purchaseLimit = Float.valueOf(request
					.getParameter("purchaseLimit"));// 采购单价
			Date expectedarrivalDate = Type.getDate(request
					.getParameter("expectedarrivalDate"));// 期望到货日期
			String technicalClaim = request.getParameter("technicalClaim");// 技术要求
			String remarkName = request.getParameter("remarkName");// 备注
			Integer userID = Integer.valueOf(request.getParameter("userID"));// 登记人ID
			String registerName4 = request.getParameter("registerName4");// 登记人name
			Date updateDate3 = Type
					.getDate(request.getParameter("updateDate3"));// 登记日期

			Integer lastModifier = Integer.valueOf(request
					.getParameter("lastModifier"));// 最后修改人ID
			String registerName5 = request.getParameter("registerName5");// 最后修改人name
			Date updateDate6 = Type
					.getDate(request.getParameter("updateDate6"));// 最后修改时间
			PurchasingDetail pl = new PurchasingDetail();
			DeviceInfo dfo = new DeviceInfo();
			ContractObj cop = new ContractObj();
			SimpleEmployee see = new SimpleEmployee();

			dfo.setId(materialID);
			cop.setContractID(contractID);
			pl.setNumber(Purchasequantity);
			pl.setUnivalent(purchaseLimit);
			pl.setExpectArrivalDate(expectedarrivalDate);
			pl.setTechnicalRequirement(technicalClaim);
			pl.setRemark(remarkName);
			pl.setRegistrationDate(DateUtil.getCurrentDateTime());
			see.setUserID(userID);
			see.setUserName(registerName4);
			see.setUserID(lastModifier);
			see.setUserName(registerName5);
			pl.setLastModified(updateDate6);
			deviceService.doAddMaterialsPurchasingDetail(pl, dfo, cop,
					contractId, see);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("保存采购清单出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	/**
	 * 删除采购明细
	 * */
	public void deletePurchasingDetail(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		ExtjsAjaxResponse jsonStr = new ExtjsAjaxResponse();
		String purdelID = request.getParameter("purdelID");

		try {
			deviceService.doDeletePurchasingDetail(purdelID);
			jsonStr.setSuccess(true);
			jsonStr.setMessageTitle("成功");
			jsonStr.setMessage("删除成功!");
		} catch (Exception e) {
			jsonStr.setSuccess(false);
			jsonStr.setMessageTitle("失败");
			jsonStr.setMessage("删除失败!");
		}
		WebUtil.returnResponse(response, jsonStr.toJsonString());

	}

	public void getPurchasingDe(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String projectID = request.getParameter("projectID");
		Integer deviceID = Integer.valueOf(request.getParameter("deviceID"));
		String store = deviceService.getPurchasingDe(projectID, deviceID);
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 查询是否重复购买
	 * */
	public void getPurchasingDetaildouble(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String store = "";
		String contractID = request.getParameter("contractID");// //项目ID
		String materialID = request.getParameter("materialID");// 物资ID
		Integer contractId = Integer
				.valueOf(request.getParameter("contractId"));// 支出合同ID
		store = deviceService.getPurchasingDetaildouble(contractID, materialID,
				contractId);
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 查询选定项目已消费金额
	 * */
	public void getcontractSum(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = "";
		String valueField = request.getParameter("valueField");
		store = deviceService.getcontractSum(valueField);
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 修改采购明细
	 * */
	public void updatePurchasingDetail(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		ExtjsAjaxResponse jsonStr = new ExtjsAjaxResponse();
		try {
			String purdelID = request.getParameter("purdelID");
			String materialID = request.getParameter("materialID");// 物资ID
			Integer Purchasequantity = Integer.valueOf(request
					.getParameter("modifyPurchaseQuantity"));// 采购数量
			float purchaseLimit = Float.valueOf(request
					.getParameter("purchasePrice"));// 采购单价
			Date expectedarrivalDate = Type.getDate(request
					.getParameter("hopearrivalDate"));// 期望到货日期
			String technicalClaim = request.getParameter("technicalRequire");// 技术要求
			String remarkName = request.getParameter("modifyRemarksName");// 备注
			Integer userID = Integer.valueOf(request.getParameter("userID"));// 最后修改人ID
			String registerName5 = request.getParameter("registerName5");// 最后修改人name
			Date updateDate6 = Type
					.getDate(request.getParameter("updateDate6"));// 最后修改时间
			PurchasingDetail pl = new PurchasingDetail();
			DeviceInfo dfo = new DeviceInfo();
			ContractObj cop = new ContractObj();
			SimpleEmployee see = new SimpleEmployee();
			dfo.setId(materialID);
			pl.setId(purdelID);
			pl.setNumber(Purchasequantity);
			pl.setUnivalent(purchaseLimit);
			pl.setExpectArrivalDate(expectedarrivalDate);
			pl.setTechnicalRequirement(technicalClaim);
			pl.setRemark(remarkName);
			pl.setLastModified(updateDate6);
			see.setUserID(userID);
			see.setUserName(registerName5);

			deviceService.doUpdatePurchasingDetail(pl, dfo, see);
			jsonStr.setSuccess(true);
			jsonStr.setMessageTitle("成功");
			jsonStr.setMessage("修改成功!");
		} catch (Exception e) {
			jsonStr.setSuccess(false);
			jsonStr.setMessageTitle("失败");
			jsonStr.setMessage("修改失败!");
		}
		WebUtil.returnResponse(response, jsonStr.toJsonString());
	}

	/**
	 * 查询项目单价数量
	 * */
	public void getPurchasingDetailTotal(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String store = "";
		String contractID = request.getParameter("contractID");
		store = deviceService.getPurchasingDetailTotal(contractID);
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 查询物资采购明细与物资信息
	 * */
	public void getRecordpurdelID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = "";
		String purdelID = request.getParameter("purdelID");
		store = deviceService.getRecordpurdelID(purdelID);
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 添加物资信息
	 * */
	public ActionForward addMaterialInformation(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		ajaxResponse.setMessageTitle("提示");
		try {
			String id = request.getParameter("id");
			String materialName = request.getParameter("materiaName");
			String materialModel = request.getParameter("modelType");
			String materialStandard = request.getParameter("standard");
			String materialVolume = request.getParameter("volume");
			String powerSupplies = request.getParameter("power");
			String materialWeight = request.getParameter("weight");
			String factoryCommodityAddress = request
					.getParameter("factoryCommodityAddress");
			BigDecimal officialQuotation = new BigDecimal(
					request.getParameter("officialQuotation"));

			Integer materialsProperty = Integer.valueOf(request
					.getParameter("materialItself"));
			Integer businessProperty = Integer.valueOf(request
					.getParameter("materialsBusiness"));
			Integer materialsUse = Integer.valueOf(request
					.getParameter("materialUse"));
			String produceStatus = request.getParameter("produceStatus");
			Date stopTime = Type.getDate(request.getParameter("stopTime"));
			Date expectedarrivalDate = Type.getDate(request
					.getParameter("listingDate"));

			Integer currencyType = Integer.valueOf(request
					.getParameter("currencyType"));
			Integer materialsManufacturerID = Integer.valueOf(request
					.getParameter("materialsManufacturerID"));
			String pigName = request.getParameter("pigName");
			Date newupdateDate = Type.getDate(request
					.getParameter("newupdateDate"));
			DeviceInfo dfo = new DeviceInfo();
			Currency crr = new Currency();
			CompanyObj cob = new CompanyObj();
			MaterialsProperty mp = new MaterialsProperty();
			Business bs = new Business();
			MaterialsUse mu = new MaterialsUse();
			dfo.setId(id);
			dfo.setMateriaName(materialName);
			dfo.setModelType(materialModel);
			dfo.setStandard(materialStandard);
			dfo.setVolume(materialVolume);
			dfo.setPower(powerSupplies);
			dfo.setWeight(materialWeight);
			dfo.setReferencePrice(officialQuotation);
			dfo.setFactoryCommodityAddress(factoryCommodityAddress);
			bs.setId(businessProperty);
			dfo.setMaterialsBusiness(bs);
			mp.setId(materialsProperty);
			dfo.setMaterialItself(mp);
			mu.setId(materialsUse);
			dfo.setMaterialUse(mu);
			dfo.setProduceStatus(produceStatus);
			dfo.setStopTime(stopTime);
			dfo.setListingDate(expectedarrivalDate);
			dfo.setLastModified(DateUtil.getCurrentDateTime());
			dfo.setNewupdateDate(DateUtil.getCurrentDateTime());
			dfo.setNewregisterName(WebUtil.getCurrentEmployee());
			dfo.setLastModifier(WebUtil.getCurrentEmployee());
			dfo.setPigName(pigName);

			crr.setId(currencyType);
			dfo.setCurrency(crr);
			cob.setCompanyID(materialsManufacturerID);
			dfo.setCurrency(crr);
			dfo.setManufacturer(cob);
			dfo.setNewregisterName(WebUtil.getCurrentEmployee());

			deviceService.doSaveMaterialsInformation(dfo);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("id", dfo.getId());
			ajaxResponse.addAttribute("name", dfo.getMateriaName());
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("保存物资库出错。", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存错误！\n错误信息：" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;

	}

	/**
	 * 
	 * */
	public ActionForward getMaterialnfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String id = request.getParameter("id");
		DeviceInfo dfo = null;
		if (id != null && !"".equals(id.trim())) {
			try {
				dfo = deviceService.getMaterialnfoObjById(id);
				Map<String, String> map = deviceService
						.getMaterialsInfomation(id);
				request.setAttribute("pricemap", map);
			} catch (Exception e) {
				e.printStackTrace();
			}

		}
		request.setAttribute("dfo", dfo);
		return mapping.findForward("materialInformation");

	}

	/**
	 * 物资列表查询
	 * */
	public ActionForward getDeviceInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String show = request.getParameter("show");
		if (show != null) {
			return mapping.findForward("deviceInfoList");
		}

		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = "";
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		store = deviceService.getDeviceInfoListStoreByQuery(query, pageInfo);

		WebUtil.returnResponse(response, store);

		return null;
	}

	/**
	 * 查询最大供应商
	 * */
	public void getMaximumSupplier(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = "";

		if ("".equals(request.getParameter("materialsID"))) {
			WebUtil.returnResponse(response, null);
		}
		String materialsID = request.getParameter("materialsID");

		store = deviceService.getMaximumSupplier(materialsID);
		WebUtil.returnResponse(response, store);
	}

	/**
	 * 最多使用项目
	 * */
	public void getUseProject(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = "";

		if ("".equals(request.getParameter("materialsID"))) {
			WebUtil.returnResponse(response, null);
		}
		String materialsID = request.getParameter("materialsID");

		store = deviceService.getUseProject(materialsID);

		WebUtil.returnResponse(response, store);
	}

	public DeviceService getDeviceService() {
		return deviceService;
	}

	public void setDeviceService(DeviceService deviceService) {
		this.deviceService = deviceService;
	}

	/** 到库信息的保存、编辑 
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void saveDeviceIn(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		DeviceInObj deviceIn = new DeviceInObj();
		// BeanUtils.populate(deviceIn, request.getParameterMap());
		deviceIn.setId(request.getParameter("ID"));
		deviceIn.setProjectId(request.getParameter("projectId"));
		String outContractId = request.getParameter("outContractId") ;
		if(outContractId == null || "".equals(outContractId)){
			deviceIn.setOutContractId(null);
		}else{
			deviceIn.setOutContractId(Type.getInteger(request.getParameter("outContractId")));
		}
		
		deviceIn.setPurchasingListId(request.getParameter("purchasingListId"));
		deviceIn.setDeviceInfoId(request.getParameter("deviceInfoId"));
		deviceIn.setInNum(Type.getInteger(request.getParameter("inNum")));
		if (request.getParameter("inDate") != null
				&& !"".equals(request.getParameter("inDate"))) {
			deviceIn.setInDate(DateUtil.parseDate(
					request.getParameter("inDate"), "yyyy-MM-dd"));
		}

		deviceIn.setPositionId(request.getParameter("positionId"));
		deviceIn.setUseStypeId(Type.getInteger(request
				.getParameter("useStypeId")));
		if (request.getParameter("outFactoryDate") != null
				&& !"".equals(request.getParameter("outFactoryDate"))) {
			deviceIn.setOutFactoryDate(DateUtil.parseDate(
					request.getParameter("outFactoryDate"), "yyyy-MM-dd"));
		}
		String backDays = request.getParameter("backDays") ;
		if(backDays == null || "".equals(backDays)){
			deviceIn.setBackDays(null);
		}else{
			deviceIn.setBackDays(Type.getInteger(request.getParameter("backDays")));
		}
		
		String changeDays = request.getParameter("changeDays") ;
		if(changeDays == null || "".equals(changeDays)){
			deviceIn.setChangeDays(null);
		}else{
			deviceIn.setChangeDays(Type.getInteger(request
					.getParameter("changeDays")));
		}
		
		String repairDays = request.getParameter("repairDays") ;
		if(repairDays == null || "".equals(repairDays)){
			deviceIn.setRepairDays(null);
		}else{
			deviceIn.setRepairDays(Type.getInteger(request
					.getParameter("repairDays")));
		}
		
		if (request.getParameter("ID") == null
				|| "".equals(request.getParameter("ID"))) {
			deviceIn.setRegisterId(WebUtil.getCurrentEmployee().getUserID());
			deviceIn.setRegisterDate(DateUtil.getCurrentDateTime());
		} else {
			deviceIn.setRegisterId(Type.getInteger(request
					.getParameter("registerId")));
			deviceIn.setRegisterDate(DateUtil.parseDate(
					request.getParameter("registerDate"), "yyyy-MM-dd"));

		}
		deviceIn.setRemarks(request.getParameter("remarks"));
		deviceIn.setLastModifier(WebUtil.getCurrentEmployee());
		deviceIn.setLastModified(DateUtil.getCurrentDateTime());

		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			deviceService.doSaveDeviceIn(deviceIn);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("保存成功");
			ajaxResponse.setSuccess(true);
		} catch (Exception e) {
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作成功");
			ajaxResponse.setSuccess(false);
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/** 库存、货架、存储位置的下拉列表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getDeviceAddressCombo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String fid = request.getParameter("fid");
		String type = request.getParameter("type");
		String store = deviceService.getDeviceAddressCombo(fid, type);
		WebUtil.returnResponse(response, store);
	}

	/** 库存、货架、存储位置的新建
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void addDeviceAddress(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String fid = request.getParameter("fid");
		String type = request.getParameter("type");
		String name = request.getParameter("name");
		String code = request.getParameter("code");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();

		try {
			deviceService.doAddDeviceAddress(type, fid, name, code);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("保存成功");
			ajaxResponse.setSuccess(true);
		} catch (Exception e) {
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作失败");
			ajaxResponse.setSuccess(false);
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/** 根据项目ID得到项目物资清单
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getPorjectDeviceInfoList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String projectId = request.getParameter("projectId");
		String store = deviceService.getPorjectDeviceInfoList(projectId);
		WebUtil.returnResponse(response, store);
	}

	/** 已到货的详细信息列表 暂时未用（第一版到货登记页面的到货详情）
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getArrivalDeviceInfos(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String contractId = request.getParameter("contractId");
		String deviceInfoId = request.getParameter("deviceInfoId");
		String store = deviceService.getArrivalDeviceInfos(contractId,
				deviceInfoId);
		WebUtil.returnResponse(response, store);
	}

	/** 到货登记页面的列表 暂时未用（第一版到货登记页面）
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getDeviceInList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = deviceService.getDeviceInList(query, pageInfo);
		WebUtil.returnResponse(response, store);
	}

	/** 物资信息列表（到货信息保存页面 ）
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getDeviceInfoList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = deviceService.getDeviceInfoList(query, pageInfo);
		WebUtil.returnResponse(response, store);
	}

	/** 根据合同ID得到其子合同的列表 含有采购清单ID、收入合同ID
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getDeviceInfoListWithPurchaseList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String contractId = request.getParameter("contractId");
		String store = deviceService
				.getDeviceInfoListWithPurchaseList(contractId);
		WebUtil.returnResponse(response, store);
	}

	/** 领用级别下拉框
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getUseStypeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String store = deviceService.getUseStypeList();
		WebUtil.returnResponse(response, store);
	}

	/** 到库清单、到货登记列表（第二版）
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public ActionForward getDeviceInListByContractId(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String show = request.getParameter("show");
		if (show != null) {
			return mapping.findForward("deviceInList");
		}
		
		String contractId = request.getParameter("contractId");
		CompositParam querys = CompositParam.buildAndCompositParam();
		// 获取权限定义
		List<StandardQuery> querysAuthority = (List<StandardQuery>) request
				.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);// 权限查询

		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		if (contractId != null && !"".equals(contractId)) {
			querys = CompositParam
					.buildAndCompositParam()
					.addStandardQuery(
							SimpleParam.buildSimpleParam("合同ID", contractId,
									Type.STRING, RelationalOperator.EQ));
		} else {
			if(querysAuthority!=null){
				querys.addStandardQuery(querysAuthority);
			}
		}
		if(query!=null){
			querys = querys.addStandardQuery(query);
		}
		querys = querys.addStandardQuery(query);
		String store = deviceService.getDeviceInListByContractId(querys,
				pageInfo);
		WebUtil.returnResponse(response, store);
		return null;
	}

	/** 根据ID得到一条到库信息
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getDeviceInInfoByID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String id = request.getParameter("id");
		String store = deviceService.getDeviceInInfoByID(id);
		WebUtil.returnResponse(response, store);
	}

	/** 到库清单的删除
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void delDeviceInObjByID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String id = request.getParameter("id");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();

		try {
			deviceService.delDeviceInObjByID(id);
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("删除成功");
			ajaxResponse.setSuccess(true);
		} catch (Exception e) {
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("删除失败");
			ajaxResponse.setSuccess(false);
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	/** 物资存放地址信息列表、项目物资清单上的存放地址列表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getDeviceAddressList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String projectId = request.getParameter("projectId");
		String deviceInfoId = request.getParameter("deviceInfoId");
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		
		CompositParam querys = CompositParam.buildAndCompositParam();
		// 获取权限定义
		List<StandardQuery> querysAuthority = (List<StandardQuery>) request
				.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);// 权限查询
		
		if (projectId != null && !"".equals(projectId) && deviceInfoId != null
				&& !"".equals(deviceInfoId)) {
			querys = CompositParam
					.buildAndCompositParam()
					.addStandardQuery(
							SimpleParam.buildSimpleParam("项目ID", projectId,
									Type.STRING, RelationalOperator.EQ))
					.addStandardQuery(
							SimpleParam.buildSimpleParam("物资ID", deviceInfoId,
									Type.STRING, RelationalOperator.EQ));
		}  else{
			
			if(querysAuthority!=null){
				querys.addStandardQuery(querysAuthority);
			}
		}
		
		if(query!=null){
			querys = querys.addStandardQuery(query);
		}
		String store = deviceService.getDeviceAddressList(querys, pageInfo);
		WebUtil.returnResponse(response, store);
	}

	/** 根据收入合同ID 得到对应的子合同列表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getContactListByProjectIdCombo(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		String fid = request.getParameter("projectId");
		String store = deviceService.getContactListByProjectIdCombo(fid);
		WebUtil.returnResponse(response, store);
	}

	/** 已采购的数量，用于到货校验
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getPurchaseSumNum(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String purchaseListId = request.getParameter("purchaseListId");
		String deviceInfoId = request.getParameter("deviceInfoId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			int num = deviceService.getPurchaseSumNum(deviceInfoId,
					purchaseListId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("num", num);
		} catch (Exception e) {
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作失败");
			ajaxResponse.setSuccess(false);
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	/** 到货页面期限的计算
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getDeadlineByDateAndDays(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		Date rescDate = Type.getDate(request.getParameter("rescDate"));
		int days = Type.GetInt(request.getParameter("days"));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try { 
			Calendar calendar = Calendar.getInstance();
			calendar.setTime(rescDate);
			calendar.add(Calendar.DAY_OF_MONTH,days);
			Date r =  new Date(calendar.getTimeInMillis());
			String result = DateUtil.formatDate(r, "yyyy-MM-dd");
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("result", result);
		} catch (Exception e) {
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.setMessage("操作失败");
			ajaxResponse.setSuccess(false);
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	

	/**  导入的设备信息列表
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getImportDeviceInfoList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		String projectId = request.getParameter("projectId");
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		if (projectId != null && !"".equals(projectId)) {
			query = CompositParam
					.buildAndCompositParam()
					.addStandardQuery(
							SimpleParam.buildSimpleParam("projectId", projectId,
									Type.STRING, RelationalOperator.EQ));
		}
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = deviceService.getImportDeviceInfoList(query, pageInfo);
		WebUtil.returnResponse(response, store);
	}

}
