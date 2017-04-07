package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;

public class DisabledReason implements Serializable {

	private static final long serialVersionUID = 5017749755138422037L;

	private String id;
	private String name;
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
 
	
	
}
