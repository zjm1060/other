package com.bitc.jk.xmis.report.model;

import java.io.Serializable;

/**
 * @author Bom Wu
 * 
 */
public class Column implements Serializable {
	private static final long serialVersionUID = 1685175766290725981L;
	private String id;
	private String name;
	private String dname;
	private String type = "auto";
	private boolean hidden = false;
	private String reportId;

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

	public String getDname() {
		return dname;
	}

	public void setDname(String dname) {
		this.dname = dname;
	}

	public boolean isHidden() {
		return hidden;
	}

	public void setHidden(boolean hidden) {
		this.hidden = hidden;
	}

	public String getReportId() {
		return reportId;
	}

	public void setReportId(String reportId) {
		this.reportId = reportId;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}
}
