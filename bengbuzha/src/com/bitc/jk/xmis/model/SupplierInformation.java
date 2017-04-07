package com.bitc.jk.xmis.model;

import java.util.Date;

import com.bitc.jk.xmis.util.JSONUtils;


public class SupplierInformation extends TableBean{
	
	private static final long serialVersionUID = 588358338404132780L;
	
	/**
	 * 公司ID
	 * */
	private int ID;
	

	/**
	 * 经理ID
	 * */
	private int managerID;
	
	/**
	 * 地址ID
	 * */
	private String addressID;
	
	/**
	 * 主管单位
	 * */
	private String headOfUnit;
	
	/**
	 * 行业评定情况
	 * */
	private String industryAssessment;
	
	/**
	 * 资质情况
	 * */
	private String qualification;
	
	/**
	 * 是否为一般纳税人
	 * */
	private int generalTaxpayer;
	
	/**
	 * 主导产品及产量
	 * */
	private String productsProduction;
	
	/**
	 * 采用标准
	 * */
	private String standard;
	
	/**
	 * 为中水科技公司配套产品
	 * */
	private String ancillaryProducts;
	
	/**
	 * 是否引进线
	 * */
	private int introductionLine;
	
	/**
	 * 成品率
	 * */
	private String Yield;
	
	/**
	 * 质量保证体系运行情况
	 * */
	private String qualityAssurance;
	
	/**
	 * 登记人ID
	 * */
	private SimpleEmployee newregisterID;
	
	/**
	 * 最后修改人ID
	 * */
	private SimpleEmployee lastModifier;
	
	/**
	 * 登记日期
	 * */
	private Date newupdateDate;
	
	/**
	 * 最后修改日期
	 * */
	private Date lastModified;
	


	public int getID() {
		return ID;
	}

	public void setID(int iD) {
		ID = iD;
	}

	public int getManagerID() {
		return managerID;
	}

	public void setManagerID(int managerID) {
		this.managerID = managerID;
	}

	

	public String getAddressID() {
		return addressID;
	}

	public void setAddressID(String addressID) {
		this.addressID = addressID;
	}

	public String getHeadOfUnit() {
		return headOfUnit;
	}

	public void setHeadOfUnit(String headOfUnit) {
		this.headOfUnit = headOfUnit;
	}

	public String getIndustryAssessment() {
		return industryAssessment;
	}

	public void setIndustryAssessment(String industryAssessment) {
		this.industryAssessment = industryAssessment;
	}

	public String getQualification() {
		return qualification;
	}

	public void setQualification(String qualification) {
		this.qualification = qualification;
	}

	public int getGeneralTaxpayer() {
		return generalTaxpayer;
	}

	public void setGeneralTaxpayer(int generalTaxpayer) {
		this.generalTaxpayer = generalTaxpayer;
	}

	public String getProductsProduction() {
		return productsProduction;
	}

	public void setProductsProduction(String productsProduction) {
		this.productsProduction = productsProduction;
	}

	public String getStandard() {
		return standard;
	}

	public void setStandard(String standard) {
		this.standard = standard;
	}

	public String getAncillaryProducts() {
		return ancillaryProducts;
	}

	public void setAncillaryProducts(String ancillaryProducts) {
		this.ancillaryProducts = ancillaryProducts;
	}

	public int getIntroductionLine() {
		return introductionLine;
	}

	public void setIntroductionLine(int introductionLine) {
		this.introductionLine = introductionLine;
	}

	public String getYield() {
		return Yield;
	}

	public void setYield(String yield) {
		Yield = yield;
	}

	public String getQualityAssurance() {
		return qualityAssurance;
	}

	public void setQualityAssurance(String qualityAssurance) {
		this.qualityAssurance = qualityAssurance;
	}



	public SimpleEmployee getNewregisterID() {
		return newregisterID;
	}

	public void setNewregisterID(SimpleEmployee newregisterID) {
		this.newregisterID = newregisterID;
	}

	public SimpleEmployee getLastModifier() {
		return lastModifier;
	}

	public void setLastModifier(SimpleEmployee lastModifier) {
		this.lastModifier = lastModifier;
	}

	public Date getNewupdateDate() {
		return newupdateDate;
	}

	public void setNewupdateDate(Date newupdateDate) {
		this.newupdateDate = newupdateDate;
	}

	public Date getLastModified() {
		return lastModified;
	}

	public void setLastModified(Date lastModified) {
		this.lastModified = lastModified;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
}
