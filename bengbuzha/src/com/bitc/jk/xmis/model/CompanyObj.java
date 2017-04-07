//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\CompanyObj.java

package com.bitc.jk.xmis.model;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Set;

import com.bitc.jk.xmis.model.crm.Address;
import com.bitc.jk.xmis.model.crm.BusinessRelation;
import com.bitc.jk.xmis.model.crm.CompanyScale;
import com.bitc.jk.xmis.model.crm.CreditLevel;
import com.bitc.jk.xmis.model.crm.CustomerLevel;
import com.bitc.jk.xmis.model.crm.RelativeAddress;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.util.JSONUtils;

public class CompanyObj extends TableBean {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	private int companyID;

	private String companyName;

	private String abbreviation;

	private String addressID; // "地址ID";

	private String telephoneNo1; // "电话1" ;

	private String faxNo1; // "传真1";

	private String telephoneNo2; // "电话2";

	private String faxNo2; // "传真2";

	private String webSite; // "网址";

	private String emailAddress; // "邮箱";

	private String bankName; // "开户银行";

	private String accountNo; // "账号";

	private String taxNo; // "税号";

	private int creditLevelDefinition; // "信用等级描述";

	private String organizationCode; // "机构代码";

	private String registeredCapital; // "注册资金" ;

	private String remark; // "备注";

	private int customerSourceID; // "客户来源ID" ;

	private String bsinessScope; // "经营范围";

	private int securityLevelID; // "保密级别";

	private Address address;
	private Set<RelativeAddress> otherAddress;
	
	private String outputContactMoney;//支出性金额
	private String inputContactMoney;//收入性金额

	public String getOutputContactMoney() {
		return outputContactMoney;
	}

	public void setOutputContactMoney(String outputContactMoney) {
		this.outputContactMoney = outputContactMoney;
	}

	public String getInputContactMoney() {
		return inputContactMoney;
	}

	public void setInputContactMoney(String inputContactMoney) {
		this.inputContactMoney = inputContactMoney;
	}

	public Set<RelativeAddress> getOtherAddress() {
		return otherAddress;
	}

	public void setOtherAddress(Set<RelativeAddress> otherAddress) {
		this.otherAddress = otherAddress;
	}

	public List<Address> getAllAddress() {
		List<Address> list = new ArrayList<Address>();
		if (this.getAddress() != null) {
			list.add(this.getAddress());
		}
		if (this.getOtherAddress() != null && this.getOtherAddress().size() > 0) {
			for (RelativeAddress ra : this.getOtherAddress()) {
				list.add(ra.getAddress());
			}
		}
		return list;
	}

	public Currency getCurrency() {
		return currency;
	}

	public void setCurrency(Currency currency) {
		this.currency = currency;
	}

	private Currency currency;
	private BusinessRelation businessRelation;
	private SimpleEmployee legalRepresentative; // 法人
	private SimpleEmployee contactPerson;
	private CompanyScale companyScale;
	private CustomerLevel customerLevel;
	private CreditLevel creditLevel;

	private Date updateDate;//
	private SimpleEmployee updatePerson;//

	public SimpleEmployee getUpdatePerson() {
		return updatePerson;
	}

	public void setUpdatePerson(SimpleEmployee updatePerson) {
		this.updatePerson = updatePerson;
	}

	public Date getUpdateDate() {
		return updateDate;
	}

	public void setUpdateDate(Date updateDate) {
		this.updateDate = updateDate;
	}

	/**
	 * @roseuid 4C1F03AE0138
	 */
	public CompanyObj() {

	}

	/**
	 * @return the abbreviation
	 */
	public String getAbbreviation() {
		return abbreviation;
	}

	public BusinessRelation getBusinessRelation() {
		return businessRelation;
	}

	public void setBusinessRelation(BusinessRelation businessRelation) {
		this.businessRelation = businessRelation;
	}

	/**
	 * @param abbreviation
	 *            the abbreviation to set
	 */
	public void setAbbreviation(String abbreviation) {
		this.abbreviation = abbreviation;
	}

	/**
	 * @return the accountNo
	 */
	public String getAccountNo() {
		return accountNo;
	}

	/**
	 * @param accountNo
	 *            the accountNo to set
	 */
	public void setAccountNo(String accountNo) {
		this.accountNo = accountNo;
	}

	public SimpleEmployee getLegalRepresentative() {
		return legalRepresentative;
	}

	public void setLegalRepresentative(SimpleEmployee legalRepresentative) {
		this.legalRepresentative = legalRepresentative;
	}

	/**
	 * @return the addressID
	 */
	public String getAddressID() {
		return addressID;
	}

	/**
	 * @param addressID
	 *            the addressID to set
	 */
	public void setAddressID(String addressID) {
		this.addressID = addressID;
	}

	/**
	 * @return the bankName
	 */
	public String getBankName() {
		return bankName;
	}

	/**
	 * @param bankName
	 *            the bankName to set
	 */
	public void setBankName(String bankName) {
		this.bankName = bankName;
	}

	/**
	 * @return the bsinessScope
	 */
	public String getBsinessScope() {
		return bsinessScope;
	}

	/**
	 * @param bsinessScope
	 *            the bsinessScope to set
	 */
	public void setBsinessScope(String bsinessScope) {
		this.bsinessScope = bsinessScope;
	}

