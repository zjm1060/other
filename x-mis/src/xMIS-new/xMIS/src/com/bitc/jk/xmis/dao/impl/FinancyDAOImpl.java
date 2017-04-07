package com.bitc.jk.xmis.dao.impl;

import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Types;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.ColumnMapRowMapper;
import org.springframework.jdbc.core.ConnectionCallback;
import org.springframework.jdbc.core.JdbcTemplate;

import com.bitc.jk.xmis.dao.FinancyDAO;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SQLScript_Financy;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.StoreResponse;

public class FinancyDAOImpl implements FinancyDAO {

	private static Logger logger = Logger.getLogger(FinancyDAOImpl.class);

	private JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate jdbcTemplate) {
		this.jt = jdbcTemplate;
	}

	public List getFinancyIncomeRunningAccountList(List<StandardQuery> querys,
			PageInfo pageInfo) {
		logger.info("getFinancyIncomeRunningAccountList");
		SQLGenerator generator = new SQLGenerator(
				SQLScript_Financy.getFinancyIncomeRunningAccountList);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}
		String sql = WebUtil.generateSQLStr(generator.getSQL(), pageInfo)
				.replace(SQLScript.getResultStringWithPagingDefault,
						SQLScript_Financy.getResultStringWithPaging);
		logger.info("sqls=" + sql);
		int end = pageInfo.getStart() + pageInfo.getLimit() + 1;
		return jt.queryForList(sql,
				new Object[] { pageInfo.getStart() + 1, end });
	}

	public List getFinancyOutpayRunningAccountList(List<StandardQuery> querys,
			PageInfo pageInfo) {
		logger.info("getFinancyOutpayRunningAccountList");
		SQLGenerator generator = new SQLGenerator(
				SQLScript_Financy.getFinancyOutpayRunningAccountList);
		for (StandardQuery query : querys) {
			query.addToSQLGenerator(generator);
		}
		String sql = WebUtil.generateSQLStr(generator.getSQL(), pageInfo)
				.replace(SQLScript.getResultStringWithPagingDefault,
						SQLScript_Financy.getResultStringWithPaging);
		logger.info("sqls=" + sql);
		int end = pageInfo.getStart() + pageInfo.getLimit() + 1;
		return jt.queryForList(sql,
				new Object[] { pageInfo.getStart() + 1, end });
	}

	public StoreResponse getProjectBalance(final String executeString,
			final int type, final String endDate, final String deptName,
			final PageInfo pageInfo) throws DataAccessException, SQLException {

		return (StoreResponse) jt.execute(new ConnectionCallback() {

			public StoreResponse doInConnection(Connection conn)
					throws SQLException, DataAccessException {

				CallableStatement cs = conn.prepareCall(executeString);
				// CallableStatement cs = conn.prepareCall(executeString,
				// ResultSet.TYPE_SCROLL_INSENSITIVE,
				// ResultSet.CONCUR_READ_ONLY);

				cs.setInt(1, type);
				cs.setString(2, endDate);
				cs.setString(3, deptName);
				cs.setString(4, String.valueOf(pageInfo.getStart() + 1));
				cs.setString(
						5,
						String.valueOf(pageInfo.getStart() + 1
								+ pageInfo.getLimit()));
				cs.setString(6, pageInfo.getSort() + "  " + pageInfo.getDir());
				cs.registerOutParameter(7, Types.VARCHAR);

				ResultSet rs = cs.executeQuery();

				int total = 0;

				ColumnMapRowMapper rowMapper = new ColumnMapRowMapper();
				List<Map<String, Object>> result = new ArrayList<Map<String, Object>>();

				while (rs.next()) {
					result.add((Map<String, Object>) rowMapper.mapRow(rs,
							result.size()));
				}

				if (((Map) result.get(result.size() - 1)).get("row_no") == null) {
					total = 0;
				} else {
					total = (Integer) ((Map) result.get(result.size() - 1))
							.get("row_no");
				}

				StoreResponse response = new StoreResponse();
				response.setSuccess(true);
				response.setTotalCount(total);
				response.setRows(result);
				response.addAttribute("type", cs.getString(7));
				return response;
			}
		});
	}

	public StoreResponse getProjectDetailsForProjectBalance(
			final String executeString, final String type,
			final String startDate, final String endDate, final String param,
			final PageInfo pageInfo) throws DataAccessException, SQLException {

		return (StoreResponse) jt.execute(new ConnectionCallback() {

			public StoreResponse doInConnection(Connection conn)
					throws SQLException, DataAccessException {

				CallableStatement cs = conn.prepareCall(executeString);

				cs.setString(1, type);
				cs.setString(2, startDate);
				cs.setString(3, endDate);
				cs.setString(4, param);

				ResultSet rs = cs.executeQuery();
				int total = 0;
				for (int i = 0; i < pageInfo.getStart(); i++) {
					rs.next();
					total++;
				}
				ColumnMapRowMapper rowMapper = new ColumnMapRowMapper();
				List<Map<String, Object>> result = new ArrayList<Map<String, Object>>();

				while (rs.next()) {
					total++;
					if (pageInfo.getLimit() == 0
							|| result.size() < pageInfo.getLimit()) {
						result.add((Map<String, Object>) rowMapper.mapRow(rs,
								result.size()));
					}
				}

				StoreResponse response = new StoreResponse();
				response.setSuccess(true);
				response.setTotalCount(total);
				response.setRows(result);
				return response;
			}
		});
	}

	public List getBFlagForProjectBalance(int year) {
		String sql = " SELECT bflag FROM xmis.fas_GL_mend WHERE year = ? AND iperiod = 12 ";
		return jt.queryForList(sql, new Object[] { year });

	}

	public StoreResponse getFundsRevenueAndExpenditureStore(
			final String executeString, final int type, final PageInfo pageInfo)
			throws DataAccessException, SQLException {

		return (StoreResponse) jt.execute(new ConnectionCallback() {

			public StoreResponse doInConnection(Connection conn)
					throws SQLException, DataAccessException {

				CallableStatement cs = conn.prepareCall(executeString);
				cs.setInt(1, type);
				ResultSet rs = cs.executeQuery();
				int total = 0;
				for (int i = 0; i < pageInfo.getStart(); i++) {
					rs.next();
					total++;
				}
				ColumnMapRowMapper rowMapper = new ColumnMapRowMapper();
				List<Map<String, Object>> result = new ArrayList<Map<String, Object>>();

				while (rs.next()) {
					total++;
					if (pageInfo.getLimit() == 0
							|| result.size() < pageInfo.getLimit()) {
						result.add((Map<String, Object>) rowMapper.mapRow(rs,
								result.size()));
					}
				}

				StoreResponse response = new StoreResponse();
				response.setSuccess(true);
				response.setTotalCount(total);
				response.setRows(result);
				return response;
			}
		});
	}

	public StoreResponse getManagementFeeStore(final String executeString,
			final PageInfo pageInfo) throws DataAccessException, SQLException {
		return (StoreResponse) jt.execute(new ConnectionCallback() {
			public StoreResponse doInConnection(Connection conn)
					throws SQLException, DataAccessException {

				CallableStatement cs = conn.prepareCall(executeString);
				ResultSet rs = cs.executeQuery();
				int total = 0;
				for (int i = 0; i < pageInfo.getStart(); i++) {
					rs.next();
					total++;
				}
				ColumnMapRowMapper rowMapper = new ColumnMapRowMapper();
				List<Map<String, Object>> result = new ArrayList<Map<String, Object>>();

				while (rs.next()) {
					total++;
					if (pageInfo.getLimit() == 0
							|| result.size() < pageInfo.getLimit()) {
						result.add((Map<String, Object>) rowMapper.mapRow(rs,
								result.size()));
					}
				}
				StoreResponse response = new StoreResponse();
				response.setSuccess(true);
				response.setTotalCount(total);
				response.setRows(result);
				return response;
			}
		});
	}
}
