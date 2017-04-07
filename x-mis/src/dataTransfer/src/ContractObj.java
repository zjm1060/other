//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\ContractObj.java

package src;

import java.sql.Date;
import java.util.List;
import java.util.Map;

public class ContractObj 
{
   private int contractID;
   private String contractNo;
   private String contractName;
   private String abbreviation;
   private int parentID;
   private Date signatureDate;
   private int buyerID;
   private String buyer;
   private int deptIDOfBuyer;
   private String deptNameOfBuyer;
   private int vendorID;
   private String vendorName;
   private int deptIDOfVendor;
   private String deptNameOfVendor;
   private double contractAmt;
   private int currencyID;
   private String currencyName;
   private int contractTypeID;
   private String contractTypeName;
   private int buyerRepresentativeID;
   private String buyerRepresentativeSurname;
   private String buyerRepresentativeName;
   private int vendorRepresentativeID;
   private String vendorRepresentativeName;
   private String vendorRepresentativeSurname;
   private String contractSummary;
   private String remark;
   private Date updateDate;
   private int registerID;
   private String registerSurname;
   private String registerName;
   private int sourceTypeID;
   private String sourceTypeName;
   private int govermentContractSourceID;
   private String govermentContractSourceName;
   
   private List parymentConditionList;
   private List paymentList;
   private List invoiceList;
   private List referenceContractList;
   private List projectList;
   
   private Map contractTypeMap;
   private Map contractSourceTypeMap;
   private Map currencyTypeMap;
   private Map currencyCodeMap;
   private String currencyDataStore;
   
   private Map companyMap;
   private String companyDataStore;
   
   private int deliveryID;
   private String deliveryName;
   
   /**
    * 实收总额
    */
   private double earnedTotalAmt;
   
   /**
    * 欠款总额 = 收入合同款 - 实收总额
    */
   private double arrearageTotalAmt;
   
   /**
    * 1) 收入合同时，为已开发票总额
    * 2）支出合同时，为已报销总额
    */
   private double invoicedTotalAmt;
   
   /**
    * 已开票未到账金额 = 已开发票总额 - 实收总额
    */
   private double unCreditedTotalAmt;
   
   /**
    * 已付金额
    */
   private double paidTotalAmt;
   
   /**
    * 欠款总额
    */
   private double unPaidTotalAmt;
   
   
   
   private Date warrantyStartDate;
   private Date warrantyEndDate;
   private String warrantyDefinition;
   private float warrantyLimit;
   private int warrantyUnit;
   
   private ProjectObj projectOBJ;
   private String parentName;
   private String parentCode;
   private String parentAbb;
   
   private boolean hadChild;
   
