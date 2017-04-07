package com.bitc.jk.xmis.report.model;

import java.io.Serializable;

public class SortInfo implements Serializable {

	private static final long serialVersionUID = -4363922295210639764L;

	private String field;
	private String direction;

	public String getField() {
		return field;
	}

	public void setField(String field) {
		this.field = field;
	}

	public String getDirection() {
		return direction;
	}

	public void setDirection(String direction) {
		this.direction = direction;
	}

}
