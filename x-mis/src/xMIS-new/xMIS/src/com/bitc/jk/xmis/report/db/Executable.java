package com.bitc.jk.xmis.report.db;

import java.sql.Connection;
import java.sql.SQLException;

public interface Executable {
	public Object execute(Connection connection) throws SQLException;
}
