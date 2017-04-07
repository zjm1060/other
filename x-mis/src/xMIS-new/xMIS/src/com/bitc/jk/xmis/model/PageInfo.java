package com.bitc.jk.xmis.model;

public class PageInfo {
	public static final String ASC = "ASC";
	public static final String DESC = "DESC";

	private int start = 0;
	private int limit = 0;
	private String sort;
	private String dir;

	public String getDir() {
		return dir;
	}

	public void setDir(String dir) {
		this.dir = dir;
	}

	public int getLimit() {
		return limit;
	}

	public void setLimit(int limit) {
		this.limit = limit;
	}

	public String getSort() {
		return sort;
	}

	public void setSort(String sort) {
		this.sort = sort;
	}

	public int getStart() {
		return start;
	}

	public void setStart(int start) {
		this.start = start;
	}
}
