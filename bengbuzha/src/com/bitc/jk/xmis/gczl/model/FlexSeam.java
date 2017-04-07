package com.bitc.jk.xmis.gczl.model;

import java.math.BigDecimal;
import java.util.Date;

public class FlexSeam {

	private int tab_seed;
	private String unit_code;
	private Date measure_date;
	private String fs_code;
	private BigDecimal seam_width;
	private BigDecimal weather;
	private String flag;
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
	public String getFs_code() {
		return fs_code;
	}
	public void setFs_code(String fs_code) {
		this.fs_code = fs_code;
	}
	public BigDecimal getSeam_width() {
		return seam_width;
	}
	public void setSeam_width(BigDecimal seam_width) {
		this.seam_width = seam_width;
	}
	public BigDecimal getWeather() {
		return weather;
	}
	public void setWeather(BigDecimal weather) {
		this.weather = weather;
	}
	public String getFlag() {
		return flag;
	}
	public void setFlag(String flag) {
		this.flag = flag;
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
