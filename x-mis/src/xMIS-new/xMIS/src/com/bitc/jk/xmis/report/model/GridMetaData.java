package com.bitc.jk.xmis.report.model;

import java.util.List;

import com.bitc.jk.xmis.util.JSONUtils;

public class GridMetaData {

	private List<ColumnModel> cm;

	private String title;

	private ReaderMetaData readerMetaData;

	public List<ColumnModel> getCm() {
		return cm;
	}

	public void setCm(List<ColumnModel> cm) {
		this.cm = cm;
	}

	public String buildJsonString() {
		return JSONUtils.toJSON(this, null).toString();
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public ReaderMetaData getReaderMetaData() {
		return readerMetaData;
	}

	public void setReaderMetaData(ReaderMetaData readerMetaData) {
		this.readerMetaData = readerMetaData;
	}
}
