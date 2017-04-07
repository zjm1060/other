package com.bitc.jk.xmis.service.impl;

import java.io.Serializable;
import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.axis.utils.StringUtils;

import com.bitc.jk.xmis.dao.BidDAO;
import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.dao.impl.BaseDAOImpl;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.bid.BidCompany;
import com.bitc.jk.xmis.model.bid.BidPrice;
import com.bitc.jk.xmis.model.bid.Bidding;
import com.bitc.jk.xmis.service.BidService;
import com.bitc.jk.xmis.util.BeanUtils;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class BidServiceImpl implements BidService {

	private BidDAO bidDAO;
	private OrganizationDAO organizationDAO;

	public BidDAO getBidDAO() {
		return bidDAO;
	}

	public void setBidDAO(BidDAO bidDAO) {
		this.bidDAO = bidDAO;
	}

	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}

	public void setOrganizationDAO(OrganizationDAO organizationDAO) {
		this.organizationDAO = organizationDAO;
	}

	public String doAddBidding(String name) {
		Bidding bidding = new Bidding();
		bidding.setName(name);
		bidDAO.saveBidding(bidding);
		return bidding.getId();
	}

	public String doAddBidCompany(String name) {
		BidCompany company = new BidCompany();
		company.setName(name);
		company.setOrder(bidDAO.getBidCompanyMaxOrder() + 1);
		bidDAO.saveBidCompany(company);
		return company.getId();
	}

	public List<BidCompany> getBidCompanies() {
		return bidDAO.getBidCompanies();
	}

	public List<Bidding> findBiddings(StandardQuery query) {
		return bidDAO.findBiddings(query);
	}

	public List<Map<String, Object>> getBiddingDisplay(List<Bidding> biddings) {
		List<BidCompany> companies = getBidCompanies();
		List<Map<String, Object>> result = new ArrayList<Map<String, Object>>();
		for (Bidding bidding : biddings) {
			if (bidding.getBidCompany() != null) {
				bidding.getBidCompany().setDepts(null);
			}
			Map<String, Object> map = BeanUtils.bean2Map(bidding);
			map.remove("bidPrices");
			for (BidCompany bidCompany : companies) {
				BidPrice price = bidding.findBidPrice(bidCompany.getId());
				map.put("company_" + bidCompany.getId() + "_bid", price == null ? null : price.getBidPrice());
				map.put("company_" + bidCompany.getId() + "_preferential",
						price == null ? null : price.getPreferentialPrice());
			}
			result.add(map);
		}
		return result;
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	public void doUpdateObject(String clazz, String id, String field, String value, String valueType) {
		try {
			Class c = Class.forName(clazz);
			BaseDAOImpl bdao = (BaseDAOImpl) bidDAO;
			Object o = bdao.load(c, id);
			BeanUtils.setProperty(o, field, Type.get(valueType, value));
			bdao.saveOrUpdateObject(o);
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
	}

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public void doUpdateAssociation(String clazz, String id, String field, String value, String valueType) {
		try {
			Class c = Class.forName(clazz);
			BaseDAOImpl bdao = (BaseDAOImpl) bidDAO;
			Object bean = bdao.load(c, id);
			Class propertyType = BeanUtils.getPropertyClass(bean, field);
			Object newValue = bdao.load(propertyType, (Serializable) Type.get(valueType, value));
			BeanUtils.setProperty(bean, field, newValue);
			bdao.saveOrUpdateObject(bean);
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
	}

	public void doUpdateBidPrice(String clazz, String biddingId, String companyId, String bidPriceField, String value) {
		Bidding bidding = bidDAO.getBidding(biddingId);
		BidPrice bp = bidding.findBidPrice(companyId);
		BaseDAOImpl bdao = (BaseDAOImpl) bidDAO;
		if (bp == null) {
			bp = new BidPrice();
			bp.setCompany(bdao.load(BidCompany.class, companyId));
			bidding.getBidPrices().add(bp);
		}
		try {
			BeanUtils.setProperty(bp, bidPriceField, StringUtils.isEmpty(value) ? null : new BigDecimal(value));
		} catch (Exception e) {
			throw new RuntimeException(e);
		}
		bdao.saveOrUpdateObject(bp);
	}

	public void doDeleteBidding(String id) {
		BaseDAOImpl bdao = (BaseDAOImpl) bidDAO;
		Bidding bidding = bdao.load(Bidding.class, id);
		bdao.deleteObject(bidding);
	}

	public String getBidAnalysisList(PageInfo pageInfo,String bidID,String query,int userId) {
		// TODO Auto-generated method stub
		
		List list =  bidDAO.getBidAnalysisList(bidID,query,userId);
		List aList = getListWithPageInfo(list, pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
					list.size());
	}
	public List getBidAnalysisDetail(String bidID) {
		// TODO Auto-generated method stub
		return bidDAO.getBidAnalysisDetail(bidID);
	}
	public List getListWithPageInfo(List list, PageInfo pageInfo) {
		List resultList = new ArrayList();
		int limit = pageInfo.getLimit();
		int start = pageInfo.getStart();
		int size = list.size();
		int limitSize = start + limit;
		if (limitSize > size) {
			limitSize = size;
		}
		if (limit == 0 || list.size() < limit) {
			return list;
		} else {
			for (int i = start; i < limitSize; i++) {
				resultList.add(list.get(i));
			}
			return resultList;
		}

	}
	public void doSaveBidPlan(String bidID, String content) {
		// TODO Auto-generated method stub
		bidDAO.doSaveBidPlan(bidID,content);
	}
}
