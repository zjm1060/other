package com.bitc.jk.xmis.service.impl;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.springframework.dao.DataAccessException;

import com.bitc.jk.xmis.dao.FinancyDAO;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.service.FinancyService;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.StoreResponse;

public class FinancyServiceImpl implements FinancyService {

	private static Logger logger = Logger.getLogger(FinancyServiceImpl.class);

	private FinancyDAO financyDAO;

	public FinancyDAO getFinancyDAO() {
		return financyDAO;
	}

	public void setFinancyDAO(FinancyDAO financyDAO) {
		this.financyDAO = financyDAO;
	}

	public String getFinancyIncomeRunningAccountList(String searchType,
			List<StandardQuery> querys, PageInfo pageInfo) {
		List results = null;
		String store = null;
		if (WebKeys.financyIncomeRunningAccount.equals(searchType)) {
			results = financyDAO.getFinancyIncomeRunningAccountList(querys,
					pageInfo);
		} else {
			results = financyDAO.getFinancyOutpayRunningAccountList(querys,
					pageInfo);
		}
		store = WebUtil.buildStoreForJsonWithTotalInfo(results, "");
		return store;
	}

	public String getProjectBalance(String executeString, int type,
			String endDate, String deptName, PageInfo pageInfo)
			throws DataAccessException, SQLException {
		return financyDAO.getProjectBalance(executeString, type, endDate,
				deptName, pageInfo).toJsonString();
	}

	public String getProjectDetailsForProjectBalance(String executeString,
			String type, String startDate, String endDate, String param,
			PageInfo pageInfo) throws DataAccessException, SQLException {
		return financyDAO.getProjectDetailsForProjectBalance(executeString,
				type, startDate, endDate, param, pageInfo).toJsonString();
	}

	public boolean getBFlagForProjectBalance(int year) {

		List list = financyDAO.getBFlagForProjectBalance(year);
		return Type.getBoolean(((Map) list.get(0)).get("bflag"));
	}

	public String getFundsRevenueAndExpenditureStore(String executeString,
			int type, PageInfo pageInfo) throws DataAccessException,
			SQLException {
		return financyDAO.getFundsRevenueAndExpenditureStore(executeString,
				type, pageInfo).toJsonString();
	}

	public String getManagementFeeStore(String executeString, PageInfo pageInfo)
			throws DataAccessException, SQLException {
		return financyDAO.getManagementFeeStore(executeString, pageInfo)
				.toJsonString();
	}
}
