package com.bitc.jk.xmis.model.bid;

import java.math.BigDecimal;
import java.util.Date;
import java.util.List;

import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.SimpleEmployee;

/**
 * 投标
 * 
 * @author BomWu
 *
 */
public class Bidding {
	private String id;
	private String name;
	private CompanySimpleObj bidCompany;// 业主单位
	private BigDecimal contractPrice;// 合同价,中票价
	private SimpleEmployee solutionWriter;// 方案编写
	private SimpleEmployee businessManager;// 商务经理
	private String region;// 区域
	private String solution;// 方案
	private BidCompany winCompany;// 中标单位

	private Date bidDate;// 投票日期
	private List<BidPrice> bidPrices;// 出价

	public List<BidPrice> getBidPrices() {
		return bidPrices;
	}

	public void setBidPrices(List<BidPrice> bidPrices) {
		this.bidPrices = bidPrices;
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public CompanySimpleObj getBidCompany() {
		return bidCompany;
	}

	public void setBidCompany(CompanySimpleObj bidCompany) {
		this.bidCompany = bidCompany;
	}

	public BigDecimal getContractPrice() {
		return contractPrice;
	}

	public void setContractPrice(BigDecimal contractPrice) {
		this.contractPrice = contractPrice;
	}

	public SimpleEmployee getSolutionWriter() {
		return solutionWriter;
	}

	public void setSolutionWriter(SimpleEmployee solutionWriter) {
		this.solutionWriter = solutionWriter;
	}

	public SimpleEmployee getBusinessManager() {
		return businessManager;
	}

	public void setBusinessManager(SimpleEmployee businessManager) {
		this.businessManager = businessManager;
	}

	public BidCompany getWinCompany() {
		return winCompany;
	}

	public void setWinCompany(BidCompany winCompany) {
		this.winCompany = winCompany;
	}

	public String getRegion() {
		return region;
	}

	public void setRegion(String region) {
		this.region = region;
	}

	public String getSolution() {
		return solution;
	}

	public void setSolution(String solution) {
		this.solution = solution;
	}

	public Date getBidDate() {
		return bidDate;
	}

	public void setBidDate(Date bidDate) {
		this.bidDate = bidDate;
	}

	public BidPrice findBidPrice(String companyId) {
		for (BidPrice bidPrice : bidPrices) {
			if (bidPrice.getCompany().getId().equals(companyId)) {
				return bidPrice;
			}
		}
		return null;
	}

	public BigDecimal[] getAnalysisPrices() {
		BigDecimal min = null;
		BigDecimal max = null;
		BigDecimal total = null;
		BigDecimal avg = null;
		int j = 0;
		boolean inited = false;
		for (int i = 0; i < getBidPrices().size(); i++) {
			BidPrice bid = getBidPrices().get(i);
			if (bid.getValidPrice() == null) {
				continue;
			}
			j++;
			if (!inited) {
				min = bid.getValidPrice();
				max = bid.getValidPrice();
				total = bid.getValidPrice();
				inited=true;
				continue;
			}
			min = bid.getValidPrice().min(min);
			max = bid.getValidPrice().max(max);
			total = total.add(bid.getValidPrice());
		}
		if (j > 0) {
			avg = total.divide(new BigDecimal(j), 0, BigDecimal.ROUND_UP);
		}
		return new BigDecimal[] { min, max, avg };
	}
}
