package com.bitc.jk.xmis.gczl.model;

import java.math.BigDecimal;
import java.util.Date;

public class weather {
	private int tab_seed;
	private String unit_code;
	private Date measure_date;
	private String sky_status;
	private BigDecimal weather;
	private String memo;
	private int measure_man;
	public int getTab_seed() {
		return tab_seed;
	}
	public void setTab_seed(int tab_seed) {
		this.tab_seed = tab_seed;
	}
	public String getUnit_code() {
		return unit_code;
	}
	public void setUnit_code(String unit_code) {
		this.unit_code = unit_code;
	}
	public Date getMeasure_date() {
		return measure_date;
	}
	public void setMeasure_date(Date measure_date) {
		this.measure_date = measure_date;
	}
	public String getSky_status() {
		return sky_status;
	}
	public void setSky_status(String sky_status) {
		this.sky_status = sky_status;
	}
	public BigDecimal getWeather() {
		return weather;
	}
	public void setWeather(BigDecimal weather) {
		this.weather = weather;
	}
	public String getMemo() {
		return memo;
	}
	public void setMemo(String memo) {
		this.memo = memo;
	}
	public int getMeasure_man() {
		return measure_man;
	}
	public void setMeasure_man(int measure_man) {
		this.measure_man = measure_man;
	}
	
}
