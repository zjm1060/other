//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\OrganizationService.java

package com.bitc.jk.xmis.service;

import java.util.List;
import java.util.Map;

import org.dom4j.Document;

import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.DeptartmentObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.DBSchema.Employee;

public interface OrganizationService {

	public void setWebDir(String aWebDri);

	public void setWebPath(String aWebPath);

	/**
	 * @param companyID
	 * @return List
	 * @roseuid 4C1F00AC007D
	 */
	public void buildDepartmentTree(EmployeeObj userObj) throws Exception;

	/**
	 * @param empID;int
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C1F00DA0242
	 */
	public EmployeeObj getEmployeeInfo(int empID);

	/**
	 * @param companyObj
	 * @roseuid 4C1F010402CE
	 */
	public void doSaveCompanyInfo(CompanyObj companyObj);

	/**
	 * @param departmentObj
	 * @roseuid 4C1F01670119
	 */
	public void doSaveDepartmentInfo(DeptartmentObj departmentObj);

	/**
	 * @param employeeObj
	 * @roseuid 4C1F01940213
	 */
	public void doSaveEmployeeInfo(EmployeeObj employeeObj, EmployeeObj userDNA);

	/**
	 * @param employeeID
	 * @return List
	 * @roseuid 4C1F01AB0109
	 */
	public List getProjectListByEmp(int employeeID);

	/**
	 * @param departmentID
	 * @return List
	 * @roseuid 4C1F01D10290
	 */
	public List getProjectListByDept(int departmentID);

	public String getProjectListByDiv(int divisionID);

	public Document getEmpTotalSummary(int departmentID, PageInfo pageInfo);
	
	public int doChangePassword(int userID, String password, int updateID);
	
	public int doSaveUserPhoto(int userID, String imgName, int updateID);	
	
	public int doEditEducation(String oldEduName, EmployeeObj emp, int updateID);
	
	public int doCreateNewUser(String deptID, String deptName, String empName, EmployeeObj operator);
	
	public int doCreateNewBatch(Map deptMap, Map empMap, Map empDeptMapping, EmployeeObj operator) throws Exception;	
	
	public int doDisableTreeNode(String itemNode, EmployeeObj operator) throws Exception;
	public int doChangeLunchAddress (int newAddress, int empID, int operatorID);
	public String getLunchAddressListByDiv(int divID);
	public int doBookLunchAddress();
}
