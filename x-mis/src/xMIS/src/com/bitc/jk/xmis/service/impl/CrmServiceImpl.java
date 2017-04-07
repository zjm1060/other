package com.bitc.jk.xmis.service.impl;

import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.dao.IncorrectResultSizeDataAccessException;

import com.bitc.jk.xmis.dao.CrmDAO;
import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.service.CrmService;
import com.bitc.jk.xmis.util.DuplicatedException;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

public class CrmServiceImpl implements CrmService {
	private static Logger logger = Logger.getLogger(CrmServiceImpl.class);

	private CrmDAO crmDAO;

	/**
	 * @return the crmDAO
	 */
	public CrmDAO getCrmDAO() {
		return crmDAO;
	}

	/**
	 * @param crmDAO
	 *            the crmDAO to set
	 */
	public void setCrmDAO(CrmDAO crmDAO) {
		this.crmDAO = crmDAO;
	}

	private int companySimpleValidate(CompanySimpleObj comp) {

		logger.info("---> companySimpleValidate ? type = " + comp.getType());

		try {
			if (WebKeys.New_Company_Simple_Type_Company.equals(comp.getType()))
				return crmDAO.companyValidate(comp.getCompanyName());

			else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
					.getType()))
				return crmDAO.departmentValidate(comp.getDeptName(), comp
						.getCompanyID());

			else if (WebKeys.New_Company_Simple_Type_ContactPerson.equals(comp
					.getType()))
				return crmDAO.empValidate(comp.getEmpName(), comp
						.getCompanyID());

