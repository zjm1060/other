package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;

public class Currency implements Serializable {

	private static final long serialVersionUID = 5017749755138422037L;

	private int id;
	private String name;
	private double rateOfExchange2RMB;
	private String symbol;

	public Currency() {
		super();
	}

	public Currency(int id) {
		this.id = id;
	}

	public static Currency buildDefaultCurrency() {
		return new Currency(1, "人民币", 1, "￥");
	}

	public Currency(int id, String name, double rateOfExchange2RMB,
			String symbol) {
		this();
		this.id = id;
		this.name = name;
		this.rateOfExchange2RMB = rateOfExchange2RMB;
		this.symbol = symbol;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public double getRateOfExchange2RMB() {
		return rateOfExchange2RMB;
	}

	public void setRateOfExchange2RMB(double rateOfExchange2RMB) {
		this.rateOfExchange2RMB = rateOfExchange2RMB;
	}

	public String getSymbol() {
		return symbol;
	}

	public void setSymbol(String symbol) {
		this.symbol = symbol;
	}

}
