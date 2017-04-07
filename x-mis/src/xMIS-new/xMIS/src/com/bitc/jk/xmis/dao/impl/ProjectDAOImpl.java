//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\impl\\ProjectDAOImpl.java

package com.bitc.jk.xmis.dao.impl;

import java.sql.SQLException;
import java.util.List;

import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.orm.hibernate3.HibernateCallback;

import com.bitc.jk.xmis.dao.ProjectDAO;
import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.ProjectDevicePurchaseStatus;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.ProjectSchedulePlan;
import com.bitc.jk.xmis.model.ProjectStatus;
import com.bitc.jk.xmis.model.ProjectTaskMember;
import com.bitc.jk.xmis.model.ProjectTaskObj;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class ProjectDAOImpl extends BaseDAOImpl implements ProjectDAO {

	private JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate ajdbcTemplate) {
		this.jt = ajdbcTemplate;
	}
	
	
	/**
	 * @roseuid 4C1F04200280
	 */
	public ProjectDAOImpl() {

	}

	/**
	 * @param projectID
	 * @return com.bitc.jk.xmis.model.ProjectObj
	 * @roseuid 4C1F042002DE
	 */
	public ProjectObj getProjectInfoByID(String projectID) {
		return load(ProjectObj.class, projectID);
	}

	public ProjectObj getProjectByProjectCode(final String projectCode) {
		return (ProjectObj) ht
				.executeWithNativeSession(new HibernateCallback() {
					public Object doInHibernate(Session session)
							throws HibernateException, SQLException {
						return session
								.createQuery(
										"from ProjectObj obj where obj.projectCode=?")
								.setParameter(0, projectCode).uniqueResult();
					}
				});
	}

	/**
	 * @param projectID
	 * @return List
	 * @roseuid 4C1F0421005D
	 */
	@SuppressWarnings("unchecked")
	public List<DeviceObj> getDeviceListByID(String projectID) {
		return ht.findByNamedQuery("getDeviceListByProjectId", projectID);
	}

	@SuppressWarnings("unchecked")
	public List<ProjectTaskObj> getTaskListByProjectID(String projectID) {
		return ht.findByNamedQuery("getTaskListByProjectID", projectID);
	}

	/**
	 * @param deviceList
	 * @roseuid 4C1F0421009C
	 */
	public void doSaveDevice(DeviceObj device) {
		saveObject(device);
	}

	/**
	 * @param deviceObj
	 * @roseuid 4C1F042100DA
	 */
	public void doUpdateDevice(DeviceObj deviceObj) {
		upldateObject(deviceObj);
	}

	/**
	 * @param projectID
	 * @param deviceID
	 * @roseuid 4C1F04210119
	 */
	public void doDeleteDeviceList(int projectID, int deviceID) {

	}

	public void doDeleteDevice(String deviceID) {
		DeviceObj o = new DeviceObj();
		o.setId(deviceID);
		deleteObject(o);
	}

	public void doSaveProjectTaskMember(ProjectTaskMember member) {
		ht.saveOrUpdate(member);
	}

	public void updateProjectID(final int mainContractID, final String projectID) {
		ht.executeWithNativeSession(new HibernateCallback() {
			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				session.getNamedQuery("updateContractProjectID")
						.setString(0, projectID).setInteger(1, mainContractID)
						.setInteger(2, mainContractID).executeUpdate();
				return null;
			}
		});
	}

	/**
	 * @param projectObj
	 * @roseuid 4C1F04210186
	 */
	public void doSaveProjectInfo(ProjectObj projectObj) {
		ht.saveOrUpdate(projectObj);
	}

	/**
	 * @param projectObj
	 * @roseuid 4C1F042101C5
	 */
	public void doUpdateProjectInfo(ProjectObj projectObj) {

	}

	/**
	 * @param projectTaskObj
	 * @roseuid 4C1F04210203
	 */
	public void doSaveProjectTask(ProjectTaskObj projectTaskObj) {
		ht.saveOrUpdate(projectTaskObj);
	}

	/**
	 * @param projectTaskObj
	 * @roseuid 4C1F04210251
	 */
	public void doUpdateProjectTask(ProjectTaskObj projectTaskObj) {

	}

	public List<ProjectObj> getProjectList(List<QueryParam> params,
			PageInfo pageInfo) {
		return getListStore(params, pageInfo, ProjectObj.class);
	}

	public List<ProjectObj> getProjectListByInkling(List<QueryParam> params,
			PageInfo pageInfo) {
		return getListStoreByInkling(params, pageInfo, ProjectObj.class);
	}

	public int getProjectCount(final List<QueryParam> params, PageInfo pageInfo) {
		return getTotalCount(params, ProjectObj.class);
	}

	public int getProjectCountByInkling(List<QueryParam> params,
			PageInfo pageInfo) {
		return getTotalCountByInkling(params, ProjectObj.class);
	}

	public List<ProjectObj> getProjectListByQuery(StandardQuery query,
			PageInfo pageInfo) {
		return getListStore(query, pageInfo, ProjectObj.class);
	}

	public int getProjectCountByQuery(StandardQuery query, PageInfo pageInfo) {
		return getTotalCount(query, ProjectObj.class);
	}

	public void doSaveProjectTasks(List<ProjectTaskObj> tasks) {
		ht.saveOrUpdateAll(tasks);
	}

	public void doDeleteProjectTaskByProjectID(final String projectID) {
		ht.executeWithNativeSession(new HibernateCallback() {
			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				session.createQuery(
						"delete ProjectTaskObj t where t.projectID = ?")
						.setString(0, projectID).executeUpdate();
				return null;
			}
		});
	}

	public void saveProjectDevicePurchaseStatus(
			ProjectDevicePurchaseStatus purchaseStatus) {
		this.saveOrUpdateObject(purchaseStatus);
	}

	public List<ProjectStatus> getProjectStatus(StandardQuery query) {
		return getListStore(query, ProjectStatus.class);
	}

	public ProjectDevicePurchaseStatus getProjectDevicePurchaseStatusByPrdId(
			final String prdId) {
		return (ProjectDevicePurchaseStatus) ht
				.executeWithNativeSession(new HibernateCallback() {
					public Object doInHibernate(Session session)
							throws HibernateException, SQLException {
						Query query = session
								.createQuery("from ProjectDevicePurchaseStatus s where s.purchaseRequisitionDetailId=?");
						query.setString(0, prdId);
						return query.uniqueResult();
					}
				});
	}

	public ProjectStatus getProjectStatus(int id) {
		return get(ProjectStatus.class, id);
	}

	public int doSaveProjectStatus(ProjectStatus status) {
		if (status.getId() == null) {
			final String sql = SQLScript.getMaxIDFromTab(
					DBSchema.ProjectStatus.ProjectStatusID,
					DBSchema.ProjectStatus.TableName);
			Integer id = (Integer) ht
					.executeWithNativeSession(new HibernateCallback() {
						public Object doInHibernate(Session session)
								throws HibernateException, SQLException {
							session.flush();
							int r = (Integer) session.createSQLQuery(sql)
									.uniqueResult();
							return r + 1;
						}
					});
			status.setId(id);
		}
		saveOrUpdateObject(status);
		return status.getId();
	}

	public double getReminderAmt(final String projectId) {
		return (Double) ht.executeWithNativeSession(new HibernateCallback() {
			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				String sql = "SELECT  ISNULL(SUM(pc.收款金额),0) "
						+ "FROM   prj_项目 p "
						+ "LEFT JOIN con_合同 c ON c.项目ID = p.项目ID "
						+ "LEFT JOIN con_收款条件表 pc ON pc.合同ID = c.合同ID "
						+ "WHERE   p.项目ID = ? " + "AND pc.收款日期 < ?";
				Query query = session.createSQLQuery(sql)
						.setString(1, projectId)
						.setDate(2, DateUtil.getCurrentDateTime());
				return query.uniqueResult();
			}
		});
	}

	public ProjectSchedulePlan getProjectSchedulePlan(String id) {
		return load(ProjectSchedulePlan.class, id);
	}

	public void saveProjectSchedulePlan(ProjectSchedulePlan plan) {
		saveOrUpdateObject(plan);
	}

	public List<ProjectSchedulePlan> getProjectSchedulePlanByQuery(
			StandardQuery query, PageInfo pageInfo) {
		return getListStore(query, pageInfo, ProjectSchedulePlan.class);
	}

	public void deleteProjectSchedulePlanByProjectId(final String projectId) {
		ht.executeWithNativeSession(new HibernateCallback() {
			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				session.createQuery(
						"delete from ProjectSchedulePlan t where t.project.projectID=?")
						.setString(0, projectId).executeUpdate();
				return null;
			}
		});
	}

	public void deleteProjectSchedulePlan(String id) {
		ProjectSchedulePlan plan = new ProjectSchedulePlan();
		plan.setId(id);
		deleteObject(plan);
	}

	public ProjectSchedulePlan getProjectSchedulePlanByStatusId(
			String projectId, Integer statusId) {
		StandardQuery query = CompositParam
				.buildAndCompositParam()
				.addStandardQuery(
						SimpleParam.buildSimpleParam("project.projectID",
								projectId, Type.STRING, RelationalOperator.EQ))
				.addStandardQuery(
						SimpleParam.buildSimpleParam("status.id", statusId,
								Type.INTEGER, RelationalOperator.EQ));
		List<ProjectSchedulePlan> plans = getProjectSchedulePlanByQuery(query,
				new PageInfo());
		if (plans == null || plans.size() == 0) {
			return null;
		}
		return plans.get(0);
	}

	public ProjectTaskObj getProjectTask(String id) {
		return get(ProjectTaskObj.class, id);
	}

	@SuppressWarnings({ "rawtypes" })
	public List getInterCheckList(final String processInstanceId) {
			final String sql = "SELECT 步骤名称,任务名称,RTRIM(b.姓氏)+RTRIM(b.名字) AS 操作人,CONVERT(VARCHAR(100),完成时间,20) AS 完成时间,备注 ,xmis.wf_历史.流程实例ID as PID FROM xmis.wf_历史"+
			" LEFT JOIN xmis.hr_人员 b ON xmis.wf_历史.任务人ID=b.人员ID WHERE xmis.wf_历史.流程实例ID=?" +
			" order by 完成时间  desc";
			return jt.queryForList(sql,  new Object[]{processInstanceId});
	}

	
	public List getRelationChildContractCodeByID(String contractID) {
		String sql = SQLScript.getContractCodeAndPurchaseIDByContarctID;

		return jt.queryForList(sql, new Object[]{contractID});
	}

	public List getRelationProjectID(Integer contractId) {
		final String sql =" select mon.合同编号 as contractCode  ,mon.合同ID as contractID,mon.合同总额 as contractSum from xmis.con_合同 mon,xmis.con_收入支出合同对应表 xcm " +
 "where xcm.支出子合同ID = mon.合同ID " + 
 "and xcm.支出合同ID = ? " +
 "order by mon.合同ID desc";
		return jt.queryForList(sql,  new Object[]{contractId});
	}

	public List getProjectQualificationName(StandardQuery query, PageInfo pageInfo) throws Exception {
		// TODO Auto-generated method stub
		String sql = "SELECT prj.项目ID AS projectID,prj.项目编码 AS projectCode,prj.项目名称 AS projectName," +
					" prj2.项目状态 AS projectZT,Replace((hr.姓氏+hr.名字),' ','') AS projectJL " +
					" into #result " +
					" FROM xmis.prj_项目 prj " +
					" LEFT OUTER JOIN xmis.prj_项目状态 prj2 " +
					" ON prj.项目状态ID = prj2.项目状态ID " +
					" LEFT OUTER JOIN xmis.hr_人员 hr " +
					" ON prj.项目经理ID =hr.人员ID";
		
		SQLGenerator generator = new SQLGenerator(sql);
		if(query!=null){
			query.addToSQLGenerator(generator);
		}
		
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {} : new Object[] {
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);
	}
}