			else
				return 0;
		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}

	}

	public String dataValidate(CompanySimpleObj comp) {

		logger.info("---> dataValidate ? type = " + comp.getType());

		String msgStr = "";
		try {
			if (WebKeys.New_Company_Simple_Type_Company.equals(comp.getType())) {
				if (crmDAO.companyValidate(comp.getCompanyName()) > 0)
					msgStr = "公司：" + comp.getCompanyName();
			}

			else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
					.getType())) {
				if (crmDAO.departmentValidate(comp.getDeptName(), comp
						.getCompanyID()) > 0)
					msgStr = "部门：" + comp.getDeptName();

			} else if (WebKeys.New_Company_Simple_Type_ContactPerson
					.equals(comp.getType())) {
				if(comp.getDeptID() == 0)
					comp.setDeptID(crmDAO.getDefaultDeptByCompany(comp.getCompanyID()));
				
				if (crmDAO.empValidate(comp.getEmpName(), comp.getCompanyID(), comp.getDeptID()) > 0)
					msgStr = "联系人：" + comp.getEmpName();
			}

		} catch (EmptyResultDataAccessException Ee) {
			Ee.printStackTrace();
			// return -1;
		} catch (IncorrectResultSizeDataAccessException Ie) {
			Ie.printStackTrace();

			if (WebKeys.New_Company_Simple_Type_Company.equals(comp.getType())) {
				msgStr = "公司：" + comp.getCompanyName();
			}

			else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
					.getType())) {
				msgStr = "部门：" + comp.getDeptName();

			} else if (WebKeys.New_Company_Simple_Type_ContactPerson
					.equals(comp.getType())) {
				msgStr = "联系人：" + comp.getEmpName();
			}

		}

		return "".equals(msgStr)?"": msgStr + " 已存在，请确认是否新建？";
	}

	public String doCreateNewCompanySimple(CompanySimpleObj comp,
			EmployeeObj userDNA) throws DuplicatedException, Exception {

		logger.info("---> doCreateNewCompanySimple ? type = " + comp.getType());

		String returnInfo = "";

		try {
			// if (companySimpleValidate(comp) > 0) {
			//
			// String errMsg = "";
			// if (WebKeys.New_Company_Simple_Type_Company.equals(comp
			// .getType()))
			// errMsg = comp.getCompanyName();
			// else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
			// .getType()))
			// errMsg = comp.getDeptName();
			// else if (WebKeys.New_Company_Simple_Type_ContactPerson
			// .equals(comp.getType()))
			// errMsg = comp.getEmpName();
			// throw new DuplicatedException(errMsg + " 已经存在，请重新输入！");
			// } else {

			// create new company
			if (WebKeys.New_Company_Simple_Type_Company.equals(comp.getType())) {
				
				if(comp.getCompanyAbb() == null || "".equals(comp.getCompanyAbb()))
					comp.setCompanyAbb(comp.getCompanyName().trim());
				
				comp.setCompanyID(crmDAO
						.doCreateNewCompanySimple(comp, userDNA));

				// create new department
				if (comp.getDeptName() == null || "".equals(comp.getDeptName())) {
					comp.setDeptName(WebKeys.New_Company_Simple_For_Dept_Other);
				}
				comp.setDeptAbb(comp.getDeptName());
				comp.setDeptID(crmDAO.doCreateNewDeptSimple(comp, userDNA));
				comp.setDeptName(WebKeys.New_Company_Simple_For_Dept_Other
						.equals(comp.getDeptName()) ? "" : comp.getDeptName());
				comp.setDepartmentDirectlyFlag(1);
				crmDAO.doSetDeptBelongToCompanyBySimple(comp, userDNA);

				// create new crm user
				if (comp.getEmpName() != null && !"".equals(comp.getEmpName()))
					comp.setEmpID(crmDAO.doCreateNewContactPersonSimple(comp,
							userDNA));
			} else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
					.getType())) {
				comp.setDeptAbb(comp.getDeptName());
				comp.setDeptID(crmDAO.doCreateNewDeptSimple(comp, userDNA));
				comp.setDepartmentDirectlyFlag(1);
				crmDAO.doSetDeptBelongToCompanyBySimple(comp, userDNA);
			} else if (WebKeys.New_Company_Simple_Type_ContactPerson
					.equals(comp.getType())) {

				if (comp.getDeptID() == 0) {
					try {
						comp.setDeptID(crmDAO.getDefaultDeptByCompany(comp
								.getCompanyID()));
					} catch (Exception e) {
						comp
								.setDeptName(WebKeys.New_Company_Simple_For_Dept_Other);
						comp.setDeptID(crmDAO.doCreateNewDeptSimple(comp,
								userDNA));
						comp.setDepartmentDirectlyFlag(1);
						crmDAO.doSetDeptBelongToCompanyBySimple(comp, userDNA);
					}
				}
				comp.setEmpID(crmDAO.doCreateNewContactPersonSimple(comp,
						userDNA));
			}

			if (WebKeys.New_Company_Simple_Type_Company.equals(comp.getType()))
				returnInfo = "compID:'" + comp.getCompanyID() + "',compName:'"
						+ comp.getCompanyName().trim() + "',deptID:'"
						+ comp.getDeptID() + "',deptName:'"
						+ comp.getDeptName() + "',empID:'" + comp.getEmpID()
						+ "',empName:'" + comp.getEmpName() + "'";
			else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
					.getType()))
				returnInfo = "deptID:'" + comp.getDeptID() + "',deptName:'"
						+ comp.getDeptName() + "'";
			else if (WebKeys.New_Company_Simple_Type_ContactPerson.equals(comp
					.getType()))
				returnInfo = "empID:'" + comp.getEmpID() + "',empName:'"
						+ comp.getEmpName() + "'";

			// }

			// } catch (DuplicatedException de) {
			//
			// throw de;

		} catch (Exception e) {
			e.getStackTrace();
			return "";
		}

		logger.info(returnInfo);
		return returnInfo;

	}

	public String getPeasonListByCompany(int compID, EmployeeObj userDNA) {

		logger.info("---> getPeasonListByCompany  ");

		List aList;
		// if (userDNA.getCompanyID() == compID) {
		// // get peason list by self-company
		//
		// aList = crmDAO.getPeasonListBySelfCompany(compID);
		//
		// } else {
		// // get peason list by other-company
		//
		// aList = crmDAO.getPeasonListByCompany(compID);
		//
		// }
		aList = crmDAO.getPeasonListBySelfCompany(compID);
		return WebUtil.buildStoreForJsonWithoutPageInfo(aList,
				WebKeys.Action_Method_Name_ContactPeason);
	}

	public String getContactPersonBySearchName(int compID, String searchName,
			EmployeeObj userDNA) {

		logger.info("---> getContactPersonBySearchName  ");

		List aList;
		// if (userDNA.getCompanyID() == compID) {
		// // get peason list by self-company
		//
		// aList = crmDAO.getPeasonListBySelf(compID, searchName);
		//
		// } else {
		// // get peason list by other-company
		//
		// aList = crmDAO.getPeasonListBySearch(compID, searchName);
		//
		// }

		aList = crmDAO.getPeasonListBySelf(compID, searchName);
		return WebUtil.buildStoreForJsonWithoutPageInfo(aList,
				WebKeys.Action_Method_Name_ContactPeason);
	}
}
