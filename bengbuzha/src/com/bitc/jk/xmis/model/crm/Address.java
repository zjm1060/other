package com.bitc.jk.xmis.model.crm;

import com.bitc.jk.xmis.model.TableBean;

/**
 * 地址
 * 
 * @author liying
 * 
 */
public class Address extends TableBean {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private String addressID;
	private Integer zipCode; // 邮编
	private String addressInfo; // 详细地址
//	private Integer cityID;

	private String district; // 区县

	// private District district;
	 private City city;
	// private Province province;
	// private Area area;
	// private Country country;
	// private Internation internation;

	public String getAddressID() {
		return addressID;
	}

	public void setAddressID(String addressID) {
		this.addressID = addressID;
	}

	public City getCity() {
		return city;
	}

	public void setCity(City city) {
		this.city = city;
	}

	public Integer getZipCode() {
		return zipCode;
	}

	public void setZipCode(Integer zipCode) {
		this.zipCode = zipCode;
	}

	public String getAddressInfo() {
		return addressInfo;
	}

	public void setAddressInfo(String addressInfo) {
		this.addressInfo = addressInfo;
	}

	public String getDistrict() {
		return district;
	}

	public void setDistrict(String district) {
		this.district = district;
	}

}
