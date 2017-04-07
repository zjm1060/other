//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\ContractService.java

package com.bitc.jk.xmis.service;

import java.sql.Date;
import java.util.List;

import org.dom4j.Document;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.GContractSourceObj;
import com.bitc.jk.xmis.model.InvoiceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PaymentConditionObj;
import com.bitc.jk.xmis.model.PaymentDetailObj;
import com.bitc.jk.xmis.util.Type;

public interface ContractService {

	/**
	 * @param userObj
	 *            TODO
	 * @return List
	 * @roseuid 4C19E536034B
	 */
	public List getUnfinishedContractList(EmployeeObj userObj);

	/**
	 * 1) getContractInfoByID 2) getPaymentConditionListByID 3)
	 * getInvoiceListByID 4) getPaymentListByID 5) getReferenceContractListByID
	 * 6) getProjectListByID
	 * 
	 * @param contractID
	 * @return com.bitc.jk.xmis.model.ContractObj
	 * @roseuid 4C19E5530177
	 */
	public ContractObj getContractInfoByID(int contractID);

	public ContractObj getContractInfoNull();

	/**
	 * @param userID
	 * @return List
	 * @roseuid 4C19E5F8001F
	 */
	public List getPaymentList(int userID);

	/**
	 * @param contractObj
	 * @roseuid 4C19E62C009C
	 */
	public void doSaveGeneralContract(ContractObj contractObj);

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C19ECDE0280
	 */
	public List getPaymentConditionListByID(int contractID);

	/**
	 * @param paymentConditionList
	 * @roseuid 4C1AE161003E
	 */
	public void doSavePaymentCondition(List paymentConditionList);

	/**
	 * @param paymentConditionObj
	 * @roseuid 4C1AE3D500DA
	 */
	public void doUpdatePaymentCondition(PaymentConditionObj paymentConditionObj);

	/**
	 * @param contractID
	 * @param paymentConditionID
	 * @roseuid 4C1AE4DB02BF
	 */
	public void doDeletePaymentCondition(int contractID, int paymentConditionID);

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C1B0C8F00DA
	 */
	public List getInvoiceListByID(int contractID);

	/**
	 * @param invoiceObj
	 * @roseuid 4C1B0CCA01A5
	 */
	public void doSaveInvoice(InvoiceObj invoiceObj);

	/**
	 * @param invoiceObj
	 * @roseuid 4C1B0DDF0271
	 */
	public void doUpdateInvoice(InvoiceObj invoiceObj);

	/**
	 * @param contractID
	 * @param invoiceID
	 * @roseuid 4C1B0DFF0196
	 */
	public void doDeleteInvoice(int contractID, int invoiceID);

	/**
	 * @param contractID
	 * @param pageInfo
	 *            TODO
	 * @return List
	 * @roseuid 4C1B109F0119
	 */
	public String getPaymentListByID(int contractID, PageInfo pageInfo);

	/**
	 * @param paymentDetailObj
	 * @roseuid 4C1B139C0251
	 */
	public void doSavePaymentInfo(Object paymentDetailObj);

	/**
	 * @param paymentDetailObj
	 * @roseuid 4C1B13E90148
	 */
	public void doUpdatePaymentInfo(Object paymentDetailObj);

	/**
	 * @param contractID
	 * @param paymentID
	 * @roseuid 4C1B15280280
	 */
	public void doDeletePaymentInfo(int contractID, int paymentID);

	/**
	 * @param userObj
	 *            TODO
	 * @return List
	 * @roseuid 4C1B169D031C
	 */
	public List getContractReport(EmployeeObj userObj);

	public Document getContractListXML(EmployeeObj userObj, List argumentList);

	public String getGovernmentContractSourceList(int selectedID,
			PageInfo pageInfo);

	public int doSaveGovernmentContractSource(GContractSourceObj st,
			EmployeeObj userDNA);

	public String getContractListByType(int contractTypeID, String searchName,
			PageInfo pageInfo);

	public String getContractListByTypeForCurrency(int contractTypeID,
			String searchName, String currencyType, PageInfo pageInfo,
			EmployeeObj userObj);

	public String getContractListForParent(int contractTypeID,
			String searchName, PageInfo pageInfo);

	public String getParentContractListPositionByID(int contractTypeID,
			int parentID, String searchName, PageInfo pageInfo);

	public String doDelGovernmentContractSource(GContractSourceObj st,
			EmployeeObj userDNA);

	public String getAttachedListStroe(int contractID, PageInfo pageInfo);

	public String getAttachedListStroe(int contractID, int applicationType,
			PageInfo pageInfo);

	public String getCompanyListStroe(int needRemovedCompanyID,
			PageInfo pageInfo);

	public String getCompanyListPositionByID(int needRemovedID,
			int positionedID, PageInfo pageInfo);

	public String getDeptListStroeByCompany(int companyID, EmployeeObj userDNA,
			PageInfo pageInfo);

	public int doSaveAttachmentForContract(AttachmentFile attFile);

	public int doSaveAttachmentForPayment(AttachmentFile attFile);

	public String getAttachmentByID(int attachmentID, String directPath);

	public void doSaveContractInfo(ContractObj con, EmployeeObj userDNA);

	public String getPaymentConditionList(int contractID);

	public String getContractList(EmployeeObj userObj, List argumentList);

	public int doSavePaymentCondition(PaymentConditionObj pay,
			EmployeeObj userDNA);

	public int doDelPaymentCondition(PaymentConditionObj payC,
			EmployeeObj userDNA);

	public String getChildContractListByID(int parentID, PageInfo pageInfo);

	public String getInvoiceListByID(int contractID, PageInfo pageInfo);

	public int doSavePaymentDetail(PaymentDetailObj pay, EmployeeObj uerDAN);


	public int doDelPaymentDetail(PaymentDetailObj pay, EmployeeObj uerDAN);
	
	public int doSaveInvoiceDetail(InvoiceObj invo,EmployeeObj uerDAN);
	
	public int doDelInvoiceDetail(InvoiceObj invo, EmployeeObj uerDAN);

	public String getContractListByTypeFilterWarning(int contractTypeID,
			int warningType, PageInfo pageInfo, EmployeeObj userDNA);

	public String getContractListByIntegration(int contractTypeID,
			String begDate, String endDate, int integrationType,
			PageInfo pageInfo, EmployeeObj userDNA);

	public String getContractListByIntegrationSum(int contractTypeID,
			String begDate, String endDate, int integrationType,
			PageInfo pageInfo, EmployeeObj userDNA);

	public String getContractListByIntegrationSum(int contractTypeID,
			String searchName, int integrationType, PageInfo pageInfo,
			EmployeeObj userDNA);

	public String getContractListByIntegrationSum(int contractTypeID,
			int warningType, int integrationType, PageInfo pageInfo,
			EmployeeObj userDNA);

	public String getWarningTotalCountForSum(int contractTypeID,
			String searchName, int integrationType, String begDate,
			String endDate, PageInfo pageInfo, EmployeeObj userObj);

	public String getContractListByIntegrationForCurrency(int contractTypeID,
			String begDate, String endDate, int integrationType,
			String currencyGraph, String subIntegrationType, PageInfo pageInfo, EmployeeObj userObj);
}
