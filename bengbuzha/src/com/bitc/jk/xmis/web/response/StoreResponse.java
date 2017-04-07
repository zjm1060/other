package com.bitc.jk.xmis.web.response;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class StoreResponse extends JsonResponse{
	private boolean success;
	private List<?> rows;
	private int totalCount;
	private Map<String, Object> attributes;

	public StoreResponse() {
		super();
		attributes = new HashMap<String, Object>();
	}

	public boolean isSuccess() {
		return success;
	}

	public void setSuccess(boolean success) {
		this.success = success;
	}

	public List<?> getRows() {
		return rows;
	}

	public void setRows(List<?> rows) {
		this.rows = rows;
	}

	public int getTotalCount() {
		return totalCount;
	}

	public void setTotalCount(int totalCount) {
		this.totalCount = totalCount;
	}

	public void addAttribute(String name, Object value) {
		this.attributes.put(name, value);
	}

	public Map<String, Object> getAttributes() {
		return attributes;
	}

	public void setAttributes(Map<String, Object> attributes) {
		this.attributes = attributes;
	}

}
