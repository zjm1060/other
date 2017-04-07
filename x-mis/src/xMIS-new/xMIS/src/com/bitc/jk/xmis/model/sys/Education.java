package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;
import java.sql.Date;

public class Education implements Serializable {
	private static final long serialVersionUID = 1926328513604542898L;
	private int id;
	private String name;
	private String schoolName;
	private Date graduationDate;
	private String major;
	private String eduRemark;

	public Education() {
		super();
	}

	public Education(int id, String name, String schoolName,
			Date graduationDate, String major, String eduRemark) {
		super();
		this.id = id;
		this.name = name;
		this.schoolName = schoolName;
		this.graduationDate = graduationDate;
		this.major = major;
		this.eduRemark = eduRemark;
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

	public String getSchoolName() {
		return schoolName;
	}

	public void setSchoolName(String schoolName) {
		this.schoolName = schoolName;
	}

	public Date getGraduationDate() {
		return graduationDate;
	}

	public void setGraduationDate(Date graduationDate) {
		this.graduationDate = graduationDate;
	}

	public String getMajor() {
		return major;
	}

	public void setMajor(String major) {
		this.major = major;
	}

	public String getEduRemark() {
		return eduRemark;
	}

	public void setEduRemark(String eduRemark) {
		this.eduRemark = eduRemark;
	}

}
