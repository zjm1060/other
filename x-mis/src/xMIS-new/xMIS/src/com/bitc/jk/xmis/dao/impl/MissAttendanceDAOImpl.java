package com.bitc.jk.xmis.dao.impl;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementCreator;

import com.bitc.jk.xmis.dao.MissAttendanceDAO;
import com.bitc.jk.xmis.model.MissAttendanceInfos;
import com.bitc.jk.xmis.model.MissAttendanceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.SpringInit;

public class MissAttendanceDAOImpl extends BaseDAOImpl implements
		MissAttendanceDAO {
	private JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate jt) {
		this.jt = jt;
	}

	public void doCreateMissAttendanceInfos(
			MissAttendanceInfos missAttendanceInfos) {
		ht.save(missAttendanceInfos);
	}

	public String doCreateMissAttendanceObj(MissAttendanceObj missAttendanceObj) {
		ht.saveOrUpdate(missAttendanceObj);
		return missAttendanceObj.getId();
	}

	public void doUpdateMissAttendanceInfos(
			MissAttendanceInfos missAttendanceInfos) {
		ht.saveOrUpdate(missAttendanceInfos);

	}

	// 根据 接收人ID、TASK_ID、创建日期 查找 满足条件的“统计的一条待办
	public Boolean isExistMissAttendanceList(String todoModel, String actorId,
			String createTime) {
		Boolean isExist = false;
		String sql = SQLScript.getAllMissAttendanceList;
		int count = jt.queryForInt(sql, new Object[] { actorId, todoModel,
				createTime });
		if (count > 0) {
			isExist = true;
		} else {
			isExist = false;
		}
		return isExist;
	}

	@SuppressWarnings("rawtypes")
	public List getMissAttendanceList(StandardQuery query, PageInfo pageInfo,
			String todoIdsQuery)  throws Exception {
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getMissAttendanceList + todoIdsQuery);
		query.addToSQLGenerator(generator);
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);

		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {} : new Object[] {
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		if (jt == null) {
			jt = (JdbcTemplate) SpringInit.getBean("JdbcTemplate");
			if (jt == null) {
				return null;
			}
		}
		List list = jt.queryForList(sqlStr, args);
		if (list != null && list.size() > 0) {
			return list;
		}
		return null;
	}

	public void doUpdateMissAttendanceObj(MissAttendanceObj missAttendanceObj) {
		ht.saveOrUpdate(missAttendanceObj);
	}

	@SuppressWarnings("rawtypes")
	public String getValueFromParamTableByName(String name) {
		String sql = SQLScript.getLastDay;
		String result = "";
		List list = jt.queryForList(sql, new Object[] { name });
		if (list != null && list.size() > 0) {
			Map m = (Map) list.get(0);
			result = m.get("value").toString();
		}
		return result;
	}

	// public Boolean isExistMissAttendanceListByEmpId(String script,
	// String actorId, String createTime) {
	// Boolean isExist = false;
	// String sql = SQLScript.isExistMissAttendanceListByEmpId;
	// int count = jt.queryForInt(sql,new Object[]{script,actorId,createTime});
	// if(count>0){
	// isExist = true;
	// }else{
	// isExist = false;
	// }
	// return isExist;
	// }

	@SuppressWarnings("rawtypes")
	public List getMissAttendanceListByEmpId(String isOfficialTravel,
			String todoModelList, String wokItemId, String isTodo,
			PageInfo pageInfo)  throws Exception {
		String sql = SQLScript.getMissAttendanceListByEmpId;
		if (jt == null) {
			jt = (JdbcTemplate) SpringInit.getBean("JdbcTemplate");
			if (jt == null) {
				return null;
			}
		}
		List list = jt.queryForList(sql, new Object[] { todoModelList,
				wokItemId, isTodo, isOfficialTravel });
		if (list != null && list.size() > 0) {
			return list;
		}
		return null;
	}

	@SuppressWarnings("rawtypes")
	public List getMissAttendanceListByQuery(StandardQuery query,
			PageInfo pageInfo, String dateFirst, String dateLast) {
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getMissAttendanceListByQuery);
		if (!StringUtils.isBlank(dateFirst)) {
			String paramsql = SQLScript.getMissAttendanceListByQueryWithMonth;
			paramsql = paramsql.replace("@DATEFIRST", dateFirst).replace(
					"@DATELAST", dateLast);
			generator = new SQLGenerator(paramsql);
		}
		query.addToSQLGenerator(generator);
		String sqlStr = this.generateSQLStrWithDivisionIDSorft(
				generator.getSQL(), pageInfo);

		// Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 &&
		// pageInfo
		// .getLimit() == 0)) ? new Object[] {} : new Object[] {
		// new Integer(pageInfo.getStart() + 1),
		// new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		Object[] args = new Object[] {};
		return jt.queryForList(sqlStr, args);
	}

	public String generateSQLStrWithDivisionIDSorft(String sqlStr,
			PageInfo pageInfo) {
		sqlStr = "SET NOCOUNT ON \n" + sqlStr;
		String orderByStr = (pageInfo.getSort() == null || "".equals(pageInfo
				.getSort())) ? "\n order by   事业部ID  asc \n"
				: "\n order by   事业部ID  asc ," + pageInfo.getSort() + " "
						+ pageInfo.getDir();

		// return (pageInfo.getStart() == 0 && pageInfo.getLimit() == 0) ?
		// (sqlStr
		// + SQLScript.getResultStrWithoutPaging + orderByStr)
		// : (sqlStr + SQLScript.getResultStrWithPaging + orderByStr +
		// SQLScript.getResultStringWithPagingDefault);
		return (sqlStr + SQLScript.getResultStrWithoutPaging + orderByStr);
	}

	@SuppressWarnings("rawtypes")
	public List getMissAttendanceInfos(String workItemId) {
		String sql = SQLScript.getMissAttendanceInfos;
		return jt.queryForList(sql, new Object[] { new String(workItemId) });
	}

	public MissAttendanceInfos getMissAttendanceInfoById(String infoId) {
		return (MissAttendanceInfos) ht.get(MissAttendanceInfos.class, infoId);
	}

	public void doDeleteMissAttendanceInfos(MissAttendanceInfos info) {
		ht.delete(info);
	}

	public MissAttendanceObj getMissAttendanceObjById(String missAttendanceId) {
		return (MissAttendanceObj) ht.get(MissAttendanceObj.class,
				missAttendanceId);
	}

	public void doDeleteMissAttendanceObj(MissAttendanceObj missAttendanceObj) {
		ht.delete(missAttendanceObj);
	}

	public boolean isExistMissAttendanceRecord(String infoId, int year,
			int month) {
		String sql = SQLScript.isExistMissAttendanceRecord;
		int num = jt.queryForInt(sql, new Object[] { infoId, year, month });
		if (num > 0) {
			return true;
		}
		return false;
	}

	public void doUpdateMissAttendanceRecordState(final String infoId,
			final int year, final int month, final int state) {
		jt.update(new PreparedStatementCreator() {

			public PreparedStatement createPreparedStatement(Connection con)
					throws SQLException {
				String sql = SQLScript.doUpdateMissAttendanceRecordState;
				PreparedStatement ps = con.prepareStatement(sql);
				ps.setString(3, infoId);
				ps.setDate(2, DateUtil.getCurrentDateTime());
				ps.setInt(4, year);
				ps.setInt(5, month);
				ps.setInt(1, state);
				return ps;
			}
		});
	}

	public void doCreateMissAttendanceRecord(final String infoId,
			final int year, final int month, final int state) {
		jt.update(new PreparedStatementCreator() {

			public PreparedStatement createPreparedStatement(Connection con)
					throws SQLException {
				String sql = SQLScript.doCreateMissAttendanceRecord;
				PreparedStatement ps = con.prepareStatement(sql);
				ps.setString(1, infoId);
				ps.setInt(2, year);
				ps.setInt(3, month);
				ps.setDate(5, DateUtil.getCurrentDateTime());
				ps.setInt(4, state);
				return ps;
			}
		});

	}

	@SuppressWarnings("rawtypes")
	public List getApprovalTaskListByQuery(StandardQuery query,
			PageInfo pageInfo) {
		SQLGenerator generator = new SQLGenerator(SQLScript.getApprovalTaskList);
		query.addToSQLGenerator(generator);
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);

		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {} : new Object[] {
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);
	}

	@SuppressWarnings("unchecked")
	public List<MissAttendanceObj> getMissAttendanceId(String processInstanceId) {
		String hql ="from MissAttendanceObj m where m.processInstanceId=?";
		return ht.find(hql, processInstanceId);
	}

	@SuppressWarnings("rawtypes")
	public List<String> getAllMyCompEmailList() {
		final String sql = 
			"SELECT    RTRIM(a.电子邮件) AS email\n" +
			"FROM      xmis.hr_人员部门关系表 b\n" + 
			"          JOIN xmis.hr_人员 a ON a.人员ID = b.人员ID\n" + 
			"          LEFT JOIN xmis.org_部门归属公司表 e ON b.部门ID = e.部门ID\n" + 
			"WHERE     a.人员ID > 0\n" + 
			"          AND e.公司ID = 1\n" + 
			"          AND  a.电子邮件 IS NOT NULL\n" + 
			"          AND ( a.禁用 = 0\n" + 
			"                OR a.禁用 IS NULL\n" + 
			"              )";

		List mapList = jt.queryForList(sql);
		List<String> result = new ArrayList<String>();
		for (int i = 0; i < mapList.size(); i++) {
			Map map = (Map) mapList.get(i);
			result.add((String) map.get("email"));
		}
		return result;
	}
}
