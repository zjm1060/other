package com.bitc.jk.xmis.dao.impl;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Types;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.hibernate.HibernateException;
import org.hibernate.Session;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementSetter;
import org.springframework.orm.hibernate3.HibernateCallback;

import com.bitc.jk.xmis.dao.CrmDAO;
import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PatentInformation;
import com.bitc.jk.xmis.model.ProjectQualification;
import com.bitc.jk.xmis.model.QualificationObj;
import com.bitc.jk.xmis.model.SupplierInformation;
import com.bitc.jk.xmis.model.crm.Address;
import com.bitc.jk.xmis.model.crm.Area;
import com.bitc.jk.xmis.model.crm.BusinessRelation;
import com.bitc.jk.xmis.model.crm.City;
import com.bitc.jk.xmis.model.crm.CompanyScale;
import com.bitc.jk.xmis.model.crm.Country;
import com.bitc.jk.xmis.model.crm.CreditLevel;
import com.bitc.jk.xmis.model.crm.CustomerLevel;
import com.bitc.jk.xmis.model.crm.Internation;
import com.bitc.jk.xmis.model.crm.Province;
import com.bitc.jk.xmis.model.crm.RelativeAddress;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class CrmDAOImpl extends BaseDAOImpl implements CrmDAO {
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
				DBSchema.Company.CompanyID, DBSchema.Company.TableName)) + 1;

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
						DBSchema.Department.TableName)) + 1;

		jt.update(SQLScript.createNewDept, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, maxID);
				ps.setString(2, comp.getDeptName());
				ps.setString(3, comp.getDeptAbb());
			}

		});

		return maxID;
	}

	public int getDefaultDeptByCompany(final int companyID) {

		logger.info("ppppp" + SQLScript.getDefaultDeptByCompany);

		return jt.queryForInt(SQLScript.getDefaultDeptByCompany,
				new Object[] { new Integer(companyID) });

	}

	public int doCreateNewContactPersonSimple(final CompanySimpleObj comp,
			final EmployeeObj userDNA) {
		// TODO Auto-generated method stub

		logger.info("===> doCreateUser " + SQLScript.createNewUser
				+ SQLScript.setEmpBelongToDept + "//" + comp.getDeptID() + "//"
				+ comp.getEmpName());

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.Employee.ID, DBSchema.Employee.TableName)) + 1;

		jt.update(SQLScript.createNewUser + SQLScript.setEmpBelongToDept,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {

						// create new crm
						ps.setInt(1, maxID);
						ps.setString(2, comp.getEmpName().substring(0, 1));
						ps.setString(3, comp.getEmpName().substring(1).trim());
						ps.setString(4, WebUtil.getRandomString(4));
						ps.setInt(5, userDNA.getUserID());

						// set user belong to dept
						int ID = jt.queryForInt(SQLScript.getMaxIDFromTab(
								DBSchema.EmployeeBelongToDept.ID,
								DBSchema.EmployeeBelongToDept.TableName)) + 1;
						ps.setInt(6, ID);
						ps.setInt(7, maxID);
						ps.setInt(8, comp.getDeptID());

					}
				});

		return maxID;

		// final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
		// DBSchema.ContactPerson.ContactPersonID,
		// DBSchema.ContactPerson.TableName))+1;
		//
		// jt.update(SQLScript.createNewSimpleCRM, new PreparedStatementSetter()
		// {
		// public void setValues(PreparedStatement ps) throws SQLException {
		//
		// ps.setInt(1, maxID);
		// ps.setString(2, comp.getEmpName());
		//
		// ps.setInt(3, comp.getCompanyID());
		//
		// if (comp.getDeptID() > 0)
		// ps.setInt(4, comp.getDeptID());
		// else
		// ps.setNull(4, Types.INTEGER);
		//
		// ps.setInt(5, userDNA.getUserID());
		//
		// }
		//
		// });
		//
		// return maxID;

	}

	public int doCreateNewContactLinkPersonSimple(final CompanySimpleObj comp,
			final EmployeeObj userDNA) {
		// TODO Auto-generated method stub

		logger.info("===> doCreateUser " + SQLScript.createNewUser
				+ SQLScript.setEmpBelongToDept + "//" + comp.getDeptID() + "//"
				+ comp.getEmpName());

		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.Employee.ID, DBSchema.Employee.TableName)) + 1;

		String sql = "INSERT INTO xmis.hr_人员(人员ID,姓氏,名字,登录名,密码,性别ID,家庭地址,单位电话,家庭邮编,职务,修改人ID,修改时间,禁用,移动电话,电子邮件) VALUES (?,?,?,?,'000000',1,?,?,?,?,?,getdate(),0,?,?)";

		jt.update(sql + SQLScript.setEmpBelongToDept,
				new PreparedStatementSetter() {

					public void setValues(PreparedStatement ps)
							throws SQLException {

						// create new crm
						ps.setInt(1, maxID);
						ps.setString(2, comp.getEmpName().substring(0, 1));
						ps.setString(3, comp.getEmpName().substring(1).trim());
						ps.setString(4, WebUtil.getRandomString(4));

						ps.setString(5, comp.getEmpAddr());
						ps.setString(6, comp.getEmpPhone());
						ps.setInt(7, Type.GetInt(comp.getEmpPostCode()));
						ps.setString(8, comp.getEmpPost());

						ps.setInt(9, userDNA.getUserID());

						ps.setString(10, comp.getEmpMobilePhone());
						ps.setString(11, comp.getEmpEmail());

						// set user belong to dept
						int ID = jt.queryForInt(SQLScript.getMaxIDFromTab(
								DBSchema.EmployeeBelongToDept.ID,
								DBSchema.EmployeeBelongToDept.TableName)) + 1;
						ps.setInt(12, ID);
						ps.setInt(13, maxID);
						ps.setInt(14, comp.getDeptID());

					}
				});

		return maxID;
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

	public int empValidate(final String empName, final int compID,
			final int deptID) {
		logger.info("===>  empValidate //" + empName
				+ SQLScript.getUserIDByCompany);
		return jt.queryForInt(SQLScript.getUserIDByCompany, new Object[] {
				new Integer(compID), new Integer(deptID), empName });
	}

	/*
	 * public List getEmpInfoById(int empID) {
	 * logger.info("===>  getEmpInfoById "); String sql =
	 * SQLScript.getEmpInfoById; return jt.queryForList(sql, new Object[] {
	 * empID }); }
	 */

	public void updateEmpInfo(final EmployeeObj target,
			final EmployeeObj userDNA) {
		logger.info("===>  updateEmpInfo ");
		String sql = SQLScript.updateEmpInfo;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				int nameLength = target.getUserName().length();
				String name = nameLength > 1 ? target.getUserName()
						.substring(1) : "";
				String xs = target.getUserName().substring(0, 1);
				ps.setString(1, name);
				ps.setString(2, xs);
				ps.setString(3, target.getHomeAddress());
				ps.setString(4, target.getPosition());
				ps.setString(5, target.getMobileNo());
				ps.setString(6, target.getFaxNo());
				ps.setString(7, target.getEmailAddress());
				ps.setString(8, target.getZipCode());
				ps.setInt(9, userDNA.getUserID());
				ps.setInt(10, target.getUserID());
			}

		});
	}

	/*
	 * 公司规模的列表
	 */
	@SuppressWarnings("unchecked")
	public List<CompanyScale> getCmpScaleCombox() {
		String hql = "from CompanyScale order by companyScaleID";
		return ht.find(hql);
	}

	/*
	 * 客户级别的列表
	 */
	@SuppressWarnings("unchecked")
	public List<CustomerLevel> getCustomerCombox() {
		String hql = "from CustomerLevel";
		return ht.find(hql);
	}

	/*
	 * 信用等级的列表
	 */
	@SuppressWarnings("unchecked")
	public List<CreditLevel> getCreditLevelCombox() {
		String hql = "from CreditLevel order by creditLevelID";
		return ht.find(hql);
	}

	/*
	 * 业务关系的列表
	 */
	@SuppressWarnings("unchecked")
	public List<BusinessRelation> getBusinessRelationCombox() {
		String hql = "from BusinessRelation order by businessRelationID";
		return ht.find(hql);
	}

	/*
	 * 洲际列表
	 */
	@SuppressWarnings("unchecked")
	public List<Internation> getInternationData() {
		String hql = "from Internation ";
		return ht.find(hql);
	}

	/*
	 * 国家列表
	 */
	@SuppressWarnings("unchecked")
	public List<Country> getCountryData(int fid) {
		String hql = "from Country c where c.internation.internationID = ?";
		return ht.find(hql, fid);
	}

	/*
	 * 地区列表
	 */
	@SuppressWarnings("unchecked")
	public List<Area> getAreaData(int fid) {
		String hql = "from Area a where  a.country.countryID = ?";
		return ht.find(hql, fid);
	}

	// 得到省的信息列表
	@SuppressWarnings("unchecked")
	public List<Province> getProvinceData(int fid) {
		String hql = "from Province p where p.area.areaID = ?";
		return ht.find(hql, fid);
	}

	// 得到城市的信息列表
	@SuppressWarnings("unchecked")
	public List<City> getCityData(int fid) {
		String hql = "from City c where  c.province.provinceID = ?";
		return ht.find(hql, fid);
	}

	/*
	 * 根据公司ID查询公司信息
	 */
	public CompanyObj getCompanyObjById(final int companyID) {
		return (CompanyObj) ht.load(CompanyObj.class, companyID);
	}

	/**
	 * 新建保存公司信息
	 */
	public void doSaveCompanyInfo(CompanyObj cmpObj) {
		if (cmpObj.getCompanyID() == 0) {
			final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
					DBSchema.Company.CompanyID, DBSchema.Company.TableName)) + 1;
			cmpObj.setCompanyID(maxID);
		}
		// saveOrUpdateObject(cmpObj);
		ht.saveOrUpdate(cmpObj);
	}

	/**
	 * 得到公司信息列表
	 */
	public List<CompanyObj> getCompanyList(StandardQuery params,
			PageInfo pageInfo) {
		return getListStore(params, pageInfo, CompanyObj.class);
	}
	
	/**
	 * 得到公司信息列表
	 */
	@SuppressWarnings({ "unchecked", "unused" })
	public List getCompanyInfoList(String sqlStr,
			PageInfo pageInfo) {
		SQLGenerator generator = new SQLGenerator(sqlStr);
		String sqlStrs = WebUtil.generateSQLStr(generator.getSQL(), pageInfo,SQLScript.sql_SearchCompanyWithPageTotal);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] { }
		: new Object[] {
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1
								+ pageInfo.getLimit()) };
				logger.info("---> getContractListByQuerys " + sqlStrs);
				return jt.queryForList(sqlStrs,args);
	}

	public List<RelativeAddress> getRelativeAddressListM(StandardQuery params,
			PageInfo pageInfo) {
		return getListStore(params, pageInfo, RelativeAddress.class);
	}

	public int getCompanyListCount(final StandardQuery params, PageInfo pageInfo) {
		return getTotalCount(params, CompanyObj.class);
	}

	/*
	 * 根据ID删除某条公司信息 暂不用
	 */
	public void dodelCompanyInfoById(final int id) {
		ht.execute(new HibernateCallback() {
			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				session.delete(session.get(CompanyObj.class, id));
				return null;
			}

		});
	}

	public void doSaveRelativeAddress(RelativeAddress relativeAddress) {
		if ("".equals(relativeAddress.getId())) {
			ht.save(relativeAddress);
		} else {
			ht.saveOrUpdate(relativeAddress);
		}
	}

	public RelativeAddress getRelativeAddress(String id) {
		return get(RelativeAddress.class, id);
	}

	public void dodelRelativeAddressById(final String id) {
		ht.execute(new HibernateCallback() {
			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				session.delete(session.get(RelativeAddress.class, id));
				return null;
			}
		});
	}

	public EmployeeObj getEmpInfoById(int empID) {
		return (EmployeeObj) ht.get(EmployeeObj.class, empID);
	}

	public List<Address> getAddressListQuery(StandardQuery query) {
		return getListStore(query, Address.class);
	}

	public Address getAddress(String id) {
		return (Address) ht.get(Address.class, id);
	}

	public List getAddressIDListByName(String name) {
		String sql = "select " +
				" a.洲际id as aid,b.国家id as bid,c.地区id as cid,d.省id as did,e.城市id as eid " +
				" from xmis.add_洲际表 a, " +
				" xmis.add_国家及地区 b, " +
				" xmis.add_地区 c, " +
				" xmis.add_省 d, " +
				" xmis.add_城市 e " +
				" where a.洲际id = b.洲际id " +
				" and b.国家id = c.国家id  " +
				" and c.地区id = d.地区id " +
				" and d.省id=e.省id " +
				" and (" +
				"  a.名称 like '%" + name + "%'" +
				" or b.名称 like '%" + name + "%'" +
				" or c.名称 like '%" + name + "%'" +
				" or d.省名称 like '%" + name + "%'" +
				" or e.城市名称 like '%" + name + "%')";
		List list = jt.queryForList(sql);
		if (list != null && list.size() > 0) {
			return list;
		} else {
			return null;
		}
	}

	public String getInputContactMoney(int companyID) {
		String sql = "select sum(a.合同总额*c.对人民币汇率) AS inputContactMoney "
				+ " from xmis.con_合同 a,  xmis.org_公司 b,xmis.sys_货币 c "
				+ " where a.甲方公司ID=b.公司ID " + " and a.货币ID = C.货币ID "
				+ " and  a.合同类型ID=1 " + " and a.甲方公司ID= " + companyID
				+ " group by b.公司ID ";
		List list = jt.queryForList(sql);
		if (list != null && list.size() > 0) {
			return ((Map) list.get(0)).get("inputContactMoney").toString();
		} else {
			return null;
		}
	}

	public String getOutputContactMoney(int companyID) {
		String sql = "select sum(a.合同总额*c.对人民币汇率) as outputContactMoney "
				+ " from xmis.con_合同 a,  xmis.org_公司 b,xmis.sys_货币 c "
				+ " where a.乙方公司ID=b.公司ID " + " and a.货币ID = C.货币ID "
				+ " and  a.合同类型ID=2 AND a.父合同ID is null " + " and a.乙方公司ID= "
				+ companyID + " group by b.公司ID ";
		List list = jt.queryForList(sql);
		if (list != null && list.size() > 0) {
			return ((Map) list.get(0)).get("outputContactMoney").toString();
		} else {
			return null;
		}
	}

	
	/**公司地址及相关地址的下拉框
	 * @param companyID
	 * @return
	 */
	@SuppressWarnings("rawtypes")
	public List getRelationAddressCombo(String companyID) {
		String sql = SQLScript.getRelationAddressCombo;
		return jt.queryForList(sql, new Object[]{companyID,companyID});
	}

	public List getRelationAddress(String companyID) {
		// TODO Auto-generated method stub
		String sql = "select org.公司ID,org2.公司名称,(add3.省名称+add2.城市名称+add1.区县+add1.详细地址) as officeAddressName,org.地址ID as officeAddressID from " +
						" xmis.org_公司相关地址表 org ,"+
						"xmis.add_地址 add1 ,"+
						"xmis.add_城市 add2 ," +
						"xmis.add_省 add3  ," +
						"xmis.org_公司 org2 "+
					"where "+
						"org.地址ID = add1.地址ID " +
						"and org.公司ID = org2.公司ID "+
						"AND add1.城市ID = add2.城市ID " +
						"AND add2.省ID = add3.省ID "+
						"AND org.公司ID = ? " +
						"UNION " +
						"select org.公司ID,org.公司名称,(add3.省名称+add2.城市名称+add1.区县+add1.详细地址) as 公司地址,org.地址ID from " +
						"xmis.org_公司 org , " +
						"xmis.add_地址 add1 , " +
						"xmis.add_城市 add2, " +
						"xmis.add_省 add3   " +
						"where " +
						"org.地址ID = add1.地址ID " +
						"AND add1.城市ID = add2.城市ID " +
						"AND add2.省ID = add3.省ID " +
						"AND org.公司ID = ? ";
		return jt.queryForList(sql, new Object[]{companyID,companyID});
	}

	public void doSupplierInformation(SupplierInformation si) {
		// TODO Auto-generated method stub
		saveOrUpdateObject(si);
	}

	public List getSupplierInformation(int companyID) {
		String sql ="SELECT (add5.区号+'-'+addc.区号+'-'+hr.单位电话) as DH," +
					"(add5.区号+'-'+addc.区号+'-'+hr.传真号码) as CZ," +
					"hr.电子邮件 AS Email," +
					"hr.人员ID AS ryid ," +
					"Replace((hr.姓氏 + hr.名字),' ','') as 经理姓名 " +
					" INTO #tmp  " +
					"FROM xmis.hr_人员 hr " +
					"LEFT JOIN xmis.add_地址 addd " +
					"ON hr.办公地址ID = addd.地址ID   "+
					"LEFT JOIN xmis.add_城市 addc " +
					"ON addd.城市ID = addc.城市ID " +
					"LEFT JOIN xmis.add_省 add3 " +
					"ON addc.省ID = add3.省ID " +
					"LEFT JOIN xmis.add_地区 add4 " +
					" ON add3.地区ID = add4.地区ID " +
					" LEFT JOIN xmis.add_国家及地区 add5 " +
					" ON add4.国家ID = add5.国家ID " +
				"select org.公司ID,org.公司名称,(add3.省名称+add2.城市名称+add1.区县+add1.详细地址) as 公司地址,org.地址ID " +
							"into #test " +
							"from " +
							"xmis.org_公司 org ," +
							"xmis.add_地址 add1 ," +
							"xmis.add_城市 add2," +
							"xmis.add_省 add3  " +
							"where " +
							"org.地址ID = add1.地址ID " +
							"AND add1.城市ID = add2.城市ID " +
							"AND add2.省ID = add3.省ID  " +
							"AND org.公司ID = ? " +
							" UNION " +
							"select org.公司ID,org2.公司名称,(add3.省名称+add2.城市名称+add1.区县+add1.详细地址) as 公司地址,org.地址ID from " +
							"xmis.org_公司相关地址表 org , " +
							"xmis.add_地址 add1 , " +
							"xmis.add_城市 add2, " +
							"xmis.add_省 add3," +
							"xmis.org_公司 org2 " +
							"where  " +
							"org.地址ID = add1.地址ID " +
							"and org.公司ID = org2.公司ID " +
							"AND add1.城市ID = add2.城市ID " +
							"AND add2.省ID = add3.省ID  " +
							"AND org.公司ID = ? " +
							"" +
							"select " +
							"orgt.公司ID as ID ," +
							"orgt.经理ID as managerID ," +
							" t2.经理姓名 AS 经理姓名, " +
							"orgt.地址ID as addressID ," +
							"orgt.主管单位 as headOfUnit ," +
							"orgt.行业评定情况 as industryAssessment ," +
							"orgt.采用标准 as standard ," +
							"orgt.资质情况 as qualification," +
							"orgt.是否为一般纳税人 as generalTaxpayer," +
							"orgt.主导产品及产量 as productsProduction," +
							"orgt.为中水科配套产品 as ancillaryProducts," +
							"orgt.质量保证体系情况 as qualityAssurance," +
							"orgt.是否引进线 as introductionLine," +
							"orgt.成品率 as Yield," +
							"orgt.登记人ID as newregisterID," +
							"orgt.登记日期 as newupdateDate," +
							"orgt.最后修改人ID as lastModifier," +
							"orgt.最后修改日期 as lastModified," +
							"(hr1.姓氏+hr1.名字) as newregisterName," +
							"(hr2.姓氏+hr2.名字) as lastModifierName," +
							"t1.公司地址 as 公司地址 ," +
							" t2.DH AS DH," +
							" t2.CZ AS CZ," +
							" t2.Email AS Email " +

						 "from  " +
					"	xmis.org_供方评定调查表 orgt," +
					"	#test t1, " +
					"	 xmis.hr_人员 hr1 ," +
					"	 xmis.hr_人员 hr2 ," +
					"	#tmp t2 " +
					
				"	where orgt.地址ID = t1.地址ID " + 
				"		AND orgt.登记人ID = hr1.人员ID " +
				"		AND orgt.最后修改人ID = hr2.人员ID " +
				"       AND orgt.经理ID = t2.ryid  " + 
				"		AND orgt.公司ID = ?";
		return jt.queryForList(sql, new Object[] { companyID,companyID,companyID });
	}

	public List getQualificationDictionary() {
		// TODO Auto-generated method stub
		String sql = "SELECT 资质ID AS qualificationID,资质名称 AS qualificationName FROM xmis.sys_资质字典表 ";
		return jt.queryForList(sql, new Object[]{});
	}

	public void doQualificationInformation(QualificationObj qfo) {
		// TODO Auto-generated method stub
		saveOrUpdateObject(qfo);
	}

	public List getQualificationList(String companyID) {
		// TODO Auto-generated method stub
		String sql = "select ID as id, 资质名称 as qualificationName ,CONVERT(varchar, 证书有效期起, 120 )  as certificateValidQi,CONVERT(varchar, 证书有效期止, 120 ) as certificateValidZhi from xmis.org_资质管理表 WHERE 公司ID = ?";
		return jt.queryForList(sql, new Object[]{companyID});
	}

	public List getQualificationAssignment(String id) {
		// TODO Auto-generated method stub
		String sql ="SELECT (add5.区号+'-'+addc.区号+'-'+hr.单位电话) as DH," +
				"(add5.区号+'-'+addc.区号+'-'+hr.传真号码) as CZ," +
				"hr.电子邮件 AS Email," +
				"hr.人员ID AS ryid ," +
				"Replace((hr.姓氏 + hr.名字),' ','') as JLM, " +
				"hr.移动电话 AS YD," +
				"hr.家庭邮编 AS YB " +
				" INTO #tmp  " +
				"FROM xmis.hr_人员 hr " +
				"LEFT JOIN xmis.add_地址 addd " +
				"ON hr.办公地址ID = addd.地址ID   "+
				"LEFT JOIN xmis.add_城市 addc " +
				"ON addd.城市ID = addc.城市ID " +
				"LEFT JOIN xmis.add_省 add3 " +
				"ON addc.省ID = add3.省ID " +
				"LEFT JOIN xmis.add_地区 add4 " +
				" ON add3.地区ID = add4.地区ID " +
				" LEFT JOIN xmis.add_国家及地区 add5 " +
				" ON add4.国家ID = add5.国家ID " +
				" SELECT count (资质名称对应ID) AS TS INTO #test  FROM xmis.org_资质使用部门表 WHERE 资质名称对应ID = ? " +
				" SELECT " +
					" ID AS id,org.公司ID AS companyID,org.联系人ID AS qualificationContactID," +
					" tp.JLM AS qualificationContact, " +
					" CONVERT(varchar,org.证书有效期起, 120 ) AS certificateValidQi,CONVERT(varchar,org.证书有效期止 , 120 )AS certificateValidZhi," +
					" org.证书号 AS certificateNo,org.备注 AS qualificationRemark," +
					" org.是否年审 AS examined,org.资质名称 AS qualificationName,org.管理部门 AS departmentName," +
					" Replace((hr1.姓氏+hr1.名字),' ','') AS newregisterName," +
					" Replace((hr2.姓氏+hr2.名字),' ','') AS lastModifierName," +
					" CONVERT(varchar,org.最后修改日期, 120 ) AS  lastModified,CONVERT(varchar,org.登记日期 , 120 )AS newupdateDate," +
					"  tp.DH AS DH,tp.CZ AS CZ,tp.Email AS Email ,tp.YD  AS dianHua,tp.YB AS youZhengBM, " +
					" ts.TS AS TS " +
					" FROM " +
					" xmis.org_资质管理表 org," +
					" xmis.hr_人员 hr1," +
					" xmis.hr_人员 hr2," +
					" #test  ts  ," +
					" #tmp tp  " +
					" WHERE org.联系人ID =tp.ryid " +
					" AND org.登记人ID = hr1.人员ID " +
					" AND org.最后修改人ID = hr2.人员ID " +
					" AND org.ID = ? ";
		return jt.queryForList(sql, new Object[]{id,id});
	}

	public List getProjectQualification(String companyID) {
		// TODO Auto-generated method stub
		String sql = "select ID AS id , 资质名称 as qualificationName  from xmis.org_资质管理表 where 公司ID = ? ";
		return jt.queryForList(sql, new Object[]{companyID});
	}

	public void doProcurementInformation(ProjectQualification rq) {
		// TODO Auto-generated method stub
		saveOrUpdateObject(rq);
	}

	public List getUseProjectQualification(String companyID) {
		// TODO Auto-generated method stub
		String sql = "SELECT org.ID AS id,org1.ID as ZIID,org1.ID AS ZIID2, org1.资质名称 AS qualificationName, " +
				" org2.部门名称 AS qualificationDepartment,REPLACE((hr.姓氏+hr.名字),' ','') AS qualificationApplicant," +
				" prj.项目名称 AS qualificationProject,CONVERT(varchar, org.申请日期,120) AS applicantDate " +
				" FROM " +
				" xmis.org_资质使用部门表 org " +
				" left join  xmis.org_资质管理表 org1 " +
				"  on org.资质名称对应ID = org1.ID " +
				"  left join  xmis.hr_人员 hr " +
				" on org.申请人ID = hr.人员ID " +
				" left join xmis.org_部门 org2 " +
				" on org.申请部门ID=org2.部门ID  " +
				" left join  xmis.prj_项目 prj  " +
				" on org.使用项目ID  = prj.项目ID " +
				"  WHERE " +
				" org1.公司ID = ? " +
				"   order by org1.资质名称 ";
		return jt.queryForList(sql, new Object[]{companyID});
		
	}

	public List getProjectQualificationAssignment(String id) {
		// TODO Auto-generated method stub
		String sql = "SELECT org.ID AS id,org.申请部门ID AS applicationSectorsID,org1.部门名称 AS applicationSectorsName," +
				" org.资质名称对应ID AS ziID,org2.资质名称 AS qualificationName," +
				" org.使用项目ID AS projectID,prj.项目名称 AS projectName," +
				" org.申请人ID AS applicantID,REPLACE((hr3.姓氏+hr3.名字),' ','') AS applicant," +
				" CONVERT(VARCHAR ,org.申请日期,120) AS applicationDate ,org.申请原因 AS applicationReasons," +
				" REPLACE((hr1.姓氏+hr1.名字),' ','') AS applicationNewregisterName," +
				"  REPLACE((hr2.姓氏+hr2.名字),' ','') AS applicationLastModifierName," +
				"  CONVERT(VARCHAR,org.登记日期 ,120) AS applicationNewupdateDate," +
				"  CONVERT(VARCHAR ,org.最后修改日期 ,120) AS applicationLastModified " +
				" from " +
				" XMIS.org_资质使用部门表 org " +
				" left join xmis.org_部门 org1 " +
				" on  org.申请部门ID = org1.部门ID " +
				" left join xmis.org_资质管理表 org2 " +
				" on org.资质名称对应ID = org2.ID " +
				" left join xmis.prj_项目 prj " +
				" on org.使用项目ID = prj.项目ID " +
				" left join xmis.hr_人员 hr1 " +
				" on org.登记人ID = hr1.人员ID " +
				" left join xmis.hr_人员 hr2  " +
				" on org.最后修改人ID = hr2.人员ID " +
				" left join xmis.hr_人员 hr3  " +
				" on org.申请人ID = hr3.人员ID " +
				"  WHERE org.ID = ? ";
		return jt.queryForList(sql, new Object[]{id});
	}

	public List getDecisionMakersInformation(Integer hrID) {
		// TODO Auto-generated method stub
		String sql = "SELECT (add5.区号+'-'+addc.区号+'-'+hr.单位电话) as DH," +
				"(add5.区号+'-'+addc.区号+'-'+hr.传真号码) as CZ," +
				"hr.电子邮件 AS Email " +
				"FROM xmis.hr_人员 hr " +
				"LEFT JOIN xmis.add_地址 addd " +
				"ON hr.办公地址ID = addd.地址ID   "+
				"LEFT JOIN xmis.add_城市 addc " +
				"ON addd.城市ID = addc.城市ID " +
				"LEFT JOIN xmis.add_省 add3 " +
				"ON addc.省ID = add3.省ID " +
				"LEFT JOIN xmis.add_地区 add4 " +
				" ON add3.地区ID = add4.地区ID " +
				" LEFT JOIN xmis.add_国家及地区 add5 " +
				" ON add4.国家ID = add5.国家ID " +
				" WHERE  hr.人员ID = ? ";
				return jt.queryForList(sql, new Object[]{hrID});
	}

	public void doPatentInformation(PatentInformation pf) {
		// TODO Auto-generated method stub
		saveOrUpdateObject(pf);
	}

	public List getPatentInformationList(String companyID) {
		// TODO Auto-generated method stub
		String sql = "select org.ID as sjid, org.专利名称 as patentName,org.类别 as patentsCategory," +
				"org.设计人1ID AS patentedDesignPeopleID1,org.设计人2ID AS patentedDesignPeopleID2," +
				"org.设计人3ID as patentedDesignPeopleID3,org.专利权人1ID as patentRightPeopleID1," +
				"org.专利权人2ID as patentRightPeopleID2,org.专利权人3ID as patentRightPeopleID3," +
				"org.专利权人4ID as patentRightPeopleID4,org.专利权人5ID as patentRightPeopleID5," +
				"REPLACE((hr1.姓氏+hr1.名字),' ','')+','" +
				"+isnull(REPLACE((hr2.姓氏+hr2.名字),' ',''),'')+','" +
				"+isnull(REPLACE((hr3.姓氏+hr3.名字),' ',''),'') AS patentedDesignPeople," +
				" REPLACE((hr4.姓氏+hr4.名字),' ','')+','" +
				"+isnull(REPLACE((hr5.姓氏+hr5.名字),' ',''),'')+','" +
				"+isnull(REPLACE((hr6.姓氏+hr6.名字),' ',''),'')+','" +
				"+isnull(REPLACE((hr7.姓氏+hr7.名字),' ',''),'')+','" +
				"+isnull(REPLACE((hr8.姓氏+hr8.名字),' ',''),'')  as patentRightPeople," +
				"CONVERT(VARCHAR,org.有效期起, 120) as validFrom,CONVERT(VARCHAR,org.有效期止, 120) as validOnly " +
				"from " +
				"xmis.org_公司专利表 org " +
				"left join xmis.hr_人员 hr1 " +
				"on org.设计人1ID = hr1.人员ID " +
				"left join  xmis.hr_人员 hr2 " +
				"on org.设计人2ID = hr2.人员ID " +
				"left join xmis.hr_人员 hr3 " +
				"on org.设计人3ID = hr3.人员ID " +
				"left join xmis.hr_人员 hr4 " +
				"on org.专利权人1ID = hr4.人员ID " +
				"left join xmis.hr_人员 hr5 " +
				"on org.专利权人2ID = hr5.人员ID " +
				"left join xmis.hr_人员 hr6 " +
				"on org.专利权人3ID = hr6.人员ID " +
				"left join xmis.hr_人员 hr7 " +
				"on org.专利权人4ID = hr7.人员ID " +
				"left join xmis.hr_人员 hr8 " +
				"on org.专利权人5ID = hr8.人员ID " +
				" where org.公司ID = ? ";
		return jt.queryForList(sql, new Object[]{companyID});
	}

	public List getPatentAssignment(String sjid) {
		// TODO Auto-generated method stub
		String sql =" select org.ID as sjid,org.公司ID AS companyID ,org.专利名称 as patentName,org.类别 as patentsCategory," +
					" org.设计人1ID AS patentedDesignPeopleID1," +
					"org.设计人2ID AS patentedDesignPeopleID2," +
					" org.设计人3ID as patentedDesignPeopleID3," +
					"org.专利权人1ID as patentRightPeopleID1," +
					" org.专利权人2ID as patentRightPeopleID2," +
					"org.专利权人3ID as patentRightPeopleID3," +
					" org.专利权人4ID as patentRightPeopleID4," +
					"org.专利权人5ID as patentRightPeopleID5," +
					" REPLACE((hr1.姓氏+hr1.名字),' ','') as patentedDesignPeople1," +
					" REPLACE((hr2.姓氏+hr2.名字),' ','') as patentedDesignPeople2," +
					" REPLACE((hr3.姓氏+hr3.名字),' ','') as patentedDesignPeople3," +
					" REPLACE((hr4.姓氏+hr4.名字),' ','') as patentRightPeople1," +
					" REPLACE((hr5.姓氏+hr5.名字),' ','') as patentRightPeople2," +
					" REPLACE((hr6.姓氏+hr6.名字),' ','') as patentRightPeople3," +
					" REPLACE((hr7.姓氏+hr7.名字),' ','') as patentRightPeople4," +
					" REPLACE((hr8.姓氏+hr8.名字),' ','') as patentRightPeople5," +
					" org.专利号 as patentNumber," +
					" org.证书号 as patentCertificateNo," +
					" CONVERT(VARCHAR,org.申请日 , 120) as applicationTime," +
					" CONVERT(VARCHAR,org.授权公告日 ,120) as authorizeTime," +
					" REPLACE((hr9.姓氏+hr9.名字),' ','') as patentregisterName," +
					" REPLACE((hr10.姓氏+hr10.名字),' ','') as patentlastModifierName," +
					" CONVERT(VARCHAR,org.登记日期 , 120) as patentupdateDate," +
					" CONVERT(VARCHAR,org.最后修改日期 ,120) as patentlastModified," +
					" CONVERT(VARCHAR,org.有效期起, 120) as validFrom," +
					" CONVERT(VARCHAR,org.有效期止, 120) as validOnly " +
					" from " +
					" xmis.org_公司专利表 org " +
					" left join xmis.hr_人员 hr1 " +
					" on org.设计人1ID = hr1.人员ID " +
					" left join  xmis.hr_人员 hr2 " +
					" on org.设计人2ID = hr2.人员ID " +
					" left join xmis.hr_人员 hr3 " +
					" on org.设计人3ID = hr3.人员ID " +
					" left join xmis.hr_人员 hr4 " +
					" on org.专利权人1ID = hr4.人员ID " +
					" left join xmis.hr_人员 hr5 " +
					" on org.专利权人2ID = hr5.人员ID " +
					" left join xmis.hr_人员 hr6 " +
					" on org.专利权人3ID = hr6.人员ID " +
					" left join xmis.hr_人员 hr7 " +
					" on org.专利权人4ID = hr7.人员ID " +
					" left join xmis.hr_人员 hr8 " +
					" on org.专利权人5ID = hr8.人员ID " +
					" left join xmis.hr_人员 hr9 " +
					" on org.登记人ID = hr9.人员ID " +
					" left join xmis.hr_人员 hr10 " +
					" on org.最后修改人ID = hr10.人员ID " +
					" where org.ID = ? ";
		return jt.queryForList(sql, new Object[]{sjid});
	}

	public List getQualificationListC(StandardQuery query, PageInfo pageInfo) {
		// TODO Auto-generated method stub
		String sql =  " SELECT org1.ID AS id,org1.公司ID AS companyID,org2.公司名称 as companyName,org1.资质名称 AS qualifiationName,org1.证书号 AS certificateNo ," +
						" org1.证书有效期起 AS certificateValidQi,org1.证书有效期止 AS certificateValidZhi ," +
						"  org1.备注 AS qualificationRemark ," +
						" COUNT(org.资质名称对应ID) AS frequencyOfUse , " +
						"CASE WHEN org1.是否年审  = '0' THEN  '是' " +
						"WHEN org1.是否年审  = '1' THEN  '否' " +
						"ELSE '' END AS examined "+
						"into #result " +
						" FROM xmis.org_资质管理表 org1 " +
						" left join xmis.org_资质使用部门表 org " +
						" on org1.ID = org.资质名称对应ID " +
						" left join xmis.org_公司 org2 " +
						"  on org1.公司ID = org2.公司ID " +
						" WHERE  org1.公司ID <>  1 " +
						" GROUP BY org1.ID,org1.公司ID,org2.公司名称,org1.资质名称,org1.证书号,org1.证书有效期起,org1.证书有效期止,org1.是否年审, org1.备注 ";
		SQLGenerator generator = new SQLGenerator(sql);
		if(query !=null){
			query.addToSQLGenerator(generator);
		}
		String sqlty = WebUtil.generateSQLStr(generator.getSQL(),pageInfo);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0))?new Object[] {} : new Object[] {
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlty,args);
	}

	public List getPatentInfoListC(StandardQuery query, PageInfo pageInfo) {
		// TODO Auto-generated method stub
		String sql = " select org.ID as sjid,org.公司ID AS companyID ,org1.公司名称 as companyName,org.专利名称 as patentName,org.类别 as patentsCategory," +
		" org.设计人1ID AS patentedDesignPeopleID1," +
		"org.设计人2ID AS patentedDesignPeopleID2," +
		" org.设计人3ID as patentedDesignPeopleID3," +
		"org.专利权人1ID as patentRightPeopleID1," +
		" org.专利权人2ID as patentRightPeopleID2," +
		"org.专利权人3ID as patentRightPeopleID3," +
		" org.专利权人4ID as patentRightPeopleID4," +
		"org.专利权人5ID as patentRightPeopleID5," +
		"REPLACE((hr1.姓氏+hr1.名字),' ','')+','" +
		"+isnull(REPLACE((hr2.姓氏+hr2.名字),' ',''),'')+','" +
		"+isnull(REPLACE((hr3.姓氏+hr3.名字),' ',''),'') AS patentedDesignPeople," +
		" REPLACE((hr4.姓氏+hr4.名字),' ','')+','" +
		"+isnull(REPLACE((hr5.姓氏+hr5.名字),' ',''),'')+','" +
		"+isnull(REPLACE((hr6.姓氏+hr6.名字),' ',''),'')+','" +
		"+isnull(REPLACE((hr7.姓氏+hr7.名字),' ',''),'')+','" +
		"+isnull(REPLACE((hr8.姓氏+hr8.名字),' ',''),'')  as patentRightPeople," +
		" org.专利号 as patentNumber," +
		" org.证书号 as patentCertificateNo," +
		" CONVERT(VARCHAR,org.申请日 , 120) as applicationTime," +
		" CONVERT(VARCHAR,org.授权公告日 ,120) as authorizeTime," +
		" REPLACE((hr9.姓氏+hr9.名字),' ','') as patentregisterName," +
		" REPLACE((hr10.姓氏+hr10.名字),' ','') as patentlastModifierName," +
		" CONVERT(VARCHAR,org.登记日期 , 120) as patentupdateDate," +
		" CONVERT(VARCHAR,org.最后修改日期 ,120) as patentlastModified," +
		" CONVERT(VARCHAR,org.有效期起, 120) as validFrom," +
		" CONVERT(VARCHAR,org.有效期止, 120) as validOnly " +
		" from " +
		" xmis.org_公司专利表 org " +
		" left join xmis.hr_人员 hr1 " +
		" on org.设计人1ID = hr1.人员ID " +
		" left join  xmis.hr_人员 hr2 " +
		" on org.设计人2ID = hr2.人员ID " +
		" left join xmis.hr_人员 hr3 " +
		" on org.设计人3ID = hr3.人员ID " +
		" left join xmis.hr_人员 hr4 " +
		" on org.专利权人1ID = hr4.人员ID " +
		" left join xmis.hr_人员 hr5 " +
		" on org.专利权人2ID = hr5.人员ID " +
		" left join xmis.hr_人员 hr6 " +
		" on org.专利权人3ID = hr6.人员ID " +
		" left join xmis.hr_人员 hr7 " +
		" on org.专利权人4ID = hr7.人员ID " +
		" left join xmis.hr_人员 hr8 " +
		" on org.专利权人5ID = hr8.人员ID " +
		" left join xmis.hr_人员 hr9 " +
		" on org.登记人ID = hr9.人员ID " +
		" left join xmis.hr_人员 hr10 " +
		" on org.最后修改人ID = hr10.人员ID " +
		" left join xmis.org_公司 org1 " +
		"  on org.公司ID = org1.公司ID " +
		" where org.公司ID <> 1  ";
		SQLGenerator generator = new SQLGenerator(sql);
		if(query !=null){
			query.addToSQLGenerator(generator);
		}
		String sqlty = WebUtil.generateSQLStr(generator.getSQL(),pageInfo);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0))?new Object[] {} : new Object[] {
						new Integer(pageInfo.getStart() + 1),
						new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlty,args);
	}

	public List getQualificationAssignmentr(String id) {
		// TODO Auto-generated method stub
		String sql = "SELECT (add5.区号+'-'+addc.区号+'-'+hr.单位电话) as DH," +
		"(add5.区号+'-'+addc.区号+'-'+hr.传真号码) as CZ," +
		"hr.电子邮件 AS Email," +
		"hr.人员ID AS ryid ," +
		"Replace((hr.姓氏 + hr.名字),' ','') as JLM, " +
		"hr.移动电话 AS YD," +
		"hr.家庭邮编 AS YB " +
		" INTO #tmp  " +
		"FROM xmis.hr_人员 hr " +
		"LEFT JOIN xmis.add_地址 addd " +
		"ON hr.办公地址ID = addd.地址ID   "+
		"LEFT JOIN xmis.add_城市 addc " +
		"ON addd.城市ID = addc.城市ID " +
		"LEFT JOIN xmis.add_省 add3 " +
		"ON addc.省ID = add3.省ID " +
		"LEFT JOIN xmis.add_地区 add4 " +
		" ON add3.地区ID = add4.地区ID " +
		" LEFT JOIN xmis.add_国家及地区 add5 " +
		" ON add4.国家ID = add5.国家ID " +
		" SELECT count (资质名称对应ID) AS TS INTO #test  FROM xmis.org_资质使用部门表 WHERE 资质名称对应ID = ? " +
		" SELECT " +
			" ID AS id,org.公司ID AS companyID,org.联系人ID AS qualificationContactID," +
			" tp.JLM AS qualificationContact, " +
			" CONVERT(varchar,org.证书有效期起, 120 ) AS certificateValidQi,CONVERT(varchar,org.证书有效期止 , 120 )AS certificateValidZhi," +
			" org.证书号 AS certificateNo,org.备注 AS qualificationRemark," +
			" org.是否年审 AS examined,org.资质名称 AS qualificationName,org.管理部门 AS departmentName," +
			" Replace((hr1.姓氏+hr1.名字),' ','') AS newregisterName," +
			" Replace((hr2.姓氏+hr2.名字),' ','') AS lastModifierName," +
			" CONVERT(varchar,org.最后修改日期, 120 ) AS  lastModified,CONVERT(varchar,org.登记日期 , 120 )AS newupdateDate," +
			"  tp.DH AS DH,tp.CZ AS CZ,tp.Email AS Email ,tp.YD  AS dianHua,tp.YB AS youZhengBM, " +
			" ts.TS AS TS " +
			" FROM " +
			" xmis.org_资质管理表 org," +
			" xmis.hr_人员 hr1," +
			" xmis.hr_人员 hr2," +
			" #test  ts  ," +
			" #tmp tp  " +
			" WHERE org.联系人ID =tp.ryid " +
			" AND org.登记人ID = hr1.人员ID " +
			" AND org.最后修改人ID = hr2.人员ID " +
			" AND org.ID = ? ";
		return jt.queryForList(sql, new Object[] { id ,id});
	}

	public List getPatentAssignmentr(String sjid) {
		// TODO Auto-generated method stub
		String sql = " select org.ID as sjid,org.公司ID AS companyID ,org.专利名称 as patentName,org.类别 as patentsCategory," +
		" org.设计人1ID AS patentedDesignPeopleID1," +
		"org.设计人2ID AS patentedDesignPeopleID2," +
		" org.设计人3ID as patentedDesignPeopleID3," +
		"org.专利权人1ID as patentRightPeopleID1," +
		" org.专利权人2ID as patentRightPeopleID2," +
		"org.专利权人3ID as patentRightPeopleID3," +
		" org.专利权人4ID as patentRightPeopleID4," +
		"org.专利权人5ID as patentRightPeopleID5," +
		" REPLACE((hr1.姓氏+hr1.名字),' ','') as patentedDesignPeople1," +
		" REPLACE((hr2.姓氏+hr2.名字),' ','') as patentedDesignPeople2," +
		" REPLACE((hr3.姓氏+hr3.名字),' ','') as patentedDesignPeople3," +
		" REPLACE((hr4.姓氏+hr4.名字),' ','') as patentRightPeople1," +
		" REPLACE((hr5.姓氏+hr5.名字),' ','') as patentRightPeople2," +
		" REPLACE((hr6.姓氏+hr6.名字),' ','') as patentRightPeople3," +
		" REPLACE((hr7.姓氏+hr7.名字),' ','') as patentRightPeople4," +
		" REPLACE((hr8.姓氏+hr8.名字),' ','') as patentRightPeople5," +
		" org.专利号 as patentNumber," +
		" org.证书号 as patentCertificateNo," +
		" CONVERT(VARCHAR,org.申请日 , 120) as applicationTime," +
		" CONVERT(VARCHAR,org.授权公告日 ,120) as authorizeTime," +
		" REPLACE((hr9.姓氏+hr9.名字),' ','') as patentregisterName," +
		" REPLACE((hr10.姓氏+hr10.名字),' ','') as patentlastModifierName," +
		" CONVERT(VARCHAR,org.登记日期 , 120) as patentupdateDate," +
		" CONVERT(VARCHAR,org.最后修改日期 ,120) as patentlastModified," +
		" CONVERT(VARCHAR,org.有效期起, 120) as validFrom," +
		" CONVERT(VARCHAR,org.有效期止, 120) as validOnly " +
		" from " +
		" xmis.org_公司专利表 org " +
		" left join xmis.hr_人员 hr1 " +
		" on org.设计人1ID = hr1.人员ID " +
		" left join  xmis.hr_人员 hr2 " +
		" on org.设计人2ID = hr2.人员ID " +
		" left join xmis.hr_人员 hr3 " +
		" on org.设计人3ID = hr3.人员ID " +
		" left join xmis.hr_人员 hr4 " +
		" on org.专利权人1ID = hr4.人员ID " +
		" left join xmis.hr_人员 hr5 " +
		" on org.专利权人2ID = hr5.人员ID " +
		" left join xmis.hr_人员 hr6 " +
		" on org.专利权人3ID = hr6.人员ID " +
		" left join xmis.hr_人员 hr7 " +
		" on org.专利权人4ID = hr7.人员ID " +
		" left join xmis.hr_人员 hr8 " +
		" on org.专利权人5ID = hr8.人员ID " +
		" left join xmis.hr_人员 hr9 " +
		" on org.登记人ID = hr9.人员ID " +
		" left join xmis.hr_人员 hr10 " +
		" on org.最后修改人ID = hr10.人员ID " +
		" where org.ID = ? ";
		return jt.queryForList(sql, new Object[] { sjid });
	}
}
