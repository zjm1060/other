package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;

public class PoliticalStatus implements Serializable {
	private static final long serialVersionUID = 3096968135117018844L;
	private int id;
	private String name;

	public PoliticalStatus() {
		super();
	}

	public PoliticalStatus(int id, String name) {
		super();
		this.id = id;
		this.name = name;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

}
