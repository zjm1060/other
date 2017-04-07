package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;

public class Post implements Serializable {
	private static final long serialVersionUID = 8399193481008317647L;
	public static final int ZHUREN = 10;
	public static final int FUZHUREN = 11;
	private int id;
	private String name;
	private String description;
	private int px;

	public Post() {
	}
	
	public Post(int id) {
		super();
		this.id = id;
	}

	public Post(int id, String description) {
		super();
		this.id = id;
		this.description = description;
	}

	public Post(int id, String name, String description, int px) {
		super();
		this.id = id;
		this.name = name;
		this.description = description;
		this.px = px;
	}

	public int getPx() {
		return px;
	}

	public void setPx(int px) {
		this.px = px;
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

	public boolean isZhuren() {
		return this.id == ZHUREN;
	}
	
	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + id;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Post other = (Post) obj;
		if (id != other.id)
			return false;
		return true;
	}

}
