//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\ProjectService.java

package com.bitc.jk.xmis.service;

import java.io.IOException;
import java.io.InputStream;
import java.text.ParseException;
import java.util.List;

import org.codehaus.jackson.JsonParseException;
import org.codehaus.jackson.map.JsonMappingException;
import org.fireflow.engine.EngineException;
import org.fireflow.kernel.KernelException;

import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.ProjectSchedulePlan;
import com.bitc.jk.xmis.model.ProjectStatus;
import com.bitc.jk.xmis.model.ProjectTaskObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;

/**
 * @author Bom Wu
 * 
 */
public interface ProjectService {

	/**
	 * 获取项目信息
	 * 
	 * @param projectID
	 * @return com.bitc.jk.xmis.model.ProjectObj
	 * @roseuid 4C19D2710280
	 */
	public ProjectObj getProjectInfoByID(String projectID);

	/**
	 * 通过项目ID获取设备列表
	 * 
	 * @param projectID
	 * @return List
	 * @roseuid 4C1AEE58036B
	 */
	public String getDeviceListStoreByID(String projectID);

	/**
	 * 保存设备信息
	 * 
	 * @param device
	 */
	public void doSaveDevice(DeviceObj device);

	/**
	 * 删除指定ID的项目设备
	 * 
	 * @param deviceID
	 */
	public void doDeleteDevice(String deviceID);

	/**
	 * 保存项目信息
	 * 
	 * @param projectObj
	 * @roseuid 4C1B36860232
	 */
	public void doSaveProjectInfo(ProjectObj projectObj);

	/**
	 * 更新项目信息
	 * 
	 * @param projectObj
	 * @roseuid 4C1EB65401C5
	 */
	public void doUpdateProjectInfo(ProjectObj projectObj);

	/**
	 * 保存项目任务信息
	 * @param ptask 
	 * @param memberJson
	 * @throws Exception 
	 */
	public void doAssignProjectTask(ProjectTaskObj ptask, String memberJson) throws Exception;

	public String getProjectListStore(List<QueryParam> params, PageInfo pageInfo);

	public String getProjectListStoreByInkling(List<QueryParam> params,
			PageInfo pageInfo);

	public String getProjectListStoreByQuery(StandardQuery query,
			PageInfo pageInfo);

	public void doStartProject(ProjectObj project, SimpleEmployee emp,
			boolean assignToDept, String taskDeptId, String ts,
			String deptTaskDesc, String workItemId) throws Exception;

	public void doAcceptProjectTask(SimpleEmployee emp, String workItemId,
			String projectTaskId, String comment) throws WorkflowException;

	public String getProjectTaskByProjectID(String projectID);

	public String getProjectStatus(StandardQuery query);

	public void doUpdateProjectStatus(String projectId, String status,
			String statusDesc, String actualStartDate) throws Exception;

	public String getProjectStatusByProjectId(String projectId,
			StandardQuery query);

	public int doImportDeviceList(String projectId, InputStream inputStream,
			SimpleEmployee entryEmp);

	public String getProjectSchedulePlanByQuery(StandardQuery query,
			PageInfo pageInfo);

	public void doSaveProjectSchedulePlan(String projectId, String plans,
			SimpleEmployee emp) throws ParseException;

	public int doSaveProjectStatus(ProjectStatus status);

	public double getReminderAmt(String projectId);

	public void doUpdateProjectSchedulePlan(ProjectSchedulePlan plan);

	public String getDefaultProjectStatus(StandardQuery query);

	public void doDirectorAssignTask(String workItemId, ProjectObj project,
			EmployeeObj emp, String ts, String comment)
			throws WorkflowException;

	public void doDeleteProjectSchedulePlan(String id);

	/** 变更执行部门
	 * @throws WorkflowException 
	 * @throws KernelException 
	 * @throws EngineException 
	 */
	public void doUpdateProjectExecutiveDept(String projectID,
			int excecutiveDeptID,String executiveDeptChangeReason,
			int managerID,String managerChangeReason,String isChangeDept,String isChangeManager) throws WorkflowException, EngineException, KernelException;

	public void doAcceptProjectExecutiveDept(String projectID,int excecutiveDeptID,String workItemId ) throws WorkflowException;

	public void doAcceptProjectManager(String projectID, int managerID,
			String workItemId) throws WorkflowException;

	public void doStartInterCheckToEngineer(String projectID, String checkDeptID,String comment) throws WorkflowException, EngineException, KernelException;
	
	public void doManagerRefuseCheck(String projectID, 
			String workItemId, String comment) throws WorkflowException, EngineException, KernelException;

	public void doCheckDeptAssignTask(ProjectTaskObj ptask,
			String memberJson, 
			String workItemId,String comment, String divisionManagerID) throws JsonParseException, JsonMappingException, IOException, EngineException, KernelException, WorkflowException;

	public void doGoOnInterCheckToEngineer(String workItemId,String comment) throws WorkflowException;

	public void doTellDeptManager(String comment,
			String workItemId,String isApprove, String checkManagerID)  throws WorkflowException, EngineException, KernelException;
	
	public void doTellDivisionDeptManager(String comment,
			String workItemId,String isApprove,String projectID)  throws WorkflowException, EngineException, KernelException;

	public String getInterCheckList(String processInstanceId);

	public void doBatchAcceptProjectExecutiveDept(String workItemId) throws WorkflowException;

	public void doBatchAcceptProjectManager(String workItemId) throws WorkflowException;
	
	public void doBatchAcceptProjectTask(SimpleEmployee emp, String workItemId,
			String comment) throws WorkflowException;

	public void doUpdateProjectExecutiveDeptByReturn( 
			int executiveDeptID, String executiveDeptChangeReason,
			int managerID, String managerChangeReason,  String workItemId) throws WorkflowException;

	public String getRelationChildContractCodeByID(String contractID);

	public String getRelationProjectID(Integer contractId);

	 String getProjectQualificationName(StandardQuery query, PageInfo pageInfo)throws Exception;

}
