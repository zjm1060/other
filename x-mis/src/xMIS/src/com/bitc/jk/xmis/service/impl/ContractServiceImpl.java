//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\Impl\\ContractServiceImpl.java

package com.bitc.jk.xmis.service.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;

import com.bitc.jk.xmis.dao.ContractDAO;
import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.GContractSourceObj;
import com.bitc.jk.xmis.model.InvoiceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PaymentConditionObj;
import com.bitc.jk.xmis.model.PaymentDetailObj;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.service.ContractService;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

public class ContractServiceImpl implements ContractService {
	private static Logger logger = Logger.getLogger(ContractServiceImpl.class);

	private ContractDAO contractDAO;

	private OrganizationDAO organizationDAO;

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

			con.setContractID(Type
					.GetInt(map.get(DBSchema.Contract.ContractID)));
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
			con.setCurrencyID(Type
					.GetInt(map.get(DBSchema.Contract.CurrencyID)));
			con.setContractTypeID(Type.GetInt(map
					.get(DBSchema.Contract.ContractTypeID)));
			con.setBuyerRepresentativeID(Type.GetInt(map
					.get(DBSchema.Contract.BuyerRepresentativeID)));
			con.setVendorRepresentativeID(Type.GetInt(map
					.get(DBSchema.Contract.VendorRepresentativeID)));
			con.setContractSummary(Type.getString(map
					.get(DBSchema.Contract.ContractSummary)));
			con.setRemark(Type.getString(map.get(DBSchema.Contract.Remark)));
			con.setRegisterID(Type
					.GetInt(map.get(DBSchema.Contract.RegisterID)));

			con.setUpdateDate(Type.getDate(map
					.get(DBSchema.Contract.UpdateDate)));

			con.setSourceTypeID(Type.GetInt(map
					.get(DBSchema.Contract.SourceTypeID)));
			con.setGovermentContractSourceID(Type.GetInt(map
					.get(DBSchema.Contract.GovermentContractSourceID)));

			// logger.info(map.get(DBSchema.Contract.WarrantyStartDate));
			con.setWarrantyStartDate(Type.getDate(map.get("start_date")));
			con.setWarrantyEndDate(Type.getDate(map.get("end_date")));
			con.setWarrantyDefinition(Type.getString(map
					.get(DBSchema.Contract.WarrantyDefinition)));
			con.setWarrantyLimit(Type.GetFloat(map
					.get(DBSchema.Contract.WarrantyLimit)));
			con.setWarrantyUnit(Type.GetInt(map
					.get(DBSchema.Contract.WarrantyUnit)));

			con.setDeliveryID(Type
					.GetInt(map.get(DBSchema.Contract.DeliveryID)));
			con.setDeliveryName(Type.getString(map
					.get(DBSchema.Contract.DeliveryName)));

			ProjectObj prj = new ProjectObj();
			prj.setProjectID(Type.GetInt(map.get(DBSchema.Project.ProjectID)));
			prj.setProjectCode(Type.getString(map
					.get(DBSchema.Project.ProjectCode)));
			prj.setProjectName(Type.getString(map
					.get(DBSchema.Project.ProjectName)));

			con.setProjectOBJ(prj);
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
			con
					.setBuyerRepresentativeName(Type.getString(map
							.get("aEmp_name")));
			con.setVendorRepresentativeName(Type
					.getString(map.get("bEmp_name")));
			con.setRegisterName(Type.getString(map.get("cEmp_name")));
			con
					.setGovermentContractSourceName(Type
							.getString(map
									.get(DBSchema.GovernmentContractSource.GovermentContractSourceName)));

