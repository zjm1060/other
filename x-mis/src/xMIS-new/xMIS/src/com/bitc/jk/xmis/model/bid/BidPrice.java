package com.bitc.jk.xmis.model.bid;

import java.math.BigDecimal;

/**
 * 投标价格
 * 
 * @author BomWu
 *
 */
public class BidPrice {
	private String id;
	private BidCompany company;// 报价公司
	private BigDecimal bidPrice;// 报价
	private BigDecimal preferentialPrice;// 优惠价

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public BidCompany getCompany() {
		return company;
	}

	public void setCompany(BidCompany company) {
		this.company = company;
	}

	public BigDecimal getBidPrice() {
		return bidPrice;
	}

	public void setBidPrice(BigDecimal bidPrice) {
		this.bidPrice = bidPrice;
	}

	public BigDecimal getPreferentialPrice() {
		return preferentialPrice;
	}

	public void setPreferentialPrice(BigDecimal preferentialPrice) {
		this.preferentialPrice = preferentialPrice;
	}

	public BigDecimal getValidPrice() {
		return this.preferentialPrice == null ? this.bidPrice : this.preferentialPrice;
	}
}
