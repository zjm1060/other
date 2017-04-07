//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\impl\\OrganizationDAOImpl.java

package com.bitc.jk.xmis.dao.impl;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Types;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Random;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.hibernate.FlushMode;
import org.hibernate.Hibernate;
import org.hibernate.HibernateException;
import org.hibernate.SQLQuery;
import org.hibernate.Session;
import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.BatchPreparedStatementSetter;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementCallback;
import org.springframework.jdbc.core.PreparedStatementSetter;
import org.springframework.jdbc.core.RowMapper;
import org.springframework.orm.hibernate3.HibernateCallback;

import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.DepartmentObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.EmployeePost;
import com.bitc.jk.xmis.model.Node;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleDeptObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.SupplierInformation;
import com.bitc.jk.xmis.model.crm.Address;
import com.bitc.jk.xmis.model.sys.DisabledReason;
import com.bitc.jk.xmis.model.sys.PoliticalStatus;
import com.bitc.jk.xmis.model.sys.Post;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.DBSchema.EmployeeBelongToDept;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.util.sql.UserSecuritySQL;
import com.bitc.jk.xmis.web.action.param.OrgTreeParam;
import com.bitc.jk.xmis.web.response.TreeNode;

public class OrganizationDAOImpl extends BaseDAOImpl implements OrganizationDAO {
//	private static Logger logger = Logger.getLogger(OrganizationDAOImpl.class);

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

		logger.info("===> userValidete = " + loginName);

		String sqlStr = "select e.人员ID 'user_id',RTRIM(e.姓氏)+RTRIM(e.名字) 'emp_name',\n"
				+ "e.锁定 'locked',\n"
				+ " dc.公司ID 'company_id',c.公司名称 'compy_name',\n"
				+ " ed.部门ID 'dept_id',d.部门名称 'dept_name' ,ed.职务 'position',\n"
				+ " t.部门ID 'division_id' , "
				+ " t.部门名称  'division_name' \n "
				+",        CONVERT(VARCHAR(12), e.入职日期, 23)'joinDate'\n" 
				// ", ISNULL(dd.父部门ID,ed.部门ID) 'division_id',ISNULL(pd.部门名称,d.部门名称) 'division_name'\n"
				+ " from xmis.hr_人员 e\n"
				+ "left join xmis.hr_人员部门关系表 ed on ed.人员ID=e.人员ID\n"
				+ "left join xmis.org_部门 d on d.部门ID=ed.部门ID\n"
				+ "left join xmis.org_部门归属公司表 dc on dc.部门ID=ed.部门ID\n"
				+ "left join xmis.org_公司 c on c.公司ID=dc.公司ID\n"
				+ "left join xmis.org_部门归属部门表 dd on dd.部门ID=ed.部门ID\n"
				+ "left join xmis.org_部门 pd on pd.部门ID =dd.父部门ID\n"
				+ "LEFT JOIN xmis.org_部门 t ON t.部门ID = xmis.f_getDivisionID(ed.部门ID)\n"
				+ "where e.登录名=? and ( e.密码=? or UPPER(dbo.MD5(e.密码))=?)";

