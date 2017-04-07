//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\PaymentDetailObj.java

package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.sql.Date;

public class PaymentDetailObj implements Serializable {
	private static final long serialVersionUID = 2323412983349350136L;
	private int contractID;
	private String paymentNo;
	private int paymentID;
	private int paymentType;
	private double paymentAmt;
	private Date paymentDate;
	private int transactorID;
	private String transactorName;
	private String remark;

	/**
	 * @roseuid 4C1B2D2601D4
	 */
	public PaymentDetailObj() {

	}

	/**
	 * Access method for the contractID property.
	 * 
	 * @return the current value of the contractID property
	 */
	public int getContractID() {
		return contractID;
	}

	/**
	 * Sets the value of the contractID property.
	 * 
	 * @param aContractID
	 *            the new value of the contractID property
	 */
	public void setContractID(int aContractID) {
		contractID = aContractID;
	}

	/**
	 * Access method for the paymentID property.
	 * 
	 * @return the current value of the paymentID property
	 */
	public int getPaymentID() {
		return paymentID;
	}

	/**
	 * Sets the value of the paymentID property.
	 * 
	 * @param aPaymentID
	 *            the new value of the paymentID property
	 */
	public void setPaymentID(int aPaymentID) {
		paymentID = aPaymentID;
	}

	/**
	 * Access method for the paymentType property.
	 * 
	 * @return the current value of the paymentType property
	 */
	public int getPaymentType() {
		return paymentType;
	}

	/**
	 * Sets the value of the paymentType property.
	 * 
	 * @param aPaymentType
	 *            the new value of the paymentType property
	 */
	public void setPaymentType(int aPaymentType) {
		paymentType = aPaymentType;
	}

	/**
	 * Access method for the paymentAmt property.
	 * 
	 * @return the current value of the paymentAmt property
	 */
	public double getPaymentAmt() {
		return paymentAmt;
	}

	/**
	 * Sets the value of the paymentAmt property.
	 * 
	 * @param aPaymentAmt
	 *            the new value of the paymentAmt property
	 */
	public void setPaymentAmt(double aPaymentAmt) {
		paymentAmt = aPaymentAmt;
	}

	/**
	 * Access method for the paymentDate property.
	 * 
	 * @return the current value of the paymentDate property
	 */
	public Date getPaymentDate() {
		return paymentDate;
	}

	/**
	 * Sets the value of the paymentDate property.
	 * 
	 * @param aPaymentDate
	 *            the new value of the paymentDate property
	 */
	public void setPaymentDate(Date aPaymentDate) {
		paymentDate = aPaymentDate;
	}

	/**
	 * Access method for the transactorID property.
	 * 
	 * @return the current value of the transactorID property
	 */
	public int getTransactorID() {
		return transactorID;
	}

	/**
	 * Sets the value of the transactorID property.
	 * 
	 * @param aTransactorID
	 *            the new value of the transactorID property
	 */
	public void setTransactorID(int aTransactorID) {
		transactorID = aTransactorID;
	}

	/**
	 * Access method for the transactorName property.
	 * 
	 * @return the current value of the transactorName property
	 */
	public String getTransactorName() {
		return transactorName;
	}

	/**
	 * Sets the value of the transactorName property.
	 * 
	 * @param aTransactorName
	 *            the new value of the transactorName property
	 */
	public void setTransactorName(String aTransactorName) {
		transactorName = aTransactorName;
	}

	/**
	 * @return the remark
	 */
	public String getRemark() {
		return remark;
	}

	/**
	 * @param remark
	 *            the remark to set
	 */
	public void setRemark(String remark) {
		this.remark = remark;
	}

	public String getPaymentNo() {
		return paymentNo;
	}

	public void setPaymentNo(String paymentNo) {
		this.paymentNo = paymentNo;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + contractID;
		result = prime * result + paymentID;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		PaymentDetailObj other = (PaymentDetailObj) obj;
		if (contractID != other.contractID)
			return false;
		if (paymentID != other.paymentID)
			return false;
		return true;
	}
}
