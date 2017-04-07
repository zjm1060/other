package com.bitc.jk.xmis.report.model;

import java.io.Serializable;
import java.util.List;
import java.util.Map;

import com.bitc.jk.xmis.util.JSONUtils;

public class GridData implements Serializable {
	private static final long serialVersionUID = 6901480102208128141L;
	private List<Map<String, String>> rows;
	private boolean success = true;
	private int totalCount = 0;

	public List<Map<String, String>> getRows() {
		return rows;
	}

	public void setRows(List<Map<String, String>> rows) {
		this.rows = rows;
	}

	public boolean isSuccess() {
		return success;
	}

	public void setSuccess(boolean success) {
		this.success = success;
	}

	public int getTotalCount() {
		return totalCount;
	}

	public void setTotalCount(int totalCount) {
		this.totalCount = totalCount;
	}

	public String buildJsonString() {
		return JSONUtils.toJSON(this, null).toString();
	}

}
