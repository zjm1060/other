package com.bitc.jk.xmis.service;

import java.sql.SQLException;
import java.util.List;

import org.springframework.dao.DataAccessException;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.StoreResponse;

public interface FinancyService {
	public String getFinancyIncomeRunningAccountList(String searchType,
			List<StandardQuery> querys, PageInfo pageInfo);

	public String getProjectBalance(String executeString, int type,
			String endDate, String deptName, PageInfo pageInfo)
			throws DataAccessException, SQLException;

	public String getProjectDetailsForProjectBalance(String executeString,
			String type, String startDate, String endDate, String param,
			PageInfo pageInfo) throws DataAccessException, SQLException;

	public boolean getBFlagForProjectBalance(int year);

	public String getFundsRevenueAndExpenditureStore(String executeString,
			int type, PageInfo pageInfo) throws DataAccessException,
			SQLException;

	public String getManagementFeeStore(String executeString, PageInfo pageInfo)
			throws DataAccessException, SQLException;
}
