package com.bitc.jk.xmis.model;

import java.sql.Date;

import com.bitc.jk.xmis.util.JSONUtils;

public class QualificationObj extends TableBean{

	private static final long serialVersionUID = 588358338404132780L;
	
	/**
	 * ID 主键
	 * */
	private String id;
	
	/**
	 * 公司ID 
	  * */
	private int companyID;
	
	/**
	 * 资质名称
	 * */
	private String qualificationName;
	
	/**
	 * 管理部门
	 * */
	private String departmentName;
	
	/**
	 * 联系人ID 
	 * */
	private int contactID;
	
	/**
	 * 联系方式
	 * */
	private String contactMode;
	
	/**
	 * 证书有效期起
	 * */
	private Date certificateValidQi;
	
	/**
	 * 证书有效期止
	 * */
	private Date certificateValidZhi;
	
	/**
	 * 证书号
	 * */
	private String certificateNo;
	
	/**
	 * 是否年审 
	 * */
	private int examined;
	
	/**
	 * 备注
	 * */
	private String remark;
	
	/**
	 * 登记人ID
	 * */
	private SimpleEmployee subscriberID;
	
	/**
	 * 登记日期
	 * */
	private Date subscriberDate;
	
	/**
	 * 最后修改人ID
	 * */
	private SimpleEmployee lastModifier;
	
	/**
	 * 最后修改日期
	 * */
	private Date lastModified;



	

	public int getCompanyID() {
		return companyID;
	}

	public void setCompanyID(int companyID) {
		this.companyID = companyID;
	}






	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getQualificationName() {
		return qualificationName;
	}

	public void setQualificationName(String qualificationName) {
		this.qualificationName = qualificationName;
	}

	public String getDepartmentName() {
		return departmentName;
	}

	public void setDepartmentName(String departmentName) {
		this.departmentName = departmentName;
	}

	public int getContactID() {
		return contactID;
	}

	public void setContactID(int contactID) {
		this.contactID = contactID;
	}

	public String getContactMode() {
		return contactMode;
	}

	public void setContactMode(String contactMode) {
		this.contactMode = contactMode;
	}

	public Date getCertificateValidQi() {
		return certificateValidQi;
	}

	public void setCertificateValidQi(Date certificateValidQi) {
		this.certificateValidQi = certificateValidQi;
	}

	public Date getCertificateValidZhi() {
		return certificateValidZhi;
	}

	public void setCertificateValidZhi(Date certificateValidZhi) {
		this.certificateValidZhi = certificateValidZhi;
	}

	public String getCertificateNo() {
		return certificateNo;
	}

	public void setCertificateNo(String certificateNo) {
		this.certificateNo = certificateNo;
	}

	public int getExamined() {
		return examined;
	}

	public void setExamined(int examined) {
		this.examined = examined;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public SimpleEmployee getSubscriberID() {
		return subscriberID;
	}

	public void setSubscriberID(SimpleEmployee subscriberID) {
		this.subscriberID = subscriberID;
	}

	public Date getSubscriberDate() {
		return subscriberDate;
	}

	public void setSubscriberDate(Date subscriberDate) {
		this.subscriberDate = subscriberDate;
	}

	public SimpleEmployee getLastModifier() {
		return lastModifier;
	}

	public void setLastModifier(SimpleEmployee lastModifier) {
		this.lastModifier = lastModifier;
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
