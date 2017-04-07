package com.bitc.jk.xmis.service;

import java.util.List;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.bid.BidCompany;
import com.bitc.jk.xmis.model.bid.Bidding;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface BidService {
	public String doAddBidding(String name);

	public List<BidCompany> getBidCompanies();

	public List<Bidding> findBiddings(StandardQuery query);

	public List<Map<String, Object>> getBiddingDisplay(List<Bidding> biddings);

	public void doUpdateObject(String clazz, String id, String field, String value, String valueType);

	public void doUpdateBidPrice(String clazz, String biddingId, String companyId, String bidPriceField, String value);

	public void doDeleteBidding(String id);

	public void doUpdateAssociation(String clazz, String id, String field, String value, String valueType);

	public String doAddBidCompany(String name);

	public String getBidAnalysisList(PageInfo pageInfo, String bidID, String query, int i);

	public List getBidAnalysisDetail(String bidID);

	public void doSaveBidPlan(String bidID, String content);
}
