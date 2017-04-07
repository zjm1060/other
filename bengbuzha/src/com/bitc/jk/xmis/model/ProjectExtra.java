package com.bitc.jk.xmis.model;

import java.io.Serializable;

public class ProjectExtra implements Serializable {
	private static final long serialVersionUID = 5375833037949890200L;
	private double paymentSum;
	private double receivedSum;
	private double debtSum;
	private double invoiceSum;
	private double invoiceNotReachSum;
	private double reimbursementSum;
	private String currencySymbol;

	public ProjectExtra() {
		super();
	}

	public ProjectExtra(double paymentSum, double receivedSum, double debtSum,
			double invoiceSum, double invoiceNotReachSum,
			double reimbursementSum, String currencySymbol) {
		this();
		this.paymentSum = paymentSum;
		this.receivedSum = receivedSum;
		this.debtSum = debtSum;
		this.invoiceSum = invoiceSum;
		this.invoiceNotReachSum = invoiceNotReachSum;
		this.reimbursementSum = reimbursementSum;
		this.currencySymbol = currencySymbol;
	}

	public double getPaymentSum() {
		return paymentSum;
	}

	public void setPaymentSum(double paymentSum) {
		this.paymentSum = paymentSum;
	}

	public double getReceivedSum() {
		return receivedSum;
	}

	public void setReceivedSum(double receivedSum) {
		this.receivedSum = receivedSum;
	}

	public double getInvoiceSum() {
		return invoiceSum;
	}

	public void setInvoiceSum(double invoiceSum) {
		this.invoiceSum = invoiceSum;
	}

	public double getInvoiceNotReachSum() {
		return invoiceNotReachSum;
	}

	public void setInvoiceNotReachSum(double invoiceNotReachSum) {
		this.invoiceNotReachSum = invoiceNotReachSum;
	}

	public double getReimbursementSum() {
		return reimbursementSum;
	}

	public void setReimbursementSum(double reimbursementSum) {
		this.reimbursementSum = reimbursementSum;
	}

	public double getDebtSum() {
		return debtSum;
	}

	public void setDebtSum(double debtSum) {
		this.debtSum = debtSum;
	}

	public String getCurrencySymbol() {
		return currencySymbol;
	}

	public void setCurrencySymbol(String currencySymbol) {
		this.currencySymbol = currencySymbol;
	}

}
