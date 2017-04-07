package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;

public class Echnicalposition implements Serializable {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private Integer id;
	private String name;
	private int px;


	public int getPx() {
		return px;
	}

	public void setPx(int px) {
		this.px = px;
	}
	public Integer getId() {
		return id;
	}

	public void setId(Integer id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

}
