package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.util.List;

public class CompanySimpleObj implements Serializable {
	private static final long serialVersionUID = -3640081681717080131L;
	private int companyID;
	private String companyName; // 公司名称
	private String companyAbb; // 名称缩写
	private int deptID;
	private String deptName;
	private String deptAbb;
	private int empID;
	private String empName;
	
	//begin 为crm_联系人临时添加变量，开发完crm后将被取消
	private String empAddr;
	private String empPhone;
	private String empPostCode;
	private String empPost;
	private String empType;
	private String empMobilePhone;
	private String empEmail;
	//end 为crm_联系人临时添加变量，开发完crm后将被取消

	public String getEmpMobilePhone() {
		return empMobilePhone;
	}

	public void setEmpMobilePhone(String empMobilePhone) {
		this.empMobilePhone = empMobilePhone;
	}

	public String getEmpEmail() {
		return empEmail;
	}

	public void setEmpEmail(String empEmail) {
		this.empEmail = empEmail;
	}

	private int departmentDirectlyFlag;
	public String getEmpAddr() {
		return empAddr;
	}

	public void setEmpAddr(String empAddr) {
		this.empAddr = empAddr;
	}

	public String getEmpPhone() {
		return empPhone;
	}

	public void setEmpPhone(String empPhone) {
		this.empPhone = empPhone;
	}

	public String getEmpPostCode() {
		return empPostCode;
	}

	public void setEmpPostCode(String empPostCode) {
		this.empPostCode = empPostCode;
	}

	public String getEmpPost() {
		return empPost;
	}

	public void setEmpPost(String empPost) {
		this.empPost = empPost;
	}

	public String getEmpType() {
		return empType;
	}

	public void setEmpType(String empType) {
		this.empType = empType;
	}

	private String type;

	private List<DepartmentObj> depts;

	public CompanySimpleObj() {

	}

	public CompanySimpleObj(int compId) {
		this.companyID = compId;
	}

	/**
	 * @return the companyID
	 */
	public int getCompanyID() {
		return companyID;
	}

	/**
	 * @param companyID
	 *            the companyID to set
	 */
	public void setCompanyID(int companyID) {
		this.companyID = companyID;
	}

	/**
	 * @return the companyName
	 */
	public String getCompanyName() {
		return companyName;
	}

	/**
	 * @param companyName
	 *            the companyName to set
	 */
	public void setCompanyName(String companyName) {
		this.companyName = companyName;
	}

	/**
	 * @return the deptID
	 */
	public int getDeptID() {
		return deptID;
	}

	/**
	 * @param deptID
	 *            the deptID to set
	 */
	public void setDeptID(int deptID) {
		this.deptID = deptID;
	}

	/**
	 * @return the deptName
	 */
	public String getDeptName() {
		return deptName;
	}

	/**
	 * @param deptName
	 *            the deptName to set
	 */
	public void setDeptName(String deptName) {
		this.deptName = deptName;
	}

	/**
	 * @return the empID
	 */
	public int getEmpID() {
		return empID;
	}

	/**
	 * @param empID
	 *            the empID to set
	 */
	public void setEmpID(int empID) {
		this.empID = empID;
	}

	/**
	 * @return the empName
	 */
	public String getEmpName() {
		return empName;
	}

	/**
	 * @param empName
	 *            the empName to set
	 */
	public void setEmpName(String empName) {
		this.empName = empName;
	}

	/**
	 * @return the companyAbb
	 */
	public String getCompanyAbb() {
		return companyAbb;
	}

	public int getDepartmentDirectlyFlag() {
		return departmentDirectlyFlag;
	}

	public void setDepartmentDirectlyFlag(int departmentDirectlyFlag) {
		this.departmentDirectlyFlag = departmentDirectlyFlag;
	}

	public void setCompanyAbb(String companyAbb) {
		this.companyAbb = companyAbb;
	}

	/**
	 * @return the type
	 */
	public String getType() {
		return type;
	}

	/**
	 * @param type
	 *            the type to set
	 */
	public void setType(String type) {
		this.type = type;
	}

	/**
	 * @return the deptAbb
	 */
	public String getDeptAbb() {
		return deptAbb;
	}

	/**
	 * @param deptAbb
	 *            the deptAbb to set
	 */
	public void setDeptAbb(String deptAbb) {
		this.deptAbb = deptAbb;
	}

	public List<DepartmentObj> getDepts() {
		return depts;
	}

	public void setDepts(List<DepartmentObj> depts) {
		this.depts = depts;
	}

}