		Object o = jt.execute(sqlStr, new PreparedStatementCallback() {
			public Object doInPreparedStatement(PreparedStatement ps)
					throws SQLException, DataAccessException {
				ps.setString(1, loginName);
				ps.setString(2, password);
				ps.setString(3, password);
				ResultSet rs = ps.executeQuery();
				EmployeeObj user = null;
				if (rs.next()) {
					user = new EmployeeObj();
					user.setUserID(rs.getInt("user_id"));
					user.setUserName(rs.getString("emp_name"));
					user.setDepartmentID(rs.getInt("dept_id"));
					user.setDepartmentName(rs.getString("dept_name"));
					user.setPosition(rs.getString("position"));//职务ID
					user.setDivisionID(rs.getInt("division_id"));
					user.setDivisionName(rs.getString("division_name"));
					user.setCompanyID(rs.getInt("company_id"));
					user.setCompanyName(rs.getString("compy_name"));
					boolean b = rs.getBoolean("locked");
					user.setLoginName(loginName);
					user.setPassword(password);
					user.setLocked(rs.wasNull() ? false : b);
					
					user.setJoinDate(rs.getDate("joinDate"));
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
//	public EmployeeObj getUserByID(int userID) {
//		return null;
//	}

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
		return jt.queryForList(SQLScript.getDepartmentTree, new Object[] {
				new Integer(divisionID), new Integer(divisionID) });

	}

	/**
	 * @param empID
	 *            ;int
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
	public void doSaveDepartmentInfo(DepartmentObj departmentObj) {

	}

	/**
	 * @param employeeObj
	 * @roseuid 4C1F03AE02BF
	 */
	public void doSaveEmployeeInfo(final EmployeeObj emp,
			final EmployeeObj userDNA) {
		logger.info("===> doSaveEmployeeInfo" + SQLScript.updateEmployeeInfo);
		jt.update(SQLScript.updateEmployeeInfo, new PreparedStatementSetter() {

			public void setValues(PreparedStatement ps) throws SQLException {

				ps.setString(1, emp.getFirstName().trim());
				ps.setString(2, emp.getLastName().trim());
				ps.setString(3, emp.getLoginName().trim());
				ps.setInt(4, emp.getSexID());

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

				if ("".equals(emp.getPositionEdit()))
					ps.setNull(11, Types.VARCHAR);
				else
					ps.setString(11, emp.getPositionEdit());

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
					ps.setInt(16, Type.GetInt(emp.getZipCode()));

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
				if (emp.isLocked())
					ps.setInt(29, 1);
				else
					ps.setInt(29, 0);
				ps.setString(30, emp.getAddress().getAddressID());
				if ("".equals(emp.getUrgentinfo())){
					ps.setNull(31, Types.VARCHAR);
				}else {
					ps.setString(31, emp.getUrgentinfo());
				}
				ps.setInt(32, emp.getUserID());
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
			// sqlStr = SQLScript.getEmpTotalSummaryStr + orderByStr
			// + SQLScript.getResultStr;
			sqlStr = WebUtil.generateSQLStr(SQLScript.getEmpTotalSummaryStr,
					pageInfo);

		logger.debug(sqlStr);

		return jt.queryForList(
				sqlStr,
				new Object[] {
						new Integer(id),
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1
								+ pageInfo.getLimit()) });
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
						ps.setString(1, emp.getEducationLevelName());
						ps.setString(2, emp.getSchoolName());

						if (emp.getGraduationDate() == null)
							ps.setNull(3, Types.DATE);
						else
							ps.setDate(3, emp.getGraduationDate());

						ps.setString(4, emp.getMajor());
						ps.setString(5, emp.getEducationRemark());
						ps.setInt(6, emp.getEducationHighestLevel());
						ps.setInt(7, emp.getEducationHighestLevel());
						ps.setInt(8, updateID);
						ps.setInt(9, emp.getUserID());
					}
				});

		return emp.getEducationHighestLevel();
	}

	public Map doCreateDeptBatch(final int divID, final Map deptMap,
			final int updateID) {

		logger.info("===> doCreateDeptBatch " + SQLScript.createNewDept
				+ SQLScript.setDeptBelongToDept);

		logger.debug(String.valueOf(deptMap.size()));

		final int maxID = jt.queryForInt(SQLScript
				.getMaxIDFromTab(DBSchema.Department.DepartmentID,
						DBSchema.Department.TableName)) + 1;

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
						DBSchema.Department.TableName)) + 1;

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

	public void doCreateOtherDept(final int compID, final int deptID,
			final int updateID) {

		logger.info("===> doCreateDept " + SQLScript.createNewDept
				+ SQLScript.setDeptBelongToDept);

		jt.update(SQLScript.createNewDept + SQLScript.setDeptBelongToCompany,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, deptID);
						ps.setString(2, "其它");
						ps.setString(3, "其它");
						ps.setInt(4, compID);
						ps.setInt(5, deptID);
						ps.setInt(6, 1);
					}
				});

	}

	public void doCreateEmpBatch(final Map empMap, final Map empDeptMapping,
			final int updateID) throws Exception {

		logger.info("===> doCreateEmpBatch " + SQLScript.createNewUser
				+ SQLScript.setEmpBelongToDept
				+ String.valueOf(Math.abs(new Random().nextInt())));

		logger.debug(String.valueOf(empMap.size()));

		try {
			final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
					DBSchema.Employee.ID, DBSchema.Employee.TableName)) + 1;

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
					ps.setString(2, ((String) entry.getValue()).substring(0, 1));
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

	public int doCreateUser(final int deptID, final String userName,
			final int updateID) {

		logger.info("===> doCreateUser " + SQLScript.createNewUser
				+ SQLScript.setEmpBelongToDept + "//" + deptID + "//"
				+ userName.substring(0, 1) + "//" + userName.substring(1));

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.Employee.ID, DBSchema.Employee.TableName)) + 1;

		jt.update(SQLScript.createNewUser + SQLScript.setEmpBelongToDept
				+ SQLScript.setDefaultAddressID
				+ SQLScript.setDefaultLunchAddress,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setInt(1, maxID);
						ps.setString(2, userName.substring(0, 1));
						ps.setString(3, userName.substring(1).trim());
						ps.setString(4, WebUtil.getRandomString(4));// 登录名
						ps.setInt(5, updateID);
						ps.setString(6, WebUtil.getRandomString(4));// ID
						ps.setInt(6 + 1, maxID);

						ps.setInt(7 + 1, deptID);
						ps.setInt(8 + 1, deptID);
						ps.setInt(9 + 1, deptID);
						ps.setInt(10 + 1, deptID);
						ps.setInt(11 + 1, maxID);
						ps.setInt(12 + 1, maxID);
						ps.setInt(13 + 1, updateID);
					}
				});

		return maxID;

	}

	public void doDisableEmployee(int empID, int operatorID) {

		logger.info("---->   doDisableEmployee " + SQLScript.disableEmployee);
		jt.update(SQLScript.disableEmployee, new Object[] {
				new Integer(operatorID), new Integer(empID) });
	}

	public void doDisableDepartment(int deptID, int operatorID) {

		logger.info("---->   doDisableDepartment " + SQLScript.disableDeparment);
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

	public List getUserRightOnFunctionLevel(int userID) {
		logger.info("===> getUserRightOnFunctionLevel "
				+ UserSecuritySQL.getUserRolesOnFunctionLevel);

		return jt.queryForList(UserSecuritySQL.getUserRolesOnFunctionLevel,
				new Object[] { new Integer(userID), new Integer(userID),
						new Integer(userID) });
	}

	public List getUserRightByDefault() {
		logger.info("===> getUserRightByDefault "
				+ UserSecuritySQL.getUserRightByDefault);

		return jt.queryForList(UserSecuritySQL.getUserRightByDefault);
	}

	public List getSecrityDataScope(String url, int userID) {

		logger.info("===> getSecrityDataScope "
				+ UserSecuritySQL.getSecrityDataScope);

		return jt.queryForList(
				UserSecuritySQL.getSecrityDataScope,
				new Object[] { url.substring(1), new Integer(userID),
						url.substring(1), new Integer(userID) });

	}

	public CompanySimpleObj getCompanySimpleObj(final int companyID) {
		return (CompanySimpleObj) ht.load(CompanySimpleObj.class, companyID);
		// return (CompanySimpleObj) ht.execute(new HibernateCallback() {
		// public Object doInHibernate(Session session)
		// throws HibernateException, SQLException {
		// List list = session
		// .createQuery(
		// "from CompanySimpleObj as company left join fetch company.depts where company.companyID=?")
		// .setInteger(0, companyID).list();
		// return list.get(0);
		// }
		// });
	}

	public List getDeptListByID(int id, String type) {

		String sqlStr = "";

		if (type.equals(WebKeys.Organization_Type_Company)) {

			sqlStr = SQLScript.getDeptListByCompID;

		} else if (type.equals(WebKeys.Organization_Type_Division)) {

			sqlStr = SQLScript.getDeptListByDivID;

		}

		logger.info("===> getDeptListByID " + sqlStr);

		return jt.queryForList(sqlStr, new Object[] { new Integer(id) });

	}

	public List getOrgTree(final int type, final int id) {
		String hql = "";

		if (type == 1) {
			final String sql = "select h.公司ID as compID,h.名称缩写 as compName,COUNT(h.人员ID) as num from "
					+ " (select a.公司ID,a.名称缩写,e.人员ID from [xmis].[xmis].[org_公司] a  "
					+ " left outer join xmis.org_部门归属公司表 b on a.公司ID = b.公司ID  "
					+ " left outer join xmis.org_部门 c on b.部门ID = c.部门ID "
					+ " left outer join xmis.hr_人员部门关系表 d on c.部门ID = d.部门ID "
					+ " left outer join xmis.hr_人员 e on e.人员ID = d.人员ID and (e.禁用 is null or e.禁用=0)"
					+ "and e.人员ID >0 "
					+ " where a.公司ID = ? "
					+ ") h group by h.公司ID,h.名称缩写 ";

			/*
			 * List list = (List) ht .executeWithNativeSession(new
			 * HibernateCallback() { public List doInHibernate(Session session)
			 * SQLQuery query = session.createSQLQuery(sql) throws
			 * HibernateException, SQLException { .addScalar("compID",
			 * Hibernate.INTEGER) .addScalar("compName", Hibernate.STRING)
			 * .addScalar("num", Hibernate.INTEGER); query.setInteger(0, id);
			 * List list = query.list(); return list; } }); return list;
			 */
			return jt.query(sql, new Object[] { id }, new RowMapper() {

				public Object mapRow(ResultSet rs, int arg1)
						throws SQLException {
					Object[] objs = new Object[3];
					objs[0] = rs.getObject(1);
					objs[1] = rs.getObject(2);
					objs[2] = rs.getObject(3);
					return objs;
				}
			});

		} else if (4 == type) {
			hql = "from EmployeePost t left join fetch t.employee left join fetch t.post where t.department.departmentID='"
					+ id + "' and t.employee.userID > 0 order by t.post.px";
			return ht.find(hql);
		} else if (3 == type) {
			final String sql = "select dd.部门ID deptid,rtrim(ee.姓氏)+rtrim(ee.名字) as name ,de.人员ID as empid, '0' as isPerson,"
					+ " case when ee.禁用 IS null then 0 else ee.禁用 end disable, "
					+ " p.职务名称 as post,"
					+ " case when p.排序 IS null then 10000 else p.排序 end px"
					+ " , CASE  WHEN ee.密码='000000'  AND ee.修改时间>CONVERT(VARCHAR(100),GETDATE(),23) THEN 1   ELSE 0 END AS newperson,de.排序 AS personPX"
					+ " from hr_人员部门关系表 de"
					+ " left join org_部门 dd on dd.部门ID=de.部门ID"
					+ " left join hr_人员 ee on ee.人员ID = de.人员ID"
					+ " left join sys_职务 p on p.职务ID = de.职务"
					+ " where dd.部门ID =?"
					+ " and de.人员ID>0 "
					+ " union all"
					+ " select a.部门ID as deptid,a.部门名称 as name,"
					+ "xmis.f_getNumByDeptId(a.部门ID) as enpid,"
					+
					// "COUNT(a.人员ID) as enpid," +
					"'1' as isPerson,"
					+ " case when a.disable IS null then 0 else a.disable end disable,"
					+ " null as post, 10000 as px ,0 AS newperson ,10000 AS personPX from ("
					+ " select aa.部门ID,aa.部门名称,"
					+ " case when aa.人员ID=0 then null else aa.人员ID end 人员ID,"
					+ " '1' as '是否是人',aa.disable as disable from"
					+ " (select d.部门ID,d.部门名称,"
					+ " null as 人员ID,"
					+ " '0' as '是否是人',"
					+ " d.禁用 as disable "
					+ " from"
					+ "  org_部门归属部门表 dd "
					+ "left join "
					+ " org_部门 d "
					+ " on dd.部门ID=d.部门ID"
					+ " where"
					+ " dd.父部门ID =?"
					+ " union all "
					+ " select"
					+ " d.部门ID,"
					+ " d.部门名称,"
					+ " case when de.人员ID IS null then 0 else de.人员ID end 人员ID,"
					+ " '1' as '是否是人',d.禁用 as disable"
					+ " from org_部门归属部门表 dd"
					+ " left join org_部门 d on dd.部门ID=d.部门ID "
					+ " left join hr_人员部门关系表 de on de.部门ID=dd.部门ID"
					+ " left join hr_人员 ee on ee.人员ID = de.人员ID"
					+ " where dd.父部门ID =?"
					+ " and (ee.禁用 is null or ee.禁用 = 0)) aa where  (aa.人员ID >0 or aa.人员ID is null)"
					+ " ) as a"
					+ " group by a.部门ID,a.部门名称,a.disable"
					+ " order by isPerson,disable,px,personPX,deptid,name";
			List list = (List) ht
					.executeWithNativeSession(new HibernateCallback() {
						public List doInHibernate(Session session)
								throws HibernateException, SQLException {
							SQLQuery query = session.createSQLQuery(sql)
									.addScalar("deptid", Hibernate.INTEGER)
									.addScalar("name", Hibernate.STRING)
									.addScalar("empid", Hibernate.INTEGER)
									.addScalar("isPerson", Hibernate.STRING)
									.addScalar("disable", Hibernate.INTEGER)
									.addScalar("post", Hibernate.STRING)
									.addScalar("px", Hibernate.INTEGER)
									.addScalar("newperson", Hibernate.INTEGER)
									.addScalar("personPX", Hibernate.INTEGER);
							query.setInteger(0, id);
							query.setInteger(1, id);
							query.setInteger(2, id);
							List list = query.list();
							return list;
						}
					});
			return list;
		} else {
			final String sql = "select "
					+ "a.部门ID as deptID, "
					+ "a.部门名称  as deptName,"
					+ "xmis.f_getNumByDeptId(a.部门ID) as num,"
					// + " COUNT(a.人员ID) as num,"
					+ " a.禁用 as disable,"
					+ " a.px as px "
					+ "from "
					+ "( "
					// --公司下所有事业部
					+ " select"
					+ "  d1.部门ID,"
					+ " d1.部门名称,"
					+ " null as 人员ID, "
//					+ "0 as 禁用, "
					+ " CASE WHEN  d1.禁用 IS NOT NULL then d1.禁用 ELSE 0 end as 禁用, "
					+ "case when cd1.排序 is null then 10000 "
					+ "+ d1.部门ID else cd1.排序 end as px  "
					+ "from org_部门 d1"
					+ "   left join  org_部门归属公司表 cd1   "
					+ " on cd1.部门ID=d1.部门ID  "
					+ "where  "
					+ "cd1.公司ID=?"
					+ " and cd1.是否直属=1 "
					+ "union all "
					// --事业部门直属人员
					+ " select     "
					+ "d1.部门ID,"
					+ "d1.部门名称,"
					+ "de1.人员ID," 
//							+"0,"
					+ " CASE WHEN  d1.禁用 IS NOT NULL then d1.禁用 ELSE 0 end as 禁用, "
					+ "case  when cd1.排序 is null then 10000 + d1.部门ID else cd1.排序  end as px  "
					+ " from org_部门 d1 "
					+ "left join     org_部门归属公司表 cd1    on cd1.部门ID=d1.部门ID  "
					+ "left join     hr_人员部门关系表 de1    on de1.部门ID=d1.部门ID  "
					+ "left join     hr_人员 ee1    on ee1.人员ID = de1.人员ID"
					+ " where     cd1.公司ID=?     and cd1.是否直属=1 "
					+ " and (  ee1.禁用 is null or ee1.禁用 = 0 ) "
					+ "and ee1.人员ID>0"
					+ " union all"
					// --二级部门下的人员
					+ " select     " + "d.部门ID," + "d.部门名称,    " + " de.人员ID,"
					+ "0 as 禁用," + "case when cd.排序 is null then 10000 "
					+ "+ d.部门ID else cd.排序 end as px  " + "  from "
					+ "org_部门 d "
					+ "left join  org_部门归属公司表 cd    on cd.部门ID=d.部门ID   "
					+ "left join org_部门归属部门表 dd  on dd.父部门ID=d.部门ID"
					+ " left join  hr_人员部门关系表 de    on de.部门ID=dd.部门ID   "
					+ "left join  hr_人员 ee  on ee.人员ID = de.人员ID "
					+ " where  cd.公司ID=? " + " and cd.是否直属=1 "
					+ "and de.人员ID>0 and "
					+ "(  ee.禁用 is null or ee.禁用 = 0)"
					+ ") as a "
					// --WHERE
					// -- a.禁用<>1
					+ "group by    a.部门ID,    a.部门名称,    a.禁用,    a.px  "
					+ "order by    a.px,    a.部门名称";
			final String sql2 = "select" + " a.部门ID as deptID,"
					+ " a.部门名称  as deptName," + " COUNT(a.人员ID) as num,"
					+ " a.禁用 as disable," + " a.px as px" + " from"
					+ " ( select" + " d1.部门ID," + " d1.部门名称," + " de1.人员ID,"
					+ " case when d1.禁用 is null then 0 else d1.禁用 end as 禁用,"
					+ " cd1.排序 as px " + " from" + " org_部门 d1 " + " left join"
					+ " org_部门归属公司表 cd1 " + " on cd1.部门ID=d1.部门ID "
					+ " left join" + " hr_人员部门关系表 de1 "
					+ " on de1.部门ID=d1.部门ID  " + " left join" + " hr_人员 ee1 "
					+ " on ee1.人员ID = de1.人员ID " + " where" + " cd1.公司ID=?"
					+ " and cd1.是否直属=1 " + " and (" + " ee1.禁用 is null "
					+ " or ee1.禁用 = 0" + " ) " + " and (" + " ee1.人员ID>0 "
					+ " or ee1.人员ID is null" + " ) " + " union" + " all select"
					+ " d.部门ID," + " d.部门名称," + " de.人员ID,"
					+ " case when d.禁用 is null then 0 else d.禁用 end as 禁用,"
					+ " cd.排序" + " from" + " org_部门 d " + " left join"
					+ " org_部门归属公司表 cd " + " on cd.部门ID=d.部门ID " + " left join"
					+ " org_部门归属部门表 dd " + " on dd.父部门ID=d.部门ID "
					+ " left join" + " hr_人员部门关系表 de " + " on de.部门ID=dd.部门ID "
					+ " left join" + " hr_人员 ee " + " on ee.人员ID = de.人员ID "
					+ " where" + " cd.公司ID=? " + " and de.人员ID>0 "

					+ " and cd.是否直属=1  " + " and (" + " ee.禁用 is null "
					+ " or ee.禁用 = 0" + " ) " + " ) as a " + " WHERE a.禁用<>1 "
					+ " group by" + " a.部门ID," + " a.部门名称," + " a.禁用,"
					+ " a.px " + " order by" + " a.px," + " a.部门名称";
			List list = (List) ht
					.executeWithNativeSession(new HibernateCallback() {
						public List doInHibernate(Session session)
								throws HibernateException, SQLException {
							SQLQuery query = session.createSQLQuery(sql);
							query.setInteger(0, id);
							query.setInteger(1, id);
							query.setInteger(2, id);
							List list = query.list();
							return list;
						}
					});
			return list;
		}

	}

	public EmployeeObj getEmployee(int id) {
		logger.info("---> getEmployee (dao impl)" + id);
		return (EmployeeObj) ht.get(EmployeeObj.class, id);
	}

	public SimpleEmployee getSimpleEmployee(int userID) {
		return (SimpleEmployee) ht.get(SimpleEmployee.class, userID);
	}

	public List getEmployeeList(String tableObj, int ID) {
		// TODO Auto-generated method stub
		String hql = "from " + tableObj + " t where t.id=" + ID
				+ " order by t.id";
		return ht.find(hql);
	}

	public List<PoliticalStatus> getPoliticsStatusTotalSummary() {
		logger.info("---> getPoliticalStatusTotalSummary (dao impl)");

		String hql = "from PoliticalStatus order by id";
		return ht.find(hql);
	}

	public EmployeeObj checkLoginNameExist(String loginName, int userid) {
		String hql = "from EmployeeObj e where e.loginName='" + loginName
				+ "' and e.id<>" + userid;
		List<EmployeeObj> list = ht.find(hql);
		return (list != null && list.size() > 0) ? list.get(0) : null;
	}

	public Map<String, String> doCreateEmplBatch(
			final Map<String, String> emplMap,
			final Map<String, String> mappingMap, final int updateID)
			throws Exception {
		logger.info("===> doCreateEmpBatch (dao impl)");
		logger.info("===> doCreateEmpBatch (dao impl) emplMap.size = "
				+ String.valueOf(emplMap.size()));
		logger.info("===> doCreateEmpBatch (dao impl) SQL = "
				+ SQLScript.createNewUser);

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.Employee.ID, DBSchema.Employee.TableName)) + 1;
		final Map<String, String> newEmplMap = new HashMap<String, String>();
		BatchPreparedStatementSetter setter = null;
		setter = new BatchPreparedStatementSetter() {
			public int getBatchSize() {
				return emplMap.size();
			}

			Iterator it = emplMap.entrySet().iterator();

			public void setValues(PreparedStatement ps, int index)
					throws SQLException {

				Map.Entry entry = (Map.Entry) it.next();

				ps.setInt(1, maxID + index);
				ps.setString(2, ((String) entry.getValue()).substring(0, 1));
				ps.setString(3, ((String) entry.getValue()).substring(1).trim());
				ps.setString(4, (String) entry.getValue());
				ps.setInt(5, updateID);

				newEmplMap.put(entry.getKey() + "",
						String.valueOf(maxID + index));
			}
		};
		jt.batchUpdate(SQLScript.createNewUser, setter);
		return newEmplMap;
	}

	public Map<String, String> doCreateDeptBatch(
			final Map<String, String> deptMap, int updateID) {
		logger.info("===> doCreateDeptBatch (dao impl)");
		logger.info("===> doCreateDeptBatch (dao impl) deptMap.size = "
				+ String.valueOf(deptMap.size()));
		logger.info("===> doCreateDeptBatch (dao impl) SQL = "
				+ SQLScript.createNewDept);

		final int maxID = jt.queryForInt(SQLScript
				.getMaxIDFromTab(DBSchema.Department.DepartmentID,
						DBSchema.Department.TableName)) + 1;

		final Map<String, String> newDeptMap = new HashMap<String, String>();
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
				newDeptMap.put(entry.getKey() + "",
						String.valueOf(maxID + index));
			}
		};
		jt.batchUpdate(SQLScript.createNewDept, setter);
		return newDeptMap;
	}

	public void doUpdateDeptBatch(final Map<String, String> mappingDeptMap,
			final Map<String, String> mappingCompMap) {
		// TODO Auto-generated method stub
		logger.info("===> doUpdateDeptBatch " + SQLScript.setDeptBelongToDept);
		// logger.debug(String.valueOf(mappingDeptMap.size()));

		if (mappingCompMap != null) {
			BatchPreparedStatementSetter setter2 = null;
			setter2 = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return mappingCompMap.size();
				}

				Iterator it = mappingCompMap.entrySet().iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {
					Map.Entry entry = (Map.Entry) it.next();

					int id = Type.GetInt(((String) entry.getKey()).trim()
							.substring(9));
					int pid = Type.GetInt(((String) entry.getValue()).trim()
							.substring(9));
					ps.setInt(1, pid);
					ps.setInt(2, id);
					ps.setInt(3, 1);
				}
			};
			jt.batchUpdate(SQLScript.setDeptBelongToCompany, setter2);
		}

		if (mappingDeptMap != null) {
			BatchPreparedStatementSetter setter1 = null;
			setter1 = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return mappingDeptMap.size();
				}

				Iterator it = mappingDeptMap.entrySet().iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {
					Map.Entry entry = (Map.Entry) it.next();

					int id = Type.GetInt(((String) entry.getKey()).trim()
							.substring(9));
					int pid = Type.GetInt(((String) entry.getValue()).trim()
							.substring(9));
					ps.setInt(1, id);
					ps.setInt(2, pid);
				}
			};
			jt.batchUpdate(SQLScript.setDeptBelongToDept, setter1);

			BatchPreparedStatementSetter setter2 = null;
			setter2 = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return mappingDeptMap.size();
				}

				Iterator it = mappingDeptMap.entrySet().iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {
					Map.Entry entry = (Map.Entry) it.next();

					int id = Type.GetInt(((String) entry.getKey()).trim()
							.substring(9));
					int pid = Type.GetInt(((String) entry.getValue()).trim()
							.substring(9));

					List list = jt.queryForList(SQLScript.getCompIDByDeptID,
							new Object[] { pid });
					int compID = -1;
					if (list != null && list.size() > 0) {
						compID = Integer.parseInt(((Map) list.get(0)).get(
								DBSchema.DeptBelongToCompany.companyID)
								.toString());
					} else {
						System.out.println("pid=" + pid);
						List pdids = jt.queryForList(
								SQLScript.getParentDeptIDByDeptID,
								new Object[] { pid });
						System.out.println("size=" + pdids.size());
						while (pdids != null && pdids.size() > 0) {
							pid = Integer.parseInt(((Map) pdids.get(0)).get(
									DBSchema.DeptBelongToDept.ParentsID)
									.toString());
							System.out.println("pid" + pid);
							pdids = jt.queryForList(
									SQLScript.getParentDeptIDByDeptID,
									new Object[] { pid });
						}
						if (pdids == null || pdids.size() == 0) {
							System.out.println("pid" + pid);
							List l = jt.queryForList(
									SQLScript.getCompIDByDeptID,
									new Object[] { pid });
							compID = Integer.parseInt(((Map) l.get(0)).get(
									DBSchema.DeptBelongToCompany.companyID)
									.toString());
						}
					}
					ps.setInt(1, compID);
					ps.setInt(2, id);
					ps.setInt(3, 0);
				}
			};
			jt.batchUpdate(SQLScript.setDeptBelongToCompany, setter2);

		}

	}

	public void doUpdateEmplBatch(final Map<String, String> mappingEmplMap,
			final int updateID) {
		logger.info("===> doUpdateEmplBatch (dao impl)");
		logger.info("===> doUpdateEmplBatch mappingEmplMap.size = "
				+ String.valueOf(mappingEmplMap.size()));
		logger.info("===> doUpdateEmplBatch SQLScript.setEmpBelongToDept = "
				+ SQLScript.setEmpBelongToDept);
		logger.info("===> doUpdateEmplBatch SQLScript.setDefaultAddressID = "
				+ SQLScript.setDefaultAddressID);
		logger.info("===> doUpdateEmplBatch SQLScript.setDefaultLunchAddress = "
				+ SQLScript.setDefaultLunchAddress);

		BatchPreparedStatementSetter setter = null;
		setter = new BatchPreparedStatementSetter() {
			public int getBatchSize() {
				return mappingEmplMap.size();
			}

			Iterator it = mappingEmplMap.entrySet().iterator();

			public void setValues(PreparedStatement ps, int index)
					throws SQLException {
				Map.Entry entry = (Map.Entry) it.next();

				int eid = Type.GetInt(((String) entry.getKey()).trim()
						.substring(9));
				int did = Type.GetInt(((String) entry.getValue()).trim()
						.substring(9));

				String a = UUIDUtils.nextCode();
				ps.setString(1, a);
				ps.setInt(2, eid);
				ps.setInt(3, did);

				ps.setInt(4, eid);
				ps.setInt(5, updateID);

				ps.setString(6, UUIDUtils.nextCode());
				ps.setInt(7, eid);
				ps.setInt(8, 5);

				// ps.setInt(4, did);
				// ps.setInt(5, did);
				// ps.setInt(6, did);
				// ps.setInt(7, eid);

				// ps.setInt(8, eid);
				// ps.setInt(9, updateID);
				//
				// ps.setString(10, UUIDUtils.nextCode());
				// ps.setInt(11, eid);
				// ps.setInt(12, 5);
			}
		};

		jt.batchUpdate(SQLScript.setEmpBelongToDept
		// + SQLScript.setDefaultAddressID
				+ SQLScript.setDefaultLunchAddress + SQLScript.setEmpRole,
				setter);
	}

	public void doUpdateDeptBelongToDeptBatch(
			final Map<String, String> dragEmplToDeptMap,
			final Map<String, String> dragDeptToDeptMap,
			final Map<String, String> dragCompToCompMap,
			final Map<String, String> dragCompToDeptMap,
			final Map<String, String> dragDeptToCompMap) {
		// TODO Auto-generated method stub
		logger.info("===> doUpdateDeptBelongToDeptBatch ");
		logger.info("===> doUpdateDeptBelongToDeptBatch="
				+ dragEmplToDeptMap.size());
		BatchPreparedStatementSetter setter = null;
		if (dragEmplToDeptMap != null && dragEmplToDeptMap.size() > 0) {
			setter = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return dragEmplToDeptMap.size();
				}

				Iterator it = dragEmplToDeptMap.entrySet().iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {
					Map.Entry entry = (Map.Entry) it.next();
					System.out.println("<<<<<<<<<<<<<<<<<<<" + entry.getKey());
					System.out
							.println("<<<<<<<<<<<<<<<<<<<" + entry.getValue());

					int id = Type
							.GetInt(((String) entry.getKey()).substring(9));
					int pid = Type.GetInt(((String) entry.getValue())
							.substring(9));
					ps.setInt(1, pid);
					ps.setInt(2, id);
				}
			};
			jt.batchUpdate(SQLScript.changeEmpBelongToDept, setter);
		}
		if (dragDeptToDeptMap != null && dragDeptToDeptMap.size() > 0) {
			setter = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return dragDeptToDeptMap.size();
				}

				Iterator it = dragDeptToDeptMap.entrySet().iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {
					Map.Entry entry = (Map.Entry) it.next();

					int id = Type
							.GetInt(((String) entry.getKey()).substring(9));
					int pid = Type.GetInt(((String) entry.getValue())
							.substring(9));
					System.out.println(id + "-->" + pid);
					ps.setInt(1, pid);
					ps.setInt(2, id);
				}
			};
			jt.batchUpdate(SQLScript.changeDeptBelongToDept, setter);
		}
		if (dragCompToCompMap != null && dragCompToCompMap.size() > 0) {
			setter = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return dragCompToCompMap.size();
				}

				Iterator it = dragCompToCompMap.entrySet().iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {
					Map.Entry entry = (Map.Entry) it.next();
					int did = Type.GetInt(((String) entry.getKey()).trim()
							.substring(9));
					int cid = Type.GetInt(((String) entry.getValue()).trim()
							.substring(9));
					ps.setInt(1, cid);
					ps.setInt(2, did);
				}
			};
			jt.batchUpdate(SQLScript.changeDeptBelongToCompany, setter);
		}
		// dragCompToDeptMap 新部门 原来是公司
		if (dragCompToDeptMap != null && dragCompToDeptMap.size() > 0) {
			logger.info("===> doUpdateDeptBelongToDeptBatch -->dragCompToDeptMap");
			setter = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return dragCompToDeptMap.size();
				}

				Iterator it = dragCompToDeptMap.entrySet().iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {
					Map.Entry entry = (Map.Entry) it.next();
					String[] tmp = ((String) entry.getValue()).split(":");
					int did = Type.GetInt(((String) entry.getKey()).trim()
							.substring(9));
					int ndid = Type.GetInt(tmp[0].trim().substring(9));
					int cid = Type.GetInt(tmp[1].trim().substring(9));
					ps.setInt(1, did);
					ps.setInt(2, ndid);
					ps.setInt(3, did);
					ps.setInt(4, cid);
				}
			};
			jt.batchUpdate(SQLScript.setDeptBelongToDept
					+ SQLScript.deleteDeptBelongToCompany, setter);
		}

		// dragDeptToCompMap 原来是部门，现在是公司
		if (dragDeptToCompMap != null && dragDeptToCompMap.size() > 0) {
			logger.info("===> doUpdateDeptBelongToDeptBatch -->dragDeptToCompMap");
			setter = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return dragDeptToCompMap.size();
				}

				Iterator it = dragDeptToCompMap.entrySet().iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {

					Map.Entry entry = (Map.Entry) it.next();
					String[] tmp = ((String) entry.getValue()).split(":");
					int did = Type.GetInt(((String) entry.getKey()).trim()
							.substring(9));
					int cid = Type.GetInt(tmp[0].trim().substring(9));
					int pdid = Type.GetInt(tmp[1].trim().substring(9));
					ps.setInt(1, cid);
					ps.setInt(2, did);
					ps.setInt(3, 1);
					ps.setInt(4, did);
					ps.setInt(5, pdid);
				}
			};
			jt.batchUpdate(SQLScript.setDeptBelongToCompany
					+ SQLScript.deleteDeptBelongToDept, setter);
		}

	}

	public void doUpdateEmpNameBatch(final Map<String, String> changeEmplName) {
		// TODO Auto-generated method stub
		logger.info("===> doUpdateEmpNameBatch ");
		BatchPreparedStatementSetter setter = null;
		setter = new BatchPreparedStatementSetter() {
			public int getBatchSize() {
				return changeEmplName.size();
			}

			Iterator it = changeEmplName.entrySet().iterator();

			public void setValues(PreparedStatement ps, int index)
					throws SQLException {

				Map.Entry entry = (Map.Entry) it.next();

				ps.setString(1, ((String) entry.getValue()).substring(1));
				ps.setString(2, ((String) entry.getValue()).substring(0, 1));
				ps.setInt(
						3,
						Type.GetInt(((String) entry.getKey()).trim().substring(
								9)));
			}
		};

		jt.batchUpdate(SQLScript.updateEmployeeName, setter);

	}

	public void doUpdateDeptNameBatch(final Map<String, String> changeDeptName) {
		// TODO Auto-generated method stub
		logger.info("===> doUpdateEmpNameBatch ");
		BatchPreparedStatementSetter setter = null;
		setter = new BatchPreparedStatementSetter() {
			public int getBatchSize() {
				return changeDeptName.size();
			}

			Iterator it = changeDeptName.entrySet().iterator();

			public void setValues(PreparedStatement ps, int index)
					throws SQLException {
				Map.Entry entry = (Map.Entry) it.next();
				ps.setString(1, (String) entry.getValue());
				ps.setInt(
						2,
						Type.GetInt(((String) entry.getKey()).trim().substring(
								9)));
			}
		};

		jt.batchUpdate(SQLScript.updateDeptName, setter);
	}

	public List getBizloigcDataList(String paramStr, String type) {

		String sql = "";
		if (type.equals(WebKeys.Biz_Logic_Department_Type))
			sql = StringUtils.replace(UserSecuritySQL.getBizlogicDeptList,
					"()", "(" + paramStr + ")");

//		else if(type.equals(WebKeys.Biz_Logic_Opration_Type))
			
			
		logger.info(sql);

		return jt.queryForList(sql, new Object[] { new String("1") });

	}

	public int getUserNumbersByDeptID(int deptID) {
		// TODO Auto-generated method stub
		String hql = "select count(*) from EmployeePost e where e.department.departmentID="
				+ deptID
				+ " and (e.employee.flag is null or e.employee.flag=0) and e.employee.userID >0";
		List list = ht.find(hql);
		return (list != null && list.size() > 0) ? Integer.parseInt(list.get(0)
				.toString()) : 0;
	}

	public List<Post> getPositionTotalSummary() {
		logger.info("---> getPositionTotalSummary (dao impl)");

		String hql = "from Post order by px";
		return ht.find(hql);
	}

	public String updatePost(int id, int postId) {
		logger.info("---> updatePost (dao impl)");

		ht.getSessionFactory().getCurrentSession().setFlushMode(FlushMode.AUTO);
		String hql = "from EmployeePost e where e.employee.userID=" + id;
		List list = ht.find(hql);
		if (list != null && list.size() == 1) {
			EmployeePost emp = (EmployeePost) list.get(0);
			hql = "from Post where id =" + postId;
			List<Post> post = ht.find(hql);
			if (post != null && post.size() == 1) {
				emp.setPost(post.get(0));
			} else {
				emp.setPost(null);
			}
			ht.getSessionFactory().getCurrentSession().flush();
			ht.getSessionFactory().getCurrentSession().close();
			return (post != null && post.size() == 1) ? emp.getPost().getName()
					: "";
		}
		return "error";
	}

	public int getUserNumbersByCompID(int CompID) {
		// TODO Auto-generated method stub
		return jt.queryForInt(SQLScript.getEmpNumOfCompany,
				new Object[] { CompID });
	}

	public void test() {
		// TODO Auto-generated method stub
		final String sql = "select a.部门ID as deptID,a.部门名称  as deptName,COUNT(a.人员ID) as num,a.禁用 as disable from ("
				+ " select d1.部门ID,d1.部门名称,de1.人员ID,d1.禁用 "
				+ " from org_部门 d1"
				+ " left join org_部门归属公司表 cd1 on cd1.部门ID=d1.部门ID"
				+ " left join hr_人员部门关系表 de1 on de1.部门ID=d1.部门ID "
				+ " left join hr_人员 ee1 on ee1.人员ID = de1.人员ID"
				+ " where cd1.公司ID=?"
				+ " and cd1.是否直属=1"
				+ " and (ee1.禁用 is null or ee1.禁用 = 0)"
				+ " union all"
				+ " select d.部门ID,d.部门名称,de.人员ID,d.禁用 "
				+ " from org_部门 d"
				+ " left join org_部门归属公司表 cd on cd.部门ID=d.部门ID"
				+ " left join org_部门归属部门表 dd on dd.父部门ID=d.部门ID"
				+ " left join hr_人员部门关系表 de on de.部门ID=dd.部门ID"
				+ " left join hr_人员 ee on ee.人员ID = de.人员ID"
				+ " where cd.公司ID=?"
				+ " and cd.是否直属=1 "
				+ " and de.人员ID>0"
				+ " and (ee.禁用 is null or ee.禁用 = 0)"
				+ " ) as a"
				+ " group by a.部门ID,a.部门名称,a.禁用 ";
		List list = (List) ht.executeWithNativeSession(new HibernateCallback() {
			public List doInHibernate(Session session)
					throws HibernateException, SQLException {
				SQLQuery query = session.createSQLQuery(sql);
				query.setInteger(0, 1);
				query.setInteger(1, 1);
				List list = query.list();
				return list;
			}
		});

	}

	public List getOrganizationTree(int type, final int id) {
		// TODO Auto-generated method stub
		if (type == 3) {
			final String sql = "select xmis.xmis.org_部门.部门ID,xmis.xmis.org_部门.部门名称 from xmis.xmis.org_部门归属部门表"
					+ " left join xmis.xmis.org_部门 on xmis.xmis.org_部门.部门ID = xmis.xmis.org_部门归属部门表.部门ID"
					+ " where 父部门ID = ? and (禁用=0 or 禁用 is null)";
			List list = (List) ht
					.executeWithNativeSession(new HibernateCallback() {
						public List doInHibernate(Session session)
								throws HibernateException, SQLException {
							SQLQuery query = session.createSQLQuery(sql);
							query.setInteger(0, id);
							List list = query.list();
							return list;
						}
					});
			return list;
		}
		if (type == 2) {
			final String sql = "select * from ("
					+ " select d.部门ID,d.部门名称,0 as num from org_部门 d "
					+ " left join org_部门归属公司表 cd on cd.部门ID=d.部门ID"
					+ " where cd.是否直属=1 and cd.公司ID=? and (d.禁用=0 or d.禁用 is null) and d.部门ID not in("
					+ " select d.部门ID"
					+ " from org_部门 d"
					+ " left join org_部门归属公司表 cd on cd.部门ID=d.部门ID"
					+ " left join org_部门归属部门表 cc on cc.父部门ID=d.部门ID"
					+ " where cd.是否直属=1 and cd.公司ID=? and (d.禁用=0 or d.禁用 is null) and cc.父部门ID is not null and d.部门ID is not null group by d.部门ID,d.部门名称 having COUNT(*)>0)"
					+ " union all"
					+ " select d.部门ID,d.部门名称,COUNT(*)"
					+ " from org_部门 d"
					+ " left join org_部门归属公司表 cd on cd.部门ID=d.部门ID"
					+ " left join org_部门归属部门表 cc on cc.父部门ID=d.部门ID"
					+ " where cd.是否直属=1 and cd.公司ID=? and (d.禁用=0 or d.禁用 is null) and cc.父部门ID is not null and d.部门ID is not null group by d.部门ID,d.部门名称 having COUNT(*)>0) as a"
					+ " order by a.部门名称";
			List list = (List) ht
					.executeWithNativeSession(new HibernateCallback() {
						public List doInHibernate(Session session)
								throws HibernateException, SQLException {
							SQLQuery query = session.createSQLQuery(sql);
							query.setInteger(0, id);
							query.setInteger(1, id);
							query.setInteger(2, id);
							List list = query.list();
							return list;
						}
					});
			return list;
		}
		return null;
	}

	public List<Object[]> getOrgTree(OrgTreeParam param) {
		String sql = null;
		int nodeLevel = param.getNodeLevel();
		int argCount = 0;
		String ordSql = "\n declare @maxP int, @maxED int,@maxDC int,@maxDD int \n"
				+ " select @maxP = MAX(排序)+1 from xmis.sys_职务 \n"
				+ " select @maxED = MAX(排序)+1 from xmis.hr_人员部门关系表 \n "
				+ " select @maxDC = MAX(排序)+1 from xmis.org_部门归属公司表 \n"
				+ " select @maxDD = MAX(排序)+1 from xmis.org_部门归属部门表 \n";
		switch (nodeLevel) {
		case OrgTreeParam.LEVEL_ROOT:
			sql = "select c.公司ID as id,c.公司名称 as name ,'1' as level ,' ' as icon,ISNULL(a.child ,0) child, 0 as ord , 0 as postord"
					+ " from org_公司 c"
					+ " LEFT OUTER JOIN ( SELECT    co.公司ID ,ISNULL(COUNT(*),0) AS child "
					+ " FROM org_部门归属公司表 co "
					+ " WHERE co.是否直属 = 1 "
					+ " GROUP BY  co.公司ID) a ON c.公司ID = a.公司ID";
			argCount = 0;
			break;
		case OrgTreeParam.LEVEL_COMP:// 公司，查一级部门
			if (param.getLeafLevel() == OrgTreeParam.LEVEL_EMP) {
				sql = "select d.部门ID as id,d.部门名称 as name,'2' as level,'users' as icon,ISNULL(a.child,0) as child,"
						+ " case when cd.排序 is null then @maxDC else cd.排序 end ord"
						+ " , 0 as postord "
						+ " from org_部门 d "
						+ " left join org_部门归属公司表 cd on cd.部门ID=d.部门ID "
						+ " LEFT JOIN (  select 部门ID,count(*) as child from (  "
						+ " SELECT  父部门ID as 部门ID FROM    org_部门归属部门表  "
						+ " union all   select 部门ID from hr_人员部门关系表  "
						+ " ) as tmp group by 部门ID ) a ON a.部门ID = d.部门ID"
						+ " where cd.是否直属=1 and cd.公司ID=? and (d.禁用=0 or d.禁用 is null) ";
				argCount = 1;
			} else {
				sql = "select d.部门ID as id,d.部门名称 as name,'2' as level,'users' as icon,ISNULL(a.child,0) as child,"
						+ " case when cd.排序 is null then @maxDC else cd.排序 end ord"
						+ " , 0 as postord "
						+ " from org_部门 d "
						+ " left join org_部门归属公司表 cd on cd.部门ID=d.部门ID "
						+ " LEFT JOIN ( SELECT  父部门ID , COUNT(*) AS child "
						+ " FROM    org_部门归属部门表 "
						+ " GROUP BY 父部门ID ) a ON a.父部门ID = d.部门ID"
						+ " where cd.是否直属=1 and cd.公司ID=? and (d.禁用=0 or d.禁用 is null) ";
				argCount = 1;
			}
			break;
		case OrgTreeParam.LEVEL_DEPT1: // 一级部门，查人员及二级部门 //加入了部门和人员的排序
			if (param.getLeafLevel() == OrgTreeParam.LEVEL_EMP) {
				sql = " select new.id as id, new.name as name, new.level as level,new.icon as icon,new.child as child,"
						+ " case when a.排序 is null then @maxDD else a.排序 end ord "
						+ " ,0 as postord"
						+ " from ("
						+ " select d.部门ID as id,d.部门名称 as name,'2' as level,'users' as icon,"
						+ " ISNULL(ddtemp.child,0) AS child"
						+ " from org_部门归属部门表 dm"
						+ " left join org_部门 d on d.部门ID = dm.部门ID "
						+ " LEFT JOIN  (  select 部门ID,count(*) as child from ( "
						+ " SELECT  父部门ID as 部门ID FROM  org_部门归属部门表"
						+ " union all   select 部门ID from hr_人员部门关系表  "
						+ " ) as tmp  group by 部门ID ) ddtemp  ON ddtemp.部门ID = dm.部门ID"
						+ " WHERE dm.父部门ID = ? and (禁用=0 or 禁用 is null)"
						+ " GROUP BY d.部门ID ,d.部门名称 ,ddtemp.child)"
						+ " as new "
						+ " left join xmis.org_部门归属部门表  a on new.id = a.部门ID "
						+ " union all "
						+ " select e.人员ID as id,rtrim(e.姓氏)+rtrim(e.名字)  as name,'8' as level, CASE e.性别ID WHEN 1 THEN 'male' ELSE 'female' END AS icon, 0 AS child ,"
						+ " case when m.排序 is null then @maxED else m.排序 end ord "
						+ " ,case when post.排序 is null then @maxP else post.排序 end postord"
						+ " from hr_人员部门关系表 m left join hr_人员 e on e.人员ID=m.人员ID"
						+ " LEFT JOIN xmis.sys_职务 post  ON m.职务 = post.职务ID "
						+ " where m.部门ID=? and (e.禁用 =0 or e.禁用 is null) ";
				argCount = 2;
			} else {
				sql = " select new.id as id, new.name as name, new.level as level,new.icon as icon,new.child as child,"
						+ " case when a.排序 is null then @maxDD else a.排序 end ord "
						+ " ,0 as postord"
						+ " from ("
						+ " select d.部门ID as id,d.部门名称 as name,'2' as level,'users' as icon,"
						+ " ISNULL(ddtemp.child,0) AS child"
						+ " from org_部门归属部门表 dm"
						+ " left join org_部门 d on d.部门ID = dm.部门ID "
						+ " LEFT JOIN  (  SELECT  父部门ID , COUNT(*) AS child  "
						+ " FROM    org_部门归属部门表"
						+ "  GROUP BY 父部门ID ) ddtemp  ON ddtemp.父部门ID = dm.部门ID  "
						+ " WHERE dm.父部门ID = ? and (禁用=0 or 禁用 is null)"
						+ " GROUP BY d.部门ID ,d.部门名称 ,ddtemp.child)"
						+ " as new "
						+ " left join xmis.org_部门归属部门表  a on new.id = a.部门ID ";
				argCount = 1;
			}
			break;
		case OrgTreeParam.LEVEL_DEPT2:
			sql = " select e.人员ID as id,rtrim(e.姓氏)+rtrim(e.名字)  as name,'8' as level, CASE e.性别ID WHEN 1 THEN 'male' ELSE 'female' END AS icon,0 as child,"
					+ " case when m.排序 is null then @maxED else m.排序 end ord "
					+ " ,case when post.排序 is null then @maxP else post.排序 end postord"
					+ " from hr_人员部门关系表 m left join hr_人员 e on e.人员ID=m.人员ID"
					+ " LEFT JOIN xmis.sys_职务 post  ON m.职务 = post.职务ID "
					+ " where m.部门ID=? and (e.禁用 =0 or e.禁用 is null)";
			argCount = 1;
			break;
		default:
			break;
		}
		final String rootFilter = param.getRootFilter();
		final boolean needFilter = (nodeLevel == param.getRootLevel() - 1 && !StringUtils
				.isBlank(rootFilter));
		String[] filters = null;
		StringBuilder builder = new StringBuilder();
		if (needFilter) {
			filters = StringUtils.split(rootFilter, ',');
			if (StringUtils.contains(rootFilter, ',')) {
				builder.append("select * from (").append(sql)
						.append(") as r where r.id in (")
						.append(rootFilter.replaceAll("[0-9|A-Z|a-z]+", "?"))
						.append(") order by level,postord,ord,name");
				// sql = "select * from (" + sql + ") as r where r.id in ("
				// + rootFilter.replaceAll("[0-9|A-Z|a-z]+", "?")
				// + ") order by level,ord";
			} else {
				builder.append("select * from (")
						.append(sql)
						.append(") as r where r.id=? order by level,postord,ord,name");
				// sql = "select * from (" + sql
				// + ") as r where r.id=? order by level,ord";
			}
		} else {
			builder.append("select * from (")
					.append(sql)
					.append(" ) as r where r.id>0 order by level,postord,ord,name");
			// sql = "select * from (" + sql
			// + " ) as r where r.id>0 order by level,ord";
		}
		final String s = ordSql + builder.toString();
		final int count = argCount;
		final String nodeId = param.getNodeId();
		final String[] footFilters = filters;
		return (List<Object[]>) ht
				.executeWithNativeSession(new HibernateCallback() {
					public Object doInHibernate(Session session)
							throws HibernateException, SQLException {
						logger.info(s);
						SQLQuery sqlQuery = session.createSQLQuery(s);
						for (int i = 0; i < count; i++) {
							sqlQuery.setString(i, nodeId);
						}
						if (needFilter) {
							for (int j = 0; j < footFilters.length; j++) {
								sqlQuery.setString(count + j, footFilters[j]);
							}
						}
						return sqlQuery.list();
					}
				});
	}

	/*
	 * 得到用户是否可以使用高级查询中的（收付款情况、发票情况、预计来款）权限
	 */
	public boolean getDeptOfUserLimit(final int id) {
		logger.info("---> getDeptOfUserLimit");

		final String sql = "select b.[父部门ID] from xmis.org_部门归属公司表 a,[xmis].[xmis].[org_部门归属部门表] b where a.部门ID=? and a.部门ID=b.部门ID";
		List<Object[]> list = (List<Object[]>) ht
				.executeWithNativeSession(new HibernateCallback() {
					public Object doInHibernate(Session session)
							throws HibernateException, SQLException {
						SQLQuery query = session.createSQLQuery(sql);
						query.setInteger(0, new Integer(id));
						return query.list();
					}
				});
		int deptId = id;
		if (list.size() > 0) {
			deptId = Type.GetInt(list.get(0));
		}
		final String limitSQL = "select 是否关心业务数据 from xmis.org_部门 where 部门ID = "
				+ deptId;
		List<Object[]> limit = (List<Object[]>) ht
				.executeWithNativeSession(new HibernateCallback() {
					public Object doInHibernate(Session session)
							throws HibernateException, SQLException {
						SQLQuery query = session.createSQLQuery(limitSQL);
						return query.list();
					}
				});
		
//		return (list.size() > 0 && Type.GetInt(list.get(0)) == 1) ? true
//				: false;
		
		return (limit!=null && limit.size() > 0  && Type.getBoolean(limit.get(0)) == true) ? true
				: false;
	}

	public List<Integer> getUserRoles(int empId) {
		logger.info("-->getUserRoles");
		String sql = "select * from [xmis].[sec_人员角色] where 人员ID = ?";
		List<Integer> results = null;
		List list = jt.queryForList(sql, new Object[] { empId });
		if (list != null && list.size() > 0) {
			results = new ArrayList<Integer>();
			for (int i = 0; i < list.size(); i++) {
				Map map = (Map) list.get(i);
				results.add(Type.getInteger(map.get("角色ID")));
			}
		}
		return results;
	}

	public List getEmpInfoByDeptID(final int deptNo) {
		logger.info("---->addressBook(dao--getEmpInfoByDeptID)");
		String sqlStr = SQLScript.getEmpInfoByDeptID;
		logger.info("---->addressBook(dao--getEmpInfoByDeptID)" + sqlStr);
		PreparedStatementCallback ps = new PreparedStatementCallback() {
			public Object doInPreparedStatement(PreparedStatement arg0)
					throws SQLException, DataAccessException {
				arg0.setInt(1, deptNo);
				ResultSet rs = arg0.executeQuery();
				List list = new ArrayList();
				ResultSetMetaData md = rs.getMetaData();
				int columnCount = md.getColumnCount();
				while (rs.next()) {
					Map map = new HashMap();
					for (int i = 1; i <= columnCount; i++) {
						map.put(md.getColumnName(i), rs.getObject(i));
					}
					list.add(map);
				}
				return list;
			}
		};
		return (List) jt.execute(sqlStr, ps);
	}

	// public List<EmployeeObj> getLeaderInfo() {
	public List getLeaderInfo() {
		logger.info("---->addressBook(dao--getLeaderInfo)");
		String sqlStr = SQLScript.getLeaderInfo;
		logger.info("---->addressBook(dao--getLeaderInfo)" + sqlStr);
		// PreparedStatementCallback ps = new PreparedStatementCallback() {
		// public Object doInPreparedStatement(PreparedStatement ps)
		// throws SQLException, DataAccessException {
		//
		// ResultSet rs = ps.executeQuery();
		// EmployeeObj user = null;
		// List<EmployeeObj> list = new ArrayList<EmployeeObj>();
		// while (rs.next()) {
		// user = buildEmpInfo(rs);
		// list.add(user);
		// }
		// return list;
		// }
		// };

		PreparedStatementCallback ps = new PreparedStatementCallback() {
			public Object doInPreparedStatement(PreparedStatement arg0)
					throws SQLException, DataAccessException {
				ResultSet rs = arg0.executeQuery();
				List list = new ArrayList();
				ResultSetMetaData md = rs.getMetaData();
				int columnCount = md.getColumnCount();
				while (rs.next()) {
					Map map = new HashMap();
					for (int i = 1; i <= columnCount; i++) {
						map.put(md.getColumnName(i), rs.getObject(i));
					}
					list.add(map);
				}
				return list;
			}
		};
		// return (List<EmployeeObj>) jt.execute(sqlStr, ps);
		return (List) jt.execute(sqlStr, ps);
	}

	public List getEmployeeInfoByQuery(StandardQuery query) {
		SQLGenerator generator = new SQLGenerator(SQLScript.getEmps);
		query.addToSQLGenerator(generator);
		String sqlStr = generator.getSQL() + " \nselect * from #result";
		logger.info("getEmployeeInfoByQuery-->" + sqlStr);
		PreparedStatementCallback ps = new PreparedStatementCallback() {
			public Object doInPreparedStatement(PreparedStatement arg0)
					throws SQLException, DataAccessException {
				ResultSet rs = arg0.executeQuery();
				List list = new ArrayList();
				ResultSetMetaData md = rs.getMetaData();
				int columnCount = md.getColumnCount();
				while (rs.next()) {
					Map map = new HashMap();
					for (int i = 1; i <= columnCount; i++) {
						map.put(md.getColumnName(i), rs.getObject(i));
					}
					list.add(map);
				}
				return list;
			}
		};
		return (List) jt.execute(sqlStr, ps);
	}

	private EmployeeObj buildEmpInfo(ResultSet rs) throws SQLException {
		EmployeeObj user;
		user = new EmployeeObj();
		user.setUserID(rs.getInt("empId"));
		user.setDepartmentID(rs.getInt("deptId"));
		user.setUserName(rs.getString("userName"));
		user.setDepartmentName(rs.getString("departmentName"));
		user.setPosition(rs.getString("position"));
		user.setMobileNo(rs.getString("mobileNo"));
		user.setSwitchboardNo(rs.getString("switchboardNo"));
		user.setExtNo(rs.getString("extNo"));
		user.setFaxNo(rs.getString("faxNo"));
		user.setEmailAddress(rs.getString("emailAddress"));
		user.setOfficeAddress(rs.getString("officeAddress"));
		return user;
	}

	public List<SimpleEmployee> getEmpsByPost(int deptId, int postId) {
		String hql = "select ep.employee from EmployeePost ep "
				+ "left outer join ep.department.parent.parent "
				+ " where (ep.department.departmentID=? or ep.department.parent.parentId=?) and ep.post.id=?";
		return ht.find(hql, new Object[] { deptId, deptId, postId });
	}

	public List<SimpleEmployee> getEmpsByRole(int roleId) {
		String hql = "select er.employee from EmployeeRole er where er.role.id=?";
		return ht.find(hql, roleId);
	}

	public List<SimpleEmployee> getEmpsByRoleAndDept(int roleId, int deptId) {
		String hql = "select er.employee from EmployeeRole er"
				+ ",EmployeePost ep"
				+ " where ep.employee.id=er.employee.id and er.role.id=? and ep.department.departmentID=?";
		return ht.find(hql, new Object[] { roleId, deptId });
	}

	public void doSaveAddressInfo(Address address) {
		ht.saveOrUpdate(address);
	}

	/**
	 * 组织树按着姓名查询某一个人
	 */
	public List getEmpListByName(String compID, String name) {
		String sql = SQLScript.getEmpListByName;
		List list = new ArrayList();
		list = jt.queryForList(sql, new Object[] { new Integer(compID), name });
		if (list != null && list.size() > 0) {
			return list;
		} else {
			return null;
		}
	}

	public Node getNodes() {
		String sql = "SELECT  r.角色ID ," + " r.角色名称 ," + " d.部门ID," + " d.部门名称,"
				+ " e.人员ID ," + "  RTRIM(e.姓氏) + RTRIM(e.名字)"
				+ " FROM    xmis.sec_角色 r ," + " xmis.sec_人员角色 er ,"
				+ " xmis.hr_人员 e ," + " xmis.hr_人员部门关系表 ed ,"
				+ " xmis.org_部门 d" + " WHERE   r.角色ID = er.角色ID"
				+ "  AND er.人员ID = e.人员ID" + " AND e.人员ID = ed.人员ID"
				+ " AND ed.部门ID = d.部门ID";
		List list = jt.query(sql, new RowMapper() {

			public Object mapRow(ResultSet rs, int arg1) throws SQLException {
				Node[] nodes = new Node[3];
				nodes[0] = new Node(rs.getString(1), rs.getString(2));
				nodes[1] = new Node(rs.getString(3), rs.getString(4));
				nodes[2] = new Node(rs.getString(5), rs.getString(6));
				return nodes;
			}
		});
		Node root = new Node();
		for (Iterator it = list.iterator(); it.hasNext();) {
			Node[] nodes = (Node[]) it.next();
			root.addChild(nodes[0]).addChild(nodes[1]).addChild(nodes[2]);
		}
		return root;
	}

	public SimpleDeptObj getSimpleDeptObj(int deptID) {
		return (SimpleDeptObj) ht.get(SimpleDeptObj.class, deptID);
	}

	// /**
	// * 根据人员ID 得到他所在的部门ID
	// */
	// public int getDeptIDByUserID(int userID) {
	// String sql = SQLScript.getDeptIDByUserID;
	// return jt.queryForInt(sql,new Object[] { new
	// Integer(userID)})!=0?jt.queryForInt(sql,new Object[] { new
	// Integer(userID)}):0;
	// }

	public List getPostListByEmpID(int userID) {
		String sql = "select " + EmployeeBelongToDept.Position + " from "
				+ EmployeeBelongToDept.TableName + " where "
				+ EmployeeBelongToDept.EmployeeID + "=?";
		return jt.queryForList(sql, new Object[] { new Integer(userID) });
	}

	public List getCustomGroupTree(final String id) {

		final String sql = SQLScript.getCustomGroupTree;

		return jt.query(sql, new Object[] { id }, new RowMapper() {

			public Object mapRow(ResultSet rs, int arg1) throws SQLException {
				Object[] objs = new Object[4];
				objs[0] = rs.getObject(1);
				objs[1] = rs.getObject(2);
				objs[2] = rs.getObject(3);
				objs[3] = rs.getObject(4);
				return objs;
			}
		});
	}

	public void deDeleteEmpOrDeptOrGroupOfCustomGroup(final String groupid,
			final String father_groupid, final String type) {
		String sql = SQLScript.deleteEmpOrDeptOrGroupOfCustomGroup;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, groupid);
				ps.setString(2, father_groupid);
				ps.setString(3, type);

			}
		});
	}

	public int getGroupBeUsedNum(final String groupid) {
		String sql = SQLScript.groupBeUsedNum;
		return jt.queryForInt(sql, new Object[] { groupid });
	}

	public void updateCustomGroupNameBatch(
			final Map<String, String> changeGroupName) {
		BatchPreparedStatementSetter setter = null;
		setter = new BatchPreparedStatementSetter() {
			public int getBatchSize() {
				return changeGroupName.size();
			}

			Iterator it = changeGroupName.entrySet().iterator();

			public void setValues(PreparedStatement ps, int index)
					throws SQLException {

				Map.Entry entry = (Map.Entry) it.next();
				String groupid = ((String) entry.getKey()).split("__")[0]
						.split("_")[((String) entry.getKey()).split("__")[0]
						.split("_").length - 1];
				ps.setString(1, (String) entry.getValue());
				ps.setString(2, groupid);
				logger.info("updateCustomGroupName" + "   sql:"
						+ SQLScript.updateCustomGroupName + "    groupname:"
						+ (String) entry.getValue() + "   groupid:" + groupid);
			}
		};

		jt.batchUpdate(SQLScript.updateCustomGroupName, setter);
	}

	public Map<String, String> doAddCustomGroupBatch(
			final Map<String, String> newGroup) {

		final Map<String, String> newGroupMap = new HashMap<String, String>();
		BatchPreparedStatementSetter setter = null;
		setter = new BatchPreparedStatementSetter() {
			public int getBatchSize() {
				return newGroup.size();
			}

			Iterator it = newGroup.entrySet().iterator();

			public void setValues(PreparedStatement ps, int index)
					throws SQLException {
				Map.Entry entry = (Map.Entry) it.next();
				String groupid = UUIDUtils.nextCode();
				String father_groupid = ((String) entry.getKey()).split("__")[1]
						.substring(((String) entry.getKey()).split("__")[1]
								.indexOf("Group") + 6);
				String type = ((String) entry.getKey()).split("_")[1];
				String groupname = (String) entry.getValue();
				ps.setString(1, groupid);
				ps.setString(2, father_groupid);
				ps.setString(3, type);
				ps.setString(4, groupname);
				logger.info("doAddCustomGroupBatch ---->" + "sql:"
						+ SQLScript.addCustomGroup + "    groupid:" + groupid
						+ "    father_groupid" + father_groupid + "     type"
						+ type + "     groupname" + groupname);

				newGroupMap.put((String) entry.getKey(), groupid);
			}
		};
		jt.batchUpdate(SQLScript.addCustomGroup, setter);
		return newGroupMap;
	}

	public void doUpdateGroupRelationMappingBatch(
			final Map<String, String> groupMapping) {
		if (groupMapping != null) {
			BatchPreparedStatementSetter setterA = null;
			setterA = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return groupMapping.size();
				}

				Iterator it = groupMapping.entrySet().iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {
					Map.Entry entry = (Map.Entry) it.next();

					String groupid = Type.getString(((String) entry.getKey())
							.trim().substring(6));
					String father_groupid = "";
					if ("root".equals(((String) entry.getValue()).trim()
							.substring(0, 4))) {
						father_groupid = Type.getString(((String) entry
								.getValue()).trim().substring(5));

					} else {
						father_groupid = Type.getString(((String) entry
								.getValue()).trim().substring(6, 38));
					}
					logger.info("doUpdateGroupRelationMappingBatch"
							+ "     grouid" + groupid + "     father_groupid"
							+ father_groupid);
					ps.setString(1, father_groupid);
					ps.setString(2, groupid);
				}
			};
			jt.batchUpdate(SQLScript.updateGroupRelation, setterA);

		}
	}

	public void dosaveDDRelationBatch(final List<TreeNode> empDDGroupList,
			final List<TreeNode> deptDDGroupList,
			final List<TreeNode> groupDDGroupList) {

		logger.info("dosaveDDRelationBatch---->");
		BatchPreparedStatementSetter setter = null;
		if (empDDGroupList != null && empDDGroupList.size() > 0) {
			setter = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return empDDGroupList.size();
				}

				Iterator it = empDDGroupList.iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {
					TreeNode node = (TreeNode) it.next();
					ps.setString(1, node.getId());
					ps.setString(2, node.getAttributes().get("father_groupid"));
					ps.setString(3, node.getAttributes().get("type"));
					ps.setString(4, node.getText());
					logger.info("dosaveDDRelationBatch  ---> id:"
							+ node.getId() + "          father_groupid:"
							+ node.getAttributes().get("father_groupid")
							+ "         type:"
							+ node.getAttributes().get("type")
							+ "        text:" + node.getText());
				}
			};
			jt.batchUpdate(SQLScript.addCustomGroup, setter);
		}

		if (deptDDGroupList != null && deptDDGroupList.size() > 0) {
			setter = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return deptDDGroupList.size();
				}

				Iterator it = deptDDGroupList.iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {
					TreeNode node = (TreeNode) it.next();
					ps.setString(1, node.getId());
					ps.setString(2, node.getAttributes().get("father_groupid"));
					ps.setString(3, node.getAttributes().get("type"));
					ps.setString(4, node.getText());
					logger.info("dosaveDDRelationBatch  ---> id:"
							+ node.getId() + "          father_groupid:"
							+ node.getAttributes().get("father_groupid")
							+ "         type:"
							+ node.getAttributes().get("type")
							+ "        text:" + node.getText());
				}
			};
			jt.batchUpdate(SQLScript.addCustomGroup, setter);
		}

		if (groupDDGroupList != null && groupDDGroupList.size() > 0) {
			setter = new BatchPreparedStatementSetter() {
				public int getBatchSize() {
					return groupDDGroupList.size();

				}

				Iterator it = groupDDGroupList.iterator();

				public void setValues(PreparedStatement ps, int index)
						throws SQLException {
					TreeNode node = (TreeNode) it.next();
					ps.setString(1, node.getId());
					ps.setString(2, node.getAttributes().get("father_groupid"));
					ps.setString(3, node.getAttributes().get("type"));
					ps.setString(4, node.getText());
					logger.info("dosaveDDRelationBatch  ---> id:"
							+ node.getId() + "          father_groupid:"
							+ node.getAttributes().get("father_groupid")
							+ "         type:"
							+ node.getAttributes().get("type")
							+ "        text:" + node.getText());
				}
			};
			jt.batchUpdate(SQLScript.addCustomGroup, setter);
		}
	}

	public Map getEmpInfo(int empID) {
		return jt.queryForMap(SQLScript.getEmpInfo, new Object[] { new Integer(
				empID) });
	}

	public List getDeptListByFatherDeptID(int deptID) {
		String sql = SQLScript.getDeptListByFatherDeptID;

		return jt.queryForList(sql, new Object[] { new Integer(deptID) });

	}

	public List getCompInfoByEmpName(String empName, PageInfo pageInfo) {
		String sql = SQLScript.getCompInfoByEmpName;
		String sqlStr = WebUtil.generateSQLStr(sql, pageInfo);
		logger.info("OrganizationDAOImpl getCompIDByEmpName   empName:"
				+ empName + "  sql:" + sqlStr);
		empName = "%" + empName + "%";
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {} : new Object[] {
				new String(empName), new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);
	}

	public List getCustomGroupList(String father_groupid) {
		String sql = SQLScript.getCustomGroupList;
		return jt
				.queryForList(sql, new Object[] { new String(father_groupid) });
	}

	public List getCustomGroupListByGroupid(String groupid) {
		String sql = SQLScript.getCustomGroupListByGroupid;
		return jt.queryForList(sql, new Object[] { new String(groupid) });
	}

	public boolean getFdeptIDIsExistByDeptID(final int deptId) {
		String sql = SQLScript.getFdeptIDIsExistByDeptID;
		Object o = jt.execute(sql, new PreparedStatementCallback() {
			
			public Object doInPreparedStatement(PreparedStatement ps)
					throws SQLException, DataAccessException {
				Boolean isExist = false;
				int num = 0;
				ps.setInt(1, deptId);
				ResultSet rs = ps.executeQuery();
				if(rs.next()){
					num = rs.getInt(1);
					if(num>0){
						isExist =  true;
					}else {
						isExist =  false;
					}
				}
				return isExist;
			}
		});
		return  (Boolean) o;
	
	}
	/**
	 * 根据部门ID 得到父部门ID
	 * 
	 * @param deptID
	 * @return
	 */
	public int getFdeptIDByDeptID(int deptID) {
		String sql = SQLScript.getFdeptIDByDeptID;
		int result = 0;
		try {
			result = jt.queryForInt(sql, new Object[] { deptID });
		} catch (Exception e) {
			logger.info("根据部门ID 得到父部门ID//getFdeptIDByDeptID--", e);
		}
		return result;
	}

	/**
	 * 根据部门ID 得到公司ID
	 * 
	 * @param fdeptID
	 * @return
	 */
	public int getCompIDByDeptID(int fdeptID) {
		String sql = SQLScript.getCompIDByDeptID;
		return jt.queryForInt(sql, new Object[] { new Integer(fdeptID) });
	}

	/**
	 * 根据部门ID 得到他所在的部门ID（两级以内）
	 */
	public int getDeptIDByUserID(int userID) {
		String sql = SQLScript.getDeptIDByUserID;
		return jt.queryForInt(sql, new Object[] { new Integer(userID) }) != 0 ? jt
				.queryForInt(sql, new Object[] { new Integer(userID) }) : 0;
	}

	public int isExsitCustomGroup(String groupid, String father_groupid,
			String type) {
		String sql = SQLScript.isExistCustomGroup;
		return jt.queryForInt(sql, new Object[] { new String(groupid),
				new String(father_groupid), new String(type) });
	}

	public List getCompIDByEmpName(String empName) {
		String sql = SQLScript.getCompIDByEmpName;
		return jt.queryForList(sql, new Object[] { new String(empName) });
	}

	/**
	 * 根据部门ID 得到事业部门ID
	 */
	public int getDivisionDeptIDByDeptID(int deptID) {
		String sql = SQLScript.getDivisionDeptIDByDeptID;
		return jt.queryForInt(sql, new Object[] { new Integer(deptID),
				new Integer(deptID) });
	}

	/**
	 * 组织树按着姓名查询某一个人 （包括被禁用的人）
	 */
	public List getEmpListByNameAdmin(String compID, String name) {
		String sql = SQLScript.getEmpListByNameAdmin;
		List list = new ArrayList();
		list = jt.queryForList(sql, new Object[] { new Integer(compID), name });
		if (list != null && list.size() > 0) {
			return list;
		} else {
			return null;
		}
	}

	/**
	 * 根据人员id得到 此人的路径 人ID-部门ID-上级部门ID-...-上级部门ID-部门ID-公司ID
	 * 
	 * @param userid
	 * @return
	 */
	public String getEmpPathId(int userid) {
		String sql = "DECLARE @deptid INT "
				+ " DECLARE @divsionid INT "
				+ " DECLARE @compid INT "
				+ " SELECT @deptid=部门ID FROM xmis.hr_人员部门关系表 WHERE 人员ID="
				+ userid
				+ " SELECT @divsionid=xmis.f_getDivisionID(@deptid) "
				+ " SELECT @compid=公司ID FROM xmis.org_部门归属公司表 WHERE 部门ID= @divsionid "
				+ " SELECT CONVERT(VARCHAR(100),"
				+ userid
				+ ")+','+xmis.f_getAFDeptPathByDeptId(@deptid)+CONVERT(VARCHAR(100),@compid) AS emppathid ";
		List list = jt.queryForList(sql);

		if (list != null && list.size() > 0) {
			String emppathid = ((Map) list.get(0)).get("emppathid").toString();
			return emppathid;
		} else {
			return "";
		}
	}

	public String getDeptNameByDeptId(int deptId) {
		String sql = "select 部门名称 from org_部门 where 部门ID = ?";
		List list = jt.queryForList(sql, new Object[] { deptId });
		if (list != null && list.size() > 0) {
			String name = ((Map) list.get(0)).get("部门名称").toString();
			return name;
		} else {
			return "";
		}
	}

	/**
	 * 获取最大ID
	 * 
	 * @param columnName
	 * @param tableName
	 * @return
	 */
	public int getMaxId(String sql) {
		int maxId = jt.queryForInt(sql) + 1;
		return maxId;
	}

	public DisabledReason getDisabledReason(String id) {
		return (DisabledReason) ht.get(DisabledReason.class, id);
	}

	public List getDisabledReasonComb() {
		return ht.find("from DisabledReason");
	}

	public void updateDisableReason(int empId, String reason, String disable) {
		String sql = "update hr_人员 set 禁用原因=?,禁用=?,锁定=? ,禁用时间=GETDATE() where 人员ID=?";
		jt.update(
				sql,
				new Object[] { new String(reason),
						new Integer(Type.GetInt(disable)), new Integer(Type.GetInt(disable)),new Integer(empId) });

	}

	public int doSavePostCardFrontPic(int userID, String postCardFrontName,
			int updateID) {

		logger.info("===> doSavePostCardFrontPic /" + String.valueOf(userID)
				+ "/" + postCardFrontName);

		logger.info(SQLScript.savePostCardFontName);
		return jt
				.update(SQLScript.savePostCardFontName, new Object[] {
						postCardFrontName, new Integer(updateID),
						new Integer(userID) });
	}

	public int doSavePostCardBackPic(int userID, String postCardBackName,
			int updateID) {

		logger.info("===> doSavePostCardBackPic /" + String.valueOf(userID)
				+ "/" + postCardBackName);

		logger.info(SQLScript.savePostCardBackName);
		return jt.update(SQLScript.savePostCardBackName, new Object[] {
				postCardBackName, new Integer(updateID), new Integer(userID) });
	}

	public int doUpdatePostCard(int empId, Boolean isFront) {
		String sql = "update xmis.hr_人员 set 名片背面名字=null where 人员ID=?";
		if (isFront) {
			sql = "update xmis.hr_人员 set 名片正面名字=null where 人员ID=?";
		}
		return jt.update(sql, new Object[] { empId });

	}

	public DepartmentObj getDepartmentById(int departmentID) {
		return (DepartmentObj) ht.get(DepartmentObj.class, departmentID);
	}

	public void saveUserTopicMessageHabit(final String tipID, final int userID,
			final String messageID) {
		jt.update(SQLScript.saveUserTopicMessageHabit,
				new PreparedStatementSetter() {
					public void setValues(PreparedStatement ps)
							throws SQLException {
						ps.setString(1, messageID);
						ps.setInt(2, userID);
					}
				});
	}

	public List getMessageByUserIDAndTopID(String topicID, int userID) {
		String sql = SQLScript.getMessageByUserIDAndTopicID;
		List list = jt.queryForList(sql, new Object[] { new Integer(userID),
				topicID });
		return list;
	}

	public int doInsertMessageDefinition(final String message_id,
			final int userid, final String text, final String topic_type) {
		String sql = SQLScript.insertMessageDefinition;
		int i = jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, message_id);
				ps.setString(2, topic_type);
				ps.setString(3, text);
				ps.setInt(4, userid);
			}
		});
		return i;
	}

	public int doInsertMessageRecord(final String message_id) {
		String sql = SQLScript.insertMessageRecord;
		int i = jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, message_id);
			}
		});
		return i;
	}

	public void doInsertPersonMessageDefinition(final String message_id,
			final int userid, final String text, final String topic_type) {
		String sql = SQLScript.insertPersonMessageDefinition;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, message_id);
				ps.setString(2, topic_type);
				ps.setString(3, text);
				ps.setInt(4, userid);
			}
		});
	}

	public void savePersonalMessageRecord(final String message_id,
			final int empid) {
		String sql = SQLScript.insertPersonMessageRecord;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, message_id);
				ps.setInt(2, empid);
			}
		});
	}

	public Boolean getIsExistByDeptId(final int deptId) {
		String sql = SQLScript.getIsDirectByDeptId;
		Object o = jt.execute(sql, new PreparedStatementCallback() {
			
			public Object doInPreparedStatement(PreparedStatement ps)
					throws SQLException, DataAccessException {
				Boolean isExist = false;
				int num = 0;
				ps.setInt(1, deptId);
				ResultSet rs = ps.executeQuery();
				if(rs.next()){
					num = rs.getInt(1);
					if(num>0){
						isExist =  true;
					}else {
						isExist =  false;
					}
				}
				return isExist;
			}
		});
		return  (Boolean) o;
	}
	public Boolean getIsDirectByDeptId(final int deptId) {
		Boolean isDirect = true;
		String sql = SQLScript.getIsDirectByDeptId;
		int a = jt.queryForInt(sql, new Object[]{deptId});
		if(a<=0){
			isDirect = false;
		}
		return  isDirect;
	}

	// public List getHaveReceivedMessageByUseId(int userid){
	// String sql = SQLScript.gethaveReceivedMessageByUseId;
	// List list = jt.queryForList(sql,new Object[]{new Integer(userid)});
	// return list;
	// }
	public List getZentaoDataFromEmployee(){
		String sql= SQLScript.getZentaoDataFromEmployee;
		
		return jt.queryForList(sql);
	}

	public List getPersonByDeptID(int deptId) {
		String sqlStr = SQLScript.getPersonByDeptID;
		return jt.queryForList(sqlStr, new Object[]{deptId});
	}
	
	public List getEmpsByRoleAndDivisionID(int roleId, int divisionId) {
		String sql = SQLScript.getEmpsByRoleAndDivisionID;
		return jt.queryForList(sql, new Object[] { roleId, divisionId });
	}

	public Boolean getIsExistRole(int roleId,int empId) {
		String sql = SQLScript.getIsExistRole;
		int count= jt.queryForInt(sql, new Object[]{roleId,empId});
		boolean isExistRole = false;
		if(count>0){
			isExistRole=true;
		}
		return isExistRole;
	}

	public List getAllDivisionDept(int companyID) {
		String sql = SQLScript.getAllDivisionDept;
		return jt.queryForList(sql,new Object[]{companyID});
	}

	public boolean isHasExistAttendanceWarnInfo(String msg, String nowdate) {
		String sql = SQLScript.isHasExistAttendanceWarnInfo;
		int count = jt.queryForInt(sql, new Object[]{new String(msg),new String(nowdate)});
		if(count>0){
			return true;
		}
		return false;
	}

	public List getSendMessageAfterProcessInstance(String processInstanceId) {
		String sql = SQLScript.getSendMessageAfterProcessInstance;
		return jt.queryForList(sql,new Object[]{processInstanceId});
	}

	public int doSavePostMaterials(String materialsID,String imgName) {
		System.out.println("doSavePostMaterials+++++++++++++"+SQLScript.savePostMaterials);
		System.out.println("materialsID============="+materialsID);
		System.out.println("imgName++++++++++++++"+imgName);
		return jt.update(SQLScript.savePostMaterials, new Object[] {
				imgName,materialsID });
	}

	
}
