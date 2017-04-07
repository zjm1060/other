//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\Impl\\ContractServiceImpl.java

package com.bitc.jk.xmis.service.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;
import net.sf.json.JsonConfig;
import net.sf.json.util.CycleDetectionStrategy;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;

import com.bitc.jk.xmis.dao.ContractDAO;
import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.dao.ProjectDAO;
import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.ContractInfoObj;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.DelayReason;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.EstimatedPayment;
import com.bitc.jk.xmis.model.GContractSourceObj;
import com.bitc.jk.xmis.model.InvoiceObj;
import com.bitc.jk.xmis.model.InvoiceSplitObj;
import com.bitc.jk.xmis.model.MoneySplitObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PaymentConditionObj;
import com.bitc.jk.xmis.model.PaymentDetailObj;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.ProjectStatus;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.Stencil;
import com.bitc.jk.xmis.service.ContractService;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.JacksonUtil;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;

import edu.emory.mathcs.backport.java.util.Collections;

public class ContractServiceImpl implements ContractService {
	private static Logger logger = Logger.getLogger(ContractServiceImpl.class);

	private ContractDAO contractDAO;

	private OrganizationDAO organizationDAO;

	private ProjectDAO projectDAO;

	/**
	 * @roseuid 4C1B2D240232
	 */
	public ContractServiceImpl() {

	}

	/**
	 * Access method for the contractDAO property.
	 * 
	 * @return the current value of the contractDAO property
	 */
	public ContractDAO getContractDAO() {
		return contractDAO;
	}

	/**
	 * Sets the value of the contractDAO property.
	 * 
	 * @param aContractDAO
	 *            the new value of the contractDAO property
	 */
	public void setContractDAO(ContractDAO aContractDAO) {
		contractDAO = aContractDAO;
	}

