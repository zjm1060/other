package bitc.sz.wzEnv.report.sql;


import bitc.sz.wzEnv.report.model.Column;

import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

public class ResultSetAdapter {

	private List<String> columnNames;

	private List<String> columnHeaders;

	private List<String> columnTypes;

	public ResultSetAdapter(ResultSet rs) throws SQLException {
		columnNames = new ArrayList<String>();
		columnTypes = new ArrayList<String>();
		columnHeaders = new ArrayList<String>();
		init(rs);
	}

	public void init(ResultSet rs) throws SQLException {
		ResultSetMetaData rsMeta = rs.getMetaData();
		for (int i = 1; i <= rsMeta.getColumnCount(); i++) {
			String name = rsMeta.getColumnLabel(i);
			columnNames.add(getUniqueColumnName(name));
			columnHeaders.add(name);
			int type = rsMeta.getColumnType(i);
			columnTypes.add(Types.getExtjsType(type));
		}
	}

	public List<Column> getColumns() {
		List<Column> list = new ArrayList<Column>();
		Column column;
		for (int i = 0; i < columnNames.size(); i++) {
			column = new Column();
			column.setName(columnNames.get(i));
			column.setDname(columnHeaders.get(i));
			column.setType(columnTypes.get(i));
			column.setSort(i);
			list.add(column);
		}
		return list;
	}

	public String getUniqueColumnName(String name) {
		return this.columnNames.contains(name) ? getUniqueColumnName(name + "_")
				: name;
	}
}
