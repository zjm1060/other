//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\OrganizationDAO.java

package com.bitc.jk.xmis.dao;

import java.util.List;
import java.util.Map;

import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.DeptartmentObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.SQLScript;

public interface OrganizationDAO {

	/**
	 * @param loginName
	 * @param password
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C197A1A029F
	 */
	public EmployeeObj userValidete(String loginName, String password);

	/**
	 * @param userID
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C197DF9006D
	 */
	public EmployeeObj getUserByID(int userID);

	/**
	 * @param companyID
	 * @return List
	 * @roseuid 4C1F023A0399
	 */
	public List getDepartmentTree(int divisionID);

	/**
	 * @param empID;int
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C1F023A03B9
	 */
	public Map getEmployeeInfo(int empID);

	/**
	 * @param companyObj
	 * @roseuid 4C1F023B000F
	 */
	public void doSaveCompanyInfo(CompanyObj companyObj);

	/**
	 * @param departmentObj
	 * @roseuid 4C1F023B002E
	 */
	public void doSaveDepartmentInfo(DeptartmentObj departmentObj);

	/**
	 * @param employeeObj
	 * @roseuid 4C1F023B004E
	 */
	public void doSaveEmployeeInfo(EmployeeObj employeeObj, EmployeeObj userDNA);

	/**
	 * @param id
	 * @param type
	 * @return List
	 * @roseuid 4C1F023B005D
	 */
	public List getProjectListByID(int id, String type);

	public List getTotalSummary(String type, int id, PageInfo pageInfo);
	
	public int doChangePassword(int userID, String password, int updateID);

	public int doSaveUserPhoto(int userID, String imgName, int updateID);
	public List getPoliticalStatusList();
	public List getLunchAddressList();	
	public List getEducationLevelList(int userID);
	
	public int doUpdateEdu(EmployeeObj emp, int updateID);
	public int doCreateNewEdu(EmployeeObj emp, int updateID);
	
	public int doCreateDept(int divID, String deptName, int updateID);
	public int doCreateUser(int deptID, String userName, int updateID);
	
	public Map doCreateDeptBatch(int divID, Map deptMap, int updateID);
	
	public void doCreateEmpBatch(Map empMap, Map empDeptMapping, int updateID) throws Exception ;
	
	public void doDisableEmployee(int empID, int operatorID);
	public void doDisableDepartment(int deptID, int operatorID);
	
	public int doUpdateLunchAddress(int newAddress, int empID, int operatorID);
	
	public List getLunchAddressListByDiv(int divID);
	public int doBookLunchAddress();
	
	public List getCompanyList();
	public List getDeptListByCompanyID(int companyID);
}
