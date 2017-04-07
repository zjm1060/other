//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\DeviceObj.java

package com.bitc.jk.xmis.model;

import java.util.Iterator;
import java.util.Set;

import com.bitc.jk.xmis.model.sys.Currency;

/**
 * 项目设备
 */
public class DeviceObj extends TableBean {
	private static final long serialVersionUID = 445700764960315895L;
	private String id;
	private String name;
	private String projectID;
	private String projectName;
	private int number;
	private String modelType;
	private String standard;
	private String weight;
	private int unitID;
	private String unitName;
	private double referencePrice;
	private Currency currency;
	/**
	 * 项目设备采购状态数据
	 */
	private Set<ProjectDevicePurchaseStatus> purchaseStatus;

	/**
	 * @roseuid 4C1F04220280
	 */
	public DeviceObj() {

	}

	/**
	 * Access method for the projectID property.
	 * 
	 * @return the current value of the projectID property
	 */
	public String getProjectID() {
		return projectID;
	}

	/**
	 * Sets the value of the projectID property.
	 * 
	 * @param aProjectID
	 *            the new value of the projectID property
	 */
	public void setProjectID(String aProjectID) {
		projectID = aProjectID;
	}

	/**
	 * Access method for the projectName property.
	 * 
	 * @return the current value of the projectName property
	 */
	public String getProjectName() {
		return projectName;
	}

	/**
	 * Sets the value of the projectName property.
	 * 
	 * @param aProjectName
	 *            the new value of the projectName property
	 */
	public void setProjectName(String aProjectName) {
		projectName = aProjectName;
	}

	/**
	 * Access method for the modelType property.
	 * 
	 * @return the current value of the modelType property
	 */
	public String getModelType() {
		return modelType;
	}

	/**
	 * Sets the value of the modelType property.
	 * 
	 * @param aModelType
	 *            the new value of the modelType property
	 */
	public void setModelType(String aModelType) {
		modelType = aModelType;
	}

	/**
	 * Access method for the unitID property.
	 * 
	 * @return the current value of the unitID property
	 */
	public int getUnitID() {
		return unitID;
	}

	/**
	 * Sets the value of the unitID property.
	 * 
	 * @param aUnitID
	 *            the new value of the unitID property
	 */
	public void setUnitID(int aUnitID) {
		unitID = aUnitID;
	}

	/**
	 * Access method for the unitName property.
	 * 
	 * @return the current value of the unitName property
	 */
	public String getUnitName() {
		return unitName;
	}

	/**
	 * Sets the value of the unitName property.
	 * 
	 * @param aUnitName
	 *            the new value of the unitName property
	 */
	public void setUnitName(String aUnitName) {
		unitName = aUnitName;
	}

	/**
	 * Access method for the referencePrice property.
	 * 
	 * @return the current value of the referencePrice property
	 */
	public double getReferencePrice() {
		return referencePrice;
	}

	/**
	 * Sets the value of the referencePrice property.
	 * 
	 * @param aReferencePrice
	 *            the new value of the referencePrice property
	 */
	public void setReferencePrice(double aReferencePrice) {
		referencePrice = aReferencePrice;
	}

	public int getNumber() {
		return number;
	}

	public void setNumber(int number) {
		this.number = number;
	}

	public String getStandard() {
		return standard;
	}

	public void setStandard(String standard) {
		this.standard = standard;
	}

	public Currency getCurrency() {
		return currency;
	}

	public void setCurrency(Currency currency) {
		this.currency = currency;
	}

	public String getWeight() {
		return weight;
	}

	public void setWeight(String weight) {
		this.weight = weight;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public Set<ProjectDevicePurchaseStatus> getPurchaseStatus() {
		return purchaseStatus;
	}

	public void setPurchaseStatus(
			Set<ProjectDevicePurchaseStatus> purchaseStatus) {
		this.purchaseStatus = purchaseStatus;
	}

	/**
	 * @return 返回一个一维整型数组，四个元素分别为已申请采购数量、采购中数量、已采购数量、三个数量之和
	 */
	public int[] getStatusCount() {
		if (this.getPurchaseStatus() == null) {
			return new int[] { 0, 0, 0, 0 };
		}
		int rSum = 0;// 已申请
		int pSum = 0;// 采购中
		int oSum = 0;// 已采购
		for (Iterator<ProjectDevicePurchaseStatus> it = this
				.getPurchaseStatus().iterator(); it.hasNext();) {
			ProjectDevicePurchaseStatus s = it.next();
			if (ProjectDevicePurchaseStatus.STATUS_REQUEST
					.equals(s.getStatus())) {
				rSum += s.getNumber();
			} else if (ProjectDevicePurchaseStatus.STATUS_PURCHASING.equals(s
					.getStatus())) {
				pSum += s.getNumber();
			} else if (ProjectDevicePurchaseStatus.STATUS_PURCHASE.equals(s
					.getStatus())) {
				oSum += s.getNumber();
			}
		}
		return new int[] { rSum, pSum, oSum, rSum + pSum + oSum };
	}
}
