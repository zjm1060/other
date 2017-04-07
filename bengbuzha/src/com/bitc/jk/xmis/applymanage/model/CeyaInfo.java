package com.bitc.jk.xmis.applymanage.model;

import java.io.Serializable;
import java.math.BigDecimal;
import java.util.Date;

public class CeyaInfo  implements Serializable{
	private int tab_seed;
	private String unit_code;
	private Date measure_date;
	private String mptp_code;
	private String mpt_code;
	private BigDecimal tubo_w_level;
	private BigDecimal ap_w_level;
	private BigDecimal bp_w_level;
	private String flag;
	private String memo;
	private String mpt_name;
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
	public String getMptp_code() {
		return mptp_code;
	}
	public void setMptp_code(String mptp_code) {
		this.mptp_code = mptp_code;
	}
	public String getMpt_code() {
		return mpt_code;
	}
	public void setMpt_code(String mpt_code) {
		this.mpt_code = mpt_code;
	}
	public BigDecimal getTubo_w_level() {
		return tubo_w_level;
	}
	public void setTubo_w_level(BigDecimal tubo_w_level) {
		this.tubo_w_level = tubo_w_level;
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
	public String getMpt_name() {
		return mpt_name;
	}
	public void setMpt_name(String mpt_name) {
		this.mpt_name = mpt_name;
	}
	public int getMeasure_man() {
		return measure_man;
	}
	public void setMeasure_man(int measure_man) {
		this.measure_man = measure_man;
	}
	
	

}
