package com.bitc.jk.xmis.model;

import java.sql.Date;

public class StaffRoles extends TableBean {
	private static final long serialVersionUID = 445700764960315895L;
	
	private int roleID;
	
	private String roleName;
	
	private String description;
	
	private String miji;
	
	/**
	 * 最后修改日期
	 * */
	private Date lastModified;
	
	/**
	 * 最后修改人ID
	 * */
	private SimpleEmployee lastModifier;
	
	private int xsbz;
	
	private int ygbz;
	
	private int px;

	public int getRoleID() {
		return roleID;
	}

	public void setRoleID(int roleID) {
		this.roleID = roleID;
	}

	public String getRoleName() {
		return roleName;
	}

	public void setRoleName(String roleName) {
		this.roleName = roleName;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getMiji() {
		return miji;
	}

	public void setMiji(String miji) {
		this.miji = miji;
	}

	public Date getLastModified() {
		return lastModified;
	}

	public void setLastModified(Date lastModified) {
		this.lastModified = lastModified;
	}

	public SimpleEmployee getLastModifier() {
		return lastModifier;
	}

	public void setLastModifier(SimpleEmployee lastModifier) {
		this.lastModifier = lastModifier;
	}

	public int getXsbz() {
		return xsbz;
	}

	public void setXsbz(int xsbz) {
		this.xsbz = xsbz;
	}

	public int getYgbz() {
		return ygbz;
	}

	public void setYgbz(int ygbz) {
		this.ygbz = ygbz;
	}

	public int getPx() {
		return px;
	}

	public void setPx(int px) {
		this.px = px;
	}

	public static long getSerialversionuid() {
		return serialVersionUID;
	}
}
