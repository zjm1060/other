//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\ContractService.java

package com.bitc.jk.xmis.service;

import java.sql.Date;
import java.util.List;
import java.util.Map;

import org.dom4j.Document;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.DelayReason;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.EstimatedPayment;
import com.bitc.jk.xmis.model.GContractSourceObj;
import com.bitc.jk.xmis.model.InvoiceObj;
import com.bitc.jk.xmis.model.InvoiceSplitObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PaymentConditionObj;
import com.bitc.jk.xmis.model.PaymentDetailObj;
import com.bitc.jk.xmis.model.Stencil;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;

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

	/**
	 * 通过项目名称查询财务信息
	 */
	public String getGLDetailListByID(String glType, String projectID,
			PageInfo pageInfo);

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
			String searchName, List<StandardQuery> query, PageInfo pageInfo);

	public String getParentContractListPositionByID(int contractTypeID,
			int parentID, String searchName, List<StandardQuery> query,
			PageInfo pageInfo);

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

	public String getAttachmentByID(int attachmentID, String directPath);

	public void doSaveContractInfo(ContractObj con, EmployeeObj userDNA);

	public String getPaymentConditionList(int contractID);
	public String getPaymentConditionListByProjectID(String projectID);

	public String getContractList(EmployeeObj userObj, List argumentList);

	public int doSavePaymentCondition(PaymentConditionObj pay,
			EmployeeObj userDNA);

	public int doDelPaymentCondition(PaymentConditionObj payC,
			EmployeeObj userDNA);

	public String getChildContractListByID(int parentID, PageInfo pageInfo);

	public String getInvoiceListByID(int contractID, PageInfo pageInfo);

	public String getReceiptListByID(int contractID, PageInfo pageInfo);

	public int doSavePaymentDetail(PaymentDetailObj pay, EmployeeObj uerDAN);

	public int doDelPaymentDetail(PaymentDetailObj pay, EmployeeObj uerDAN);

	public int doSaveInvoiceDetail(InvoiceObj invo, EmployeeObj uerDAN);

	public int doSaveReceiptDetail(InvoiceObj invo, EmployeeObj uerDAN);

	public int doDelInvoiceDetail(InvoiceObj invo, EmployeeObj uerDAN);

	public int doDelReceiptDetail(InvoiceObj invo, EmployeeObj uerDAN);

	public String getContractListByTypeFilterWarning(int contractTypeID,
			int warningType, PageInfo pageInfo, EmployeeObj userDNA,
			List<QueryParam> params);

	public String getContractListByIntegration(int contractTypeID,
			String begDate, String endDate, int integrationType,
			PageInfo pageInfo, EmployeeObj userDNA);

	public String getContractListByIntegration(int contractTypeID,
			String begDate, String endDate, int integrationType,
			PageInfo pageInfo, EmployeeObj userDNA, List<StandardQuery> querys);

	public String getContractListByIntegrationSum(int contractTypeID,
			String begDate, String endDate, int integrationType,
			PageInfo pageInfo, EmployeeObj userDNA, List<StandardQuery> querys);

	public String getContractListByIntegrationSum(int contractTypeID,
			List<StandardQuery> querys, int integrationType, PageInfo pageInfo,
			EmployeeObj userDNA);

	public String getContractListByIntegrationSum(int contractTypeID,
			int warningType, int integrationType, PageInfo pageInfo,
			EmployeeObj userDNA, List<StandardQuery> querys);

	public String getWarningTotalCountForSum(int contractTypeID,
			String searchName, int integrationType, String begDate,
			String endDate, PageInfo pageInfo, EmployeeObj userObj,
			List<QueryParam> params);

	public String getContractListByIntegrationForCurrency(int contractTypeID,
			String begDate, String endDate, int integrationType,
			String currencyGraph, String subIntegrationType, PageInfo pageInfo,
			EmployeeObj userObj, List<StandardQuery> querys);

	public String getRelatedContractList(int contractID, int contractType,PageInfo pageInfo);

	public String getContractListByProjectID(String projectID);

	public String getOutContractListByProjectID(String projectID);

	public String getContractListByInkling(int contractTypeID,
			List<QueryParam> params, PageInfo pageInfo);

	public String getContractListByQuerys(int contractTypeID,
			List<StandardQuery> query, PageInfo pageInfo);

	public String getCompanyListStroeByInkling(StandardQuery query,
			PageInfo pageInfo);

	public void doSaveRelatedContract(int contract1, int contract2, double amt,
			String remark, String contractNo, int userID);

	public ContractObj getContractInfoByNo(String relatedNo);

	public String getDelayReasonListByContractID(int contractID,
			PageInfo pageInfo);
	
	
	public boolean hasFullRightsOnContract(int contratcID,int contractTypeID, List<StandardQuery> querys);

	/**
	 * 保存推迟付款原因
	 */
	public boolean doUpdateDelayInfo(DelayReason dr);

	/**
	 * 删除推迟付款原因
	 */
	public boolean doDeleteDelayReasonInfo(final DelayReason dr);

	/**
	 * 查询发票拆分信息
	 */
	public String getInvoiceSplitDetailBySimpleInvoice(InvoiceSplitObj iso);

	/**
	 * 查询发票基本拆分信息
	 */
	public String getInvoiceSplitInfoBySimpleInvoice(InvoiceSplitObj iso);

	/**
	 * 删除发票拆分信息
	 */
	public boolean doDeleteInvoiceSplitInfo(final InvoiceSplitObj iso);

	/**
	 * 新建发票拆分信息
	 */
	public boolean doSaveInvoiceSplitInfo(InvoiceSplitObj iso);

	/**
	 * 更新发票拆分信息
	 */
	public boolean doUpdateInvoiceSplitInfo(InvoiceSplitObj iso);

	/**
	 * 得到辅助查询列表
	 */
	public String getIncomeAssistList(List<StandardQuery> query,
			PageInfo pageInfo);

	/**
	 * 修改付款信息
	 */
	public int doSavePaymentDetailQuickly(PaymentDetailObj pay,
			EmployeeObj userDNA, String sourceType, Object obj) throws Exception;
	
	public Map<String,String> getPaymentConditionTotalAmtById(int id);
	
	public String getContractRelationInfo(int pCID,int relatedID);
	
	public void updateRelatedContract(int inContractID,String contractNo,double relateAmt,String remark,int relatedID,int outContractID);
	
	/**
	 * 根据部门ID 查找相应模版
	 */
	public Stencil getStencilByDeptId(int deptID);
	
	/**
	 * 根据合同ID,原部门ID,现部门ID查找模版及摘要信息
	 */
	public Map<String,String> getContractSummaryByConId(int deptID);
	
	/**
	 * 根据部门ID 查找父部门ID
	 */
	public int getParentDeptIdByDeptId(int deptID);

	/**
	 * 通过项目ID查找合同ID
	 * @author LHY
	 * @param projectID
	 * @return contractID
	 */
	public int getContractIDByProjectID(String projectID);
	
	public void doSavePaymentCondition1(int contractID,String store,
			EmployeeObj userDNA) throws Exception;
	
	public double getIncomAmtTotalByGl(String projectID);
	
	/**
	 * 获取合同其他财务来款 
	 * */
	public double getElseIncomAmtTotal(String projectID,int type);
	
	//合同基本信息（项目来源等）
	/**
	 * 通过父ID得到合同基本信息（项目来源等）
	 * @param parentID
	 * */
	public String getContractInfoByParentID(int parentID);
	
	/**
	 * 通过type得到合同基本信息（项目来源等）
	 * @param type
	 * */
	public String getContractInfoByType(int type);
	
	/**
	 * 获得projectCode 
	 * @param parentID
	 * */
	public String getProjectCodeTypeByParentID(int parentID);
	
	/**
	 * 获得部门Code
	 * */
	public String getDeptCode();
	
	/**
	 * 获得项目编码后三位
	 * @param projectNumber
	 * */
	public String getProjectNumber(String projectNumber);
	
	/**
	 * 判断项目编码是否存在
	 * @param projectCode
	 * */
	public boolean existProjectCode(String projectCode);
	
	/**
	 * 通过项目编码获取合同信息
	 * @param projectCode
	 * */
	public String getConInfoByProjectCode(String projectCode);
	
	/**
	 * 按合同ID、分解类型ID删除按合同文本规定分解的金额记录
	 * @param contractID
	 * @param typeID
	 * return result
	 * */
	public int doDeleteConMoneySplit(int contractID,int typeID);
	
	/**
	 * 获取按合同文本规定分解的金额列表
	 * @param contractID
	 * @param type
	 * return list
	 * */
	public String getConTextSplitList(int contractID,int type);
	
	/**
	 * 获取合同状态列表
	 * return list
	 * */
	public String getContractStateList(List<StandardQuery> query, PageInfo pageInfo);
	
	/**
	 * 判断人员是否属于一个部门
	 * @param deptId
	 * @param empId
	 * */
	public boolean isEmpBelongDept(int deptId,int empId);
	
	/**
	 * 获取收付款提示列表
	 * return list
	 * */
	public String getPaymentPromptList();
	
	/**
	 * 获取研究所名称
	 * return list
	 * */
	public String getScientificName(int deptId);
	
	/**
	 * 获得甲方合同编号后三位 
	 * */
	public String getContractCodeNumber(String contractCode);
	
	/**
	 * 判断甲方合同编号是否存在
	 * @param projectCode
	 * */
	public boolean existContractCode(String contractCode);
	
	public void doSaveEstimatedInfo(String projectID,String store,
			EmployeeObj userDNA) throws Exception;
	
	public String getProjectEstimatedPaymentList(String projectID);
	
	public void doDeleteEstimatedInfo(String estimatedIds) throws Exception;
	
	public void doUpdateEstimatedPayedFlag(EstimatedPayment ep);
	
	public double getTotalSumByID(int contractID, int sumType);
	
	
	/**
	 * 通过合同编码获取合同信息
	 * */
	public String getContractInfoByContractCode(String contractCode);
	
	/**
	 * 通过合同ID 获取合同金额
	 * flag == 1  获取合同总额
	 * flag == 2 获取合同的子合同的金额总数
	 * */
	public double getAmtByConID(int contractId,int flag);
	
	/**
	 * 修改支出合同的newflag 新建标记
	 * */
	public void updateContractNewflag(int contractId,int newflag);
	
	/**
	 * 获取收入合同>80万的金额分解列表（计算机信息系统集成项目汇总表）
	 * */
	public String getContractAmtSplitList(List<StandardQuery> querys, PageInfo pageInfo);
	
	/**
	 * 获取支出合同分隔符 '-ZC'
	 * */
	public String getOutContractDelimiter();
	
	/**
	 * 获取收入合同项目编码前缀 'BITC'
	 * */
	public String getIncomeContractPrefix();

	/**
	 * 判断人员部门特殊情况（针对于支出合同，人员除了自己所在部门外是否可修改其他部门的合同）
	 * */
	public boolean isEmpDeptSpecial(int deptId, int empId);
	
	/**
	 * 获取合同来源
	 * */
	public String getContractSourceTypeID(int level);
	
	/**
	 * 获取执行部门信息
	 * */
	public String getExcuteDeptInfo(String deptIds);
	
	/**
	 * 获取支出合同 -ZC' '/Q'
	 * */
	public String getOutContractDelimiterStroe();
}
