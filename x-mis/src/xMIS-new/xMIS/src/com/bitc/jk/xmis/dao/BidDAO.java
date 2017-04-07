package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.bid.BidCompany;
import com.bitc.jk.xmis.model.bid.Bidding;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface BidDAO {

	void saveBidding(Bidding bidding);

	List<BidCompany> getBidCompanies();

	List<Bidding> findBiddings(StandardQuery query);

	Bidding getBidding(String id);

	int getBidCompanyMaxOrder();

	void saveBidCompany(BidCompany company);

	List getBidAnalysisList(String bidID, String query, int userId);

	List getBidAnalysisDetail(String bidID);

	void doSaveBidPlan(String bidID, String content);

}
