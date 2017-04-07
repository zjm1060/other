package com.bitc.jk.xmis.gczl.model;

import java.math.BigDecimal;
import java.util.Date;

public class Sink {
	private int tab_seed;
	private String unit_code;
	private Date measure_date;
	private String sp_code;
	private String s_point_code;
	private BigDecimal high_degree;
	private BigDecimal ap_w_level;
	private BigDecimal bp_w_level;
	private String memo;
	private String s_point_name;
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
	public String getSp_code() {
		return sp_code;
	}
	public void setSp_code(String sp_code) {
		this.sp_code = sp_code;
	}
	public String getS_point_code() {
		return s_point_code;
	}
	public void setS_point_code(String s_point_code) {
		this.s_point_code = s_point_code;
	}
	public BigDecimal getHigh_degree() {
		return high_degree;
	}
	public void setHigh_degree(BigDecimal high_degree) {
		this.high_degree = high_degree;
	}
	public BigDecimal getAp_w_level() {
		return ap_w_level;
	}
	public void setAp_w_level(BigDecimal ap_w_level) {
		this.ap_w_level = ap_w_level;
	}
	public BigDecimal getBp_w_level() {
		return bp_w_level;
	}
	public void setBp_w_level(BigDecimal bp_w_level) {
		this.bp_w_level = bp_w_level;
	}
	public String getMemo() {
		return memo;
	}
	public void setMemo(String memo) {
		this.memo = memo;
	}
	public String getS_point_name() {
		return s_point_name;
	}
	public void setS_point_name(String s_point_name) {
		this.s_point_name = s_point_name;
	}
	public int getMeasure_man() {
		return measure_man;
	}
	public void setMeasure_man(int measure_man) {
		this.measure_man = measure_man;
	}
	
}
