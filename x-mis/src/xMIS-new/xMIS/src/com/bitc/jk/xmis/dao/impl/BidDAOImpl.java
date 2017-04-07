package com.bitc.jk.xmis.dao.impl;

import java.math.BigDecimal;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.orm.hibernate3.HibernateCallback;

import com.bitc.jk.xmis.dao.BidDAO;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.bid.BidCompany;
import com.bitc.jk.xmis.model.bid.Bidding;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class BidDAOImpl extends BaseDAOImpl implements BidDAO {
	private JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate jt) {
		this.jt = jt;
	}

	public void saveBidding(Bidding bidding) {
		saveOrUpdateObject(bidding);
	}

	public void saveBidCompany(BidCompany company) {
		saveOrUpdateObject(company);
	}

	public int getBidCompanyMaxOrder() {
		final String hql = "select max(t.order) from BidCompany t";
		return (Integer) getHt().executeWithNativeSession(
				new HibernateCallback() {

					public Object doInHibernate(Session session)
							throws HibernateException, SQLException {
						Query query = session.createQuery(hql);
						return query.uniqueResult();
					}
				});
	}

	public List<BidCompany> getBidCompanies() {
		PageInfo pageInfo = new PageInfo();
		pageInfo.setDir(PageInfo.ASC);
		pageInfo.setSort("order");
		StandardQuery query = null;
		return getListStore(query, pageInfo, BidCompany.class);
	}

	public List<Bidding> findBiddings(StandardQuery query) {
		return getListStore(query, Bidding.class);
	}

	public Bidding getBidding(String id) {
		return load(Bidding.class, id);
	}

	public List getBidAnalysisList(String bidID, String query, int userId) {
		// TODO Auto-generated method stub
		String sql = " SELECT  tb.ID , "
				+ "         tb.名称 , "
				+ "         org.公司名称 , "
				+ "         tb.区域 , "
				+ "        CONVERT(VARCHAR(10),tb.投标日期,120) as 投标日期  , "
				+ "         LTRIM(RTRIM(hr.姓氏)) + LTRIM(RTRIM(hr.名字)) AS 商务经理 , "
				+ "         LTRIM(RTRIM(fang_an.姓氏)) + LTRIM(RTRIM(fang_an.名字)) AS 方案编写 , "
				+ "         tb.方案, " + "         tb.中标单位 , "
				+ "         tb.合同价  " + "          "
				+ " FROM    xmis.bid_投标 tb , " + "         xmis.org_公司 org , "
				+ "         xmis.hr_人员 hr , " + "         xmis.hr_人员 fang_an "
				+ " WHERE   tb.业主单位 *= org.公司ID "
				+ "         AND tb.商务经理 *= hr.人员ID "
				+ "         AND tb.方案编写 *= fang_an.人员ID "
				+ " 		ORDER BY tb.投标日期 ";
		String sqlSearch = " SELECT  tb.ID , "
				+ "         tb.名称  as 名称, "
				+ "         org.公司名称   as 公司名称, "
				+ "         tb.区域  as 区域 , "
				+ "        CONVERT(VARCHAR(10),tb.投标日期,120) as 投标日期  , "
				+ "         LTRIM(RTRIM(hr.姓氏)) + LTRIM(RTRIM(hr.名字)) AS 商务经理 , "
				+ "         LTRIM(RTRIM(fang_an.姓氏)) + LTRIM(RTRIM(fang_an.名字)) AS 方案编写 , "
				+ "         tb.方案, " + "         tb.中标单位  as 中标单位 , "
				+ "         tb.合同价  " + "   		into #result   "
				+ " FROM    xmis.bid_投标 tb , " + "         xmis.org_公司 org , "
				+ "         xmis.hr_人员 hr , " + "         xmis.hr_人员 fang_an "
				+ " WHERE   tb.业主单位 *= org.公司ID "
				+ "         AND tb.商务经理 *= hr.人员ID "
				+ "         AND tb.方案编写 *= fang_an.人员ID "
				+ " 		ORDER BY tb.投标日期 ";
		String sqlDetali = " SELECT  tb.ID , "
				+ "         tb.名称 , "
				+ "         org.公司名称 , "
				+ "         tb.区域 , "
				+ "        CONVERT(VARCHAR(10),tb.投标日期,120) as 投标日期  , "
				+ "         LTRIM(RTRIM(hr.姓氏)) + LTRIM(RTRIM(hr.名字)) AS 商务经理 , "
				+ "         LTRIM(RTRIM(fang_an.姓氏)) + LTRIM(RTRIM(fang_an.名字)) AS 方案编写 , "
				+ "         tb.方案, " + "         tb.中标单位 , "
				+ "         tb.合同价  " + "          "
				+ " FROM    xmis.bid_投标 tb , " + "         xmis.org_公司 org , "
				+ "         xmis.hr_人员 hr , " + "         xmis.hr_人员 fang_an "
				+ " WHERE   tb.业主单位 *= org.公司ID "
				+ "         AND tb.商务经理 *= hr.人员ID "
				+ "         AND tb.方案编写 *= fang_an.人员ID "
				+ "         AND tb.id =?" + " 		ORDER BY tb.投标日期 ";
		String sqlMaxAndMin = " SELECT Max(投标价) as max_price,MIN(投标价) AS min_price,Round(AVG(投标价),0) AS ave_price"
				+ " FROM    xmis.bid_投标价格 price , "
				+ "         xmis.bid_投标单位 dw "
				+ " WHERE   price.报价公司 = dw.ID "
				+ "         AND ( CASE WHEN price.优惠价 IS NULL THEN 0 "
				+ "                    ELSE price.优惠价 "
				+ "               END + CASE WHEN price.投标价 IS NULL THEN 0 "
				+ "                          ELSE price.投标价 "
				+ "                     END ) > 0 " + "         AND 投标ID = ?";
		if (!bidID.equals("")) {
			List list = jt.queryForList(sqlDetali, new Object[] { bidID });
			List list1 = jt.queryForList(sqlMaxAndMin, new Object[] { bidID });
			Map tempMap = new HashMap<String, BigDecimal>();
			if (list1.size() > 0) {
				((Map) (list.get(0))).put("max_price",
						((Map) list1.get(0)).get("max_price"));
				((Map) (list.get(0))).put("min_price",
						((Map) list1.get(0)).get("min_price"));
				((Map) (list.get(0))).put("ave_price",
						((Map) list1.get(0)).get("ave_price"));
			}
			return list;
		} else {
			if (query != null && !"".equals(query.trim())) {
				Map map = jt.queryForMap(
						"{call  xmis.sp_fuzzySearchForBidInfo(?,?)}",
						new Object[] {  userId + "",query });
				String value = (String) map.get("idStr");
				String sqlSearchString = "set nocount on "
						+ sqlSearch
						+ " SELECT * FROM #result , xmis.bid_tempPrjID WHERE ID = temp_prj_id AND temp_id = '"+value+"'  " +
								" drop table #result ";
				return jt.queryForList(sqlSearchString);
			}
			return jt.queryForList(sql);
		}
	}

	public List getBidAnalysisDetail(String bidID) {
		// TODO Auto-generated method stub
		String sql = " SELECT  报价公司 , " + "         dw.名称 投标单位 , "
				+ "         投标价 , " + "         优惠价 "
				+ " FROM    xmis.bid_投标价格 price , "
				+ "         xmis.bid_投标单位 dw " + " WHERE   price.报价公司 = dw.ID "
				+ "         AND ( CASE WHEN price.优惠价 IS NULL THEN 0 "
				+ "                    ELSE price.优惠价 "
				+ "               END + CASE WHEN price.投标价 IS NULL THEN 0 "
				+ "                          ELSE price.投标价 "
				+ "                     END ) > 0 " + "         AND 投标ID = ? "
				+ " ORDER BY dw.排序         ";
		// TODO Auto-generated method stub
		List list = jt.queryForList(sql, new Object[] { bidID });
		return list;
	}

	public void doSaveBidPlan(String bidID, String content) {
		// TODO Auto-generated method stub
		String sql = "UPDATE [xmis].[xmis].[bid_投标] SET 方案 = ? where ID = ?";
		jt.update(sql, new Object[] { content, bidID });
	}
}
