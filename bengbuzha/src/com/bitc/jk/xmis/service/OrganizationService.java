//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\OrganizationService.java

package com.bitc.jk.xmis.service;

import java.util.List;
import java.util.Map;

import org.dom4j.Document;

import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.DepartmentObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleDeptObj;
import com.bitc.jk.xmis.model.T_OnWatch_schedule;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.action.param.OrgTreeParam;
import com.bitc.jk.xmis.web.response.TreeLoaderResponse;
import com.bitc.jk.xmis.web.response.TreeNode;

public interface OrganizationService {

	public void setWebDir(String aWebDri);

	public void setWebPath(String aWebPath);

	/**
	 * @param companyID
	 * @return List
	 * @roseuid 4C1F00AC007D
	 */
	public void buildDepartmentTree(EmployeeObj userObj) throws Exception;

	/**
	 * @param empID
	 *            ;int
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C1F00DA0242
	 */
	public EmployeeObj getEmployeeInfo(int empID);

	/**
	 * @param companyObj
	 * @roseuid 4C1F010402CE
	 */
	public void doSaveCompanyInfo(CompanyObj companyObj);

	/**
	 * @param departmentObj
	 * @roseuid 4C1F01670119
	 */
	public void doSaveDepartmentInfo(DepartmentObj departmentObj);

	/**
	 * @param employeeObj
	 * @param compID
	 * @param positionEdit
	 * @roseuid 4C1F01940213
	 */
	public void doSaveEmployeeInfo(EmployeeObj employeeObj, EmployeeObj userDNA);

	public String getProjectListByDiv(int divisionID);

	public Document getEmpTotalSummary(int departmentID, PageInfo pageInfo);

	public int doChangePassword(int userID, String password, int updateID);

	public int doSaveUserPhoto(int userID, String imgName, int updateID);

	public int doEditEducation(String oldEduName, EmployeeObj emp, int updateID);

	public int doCreateNewUser(String deptID, String deptName, String empName,
			EmployeeObj operator);

	public int doCreateNewBatch(Map deptMap, Map empMap, Map empDeptMapping,
			EmployeeObj operator) throws Exception;

	public String doSaveAllOperation(Map<String, String> deptMap,
			Map<String, String> emplMap, List<List<String>> mappingList,
			List<List<String>> dragList, Map<String, String> changeEmplName,
			Map<String, String> changeDeptName, EmployeeObj operator)
			throws Exception;

	public int doDisableTreeNode(String itemNode, EmployeeObj operator)
			throws Exception;

	public int doChangeLunchAddress(int newAddress, int empID, int operatorID);

	public String getLunchAddressListByDiv(int divID);

	public int doBookLunchAddress();

	public String getCompanyStore(int companyID);

	public String getOrgTree(String node, int id);

	public String getOrganizationTree(String node, int type);

	public String getEmployee(int id);

	public String getEmployeeList(String ID, int start, int size);

	public String getPoliticsStatusTotalSummary();

	public String getPositionTotalSummary();

	public boolean checkLoginNameExist(String loginName, int userid);

	public String updatePost(int id, int postId);

	public TreeLoaderResponse getOrganizationTree(OrgTreeParam param);

	public List getEmployeeInfoByDeptID(int deptNo);

	public List getLeaderInfo();

	public List getEmployeeInfoByQuery(StandardQuery query);

	public void test();

	public String getEmpListByName(String compID, String name, String isAdmin);

	/**
	 * 获取通讯录自定义组的树
	 * */
	public String getCustomGroupTree(String node, String groupID);

	/**
	 * 获取通讯录自定义组树 当前节点的子节点
	 * */
	public List<TreeNode> getCustomChildrenTreeNodes(String node, String groupID);

	/**
	 * 删除通讯录自定义组
	 * */
	public boolean doDeleteCustomGroup(String node, String groupid,
			String father_groupid, String type);

	/**
	 * 保存通讯录自定义组
	 * */
	public void doSaveAllOfCustomGroup(Map<String, String> changeGroupName,
			Map<String, String> newGroup, List<List<String>> relationList,
			List<List<String>> ddRelationList, EmployeeObj emp)
			throws Exception;

	/**
	 * 通过empID获取人员信息
	 * */
	public Map getEmpInfo(int empID);

	/**
	 * 通过父部门ID获取子部门
	 * */
	public List getDeptsByFatherDeptID(int deptID);

	/**
	 * 通过员工姓名获取该员工所在公司信息
	 * */
	public String getCompStoreByEmpName(String empName, PageInfo pageInfo);

	/**
	 * 根据人员ID 得到此节点的路径
	 * 
	 * @param empID
	 * @return
	 */
	public String getEmpNodePath(int empID);

	public List getDisabledReasonComb();

	public String doUpdateDisableReasonAndDate(int empId, String reason,
			String disable);

	public void doUpdatePostCard(int empId, Boolean isFront);

	/**
	 * 获取当前登录人的自定义组列表
	 * */
	public String getCustomGroupList(EmployeeObj empObj);

	/**
	 * 查找empName所在的组（当前登录人所在的组）
	 * */
	public String getCustomGroupListByEmpname(EmployeeObj empObj, String empName);

	/**
	 * 发送短信
	 * */
	public boolean sendMessage(int userid, String empids, String text);


	/**
	 * 保存用户消息提示习惯（下次是否提示）
	 * */
	public void saveUserTopicMessageHabit(String tipID, int userID,
			String[] messageIDs);

	/**
	 * 获取消息
	 * */
	public String getMessageByUserIDAndTopID(String topicID, int userID);

	/**
	 * 发送公告
	 * */
	public boolean sendAnnouncementMessage(int userid, String text,
			String topic_type);

	/**
	 * 发送个人消息
	 * */
	public void sendPersonalMessage(int userid, String text, String topic_type,
			String empIds);

	/**
	 * 获取用户已接收的用户列表
	 * */
	// public String getHaveReceivedMessageByUseId(int userid);
	
	public SimpleDeptObj getSimpleDeptObj(int deptID) ;
	public EmployeeObj getEmployeeById(int id);
	
	//2013-11-06 zentao代码注释
/*	public List getZentaoDataFromEmployee();
	public void doUpdateZentaoPwd(int userIDInt, String password);
	public void doInsertZentaoData();
	*/

	public List getPersonByDeptID(String detpId);

	public List getAllDivisionDept(int companyID);
	
	public List getSendMessageAfterProcessInstance(String processInstanceId);

	public void doSaveDutyPlan(T_OnWatch_schedule tOsc);

	public void deleteDutyPlan(String id);

	public String getDutyPlanList(PageInfo pageInfo, String department,
			String onWatchDate);

	public String getSexCount();

	public String getSexList(String sex,PageInfo pageInfo);

	public String getZzmmCount();

	public String getZzmmList(String zzmm, PageInfo pageInfo);

	public String getAgeCount();

	public String getAgeList(String age, PageInfo pageInfo);

	public String getDutyCount();

	public String getDutyList(String duty, PageInfo pageInfo);

	public String getTitlesCount();

	public String getTitlesList(String titles, PageInfo pageInfo);

	public String getJsjbCount();

	public String getJsjbList(String jsjb, PageInfo pageInfo);

	public String getWhcdCount();

	public String getWhcdList(String whcd, PageInfo pageInfo);

	public String getZgsfCount();

	public String getZgsfList(String zgsf, PageInfo pageInfo);

	public String getZzygList(String zzyg, PageInfo pageInfo);

	public String getZzygCount();

	public String getDeptManagerList();

}
