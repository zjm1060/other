package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.EmployeeObj;

public interface CrmDAO {

	public int companyValidate(String compnayName);
	public int departmentValidate(String deptName, int compID);
	public int empValidate(String empName, int compID);
	public int empValidate(String empName, int compID, int deptID);
	
	public int doCreateNewCompanySimple(CompanySimpleObj comp, EmployeeObj userDNA);
	public int doCreateNewDeptSimple(CompanySimpleObj comp, EmployeeObj userDNA);
	public int doCreateNewContactPersonSimple(CompanySimpleObj comp, EmployeeObj userDNA);
	public void doSetDeptBelongToCompanyBySimple(CompanySimpleObj comp, EmployeeObj userDNA);
	
	public int getDefaultDeptByCompany(int companyID);
	public List getPeasonListBySelfCompany(int companyID);
	public List getPeasonListByCompany(int companyID);

	public List getPeasonListBySelf(int companyID, String searchName);
	public List getPeasonListBySearch(int companyID, String searchName);
	
}
