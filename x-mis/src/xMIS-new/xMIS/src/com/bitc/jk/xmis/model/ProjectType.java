package com.bitc.jk.xmis.model;

import java.io.Serializable;

public class ProjectType implements Serializable {
	private static final long serialVersionUID = -894476911838530192L;
	private int id;
	private String name;

	public ProjectType() {
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
