package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;
/**
 * 物资业务属性
 * */
public class Business implements Serializable {
	private static final long serialVersionUID = 5017749755138422037L;
	
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