	/**
	 * @return the organizationDAO
	 */
	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}

	/**
	 * @param organizationDAO
	 *            the organizationDAO to set
	 */
	public void setOrganizationDAO(OrganizationDAO organizationDAO) {
		this.organizationDAO = organizationDAO;
	}

	/**
	 * @return List
	 * @roseuid 4C1B2D240251
	 */
	public List getUnfinishedContractList(EmployeeObj userObj) {
		return null;
	}

	/**
	 * @param contractID
	 * @return com.bitc.jk.xmis.model.ContractObj
	 * @roseuid 4C1B2D240290
	 */
	public ContractObj getContractInfoByID(int contractID) {

		ContractObj con = new ContractObj();

		try {

			Map map = contractDAO.getContractInfo(contractID);
			logger.info("合同map：" + map);
			con.setContractID(Type.GetInt(map.get(DBSchema.Contract.ContractID)));
			con.setContractNo(Type.getString(map
					.get(DBSchema.Contract.ContractNo)));
			con.setContractName(Type.getString(map
					.get(DBSchema.Contract.ContractName)));
			con.setAbbreviation(Type.getString(map
					.get(DBSchema.Contract.Abbreviation)));
			con.setParentID(Type.GetInt(map.get(DBSchema.Contract.ParentID)));

			con.setSignatureDate(Type.getDate(map
					.get(DBSchema.Contract.SignatureDate)));

			con.setBuyerID(Type.GetInt(map.get(DBSchema.Contract.BuyerID)));
			con.setDeptIDOfBuyer(Type.GetInt(map
					.get(DBSchema.Contract.DeptIDOfBuyer)));
			con.setVendorID(Type.GetInt(map.get(DBSchema.Contract.VendorID)));
			con.setDeptIDOfVendor(Type.GetInt(map
					.get(DBSchema.Contract.DeptIDOfVendor)));
			con.setContractAmt(Type.GetDouble(map
					.get(DBSchema.Contract.ContractAmt)));
			con.setCurrencyID(Type.GetInt(map.get(DBSchema.Contract.CurrencyID)));
			con.setCurrencyName(Type.getString(map
					.get(DBSchema.CurrencyType.CurrencyName)));
			con.setContractTypeID(Type.GetInt(map
					.get(DBSchema.Contract.ContractTypeID)));
			con.setBuyerRepresentativeID(Type.GetInt(map
					.get(DBSchema.Contract.BuyerRepresentativeID)));
			con.setVendorRepresentativeID(Type.GetInt(map
					.get(DBSchema.Contract.VendorRepresentativeID)));
			con.setContractSummary(Type.getString(map
					.get(DBSchema.Contract.ContractSummary)));
			con.setRemark(Type.getString(map.get(DBSchema.Contract.Remark)));
			con.setRegisterID(Type.GetInt(map.get(DBSchema.Contract.RegisterID)));

			con.setUpdateDate(Type.getDate(map
					.get(DBSchema.Contract.UpdateDate)));

			con.setSourceTypeID(Type.GetInt(map
					.get(DBSchema.Contract.SourceTypeID)));
			con.setGovermentContractSourceID(Type.GetInt(map
					.get(DBSchema.Contract.GovermentContractSourceID)));
			// 质保期
			con.setWarrantyStartDate(Type.getDate(map.get("start_date")));
			con.setWarrantyEndDate(Type.getDate(map.get("end_date")));
			con.setWarrantyDefinition(Type.getString(map
					.get(DBSchema.Contract.WarrantyDefinition)));
			con.setWarrantyLimit(Type.GetFloat(map
					.get(DBSchema.Contract.WarrantyLimit)));
			con.setWarrantyUnit(Type.GetInt(map
					.get(DBSchema.Contract.WarrantyUnit)) == 0 ? 1 : Type
					.GetInt(map.get(DBSchema.Contract.WarrantyUnit)));
			// 有效期
			con.setValidityStartDate(Type.getDate(map
					.get("validity_start_date")));
			con.setValidityEndDate(Type.getDate(map.get("validity_end_date")));
			con.setValidityUnit(Type.GetInt(map
					.get(DBSchema.Contract.ValidityUnit)) == 0 ? 1 : Type
					.GetInt(map.get(DBSchema.Contract.ValidityUnit)));
			con.setValidityLimit(Type.GetFloat(map
					.get(DBSchema.Contract.ValidityLimit)));

			con.setDeliveryID(Type.GetInt(map.get(DBSchema.Contract.DeliveryID)));
			con.setDeliveryName(Type.getString(map
					.get(DBSchema.Contract.DeliveryName)));

			con.setDirectorID(Type.GetInt(map.get(DBSchema.Contract.DirectorID)));
			con.setDirectorName(Type.getString(map
					.get(DBSchema.Contract.DirectorName)));
			con.setNewflag(Type.GetInt(map.get(DBSchema.Contract.newflag)));

			ProjectObj prj = new ProjectObj();
			prj.setProjectID(Type.getString(map.get(DBSchema.Project.ProjectID)));
			prj.setProjectCode(Type.getString(map
					.get(DBSchema.Project.ProjectCode)));
			prj.setProjectName(Type.getString(map
					.get(DBSchema.Project.ProjectName)));// 合同名称回显
			con.setProjectOBJ(prj);

			// 合同基本信息
			Map mapObj = contractDAO.getContractInfoObj(contractID);
			ContractInfoObj conInfoObj = new ContractInfoObj();
			if (mapObj == null) {
				con.setContractInfoObj(null);
			} else {
				conInfoObj.setContractID(Type.GetInt(mapObj
						.get(DBSchema.ContractInfoCon.contractID)));
				conInfoObj.setProjectResourceParentID(Type.GetInt(mapObj
						.get(DBSchema.ContractInfoCon.projectSourceParentID)));
				conInfoObj.setProjectResourceID(Type.GetInt(mapObj
						.get(DBSchema.ContractInfoCon.projectSourceID)));
				conInfoObj
						.setProjectOperationTypeParentID(Type.GetInt(mapObj
								.get(DBSchema.ContractInfoCon.projectOperationTypeParentID)));
				conInfoObj.setProjectOperationTypeID(Type.GetInt(mapObj
						.get(DBSchema.ContractInfoCon.projectOperationTypeID)));
				conInfoObj.setProjectPropertyID(Type.GetInt(mapObj
						.get(DBSchema.ContractInfoCon.projectPropertyID)));
				conInfoObj.setAccountingAgencyID(Type.GetInt(mapObj
						.get(DBSchema.ContractInfoCon.accountingAgencyID)));
				conInfoObj.setExecutiveOrgID(Type.GetInt(mapObj
						.get(DBSchema.ContractInfoCon.executiveOrgID)));
			}
			con.setContractInfoObj(conInfoObj);

			// 合同金额分解
			List<MoneySplitObj> moneyList = new ArrayList<MoneySplitObj>();
			List list = contractDAO.getConTextSplitList(contractID, 1);// 合同资质管理字段表中按科质部管理标准合同金额分解的type=1
			for (int i = 0; i < list.size(); i++) {
				MoneySplitObj moneySplit = new MoneySplitObj();
				Map obj = (Map) list.get(i);
				moneySplit.setSplitTypeID(Type.GetInt(obj
						.get(DBSchema.ContractManagement.ID)));
				moneySplit.setName(Type.getString(obj
						.get(DBSchema.ContractManagement.name)));
				moneySplit.setMoney(Type.GetDouble(obj
						.get(DBSchema.conMoneySplit.money)));
				moneyList.add(moneySplit);
			}
			con.setMoneyList(moneyList);

			con.setParentName(Type.getString(map.get("contractName")));
			con.setParentCode(Type.getString(map.get("contractNo")));
			con.setParentAbb(Type.getString(map.get("abbreviation")));

			con.setHadChild(contractDAO.isHadChildContractListByID(con
					.getContractID()) > 0 ? true : false);

			con.setBuyer(con.getBuyerID() > 0 ? Type.getString(map
					.get("aCompany_name")) : "");
			con.setVendorName(con.getVendorID() > 0 ? Type.getString(map
					.get("bCompany_name")) : "");
			con.setDeptNameOfBuyer(Type.getString(map.get("aDept_name")));
			con.setDeptNameOfVendor(Type.getString(map.get("bDept_name")));
			con.setBuyerRepresentativeName(Type.getString(map.get("aEmp_name")));
			con.setVendorRepresentativeName(Type.getString(map.get("bEmp_name")));
			con.setRegisterName(Type.getString(map.get("cEmp_name")));
			con.setGovermentContractSourceName(Type.getString(map
					.get(DBSchema.GovernmentContractSource.GovermentContractSourceName)));

			con.setEarnedTotalAmt(contractDAO.getTotalSumByID(contractID,
					WebKeys.Type_Total_Sum_Paymented));
			con.setArrearageTotalAmt(con.getContractAmt()
					- con.getEarnedTotalAmt());

			con.setInvoicedTotalAmt(contractDAO.getTotalSumByID(contractID,
					WebKeys.Type_Total_Sum_Invoiced));
			con.setUnCreditedTotalAmt(con.getInvoicedTotalAmt()
					- con.getEarnedTotalAmt() > 0 ? con.getInvoicedTotalAmt()
					- con.getEarnedTotalAmt() : 0);

			// con.setWarningDays(contractDAO.getWarningDays(contractID));

			// 联系人
			con.setDeliveryAddr(Type.getString(map.get("delivery_addr")));
			con.setDeliveryPhone(Type.getString(map.get("delivery_phone")));
			con.setDeliveryPostCode(Type.getString(map.get("delivery_postCode")));
			con.setDeliveryPost(Type.getString(map.get("delivery_post")));
			con.setDeliveryMobilePhone(Type.getString(map
					.get("delivery_mobilePhone")));
			con.setDeliveryEmail(Type.getString(map.get("delivery_email")));
			// 得到摘要模版
			// Stencil stencil = contractDAO.getStencilByDeptId(Type.GetInt(map
			// .get(DBSchema.Contract.DeptIDOfVendor)));
			// con.setStencil(stencil);
			// 最后修改人及时间
			con.setLastModificator(Type.GetInt(map
					.get(DBSchema.Contract.LastModificatorID)));
			con.setLastModificatorName(Type.getString(map
					.get("lastModificator")));
			con.setLastModificationDate(Type.getDate(map
					.get(DBSchema.Contract.LastModificationDate)));

			if (Type.GetInt(map.get(DBSchema.Contract.ContractTypeID)) == 1) {
				con.setIncomAmtTotalByGl(contractDAO.getIncomAmtTotalByGl(Type
						.getString(map.get(DBSchema.Project.ProjectID))));
				con.setPurAmtTotalByGl(contractDAO.getPurAmtTotalByGl(Type
						.getString(map.get(DBSchema.Project.ProjectID))));
				con.setElseTotalAmt(contractDAO.getElseIncomAmtTotal(
						Type.getString(map.get(DBSchema.Project.ProjectID)),
						101));
				con.setPrefix(getIncomeContractPrefix());
			}
			if (Type.GetInt(map.get(DBSchema.Contract.ContractTypeID)) == 2) {
				con.setDelimiter(getOutContractDelimiter());
			}

		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}
		return con;
	}

	public ContractObj getContractInfoNull() {
		logger.info("----> getContractInfoNull");
		ContractObj con = new ContractObj();
		con.setPrefix(getIncomeContractPrefix());
		// con.setDelimiter(getOutContractDelimiter());
		con.setProjectOBJ(new ProjectObj());
		return con;
	}

	/**
	 * @param userID
	 * @return List
	 * @roseuid 4C1B2D2402CE
	 */
	public List getPaymentList(int userID) {
		return null;
	}

	/**
	 * @param contractObj
	 * @roseuid 4C1B2D24030D
	 */
	public void doSaveGeneralContract(ContractObj contractObj) {

	}

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C1B2D24035B
	 */
	public List getPaymentConditionListByID(int contractID) {
		return null;
	}

	/**
	 * @param paymentConditionList
	 * @roseuid 4C1B2D240399
	 */
	public void doSavePaymentCondition(List paymentConditionList) {

	}

	/**
	 * @param paymentConditionObj
	 * @roseuid 4C1B2D250000
	 */
	public void doUpdatePaymentCondition(PaymentConditionObj paymentConditionObj) {

	}

	/**
	 * @param contractID
	 * @param paymentConditionID
	 * @roseuid 4C1B2D25004E
	 */
	public void doDeletePaymentCondition(int contractID, int paymentConditionID) {

	}

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C1B2D2500CB
	 */
	public List getInvoiceListByID(int contractID) {
		return null;
	}

	/**
	 * @param invoiceObj
	 * @roseuid 4C1B2D250119
	 */
	public void doSaveInvoice(InvoiceObj invoiceObj) {

	}

	/**
	 * @param invoiceObj
	 * @roseuid 4C1B2D250167
	 */
	public void doUpdateInvoice(InvoiceObj invoiceObj) {

	}

	/**
	 * @param contractID
	 * @param invoiceID
	 * @roseuid 4C1B2D2501B5
	 */
	public void doDeleteInvoice(int contractID, int invoiceID) {

	}

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C1B2D250242
	 */
	public String getPaymentListByID(int contractID, PageInfo pageInfo) {

		logger.info("--->   getPaymentListByID // contractID=" + contractID);

		return WebUtil.buildStoreForJsonWithoutPageInfo(
				contractDAO.getPaymentListByID(contractID, pageInfo),
				WebKeys.Action_Method_Name_PaymentList);

	}

	public String getGLDetailListByID(String glType, String projectID,
			PageInfo pageInfo) {

		logger.info("--->   getGLDetailListByID // projectID=" + projectID
				+ "//glType=" + glType);

		List aList = new ArrayList();

		if ("income".equals(glType)) {
			aList = contractDAO.getGLIncomeDetailByID(projectID, pageInfo);
		} else if ("elseincome".equals(glType)) {
			aList = contractDAO.getGLElseIncomeDetailByID(projectID, pageInfo,
					101);
		} else {
			aList = contractDAO.getGLPurchaseDetailByID(projectID, pageInfo);
		}

		return WebUtil.buildStoreForJsonWithoutPageInfo(aList,
				WebKeys.Action_Method_Name_GLDetailList);

	}

	/**
	 * @param paymentDetailObj
	 * @roseuid 4C1B2D250290
	 */
	public void doSavePaymentInfo(Object paymentDetailObj) {

	}

	/**
	 * @param paymentDetailObj
	 * @roseuid 4C1B2D2502DE
	 */
	public void doUpdatePaymentInfo(Object paymentDetailObj) {

	}

	/**
	 * @param contractID
	 * @param paymentID
	 * @roseuid 4C1B2D25033C
	 */
	public void doDeletePaymentInfo(int contractID, int paymentID) {

	}

	/**
	 * @return List
	 * @roseuid 4C1B2D2503D8
	 */
	public List getContractReport(EmployeeObj userObj) {
		return null;
	}

	public Document getContractListXML(EmployeeObj userObj, List argumentList) {

		// logger.info(String.valueOf(userObj.getUserID()));
		// List contractList = contractDAO.getContractList(userObj, false);
		// /** 建立document对象 */
		Document document = DocumentHelper.createDocument();
		// /** 建立XML文档的根 */
		// /*
		// * document.addProcessingInstruction("xml", "version=\"1.0\"
		// * encoding=\"UTF-8\"");
		// */
		// Element rootElement = document.addElement("ItemSearchResponse");
		// /* 应答信息 */
		// Element requestElement = rootElement.addElement("Request");
		// // 处理时间
		// Double requestProcessingTime = new Double(0);
		// Element requestProcessingTimeElement = requestElement.addElement(
		// "RequestProcessingTime", requestProcessingTime.toString());
		// // 查询参数
		// Element argumentsElement = requestElement.addElement("Arguments");
		// for (Iterator i = argumentList.listIterator(); i.hasNext();) {
		// Argument argument = (Argument) i.next();
		// Element argumentElement = argumentsElement.addElement("Argument");
		// argumentElement.addAttribute("Name", argument.ArgumentName);
		// argumentElement.addAttribute("Value", argument.ArgumentValue);
		// }
		// /* 合同记录 */
		// Element itemsElement = requestElement.addElement("Items");
		// itemsElement.addElement("RecordCount", String.valueOf(contractList
		// .size()));
		// String surName = "";
		// for (int j = 0; j < 20; j++) {
		//
		// for (Iterator i = contractList.listIterator(); i.hasNext();) {
		// ContractObj contractObj = (ContractObj) i.next();
		// Element itemElement = itemsElement.addElement("Item");
		// itemElement.addElement("ContractID").setText(
		// String.valueOf(contractObj.getContractID()));
		// itemElement.addElement("ContractNo").setText(
		// contractObj.getContractNo());
		// itemElement.addElement("ContractName").setText(
		// contractObj.getContractName());
		// itemElement.addElement("Abbreviation").setText(
		// Type.getString(contractObj.getAbbreviation()));
		// itemElement.addElement("ParentID").setText(
		// String.valueOf(contractObj.getParentID()));
		// itemElement.addElement("SignatureDate").setText(
		// String.valueOf(contractObj.getSignatureDate()));
		// itemElement.addElement("BuyerID").setText(
		// String.valueOf(contractObj.getBuyerID()));
		// itemElement.addElement("DeptIDOfBuyer").setText(
		// String.valueOf(contractObj.getDeptIDOfBuyer()));
		// itemElement.addElement("Buyer").setText(
		// Type.getString(contractObj.getBuyer()));
		//
		// itemElement.addElement("DeptNameOfBuyer").setText(
		// Type.getString(contractObj.getDeptNameOfBuyer()));
		// itemElement.addElement("VendorID").setText(
		// String.valueOf(contractObj.getVendorID()));
		// itemElement.addElement("DeptIDOfVendor").setText(
		// String.valueOf(contractObj.getDeptIDOfVendor()));
		// itemElement.addElement("VendorName").setText(
		// Type.getString(contractObj.getVendorName()));
		// itemElement.addElement("DeptNameOfVendor").setText(
		// Type.getString(contractObj.getDeptNameOfVendor()));
		// itemElement.addElement("ContractAmt").setText(
		// String.valueOf(contractObj.getContractAmt()));
		// itemElement.addElement("CurrencyID").setText(
		// String.valueOf(contractObj.getCurrencyID()));
		// itemElement.addElement("CurrencyName").setText(
		// Type.getString(contractObj.getCurrencyName()));
		// itemElement.addElement("ContractTypeID").setText(
		// String.valueOf(contractObj.getContractTypeID()));
		// itemElement.addElement("ContractTypeName").setText(
		// Type.getString(contractObj.getContractTypeName()));
		// itemElement.addElement("BuyerRepresentativeID").setText(
		// String.valueOf(contractObj.getBuyerRepresentativeID()));
		// //
		// itemElement.addElement("BuyerRepresentativeSurname").setText(contractObj.getBuyerRepresentativeSurname());
		// surName = contractObj.getBuyerRepresentativeSurname();
		// itemElement.addElement("BuyerRepresentativeName").setText(
		// surName + contractObj.getBuyerRepresentativeName());
		// itemElement.addElement("VendorRepresentativeID")
		// .setText(
		// String.valueOf(contractObj
		// .getVendorRepresentativeID()));
		// //
		// itemElement.addElement("VendorRepresentativeSurname").setText(contractObj.getVendorRepresentativeSurname());
		// surName = contractObj.getVendorRepresentativeSurname();
		// itemElement.addElement("VendorRepresentativeName").setText(
		// surName + contractObj.getVendorRepresentativeName());
		// itemElement.addElement("ContractSummary").setText(
		// String.valueOf(contractObj.getContractSummary()));
		// itemElement.addElement("Remark").setText(
		// Type.getString(contractObj.getRemark()));
		// itemElement.addElement("RegisterID").setText(
		// String.valueOf(contractObj.getRegisterID()));
		// //
		// itemElement.addElement("RegisterSurname").setText(contractObj.getRegisterSurname());
		// surName = contractObj.getRegisterSurname();
		// itemElement.addElement("RegisterName").setText(
		// surName + contractObj.getRegisterName());
		// itemElement.addElement("UpdateDate").setText(
		// String.valueOf(contractObj.getUpdateDate()));
		// itemElement.addElement("SourceTypeID").setText(
		// String.valueOf(contractObj.getSourceTypeID()));
		// itemElement.addElement("SourceTypeName").setText(
		// Type.getString(contractObj.getSourceTypeName()));
		// itemElement.addElement("GovermentContractSourceID").setText(
		// String.valueOf(contractObj
		// .getGovermentContractSourceID()));
		// itemElement.addElement("GovermentContractSourceName").setText(
		// Type.getString(contractObj.getGovermentContractSourceName()));
		//
		// }
		// }
		/*
		 * String filePath=new
		 * String("D:/apache-tomcat-5.5.28/webapps/xMIS/contract/contractList1.xml"
		 * ); try { XMLWriter writer = new XMLWriter(new
		 * java.io.FileOutputStream(filePath));
		 * 
		 * writer.write(document);
		 * 
		 * writer.close(); } catch (Exception ex) {
		 * 
		 * ex.printStackTrace(); }
		 */
		return document;

		// WebUtil.buildStoreForJson(contractDAO.getContractList(userObj, true),
		// WebKeys.Action_Method_Name_ContractList);

	}

	public String getContractList(EmployeeObj userObj, List argumentList) {

		return WebUtil.buildStoreForJson(
				contractDAO.getContractList(userObj, true),
				WebKeys.Action_Method_Name_ContractList);

	}

	public String getGovernmentContractSourceList(int selectedID,
			PageInfo pageInfo) {

		logger.info("--->   getGovernmentContractSourceList // selectedID="
				+ selectedID);

		List aList = contractDAO.getGovernmentContractSourceList(pageInfo);
		String selected;

		String dataStr = "";
		for (Iterator i = aList.listIterator(); i.hasNext();) {
			Map map = (Map) i.next();

			selected = Type
					.GetInt(map
							.get(DBSchema.GovernmentContractSource.GovermentContractSourceID)) == selectedID ? "true"
					: "false";
			dataStr += "{'st_id':'"
					+ Type.getString(map
							.get(DBSchema.GovernmentContractSource.GovermentContractSourceID))
					+ "','st_name':'"
					+ Type.getString(map
							.get(DBSchema.GovernmentContractSource.GovermentContractSourceName))
					+ "','st_company_id':'"
					+ Type.getString(map
							.get(DBSchema.GovernmentContractSource.CompanyID))
					+ "','st_company_name':'"
					+ Type.getString(map.get("company"))
					+ "','st_dept_id':'"
					+ Type.getString(map
							.get(DBSchema.GovernmentContractSource.DepartmentID))
					+ "','st_dept_name':'"
					+ Type.getString(map
							.get(DBSchema.Department.DepartmentName))
					+ "','st_amt':'"
					+ Type.getString(map
							.get(DBSchema.GovernmentContractSource.FundingLimit))
					+ "','st_currency_id':'"
					+ Type.getString(map.get(DBSchema.CurrencyType.CurrencyID))
					+ "','st_currency_name':'"
					+ Type.getString(map
							.get(DBSchema.CurrencyType.CurrencyName))
					+ "','st_remark':'"
					+ Type.getString(map
							.get(DBSchema.GovernmentContractSource.Specialty))
					+ "','st_selected':'" + selected + "'},";
		}
		if (!"".equals(dataStr)) {
			dataStr = dataStr.substring(0, dataStr.length() - 1);
		}

		// logger.info(dataStr);

		return "{'totalCount':'" + String.valueOf(aList.size()) + "','rows':["
				+ dataStr + "]}";
	}

	public int doSaveGovernmentContractSource(GContractSourceObj st,
			EmployeeObj userDNA) {

		logger.info("---> doSaveGovernmentContractSource");
		if (st.getGovermentContractSourceID() > 0) {
			// update GovermentContractSource
			contractDAO.doUpdateGContractSource(st, userDNA);

			// if contract id not null then update contract table set sourceID =
			// ? (case selected=true/false)
			if (st.getContractID() > 0)
				contractDAO.doSetSTonContract(st, userDNA);

			return st.getGovermentContractSourceID();

		} else {
			// create new Gov
			return contractDAO.doCreateGContrctSournce(st, userDNA);

		}

	}

	public String doDelGovernmentContractSource(GContractSourceObj st,
			EmployeeObj userDNA) {

		logger.info("---> doDelGovernmentContractSource ? stID = "
				+ Type.getString(st.getGovermentContractSourceID()));

		int existFlag = contractDAO.existsContractByGST(st
				.getGovermentContractSourceID());

		if (existFlag == 1)
			return "“" + st.getGovermentContractSourceName() + "”已被其他合同选用，不能删除";
		else {
			contractDAO.doDeleteGContrctSournce(
					st.getGovermentContractSourceID(), userDNA);
			return "";
		}
	}

	public String getContractListByTypeForCurrency(int contractTypeID,
			String searchName, String currencyType, PageInfo pageInfo,
			EmployeeObj userObj) {

		logger.info("---> getContractListByTypeForCurrency ? currencyType="
				+ String.valueOf(currencyType));

		int currencyFlag = WebUtil.getCurrencyType(currencyType);

		List aList = contractDAO.getContractListByTypeForCurrency(
				contractTypeID, searchName, currencyType, pageInfo,
				userObj.getUserID());

		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_ContractList);

	}

	public String getContractListByType(int contractTypeID, String searchName,
			PageInfo pageInfo) {

		logger.info("---> getContractListByType ? contractTypeID="
				+ String.valueOf(contractTypeID));

		List aList = contractDAO.getContractListByType(contractTypeID,
				searchName, pageInfo);

		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_ContractList, pageInfo);
	}

	public String getContractListByTypeFilterWarning(int contractTypeID,
			int warningType, PageInfo pageInfo, EmployeeObj userDNA,
			List<QueryParam> params) {

		logger.info("---> getContractListByTypeFilterWarning ? warningType="
				+ String.valueOf(contractTypeID));

		List aList = contractDAO.getContractListByTypeFilterWarning(
				contractTypeID, warningType, pageInfo, userDNA.getUserID(),
				params);

		// logger.info("FilterWarning = "+aList.size());
		return WebUtil.buildStoreForJsonWithTotalInfo(aList,
				WebKeys.Action_Method_Name_ContractList
		// , pageInfo
				);

	}

	public String getContractListForParent(int contractTypeID,
			String searchName, List<StandardQuery> query, PageInfo pageInfo) {

		logger.info("---> getContractListForParent ?typeID="
				+ String.valueOf(contractTypeID) + " &searchName=" + searchName);

		List aList = contractDAO.getContractListForParent(contractTypeID,
				searchName, query, pageInfo);

		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_ContractListForParentID, pageInfo);

	}

	public String getParentContractListPositionByID(int contractTypeID,
			int parentID, String searchName, List<StandardQuery> query,
			PageInfo pageInfo) {

		logger.info("---> getContractListForParent ? parentID="
				+ String.valueOf(parentID));

		List aList = contractDAO.getParentContractListPositionByID(
				contractTypeID, parentID, searchName, query, pageInfo);

		int pageNo = Type.GetInt(((Map) aList.get(1)).get("row_no"))
				% pageInfo.getLimit() == 0 ? Type.GetInt(((Map) aList.get(1))
				.get("row_no")) / pageInfo.getLimit() : Type
				.GetInt(((Map) aList.get(1)).get("row_no"))
				/ pageInfo.getLimit() + 1;

		Map aMap = new HashMap();
		aMap.put("pageNo", Integer.valueOf(pageNo));
		aList.remove(1);
		aList.add(aMap);
		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_CompanyList, pageInfo);

	}

	public String getAttachedListStroe(int contractID, PageInfo pageInfo) {

		logger.info("---> getAttachedListStroe ? contractID="
				+ String.valueOf(contractID));

		return getAttachedListStroe(contractID,
				WebKeys.Upload_Application_Contract_Type, pageInfo);
	}

	public String getAttachedListStroe(int contractID, int applicationType,
			PageInfo pageInfo) {

		logger.info("---> getAttachedListStroe ? contractID="
				+ String.valueOf(contractID));

		String refTypeName = "";
		switch (applicationType) {
		case WebKeys.Upload_Application_Contract_Type:
			refTypeName = DBSchema.Contract.TableName;
			break;
		case WebKeys.Upload_Application_PaymentInfo_Type:
			refTypeName = DBSchema.ChargedDetail.TableName;
			break;

		case WebKeys.Upload_Application_InvoiceInfo_Type:
			refTypeName = DBSchema.InvoiceDetail.TableName;
			break;

		case WebKeys.Upload_Application_ReceiptInfo_Type:
			refTypeName = DBSchema.Receipt.TableName;
			break;
		}

		int attTypeID = getAttachmentTypeID(refTypeName);

		List aList = contractDAO.getAttachedListByContractID(contractID,
				attTypeID, pageInfo);

		return WebUtil.buildStoreForJsonWithoutPageInfo(aList,
				WebKeys.Action_Method_Name_AttachedList);

	}

	public String getAttachmentByID(int attachmentID, String directPath) {

		// return contractDAO.getAttachmentByID(attachmentID);

		return contractDAO.getAttachmentByID(attachmentID, directPath);

	}

	private int getAttachmentTypeID(String tableName) {

		return getAttachmentTypeID(tableName, "");
	}

	// private int getAttachmentTypeID(String tableName, String fieldName) {
	//
	// int attTypeID;
	//
	// try {
	// attTypeID = contractDAO.getAttachmentTypeID(tableName, fieldName);
	// } catch (Exception e) {
	// logger.error("",e);
	// attTypeID = contractDAO.createNewAttachmentTypeID(tableName,
	// fieldName);
	// }
	//
	// return attTypeID;
	// }
	// 附件主体类型ID
	private int getAttachmentTypeID(String tableName, String fieldName) {

		int attTypeID = 0;
		try {
			List list = contractDAO.getAttachmentTypeID(tableName, fieldName);
			List<Integer> tList = null;
			if (list != null && list.size() > 0) {
				if (list.size() == 1) {
					return Type.GetInt(((Map) list.get(0))
							.get(DBSchema.AttachmentType.AttachmentTypeID));
				}
				tList = new ArrayList<Integer>();
				for (int i = 0; i < list.size(); i++) {
					tList.add(Type.GetInt(((Map) list.get(i))
							.get(DBSchema.AttachmentType.AttachmentTypeID)));
				}
				attTypeID = (Integer) Collections.min(tList);
			} else {
				attTypeID = contractDAO.createNewAttachmentTypeID(tableName,
						fieldName);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return attTypeID;
	}

	public String getCompanyListStroe(int needRemovedCompanyID,
			PageInfo pageInfo) {

		logger.info("---> getCompanyListStroe ? needRemovedCompanyID="
				+ String.valueOf(needRemovedCompanyID));

		List aList = contractDAO.getCompanyList(needRemovedCompanyID, pageInfo);

		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_CompanyList, pageInfo);

	}

	public String getCompanyListPositionByID(int needRemovedID,
			int positionedID, PageInfo pageInfo) {

		logger.info("---> getCompanyListStroe ? needRemovedCompanyID="
				+ String.valueOf(needRemovedID));

		List aList = contractDAO.getCompanyPositionByID(needRemovedID,
				positionedID, pageInfo);

		// int totalCount = Type.GetInt(((HashMap)aList.get(0)).get("row_no"));

		// logger.info(Type.GetInt(((HashMap)aList.get(1)).get("row_no"))%pageInfo.getLimit());

		int pageNo = Type.GetInt(((Map) aList.get(1)).get("row_no"))
				% pageInfo.getLimit() == 0 ? Type.GetInt(((Map) aList.get(1))
				.get("row_no")) / pageInfo.getLimit() : Type
				.GetInt(((Map) aList.get(1)).get("row_no"))
				/ pageInfo.getLimit() + 1;
		Map aMap = new HashMap();
		aMap.put("pageNo", Integer.valueOf(pageNo));
		aList.remove(1);
		aList.add(aMap);
		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_CompanyList, pageInfo);

		// aMap.put("pageNo",
		// Integer.valueOf(Type.GetInt(aMap.get("row_no"))/pageInfo.getLimit()+1));
		// logger.info(aMap.get("pageNo"));
		// List aList = new ArrayList();
		// aList.add(aMap);
		// return WebUtil.buildStoreForJsonWithoutPageInfo(aList,
		// WebKeys.Action_Method_Name_CompanyList);

		// int rowNo = Type.GetInt(aMap.get("row_no"));
		// pageInfo.setStart(rowNo/pageInfo.getLimit()*pageInfo.getLimit());
		// return getCompanyListStroe(needRemovedID, pageInfo);

	}

	public String getDeptListStroeByCompany(int companyID, EmployeeObj userDNA,
			PageInfo pageInfo) {

		logger.info("---> getDeptListStroeByCompany ? companyID="
				+ String.valueOf(companyID));

		if (userDNA.getCompanyID() == companyID)
			return WebUtil
					.buildStoreForJsonWithoutPageInfo(contractDAO
							.getDeptListBySelfCompanyID(companyID, pageInfo),
							WebKeys.Action_Method_Name_DeptList);
		else {
			return WebUtil.buildStoreForJsonWithoutPageInfo(
					contractDAO.getDeptListByCompanyID(companyID, pageInfo),
					WebKeys.Action_Method_Name_DeptList);
		}

		// return WebUtil.buildStoreForJsonWithoutPageInfo(aList,
		// WebKeys.Action_Method_Name_DeptList);

	}

	public void doSaveContractInfo(ContractObj con, EmployeeObj userDNA) {

		logger.info("---> doSaveContractInfo & contractID = "
				+ Type.getString(con.getContractID()));

		ContractInfoObj conInfoObj = new ContractInfoObj();
		conInfoObj = con.getContractInfoObj();

		int newconID = 0;
		if (con.getContractID() == 0) {
			newconID = contractDAO.doCreateNullContract(userDNA.getUserID());
			con.setContractID(newconID);
			if (con.getContractTypeID() == 1) {
				conInfoObj.setContractID(newconID);
				for (int i = 0; i < con.getMoneyList().size(); i++) {
					MoneySplitObj moneySplitObj = con.getMoneyList().get(i);
					moneySplitObj.setContractID(newconID);
				}
			}
		}else{
			if (con.getContractTypeID() == WebKeys.Type_ContractType_Income){
				for (int i = 0; i < con.getMoneyList().size(); i++) {
					MoneySplitObj moneySplitObj = con.getMoneyList().get(i);

					int result = contractDAO.existMoneySplit(
							moneySplitObj.getContractID(),
							moneySplitObj.getSplitTypeID());
					if (result > 0) {
						contractDAO.doSaveConMoneySplit(moneySplitObj);
					} else {
						contractDAO.doCreateNewConTextSplitNull(
								moneySplitObj.getContractID(),
								moneySplitObj.getSplitTypeID(),
								moneySplitObj.getLastModificatorID());
						contractDAO.doSaveConMoneySplit(moneySplitObj);
					}
				}
			}
		}

		if (con.getContractTypeID() != WebKeys.Type_ContractType_Payout
				&& StringUtils.isEmpty(con.getProjectOBJ().getProjectID())
				&& !StringUtils.isEmpty(con.getProjectOBJ().getProjectCode())) {
			ProjectObj project = projectDAO.getProjectByProjectCode(con
					.getProjectOBJ().getProjectCode());
			if (project == null) {
				project = new ProjectObj();
				project.setProjectCode(con.getProjectOBJ().getProjectCode());
				project.setProjectName(con.getContractName());
				ProjectStatus status = new ProjectStatus();
				status.setId(ProjectStatus.NEW);
				project.setStatus(status);
				project.setManager(con.getDirectorID() == null ? null
						: new SimpleEmployee(con.getDirectorID()));
				project.setExecutiveDept(con.getProjectOBJ().getExecutiveDept());
				project.setMainContract(con);//创建新合同的同时，创建对应的新项目，并保存项目的‘项目合同ID’
				projectDAO.doSaveProjectInfo(project);
			}
			con.getProjectOBJ().setProjectID(project.getProjectID());
		}

		contractDAO.doSaveContractInfo(con, userDNA.getUserID());
		if (con.getContractTypeID() == WebKeys.Type_ContractType_Payout) {
			contractDAO.doSavePayoutContractInfo(con, userDNA.getUserID());
		}
		if (con.getContractTypeID() == WebKeys.Type_ContractType_Income) {
			contractDAO.doSaveContractInfoObj(conInfoObj);
			// for (int i = 0; i < con.getMoneyList().size(); i++) {
			// MoneySplitObj moneySplitObj = con.getMoneyList().get(i);
			//
			// int result = contractDAO.existMoneySplit(
			// moneySplitObj.getContractID(),
			// moneySplitObj.getSplitTypeID());
			// if (result > 0) {
			// contractDAO.doSaveConMoneySplit(moneySplitObj);
			// } else {
			// contractDAO.doCreateNewConTextSplitNull(
			// moneySplitObj.getContractID(),
			// moneySplitObj.getSplitTypeID(),
			// moneySplitObj.getLastModificatorID());
			// contractDAO.doSaveConMoneySplit(moneySplitObj);
			// }
			// }
			JSONArray array = JSONArray.fromObject(con.getMoneySplitStore());
			for (int i = 0; i < array.size(); i++) {
				JSONObject object = array.getJSONObject(i);
				MoneySplitObj moneySplit = new MoneySplitObj();

				moneySplit.setContractID(con.getContractID());
				moneySplit.setSplitTypeID(Type.GetInt(object.get("costID")));
				moneySplit.setName(Type.getString(object.get("costName")));
				moneySplit
						.setDescription(Type.getString(object.get("costDesc")));
				// moneySplit.setLastModificationDate(lastModificationDate);
				moneySplit.setLastModificatorID(userDNA.getUserID());
				moneySplit.setMoney(Type.GetDouble(object.get("costMoney")));
				moneySplit
						.setPercent(Type.GetDouble(object.get("costPercent")));
				moneySplit.setType(10000);

				if (moneySplit.getSplitTypeID() == 0) {
					moneySplit.setSplitTypeID(contractDAO
							.doCreateNewConQualifyManageNull(moneySplit
									.getType()));
					contractDAO.doCreateNewConTextSplitNull(
							con.getContractID(), moneySplit.getSplitTypeID(),
							moneySplit.getLastModificatorID());
				}

				contractDAO.doSaveConQualifyManage(moneySplit);
				contractDAO.doSaveConMoneySplit(moneySplit);

			}
		}

		// re-distribute payment condition
		if (contractDAO.isHadPaymentConditionByID(con.getContractID()) > 0)
			contractDAO.doUpdatePaymentConditionForDistruted(
					con.getContractID(), con.getContractAmt());

	}

	public String getPaymentConditionList(int contractID) {

		logger.info("--->   getPaymentConditionList // contractID="
				+ contractID);

		return WebUtil.buildStoreForJsonWithoutPageInfo(
				contractDAO.getPaymentConditionList(contractID),
				WebKeys.Action_Method_Name_PaymentCondition);

	}

	public void doSavePaymentCondition1(int contractID, String store,
			EmployeeObj userDNA) throws Exception {
		logger.info("--->   doSavePaymentCondition1" + store);
		List<Map<String, Object>> array = JacksonUtil.readValue(store,
				List.class);

		for (int i = 0; i < array.size(); i++) {
			Map<String, Object> object = array.get(i);
			PaymentConditionObj pay = new PaymentConditionObj();

			pay.setContractID(contractID);
			pay.setPaymentConditionID(Type.GetInt(object.get("收款条件ID")));
			pay.setDescription((String) object.get("收款条件描述"));
			pay.setProjectStatusMapping(Type.GetInt(object.get("对应项目状态")));
			pay.setDelayDays(Type.GetInt(object.get("延迟天数")));
			pay.setPaymentPercent(Type.GetDouble(object.get("收款百分比")));
			pay.setPamymentAmt(Type.GetDouble(object.get("收款金额")));
			pay.setLastModificatorID(userDNA.getUserID());
			pay.setPaymentNodeName((String) (object.get("收款节点名称")));

			if (pay.getPaymentConditionID() == 0)
				pay.setPaymentConditionID(contractDAO
						.doCreateNewPaymentConditionNull(pay.getContractID(),
								userDNA.getUserID()));
			contractDAO.doSavePaymentCondition(pay, userDNA.getUserID());
		}
	}

	public int doSavePaymentCondition(PaymentConditionObj pay,
			EmployeeObj userDNA) {

		logger.info("--->   doSavePaymentCondition // contractID="
				+ Type.getString(pay.getContractID()));

		if (pay.getProjectStatusMapping() == 0)
			pay.setProjectStatusMapping(contractDAO.doCreateNewProjectStatus(
					pay.getProjectId(), pay.getProjectStatusName(),
					pay.getDescription(), userDNA.getUserID()));

		if (pay.getPaymentConditionID() == 0)
			pay.setPaymentConditionID(contractDAO
					.doCreateNewPaymentConditionNull(pay.getContractID(),
							userDNA.getUserID()));
		contractDAO.doUpdateProjectStatusName(pay.getProjectStatusMapping(),
				pay.getProjectStatusName());
		return contractDAO.doSavePaymentCondition(pay, userDNA.getUserID());
	}

	public int doDelPaymentCondition(PaymentConditionObj payC,
			EmployeeObj userDNA) {

		logger.debug("---> doDelPaymentCondition ? contractID = "
				+ Type.getString(payC.getContractID()) + "&conditionID="
				+ Type.getString(payC.getPaymentConditionID()) + "&userID="
				+ Type.getString(userDNA.getUserID()));

		contractDAO.doDelPaymentCondition(payC.getContractID(),
				payC.getPaymentConditionID(), userDNA.getUserID());

		return 1;
	}

	public String getChildContractListByID(int parentID, PageInfo pageInfo) {

		logger.info("--->   getChildContractListByID // parentID=" + parentID);

		return WebUtil.buildStoreForJsonWithoutPageInfo(
				contractDAO.getChildContractListByID(parentID, pageInfo),
				WebKeys.Action_Method_Name_ChildContractList);

	}

	public String getInvoiceListByID(int contractID, PageInfo pageInfo) {

		logger.info("--->   getInvoiceListByID // contractID=" + contractID);

		return WebUtil.buildStoreForJsonWithoutPageInfo(
				contractDAO.getInvoiceListByID(contractID, pageInfo),
				WebKeys.Action_Method_Name_InvoiceList);

	}

	public String getReceiptListByID(int contractID, PageInfo pageInfo) {

		logger.info("--->   getReceiptListByID // contractID=" + contractID);

		return WebUtil.buildStoreForJsonWithoutPageInfo(
				contractDAO.getReceiptListAsInvoiceByID(contractID, pageInfo),
				WebKeys.Action_Method_Name_InvoiceList);

	}

	public int doSavePaymentDetail(PaymentDetailObj pay, EmployeeObj userDNA) {

		logger.info("--->   doSavePaymentDetail // contractID="
				+ Type.getString(pay.getPaymentID()));

		if (pay.getPaymentID() == 0)
			pay.setPaymentID(contractDAO.doCreateNewPaymentDetailNull(
					pay.getContractID(), userDNA.getUserID()));

		return contractDAO.doSavePaymentDetail(pay, userDNA.getUserID());
	}

	public int doDelPaymentDetail(PaymentDetailObj pay, EmployeeObj userDNA) {

		logger.debug("---> doDelPaymentCondition ? contractID = "
				+ Type.getString(pay.getContractID()) + "&paymentDetailID="
				+ Type.getString(pay.getPaymentID()) + "&userID="
				+ Type.getString(userDNA.getUserID()));

		contractDAO.doDelPaymentDetail(pay.getContractID(), pay.getPaymentID(),
				userDNA.getUserID());

		return 1;
	}

	public String getContractListByIntegrationSum(int contractTypeID,
			String begDate, String endDate, int integrationType,
			PageInfo pageInfo, EmployeeObj userDNA, List<StandardQuery> querys) {

		logger.info("---> getContractListByIntegrationSum ? integrationType="
				+ String.valueOf(integrationType));
		List sumList = new ArrayList();

		switch (integrationType) {
		case WebKeys.Type_Contract_Integration_New:

			sumList = contractDAO.getSumInfoByIntegrationNewType(
					contractTypeID, begDate, endDate, new PageInfo(),
					userDNA.getUserID(), querys);
			break;

		case WebKeys.Type_Contract_Integration_Performed:

			sumList = contractDAO.getSumInfoByIntegrationPerformed(
					contractTypeID, begDate, endDate, new PageInfo(),
					userDNA.getUserID(), querys);

			break;
		}

		return WebUtil.buildStoreForJsonWithoutPageInfo(sumList,
				WebKeys.Action_Method_Name_ContractListCount);

	}

	public String getContractListByIntegration(int contractTypeID,
			String begDate, String endDate, int integrationType,
			PageInfo pageInfo, EmployeeObj userDNA) {

		logger.info("---> getContractListByIntegration ? integrationType="
				+ String.valueOf(integrationType));

		List aList = null;

		switch (integrationType) {
		case WebKeys.Type_Contract_Integration_New:
			aList = contractDAO.getContractListByIntegrationNewType(
					contractTypeID, begDate, endDate, pageInfo,
					userDNA.getUserID(), null);
			break;

		case WebKeys.Type_Contract_Integration_Performed:
			aList = contractDAO.getContractListByIntegrationPerformed(
					contractTypeID, begDate, endDate, pageInfo,
					userDNA.getUserID(), null);
			break;

		case WebKeys.Type_Contract_Integration_Warranty:
			aList = contractDAO.getContractListByIntegrationWarranty(
					contractTypeID, begDate, endDate, pageInfo,
					userDNA.getUserID(), null);
			break;

		}

		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_ContractList, pageInfo);
	}

	public String getContractListByIntegration(int contractTypeID,
			String begDate, String endDate, int integrationType,
			PageInfo pageInfo, EmployeeObj userDNA, List<StandardQuery> querys) {

		logger.info("---> getContractListByIntegration ? integrationType="
				+ String.valueOf(integrationType));

		List aList = null;

		switch (integrationType) {
		case WebKeys.Type_Contract_Integration_New:
			aList = contractDAO.getContractListByIntegrationNewType(
					contractTypeID, begDate, endDate, pageInfo,
					userDNA.getUserID(), querys);
			break;

		case WebKeys.Type_Contract_Integration_Performed:
			aList = contractDAO.getContractListByIntegrationPerformed(
					contractTypeID, begDate, endDate, pageInfo,
					userDNA.getUserID(), querys);
			break;

		case WebKeys.Type_Contract_Integration_EstimateContractAmt:
			aList = contractDAO
					.getContractListByIntegrationEstimateContractAmt(
							contractTypeID, begDate, endDate, pageInfo,
							userDNA.getUserID(), querys);
			break;

		case WebKeys.Type_Contract_Integration_Invoice:
			aList = contractDAO.getContractListByIntegrationInvoice(
					contractTypeID, begDate, endDate, pageInfo,
					userDNA.getUserID(), querys);
			break;

		case WebKeys.Type_Contract_Integration_Finance:
			aList = contractDAO.getContractListByIntegrationFinance(
					contractTypeID, begDate, endDate, pageInfo,
					userDNA.getUserID(), querys);
			break;

		case WebKeys.Type_Contract_Integration_Warranty:
			aList = contractDAO.getContractListByIntegrationWarranty(
					contractTypeID, begDate, endDate, pageInfo,
					userDNA.getUserID(), querys);
			break;

		}

		return WebUtil.buildStoreForJsonWithTotalInfo(aList,
				WebKeys.Action_Method_Name_ContractList
		// , pageInfo
				);

	}

	public String getContractListByIntegrationSum(int contractTypeID,
			List<StandardQuery> querys, int integrationType, PageInfo pageInfo,
			EmployeeObj userDNA) {

		logger.info("---> getContractListByIntegrationSum ? integrationType="
				+ String.valueOf(integrationType));

		List sumList = contractDAO.getSumInfoByIntegrationSimpleType(
				contractTypeID, querys, new PageInfo(), userDNA.getUserID());

		return WebUtil.buildStoreForJsonWithoutPageInfo(sumList,
				WebKeys.Action_Method_Name_ContractList);

	}

	public String getContractListByIntegrationSum(int contractTypeID,
			int warningType, int integrationType, PageInfo pageInfo,
			EmployeeObj userDNA, List<StandardQuery> querys) {

		return null;
	}

	public String getWarningTotalCountForSum(int contractTypeID,
			String searchName, int integrationType, String begDate,
			String endDate, PageInfo pageInfo, EmployeeObj userDNA,
			List<QueryParam> params) {

		logger.info("---> getWarningTotalCountForSum ? integrationType="
				+ String.valueOf(integrationType));
		List aList = null;
		switch (integrationType) {
		case WebKeys.Type_Contract_Integration_Simple:

			// aList = contractDAO.getWarningTotalCountForIntegrationSimpleType(
			// contractTypeID, searchName, new PageInfo(),
			// userDNA.getUserID());

			aList = contractDAO.getWarningTotalCountForIntegrationSimpleType(
					contractTypeID, searchName, new PageInfo(),
					userDNA.getUserID(), params);

			break;

		}

		return WebUtil.buildStoreForJsonWithoutPageInfo(aList,
				WebKeys.Action_Method_Name_ContractList);

		// return null;
	}

	public String getContractListByIntegrationForCurrency(int contractTypeID,
			String begDate, String endDate, int integrationType,
			String currencyGraph, String subIntegrationType, PageInfo pageInfo,
			EmployeeObj userObj, List<StandardQuery> querys) {

		logger.info("---> getContractListByIntegrationForCurrency ? integrationType="
				+ String.valueOf(integrationType));

		List aList = null;

		int currencyFlag = WebUtil.getCurrencyType(currencyGraph);

		switch (integrationType) {
		case WebKeys.Type_Contract_Integration_New:
			aList = contractDAO.getContractListByIntegrationNewTypeForCurrency(
					contractTypeID, begDate, endDate, currencyGraph, pageInfo,
					userObj.getUserID(), querys);
			break;

		case WebKeys.Type_Contract_Integration_Performed:
			aList = contractDAO
					.getContractListByIntegrationPerformedForCurrency(
							contractTypeID, begDate, endDate, currencyGraph,
							subIntegrationType, pageInfo, userObj.getUserID(),
							querys);
			break;

		}

		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_ContractList, pageInfo);
	}

	public int doSaveInvoiceDetail(InvoiceObj invoice, EmployeeObj userDNA) {

		logger.info("--->   doInvoiceDetail // contractID="
				+ Type.getString(invoice.getInvoiceID()));

		if (invoice.getInvoiceID() == 0)
			invoice.setInvoiceID(contractDAO.doCreateNewInvoiceDetailNull(
					invoice.getContractID(), userDNA.getUserID()));

		return contractDAO.doSaveInvoiceDetail(invoice, userDNA.getUserID());
	}

	public int doSaveReceiptDetail(InvoiceObj invoice, EmployeeObj userDNA) {

		logger.info("--->   doInvoiceDetail // contractID="
				+ Type.getString(invoice.getInvoiceID()));

		if (invoice.getInvoiceID() == 0)
			invoice.setInvoiceID(contractDAO.doCreateNewReceiptDetailNull(
					invoice.getContractID(), userDNA.getUserID()));

		return contractDAO.doSaveReceiptDetail(invoice, userDNA.getUserID());
	}

	public int doDelReceiptDetail(InvoiceObj invo, EmployeeObj userDNA) {

		logger.debug("---> doDelReceiptDetail ? contractID = "
				+ Type.getString(invo.getContractID()) + "&receiptDetailID="
				+ Type.getString(invo.getInvoiceID()) + "&userID="
				+ Type.getString(userDNA.getUserID()));

		contractDAO.doDelReceiptDetail(invo.getContractID(),
				invo.getInvoiceID(), userDNA.getUserID());
		return 1;
	}

	public int doDelInvoiceDetail(InvoiceObj invo, EmployeeObj userDNA) {

		logger.debug("---> doDelInvoiceCondition ? contractID = "
				+ Type.getString(invo.getContractID()) + "&invoiceDetailID="
				+ Type.getString(invo.getInvoiceID()) + "&userID="
				+ Type.getString(userDNA.getUserID()));

		contractDAO.doDelInvoiceDetail(invo.getContractID(),
				invo.getInvoiceID(), userDNA.getUserID());

		return 1;
	}

	public String getRelatedContractList(int contractID, int contractType,
			PageInfo pageInfo) {
		logger.info("---> getRelatedContractList // contractID: " + contractID
				+ "//contractType:" + contractType);
		List aList = contractDAO.getRelatedContractList(contractID,
				contractType, pageInfo);
		return WebUtil.buildStoreForJsonWithoutPageInfo(aList, "");
	}

	public String getContractListByProjectID(String projectID) {
		List list = contractDAO.getContractListByProjectID(projectID);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list,
				WebKeys.Action_Method_Name_ContractList);
	}

	public String getOutContractListByProjectID(String projectID) {
		List list = contractDAO.getOutContractListByProjectID(projectID);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list,
				WebKeys.Action_Method_Name_ContractList);
	}

	public String getContractListByInkling(int contractTypeID,
			List<QueryParam> params, PageInfo pageInfo) {
		logger.info("---> getContractListByInkling // paramSize: "
				+ params.size());
		List aList = contractDAO.getContractListByInkling(contractTypeID,
				params, pageInfo);

		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_ContractList, pageInfo);
	}

	public String getContractListByQuerys(int contractTypeID,
			List<StandardQuery> querys, PageInfo pageInfo) {
		logger.info("---> getContractListByQuery");
		List aList = contractDAO.getContractListByQuerys(contractTypeID,
				querys, pageInfo);

		return (pageInfo != null && pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0) ? WebUtil.buildStoreForJsonWithoutPageInfo(
				aList, WebKeys.Action_Method_Name_ContractList) : WebUtil
				.buildStoreForJsonWithTotalInfo(aList,
						WebKeys.Action_Method_Name_ContractList);
	}

	public boolean hasFullRightsOnContract(int contratcID, int contractTypeID, List<StandardQuery> querys){
		
		boolean hasFullRights = contractDAO.hasFullRightsOnConcrat(contratcID, contractTypeID,
				querys);
		
		return hasFullRights;
	}
	
	public String getCompanyListStroeByInkling(StandardQuery query,
			PageInfo pageInfo) {
		List aList = contractDAO.getCompanyListByInkling(query, pageInfo);
		return (pageInfo != null && pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0) ? WebUtil.buildStoreForJsonWithoutPageInfo(
				aList, WebKeys.Action_Method_Name_ContractList) : WebUtil
				.buildStoreForJson(aList,
						WebKeys.Action_Method_Name_ContractList);
	}

	public void doSaveRelatedContract(int contract1, int contract2, double amt,
			String remark, String contractNo, int userID) {
		ContractObj contract = getContractInfoByID(contract2);
		int contractID = contractDAO.doCreateNullContract(userID);
		contract.setContractID(contractID);
		contract.setContractAmt(amt);
		contract.setRemark(remark);
		contract.setParentID(contract2);
		contract.setContractNo(contractNo);
		contractDAO.doSaveContractInfo(contract, userID);
		contractDAO.doSaveRelatedContract(contract1, contract2, amt, remark,
				contractID);

		if (Type.GetDouble(contractDAO.getAmtByConID(contract2, 1)) == Type
				.GetDouble(contractDAO.getAmtByConID(contract2, 2))) {
			contractDAO.updateContractNewflag(contract2, 1);
		} else if (Type.GetDouble(contractDAO.getAmtByConID(contract2, 1)) != Type
				.GetDouble(contractDAO.getAmtByConID(contract2, 2))) {
			contractDAO.updateContractNewflag(contract2, 0);
		}
	}

	public ContractObj getContractInfoByNo(String relatedNo) {
		return contractDAO.getContractInfoByNo(relatedNo);
	}

	public ProjectDAO getProjectDAO() {
		return projectDAO;
	}

	public void setProjectDAO(ProjectDAO projectDAO) {
		this.projectDAO = projectDAO;
	}

	public String getDelayReasonListByContractID(int contractID,
			PageInfo pageInfo) {
		return WebUtil.buildStoreForJsonWithoutPageInfo(contractDAO
				.getDelayReasonListByContractID(contractID, pageInfo), "");
	}

	public boolean doUpdateDelayInfo(DelayReason dr) {
		return contractDAO.doUpdateDelayInfo(dr);
	}

	public boolean doDeleteDelayReasonInfo(DelayReason dr) {
		return contractDAO.doDeleteDelayReasonInfo(dr);
	}

	/**
	 * 查询发票拆分信息
	 */
	public String getInvoiceSplitDetailBySimpleInvoice(InvoiceSplitObj iso) {
		List list = contractDAO.getInvoiceSplitDetailBySimpleInvoice(iso);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	/**
	 * 查询发票基本拆分信息
	 */
	public String getInvoiceSplitInfoBySimpleInvoice(InvoiceSplitObj iso) {
		List list = contractDAO.getInvoiceSplitInfoBySimpleInvoice(iso);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	/**
	 * 删除发票拆分信息
	 */
	public boolean doDeleteInvoiceSplitInfo(InvoiceSplitObj iso) {
		return contractDAO.doDeleteInvoiceSplitInfo(iso);
	}

	/**
	 * 新建发票拆分信息
	 */
	public boolean doSaveInvoiceSplitInfo(InvoiceSplitObj iso) {
		return contractDAO.doSaveInvoiceSplitInfo(iso);
	}

	/**
	 * 更新发票拆分信息
	 */
	public boolean doUpdateInvoiceSplitInfo(InvoiceSplitObj iso) {
		return contractDAO.doUpdateInvoiceSplitInfo(iso);
	}

	/**
	 * 得到辅助查询列表
	 */
	public String getIncomeAssistList(List<StandardQuery> query,
			PageInfo pageInfo) {
		logger.info("---> getIncomeAssistList");
		List list = contractDAO.getIncomeAssistList(query, pageInfo);
		return WebUtil.buildStoreForJson(list, "", pageInfo);
	}

	public int doSavePaymentDetailQuickly(PaymentDetailObj pay,
			EmployeeObj userDNA, String sourceType, Object obj)
			throws Exception {
		int flag = -1;
		try {
			flag = this.doSavePaymentDetail(pay, userDNA);
			if ("estimated".equals(sourceType))
				contractDAO.doUpdateEstimatedPayedFlag((EstimatedPayment) obj);
			if ("paymentCondition".equals(sourceType))
				contractDAO
						.doUpdatePaymentConditionPayedFlag((PaymentConditionObj) obj);
			if ("invoice".equals(sourceType))
				contractDAO.doUpdateInvoicePayedFlag((InvoiceObj) obj);
			if ("receipt".equals(sourceType))
				contractDAO.doUpdateReceiptPayedFlag((InvoiceObj) obj);
		} catch (Exception e) {
			e.printStackTrace();
			throw new Exception();
		}
		return flag;
	}

	public Map<String, String> getPaymentConditionTotalAmtById(int id) {
		logger.info("-->getPaymentConditionTotalAmtById");
		return contractDAO.getPaymentConditionTotalAmtById(id);
	}

	public String getContractRelationInfo(int pCID, int relatedID) {
		logger.info("-->getContractRelationInfo");
		List list = contractDAO.getContractRelationInfo(pCID, relatedID);
		return JSONArray.fromObject(list).toString();
	}

	/**
	 * 修改支出合同对应拆分合同信息 inContractID 对应收入合同 contractNo 支出合同编号 relateAmt 对应金额
	 * remark 备注 relatedID 对应ID
	 */
	public void updateRelatedContract(int inContractID, String contractNo,
			double relateAmt, String remark, int relatedID, int outContractID) {
		logger.info("-->updateRelatedContract");
		contractDAO.updateRelatedContract(inContractID, contractNo, relateAmt,
				remark, relatedID, outContractID);
		if (Type.GetDouble(contractDAO.getAmtByConID(outContractID, 1)) == Type
				.GetDouble(contractDAO.getAmtByConID(outContractID, 2))) {
			contractDAO.updateContractNewflag(outContractID, 1);
		} else if (Type.GetDouble(contractDAO.getAmtByConID(outContractID, 1)) != Type
				.GetDouble(contractDAO.getAmtByConID(outContractID, 2))) {
			contractDAO.updateContractNewflag(outContractID, 0);
		}
	}

	public Stencil getStencilByDeptId(int deptID) {
		logger.info("-->getStencilByDeptId");
		return contractDAO.getStencilByDeptId(deptID);
	}

	public Map<String, String> getContractSummaryByConId(int deptID) {
		logger.info("-->getContractSummaryByConId");

		String existStencil = "0";
		String summaryResult = "";
		String stencilContent = "";
		String stencilKeyWords = "";

		Stencil newStencil = contractDAO.getStencilByDeptId(deptID);

		if (newStencil != null) {
			stencilContent = newStencil.getContent();
			stencilKeyWords = newStencil.getKeyWords();
			existStencil = "1";
		}

		Map<String, String> map = new HashMap<String, String>();
		map.put("existStencil", existStencil);
		map.put("summaryResult", summaryResult);
		map.put("stencilContent", stencilContent);
		map.put("stencilKeyWords", stencilKeyWords);
		return map;
	}

	public int getParentDeptIdByDeptId(int deptID) {
		return contractDAO.getParentDeptIdByDeptId(deptID);
	}

	public int getContractIDByProjectID(String projectID) {
		return contractDAO.getContractIDByProjectID(projectID);
	}

	public String getPaymentConditionListByProjectID(String projectID) {
		logger.info("--->   getPaymentConditionListByProjectID // projectID="
				+ projectID);
		int contractID = this.getContractIDByProjectID(projectID);
		return WebUtil.buildStoreForJsonWithoutPageInfo(
				contractDAO.getPaymentConditionList(contractID),
				WebKeys.Action_Method_Name_PaymentCondition);
	}

	public double getIncomAmtTotalByGl(String projectID) {
		// TODO Auto-generated method stub
		return contractDAO.getIncomAmtTotalByGl(projectID);
	}

	public double getElseIncomAmtTotal(String projectID, int type) {
		return contractDAO.getElseIncomAmtTotal(projectID, type);
	}

	public String getContractInfoByParentID(int parentID) {
		List storeList = contractDAO.getContractInfoByParentID(parentID);
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(storeList, config).toString();
	}

	public String getContractInfoByType(int type) {
		List storeList = contractDAO.getContractInfoByType(type);
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(storeList, config).toString();
	}

	public String getProjectCodeTypeByParentID(int parentID) {
		List storeList = contractDAO.getProjectCodeTypeByParentID(parentID);
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(storeList, config).toString();
	}

	public String getDeptCode() {
		List storeList = contractDAO.getDeptCode();
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(storeList, config).toString();
	}

	public String getProjectNumber(String projectNumber) {
		List storeList = contractDAO.getProjectNumber(projectNumber);
		String number = null;
		int numberList[] = new int[storeList.size()];
		if (storeList.size() == 0) {
			number = "001";
		} else {

			logger.info("storeListsize=" + storeList.size());
			for (int i = 0; i < storeList.size(); i++) {
				String item = Type.getString(storeList.get(i)).substring(6,
						Type.getString(storeList.get(i)).length() - 1);
				int num;
				String itemSplit[] = item.split("-");
				int index = 0;
				for (int j = 0; j < itemSplit.length; j++) {
					Pattern p = Pattern.compile("^20[0-9][0-9]$");
					Matcher m = p.matcher(itemSplit[j]);
					boolean result = m.find();
					if (result == true) {
						index = j + 2;
					}
				}
				num = Type.GetInt(itemSplit[index].substring(0, 3).trim());
				numberList[i] = num;
			}

			int n = numberList[0];
			for (int i = 0; i < numberList.length; i++) {
				if (n < numberList[i]) {
					n = numberList[i];
				}
			}
			number = String.format("%03d", n + 1);
			// StringUtils.leftPad("10", 10, '0')

		}
		return number;
	}

	public boolean existProjectCode(String projectCode) {
		boolean flag = contractDAO.existProjectNumber(projectCode) > 0;
		return flag;
	}

	public String getConInfoByProjectCode(String projectCode) {
		List storeList = contractDAO.getConInfoByProjectCode(projectCode);
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(storeList, config).toString();
	}

	public int doDeleteConMoneySplit(int contractID, int typeID) {
		int result = 1;
		try {

			contractDAO.doDeleteConMoneySplit(contractID, typeID);
			contractDAO.doDeleteQualifyManage(typeID);

		} catch (Exception e) {
			e.printStackTrace();
			result = 0;
		}
		return result;
	}

	public String getConTextSplitList(int contractID, int type) {
		List list = contractDAO.getConTextSplitList(contractID, type);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	public String getContractStateList(List<StandardQuery> query,
			PageInfo pageInfo) {
		List list = contractDAO.getContractStateList(query, pageInfo);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	public boolean isEmpBelongDept(int deptId, int empId) {
		boolean flag = contractDAO.isEmpBelongDept(deptId, empId) > 0;
		return flag;
	}

	public String getPaymentPromptList() {
		List list = contractDAO.getPaymentPromptList();
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	public String getScientificName(int deptId) {
		List list = contractDAO.getScientificName(deptId);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	public String getContractCodeNumber(String contractCode) {
		List storeList = contractDAO.getContractCodeNumber(contractCode);
		String number = null;
		int numberList[] = new int[storeList.size()];
		if (storeList.size() == 0) {
			number = "001";
		} else {
			logger.info("storeListsize=" + storeList.size());
			for (int i = 0; i < storeList.size(); i++) {
				String item = Type.getString(storeList.get(i)).substring(6,
						Type.getString(storeList.get(i)).length() - 1);
				item = item.trim();
				int num;

				if (item.indexOf("-ZC") != -1) {
					String itemSplit[] = item.split("-ZC");
					num = Type.GetInt(itemSplit[1]);
				} else {
					String itemSplit[] = item.split("/");
					num = Type.GetInt(itemSplit[1].split("-")[0].substring(1));
				}
				numberList[i] = num;
			}

			int n = numberList[0];
			for (int i = 0; i < numberList.length; i++) {
				if (n < numberList[i]) {
					n = numberList[i];
				}
			}
			number = String.format("%03d", n + 1);
			// StringUtils.leftPad("10", 10, '0')

		}
		return number;
	}

	public boolean existContractCode(String contractCode) {
		boolean flag = contractDAO.existContractCode(contractCode) > 0;
		return flag;
	}

	public String getContractInfoByContractCode(String contractCode) {
		List storeList = contractDAO
				.getContractInfoByContractCode(contractCode);
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(storeList, config).toString();
	}

	public double getAmtByConID(int contractId, int flag) {
		return contractDAO.getAmtByConID(contractId, flag);
	}

	public void updateContractNewflag(int contractId, int newflag) {
		contractDAO.updateContractNewflag(contractId, newflag);
	}

	public void doSaveEstimatedInfo(String projectID, String store,
			EmployeeObj userDNA) throws Exception {
		logger.info("---> doSaveEstimatedInfo" + store);
		JSONArray array = JSONArray.fromObject(store);

		for (int i = 0; i < array.size(); i++) {
			JSONObject object = array.getJSONObject(i);
			EstimatedPayment ep = new EstimatedPayment();

			ep.setProjectId(projectID);
			ep.setEstimatedDate(Type.getDate(object.get("estimatedDate")));
			ep.setEstimatedAmt(Type.GetDouble(object.get("estimatedAmt")));
			ep.setEstimatedPercent(Type.GetDouble(object
					.get("estimatedPercent")));
			ep.setLastModifiedId(userDNA.getUserID());
			ep.setLastModifiedDate(Type.getDate(new java.util.Date()));

			String id = Type.getString(object.get("ID"));
			if (id == null || "".equals(id.trim())) {
				id = UUIDUtils.nextCode();
				ep.setId(id);
				ep.setRegisterId(userDNA.getUserID());
				ep.setRegisterDate(Type.getDate(new java.util.Date()));
				contractDAO.doSaveEstimatedInfo(ep);
			} else {
				ep.setId(id);
				contractDAO.doUpdateEstimatedInfo(ep);
			}
		}

	}

	public String getProjectEstimatedPaymentList(String projectID) {
		List list = contractDAO.getProjectEstimatedPaymentList(projectID);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	public void doDeleteEstimatedInfo(String estimatedIds) throws Exception {
		String[] ids = estimatedIds.split(",");
		for (int i = 0; i < ids.length; i++) {
			contractDAO.doDeleteEstimatedInfo(ids[i]);
		}
	}

	public void doUpdateEstimatedPayedFlag(EstimatedPayment ep) {
		contractDAO.doUpdateEstimatedPayedFlag(ep);
	}

	public double getTotalSumByID(int contractID, int sumType) {
		return contractDAO.getTotalSumByID(contractID, sumType);
	}

	public String getContractAmtSplitList(List<StandardQuery> querys,
			PageInfo pageInfo) {
		List aList = contractDAO.getContractAmtSplitListByQuerys(querys,
				pageInfo);

		return (pageInfo != null && pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0) ? WebUtil.buildStoreForJsonWithoutPageInfo(
				aList, WebKeys.Action_Method_Name_ContractList) : WebUtil
				.buildStoreForJsonWithTotalInfo(aList,
						WebKeys.Action_Method_Name_ContractList);

	}

	public String getOutContractDelimiter() {
		String delimiter = contractDAO.getOutContractDelimiter();
		return delimiter;
	}

	public String getIncomeContractPrefix() {
		String prefix = contractDAO.getIncomeContractPrefix();
		return prefix;
	}

	public boolean isEmpDeptSpecial(int deptId, int empId) {
		boolean flag = contractDAO.isEmpDeptSpecial(deptId, empId) > 0;
		return flag;
	}
	
	public String getContractSourceTypeID(int level){
		List storeList = contractDAO.getContractSourceTypeID(level);
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(storeList, config).toString();
	}
	
	public String getExcuteDeptInfo(String deptIds){
		List deptInfo = contractDAO.getExcuteDeptInfo(deptIds);
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(deptInfo,config).toString();
		
	}
	
	public String getOutContractDelimiterStroe(){
		List list = contractDAO.getOutContractDelimiterStroe();
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(list, config).toString();
	}
}
