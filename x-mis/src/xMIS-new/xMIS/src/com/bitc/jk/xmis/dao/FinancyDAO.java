package com.bitc.jk.xmis.dao;

import java.sql.SQLException;
import java.util.List;

import org.springframework.dao.DataAccessException;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.StoreResponse;

public interface FinancyDAO {

	public List getFinancyIncomeRunningAccountList(List<StandardQuery> querys,
			PageInfo pageInfo);

	public List getFinancyOutpayRunningAccountList(List<StandardQuery> querys,
			PageInfo pageInfo);

	public StoreResponse getProjectBalance(String executeString, int type,
			String endDate, String deptName, PageInfo pageInfo)
			throws DataAccessException, SQLException;

	public StoreResponse getProjectDetailsForProjectBalance(
			String executeString, String type, String startDate,
			String endDate, String param, PageInfo pageInfo)
			throws DataAccessException, SQLException;

	public List getBFlagForProjectBalance(int year);

	public StoreResponse getFundsRevenueAndExpenditureStore(
			String executeString, int type, PageInfo pageInfo)
			throws DataAccessException, SQLException;

	public StoreResponse getManagementFeeStore(String executeString,
			PageInfo pageInfo) throws DataAccessException, SQLException;
}
