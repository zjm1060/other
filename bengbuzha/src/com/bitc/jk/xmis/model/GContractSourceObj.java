package com.bitc.jk.xmis.model;

public class GContractSourceObj {
	
	private int govermentContractSourceID ;  // "横向合同来源ID";
	private String govermentContractSourceName ;  // "名称";
	private int companyID ;  // "来源单位ID";
	private int departmentID ;  // "来源部门ID";
	private double amt ;  // "资助额度上限";
	private int currencyTypeID ;  // "货币ID";
	private String specialty ;  // "特点";
	private String template ;  // "申请模板";
	
	private int contractID;
	private String selected;
	
	/**
	 * @return the amt
	 */
	public double getAmt() {
		return amt;
	}
	/**
	 * @param amt the amt to set
	 */
	public void setAmt(double amt) {
		this.amt = amt;
	}
	/**
	 * @return the companyID
	 */
	public int getCompanyID() {
		return companyID;
	}
	/**
	 * @param companyID the companyID to set
	 */
	public void setCompanyID(int companyID) {
		this.companyID = companyID;
	}
	/**
	 * @return the currencyTypeID
	 */
	public int getCurrencyTypeID() {
		return currencyTypeID;
	}
	/**
	 * @param currencyTypeID the currencyTypeID to set
	 */
	public void setCurrencyTypeID(int currencyTypeID) {
		this.currencyTypeID = currencyTypeID;
	}
	/**
	 * @return the departmentID
	 */
	public int getDepartmentID() {
		return departmentID;
	}
	/**
	 * @param departmentID the departmentID to set
	 */
	public void setDepartmentID(int departmentID) {
		this.departmentID = departmentID;
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
	 * @return the specialty
	 */
	public String getSpecialty() {
		return specialty;
	}
	/**
	 * @param specialty the specialty to set
	 */
	public void setSpecialty(String specialty) {
		this.specialty = specialty;
	}
	/**
	 * @return the template
	 */
	public String getTemplate() {
		return template;
	}
	/**
	 * @param template the template to set
	 */
	public void setTemplate(String template) {
		this.template = template;
	}

	/**
	 * @return the selected
	 */
	public String getSelected() {
		return selected;
	}
	/**
	 * @param selected the selected to set
	 */
	public void setSelected(String selected) {
		this.selected = selected;
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
}
