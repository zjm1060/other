//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\ContractDAO.java

package com.bitc.jk.xmis.dao;

import java.util.List;
import java.util.Map;

import com.bitc.jk.xmis.model.AttachmentFile;
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
import com.bitc.jk.xmis.model.Stencil;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface ContractDAO {

	/**
	 * @param userObj
	 * TODO
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

	public List getGLPurchaseDetailByID(String projectID, PageInfo pageInfo);

	/**
	 * 查询收入合同财务信息
	 * 
	 * @param projectID
	 * @param pageInfo
	 * @return
	 */
	public List getGLIncomeDetailByID(String projectID, PageInfo pageInfo);
	
	/**
	 * 查询收入合同其他财务来款信息
	 * 
	 * @param projectID
	 * @param pageInfo
	 * @return
	 */
	public List getGLElseIncomeDetailByID(String projectID, PageInfo pageInfo,int type);

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
			List<StandardQuery> query, PageInfo pageInfo);

	public List getParentContractListPositionByID(int contractTypeID,
			int parentID, String searchName, List<StandardQuery> query,
			PageInfo pageInfo);

	public int existsContractByGST(int stID);

	public void doDeleteGContrctSournce(int stID, EmployeeObj userDNA);

	public List getAttachedListByContractID(int contractID,
			int attachmentTypeID, PageInfo pageInfo);
	
	
	public int doSaveContractAttachment(AttachmentFile attFile);
		

