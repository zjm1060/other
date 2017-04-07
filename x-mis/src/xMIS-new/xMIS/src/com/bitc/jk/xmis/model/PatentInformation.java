package com.bitc.jk.xmis.model;

import java.sql.Date;

import com.bitc.jk.xmis.util.JSONUtils;

public class PatentInformation extends TableBean{
	
	private static final long serialVersionUID = 588358338404132780L;
	
	/**
	 * 主键
	 * */
	private String sjid;
	
	/**
	 * 公司ID 
	 * */
	private int companyID;
	
	/**
	 * 专利名称
	 * */
	
	private String patentName;
	
	/**
	 * 类别
	 * */
	
	private String patentsCategory;
	
	/**
	 * 设计人1ID
	 * */
	
	private int patentedDesignPeopleID1;
	
	/**
	 * 设计人2ID
	 * */
	
	private int patentedDesignPeopleID2;
	
	/**
	 * 设计人3ID
	 * */
	
	private int patentedDesignPeopleID3;
	
	/**
	 * 设计人4ID
	 * */
//	
//	private int patentedDesignPeopleID4;
//	
//	/**
//	 * 设计人5ID
//	 * */
//	private int patentedDesignPeopleID5;
	/**
	 * 专利权人1ID
	 * */
	
	private int patentRightPeopleID1;
	
	/**
	 * 专利权人2ID
	 * */
	
	private int patentRightPeopleID2;
	
	/**
	 * 专利权人3ID
	 * */
	
	private int patentRightPeopleID3;
	
	/**
	 * 专利权人4ID
	 * */
	private int patentRightPeopleID4;
	
	/**
	 * 专利权人5ID
	 * */
	private int patentRightPeopleID5;
	/**
	 * 专利号
	 * */
	
	private String patentNumber;
	
	/**
	 * 申请日
	 * */
	
	private Date applicationTime;
	
	/**
	 * 证书号
	 * */
	
	private String patentCertificateNo;
	
	/**
	 * 授权公告日
	 * */
	private Date authorizeTime;
	
	/**
	 * 有效期起
	 * */
	
	private Date validFrom;
	
	/**
	 * 有效期止
	 * */
	private Date validOnly;
	
	/**
	 * 登记人ID 
	 * */
	
	private SimpleEmployee patentregisterName;
	
	/**
	 * 登记日期
	 * */
	private Date patentupdateDate;
	
	/**
	 * 最后修改人ID
	 * */
	
	private SimpleEmployee patentlastModifierName;
	
	/**
	 * 最后修改日期
	 * */
	
	private Date patentlastModified;



	public String getSjid() {
		return sjid;
	}

	public void setSjid(String sjid) {
		this.sjid = sjid;
	}

	public int getCompanyID() {
		return companyID;
	}

	public void setCompanyID(int companyID) {
		this.companyID = companyID;
	}

	public String getPatentName() {
		return patentName;
	}

	public void setPatentName(String patentName) {
		this.patentName = patentName;
	}

	public String getPatentsCategory() {
		return patentsCategory;
	}

	public void setPatentsCategory(String patentsCategory) {
		this.patentsCategory = patentsCategory;
	}



	

	public int getPatentedDesignPeopleID1() {
		return patentedDesignPeopleID1;
	}

	public void setPatentedDesignPeopleID1(int patentedDesignPeopleID1) {
		this.patentedDesignPeopleID1 = patentedDesignPeopleID1;
	}

	public int getPatentedDesignPeopleID2() {
		return patentedDesignPeopleID2;
	}

	public void setPatentedDesignPeopleID2(int patentedDesignPeopleID2) {
		this.patentedDesignPeopleID2 = patentedDesignPeopleID2;
	}

	public int getPatentedDesignPeopleID3() {
		return patentedDesignPeopleID3;
	}

	public void setPatentedDesignPeopleID3(int patentedDesignPeopleID3) {
		this.patentedDesignPeopleID3 = patentedDesignPeopleID3;
	}

//	public int getPatentedDesignPeopleID4() {
//		return patentedDesignPeopleID4;
//	}
//
//	public void setPatentedDesignPeopleID4(int patentedDesignPeopleID4) {
//		this.patentedDesignPeopleID4 = patentedDesignPeopleID4;
//	}
//
//	public int getPatentedDesignPeopleID5() {
//		return patentedDesignPeopleID5;
//	}
//
//	public void setPatentedDesignPeopleID5(int patentedDesignPeopleID5) {
//		this.patentedDesignPeopleID5 = patentedDesignPeopleID5;
//	}

	public int getPatentRightPeopleID1() {
		return patentRightPeopleID1;
	}

	public void setPatentRightPeopleID1(int patentRightPeopleID1) {
		this.patentRightPeopleID1 = patentRightPeopleID1;
	}

	public int getPatentRightPeopleID2() {
		return patentRightPeopleID2;
	}

	public void setPatentRightPeopleID2(int patentRightPeopleID2) {
		this.patentRightPeopleID2 = patentRightPeopleID2;
	}

	public int getPatentRightPeopleID3() {
		return patentRightPeopleID3;
	}

	public void setPatentRightPeopleID3(int patentRightPeopleID3) {
		this.patentRightPeopleID3 = patentRightPeopleID3;
	}

	public int getPatentRightPeopleID4() {
		return patentRightPeopleID4;
	}

	public void setPatentRightPeopleID4(int patentRightPeopleID4) {
		this.patentRightPeopleID4 = patentRightPeopleID4;
	}

	public int getPatentRightPeopleID5() {
		return patentRightPeopleID5;
	}

	public void setPatentRightPeopleID5(int patentRightPeopleID5) {
		this.patentRightPeopleID5 = patentRightPeopleID5;
	}

	public Date getValidFrom() {
		return validFrom;
	}

	public void setValidFrom(Date validFrom) {
		this.validFrom = validFrom;
	}

	public Date getValidOnly() {
		return validOnly;
	}

	public void setValidOnly(Date validOnly) {
		this.validOnly = validOnly;
	}

	public String getPatentNumber() {
		return patentNumber;
	}

	public void setPatentNumber(String patentNumber) {
		this.patentNumber = patentNumber;
	}

	public Date getApplicationTime() {
		return applicationTime;
	}

	public void setApplicationTime(Date applicationTime) {
		this.applicationTime = applicationTime;
	}

	public String getPatentCertificateNo() {
		return patentCertificateNo;
	}

	public void setPatentCertificateNo(String patentCertificateNo) {
		this.patentCertificateNo = patentCertificateNo;
	}

	public Date getAuthorizeTime() {
		return authorizeTime;
	}

	public void setAuthorizeTime(Date authorizeTime) {
		this.authorizeTime = authorizeTime;
	}

	

	public SimpleEmployee getPatentregisterName() {
		return patentregisterName;
	}

	public void setPatentregisterName(SimpleEmployee patentregisterName) {
		this.patentregisterName = patentregisterName;
	}

	public Date getPatentupdateDate() {
		return patentupdateDate;
	}

	public void setPatentupdateDate(Date patentupdateDate) {
		this.patentupdateDate = patentupdateDate;
	}

	public SimpleEmployee getPatentlastModifierName() {
		return patentlastModifierName;
	}

	public void setPatentlastModifierName(SimpleEmployee patentlastModifierName) {
		this.patentlastModifierName = patentlastModifierName;
	}

	public Date getPatentlastModified() {
		return patentlastModified;
	}

	public void setPatentlastModified(Date patentlastModified) {
		this.patentlastModified = patentlastModified;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
	
	
	
	
}
