//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\ContractDAO.java

package com.bitc.jk.xmis.dao;

import java.sql.Date;
import java.util.List;
import java.util.Map;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.GContractSourceObj;
import com.bitc.jk.xmis.model.InvoiceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PaymentConditionObj;
import com.bitc.jk.xmis.model.PaymentDetailObj;

public interface ContractDAO {

	/**
	 * @param userObj
	 *            TODO
	 * @param isFinished
	 * @return List
	 * @roseuid 4C19E59A0119
	 */
	public List getContractList(EmployeeObj userObj, boolean isFinished);

	/**
	 * @param userObj
	 * @return List
	 * @roseuid 4C19E616005D
	 */
	public List getPaymentList(EmployeeObj userObj);

	/**
	 * @param contractObj
	 * @roseuid 4C19E70C02BF
	 */
	public void doSaveGeneralContract(ContractObj contractObj);

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C1AE1310203
	 */
	public List getPaymentConditionListByID(int contractID);

	/**
	 * @param paymentConditionList
	 * @roseuid 4C1AE1A302CE
	 */
	public void doSavePaymentCondition(List paymentConditionList);

	/**
	 * @param paymentConditionObj
	 * @roseuid 4C1AE4A0031C
	 */
	public void doUpdatePaymentCondition(PaymentConditionObj paymentConditionObj);

	/**
	 * @param contractID
	 * @param paymentConditionID
	 * @roseuid 4C1AE511031C
	 */
	public void doDeletePaymentCondition(int contractID, int paymentConditionID);

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C1B0CAD0232
	 */
	public List getInvoiceListByID(int contractID);

	/**
	 * @param invoiceObj
	 * @roseuid 4C1B0CE701C5
	 */
	public void doSaveInvoice(InvoiceObj invoiceObj);

	/**
	 * @param invoiceObj
	 * @roseuid 4C1B0DF9036B
	 */
	public void doUpdateInvoice(InvoiceObj invoiceObj);

	/**
	 * @param contractID
	 * @param invoiceID
	 * @roseuid 4C1B0E150177
	 */
	public void doDeleteInvoice(int contractID, int invoiceID);

	/**
	 * @param contractID
	 * @param pageInfo
	 *            TODO
	 * @return List
	 * @roseuid 4C1B13900213
	 */
	public List getPaymentListByID(int contractID, PageInfo pageInfo);

	/**
	 * @param paymentDetailObj
	 * @roseuid 4C1B13C8034B
	 */
	public void doSavePaymentInfo(Object paymentDetailObj);

	/**
	 * @param paymentDetailObj
	 * @roseuid 4C1B141301F4
	 */
	public void doUpdatePaymentInfo(Object paymentDetailObj);

	/**
	 * @param contractID
	 * @param paymentID
	 * @roseuid 4C1B15470213
	 */
	public void doDeletePaymentInfo(int contractID, int paymentID);

	/**
	 * @param contractID
	 * @return List
	 * @roseuid 4C1B1639038A
	 */
	public List getReferenceContractListByID(int contractID);

	/**
	 * @param contractObj
	 * @return List
	 * @roseuid 4C1B175F033C
	 */
	public List getProjectListByID(EmployeeObj contractObj);

	public Map getContractInfo(int contractID);

	public List getContractTypeList();

	public List getContractSourceTypeList();

	public List getCurrencyTypeList();

	public List getGovernmentContractSourceList(PageInfo pageInfo);

	public int doCreateGContrctSournce(GContractSourceObj st,
			EmployeeObj userDAN);

	public void doUpdateGContractSource(GContractSourceObj st,
			EmployeeObj userDAN);

	public void doSetSTonContract(GContractSourceObj st, EmployeeObj userDAN);

	public List getContractListByType(int contractTypeID, String searchName,
			PageInfo pageInfo);

	public List getContractListForParent(int contractTypeID, String searchName,
			PageInfo pageInfo);

	public List getParentContractListPositionByID(int contractTypeID,
			int parentID, String searchName, PageInfo pageInfo);

	public int existsContractByGST(int stID);

	public void doDeleteGContrctSournce(int stID, EmployeeObj userDNA);

	public List getAttachedListByContractID(int contractID,
			int attachmentTypeID, PageInfo pageInfo);

	public int doSaveContractAttachment(AttachmentFile attFile);

	public int getAttachmentTypeID(String tableName, String fieldName);

	public int createNewAttachmentTypeID(String tableName, String fieldName);

	public String getAttachmentByID(int attachmentID, String directPath);

	public int doCreateNullContract(int userID);

	public List getCompanyList(int needRemovedCompanyID, PageInfo pageInfo);

	public List getCompanyPositionByID(int needRemovedID, int companyID,
			PageInfo pageInfo);

	public List getDeptListByCompanyID(int companyID, PageInfo pageInfo);

	public List getDeptListBySelfCompanyID(int companyID, PageInfo pageInfo);

	public void doSaveContractInfo(ContractObj con, int userID);

	public List getPaymentConditionList(int contractID);

	public int doCreateNewProjectStatus(String statusName, String desc,
			int userID);

	public int doSavePaymentCondition(PaymentConditionObj pay, int userID);

	public void doDelPaymentCondition(int contractID, int paymentConditionID,
			int userID);

	public int doCreateNewPaymentConditionNull(int contractID, int userID);

	public int isHadChildContractListByID(int parentID);

	public List getChildContractListByID(int parentID, PageInfo pageInfo);

	public int isHadPaymentConditionByID(int contractID);

	public void doUpdatePaymentConditionForDistruted(int contractID,
			double contractAmt);

	public double getTotalSumByID(int contractID, int sumType);

	public List getInvoiceListByID(int contractID, PageInfo pageInfo);
	
	public int doCreateNewInvoiceDetailNull(int contractID, int userID);
	public int doSaveInvoiceDetail(InvoiceObj pay, int userID);

	public void doDelInvoiceDetail(int contractID, int invoiceDetailID,
			int userID);
	

	public int doCreateNewPaymentDetailNull(int contractID, int userID);

	public int doSavePaymentDetail(PaymentDetailObj pay, int userID);

	public void doDelPaymentDetail(int contractID, int paymentDetailID,
			int userID);

	public List getContractListByTypeFilterWarning(int contractTypeID,
			int warningType, PageInfo pageInfo, int userID);

	public List getContractListByTypeForCurrency(int contractTypeID,
			String searchName, String currencyFlag, PageInfo pageInfo,
			int userID);

	public List getContractListByIntegrationNewType(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID);

	public List getContractListByIntegrationPerformed(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID);
	
	public List getContractListByIntegrationWarranty(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID);
	
	public List getSumInfoByIntegrationNewType(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID);

	public List getSumInfoByIntegrationPerformed(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID);

	public List getSumInfoByIntegrationSimpleType(int contractTypeID,
			String searchName, PageInfo pageInfo, int userID);

	public List getWarningTotalCountForIntegrationSimpleType(
			int contractTypeID, String searchName, PageInfo pageInfo, int userID);

	public List getContractListByIntegrationNewTypeForCurrency(
			int contractTypeID, String begDate, String endDate,
			String currencyFlag, PageInfo pageInfo, int userID);

	public List getContractListByIntegrationPerformedForCurrency(
			int contractTypeID, String begDate, String endDate,
			String currencyFlag, String subIntegrationType, PageInfo pageInfo, int userID);

	
	
}
