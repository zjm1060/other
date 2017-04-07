package com.bitc.jk.xmis.workflow.dao;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.fireflow.engine.impl.ProcessInstance;
import org.fireflow.engine.impl.WorkItem;
import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementCreator;
import org.springframework.orm.hibernate3.HibernateCallback;

import com.bitc.jk.xmis.dao.impl.BaseDAOImpl;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.model.ProcessWithKeyword;
import com.bitc.jk.xmis.workflow.model.WorkItemHistory;
import com.bitc.jk.xmis.workflow.model.WorkflowApprovalInfo;

@SuppressWarnings("unchecked")
public class WorkflowDAO extends BaseDAOImpl {
	public JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate jt) {
		this.jt = jt;
	}

	public List<WorkItem> getWorkItemsInInitializedState(String taskInstanceId) {
		return ht
				.find("from org.fireflow.engine.impl.WorkItem  where taskInstance.id=? and state=0",
						taskInstanceId);
	}

	public List<WorkItem> getWorkItemsByTaskInstance(String taskInstanceId) {
		return ht
				.find("from org.fireflow.engine.impl.WorkItem  where taskInstance.id=?",
						taskInstanceId);
	}

	public void abortTaskInstance(final String taskInstanceId) {

		ht.executeWithNativeSession(new HibernateCallback() {

			public Object doInHibernate(Session arg0)
					throws HibernateException, SQLException {
				String hql = "Update  com.bitc.jk.xmis.workflow.model.WorkItemHistory m set m.state=:state ,m.endTime=:endTime Where m.taskInstanceId=:taskInstanceId And (m.state=0 Or m.state=1)";
				Query query = arg0.createQuery(hql);
				query.setInteger("state", 9);
				query.setDate("endTime", DateUtil.getCurrentDateTime());
				query.setString("taskInstanceId", taskInstanceId);
				query.executeUpdate();
				return null;
			}
		});
	}

	public void overTaskInstanceHistory(final String taskInstanceId) {

		ht.executeWithNativeSession(new HibernateCallback() {

			public Object doInHibernate(Session arg0)
					throws HibernateException, SQLException {
				String hql = "Update  com.bitc.jk.xmis.workflow.model.WorkItemHistory m set m.state=:state ,m.endTime=:endTime Where m.taskInstanceId=:taskInstanceId And (m.state=0 Or m.state=1)";
				Query query = arg0.createQuery(hql);
				query.setInteger("state", 7);
				query.setDate("endTime", DateUtil.getCurrentDateTime());
				query.setString("taskInstanceId", taskInstanceId);
				query.executeUpdate();
				return null;
			}
		});
	}

	public void overTaskInstance(final String taskInstanceId) {

		ht.executeWithNativeSession(new HibernateCallback() {

			public Object doInHibernate(Session arg0)
					throws HibernateException, SQLException {
				String hql = "Update  from org.fireflow.engine.impl.WorkItem m set m.state=:state ,m.endTime=:endTime Where m.taskInstance.id=:taskInstanceId And (m.state=0 Or m.state=1)";
				Query query = arg0.createQuery(hql);
				query.setInteger("state", 7);
				query.setDate("endTime", DateUtil.getCurrentDateTime());
				query.setString("taskInstanceId", taskInstanceId);
				query.executeUpdate();
				return null;
			}
		});
	}

	// 一个任务实例的成员个数
	@SuppressWarnings({ "rawtypes" })
	public List getNumsOfTaskIdByWorkItemId(String workItemId) {
		final String sql = "SELECT COUNT(ID) AS nums FROM xmis.wf_历史 "
				+ " where 任务实例ID in " + " (SELECT 任务实例ID  FROM xmis.wf_历史 "
				+ " WHERE ID='" + workItemId + "')" + " union "
				+ " SELECT COUNT(ID) AS agreenums FROM xmis.wf_历史 "
				+ " where 状态=7 and 任务实例ID in "
				+ " (SELECT 任务实例ID  FROM xmis.wf_历史 " + " WHERE ID= '"
				+ workItemId + "')";
		return (List) ht.executeFind(new HibernateCallback() {

			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				Query query = session.createSQLQuery(sql);
				return query.list();
			}
		});
	}

	public ProcessInstance getProcessInstanceByID(String id) {
		return (ProcessInstance) ht.get(ProcessInstance.class, id);

	}

	/**
	 * 得到正在进行的任务的接收人、及任务分配策略
	 * 
	 * @param processInstanceId
	 * @return
	 */
	@SuppressWarnings("rawtypes")
	public List<Object[]> getActorAndAssignmentStrategyList(
			final String processInstanceId) {
		final String sql = SQLScript.getActorAndAssignmentStrategyList;
		return (List) ht.executeFind(new HibernateCallback() {

			public Object doInHibernate(Session session)
					throws HibernateException, SQLException {
				Query query = session.createSQLQuery(sql);
				query.setParameter(0, processInstanceId);
				return query.list();
			}
		});
	}

	/**
	 * 得到流程实例列表
	 * 
	 * @param query
	 * 
	 * @return
	 */
	// public List<Object[]> getProcessInstanceList() {
	// final String sql = SQLScript.getProcessInstanceList;
	// return ht.executeFind(new HibernateCallback() {
	//
	// public (List)Object doInHibernate(Session session) throws
	// HibernateException,
	// SQLException {
	// Query query = session.createSQLQuery(sql);
	// return query.list();
	// }
	// });
	// }
	@SuppressWarnings("rawtypes")
	public List getProcessInstanceList(StandardQuery query, PageInfo pageInfo) {
		String sql = SQLScript.getProcessInstanceList;
		SQLGenerator generator = new SQLGenerator(sql);
		if (query != null) {
			query.addToSQLGenerator(generator);
		}

		String sqlStrs = WebUtil.generateSQLStr(generator.getSQL(), pageInfo,
				SQLScript.sql_SearchProcessInstanceListWithPageTotal);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {} : new Object[] {
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStrs, args);
	}

	 public List getProcessInstanceComb() {
	 String sql = SQLScript.getProcessInstanceComb;
	 return jt.queryForList(sql);
	 }

	@SuppressWarnings("rawtypes")
	public List getTaskListByQuery(StandardQuery query, PageInfo pageInfo) {
		String sql = SQLScript.getTaskListByQuery;
		SQLGenerator generator = new SQLGenerator(sql);
		if (query != null) {
			query.addToSQLGenerator(generator);
		}

		String sqlStrs = WebUtil.generateSQLStr(generator.getSQL(), pageInfo,
				SQLScript.sql_SearchTaskListWithPageTotal);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {} : new Object[] {
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStrs, args);
	}

	@SuppressWarnings("rawtypes")
	public List getTileAndTaskIDByUserID(final int userID) {
		String sql = SQLScript.GetTileAndTaskIDByUserID;
		return jt.queryForList(sql, new Object[] { userID });
	}

	public void doUpdateWorkHistoryByProcessInstanceId(String id,
			String appendName, int userID) {
		String sql = " UPDATE xmis.wf_历史 SET 任务名称='" + appendName
				+ "' WHERE 流程实例ID='" + id + "' AND 任务人ID='" + userID
				+ "' AND 状态=7";
		jt.update(sql);
	}

	public void doCreateProcessWithKeyword(ProcessWithKeyword processWithKeyword) {
		saveOrUpdateObject(processWithKeyword);
	}

	public void doSaveWorkflowApprovalInfo(WorkflowApprovalInfo approvalInfo) {
		ht.save(approvalInfo);
	}

	@SuppressWarnings("rawtypes")
	public List getApprovalInfos(String processinstanceId) {
		String sql = SQLScript.getApprovalInfos;
		return jt.queryForList(sql, new Object[] { processinstanceId });
	}

	public WorkItem getWorkItemById(String id) {
		return (WorkItem) ht.get(WorkItem.class, id);
	}

	public void doUpdateWorkItemHistory(WorkItemHistory workItemHistory) {
		ht.update(workItemHistory);
	}

	@SuppressWarnings("rawtypes")
	public List getHaveDoneHistoryListStore(StandardQuery query,
			PageInfo pageInfo)  throws Exception {
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getHaveDoneHistoryListStore);
		query.addToSQLGenerator(generator);
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);

		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {} : new Object[] {
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		List list =  null;
		try {
			list =  jt.queryForList(sqlStr, args);
		} catch (Exception e) {
			logger.info(e);
		}
		return list;

	}

	/** 根据任务ID 求邮箱
	 * @param taskId
	 * @return
	 */
