package com.bitc.jk.xmis.gczl.model;

import java.math.BigDecimal;
import java.util.Date;

public class ConcSeam {
	private int tab_seed;
	private String unit_code;
	private Date measure_date;
	private String conc_seam_code;
	private String conc_mp_code;
	private BigDecimal weather;
	private BigDecimal seam_long;
	private BigDecimal seam_width;
	private String xzms;
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
	public String getConc_seam_code() {
		return conc_seam_code;
	}
	public void setConc_seam_code(String conc_seam_code) {
		this.conc_seam_code = conc_seam_code;
	}
	public String getConc_mp_code() {
		return conc_mp_code;
	}
	public void setConc_mp_code(String conc_mp_code) {
		this.conc_mp_code = conc_mp_code;
	}
	public BigDecimal getWeather() {
		return weather;
	}
	public void setWeather(BigDecimal weather) {
		this.weather = weather;
	}
	public BigDecimal getSeam_long() {
		return seam_long;
	}
	public void setSeam_long(BigDecimal seam_long) {
		this.seam_long = seam_long;
	}
	public BigDecimal getSeam_width() {
		return seam_width;
	}
	public void setSeam_width(BigDecimal seam_width) {
		this.seam_width = seam_width;
	}
	public String getXzms() {
		return xzms;
	}
	public void setXzms(String xzms) {
		this.xzms = xzms;
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
