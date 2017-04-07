package com.bitc.jk.xmis.report.core;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import net.sf.json.JSONArray;

import com.bitc.jk.xmis.report.db.Executable;
import com.bitc.jk.xmis.report.db.ReportDAOImpl;
import com.bitc.jk.xmis.report.db.SQLExecutor;
import com.bitc.jk.xmis.report.model.Column;
import com.bitc.jk.xmis.report.model.DBConnect;
import com.bitc.jk.xmis.report.model.Report;
import com.bitc.jk.xmis.report.sql.ResultSetAdapter;
import com.bitc.jk.xmis.report.util.SQLReportUtil;

public class ReportService {
	private ReportDAOImpl reportDAO;

	public List<DBConnect> getDbConnects() {
		return reportDAO.getDbConnects();
	}

	public String getDBConnectsStore() {
		List<DBConnect> list = getDbConnects();
		JSONArray jsonArray = JSONArray.fromObject(list);
		return jsonArray.toString();
	}

	public ReportDAOImpl getReportDAO() {
		return reportDAO;
	}

	public void setReportDAO(ReportDAOImpl reportDAO) {
		this.reportDAO = reportDAO;
	}

	@SuppressWarnings("unchecked")
	public String doInitSQLReport(Report report) throws SQLException {
		SQLExecutor executor = new SQLExecutor(report.getDbName());
		final String sql = SQLReportUtil.getNoDataSQL(report.getSql());
		List<Column> cols = (List<Column>) executor.execute(new Executable() {
			public Object execute(Connection connection) throws SQLException {
				PreparedStatement ps = connection.prepareStatement(sql);
				ResultSet rs = ps.executeQuery();
				ResultSetAdapter rsa = new ResultSetAdapter(rs);
				return rsa.getColumns();
			}
		});
		report.setColumns(cols);
		reportDAO.saveReport(report);
		return report.getId();
	}

	public Report getReportByName(String token) throws ReportException {
		Report report = reportDAO.findReportByName(token);
		if (report == null) {
			throw new ReportException("不存在名为" + token + "的报表");
		}
		return report;
	}
}
