package src;

public class CompanySimpleObj {
	private int companyID;
	private String companyName;
	private String companyAbb;
	private int deptID;
	private String deptName;
	private String deptAbb;
	private int empID;
	private String empName;
	
	private int departmentDirectlyFlag;
	private String type;

	
	public CompanySimpleObj() {

	}
	/**
	 * @return the companyID
	 */
	public int getCompanyID() {
		return companyID;
	}
	/**
	 * @param companyID the companyID to set
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
	 * @param companyName the companyName to set
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
	 * @param deptID the deptID to set
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
	 * @param deptName the deptName to set
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
	 * @param empID the empID to set
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
	 * @param empName the empName to set
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
	 * @param type the type to set
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
	 * @param deptAbb the deptAbb to set
	 */
	public void setDeptAbb(String deptAbb) {
		this.deptAbb = deptAbb;
	}

}
