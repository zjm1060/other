package com.bitc.jk.xmis.applymanage.model;

import java.io.Serializable;
import java.math.BigDecimal;
import java.util.Date;

public class GczlInfo  implements Serializable{
	private int tab_seed;
	private String unit_code;
	private Date measure_date;
	private BigDecimal ap_w_level;
	private BigDecimal bp_w_level;
	private int gate_open_num_1;
	private int gate_open_num_2;
	private String gate_open_degree_1;
	private String gate_open_degree_2;
	private int elec_machine;
	private int flow_qty;
	private BigDecimal w_level_chan;
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
	public BigDecimal getW_level_chan() {
		return w_level_chan;
	}
	public void setW_level_chan(BigDecimal w_level_chan) {
		this.w_level_chan = w_level_chan;
	}
	public int getGate_open_num_1() {
		return gate_open_num_1;
	}
	public void setGate_open_num_1(int gate_open_num_1) {
		this.gate_open_num_1 = gate_open_num_1;
	}
	public int getGate_open_num_2() {
		return gate_open_num_2;
	}
	public void setGate_open_num_2(int gate_open_num_2) {
		this.gate_open_num_2 = gate_open_num_2;
	}
	public String getGate_open_degree_1() {
		return gate_open_degree_1;
	}
	public void setGate_open_degree_1(String gate_open_degree_1) {
		this.gate_open_degree_1 = gate_open_degree_1;
	}
	public String getGate_open_degree_2() {
		return gate_open_degree_2;
	}
	public void setGate_open_degree_2(String gate_open_degree_2) {
		this.gate_open_degree_2 = gate_open_degree_2;
	}
	public int getElec_machine() {
		return elec_machine;
	}
	public void setElec_machine(int elec_machine) {
		this.elec_machine = elec_machine;
	}
	public int getFlow_qty() {
		return flow_qty;
	}
	public void setFlow_qty(int flow_qty) {
		this.flow_qty = flow_qty;
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
