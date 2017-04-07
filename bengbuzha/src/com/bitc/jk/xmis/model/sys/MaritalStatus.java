package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;

public class MaritalStatus implements Serializable {
	private static final long serialVersionUID = 8770541697988411376L;
	private int id;
	private String description;

	public MaritalStatus() {
		super();
	}

	public MaritalStatus(int id, String description) {
		super();
		this.id = id;
		this.description = description;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

}