//	public int getAttachmentTypeID(String tableName, String fieldName);
	public List getAttachmentTypeID(String tableName, String fieldName);

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

	public int doCreateNewProjectStatus(String projectId, String statusName,
			String desc, int userID);

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

	public List getDelayReasonListByContractID(int contractID, PageInfo pageInfo);

	public List getReceiptListAsInvoiceByID(int contractID, PageInfo pageInfo);

	public int doCreateNewInvoiceDetailNull(int contractID, int userID);

	public int doCreateNewReceiptDetailNull(int contractID, int userID);

	public int doSaveInvoiceDetail(InvoiceObj pay, int userID);

	public int doSaveReceiptDetail(InvoiceObj pay, int userID);

	public void doDelInvoiceDetail(int contractID, int invoiceDetailID,
			int userID);

	public void doDelReceiptDetail(int contractID, int invoiceDetailID,
			int userID);

	public int doCreateNewPaymentDetailNull(int contractID, int userID);

	public int doSavePaymentDetail(PaymentDetailObj pay, int userID);

	public void doDelPaymentDetail(int contractID, int paymentDetailID,
			int userID);

	public List getContractListByTypeFilterWarning(int contractTypeID,
			int warningType, PageInfo pageInfo, int userID,
			List<QueryParam> params);

	public List getContractListByTypeForCurrency(int contractTypeID,
			String searchName, String currencyFlag, PageInfo pageInfo,
			int userID);

	/**
	 * 合同查询中，高级查询的新签合同
	 * @param contractTypeID
	 * @param begDate
	 * @param endDate
	 * @param pageInfo
	 * @param userID
	 * @param querys
	 * @return
	 */
	public List getContractListByIntegrationNewType(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys);

	/**
	 * 合同查询中，高级查询的收付款情况
	 * @param contractTypeID
	 * @param begDate
	 * @param endDate
	 * @param pageInfo
	 * @param userID
	 * @param querys
	 * @return
	 */
	public List getContractListByIntegrationPerformed(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys);
	
	/**
	 * 合同查询中，高级查询的预计来款
	 * @param contractTypeID
	 * @param begDate
	 * @param endDate
	 * @param pageInfo
	 * @param userID
	 * @param querys
	 * @return
	 */
	public List getContractListByIntegrationEstimateContractAmt(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys);
	/**
	 * 合同查询中，高级查询的发票情况
	 * @param contractTypeID
	 * @param begDate
	 * @param endDate
	 * @param pageInfo
	 * @param userID
	 * @param querys
	 * @return
	 */
	public List getContractListByIntegrationInvoice(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys);
	
	/**
	 * 合同查询中，高级查询的财务来款情况
	 * @param contractTypeID
	 * @param begDate
	 * @param endDate
	 * @param pageInfo
	 * @param userID
	 * @param querys
	 * @return
	 */
	public List getContractListByIntegrationFinance(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys);

	/**
	 * 合同查询中，高级查询的质保到期
	 * @param contractTypeID
	 * @param begDate
	 * @param endDate
	 * @param pageInfo
	 * @param userID
	 * @param querys
	 * @return
	 */
	public List getContractListByIntegrationWarranty(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys);

	public List getSumInfoByIntegrationNewType(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys);

	public List getSumInfoByIntegrationPerformed(int contractTypeID,
			String begDate, String endDate, PageInfo pageInfo, int userID,
			List<StandardQuery> querys);

	public List getSumInfoByIntegrationSimpleType(int contractTypeID,
			List<StandardQuery> querys, PageInfo pageInfo, int userID);

	public List getWarningTotalCountForIntegrationSimpleType(
			int contractTypeID, String searchName, PageInfo pageInfo, int userID);

	public List getWarningTotalCountForIntegrationSimpleType(
			int contractTypeID, String searchName, PageInfo pageInfo,
			int userID, List<QueryParam> params);

	public List getContractListByIntegrationNewTypeForCurrency(
			int contractTypeID, String begDate, String endDate,
			String currencyFlag, PageInfo pageInfo, int userID,
			List<StandardQuery> querys);

	public List getContractListByIntegrationPerformedForCurrency(
			int contractTypeID, String begDate, String endDate,
			String currencyFlag, String subIntegrationType, PageInfo pageInfo,
			int userID, List<StandardQuery> querys);

	public List getRelatedContractList(int contractID, int contractType,PageInfo pageInfo);

	public List getContractListByProjectID(String projectID);

	public List getOutContractListByProjectID(String projectID);

	public List getContractListByInkling(int contractTypeID,
			List<QueryParam> params, PageInfo pageInfo);

	public List getContractListByQuerys(int contractTypeID,
			List<StandardQuery> querys, PageInfo pageInfo);

	public boolean hasFullRightsOnConcrat(int contractID,int contractTypeID,
			List<StandardQuery> querys);

	public List getCompanyListByInkling(StandardQuery query, PageInfo pageInfo);

	public void doSaveRelatedContract(int incomeContractID, int outContractID,
			double amt, String remark, int contractID);

	public ContractObj getContractInfoByNo(String relatedNo);

	public double getIncomAmtTotalByGl(String projectID);

	/**
	 * 获取合同其他财务来款
	 * */
	public double getElseIncomAmtTotal(String projectID,int type);
	
	public double getPurAmtTotalByGl(String projectID);

	public int getWarningDays(int contractID);

	public boolean doUpdateDelayInfo(DelayReason dr);

	public boolean doDeleteDelayReasonInfo(final DelayReason dr);

	/**
	 * 查询发票拆分信息
	 */
	public List getInvoiceSplitDetailBySimpleInvoice(InvoiceSplitObj iso);

	/**
	 * 查询发票基本拆分信息
	 */
	public List getInvoiceSplitInfoBySimpleInvoice(InvoiceSplitObj iso);

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
	public List getIncomeAssistList(List<StandardQuery> querys,
			PageInfo pageInfo);

	/**
	 * 更新‘发票拆分’付款状态
	 */
	public boolean doUpdateInvoiceSplitStatus(final InvoiceSplitObj iso);

	/**
	 * 通过合同ID 查找付款条件定义的预计金额总和
	 */
	public Map<String,String> getPaymentConditionTotalAmtById(int id);
	
	public List getContractRelationInfo(int pCID,int relatedID);
	
	public void updateRelatedContract(int inContractID,String contractNo,double relateAmt,String remark,int relatedID,int outContractID);
	
	/**
	 * 根据部门ID 查找相应模版
	 */
	public Stencil getStencilByDeptId(int deptID);

	/**
	 * 根据合同ID 查找相应摘要信息
	 */
	public String getContractSummaryByConId(int conID);

	/**
	 * 更新付款条件的预计到款日期
	 * 
	 * @param projectID
	 * @param id
	 */
	public void doUpdatePaymentConditionDate(String projectID, Integer id);
	
	/**
	 * 根据部门ID 查找父部门ID
	 */
	public int getParentDeptIdByDeptId(int deptID);
	
	/**
	 * 更新项目状态名称
	 */
	public void doUpdateProjectStatusName(int psid,String name);

	
	
	/**
	 * 通过项目ID查找合同ID
	 * @param projectID
	 * 		     项目ID
	 * @return the id of contract
	 */
	public int getContractIDByProjectID(String projectID);
	
	/**
	 * 保存手动设置的预计来款信息
	 * @param EstimateIncomeObj
	 * 		     预计来款信息对象
	 * @return void
	 * 
	 * @see com.bitc.jk.xmis.model.EstimateIncomeObj
	 */
	public void doSaveEstimatedInfo(EstimatedPayment ep);
	
	//合同基本信息（项目来源等）
	/**
	 * 通过父ID得到合同基本信息（项目来源等）
	 * @param parentID
	 * */
	public List getContractInfoByParentID(int parentID);
	
	/**
	 * 通过type得到合同基本信息（项目来源等）
	 * @param type
	 * */
	public List getContractInfoByType(int type);
	
	/**
	 * 保存合同基本信息（项目来源等）
	 * @param conInfoObj
	 * */
	public void doSaveContractInfoObj(ContractInfoObj conInfoObj);
	
	/**
	 * 获得合同基本信息（项目来源等）
	 * @param contractID
	 * */
	public Map getContractInfoObj(int contractID);
	
	/**
	 * 获得projectCode 
	 * @param parentID
	 * */
	public List getProjectCodeTypeByParentID(int parentID);
	
	/**
	 * 获得部门Code
	 * */
	public List getDeptCode();
	
	/**
	 * 获得项目编码后三位
	 * @param projectNumber
	 * */
	public List getProjectNumber(String projectNumber);
	
	/**
	 * 判断项目编码是否存在
	 * @param projectCode
	 * */
	public int existProjectNumber(String projectCode);
	
	/**
	 * 通过项目编码获取合同信息
	 * @param projectCode
	 * */
	public List getConInfoByProjectCode(String projectCode);
	
	/**
	 * insert new合同文本自定义的金额类型（在科质部资质管理字典表中）
	 * @param type
	 * */
	public int doCreateNewConQualifyManageNull(int type);
	
	/**
	 *  insert null记录（在合同金额分解表中）
	 * @param contractID,typeID,userID
	 * */
	public void doCreateNewConTextSplitNull(int contractID,int typeID,int userID);
	
	/**
	 * 保存金额类型（在科质部字典表中）
	 * @param moneySplit
	 * */
	public void doSaveConQualifyManage(final MoneySplitObj moneySplit);
	
	/**
	 * 保存金额分解
	 * @param moneySplit
	 * */
	public void doSaveConMoneySplit(final MoneySplitObj moneySplit);
	
	/**
	 * 删除按合同文本规定分解的金额记录
	 * @param contractID
	 * @param typeID
	 * return result
	 * */
	public void doDeleteConMoneySplit(int contractID,int typeID);
	
	/**
	 * 删除科质部资质管理字典表按合同文本自定的金额类型记录
	 * @param typeID
	 * */
	public void doDeleteQualifyManage(int typeID);
	
	/**
	 * 获取按合同文本规定分解的金额列表
	 * @param contractID
	 * @param type
	 * return list
	 * */
	public List getConTextSplitList(final int contractID,final int type);
	
	/**
	 * 判断分解的金额类型是否存在
	 * @param contractID
	 * @param typeID
	 * */
	public int existMoneySplit(int contractID,int typeID);
	
	/**
	 * 获取合同状态列表
	 * return list
	 * */
	public List getContractStateList(List<StandardQuery> querys, PageInfo pageInfo);
	
	/**
	 * 判断人员是否属于一个部门
	 * @param deptId
	 * @param empId
	 * */
	public int isEmpBelongDept(int deptId,int EmpId);
	
	/**
	 * 获取收付款提示列表
	 * return list
	 * */
	public List getPaymentPromptList();
	
	/**
	 * 获取研究所名称
	 * */
	public List getScientificName(int deptId);
	
	
	/**
	 * 获取甲方公司编码
	 * */
	public List getContractCodeNumber(String contractCode);
	
	/**
	 * 判断合同编号是否存在
	 * @param projectCode
	 * */
	public int existContractCode(String contractCode);
		
	/**
	 * 修改手动设置的预计来款信息
	 * @param ep
	 * 预计来款信息对象
	 * @return void
	 * 
	 * @see com.bitc.jk.xmis.model.EstimatedPayment
	 */
	public void doUpdateEstimatedInfo(EstimatedPayment ep);
	
	/**
	 * 得到项目手动设置的预计来款列表
	 * @param projectID
	 * @return
	 */
	public List getProjectEstimatedPaymentList(String projectID);
	
	public void doDeleteEstimatedInfo(String estimatedID);
	
	public void doUpdateEstimatedPayedFlag(EstimatedPayment ep);
	
	public void doUpdatePaymentConditionPayedFlag(PaymentConditionObj pc);
	public void doUpdateInvoicePayedFlag(InvoiceObj iv);
	public void doUpdateReceiptPayedFlag(InvoiceObj rp);
	
	/**
	 * 通过合同编码获取合同信息
	 * */
	public List getContractInfoByContractCode(String contractCode);	
	
	/**
	 * 通过合同ID 获取合同金额
	 * */
	public double getAmtByConID(int contractId,int flag);
	
	/**
	 * 修改支出合同的newflag 新建标记
	 * */
	public void updateContractNewflag(int contractId,int newflag);
	
	/**
	 * 获取收入合同>80万的金额分解列表（计算机信息系统集成项目汇总表）
	 * */
	public List getContractAmtSplitListByQuerys(
			List<StandardQuery> querys, PageInfo pageInfo);
	
	/**
	 * 获取支出合同分隔符 '-ZC'
	 * */
	public String getOutContractDelimiter();
	
	/**
	 * 获取收入合同前缀 'BITC'
	 * */
	public String getIncomeContractPrefix();
	
	/**
	 * 修改支出合同主合同对应的支出子合同信息(**当支出合同主合同有改动时，当前方法也需要做相应的调整**)
	 * */
	public void doSavePayoutContractInfo(final ContractObj con, final int userID); 

	/**
	 * 判断人员部门特殊情况（针对于支出合同，人员除了自己所在部门外是否可修改其他部门的合同）
	 * */
	public int isEmpDeptSpecial(int deptId, int empId);

	public int doUpdateContractFlagByProjectID(String projectID) ;

	/**
	 * 获取合同来源
	 * */
	public List getContractSourceTypeID(int level);
	
	/**
	 * 获取执行部门信息列表
	 * */
	public List getExcuteDeptInfo(String deptIds);
	
	/**
	 * 获取支出合同 -ZC' '/Q'
	 * */
	public List getOutContractDelimiterStroe();
	
	/**
	 * 根据传入的合同的ID获取对应的部门的code（编码）、合同（项目）的名称
	 */
	public Map getContractMessage(String contractID);
	
	/**
	 * 根据传入的文件名判断数据库中是否存在该文件
	 * @param fileName
	 * @return
	 */
	public boolean fileIsExist(String fileName);
}