	/**
	 * @return the companyID
	 */
	public int getCompanyID() {
		return companyID;
	}

	/**
	 * @param companyID
	 *            the companyID to set
	 */
	public void setCompanyID(int companyID) {
		this.companyID = companyID;
	}

	/**
	 * @return the companyName
	 */
	public String getCompanyName() {
		return companyName;
	}

	/**
	 * @param companyName
	 *            the companyName to set
	 */
	public void setCompanyName(String companyName) {
		this.companyName = companyName;
	}

	/**
	 * @return the creditLevelDefinition
	 */
	public int getCreditLevelDefinition() {
		return creditLevelDefinition;
	}

	/**
	 * @param creditLevelDefinition
	 *            the creditLevelDefinition to set
	 */
	public void setCreditLevelDefinition(int creditLevelDefinition) {
		this.creditLevelDefinition = creditLevelDefinition;
	}

	/**
	 * @return the customerSourceID
	 */
	public int getCustomerSourceID() {
		return customerSourceID;
	}

	/**
	 * @param customerSourceID
	 *            the customerSourceID to set
	 */
	public void setCustomerSourceID(int customerSourceID) {
		this.customerSourceID = customerSourceID;
	}

	/**
	 * @return the emailAddress
	 */
	public String getEmailAddress() {
		return emailAddress;
	}

	/**
	 * @param emailAddress
	 *            the emailAddress to set
	 */
	public void setEmailAddress(String emailAddress) {
		this.emailAddress = emailAddress;
	}

	/**
	 * @return the faxNo1
	 */
	public String getFaxNo1() {
		return faxNo1;
	}

	/**
	 * @param faxNo1
	 *            the faxNo1 to set
	 */
	public void setFaxNo1(String faxNo1) {
		this.faxNo1 = faxNo1;
	}

	/**
	 * @return the faxNo2
	 */
	public String getFaxNo2() {
		return faxNo2;
	}

	/**
	 * @param faxNo2
	 *            the faxNo2 to set
	 */
	public void setFaxNo2(String faxNo2) {
		this.faxNo2 = faxNo2;
	}

	/**
	 * @return the organizationCode
	 */
	public String getOrganizationCode() {
		return organizationCode;
	}

	/**
	 * @param organizationCode
	 *            the organizationCode to set
	 */
	public void setOrganizationCode(String organizationCode) {
		this.organizationCode = organizationCode;
	}

	/**
	 * @return the registeredCapital
	 */
	public String getRegisteredCapital() {
		return registeredCapital;
	}

	/**
	 * @param registeredCapital
	 *            the registeredCapital to set
	 */
	public void setRegisteredCapital(String registeredCapital) {
		this.registeredCapital = registeredCapital;
	}

	/**
	 * @return the remark
	 */
	public String getRemark() {
		return remark;
	}

	/**
	 * @param remark
	 *            the remark to set
	 */
	public void setRemark(String remark) {
		this.remark = remark;
	}

	/**
	 * @return the securityLevelID
	 */
	public int getSecurityLevelID() {
		return securityLevelID;
	}

	/**
	 * @param securityLevelID
	 *            the securityLevelID to set
	 */
	public void setSecurityLevelID(int securityLevelID) {
		this.securityLevelID = securityLevelID;
	}

	/**
	 * @return the taxNo
	 */
	public String getTaxNo() {
		return taxNo;
	}

	/**
	 * @param taxNo
	 *            the taxNo to set
	 */
	public void setTaxNo(String taxNo) {
		this.taxNo = taxNo;
	}

	/**
	 * @return the telephoneNo1
	 */
	public String getTelephoneNo1() {
		return telephoneNo1;
	}

	/**
	 * @param telephoneNo1
	 *            the telephoneNo1 to set
	 */
	public void setTelephoneNo1(String telephoneNo1) {
		this.telephoneNo1 = telephoneNo1;
	}

	/**
	 * @return the telephoneNo2
	 */
	public String getTelephoneNo2() {
		return telephoneNo2;
	}

	/**
	 * @param telephoneNo2
	 *            the telephoneNo2 to set
	 */
	public void setTelephoneNo2(String telephoneNo2) {
		this.telephoneNo2 = telephoneNo2;
	}

	/**
	 * @return the webSite
	 */
	public String getWebSite() {
		return webSite;
	}

	/**
	 * @param webSite
	 *            the webSite to set
	 */
	public void setWebSite(String webSite) {
		this.webSite = webSite;
	}

	public Address getAddress() {
		return address;
	}

	public void setAddress(Address address) {
		this.address = address;
	}

	public SimpleEmployee getContactPerson() {
		return contactPerson;
	}

	public void setContactPerson(SimpleEmployee contactPerson) {
		this.contactPerson = contactPerson;
	}

	public CompanyScale getCompanyScale() {
		return companyScale;
	}

	public void setCompanyScale(CompanyScale companyScale) {
		this.companyScale = companyScale;
	}

	public CustomerLevel getCustomerLevel() {
		return customerLevel;
	}

	public void setCustomerLevel(CustomerLevel customerLevel) {
		this.customerLevel = customerLevel;
	}

	public CreditLevel getCreditLevel() {
		return creditLevel;
	}

	public void setCreditLevel(CreditLevel creditLevel) {
		this.creditLevel = creditLevel;
	}

	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}

}
