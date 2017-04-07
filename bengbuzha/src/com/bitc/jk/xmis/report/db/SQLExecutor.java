package com.bitc.jk.xmis.report.db;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class SQLExecutor {

	private String dbname;

	private Connection connection;

	public SQLExecutor(String dbName) throws SQLException {
		this.dbname = dbName;
	}

	public ResultSet executeQuery(String sql) throws SQLException {
		return executeQuery(sql, null);
	}

	public ResultSet executeQuery(String sql, Object[] values)
			throws SQLException {
		PreparedStatement ps = getConnection().prepareStatement(sql);
		if (values != null) {
			for (int i = 0; i < values.length; i++) {
				ps.setObject(i, values[i]);
			}
		}
		return ps.executeQuery();
	}

	public Object execute(Executable executable) throws SQLException {
		Object object = null;
		try {
			object = executable.execute(getConnection());
			return object;
		} catch (SQLException e) {
			throw e;
		} finally {
			this.close();
		}
	}

	public void close() {
		try {
			if (connection != null && !connection.isClosed()) {
				connection.close();
			}
			this.connection = null;
		} catch (SQLException e) {
		}
	}

	public String getDbname() {
		return dbname;
	}

	public void setDbname(String dbname) {
		this.dbname = dbname;
	}

	public Connection getConnection() throws SQLException {
		if (connection == null) {
			connection = DBConnector.getConnection(this.dbname);
		}
		return connection;
	}
}
