//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\impl\\OrganizationDAOImpl.java

package com.bitc.jk.xmis.dao.impl;

import java.sql.CallableStatement;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Types;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Random;

import org.apache.log4j.Logger;
import org.springframework.jdbc.core.BatchPreparedStatementSetter;
import org.springframework.jdbc.core.CallableStatementCallback;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementSetter;

import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.DeptartmentObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

public class OrganizationDAOImpl implements OrganizationDAO {
	private static Logger logger = Logger.getLogger(OrganizationDAOImpl.class);

	private JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate ajdbcTemplate) {
		this.jt = ajdbcTemplate;
	}


	/**
	 * @param loginName
	 * @param password
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C197CA10251
	 */
	public EmployeeObj userValidete(final String loginName,
			final String password) {

		logger.info("===> userValidete = " + loginName + "/" + password);

		String sqlStr = "{call xmis.userValidata(?,?)}";
		// final EmployeeObj user = new EmployeeObj();

		Object o = jt.execute(sqlStr, new CallableStatementCallback() {

			public Object doInCallableStatement(CallableStatement cs)
					throws SQLException {

				cs.setString(1, loginName);
				cs.setString(2, password);
				ResultSet rs = cs.executeQuery();

				EmployeeObj user = null;
				while (rs.next()) {

					user = new EmployeeObj();
					user.setUserID(rs.getInt("user_id"));
					user.setUserName(rs.getString("emp_name"));
					user.setDepartmentID(rs.getInt("dept_id"));
					user.setDepartmentName(rs.getString("dept_name"));
					user.setPosition(rs.getString("position"));
					user.setDivisionID(rs.getInt("division_id"));
					user.setDivisionName(rs.getString("division_name"));
					user.setCompanyID(rs.getInt("company_id"));
					user.setCompanyName(rs.getString("compy_name"));
				}

				return user;
			}

		});

		return (EmployeeObj) o;
	}

	/**
	 * @param userID
	 * @return ataSource
	 * @roseuid 4C197E6B0261
	 */
	public EmployeeObj getUserByID(int userID) {
		return null;
	}

	/**
	 * @param companyID
	 * @return List
	 * @roseuid 4C1F03AE01D4
	 */
	public List getDepartmentTree(final int divisionID) {

		logger.info("===> getDepartmentTree = " + String.valueOf(divisionID));

		// JdbcTemplate jt = new JdbcTemplate(XMISDB);
		// jt.update("insert into xmis.temp values(10,'3')");

		/*
		 * String sqlStr = "{call xmis.departmentTree(?)}"; final List aList =
		 * new ArrayList();
		 * 
		 * Object o = jt.execute(sqlStr, new CallableStatementCallback() {
		 * 
		 * public Object doInCallableStatement(CallableStatement cs) throws
		 * SQLException {
		 * 
		 * cs.setInt(1, divisionID); ResultSet rs = cs.executeQuery();
		 * 
		 * while (rs.next()) { DepartmentTree deptTree = new DepartmentTree();
		 * 
		 * deptTree.setDepartID(rs.getInt("dept_id"));
		 * deptTree.setDepartName(rs.getString("dept_name")); //
		 * deptTree.setUserID(rs.getString("user_id") == null ? 0 : // rs //
		 * .getInt("user_id")); //
		 * deptTree.setUserName(rs.getString("user_name") == null ? // "" // :
		 * rs.getString("user_name")); deptTree.setUserID(rs.getInt("user_id"));
		 * deptTree.setUserName(rs.getString("user_name"));
		 * 
		 * aList.add(deptTree); } return null; }
		 * 
		 * });
		 */

		logger.info(SQLScript.getDepartmentTree);
		return jt.queryForList(SQLScript.getDepartmentTree,
				new Object[] { new Integer(divisionID), new Integer(divisionID) });

	}

	/**
	 * @param empID;int
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C1F03AE0222
	 */
	public Map getEmployeeInfo(int empID) {

		logger.info("===> getEmployeeInfo");

		logger.info(SQLScript.getEmployeeInof);

		return jt.queryForMap(SQLScript.getEmployeeInof,
				new Object[] { new Integer(empID) });

	}

	/**
	 * @param companyObj
	 * @roseuid 4C1F03AE0251
	 */
	public void doSaveCompanyInfo(CompanyObj companyObj) {

	}

	/**
	 * @param departmentObj
	 * @roseuid 4C1F03AE0280
	 */
	public void doSaveDepartmentInfo(DeptartmentObj departmentObj) {

	}

	/**
	 * @param employeeObj
	 * @roseuid 4C1F03AE02BF
	 */
	public void doSaveEmployeeInfo(final EmployeeObj emp,
			final EmployeeObj userDNA) {
		logger.info("===> doSaveEmployeeInfo");

		jt.update(SQLScript.updateEmployeeInfo, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {

				ps.setString(1, emp.getFirstName().trim());
				ps.setString(2, emp.getLastName().trim());
				ps.setString(3, emp.getLoginName().trim());
				ps.setInt(4, emp.getSex());

				if (emp.getJoinDate() == null)
					ps.setNull(5, Types.DATE);
				else
					ps.setDate(5, emp.getJoinDate());

				if (emp.getBirthday() == null)
					ps.setNull(6, Types.DATE);
				else
					ps.setDate(6, emp.getBirthday());

				if ("".equals(emp.getIDCard()))
					ps.setNull(7, Types.VARCHAR);
				else
					ps.setString(7, emp.getIDCard());

				if ("".equals(emp.getMobileNo()))
					ps.setNull(8, Types.VARCHAR);
				else
					ps.setString(8, emp.getMobileNo());

				if ("".equals(emp.getEmailAddress()))
					ps.setNull(9, Types.VARCHAR);
				else
					ps.setString(9, emp.getEmailAddress());

				if ("".equals(emp.getWebPage()))
					ps.setNull(10, Types.VARCHAR);
				else
					ps.setString(10, emp.getWebPage());

				if ("".equals(emp.getPosition()))
					ps.setNull(11, Types.VARCHAR);
				else
					ps.setString(11, emp.getPosition());

				if ("".equals(emp.getSwitchboardNo()))
					ps.setNull(12, Types.VARCHAR);
				else
					ps.setString(12, emp.getSwitchboardNo());

				if ("".equals(emp.getExtNo()))
					ps.setNull(13, Types.VARCHAR);
				else
					ps.setString(13, emp.getExtNo());

				if ("".equals(emp.getFaxNo()))
					ps.setNull(14, Types.VARCHAR);
				else
					ps.setString(14, emp.getFaxNo());

				if ("".equals(emp.getHomeAddress()))
					ps.setNull(15, Types.VARCHAR);
				else
					ps.setString(15, emp.getHomeAddress());

				if ("".equals(emp.getZipCode()))
					ps.setNull(16, Types.INTEGER);
				else
					ps.setInt(16, Integer.parseInt(emp.getZipCode()));

				if ("".equals(emp.getHomeTelNo()))
					ps.setNull(17, Types.VARCHAR);
				else
					ps.setString(17, emp.getHomeTelNo());

				if (emp.getPoliticalStatus() == 0)
					ps.setNull(18, Types.INTEGER);
				else
					ps.setInt(18, emp.getPoliticalStatus());

				if ("".equals(emp.getNativePlace()))
					ps.setNull(19, Types.VARCHAR);
				else
					ps.setString(19, emp.getNativePlace());

				ps.setInt(20, emp.getMaritalStatus());

				if ("".equals(emp.getSpouseName()))
					ps.setNull(21, Types.VARCHAR);
				else
					ps.setString(21, emp.getSpouseName());

				if ("".equals(emp.getIssueName()))
					ps.setNull(22, Types.VARCHAR);
				else
					ps.setString(22, emp.getIssueName());

				ps.setInt(23, emp.getIssueSex());

				if ("".equals(emp.getHobby()))
					ps.setNull(24, Types.VARCHAR);
				else
					ps.setString(24, emp.getHobby());

				if ("".equals(emp.getSpecialty()))
					ps.setNull(25, Types.VARCHAR);
				else
					ps.setString(25, emp.getSpecialty());

				ps.setInt(26, emp.getDisabledFlag());

				if ("".equals(emp.getRemark()))
					ps.setNull(27, Types.VARCHAR);
				else
					ps.setString(27, emp.getRemark());

				ps.setInt(28, userDNA.getUserID());
				ps.setInt(29, emp.getUserID());

			}
		});
	}

	/**
	 * @param id
	 * @param type
	 * @return List
	 * @roseuid 4C1F03AE02FD
	 */
	public List getProjectListByID(int id, String type) {

		logger.info("===> getProjectListByID " + String.valueOf(id) + "/"
				+ type);

		String sqlStr = new String();

		if (WebKeys.Type_Division.equals(type))
			sqlStr = SQLScript.getProjectListByDivision;

		logger.debug(sqlStr);

		return jt.queryForList(sqlStr, new Object[] { new Integer(id),
				new Integer(id), new Integer(id) });

	}

	public List getTotalSummary(String type, int id, PageInfo pageInfo) {

		logger.info("===> getTotalSummary " + String.valueOf(id) + "/" + type);

		String sqlStr = new String();
		String orderByStr = pageInfo.getSort() == null ? "" : "\n order by "
				+ pageInfo.getSort() + " " + pageInfo.getDir();
		

		if (WebKeys.Type_Department.equals(type))
//			sqlStr = SQLScript.getEmpTotalSummaryStr + orderByStr
//					+ SQLScript.getResultStr;
			sqlStr = WebUtil.generateSQLStr(SQLScript.getEmpTotalSummaryStr,
					pageInfo);

		logger.debug(sqlStr);

		return jt.queryForList(sqlStr, new Object[] { new Integer(id),
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) });
	}

	public int doChangePassword(int userID, String password, int updateID) {

		logger.info("===> doChangePassword /" + String.valueOf(userID) + "/"
				+ password);

		logger.info(SQLScript.changePassword);
		return jt.update(SQLScript.changePassword, new Object[] { password,
				new Integer(updateID), new Integer(userID) });

	}

	public int doSaveUserPhoto(int userID, String imgName, int updateID) {

		logger.info("===> doSaveUserPhoto /" + String.valueOf(userID) + "/"
				+ imgName);

		logger.info(SQLScript.saveUserPhotoName);
		return jt.update(SQLScript.saveUserPhotoName, new Object[] { imgName,
				new Integer(updateID), new Integer(userID) });
	}

	public List getPoliticalStatusList() {

		logger.info("===> getPoliticalStatusList ");

		logger.info(SQLScript.getPoliticalStatusList);

		return jt.queryForList(SQLScript.getPoliticalStatusList);
	}

	public List getLunchAddressList() {

		logger.info("===> getLunchAddressList ");

		logger.info(SQLScript.getLunchAddressList);

		return jt.queryForList(SQLScript.getLunchAddressList);
	}

	public List getEducationLevelList(int userID) {
		return null;
	}

	public int doUpdateEdu(final EmployeeObj emp, final int updateID) {

		logger.info("===> doUpdateEdu " + SQLScript.updateEdu
				+ SQLScript.changeEduForEmployee + "//"
				+ emp.getGraduationDate().toString());

		jt.update(SQLScript.updateEdu + SQLScript.changeEduForEmployee,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {

						ps.setString(1, emp.getSchoolName());

						if (emp.getGraduationDate() == null)
							ps.setNull(2, Types.DATE);
						else
							ps.setDate(2, emp.getGraduationDate());

						ps.setString(3, emp.getMajor());
						ps.setString(4, emp.getEducationRemark());
						ps.setInt(5, emp.getEducationHighestLevel());
						ps.setInt(6, emp.getEducationHighestLevel());
						ps.setInt(7, updateID);
						ps.setInt(8, emp.getUserID());

					}
				});

		return emp.getEducationHighestLevel();
	}

	public int doCreateNewEdu(final EmployeeObj emp, final int updateID) {
		logger.info("===> doCreateNewEdu " + SQLScript.createNewEdu
				+ SQLScript.changeEduForEmployee);

		final int maxEduID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.EducationLevel.EducationLevelID,
				DBSchema.EducationLevel.TableName)) + 1;

		logger.debug(String.valueOf(maxEduID));

		jt.update(SQLScript.createNewEdu + SQLScript.changeEduForEmployee,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, maxEduID);
						ps.setString(2, emp.getEducationLevelName());
						ps.setString(3, emp.getSchoolName());

						if (emp.getGraduationDate() == null)
							ps.setNull(4, Types.DATE);
						else
							ps.setDate(4, emp.getGraduationDate());

						ps.setString(5, emp.getMajor());
						ps.setString(6, emp.getEducationRemark());
						ps.setInt(7, maxEduID);
						ps.setInt(8, updateID);
						ps.setInt(9, emp.getUserID());

					}
				});

		return maxEduID;
	}

	public int doCreateDept(final int divID, final String deptName,
			final int updateID) {

		logger.info("===> doCreateDept " + SQLScript.createNewDept
				+ SQLScript.setDeptBelongToDept);

		final int maxID = jt.queryForInt(SQLScript
				.getMaxIDFromTab(DBSchema.Department.DepartmentID,
						DBSchema.Department.TableName))+1;

		jt.update(SQLScript.createNewDept + SQLScript.setDeptBelongToDept,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, maxID);
						ps.setString(2, deptName);
						ps.setInt(3, maxID);
						ps.setInt(4, divID);
					}
				});

		return maxID;
	}

	public int doCreateUser(final int deptID, final String userName,
			final int updateID) {

		logger.info("===> doCreateUser " + SQLScript.createNewUser
				+ SQLScript.setEmpBelongToDept + "//" + deptID + "//"
				+ userName.substring(0, 1) + "//" + userName.substring(1));

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.Employee.ID, DBSchema.Employee.TableName))+1;

		jt.update(SQLScript.createNewUser + SQLScript.setEmpBelongToDept
				+ SQLScript.setDefaultAddressID
				+ SQLScript.setDefaultLunchAddress,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, maxID);
						ps.setString(2, userName.substring(0, 1));
						ps.setString(3, userName.substring(1).trim());
						ps.setString(4, WebUtil.getRandomString(4));
						ps.setInt(5, updateID);
						ps.setInt(6, maxID);

						ps.setInt(7, deptID);
						ps.setInt(8, deptID);
						ps.setInt(9, deptID);
						ps.setInt(10, deptID);
						ps.setInt(11, maxID);
						ps.setInt(12, maxID);
						ps.setInt(13, updateID);
					}
				});

		return maxID;

	}

	public Map doCreateDeptBatch(final int divID, final Map deptMap,
			final int updateID) {

		logger.info("===> doCreateDeptBatch " + SQLScript.createNewDept
				+ SQLScript.setDeptBelongToDept);

		logger.debug(String.valueOf(deptMap.size()));

		final int maxID = jt.queryForInt(SQLScript
				.getMaxIDFromTab(DBSchema.Department.DepartmentID,
						DBSchema.Department.TableName))+1;

		final Map newDeptMap = new HashMap();
		BatchPreparedStatementSetter setter = null;
		setter = new BatchPreparedStatementSetter() {
			public int getBatchSize() {

				return deptMap.size();
			}

			Iterator it = deptMap.entrySet().iterator();

			public void setValues(PreparedStatement ps, int index)
					throws SQLException {

				Map.Entry entry = (Map.Entry) it.next();

				ps.setInt(1, maxID + index);
				ps.setString(2, (String) entry.getValue());
				ps.setString(3, (String) entry.getValue());
				ps.setInt(4, maxID + index);
				ps.setInt(5, divID);

				// newDeptMap.remove(entry.getKey());
				newDeptMap.put(entry.getKey(), String.valueOf(maxID + index));

			}
		};

		jt.batchUpdate(SQLScript.createNewDept + SQLScript.setDeptBelongToDept,
				setter);

		return newDeptMap;
	}

	public void doCreateEmpBatch(final Map empMap, final Map empDeptMapping,
			final int updateID) throws Exception {

		logger.info("===> doCreateEmpBatch " + SQLScript.createNewUser
				+ SQLScript.setEmpBelongToDept
				+ String.valueOf(Math.abs(new Random().nextInt())));

		logger.debug(String.valueOf(empMap.size()));

		try {
			final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
					DBSchema.Employee.ID, DBSchema.Employee.TableName))+1;

			BatchPreparedStatementSetter setter = null;
			setter = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return empMap.size();
				}

				Iterator it = empMap.entrySet().iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {

					Map.Entry entry = (Map.Entry) it.next();

					ps.setInt(1, maxID + index);
					ps
							.setString(2, ((String) entry.getValue())
									.substring(0, 1));
					ps.setString(3, ((String) entry.getValue()).substring(1)
							.trim());
					ps.setString(4, WebUtil.getRandomString(4));
					ps.setInt(5, updateID);
					ps.setInt(6, maxID + index);

					logger.info((String) empDeptMapping.get(entry.getKey()));

					int deptID = ((String) empDeptMapping.get(entry.getKey()))
							.indexOf("dept-") == 0 ? Integer
							.parseInt(((String) empDeptMapping.get(entry
									.getKey())).substring(5)) : Integer
							.parseInt((String) empDeptMapping.get(entry
									.getKey()));

					ps.setInt(7, deptID);
					ps.setInt(8, deptID);
					ps.setInt(9, deptID);
					ps.setInt(10, deptID);
					ps.setInt(11, maxID + index);
					ps.setInt(12, maxID + index);
					ps.setInt(13, updateID);
				}
			};

			jt.batchUpdate(SQLScript.createNewUser
					+ SQLScript.setEmpBelongToDept
					+ SQLScript.setDefaultAddressID
					+ SQLScript.setDefaultLunchAddress, setter);
		} catch (Exception e) {
			throw e;
		}
	}

	public void doDisableEmployee(int empID, int operatorID) {

		logger.info("---->   doDisableEmployee " + SQLScript.disableEmployee);
		jt.update(SQLScript.disableEmployee, new Object[] {
				new Integer(operatorID), new Integer(empID) });
	}

	public void doDisableDepartment(int deptID, int operatorID) {

		logger
				.info("---->   doDisableDepartment "
						+ SQLScript.disableDeparment);
		jt.update(SQLScript.disableDeparment,
				new Object[] { new Integer(deptID) });
	}

	public int doUpdateLunchAddress(final int newAddress, final int empID,
			final int operatorID) {

		logger.info("---->   doUpdateLunchAddress "
				+ SQLScript.updateLunchAddress);

		return jt.update(SQLScript.inertLunchAddress, new Object[] {
				new Integer(empID), new Integer(newAddress),
				new Integer(operatorID) });

	}

	public List getLunchAddressListByDiv(int divID) {

		logger.info("===> getLunchAddressListByDiv " + String.valueOf(divID)
				+ SQLScript.getLunchAddressListByDiv);

		return jt.queryForList(SQLScript.getLunchAddressListByDiv,
				new Object[] { new Integer(divID) });

	}

	public int doBookLunchAddress() {

		logger.info("===> doBookLunchAddress" + SQLScript.lunchAddressBooked);

		return jt.update(SQLScript.lunchAddressBooked);

	}

	public List getCompanyList() {
		logger.info("===> getCompanyList " + SQLScript.getCompanyList);

		return jt.queryForList(SQLScript.getCompanyList);
	}

	public List getCompanyList(int needRemovedCompanyID) {
		logger.info("===> getCompanyList " + SQLScript.getCompanyList);

		return jt.queryForList(SQLScript.getCompanyList);
	}

	public List getDeptListByCompanyID(int companyID) {
		return null;
	}

}