/*	public List<String> getActorsEmailListByTaskId(final String taskId) {
		final String sql = "SELECT RTRIM(p.电子邮件) AS email\n"
				+ "FROM   xmis.T_FF_RT_WORKITEM w ,\n"
				+ "       xmis.hr_人员 p\n" + "WHERE  w.ACTOR_ID = p.人员ID\n"
				+ "       AND w.TASKINSTANCE_ID = ?";
		List mapList = jt.queryForList(sql, new Object[] { taskId });
		List<String> result = new ArrayList<String>();
		for (int i = 0; i < mapList.size(); i++) {
			Map map = (Map) mapList.get(i);
			result.add((String) map.get("email"));
		}
		return result;
	}
*/
	public List<String> getActorsEmailByActorId(final String userId) {
		final String sql = SQLScript.getActorsEmailByActorId;

		List<Map<String, String>> mapList = jt.queryForList(sql,
				new Object[] { userId });
		List<String> result = new ArrayList<String>();
		if (mapList != null && mapList.size()>0) {
			Map<String, String> map = (Map<String, String>) mapList.get(0);
			result.add((String) map.get("email"));
		}
		return result;
	}

	public boolean isWaitByWorkItemId(String id) {
		String sql = SQLScript.isWaitByWorkItemId;
		int count = jt.queryForInt(sql, new Object[] { id });
		if (count > 0) {
			return true;
		}
		return false;
	}

	public String getContentByWorkItemId(String id) {
		String sql = SQLScript.getContentByWorkItemId;
		List<Map<String, String>> list = jt.queryForList(sql, new Object[] { id });
		if (list != null && list.size() > 0) {
			Map<String, String> map = (Map<String, String>) list.get(0);
			return  (String) map.get("预计执行日期");
		}
		return "";
	}
	
	/**  
	 *  在流程结束后，讲wf-历史表的当前执行人设置为空
	 *  
	 */
	public void updateLastActorToNull(final String processInstanceId) {
		jt.update(new PreparedStatementCreator() {
			
			public PreparedStatement createPreparedStatement(Connection con)
					throws SQLException {
				String sql = SQLScript.updateLastActorToNull;
				PreparedStatement ps = con.prepareStatement(sql);
				ps.setString(1,processInstanceId);
				return ps;
			}
		});
		
	}

	public List getTodoListSQL(StandardQuery query, PageInfo pageInfo)  throws Exception {
		logger.info("now in workflowdao// getTodoListSQL+------------");
		EmployeeObj employee = WebUtil.getCurrentEmployee();
		int empID = employee.getUserID();
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getTodoListSQL);
		query.addToSQLGenerator(generator);
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);

		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {new Integer(empID),new Integer(empID)} : new Object[] {new Integer(empID),new Integer(empID),
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);

	}

	public List getTodoListSQLByApp(StandardQuery query, PageInfo pageInfo)  throws Exception {
		logger.info("now in workflowdao// getTodoListByApp+------------");
		EmployeeObj employee = WebUtil.getCurrentEmployee();
		int empID = employee.getUserID();
		SQLGenerator generator = new SQLGenerator(
				SQLScript.getTodoListByApp);
		query.addToSQLGenerator(generator);
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
		
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {new Integer(empID),new Integer(empID)} : new Object[] {new Integer(empID),new Integer(empID),
					new Integer(pageInfo.getStart() + 1),
					new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
				return jt.queryForList(sqlStr, args);
				
	}


	@SuppressWarnings("rawtypes")
	public List getAuthorizeList(StandardQuery querys, PageInfo pageInfo) throws Exception{
		String sql = SQLScript.getAuthorizeList;
		SQLGenerator generator = new SQLGenerator(sql);
		querys.addToSQLGenerator(generator);
		String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
		Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? new Object[] {} : new Object[] {
				new Integer(pageInfo.getStart() + 1),
				new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit()) };
		return jt.queryForList(sqlStr, args);
	}
	
	@SuppressWarnings("rawtypes")
	public String getCurrentActorName(String processInstanceId ) {
		String sql = SQLScript.getCurrentActor;
		List list= jt.queryForList(sql, new Object[]{processInstanceId});
		if(list != null && list.size()>0){
			return Type.getString(((Map)list.get(0)).get("name"));
		}
		return "";
	}

	public int doCancelAuthorizeById(final String id) {
		int n = jt.update(new PreparedStatementCreator() {
			
			public PreparedStatement createPreparedStatement(Connection conn)
					throws SQLException {
				String sql = SQLScript.doCancelAuthorizeById;
				PreparedStatement ps = conn.prepareStatement(sql);
				ps.setString(1, id);
				return ps;
			}
		});
		return n;
	}

	public boolean doValidate(String id,String processId, int AuthorizerID, int AuthorizeeID) {
		String sql = SQLScript.isValidateAuthorizeSQL;
		int count = jt.queryForInt(sql, new Object[] { id,processId,AuthorizerID,AuthorizerID,AuthorizeeID });
		if (count == 0) {
			return true;
		}
		return false;
	}
}
