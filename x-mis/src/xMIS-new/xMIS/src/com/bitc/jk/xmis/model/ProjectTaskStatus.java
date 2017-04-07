package com.bitc.jk.xmis.model;

import java.io.Serializable;

public class ProjectTaskStatus implements Serializable {

	private static final long serialVersionUID = -7405174879829574739L;
	public static final int NEW = 0;
	public static final int ASSIGNED = 1;
	public static final int TAKED = 2;
	public static final int COMPLETED = 3;
	public static final int EXPIRED = 4;
	private int id;
	private String name;

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
