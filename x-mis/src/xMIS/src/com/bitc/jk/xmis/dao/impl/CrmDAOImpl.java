package com.bitc.jk.xmis.dao.impl;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Types;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementSetter;

import com.bitc.jk.xmis.dao.CrmDAO;
import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;

public class CrmDAOImpl implements CrmDAO {
	private static Logger logger = Logger.getLogger(CrmDAOImpl.class);

	private JdbcTemplate jt;

	/**
	 * @return the jt
	 */
	public JdbcTemplate getJt() {
		return jt;
	}

	/**
	 * @param jt
	 *            the jt to set
	 */
	public void setJt(JdbcTemplate jt) {
		this.jt = jt;
	}

	public CrmDAOImpl() {

	}

	public int doCreateNewCompanySimple(final CompanySimpleObj comp,
			final EmployeeObj userDNA) {

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.Company.CompanyID, DBSchema.Company.TableName))+1;

		jt.update(SQLScript.createNewSimpleCompany,
				new PreparedStatementSetter() {
					public void setValues(PreparedStatement ps)
							throws SQLException {

						ps.setInt(1, maxID);
						ps.setString(2, comp.getCompanyName());

						if (comp.getCompanyAbb() != null
								&& !"".equals(comp.getCompanyAbb()))
							ps.setString(3, comp.getCompanyAbb());
						else
							ps.setNull(3, Types.VARCHAR);

						ps.setInt(4, userDNA.getUserID());

					}

				});

		return maxID;
	}

	public int doCreateNewDeptSimple(final CompanySimpleObj comp,
			final EmployeeObj userDNA) {
		// TODO Auto-generated method stub

		final int maxID = jt.queryForInt(SQLScript
				.getMaxIDFromTab(DBSchema.Department.DepartmentID,
						DBSchema.Department.TableName))+1;

		jt.update(SQLScript.createNewDept, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, maxID);
				ps.setString(2, comp.getDeptName());
				ps.setString(3, comp.getDeptAbb());
			}

		});

		return maxID;
	}

	public int getDefaultDeptByCompany(final int companyID){
		
		logger.info(SQLScript.getDefaultDeptByCompany);
		
		return jt.queryForInt(SQLScript.getDefaultDeptByCompany,new Object[] { new Integer(companyID) });
		
	}
	
	
	public int doCreateNewContactPersonSimple(final CompanySimpleObj comp,
			final EmployeeObj userDNA) {
		// TODO Auto-generated method stub
		
		logger.info("===> doCreateUser " + SQLScript.createNewUser
				+ SQLScript.setEmpBelongToDept + "//" + comp.getDeptID() + "//"
				+ comp.getEmpName());

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.Employee.ID, DBSchema.Employee.TableName))+1;

		jt.update(SQLScript.createNewUser + SQLScript.setEmpBelongToDept,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						
						//create new crm
						ps.setInt(1, maxID);
						ps.setString(2, comp.getEmpName().substring(0, 1));
						ps.setString(3, comp.getEmpName().substring(1).trim());
						ps.setString(4, WebUtil.getRandomString(4));
						ps.setInt(5, userDNA.getUserID());
						
						//set user belong to dept
						ps.setInt(6, maxID);
						ps.setInt(7, comp.getDeptID());

					}
				});

		return maxID;	

//		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
//				DBSchema.ContactPerson.ContactPersonID,
//				DBSchema.ContactPerson.TableName))+1;
//
//		jt.update(SQLScript.createNewSimpleCRM, new PreparedStatementSetter() {
//			public void setValues(PreparedStatement ps) throws SQLException {
//
//				ps.setInt(1, maxID);
//				ps.setString(2, comp.getEmpName());
//
//				ps.setInt(3, comp.getCompanyID());
//
//				if (comp.getDeptID() > 0)
//					ps.setInt(4, comp.getDeptID());
//				else
//					ps.setNull(4, Types.INTEGER);
//
//				ps.setInt(5, userDNA.getUserID());
//
//			}
//
//		});
//
//		return maxID;
		
		
	}

	public void doSetDeptBelongToCompanyBySimple(final CompanySimpleObj comp,
			final EmployeeObj userDNA) {
		// TODO Auto-generated method stub

		jt.update(SQLScript.setDeptBelongToCompany,
				new PreparedStatementSetter() {
					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, comp.getCompanyID());
						ps.setInt(2, comp.getDeptID());
						ps.setInt(3, comp.getDepartmentDirectlyFlag());
					}

				});

	}

	public List getPeasonListBySelfCompany(final int companyID) {

		logger.info("===>  getPeasonListBySelfCompany //"
				+ Type.getString(companyID)
				+ SQLScript.getPeasonListBySelfCompany);

		return jt
				.queryForList(SQLScript.getPeasonListBySelfCompany,
						new Object[] { new Integer(companyID),
								new Integer(companyID) });

	}

	public List getPeasonListByCompany(final int companyID) {

		logger.info("===>  getPeasonListByCompany //"
				+ Type.getString(companyID) + SQLScript.getPeasonListByCompany);

		return jt.queryForList(SQLScript.getPeasonListByCompany,
				new Object[] { new Integer(companyID) });
	}

	public List getPeasonListBySelf(final int companyID, final String searchName) {

		logger.info("===>  getPeasonListBySelf //" + Type.getString(companyID)
				+ "//" + searchName + SQLScript.getPeasonListBySelf);

		return jt.queryForList(SQLScript.getPeasonListBySelf, new Object[] {
				new Integer(companyID), new Integer(companyID),
				"%" + searchName + "%" });

	}

	public List getPeasonListBySearch(final int companyID,
			final String searchName) {

		logger.info("===>  getPeasonListBySearch //"
				+ Type.getString(companyID) + SQLScript.getPeasonListBySearch);

		return jt.queryForList(SQLScript.getPeasonListBySearch, new Object[] {
				new Integer(companyID), "%" + searchName + "%" });
	}

	public int companyValidate(final String compnayName) {

		logger.info("===>  companyValidate //" + compnayName
				+ SQLScript.getCompanyID);

		return jt.queryForInt(SQLScript.getCompanyID,
				new Object[] { compnayName });
	}

	public int departmentValidate(final String deptName, final int compID) {
		logger.info("===>  departmentValidate //" + deptName
				+ SQLScript.getDepartmentIDByCompany);
		return jt.queryForInt(SQLScript.getDepartmentIDByCompany, new Object[] {
				deptName, new Integer(compID) });
	}

	public int empValidate(final String empName, final int compID) {
		logger.info("===>  empValidate //" + empName
				+ SQLScript.getContactPeasonIDByCompany);
		return jt.queryForInt(SQLScript.getContactPeasonIDByCompany,
				new Object[] { empName, new Integer(compID) });
	}
	
	public int empValidate(final String empName, final int compID, final int deptID) {
		logger.info("===>  empValidate //" + empName
				+ SQLScript.getUserIDByCompany);
		return jt.queryForInt(SQLScript.getUserIDByCompany,
				new Object[] {  new Integer(compID), new Integer(deptID), empName });
	}
}