			List aList = contractDAO.getContractTypeList();
			Map aMap = new LinkedHashMap();
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map iMap = (HashMap) i.next();
				aMap.put(Type.getInteger(iMap
						.get(DBSchema.ContractType.ContractTypeID)), iMap
						.get(DBSchema.ContractType.ContractTypeName));
			}

			con.setContractTypeMap(aMap);

			List bList = contractDAO.getContractSourceTypeList();
			Map bMap = new LinkedHashMap();
			for (Iterator i = bList.listIterator(); i.hasNext();) {
				Map iMap = (HashMap) i.next();
				bMap
						.put(
								Type
										.getInteger(iMap
												.get(DBSchema.ContractSourceType.ContractSourceTypeID)),
								iMap
										.get(DBSchema.ContractSourceType.ContractSourceTypeName));
			}

			con.setContractSourceTypeMap(bMap);

			List cList = contractDAO.getCurrencyTypeList();
			Map cMap = new LinkedHashMap();
			Map ccMap = new LinkedHashMap();
			String currencyData = "[";
			for (Iterator i = cList.listIterator(); i.hasNext();) {
				Map iMap = (HashMap) i.next();
				cMap.put(Type.getInteger(iMap
						.get(DBSchema.CurrencyType.CurrencyID)), iMap
						.get(DBSchema.CurrencyType.CurrencyName));

				ccMap.put(Type.getInteger(iMap
						.get(DBSchema.CurrencyType.CurrencyID)), iMap
						.get(DBSchema.CurrencyType.CurrenyGraph));

				currencyData += "["
						+ Type.getString(iMap
								.get(DBSchema.CurrencyType.CurrencyID))
						+ ",'"
						+ Type.getString(iMap
								.get(DBSchema.CurrencyType.CurrencyName))
						+ "'],";
			}
			con.setCurrencyTypeMap(cMap);
			con.setCurrencyCodeMap(ccMap);
			con.setCurrencyDataStore(currencyData.substring(0, currencyData
					.length() - 1)
					+ "]");

			// logger.info(con.getCurrencyDataStore());

			List dList = organizationDAO.getCompanyList();
			Map dMap = new LinkedHashMap();
			String companyData = "[";
			for (Iterator i = dList.listIterator(); i.hasNext();) {
				Map iMap = (HashMap) i.next();
				dMap.put(Type.getInteger(iMap.get(DBSchema.Company.CompanyID)),
						iMap.get(DBSchema.Company.Abbreviation));

				// companyData += "['"
				// + Type.getString(iMap
				// .get(DBSchema.Company.CompanyID))
				// + "','"
				// + Type.getString(iMap
				// .get(DBSchema.Company.Abbreviation))
				// + "'],";

				companyData += "{'compID':'"
						+ Type.getString(iMap.get(DBSchema.Company.CompanyID))
						+ "','compAbb':'"
						+ Type.getString(iMap
								.get(DBSchema.Company.Abbreviation))
						+ "','compName':'"
						+ Type
								.getString(iMap
										.get(DBSchema.Company.CompanyName))
						+ "'},";

			}

			con.setCompanyMap(dMap);

			// logger.info(companyData);
			con.setCompanyDataStore(companyData.substring(0, companyData
					.length() - 1)
					+ "]");

			con.setEarnedTotalAmt(contractDAO.getTotalSumByID(contractID,
					WebKeys.Type_Total_Sum_Paymented));
			con.setArrearageTotalAmt(con.getContractAmt()
					- con.getEarnedTotalAmt());

			con.setInvoicedTotalAmt(contractDAO.getTotalSumByID(contractID,
					WebKeys.Type_Total_Sum_Invoiced));
			con.setUnCreditedTotalAmt(con.getInvoicedTotalAmt()
					- con.getEarnedTotalAmt());

			// String lunchAddressData = "[";
			// List tList = organizationDAO.getLunchAddressList();
			// Map tMap = new LinkedHashMap();
			// for (Iterator i = tList.listIterator(); i.hasNext();) {
			// Map iMap = (HashMap) i.next();
			// tMap.put(Type.getInteger(iMap
			// .get(DBSchema.LunchAddress.LunchAddressID)), iMap
			// .get(DBSchema.LunchAddress.LunchAddressName));
			//
			// lunchAddressData += "["
			// + Type.getString(iMap
			// .get(DBSchema.LunchAddress.LunchAddressID))
			// + ",'"
			// + Type.getString(iMap
			// .get(DBSchema.LunchAddress.LunchAddressName))
			// + "'],";
			//
			// }
			//
			// emp.setLunchAddressMap(tMap);
			// logger.info(lunchAddressData.substring(0,
			// lunchAddressData.length() - 1));
			// emp.setLunchAddressData(lunchAddressData.substring(0,
			// lunchAddressData.length() - 1)
			// + "]");

		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}

		return con;
	}

	public ContractObj getContractInfoNull() {

		logger.info("----> getContractInfoNull");

		ContractObj con = new ContractObj();

		con.setProjectOBJ(new ProjectObj());
		try {

			List aList = contractDAO.getContractTypeList();
			Map aMap = new LinkedHashMap();
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map iMap = (HashMap) i.next();
				aMap.put(Type.getInteger(iMap
						.get(DBSchema.ContractType.ContractTypeID)), iMap
						.get(DBSchema.ContractType.ContractTypeName));
			}

			con.setContractTypeMap(aMap);

			List bList = contractDAO.getContractSourceTypeList();
			Map bMap = new LinkedHashMap();
			for (Iterator i = bList.listIterator(); i.hasNext();) {
				Map iMap = (HashMap) i.next();
				bMap
						.put(
								Type
										.getInteger(iMap
												.get(DBSchema.ContractSourceType.ContractSourceTypeID)),
								iMap
										.get(DBSchema.ContractSourceType.ContractSourceTypeName));
			}

			con.setContractSourceTypeMap(bMap);

			List cList = contractDAO.getCurrencyTypeList();
			Map cMap = new LinkedHashMap();
			Map ccMap = new LinkedHashMap();
			String currencyData = "[";
			for (Iterator i = cList.listIterator(); i.hasNext();) {
				Map iMap = (HashMap) i.next();
				cMap.put(Type.getInteger(iMap
						.get(DBSchema.CurrencyType.CurrencyID)), iMap
						.get(DBSchema.CurrencyType.CurrencyName));

				ccMap.put(Type.getInteger(iMap
						.get(DBSchema.CurrencyType.CurrencyID)), iMap
						.get(DBSchema.CurrencyType.CurrenyGraph));

				currencyData += "{'currencyID':'"
						+ Type.getString(iMap
								.get(DBSchema.CurrencyType.CurrencyID))
						+ "','currencyName':'"
						+ Type.getString(iMap
								.get(DBSchema.CurrencyType.CurrencyName))
						+ "'},";

			}
			con.setCurrencyTypeMap(cMap);
			con.setCurrencyCodeMap(ccMap);
			con.setCurrencyDataStore(currencyData.substring(0, currencyData
					.length() - 1)
					+ "]");

			logger.info(con.getCurrencyDataStore());

			List dList = organizationDAO.getCompanyList();
			Map dMap = new LinkedHashMap();
			String companyData = "[";
			for (Iterator i = dList.listIterator(); i.hasNext();) {
				Map iMap = (HashMap) i.next();
				dMap.put(Type.getInteger(iMap.get(DBSchema.Company.CompanyID)),
						iMap.get(DBSchema.Company.Abbreviation));

				// companyData += "['"
				// + Type.getString(iMap
				// .get(DBSchema.Company.CompanyID))
				// + "','"
				// + Type.getString(iMap
				// .get(DBSchema.Company.Abbreviation))
				// + "'],";

				companyData += "{'compID':'"
						+ Type.getString(iMap.get(DBSchema.Company.CompanyID))
						+ "','compAbb':'"
						+ Type.getString(iMap
								.get(DBSchema.Company.Abbreviation))
						+ "','compName':'"
						+ Type
								.getString(iMap
										.get(DBSchema.Company.CompanyName))
						+ "'},";

			}

			con.setCompanyMap(dMap);

			logger.info(companyData);
			con.setCompanyDataStore(companyData.substring(0, companyData
					.length() - 1)
					+ "]");

		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}

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

		return WebUtil.buildStoreForJsonWithoutPageInfo(contractDAO
				.getPaymentListByID(contractID, pageInfo),
				WebKeys.Action_Method_Name_PaymentList);

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
		 * String("D:/apache-tomcat-5.5.28/webapps/xMIS/contract/contractList1.xml");
		 * try { XMLWriter writer = new XMLWriter(new
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

		return WebUtil.buildStoreForJson(contractDAO.getContractList(userObj,
				true), WebKeys.Action_Method_Name_ContractList);

	}

	public String getGovernmentContractSourceList(int selectedID,
			PageInfo pageInfo) {

		logger.info("--->   getGovernmentContractSourceList // selectedID="
				+ selectedID);

		List aList = contractDAO.getGovernmentContractSourceList(pageInfo);
		String selected;

		String dataStr = "";
		for (Iterator i = aList.listIterator(); i.hasNext();) {
			Map map = (HashMap) i.next();

			selected = Type
					.GetInt(map
							.get(DBSchema.GovernmentContractSource.GovermentContractSourceID)) == selectedID ? "true"
					: "false";
			dataStr += "{'st_id':'"
					+ Type
							.getString(map
									.get(DBSchema.GovernmentContractSource.GovermentContractSourceID))
					+ "','st_name':'"
					+ Type
							.getString(map
									.get(DBSchema.GovernmentContractSource.GovermentContractSourceName))
					+ "','st_company_id':'"
					+ Type.getString(map
							.get(DBSchema.GovernmentContractSource.CompanyID))
					+ "','st_company_name':'"
					+ Type.getString(map.get("company"))
					+ "','st_dept_id':'"
					+ Type
							.getString(map
									.get(DBSchema.GovernmentContractSource.DepartmentID))
					+ "','st_dept_name':'"
					+ Type.getString(map
							.get(DBSchema.Department.DepartmentName))
					+ "','st_amt':'"
					+ Type
							.getString(map
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

		dataStr = dataStr.substring(0, dataStr.length() - 1);

		logger.info(dataStr);

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
			contractDAO.doDeleteGContrctSournce(st
					.getGovermentContractSourceID(), userDNA);
			return "";
		}
	}

	public String getContractListByTypeForCurrency(int contractTypeID,
			String searchName, String currencyType, PageInfo pageInfo,
			EmployeeObj userObj) {

		logger.info("---> getContractListByTypeForCurrency ? currencyType="
				+ String.valueOf(currencyType));

		int currencyFlag = WebUtil.getCurrencyType(currencyType);
		
		List aList = contractDAO.getContractListByTypeForCurrency(contractTypeID,
				searchName, currencyType, pageInfo, userObj.getUserID());

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
				WebKeys.Action_Method_Name_ContractList);
	}

	public String getContractListByTypeFilterWarning(int contractTypeID,
			int warningType, PageInfo pageInfo, EmployeeObj userDNA) {

		logger.info("---> getContractListByTypeFilterWarning ? warningType="
				+ String.valueOf(contractTypeID));

		List aList = contractDAO.getContractListByTypeFilterWarning(
				contractTypeID, warningType, pageInfo, userDNA.getUserID());

		// logger.info("FilterWarning = "+aList.size());
		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_ContractList);

	}

	public String getContractListForParent(int contractTypeID,
			String searchName, PageInfo pageInfo) {

		logger
				.info("---> getContractListForParent ?typeID="
						+ String.valueOf(contractTypeID) + " &searchName="
						+ searchName);

		List aList = contractDAO.getContractListForParent(contractTypeID,
				searchName, pageInfo);

		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_ContractListForParentID);

	}

	public String getParentContractListPositionByID(int contractTypeID,
			int parentID, String searchName, PageInfo pageInfo) {

		logger.info("---> getContractListForParent ? parentID="
				+ String.valueOf(parentID));

		List aList = contractDAO.getParentContractListPositionByID(
				contractTypeID, parentID, searchName, pageInfo);

		int pageNo = Type.GetInt(((HashMap) aList.get(1)).get("row_no"))
				% pageInfo.getLimit() == 0 ? Type.GetInt(((HashMap) aList
				.get(1)).get("row_no"))
				/ pageInfo.getLimit() : Type.GetInt(((HashMap) aList.get(1))
				.get("row_no"))
				/ pageInfo.getLimit() + 1;

		Map aMap = new HashMap();
		aMap.put("pageNo", Integer.valueOf(pageNo));
		aList.remove(1);
		aList.add(aMap);
		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_CompanyList);

	}

	public String getAttachedListStroe(int contractID, PageInfo pageInfo) {

		logger.info("---> getAttachedListStroe ? contractID="
				+ String.valueOf(contractID));

		return getAttachedListStroe(contractID, WebKeys.Upload_Application_Contract_Type, pageInfo);
	}
	
	public String getAttachedListStroe(int contractID, int applicationType, PageInfo pageInfo) {

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
		}
		
		int attTypeID = getAttachmentTypeID(refTypeName);

		List aList = contractDAO.getAttachedListByContractID(contractID,
				attTypeID, pageInfo);

		return WebUtil.buildStoreForJsonWithoutPageInfo(aList,
				WebKeys.Action_Method_Name_AttachedList);

	}

	public int doSaveAttachmentForContract(AttachmentFile attFile) {
		// return contract_id

		logger
				.info("---> doSaveAttachmentForContract ? fileName = "
						+ attFile.getFileName() + "&contractID = "
						+ attFile.getRefID());

		attFile
				.setAttachmentTypeID(getAttachmentTypeID(DBSchema.Contract.TableName));

		if (attFile.getRefID() == 0)
			attFile.setRefID(contractDAO.doCreateNullContract(attFile
					.getUploadID()));

		// ContractObj con = new ContractObj();
		// if("".equals(attFile.getRefName().trim()))
		// attFile.setRefName(attFile.getFileName());
		//		
		// con.setContractID(attFile.getRefID());
		// con.setContractTypeID(attFile.getRefType());
		// //con.setContractName(("".equals(attFile.getRefName().trim()))?
		// attFile.getFileName():attFile.getRefName());
		// con.setAbbreviation(("".equals(attFile.getRefName().trim()))?
		// attFile.getFileName():attFile.getRefName());
		//		
		// contractDAO.doSaveContractInfo(con, attFile.getUploadID());

		contractDAO.doSaveContractAttachment(attFile);

		return attFile.getRefID();

	}
	
	public int doSaveAttachmentForPayment(AttachmentFile attFile) {
		// return contract_id

		logger
				.info("---> doSaveAttachmentForPayment ? fileName = "
						+ attFile.getFileName() + "&contractID = "
						+ attFile.getRefID());

		attFile
				.setAttachmentTypeID(getAttachmentTypeID(DBSchema.ChargedDetail.TableName));

		contractDAO.doSaveContractAttachment(attFile);

		return attFile.getRefID();

	}	
	
	public int doSaveAttachmentForInvoice(AttachmentFile attFile) {
		// return contract_id

		logger
				.info("---> doSaveAttachmentForInvoice ? fileName = "
						+ attFile.getFileName() + "&contractID = "
						+ attFile.getRefID());

		attFile
				.setAttachmentTypeID(getAttachmentTypeID(DBSchema.InvoiceDetail.TableName));

		contractDAO.doSaveContractAttachment(attFile);

		return attFile.getRefID();

	}		

	public String getAttachmentByID(int attachmentID, String directPath) {

		// return contractDAO.getAttachmentByID(attachmentID);

		return contractDAO.getAttachmentByID(attachmentID, directPath);

	}

	private int getAttachmentTypeID(String tableName) {

		return getAttachmentTypeID(tableName, "");
	}

	private int getAttachmentTypeID(String tableName, String fieldName) {

		int attTypeID;

		try {
			attTypeID = contractDAO.getAttachmentTypeID(tableName, fieldName);
		} catch (Exception e) {
			attTypeID = contractDAO.createNewAttachmentTypeID(tableName,
					fieldName);
		}

		return attTypeID;
	}

	public String getCompanyListStroe(int needRemovedCompanyID,
			PageInfo pageInfo) {

		logger.info("---> getCompanyListStroe ? needRemovedCompanyID="
				+ String.valueOf(needRemovedCompanyID));

		List aList = contractDAO.getCompanyList(needRemovedCompanyID, pageInfo);

		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_CompanyList);

	}

	public String getCompanyListPositionByID(int needRemovedID,
			int positionedID, PageInfo pageInfo) {

		logger.info("---> getCompanyListStroe ? needRemovedCompanyID="
				+ String.valueOf(needRemovedID));

		List aList = contractDAO.getCompanyPositionByID(needRemovedID,
				positionedID, pageInfo);

		// int totalCount = Type.GetInt(((HashMap)aList.get(0)).get("row_no"));

		// logger.info(Type.GetInt(((HashMap)aList.get(1)).get("row_no"))%pageInfo.getLimit());

		int pageNo = Type.GetInt(((HashMap) aList.get(1)).get("row_no"))
				% pageInfo.getLimit() == 0 ? Type.GetInt(((HashMap) aList
				.get(1)).get("row_no"))
				/ pageInfo.getLimit() : Type.GetInt(((HashMap) aList.get(1))
				.get("row_no"))
				/ pageInfo.getLimit() + 1;
		Map aMap = new HashMap();
		aMap.put("pageNo", Integer.valueOf(pageNo));
		aList.remove(1);
		aList.add(aMap);
		return WebUtil.buildStoreForJson(aList,
				WebKeys.Action_Method_Name_CompanyList);

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
			return WebUtil.buildStoreForJsonWithoutPageInfo(contractDAO
					.getDeptListBySelfCompanyID(companyID, pageInfo),
					WebKeys.Action_Method_Name_DeptList);
		else {
			return WebUtil.buildStoreForJsonWithoutPageInfo(contractDAO
					.getDeptListByCompanyID(companyID, pageInfo),
					WebKeys.Action_Method_Name_DeptList);
		}

		// return WebUtil.buildStoreForJsonWithoutPageInfo(aList,
		// WebKeys.Action_Method_Name_DeptList);

	}

	public void doSaveContractInfo(ContractObj con, EmployeeObj userDNA) {

		logger.info("---> doSaveContractInfo & contractID = "
				+ Type.getString(con.getContractID()));

		if (con.getContractID() == 0)
			con.setContractID(contractDAO.doCreateNullContract(userDNA
					.getUserID()));

		contractDAO.doSaveContractInfo(con, userDNA.getUserID());

		// re-distribute payment condition
		if (contractDAO.isHadPaymentConditionByID(con.getContractID()) > 0)
			contractDAO.doUpdatePaymentConditionForDistruted(con
					.getContractID(), con.getContractAmt());

	}

	public String getPaymentConditionList(int contractID) {

		logger.info("--->   getPaymentConditionList // contractID="
				+ contractID);

		return WebUtil.buildStoreForJsonWithoutPageInfo(contractDAO
				.getPaymentConditionList(contractID),
				WebKeys.Action_Method_Name_PaymentCondition);

	}

	public int doSavePaymentCondition(PaymentConditionObj pay,
			EmployeeObj userDNA) {

		logger.info("--->   doSavePaymentCondition // contractID="
				+ Type.getString(pay.getContractID()));

		if (pay.getProjectStatusMapping() == 0)
			pay.setProjectStatusMapping(contractDAO.doCreateNewProjectStatus(
					pay.getProjectStatusName(), pay.getDescription(), userDNA
							.getUserID()));

		if (pay.getPaymentConditionID() == 0)
			pay.setPaymentConditionID(contractDAO
					.doCreateNewPaymentConditionNull(pay.getContractID(),
							userDNA.getUserID()));
		
		return contractDAO.doSavePaymentCondition(pay, userDNA.getUserID());
	}

	public int doDelPaymentCondition(PaymentConditionObj payC,
			EmployeeObj userDNA) {

		logger.debug("---> doDelPaymentCondition ? contractID = "
				+ Type.getString(payC.getContractID()) + "&conditionID="
				+ Type.getString(payC.getPaymentConditionID()) + "&userID="
				+ Type.getString(userDNA.getUserID()));

		contractDAO.doDelPaymentCondition(payC.getContractID(), payC
				.getPaymentConditionID(), userDNA.getUserID());

		return 1;
	}

	public String getChildContractListByID(int parentID, PageInfo pageInfo) {

		logger.info("--->   getChildContractListByID // parentID=" + parentID);

		return WebUtil.buildStoreForJsonWithoutPageInfo(contractDAO
				.getChildContractListByID(parentID, pageInfo),
				WebKeys.Action_Method_Name_ChildContractList);

	}

	public String getInvoiceListByID(int contractID, PageInfo pageInfo) {

		logger.info("--->   getInvoiceListByID // contractID=" + contractID);

		return WebUtil.buildStoreForJsonWithoutPageInfo(contractDAO
				.getInvoiceListByID(contractID, pageInfo),
				WebKeys.Action_Method_Name_InvoiceList);

	}

	public int doSavePaymentDetail(PaymentDetailObj pay, EmployeeObj userDNA) {

		logger.info("--->   doSavePaymentDetail // contractID="
				+ Type.getString(pay.getPaymentID()));

		if (pay.getPaymentID() == 0)
			pay.setPaymentID(contractDAO.doCreateNewPaymentDetailNull(pay
					.getContractID(), userDNA.getUserID()));

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

	public String getContractListByIntegrationSum(int contractTypeID, String begDate, String endDate,
			int integrationType, PageInfo pageInfo, EmployeeObj userDNA) {

		logger.info("---> getContractListByIntegrationSum ? integrationType="
				+ String.valueOf(integrationType));
		List sumList = new ArrayList();
		
		switch(integrationType){
			case WebKeys.Type_Contract_Integration_New:
				
				sumList = contractDAO.getSumInfoByIntegrationNewType(contractTypeID, begDate, endDate,
						new PageInfo(), userDNA.getUserID());
				break;

			case WebKeys.Type_Contract_Integration_Performed:
				
				sumList = contractDAO.getSumInfoByIntegrationPerformed(contractTypeID, begDate, endDate,
						new PageInfo(), userDNA.getUserID());
				
				break;
		}
			

		return WebUtil.buildStoreForJsonWithoutPageInfo(sumList,WebKeys.Action_Method_Name_ContractListCount); 

	}

	public String getContractListByIntegration(int contractTypeID, String begDate, String endDate,
			int integrationType, PageInfo pageInfo, EmployeeObj userDNA) {

		logger.info("---> getContractListByIntegration ? integrationType="
				+ String.valueOf(integrationType));
		
		List aList = null;
		
		switch(integrationType){
			case WebKeys.Type_Contract_Integration_New:
				aList = contractDAO.getContractListByIntegrationNewType(contractTypeID, begDate, endDate,
						pageInfo, userDNA.getUserID());
				break;

			case WebKeys.Type_Contract_Integration_Performed:
				aList = contractDAO.getContractListByIntegrationPerformed(contractTypeID, begDate, endDate,
						pageInfo, userDNA.getUserID());
				break;
				
			case WebKeys.Type_Contract_Integration_Warranty:
				aList = contractDAO.getContractListByIntegrationWarranty(contractTypeID, begDate, endDate,
						pageInfo, userDNA.getUserID());
				break;
				
		}
			
		return WebUtil.buildStoreForJson(aList, WebKeys.Action_Method_Name_ContractList);

	}	
	
	public String getContractListByIntegrationSum(int contractTypeID, String searchName,
			int integrationType, PageInfo pageInfo, EmployeeObj userDNA){
		
		
		logger.info("---> getContractListByIntegrationSum ? integrationType="
				+ String.valueOf(integrationType));

		List sumList = contractDAO.getSumInfoByIntegrationSimpleType(contractTypeID, searchName,
				new PageInfo(), userDNA.getUserID());		
		
		return WebUtil.buildStoreForJsonWithoutPageInfo(sumList,WebKeys.Action_Method_Name_ContractList);

	}
	public String getContractListByIntegrationSum(int contractTypeID, int warningType,
			int integrationType, PageInfo pageInfo, EmployeeObj userDNA){
		
		return null;
	}

	public String getWarningTotalCountForSum(int contractTypeID,
			String searchName, int integrationType, String begDate,
			String endDate, PageInfo pageInfo, EmployeeObj userDNA){
		
		
		logger.info("---> getWarningTotalCountForSum ? integrationType="
				+ String.valueOf(integrationType));
		List aList = null;
		switch(integrationType){
		case WebKeys.Type_Contract_Integration_Simple:
			
			aList = contractDAO.getWarningTotalCountForIntegrationSimpleType(contractTypeID, searchName,
					new PageInfo(), userDNA.getUserID());		
				
			break;
		
		}
		
		return WebUtil.buildStoreForJsonWithoutPageInfo(aList,WebKeys.Action_Method_Name_ContractList);		
		
//		return null;
	}


	public String getContractListByIntegrationForCurrency(int contractTypeID,
			String begDate, String endDate, int integrationType,
			String currencyGraph, String subIntegrationType, PageInfo pageInfo, EmployeeObj userObj){
		
		logger.info("---> getContractListByIntegrationForCurrency ? integrationType="
				+ String.valueOf(integrationType));
		
		List aList = null;
		
		int currencyFlag = WebUtil.getCurrencyType(currencyGraph);
		
		switch(integrationType){
			case WebKeys.Type_Contract_Integration_New:
				aList = contractDAO.getContractListByIntegrationNewTypeForCurrency(contractTypeID, begDate, endDate,
						currencyGraph, pageInfo, userObj.getUserID());
				break;
				
			case WebKeys.Type_Contract_Integration_Performed:
				aList = contractDAO.getContractListByIntegrationPerformedForCurrency(contractTypeID, begDate, endDate,
						currencyGraph, subIntegrationType, pageInfo, userObj.getUserID());
				break;

		}
			
		return WebUtil.buildStoreForJson(aList, WebKeys.Action_Method_Name_ContractList);
	}


	public int doSaveInvoiceDetail(InvoiceObj invoice, EmployeeObj userDNA) {

		logger.info("--->   doInvoiceDetail // contractID="
				+ Type.getString(invoice.getInvoiceID()));

		if (invoice.getInvoiceID() == 0)
			invoice.setInvoiceID(contractDAO.doCreateNewInvoiceDetailNull(invoice
					.getContractID(), userDNA.getUserID()));

		return contractDAO.doSaveInvoiceDetail(invoice, userDNA.getUserID());
	}

	public int doDelInvoiceDetail(InvoiceObj invo, EmployeeObj userDNA){

		logger.debug("---> doDelInvoiceCondition ? contractID = "
				+ Type.getString(invo.getContractID()) + "&invoiceDetailID="
				+ Type.getString(invo.getInvoiceID()) + "&userID="
				+ Type.getString(userDNA.getUserID()));

		contractDAO.doDelInvoiceDetail(invo.getContractID(), invo.getInvoiceID(),
				userDNA.getUserID());

		return 1;
	}


}
