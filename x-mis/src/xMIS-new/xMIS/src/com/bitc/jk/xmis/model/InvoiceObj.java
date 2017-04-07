//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\InvoiceObj.java

package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.sql.Date;

public class InvoiceObj implements Serializable {
	private static final long serialVersionUID = 3274653399523676155L;
	private int invoiceID;
	private String invoiceCode;
	private int contractID;
	private double invoiceAMT;
	private Date invoiceDate;
	private int transactorID;
	private String transactorName;
	private String invoiecRemark;
	private int receiverId;
	private String mailCode;

	private int contractType;
	private int lastModificatorID;
	private Date lastModificationDate;

	/**
	 * @roseuid 4C1B2D260261
	 */
	public InvoiceObj() {

	}

	/**
	 * Access method for the invoiceID property.
	 * 
	 * @return the current value of the invoiceID property
	 */
	public int getInvoiceID() {
		return invoiceID;
	}

	/**
	 * Sets the value of the invoiceID property.
	 * 
	 * @param aInvoiceID
	 *            the new value of the invoiceID property
	 */
	public void setInvoiceID(int aInvoiceID) {
		invoiceID = aInvoiceID;
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
	 * Access method for the invoiceAMT property.
	 * 
	 * @return the current value of the invoiceAMT property
	 */
	public double getInvoiceAMT() {
		return invoiceAMT;
	}

	/**
	 * Sets the value of the invoiceAMT property.
	 * 
	 * @param aInvoiceAMT
	 *            the new value of the invoiceAMT property
	 */
	public void setInvoiceAMT(double aInvoiceAMT) {
		invoiceAMT = aInvoiceAMT;
	}

	/**
	 * Access method for the invoiceDate property.
	 * 
	 * @return the current value of the invoiceDate property
	 */
	public Date getInvoiceDate() {
		return invoiceDate;
	}

	/**
	 * Sets the value of the invoiceDate property.
	 * 
	 * @param aInvoiceDate
	 *            the new value of the invoiceDate property
	 */
	public void setInvoiceDate(Date aInvoiceDate) {
		invoiceDate = aInvoiceDate;
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
	 * @return the contractType
	 */
	public int getContractType() {
		return contractType;
	}

	/**
	 * @param contractType
	 *            the contractType to set
	 */
	public void setContractType(int contractType) {
		this.contractType = contractType;
	}

	/**
	 * @return the invoiecRemark
	 */
	public String getInvoiecRemark() {
		return invoiecRemark;
	}

	/**
	 * @param invoiecRemark
	 *            the invoiecRemark to set
	 */
	public void setInvoiecRemark(String invoiecRemark) {
		this.invoiecRemark = invoiecRemark;
	}

	/**
	 * @return the invoiceNo
	 */
	public String getInvoiceCode() {
		return invoiceCode;
	}

	/**
	 * @param invoiceNo
	 *            the invoiceNo to set
	 */
	public void setInvoiceCode(String invoiceCode) {
		this.invoiceCode = invoiceCode;
	}

	public int getReceiverId() {
		return receiverId;
	}

	public void setReceiverId(int receiverId) {
		this.receiverId = receiverId;
	}

	public String getMailCode() {
		return mailCode;
	}

	public void setMailCode(String mailCode) {
		this.mailCode = mailCode;
	}
	
	public int getLastModificatorID() {
		return lastModificatorID;
	}

	public void setLastModificatorID(int lastModificatorID) {
		this.lastModificatorID = lastModificatorID;
	}
	
	public Date getLastModificationDate() {
		return lastModificationDate;
	}

	public void setLastModificationDate(Date lastModificationDate) {
		this.lastModificationDate = lastModificationDate;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + contractID;
		result = prime * result + invoiceID;
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
		InvoiceObj other = (InvoiceObj) obj;
		if (contractID != other.contractID)
			return false;
		if (invoiceID != other.invoiceID)
			return false;
		return true;
	}

}
