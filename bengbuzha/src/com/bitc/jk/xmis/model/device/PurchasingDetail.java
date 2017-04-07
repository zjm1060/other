package com.bitc.jk.xmis.model.device;

import java.io.Serializable;
import java.util.Date;

import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.SimpleDeptObj;
import com.bitc.jk.xmis.model.SimpleEmployee;

/**
 * 采购清单明细
 * 
 * @author Bom Wu
 * 
 */
public class PurchasingDetail implements Serializable {

	private static final long serialVersionUID = 998853260514376070L;
	private String id;
	private PurchasingList purchasing;
	/**
	 * 名称
	 */
	private String name;
	/**
	 * 数量
	 */
	private int number;
	/**
	 * 型号
	 */
//	private String modelType;
	/**
	 * 规格
	 */
//	private String standard;
	/**
	 * 重量
	 */
//	private String weight;
	/**
	 * 项目设备ID
	 */
//	private String projectDeviceId;
	private DeviceInfo deviceInfo;

	/**
	 * 技术要求
	 */
	private String technicalRequirement;

	/**
	 * 期望到货日期
	 */
	private Date expectArrivalDate;

	/**
	 * 备注
	 */
	private String remark;

	/**
	 * 支出合同ID
	 */
	private ContractObj contractID;



	//	/**
//	 * 项目编码
//	 */
//	private String projectCode;
//
//	/**
//	 * 项目名称
//	 */
//	private String projectName;
	/**
	 * 采购申请ID
	 */
	private String requisitionId;

	/**
	 * 采购申请明细ID
	 */
	private String requisitionDetailId;

	/**
	 * 申请部门
	 */
	private SimpleDeptObj dept;

	/**
	 * 申请人
	 */
	private SimpleEmployee applicant;
	/**
	 * 单价
	 * */
	private float univalent;
	/***
	 * 登记人ID
	 * */
	private SimpleEmployee subscriber;
	/***
	 * 登记日期
	 * */
	private Date registrationDate;
	/**
	 * 最后修改人ID
	 * */
	private SimpleEmployee lastModifier;
	/**
	 * 最后修改日期
	 * */
	private Date lastModified;
	
	
	
	
	
	
	
	
	
	public SimpleEmployee getSubscriber() {
		return subscriber;
	}

	public void setSubscriber(SimpleEmployee subscriber) {
		this.subscriber = subscriber;
	}

	public Date getRegistrationDate() {
		return registrationDate;
	}

	public void setRegistrationDate(Date registrationDate) {
		this.registrationDate = registrationDate;
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

	public float getUnivalent() {
		return univalent;
	}

	public void setUnivalent(float univalent) {
		this.univalent = univalent;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = StringUtils.isBlank(id) ? null : id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getNumber() {
		return number;
	}

	public void setNumber(int number) {
		this.number = number;
	}

//	public String getModelType() {
//		return modelType;
//	}
//
//	public void setModelType(String modelType) {
//		this.modelType = modelType;
//	}
//
//	public String getStandard() {
//		return standard;
//	}
//
//	public void setStandard(String standard) {
//		this.standard = standard;
//	}
//
//	public String getWeight() {
//		return weight;
//	}
//
//	public void setWeight(String weight) {
//		this.weight = weight;
//	}

	public ContractObj getContractID() {
		return contractID;
	}

	public void setContractID(ContractObj contractID) {
		this.contractID = contractID;
	}


	public DeviceInfo getDeviceInfo() {
		return deviceInfo;
	}

	public void setDeviceInfo(DeviceInfo deviceInfo) {
		this.deviceInfo = deviceInfo;
	}

	public String getTechnicalRequirement() {
		return technicalRequirement;
	}

	public void setTechnicalRequirement(String technicalRequirement) {
		this.technicalRequirement = technicalRequirement;
	}

	public Date getExpectArrivalDate() {
		return expectArrivalDate;
	}

	public void setExpectArrivalDate(Date expectArrivalDate) {
		this.expectArrivalDate = expectArrivalDate;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}


//	public String getProjectCode() {
//		return projectCode;
//	}
//
//	public void setProjectCode(String projectCode) {
//		this.projectCode = projectCode;
//	}
//
//	public String getProjectName() {
//		return projectName;
//	}
//
//	public void setProjectName(String projectName) {
//		this.projectName = projectName;
//	}

	public String getRequisitionId() {
		return requisitionId;
	}

	public void setRequisitionId(String requisitionId) {
		this.requisitionId = requisitionId;
	}

	public String getRequisitionDetailId() {
		return requisitionDetailId;
	}

	public void setRequisitionDetailId(String requisitionDetailId) {
		this.requisitionDetailId = requisitionDetailId;
	}

	public SimpleDeptObj getDept() {
		return dept;
	}

	public void setDept(SimpleDeptObj dept) {
		this.dept = dept;
	}

	public SimpleEmployee getApplicant() {
		return applicant;
	}

	public void setApplicant(SimpleEmployee applicant) {
		this.applicant = applicant;
	}

	public PurchasingList getPurchasing() {
		return purchasing;
	}

	public void setPurchasing(PurchasingList purchasing) {
		this.purchasing = purchasing;
	}
}
