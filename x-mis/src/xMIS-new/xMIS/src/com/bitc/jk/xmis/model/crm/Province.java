package com.bitc.jk.xmis.model.crm;

import com.bitc.jk.xmis.model.TableBean;
import com.bitc.jk.xmis.util.JSONUtils;

/**
 * @author liying
 * 		TableName = "xmis.add_省";
		provinceID = "省ID";
		provinceCode = "省编码";
		provinceName = "省名称";
		abbreviation = "省简称";
		areaID = "地区ID";
 *
 */
public class Province extends TableBean{
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private Integer provinceID;
	private String provinceName;
	private String provinceCode;
	private String abbreviation;
	private Area area;
//	private Integer areaID;
	
	public Area getArea() {
		return area;
	}
	public void setArea(Area area) {
		this.area = area;
	}
	public String getProvinceName() {
		return provinceName;
	}
	public void setProvinceName(String provinceName) {
		this.provinceName = provinceName;
	}
	public String getProvinceCode() {
		return provinceCode;
	}
	public void setProvinceCode(String provinceCode) {
		this.provinceCode = provinceCode;
	}
	public String getAbbreviation() {
		return abbreviation;
	}
	public void setAbbreviation(String abbreviation) {
		this.abbreviation = abbreviation;
	}
	public Integer getProvinceID() {
		return provinceID;
	}
	public void setProvinceID(Integer provinceID) {
		this.provinceID = provinceID;
	}
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
	
}
