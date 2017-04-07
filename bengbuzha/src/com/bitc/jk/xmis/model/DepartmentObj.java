package com.bitc.jk.xmis.model;

import java.util.List;

public class DepartmentObj extends SimpleDeptObj {
	private static final long serialVersionUID = -1025585296406032711L;
	private String abbreviation;
	private String companyID;

	private transient List<EmployeePost> posts;

	private transient List<DepartmentObj> depts;
	
	private transient DeptDept parent;

	/**
	 * @roseuid 4C1F03AE0186
	 */
	public DepartmentObj() {

	}

	/**
	 * Access method for the abbreviation property.
	 * 
	 * @return the current value of the abbreviation property
	 */
	public String getAbbreviation() {
		return abbreviation;
	}

	/**
	 * Sets the value of the abbreviation property.
	 * 
	 * @param aAbbreviation
	 *            the new value of the abbreviation property
	 */
	public void setAbbreviation(String aAbbreviation) {
		abbreviation = aAbbreviation;
	}

	/**
	 * Access method for the companyID property.
	 * 
	 * @return the current value of the companyID property
	 */
	public String getCompanyID() {
		return companyID;
	}

	/**
	 * Sets the value of the companyID property.
	 * 
	 * @param aCompanyID
	 *            the new value of the companyID property
	 */
	public void setCompanyID(String aCompanyID) {
		companyID = aCompanyID;
	}

	public SimpleEmployee getDirector() {
		List<EmployeePost> ps = getPosts();
		if (ps == null || ps.size() == 0) {
			return null;
		}
		for (EmployeePost post : ps) {
			if (post.getPost() != null && post.getPost().isZhuren()) {
				return post.getEmployee();
			}
		}
		return null;
	}

	public List<EmployeePost> getPosts() {
		return posts;
	}

	public void setPosts(List<EmployeePost> posts) {
		this.posts = posts;
	}

	public List<DepartmentObj> getDepts() {
		return depts;
	}

	public void setDepts(List<DepartmentObj> depts) {
		this.depts = depts;
	}

	public DeptDept getParent() {
		return parent;
	}

	public void setParent(DeptDept parent) {
		this.parent = parent;
	}

}
