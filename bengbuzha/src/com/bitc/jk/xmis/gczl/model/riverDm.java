package com.bitc.jk.xmis.gczl.model;

import java.math.BigDecimal;
import java.util.Date;

public class riverDm {
	private int tab_seed;
	private String unit_code;
	private Date measure_date;
	private String pillar_code;
	private BigDecimal distance;
	private BigDecimal high_degree;
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
	public String getPillar_code() {
		return pillar_code;
	}
	public void setPillar_code(String pillar_code) {
		this.pillar_code = pillar_code;
	}
	public BigDecimal getDistance() {
		return distance;
	}
	public void setDistance(BigDecimal distance) {
		this.distance = distance;
	}
	public BigDecimal getHigh_degree() {
		return high_degree;
	}
	public void setHigh_degree(BigDecimal high_degree) {
		this.high_degree = high_degree;
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
