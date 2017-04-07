package com.bitc.jk.xmis.applymanage.model;

import java.io.Serializable;
import java.math.BigDecimal;
import java.util.Date;

public class BusinessEntertainment implements Serializable{

	/**
	 * 
	 */
	private static final long serialVersionUID = -7428038131323468235L;
	private String form_id;
	private String visitor_company_name;
	private int visitor_qty;
	private Date meals_date;
	private int meals_type;
	private BigDecimal meals_fee_budget;
	private int accompany_qty;
	private BigDecimal cost;
	private BigDecimal profit;
	private BigDecimal cigarettes_wine_amt;
	private BigDecimal total_amt;
	private Date app_datetime;
	private int app_user_id;
	private int app_deptment;
	private String sign_No;
	private String remark;
	private int is_worked;//0 有效的； 1 无效的（强制终止的）
	public String getForm_id() {
		return form_id;
	}
	public void setForm_id(String form_id) {
		this.form_id = form_id;
	}
	public String getVisitor_company_name() {
		return visitor_company_name;
	}
	public void setVisitor_company_name(String visitor_company_name) {
		this.visitor_company_name = visitor_company_name;
	}
	public int getVisitor_qty() {
		return visitor_qty;
	}
	public void setVisitor_qty(int visitor_qty) {
		this.visitor_qty = visitor_qty;
	}
	public Date getMeals_date() {
		return meals_date;
	}
	public void setMeals_date(Date meals_date) {
		this.meals_date = meals_date;
	}
	public int getMeals_type() {
		return meals_type;
	}
	public void setMeals_type(int meals_type) {
		this.meals_type = meals_type;
	}
	public BigDecimal getMeals_fee_budget() {
		return meals_fee_budget;
	}
	public void setMeals_fee_budget(BigDecimal meals_fee_budget) {
		this.meals_fee_budget = meals_fee_budget;
	}
	public int getAccompany_qty() {
		return accompany_qty;
	}
	public void setAccompany_qty(int accompany_qty) {
		this.accompany_qty = accompany_qty;
	}
	public BigDecimal getCost() {
		return cost;
	}
	public void setCost(BigDecimal cost) {
		this.cost = cost;
	}
	public BigDecimal getProfit() {
		return profit;
	}
	public void setProfit(BigDecimal profit) {
		this.profit = profit;
	}
	public BigDecimal getCigarettes_wine_amt() {
		return cigarettes_wine_amt;
	}
	public void setCigarettes_wine_amt(BigDecimal cigarettes_wine_amt) {
		this.cigarettes_wine_amt = cigarettes_wine_amt;
	}
	public BigDecimal getTotal_amt() {
		return total_amt;
	}
	public void setTotal_amt(BigDecimal total_amt) {
		this.total_amt = total_amt;
	}
	public Date getApp_datetime() {
		return app_datetime;
	}
	public void setApp_datetime(Date app_datetime) {
		this.app_datetime = app_datetime;
	}
	public int getApp_user_id() {
		return app_user_id;
	}
	public void setApp_user_id(int app_user_id) {
		this.app_user_id = app_user_id;
	}
	public int getApp_deptment() {
		return app_deptment;
	}
	public void setApp_deptment(int app_deptment) {
		this.app_deptment = app_deptment;
	}
	public String getSign_No() {
		return sign_No;
	}
	public void setSign_No(String sign_No) {
		this.sign_No = sign_No;
	}
	public String getRemark() {
		return remark;
	}
	public void setRemark(String remark) {
		this.remark = remark;
	}
	public int getIs_worked() {
		return is_worked;
	}
	public void setIs_worked(int is_worked) {
		this.is_worked = is_worked;
	}

	
}
