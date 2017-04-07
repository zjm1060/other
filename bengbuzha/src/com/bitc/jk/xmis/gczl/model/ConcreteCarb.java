package com.bitc.jk.xmis.gczl.model;

import java.math.BigDecimal;
import java.util.Date;

public class ConcreteCarb {
	private int tab_seed;
	private String unit_code;
	private Date measure_date;
	private String place_code;
	private BigDecimal carbonize_depth;
	private BigDecimal allow_carb_depth;
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
	public String getPlace_code() {
		return place_code;
	}
	public void setPlace_code(String place_code) {
		this.place_code = place_code;
	}
	public BigDecimal getCarbonize_depth() {
		return carbonize_depth;
	}
	public void setCarbonize_depth(BigDecimal carbonize_depth) {
		this.carbonize_depth = carbonize_depth;
	}
	public BigDecimal getAllow_carb_depth() {
		return allow_carb_depth;
	}
	public void setAllow_carb_depth(BigDecimal allow_carb_depth) {
		this.allow_carb_depth = allow_carb_depth;
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
