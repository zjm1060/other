package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;

public class Sex implements Serializable {
	private static final long serialVersionUID = 3125088598357593630L;
	private int id;
	private String name;

	public Sex() {
		super();
	}

	public Sex(int id, String name) {
		this();
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
