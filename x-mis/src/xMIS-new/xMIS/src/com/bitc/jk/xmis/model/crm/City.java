package com.bitc.jk.xmis.model.crm;

import com.bitc.jk.xmis.model.TableBean;
import com.bitc.jk.xmis.util.JSONUtils;

/**
 *		TableName = "xmis.add_城市";
		cityID = "城市ID";
		cityCode = "城市编码";
		cityName = "城市名称";
		abbreviation = "城市简称";
		areaCode = "区号";
		provinceID = "省ID";
 */
public class City  extends TableBean{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private Integer cityID;
	private String cityName;
	private String cityCode;
	private String abbreviation;
	private String areaCode;
	private Province province;
	
//	private Integer provinceID;
	
	public Province getProvince() {
		return province;
	}
	public void setProvince(Province province) {
		this.province = province;
	}
	public Integer getCityID() {
		return cityID;
	}
	public void setCityID(Integer cityID) {
		this.cityID = cityID;
	}
	public String getCityName() {
		return cityName;
	}
	public void setCityName(String cityName) {
		this.cityName = cityName;
	}
	public String getCityCode() {
		return cityCode;
	}
	public void setCityCode(String cityCode) {
		this.cityCode = cityCode;
	}
	public String getAbbreviation() {
		return abbreviation;
	}
	public void setAbbreviation(String abbreviation) {
		this.abbreviation = abbreviation;
	}
	public String getAreaCode() {
		return areaCode;
	}
	public void setAreaCode(String areaCode) {
		this.areaCode = areaCode;
	}
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
}
