//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\OrganizationDAO.java

package com.bitc.jk.xmis.dao;

import java.util.List;
import java.util.Map;

import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.DepartmentObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleDeptObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.SupplierInformation;
import com.bitc.jk.xmis.model.crm.Address;
import com.bitc.jk.xmis.model.sys.DisabledReason;
import com.bitc.jk.xmis.model.sys.PoliticalStatus;
import com.bitc.jk.xmis.model.sys.Post;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.action.param.OrgTreeParam;
import com.bitc.jk.xmis.web.response.TreeNode;

public interface OrganizationDAO {

	/**
	 * @param loginName
	 * @param password
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C197A1A029F
	 */
	public EmployeeObj userValidete(String loginName, String password);

	/**
	 * @param userID
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C197DF9006D
	 */
//	public EmployeeObj getUserByID(int userID);

	/**
	 * @param companyID
	 * @return List
	 * @roseuid 4C1F023A0399
	 */
	public List getDepartmentTree(int divisionID);

	/**
	 * @param empID
	 *            ;int
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C1F023A03B9
	 */
	public Map getEmployeeInfo(int empID);

	/**
	 * @param companyObj
	 * @roseuid 4C1F023B000F
	 */
	public void doSaveCompanyInfo(CompanyObj companyObj);

	/**
	 * @param departmentObj
	 * @roseuid 4C1F023B002E
	 */
	public void doSaveDepartmentInfo(DepartmentObj departmentObj);

	/**
	 * @param employeeObj
	 * @param compID
	 * @roseuid 4C1F023B004E
	 */
	public void doSaveEmployeeInfo(EmployeeObj employeeObj, EmployeeObj userDNA);

	/**
	 * @param id
	 * @param type
	 * @return List
	 * @roseuid 4C1F023B005D
	 */
	public List getProjectListByID(int id, String type);

	public List getTotalSummary(String type, int id, PageInfo pageInfo);

	public int doChangePassword(int userID, String password, int updateID);

	public int doSaveUserPhoto(int userID, String imgName, int updateID);

	public List getPoliticalStatusList();

	public List getLunchAddressList();

	public List getEducationLevelList(int userID);

	public int doUpdateEdu(EmployeeObj emp, int updateID);

	public int doCreateNewEdu(EmployeeObj emp, int updateID);

	public Map doCreateDeptBatch(int divID, Map deptMap, int updateID);

	public int doCreateDept(int divID, String deptName, int updateID);

	public int doCreateUser(int deptID, String userName, int updateID);

	public Map<String, String> doCreateDeptBatch(Map<String, String> deptMap,
			int updateID);

	public void doDisableEmployee(int empID, int operatorID);

	public void doDisableDepartment(int deptID, int operatorID);

	public int doUpdateLunchAddress(int newAddress, int empID, int operatorID);

	public List getLunchAddressListByDiv(int divID);

	public int doBookLunchAddress();

	public List getCompanyList();

	public List getDeptListByCompanyID(int companyID);

	public List getUserRightOnFunctionLevel(int userID);

	public List getSecrityDataScope(String url, int userID);

	public List getUserRightByDefault();

	public CompanySimpleObj getCompanySimpleObj(int companyID);

	public List getDeptListByID(int id, String type);

	public List getBizloigcDataList(String paramStr, String type);

	public List getOrgTree(int type, int id);

	public List getOrganizationTree(int type, int id);

	public EmployeeObj getEmployee(int id);

	public List getEmployeeList(String type, int ID);

	public List<PoliticalStatus> getPoliticsStatusTotalSummary();

	public List<Post> getPositionTotalSummary();

	public EmployeeObj checkLoginNameExist(String loginName, int userid);

	public void doCreateEmpBatch(Map empMap, Map empDeptMapping, int updateID)
			throws Exception;

	public Map<String, String> doCreateEmplBatch(Map<String, String> emplMap,
			Map<String, String> mappingMap, int updateID) throws Exception;

	public void doUpdateDeptBatch(Map<String, String> mappingDeptMap,
			Map<String, String> mappingCompMap);

	//
	public void doUpdateEmplBatch(Map<String, String> mappingEmplMap,
			int updateID);

	public void doUpdateDeptBelongToDeptBatch(
			Map<String, String> dragEmplToDeptMap,
			Map<String, String> dragDeptToDeptMap,
			Map<String, String> dragCompToCompMap,
			Map<String, String> dragCompToDeptMap,
			Map<String, String> dragDeptToCompMap);

	public void doUpdateEmpNameBatch(Map<String, String> changeEmplName);

	public void doUpdateDeptNameBatch(Map<String, String> changeDeptName);

	public int getUserNumbersByDeptID(int deptID);

	public int getUserNumbersByCompID(int CompID);

	public String updatePost(int id, int postId);

	public void test();

	public List<Object[]> getOrgTree(OrgTreeParam param);

	/*
	 * 得到用户所在部门访问业务数据的权限
	 */
	public boolean getDeptOfUserLimit(int id);

	public List<Integer> getUserRoles(int empId);

	public List getEmpInfoByDeptID(int deptt);

	public List getLeaderInfo();

	public List getEmployeeInfoByQuery(StandardQuery query);

	public List<SimpleEmployee> getEmpsByPost(int deptId, int postId);

	public List<SimpleEmployee> getEmpsByRole(int roleId);

	public List<SimpleEmployee> getEmpsByRoleAndDept(int roleId, int deptId);

	public SimpleEmployee getSimpleEmployee(int userID);

	public void doSaveAddressInfo(Address address);

	// public List getDeptNodePath(String deptID);
	//
	// public List getEmpNodePath(String empID);

	public List getEmpListByName(String compID, String name);

