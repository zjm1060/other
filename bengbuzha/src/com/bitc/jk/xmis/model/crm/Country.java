package com.bitc.jk.xmis.model.crm;

import java.util.Date;

import com.bitc.jk.xmis.model.TableBean;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.util.JSONUtils;

/**
 *     [国家ID]
      ,[编码]
      ,[名称]
      ,[简称]
      ,[货币ID]
      ,[洲际ID]
      ,[最后修改日期]
      ,[最后修改人ID]
  [xmis].[add_国家及地区]
 */
public class Country  extends TableBean{
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private Integer countryID;
	private String countryCode;
	private String countryName;
	private String abbreviation;
	private Currency currency;
	private Internation internation;
	private String areaCode;
	
	private Date updateDate;
	private int updateID;
	
	public String getAreaCode() {
		return areaCode;
	}
	public void setAreaCode(String areaCode) {
		this.areaCode = areaCode;
	}
	public Internation getInternation() {
		return internation;
	}
	public void setInternation(Internation internation) {
		this.internation = internation;
	}
	public static long getSerialversionuid() {
		return serialVersionUID;
	}
	public Integer getCountryID() {
		return countryID;
	}
	public void setCountryID(Integer countryID) {
		this.countryID = countryID;
	}
	public String getCountryCode() {
		return countryCode;
	}
	public void setCountryCode(String countryCode) {
		this.countryCode = countryCode;
	}
	public String getCountryName() {
		return countryName;
	}
	public void setCountryName(String countryName) {
		this.countryName = countryName;
	}
	public String getAbbreviation() {
		return abbreviation;
	}
	public void setAbbreviation(String abbreviation) {
		this.abbreviation = abbreviation;
	}
	public Currency getCurrency() {
		return currency;
	}
	public void setCurrency(Currency currency) {
		this.currency = currency;
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
	
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
}
