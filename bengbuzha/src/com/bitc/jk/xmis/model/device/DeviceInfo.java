package com.bitc.jk.xmis.model.device;


import java.math.BigDecimal;
import java.util.Date;

import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.TableBean;

import com.bitc.jk.xmis.model.sys.Business;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.MaterialsProperty;
import com.bitc.jk.xmis.model.sys.MaterialsUse;
import com.bitc.jk.xmis.model.sys.Power;
import com.bitc.jk.xmis.model.sys.ProduceStatus;
import com.bitc.jk.xmis.model.sys.Weight;
import com.bitc.jk.xmis.util.JSONUtils;
import com.sun.jmx.snmp.Timestamp;

/**
 * 
 * 设备，物资信息类
 * 
 * @author Bom Wu
 * 
 */
public class DeviceInfo extends TableBean {

	private static final long serialVersionUID = 588358338404132780L;

	/**
	 * ID
	 */
	private String id;
	/**
	 * 物资名称
	 */
	private String materiaName;
	/**
	 * 型号
	 */
	private String modelType;
	/**
	 * 规格
	 */
	private String standard;
	/**
	 * 重量
	 */
	private String weight;

	/**
	 * 功率
	 */
	private String power;

	/**
	 * 官方报价
	 */
	private BigDecimal referencePrice;

	/**
	 * 价格货币
	 */
	private Currency currency;
	/**
	 * 停产时间
	 * */
	private Date stopTime;
	/**
	 * 三包信息
	 */
	private String guarantees;
	/**
	 * 最后修改日期
	 * */
	private Date lastModified;
	/**
	 * 登记人ID
	 * */
	private SimpleEmployee newregisterName;
	/**
	 * 登记时间
	 * */
	private Date newupdateDate;
	

	/**
	 * 最后修改人ID
	 * */
	private SimpleEmployee lastModifier;
	/**
	 * 生产厂家ID
	 * */
	private CompanyObj manufacturer;
	/**
	 * 配置
	 * */
	private String configuration;
	public CompanyObj getManufacturer() {
		return manufacturer;
	}

	public void setManufacturer(CompanyObj manufacturer) {
		this.manufacturer = manufacturer;
	}

	/**
	 * 体积
	 * */
	private String volume;
	/**
	 * 物资业务属性
	 * */
	private Business materialsBusiness;
	/**
	 * 物资自身属性
	 * */
	private MaterialsProperty materialItself;
	/**
	 * 物资用途
	 * */
	private MaterialsUse materialUse;
	/**
	 * 物资生产状态
	 * */
	private String produceStatus;


	/**
	 * 最新采购价格
	 * */
	private BigDecimal newestPrice;
	/**
	 * 投入市场日期
	 * */
	private Date listingDate;
	
	private Power powerSuppliesUnit;
	
	private Weight materialWeight;
	
	private String factoryCommodityAddress;
	
	private String pigName;
	
	public String getPigName() {
		return pigName;
	}

	public void setPigName(String pigName) {
		this.pigName = pigName;
	}

	public String getFactoryCommodityAddress() {
		return factoryCommodityAddress;
	}

	public void setFactoryCommodityAddress(String factoryCommodityAddress) {
		this.factoryCommodityAddress = factoryCommodityAddress;
	}

	public Power getPowerSuppliesUnit() {
		return powerSuppliesUnit;
	}

	public void setPowerSuppliesUnit(Power powerSuppliesUnit) {
		this.powerSuppliesUnit = powerSuppliesUnit;
	}

	public Weight getMaterialWeight() {
		return materialWeight;
	}

	public void setMaterialWeight(Weight materialWeight) {
		this.materialWeight = materialWeight;
	}

	public Date getLastModified() {
		return lastModified;
	}

	public void setLastModified(Date lastModified) {
		this.lastModified = lastModified;
	}	
	


	public SimpleEmployee getLastModifier() {
		return lastModifier;
	}

	public void setLastModifier(SimpleEmployee lastModifier) {
		this.lastModifier = lastModifier;
	}
	public String getProduceStatus() {
		return produceStatus;
	}

	public void setProduceStatus(String produceStatus) {
		this.produceStatus = produceStatus;
	}
	



	public String getConfiguration() {
		return configuration;
	}

	public void setConfiguration(String configuration) {
		this.configuration = configuration;
	}

	public String getVolume() {
		return volume;
	}

	public void setVolume(String volume) {
		this.volume = volume;
	}

	
	
	public Business getMaterialsBusiness() {
		return materialsBusiness;
	}

	public void setMaterialsBusiness(Business materialsBusiness) {
		this.materialsBusiness = materialsBusiness;
	}

	public MaterialsProperty getMaterialItself() {
		return materialItself;
	}

	public void setMaterialItself(MaterialsProperty materialItself) {
		this.materialItself = materialItself;
	}

	public MaterialsUse getMaterialUse() {
		return materialUse;
	}

	public void setMaterialUse(MaterialsUse materialUse) {
		this.materialUse = materialUse;
	}


	

	public BigDecimal getNewestPrice() {
		return newestPrice;
	}

	public void setNewestPrice(BigDecimal newestPrice) {
		this.newestPrice = newestPrice;
	}

	public Date getListingDate() {
		return listingDate;
	}

	public void setListingDate(Date listingDate) {
		this.listingDate = listingDate;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}



	public String getMateriaName() {
		return materiaName;
	}

	public void setMateriaName(String materiaName) {
		this.materiaName = materiaName;
	}

	public String getModelType() {
		return modelType;
	}

	public void setModelType(String modelType) {
		this.modelType = modelType;
	}

	public String getStandard() {
		return standard;
	}

	public void setStandard(String standard) {
		this.standard = standard;
	}
	public Date getStopTime() {
		return stopTime;
	}

	public void setStopTime(Date stopTime) {
		this.stopTime = stopTime;
	}
	public String getWeight() {
		return weight;
	}

	public void setWeight(String weight) {
		this.weight = weight;
	}

	public String getPower() {
		return power;
	}

	public void setPower(String power) {
		this.power = power;
	}

	public BigDecimal getReferencePrice() {
		return referencePrice;
	}

	public void setReferencePrice(BigDecimal referencePrice) {
		this.referencePrice = referencePrice;
	}

	public String getGuarantees() {
		return guarantees;
	}

	public void setGuarantees(String guarantees) {
		this.guarantees = guarantees;
	}

	public Currency getCurrency() {
		return currency;
	}

	public void setCurrency(Currency currency) {
		this.currency = currency;
	}
	


	public SimpleEmployee getNewregisterName() {
		return newregisterName;
	}

	public void setNewregisterName(SimpleEmployee newregisterName) {
		this.newregisterName = newregisterName;
	}

	public Date getNewupdateDate() {
		return newupdateDate;
	}

	public void setNewupdateDate(Date newupdateDate) {
		this.newupdateDate = newupdateDate;
	}
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
}
