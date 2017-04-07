package com.bitc.jk.xmis.model.crm;

import java.sql.Date;

import com.bitc.jk.xmis.model.TableBean;

/**
 * @author liying TableName = "xmis.org_公司规模"; companyScaleID = "公司规模ID";
 *         companyScaleName = "名称"; companyScaleDefinition = "描述";
 */
public class CompanyScale  extends TableBean {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private int companyScaleID;
	private String companyScaleName;
	private String companyScaleDefinition;
	private Date updateDate;
	private int updateID;

	public int getCompanyScaleID() {
		return companyScaleID;
	}

	public void setCompanyScaleID(int companyScaleID) {
		this.companyScaleID = companyScaleID;
	}

	public String getCompanyScaleName() {
		return companyScaleName;
	}

	public void setCompanyScaleName(String companyScaleName) {
		this.companyScaleName = companyScaleName;
	}

	public String getCompanyScaleDefinition() {
		return companyScaleDefinition;
	}

	public void setCompanyScaleDefinition(String companyScaleDefinition) {
		this.companyScaleDefinition = companyScaleDefinition;
	}

	public Date getUpdateDate() {
		return updateDate;
	}

	public void setUpdateDate(Date updateDate) {
		this.updateDate = updateDate;
	}

	public int getUpdateID() {
		return updateID;
	}

	public void setUpdateID(int updateID) {
		this.updateID = updateID;
	}

}
