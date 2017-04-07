//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\ProjectDAO.java

package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.ProjectDevicePurchaseStatus;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.ProjectSchedulePlan;
import com.bitc.jk.xmis.model.ProjectStatus;
import com.bitc.jk.xmis.model.ProjectTaskMember;
import com.bitc.jk.xmis.model.ProjectTaskObj;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface ProjectDAO {

	/**
	 * @param projectID
	 * @return com.bitc.jk.xmis.model.ProjectObj
	 * @roseuid 4C19D26B005D
	 */
	public ProjectObj getProjectInfoByID(String projectID);

	/**
	 * @param projectID
	 * @return List
	 * @roseuid 4C1AEE9601D4
	 */
	public List<DeviceObj> getDeviceListByID(String projectID);

	/**
	 * @param deviceList
	 * @roseuid 4C1AEF9F03B9
	 */
	public void doSaveDevice(DeviceObj device);

	/**
	 * @param deviceObj
	 * @roseuid 4C1AEFC80251
	 */
	public void doUpdateDevice(DeviceObj deviceObj);

	/**
	 * @param projectID
	 * @param deviceID
	 * @roseuid 4C1AEFCC008C
	 */
	public void doDeleteDeviceList(int projectID, int deviceID);

	/**
	 * @param projectObj
	 * @roseuid 4C1B36BA037A
	 */
	public void doSaveProjectInfo(ProjectObj projectObj);

	/**
	 * @param projectObj
	 * @roseuid 4C1EB68A009C
	 */
	public void doUpdateProjectInfo(ProjectObj projectObj);

	/**
	 * @param projectTaskObj
	 * @roseuid 4C1B39E0005D
	 */
	public void doSaveProjectTask(ProjectTaskObj projectTaskObj);

	/**
	 * @param projectTaskObj
	 * @roseuid 4C1EB67A0242
	 */
	public void doUpdateProjectTask(ProjectTaskObj projectTaskObj);

	public List<ProjectObj> getProjectList(List<QueryParam> params,
			PageInfo pageInfo);

	public int getProjectCount(List<QueryParam> params, PageInfo pageInfo);

	public void doSaveProjectTasks(List<ProjectTaskObj> list);

	public void doDeleteProjectTaskByProjectID(String projectID);

	public List<ProjectObj> getProjectListByInkling(List<QueryParam> params,
			PageInfo pageInfo);

	public int getProjectCountByInkling(List<QueryParam> params,
			PageInfo pageInfo);

	/**
	 * 查询项目
	 * 
	 * @param query
	 * @param pageInfo
	 * @return
	 */
	public List<ProjectObj> getProjectListByQuery(StandardQuery query,
			PageInfo pageInfo);

	/**
	 * 查询项目数量
	 * 
	 * @param query
	 * @param pageInfo
	 * @return
	 */
	public int getProjectCountByQuery(StandardQuery query, PageInfo pageInfo);

	/**
	 * 根据项目编码查询项目
	 * 
	 * @param projectCode
	 * @return
	 */
	public ProjectObj getProjectByProjectCode(String projectCode);

	/**
	 * 删除设备
	 * 
	 * @param deviceID
	 */
	public void doDeleteDevice(String deviceID);

	/**
	 * 保存项目任务成员
	 * 
	 * @param member
	 */
	public void doSaveProjectTaskMember(ProjectTaskMember member);

	/**
	 * 更新合同的项目ID
	 * 
	 * @param mainContractID
	 * @param projectID
	 */
	public void updateProjectID(int mainContractID, String projectID);

	/**
	 * 根据项目ID查询项目任务
	 * 
	 * @param projectID
	 * @return
	 */
	public List<ProjectTaskObj> getTaskListByProjectID(String projectID);

	/**
	 * 保存项目设备采购状态
	 * 
	 * @param purchaseStatus
	 */
	public void saveProjectDevicePurchaseStatus(
			ProjectDevicePurchaseStatus purchaseStatus);

	public List<ProjectStatus> getProjectStatus(StandardQuery query);

	/**
	 * 根据采购申请明细ID查询项目设备状态
	 * 
	 * @param id
	 * @return
	 */
	public ProjectDevicePurchaseStatus getProjectDevicePurchaseStatusByPrdId(
			String id);

	public ProjectStatus getProjectStatus(int id);

	public int doSaveProjectStatus(ProjectStatus status);

	public List<ProjectSchedulePlan> getProjectSchedulePlanByQuery(
			StandardQuery query, PageInfo pageInfo);

	public void saveProjectSchedulePlan(ProjectSchedulePlan plan);

	public double getReminderAmt(String projectId);

	public ProjectSchedulePlan getProjectSchedulePlan(String id);

	public void deleteProjectSchedulePlanByProjectId(String projectId);

	public ProjectSchedulePlan getProjectSchedulePlanByStatusId(
			String projectId, Integer statusId);

	public void deleteProjectSchedulePlan(String id);

	public ProjectTaskObj getProjectTask(String id);

	public List getInterCheckList(String processInstanceId);

	public List getRelationChildContractCodeByID(String contractID);

	public List getRelationProjectID(Integer contractId);

	public List getProjectQualificationName(StandardQuery query, PageInfo pageInfo) throws Exception;

}
