package com.bitc.jk.xmis.workflow.service;

import java.util.List;
import java.util.Map;

import org.fireflow.engine.definition.WorkflowDefinition;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;
import com.bitc.jk.xmis.workflow.model.AuthorizeInfo;

public interface WorkflowService {
	/**
	 * 签收任务
	 * 
	 * @param workItemId
	 *            任务ID
	 * @param userId
	 *            用户ID
	 * @throws WorkflowException
	 */
	void doClaimTask(String workItemId, String userId) throws WorkflowException;

	/**
	 * 获取工作日历设置Store
	 * 
	 * @param year
	 * @param month
	 * @return
	 */
	String doGetWfCalendarStore(int year, int month);

	/**
	 * 保存工作日历假期信息
	 * 
	 * @param year
	 * @param month
	 * @param holiday
	 */
	void doSaveHoliday(String year, String month, String[] holiday);

	/**
	 * 获取待办列表
	 * 
	 * @param userID
	 * @return
	 */
	String getTodoListByUserID(int userID);

	String getFormUri(String taskId, String userId) throws WorkflowException;

	String getTodoListByUserID(int userID, int size);

	String getTodoListByQuery(StandardQuery query, PageInfo pageInfo);

	String getTodoInfo(String todoId);

	String getWorkItemHistoryListStore(StandardQuery query, PageInfo pageInfo)
			throws Exception;
	String getHaveDoneHistoryListStore(StandardQuery query, PageInfo pageInfo)
	throws Exception;

	String getProcessDefinitionXML(String processId, Integer versionNumber);

	String getProcessDefinitionXMLByInstanceId(String processInstanceId);

	String getHistoryTrace(String processInstanceId);

	List<SimpleEmployee> getCandidate(String expression);

	/**
	 * 拒收工作项
	 * 
	 * @param workItemId
	 * @param userId
	 * @param comments 
	 * @throws WorkflowException
	 */
	void doRejectTask(String workItemId, String userId, String comments)
			throws WorkflowException;
	
	void doWithdrawTask(String workItemId, String userId, String comments)
			throws WorkflowException;

	void doDeploy(WorkflowDefinition workflowDefinition);

	String getDeployedProcessList();
	
	public String getActorAndAssignmentStrategyList(String processInstanceId);
	
	public String getProcessInstanceList(StandardQuery query, PageInfo pageInfo);

	public List getProcessInstanceComb();

	String getTaskListByQuery(StandardQuery query, PageInfo pageInfo);

	Map<String, String> getTileAndTaskIDByUserID(int userID);

	void doClaimTaskList(String workItemId, String userId) throws WorkflowException;

	String getApprovalInfos(String processinstanceId);

	public String getTodoListSQL(StandardQuery query, PageInfo pageInfo)  throws Exception ;
	public String getTodoListSQLByApp(StandardQuery query, PageInfo pageInfo)  throws Exception ;

	public String getAuthorizeList(StandardQuery querys, PageInfo pageInfo) throws Exception;
	
	public String getCurrentActorName(String processInstanceId);

	public boolean doCancelAuthorizeById(String id);

	public boolean doAddAuthorize(AuthorizeInfo info);
}
