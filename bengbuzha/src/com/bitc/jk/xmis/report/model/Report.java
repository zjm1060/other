package com.bitc.jk.xmis.report.model;

import java.io.Serializable;
import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.bitc.jk.xmis.report.core.ReportException;
import com.bitc.jk.xmis.report.db.SQLExecutor;

public class Report implements Serializable {
	private static final long serialVersionUID = -4864109563428911073L;
	private String id;
	private String name;
	private String sql;
	private String dbName;
	private String title;
	private List<Column> columns;

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getSql() {
		return sql;
	}

	public void setSql(String sql) {
		this.sql = sql;
	}

	public String getDbName() {
		return dbName;
	}

	public void setDbName(String dbName) {
		this.dbName = dbName;
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public List<Column> getColumns() {
		return columns;
	}

	public void setColumns(List<Column> columns) {
		this.columns = columns;
	}

	public GridMetaData buildGridMetaData() {
		GridMetaData data = new GridMetaData();
		List<ColumnModel> cm = new ArrayList<ColumnModel>();
		ReaderMetaData readerMetaData = new ReaderMetaData();
		List<ReaderField> fields = new ArrayList<ReaderField>();
		ColumnModel model;
		ReaderField field;
		for (int i = 0; i < this.columns.size(); i++) {
			Column column = this.columns.get(i);
			model = new ColumnModel();
			model.setHeader(column.getDname());
			model.setDataIndex(column.getName());
			cm.add(model);

			field = new ReaderField();
			field.setMapping(column.getName());
			field.setName(column.getName());
			field.setType(column.getType());
			fields.add(field);
		}
		readerMetaData.setFields(fields);
		data.setTitle(this.getTitle());
		data.setCm(cm);
		data.setReaderMetaData(readerMetaData);
		return data;
	}

	public GridData selectGridData() throws ReportException {
		GridData data = new GridData();
		SQLExecutor executor = null;
		try {
			executor = new SQLExecutor(getDbName());
			ResultSet rs = executor.executeQuery(getSql());
			int columnCount = rs.getMetaData().getColumnCount();
			List<Map<String, String>> rows = new ArrayList<Map<String, String>>();
			Map<String, String> map;
			while (rs.next()) {
				map = new HashMap<String, String>();
				for (int i = 1; i <= columnCount; i++) {
					map.put(getColumns().get(i - 1).getName(), rs.getString(i));
				}
				rows.add(map);
			}
			data.setRows(rows);
			return data;
		} catch (Exception e) {
			throw new ReportException(e.getMessage());
		} finally {
			executor.close();
		}
	}
}