	public SimpleDeptObj getSimpleDeptObj(int deptID);

	/**
	 * 获取通讯录自定组的树
	 * */
	public List getCustomGroupTree(String id);

	/**
	 * 删除通讯录自定组的组成员
	 * */
	public void deDeleteEmpOrDeptOrGroupOfCustomGroup(String groupid,
			String father_groupid, String type);

	/**
	 * 查看通讯录自定组的指定组被使用的次数
	 * */
	public int getGroupBeUsedNum(String groupid);

	/**
	 * 批量处理通讯录自定义组的重命名操作
	 * */
	public void updateCustomGroupNameBatch(Map<String, String> changeGroupName);

	/**
	 * 批量处理通讯录自定组的新增组操作
	 * */
	public Map<String, String> doAddCustomGroupBatch(
			Map<String, String> newGroup);

	/**
	 * 批量处理通讯里自定义组的组关系的修改操作（修改已存在的组关系的father_groupid）
	 * */
	public void doUpdateGroupRelationMappingBatch(
			Map<String, String> groupMapping);

	/**
	 * 批量处理通讯录自定义组的 新增人员-组，部门-组，组-组关系的保存操作
	 * */
	public void dosaveDDRelationBatch(List<TreeNode> empDDGroupList,
			List<TreeNode> deptDDGroupList, List<TreeNode> groupDDGroupList);

	/**
	 * 通过empID获取人员信息
	 * */
	public Map getEmpInfo(int empID);

	/**
	 * 通过父部门ID获取子部门
	 * */
	public List getDeptListByFatherDeptID(int deptID);

	/**
	 * 通过员工姓名获取该员工所在公司的公司信息
	 * */
	public List getCompInfoByEmpName(String empName, PageInfo pageInfo);

	public List getPostListByEmpID(int userID);

	/**
	 * 获取自定义组列表
	 * 
	 * @param String
	 *            father_groupid
	 * */
	public List getCustomGroupList(String father_groupid);

	/**
	 * 获取自定义组列表
	 * 
	 * @param String
	 *            groupid
	 * */
	public List getCustomGroupListByGroupid(String groupid);

	/**
	 * 根据人员ID 得到他所在的部门ID
	 */
	public int getDeptIDByUserID(int userID);

	/**
	 * 根据人员ID 得到事业部门ID
	 */
	public int getDivisionDeptIDByDeptID(int deptID);

	/**
	 * 根据部门ID 得到父部门ID
	 * 
	 * @param deptID
	 * @return
	 */
	public int getFdeptIDByDeptID(int deptID);
	public boolean getFdeptIDIsExistByDeptID(final int deptId);
	/**
	 * 根据部门ID 得到公司ID
	 * 
	 * @param fdeptID
	 * @return
	 */
	public int getCompIDByDeptID(int fdeptID);

	/**
	 * 判断自定义组（组或者组成员）是否存在
	 * */
	public int isExsitCustomGroup(String groupid, String father_groupid,
			String type);

	/**
	 * 根据公司ID 姓名查询所有人 包括被禁用的
	 * 
	 * @param compID
	 * @param name
	 * @return
	 */
	public List getEmpListByNameAdmin(String compID, String name);

	/**
	 * 根据人员id得到 此人的路径 人ID-部门ID-上级部门ID-...-上级部门ID-部门ID-公司ID
	 * 
	 * @param userid
	 * @return
	 */
	public String getEmpPathId(int userid);

	public String getDeptNameByDeptId(int deptId);

	/**
	 * 获取最大ID
	 * 
	 * @param columnName
	 * @return
	 */
	public int getMaxId(String sql);

	/**
	 * 保存其它部门
	 * 
	 * @param compID
	 * @param deptID
	 * @param updateID
	 */
	public void doCreateOtherDept(final int compID, final int deptID,
			final int updateID);

	/**
	 * 根据人员ID，禁用原因的ID(string) 查询此人的禁用的原因
	 * 
	 * @param id
	 * @param disabledReason
	 * @return
	 */
	public DisabledReason getDisabledReason(String id);

	public List getDisabledReasonComb();

	public void updateDisableReason(int empId, String reason, String disable);

	public int doSavePostCardFrontPic(int userID, String postCardFrontName,
			int updateID);

	public int doSavePostCardBackPic(int userID, String postCardBackName,
			int updateID);

	public int doUpdatePostCard(int empId, Boolean isFront);

	public DepartmentObj getDepartmentById(int departmentID);

	public void saveUserTopicMessageHabit(String tipID, int userID,
			String messageID);

	public List getMessageByUserIDAndTopID(String topicID, int userID);

	public int doInsertMessageDefinition(String message_id, int userid,
			String text, String topic_type);

	public int doInsertMessageRecord(String message_id);

	public void doInsertPersonMessageDefinition(final String message_id,
			final int userid, final String text, final String topic_type);

	public void savePersonalMessageRecord(final String message_id,
			final int empid);
	/**
	 * 获取用户已接收的消息列表
	 * */
	// public List getHaveReceivedMessageByUseId(int userid);
	public Boolean getIsExistByDeptId(int deptId);
	/**
	 * 判断中水科技公司下  是否直属部门
	 * */
	public Boolean getIsDirectByDeptId(int deptId);
	
	public List getZentaoDataFromEmployee();

	public List getPersonByDeptID(int deptId);

	public List getEmpsByRoleAndDivisionID(int roleId, int divisionId);

	public Boolean getIsExistRole(int roleId,int empId);

	public List getAllDivisionDept(int companyID);

	public boolean isHasExistAttendanceWarnInfo(String msg, String nowdate);

	public List getSendMessageAfterProcessInstance(String processInstanceId);

	public int doSavePostMaterials(String materialsID, String imgName);

}
