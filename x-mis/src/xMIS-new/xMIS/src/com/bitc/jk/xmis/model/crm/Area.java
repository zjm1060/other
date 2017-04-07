package com.bitc.jk.xmis.model.crm;

import com.bitc.jk.xmis.model.TableBean;
import com.bitc.jk.xmis.util.JSONUtils;

/**
 * @author liying
 *    [地区ID]
      ,[编码]
      ,[名称]
      ,[简称]
      ,[国家ID]
      ,[最后修改日期]
      ,[最后修改人ID]
  [xmis].[add_地区]
 */
public class Area  extends TableBean{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private Integer areaID;
	private String areaCode;
	private String areaName;
	private String abbreviation;
	private Country country;
//	private Integer countryID;
	
	public Integer getAreaID() {
		return areaID;
	}
	public Country getCountry() {
		return country;
	}
	public void setCountry(Country country) {
		this.country = country;
	}
	public void setAreaID(Integer areaID) {
		this.areaID = areaID;
	}
	public String getAreaCode() {
		return areaCode;
	}
	public void setAreaCode(String areaCode) {
		this.areaCode = areaCode;
	}
	public String getAreaName() {
		return areaName;
	}
	public void setAreaName(String areaName) {
		this.areaName = areaName;
	}
	public String getAbbreviation() {
		return abbreviation;
	}
	public void setAbbreviation(String abbreviation) {
		this.abbreviation = abbreviation;
	}
	
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
}