   /**
 * @return the parentAbb
 */
public String getParentAbb() {
	return parentAbb;
}

/**
 * @param parentAbb the parentAbb to set
 */
public void setParentAbb(String parentAbb) {
	this.parentAbb = parentAbb;
}

/**
    * @roseuid 4C1AD5E5029F
    */
   public ContractObj() 
   {
   }

/**
 * @return the abbreviation
 */
public String getAbbreviation() {
	return abbreviation;
}

/**
 * @param abbreviation the abbreviation to set
 */
public void setAbbreviation(String abbreviation) {
	this.abbreviation = abbreviation;
}

/**
 * @return the arrearageTotalAmt
 */
public double getArrearageTotalAmt() {
	return arrearageTotalAmt;
}

/**
 * @param arrearageTotalAmt the arrearageTotalAmt to set
 */
public void setArrearageTotalAmt(double arrearageTotalAmt) {
	this.arrearageTotalAmt = arrearageTotalAmt;
}

/**
 * @return the buyer
 */
public String getBuyer() {
	return buyer;
}

/**
 * @param buyer the buyer to set
 */
public void setBuyer(String buyer) {
	this.buyer = buyer;
}

/**
 * @return the buyerID
 */
public int getBuyerID() {
	return buyerID;
}

/**
 * @param buyerID the buyerID to set
 */
public void setBuyerID(int buyerID) {
	this.buyerID = buyerID;
}

/**
 * @return the buyerRepresentativeID
 */
public int getBuyerRepresentativeID() {
	return buyerRepresentativeID;
}

/**
 * @param buyerRepresentativeID the buyerRepresentativeID to set
 */
public void setBuyerRepresentativeID(int buyerRepresentativeID) {
	this.buyerRepresentativeID = buyerRepresentativeID;
}

/**
 * @return the buyerRepresentativeName
 */
public String getBuyerRepresentativeName() {
	return buyerRepresentativeName;
}

/**
 * @param buyerRepresentativeName the buyerRepresentativeName to set
 */
public void setBuyerRepresentativeName(String buyerRepresentativeName) {
	this.buyerRepresentativeName = buyerRepresentativeName;
}

/**
 * @return the buyerRepresentativeSurname
 */
public String getBuyerRepresentativeSurname() {
	return buyerRepresentativeSurname;
}

/**
 * @param buyerRepresentativeSurname the buyerRepresentativeSurname to set
 */
public void setBuyerRepresentativeSurname(String buyerRepresentativeSurname) {
	this.buyerRepresentativeSurname = buyerRepresentativeSurname;
}

/**
 * @return the companyDataStore
 */
public String getCompanyDataStore() {
	return companyDataStore;
}

/**
 * @param companyDataStore the companyDataStore to set
 */
public void setCompanyDataStore(String companyDataStore) {
	this.companyDataStore = companyDataStore;
}

/**
 * @return the companyMap
 */
public Map getCompanyMap() {
	return companyMap;
}

/**
 * @param companyMap the companyMap to set
 */
public void setCompanyMap(Map companyMap) {
	this.companyMap = companyMap;
}

/**
 * @return the contractAmt
 */
public double getContractAmt() {
	return contractAmt;
}

/**
 * @param contractAmt the contractAmt to set
 */
public void setContractAmt(double contractAmt) {
	this.contractAmt = contractAmt;
}

/**
 * @return the contractID
 */
public int getContractID() {
	return contractID;
}

/**
 * @param contractID the contractID to set
 */
public void setContractID(int contractID) {
	this.contractID = contractID;
}

/**
 * @return the contractName
 */
public String getContractName() {
	return contractName;
}

/**
 * @param contractName the contractName to set
 */
public void setContractName(String contractName) {
	this.contractName = contractName;
}

/**
 * @return the contractNo
 */
public String getContractNo() {
	return contractNo;
}

/**
 * @param contractNo the contractNo to set
 */
public void setContractNo(String contractNo) {
	this.contractNo = contractNo;
}

/**
 * @return the contractSourceTypeMap
 */
public Map getContractSourceTypeMap() {
	return contractSourceTypeMap;
}

/**
 * @param contractSourceTypeMap the contractSourceTypeMap to set
 */
public void setContractSourceTypeMap(Map contractSourceTypeMap) {
	this.contractSourceTypeMap = contractSourceTypeMap;
}

/**
 * @return the contractSummary
 */
public String getContractSummary() {
	return contractSummary;
}

/**
 * @param contractSummary the contractSummary to set
 */
public void setContractSummary(String contractSummary) {
	this.contractSummary = contractSummary;
}

/**
 * @return the contractTypeID
 */
public int getContractTypeID() {
	return contractTypeID;
}

/**
 * @param contractTypeID the contractTypeID to set
 */
public void setContractTypeID(int contractTypeID) {
	this.contractTypeID = contractTypeID;
}

/**
 * @return the contractTypeMap
 */
public Map getContractTypeMap() {
	return contractTypeMap;
}

/**
 * @param contractTypeMap the contractTypeMap to set
 */
public void setContractTypeMap(Map contractTypeMap) {
	this.contractTypeMap = contractTypeMap;
}

/**
 * @return the contractTypeName
 */
public String getContractTypeName() {
	return contractTypeName;
}

/**
 * @param contractTypeName the contractTypeName to set
 */
public void setContractTypeName(String contractTypeName) {
	this.contractTypeName = contractTypeName;
}

/**
 * @return the currencyDataStore
 */
public String getCurrencyDataStore() {
	return currencyDataStore;
}

/**
 * @param currencyDataStore the currencyDataStore to set
 */
public void setCurrencyDataStore(String currencyDataStore) {
	this.currencyDataStore = currencyDataStore;
}

/**
 * @return the currencyID
 */
public int getCurrencyID() {
	return currencyID;
}

/**
 * @param currencyID the currencyID to set
 */
public void setCurrencyID(int currencyID) {
	this.currencyID = currencyID;
}

/**
 * @return the currencyName
 */
public String getCurrencyName() {
	return currencyName;
}

/**
 * @param currencyName the currencyName to set
 */
public void setCurrencyName(String currencyName) {
	this.currencyName = currencyName;
}

/**
 * @return the currencyTypeMap
 */
public Map getCurrencyTypeMap() {
	return currencyTypeMap;
}

/**
 * @param currencyTypeMap the currencyTypeMap to set
 */
public void setCurrencyTypeMap(Map currencyTypeMap) {
	this.currencyTypeMap = currencyTypeMap;
}

/**
 * @return the deliveryID
 */
public int getDeliveryID() {
	return deliveryID;
}

/**
 * @param deliveryID the deliveryID to set
 */
public void setDeliveryID(int deliveryID) {
	this.deliveryID = deliveryID;
}

/**
 * @return the deliveryName
 */
public String getDeliveryName() {
	return deliveryName;
}

/**
 * @param deliveryName the deliveryName to set
 */
public void setDeliveryName(String deliveryName) {
	this.deliveryName = deliveryName;
}

/**
 * @return the deptIDOfBuyer
 */
public int getDeptIDOfBuyer() {
	return deptIDOfBuyer;
}

/**
 * @param deptIDOfBuyer the deptIDOfBuyer to set
 */
public void setDeptIDOfBuyer(int deptIDOfBuyer) {
	this.deptIDOfBuyer = deptIDOfBuyer;
}

/**
 * @return the deptIDOfVendor
 */
public int getDeptIDOfVendor() {
	return deptIDOfVendor;
}

/**
 * @param deptIDOfVendor the deptIDOfVendor to set
 */
public void setDeptIDOfVendor(int deptIDOfVendor) {
	this.deptIDOfVendor = deptIDOfVendor;
}

/**
 * @return the deptNameOfBuyer
 */
public String getDeptNameOfBuyer() {
	return deptNameOfBuyer;
}

/**
 * @param deptNameOfBuyer the deptNameOfBuyer to set
 */
public void setDeptNameOfBuyer(String deptNameOfBuyer) {
	this.deptNameOfBuyer = deptNameOfBuyer;
}

/**
 * @return the deptNameOfVendor
 */
public String getDeptNameOfVendor() {
	return deptNameOfVendor;
}

/**
 * @param deptNameOfVendor the deptNameOfVendor to set
 */
public void setDeptNameOfVendor(String deptNameOfVendor) {
	this.deptNameOfVendor = deptNameOfVendor;
}

/**
 * @return the earnedTotalAmt
 */
public double getEarnedTotalAmt() {
	return earnedTotalAmt;
}

/**
 * @param earnedTotalAmt the earnedTotalAmt to set
 */
public void setEarnedTotalAmt(double earnedTotalAmt) {
	this.earnedTotalAmt = earnedTotalAmt;
}

/**
 * @return the govermentContractSourceID
 */
public int getGovermentContractSourceID() {
	return govermentContractSourceID;
}

/**
 * @param govermentContractSourceID the govermentContractSourceID to set
 */
public void setGovermentContractSourceID(int govermentContractSourceID) {
	this.govermentContractSourceID = govermentContractSourceID;
}

/**
 * @return the govermentContractSourceName
 */
public String getGovermentContractSourceName() {
	return govermentContractSourceName;
}

/**
 * @param govermentContractSourceName the govermentContractSourceName to set
 */
public void setGovermentContractSourceName(String govermentContractSourceName) {
	this.govermentContractSourceName = govermentContractSourceName;
}

/**
 * @return the invoicedTotalAmt
 */
public double getInvoicedTotalAmt() {
	return invoicedTotalAmt;
}

/**
 * @param invoicedTotalAmt the invoicedTotalAmt to set
 */
public void setInvoicedTotalAmt(double invoicedTotalAmt) {
	this.invoicedTotalAmt = invoicedTotalAmt;
}

/**
 * @return the invoiceList
 */
public List getInvoiceList() {
	return invoiceList;
}

/**
 * @param invoiceList the invoiceList to set
 */
public void setInvoiceList(List invoiceList) {
	this.invoiceList = invoiceList;
}

/**
 * @return the paidTotalAmt
 */
public double getPaidTotalAmt() {
	return paidTotalAmt;
}

/**
 * @param paidTotalAmt the paidTotalAmt to set
 */
public void setPaidTotalAmt(double paidTotalAmt) {
	this.paidTotalAmt = paidTotalAmt;
}

/**
 * @return the parentCode
 */
public String getParentCode() {
	return parentCode;
}

/**
 * @param parentCode the parentCode to set
 */
public void setParentCode(String parentCode) {
	this.parentCode = parentCode;
}

/**
 * @return the parentID
 */
public int getParentID() {
	return parentID;
}

/**
 * @param parentID the parentID to set
 */
public void setParentID(int parentID) {
	this.parentID = parentID;
}

/**
 * @return the parentName
 */
public String getParentName() {
	return parentName;
}

/**
 * @param parentName the parentName to set
 */
public void setParentName(String parentName) {
	this.parentName = parentName;
}

/**
 * @return the parymentConditionList
 */
public List getParymentConditionList() {
	return parymentConditionList;
}

/**
 * @param parymentConditionList the parymentConditionList to set
 */
public void setParymentConditionList(List parymentConditionList) {
	this.parymentConditionList = parymentConditionList;
}

/**
 * @return the paymentList
 */
public List getPaymentList() {
	return paymentList;
}

/**
 * @param paymentList the paymentList to set
 */
public void setPaymentList(List paymentList) {
	this.paymentList = paymentList;
}

/**
 * @return the projectList
 */
public List getProjectList() {
	return projectList;
}

/**
 * @param projectList the projectList to set
 */
public void setProjectList(List projectList) {
	this.projectList = projectList;
}

/**
 * @return the projectOBJ
 */
public ProjectObj getProjectOBJ() {
	return projectOBJ;
}

/**
 * @param projectOBJ the projectOBJ to set
 */
public void setProjectOBJ(ProjectObj projectOBJ) {
	this.projectOBJ = projectOBJ;
}

/**
 * @return the referenceContractList
 */
public List getReferenceContractList() {
	return referenceContractList;
}

/**
 * @param referenceContractList the referenceContractList to set
 */
public void setReferenceContractList(List referenceContractList) {
	this.referenceContractList = referenceContractList;
}

/**
 * @return the registerID
 */
public int getRegisterID() {
	return registerID;
}

/**
 * @param registerID the registerID to set
 */
public void setRegisterID(int registerID) {
	this.registerID = registerID;
}

/**
 * @return the registerName
 */
public String getRegisterName() {
	return registerName;
}

/**
 * @param registerName the registerName to set
 */
public void setRegisterName(String registerName) {
	this.registerName = registerName;
}

/**
 * @return the registerSurname
 */
public String getRegisterSurname() {
	return registerSurname;
}

/**
 * @param registerSurname the registerSurname to set
 */
public void setRegisterSurname(String registerSurname) {
	this.registerSurname = registerSurname;
}

/**
 * @return the remark
 */
public String getRemark() {
	return remark;
}

/**
 * @param remark the remark to set
 */
public void setRemark(String remark) {
	this.remark = remark;
}

/**
 * @return the signatureDate
 */
public Date getSignatureDate() {
	return signatureDate;
}

/**
 * @param signatureDate the signatureDate to set
 */
public void setSignatureDate(Date signatureDate) {
	this.signatureDate = signatureDate;
}

/**
 * @return the sourceTypeID
 */
public int getSourceTypeID() {
	return sourceTypeID;
}

/**
 * @param sourceTypeID the sourceTypeID to set
 */
public void setSourceTypeID(int sourceTypeID) {
	this.sourceTypeID = sourceTypeID;
}

/**
 * @return the sourceTypeName
 */
public String getSourceTypeName() {
	return sourceTypeName;
}

/**
 * @param sourceTypeName the sourceTypeName to set
 */
public void setSourceTypeName(String sourceTypeName) {
	this.sourceTypeName = sourceTypeName;
}

/**
 * @return the unCreditedTotalAmt
 */
public double getUnCreditedTotalAmt() {
	return unCreditedTotalAmt;
}

/**
 * @param unCreditedTotalAmt the unCreditedTotalAmt to set
 */
public void setUnCreditedTotalAmt(double unCreditedTotalAmt) {
	this.unCreditedTotalAmt = unCreditedTotalAmt;
}

/**
 * @return the unPaidTotalAmt
 */
public double getUnPaidTotalAmt() {
	return unPaidTotalAmt;
}

/**
 * @param unPaidTotalAmt the unPaidTotalAmt to set
 */
public void setUnPaidTotalAmt(double unPaidTotalAmt) {
	this.unPaidTotalAmt = unPaidTotalAmt;
}

/**
 * @return the updateDate
 */
public Date getUpdateDate() {
	return updateDate;
}

/**
 * @param updateDate the updateDate to set
 */
public void setUpdateDate(Date updateDate) {
	this.updateDate = updateDate;
}

/**
 * @return the vendorID
 */
public int getVendorID() {
	return vendorID;
}

/**
 * @param vendorID the vendorID to set
 */
public void setVendorID(int vendorID) {
	this.vendorID = vendorID;
}

/**
 * @return the vendorName
 */
public String getVendorName() {
	return vendorName;
}

/**
 * @param vendorName the vendorName to set
 */
public void setVendorName(String vendorName) {
	this.vendorName = vendorName;
}

/**
 * @return the vendorRepresentativeID
 */
public int getVendorRepresentativeID() {
	return vendorRepresentativeID;
}

/**
 * @param vendorRepresentativeID the vendorRepresentativeID to set
 */
public void setVendorRepresentativeID(int vendorRepresentativeID) {
	this.vendorRepresentativeID = vendorRepresentativeID;
}

/**
 * @return the vendorRepresentativeName
 */
public String getVendorRepresentativeName() {
	return vendorRepresentativeName;
}

/**
 * @param vendorRepresentativeName the vendorRepresentativeName to set
 */
public void setVendorRepresentativeName(String vendorRepresentativeName) {
	this.vendorRepresentativeName = vendorRepresentativeName;
}

/**
 * @return the vendorRepresentativeSurname
 */
public String getVendorRepresentativeSurname() {
	return vendorRepresentativeSurname;
}

/**
 * @param vendorRepresentativeSurname the vendorRepresentativeSurname to set
 */
public void setVendorRepresentativeSurname(String vendorRepresentativeSurname) {
	this.vendorRepresentativeSurname = vendorRepresentativeSurname;
}

/**
 * @return the warrantyDefinition
 */
public String getWarrantyDefinition() {
	return warrantyDefinition;
}

/**
 * @param warrantyDefinition the warrantyDefinition to set
 */
public void setWarrantyDefinition(String warrantyDefinition) {
	this.warrantyDefinition = warrantyDefinition;
}

/**
 * @return the warrantyEndDate
 */
public Date getWarrantyEndDate() {
	return warrantyEndDate;
}

/**
 * @param warrantyEndDate the warrantyEndDate to set
 */
public void setWarrantyEndDate(Date warrantyEndDate) {
	this.warrantyEndDate = warrantyEndDate;
}

/**
 * @return the warrantyStartDate
 */
public Date getWarrantyStartDate() {
	return warrantyStartDate;
}

/**
 * @param warrantyStartDate the warrantyStartDate to set
 */
public void setWarrantyStartDate(Date warrantyStartDate) {
	this.warrantyStartDate = warrantyStartDate;
}




/**
 * @return the warrantyLimit
 */
public float getWarrantyLimit() {
	return warrantyLimit;
}

/**
 * @param warrantyLimit the warrantyLimit to set
 */
public void setWarrantyLimit(float warrantyLimit) {
	this.warrantyLimit = warrantyLimit;
}

/**
 * @return the warrantyUnit
 */
public int getWarrantyUnit() {
	return warrantyUnit;
}

/**
 * @param warrantyUnit the warrantyUnit to set
 */
public void setWarrantyUnit(int warrantyUnit) {
	this.warrantyUnit = warrantyUnit;
}

/**
 * @return the currencyCodeMap
 */
public Map getCurrencyCodeMap() {
	return currencyCodeMap;
}

/**
 * @param currencyCodeMap the currencyCodeMap to set
 */
public void setCurrencyCodeMap(Map currencyCodeMap) {
	this.currencyCodeMap = currencyCodeMap;
}

/**
 * @return the hasChild
 */
public boolean isHadChild() {
	return hadChild;
}

/**
 * @param hasChild the hasChild to set
 */
public void setHadChild(boolean hasChild) {
	this.hadChild = hasChild;
}

}
