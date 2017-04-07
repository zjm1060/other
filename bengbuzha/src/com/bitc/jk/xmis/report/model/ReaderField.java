package com.bitc.jk.xmis.report.model;

import java.io.Serializable;

public class ReaderField implements Serializable {
	private static final long serialVersionUID = -7107470362134753553L;
	private String name;
	private String mapping;
	private String type;

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getMapping() {
		return mapping;
	}

	public void setMapping(String mapping) {
		this.mapping = mapping;
	}

	public String getType() {
		return type;
	}

	public void setType(String type) {
		this.type = type;
	}
}
