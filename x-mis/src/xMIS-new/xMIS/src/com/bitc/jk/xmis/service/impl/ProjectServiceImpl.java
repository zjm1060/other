//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\Impl\\ProjectServiceImpl.java

package com.bitc.jk.xmis.service.impl;

import java.io.IOException;
import java.io.InputStream;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Map;

import jxl.Cell;
import jxl.Sheet;
import jxl.Workbook;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.codehaus.jackson.JsonParseException;
import org.codehaus.jackson.map.JsonMappingException;
import org.codehaus.jackson.type.TypeReference;
import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.IWorkItem;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.RuntimeContext;
import org.fireflow.engine.impl.TaskInstance;
import org.fireflow.kernel.KernelException;

import com.bitc.jk.xmis.dao.ContractDAO;
import com.bitc.jk.xmis.dao.DeviceDAO;
import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.dao.ProjectDAO;
import com.bitc.jk.xmis.model.DepartmentObj;
import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.ProjectSchedulePlan;
import com.bitc.jk.xmis.model.ProjectStatus;
import com.bitc.jk.xmis.model.ProjectTaskMember;
import com.bitc.jk.xmis.model.ProjectTaskObj;
import com.bitc.jk.xmis.model.ProjectTaskStatus;
import com.bitc.jk.xmis.model.SimpleDeptObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.service.ProjectService;
import com.bitc.jk.xmis.util.BeanUtils;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.JSONUtils;
import com.bitc.jk.xmis.util.JacksonUtil;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.TemplateUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.WorkflowConstants;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.dao.TodoDAO;
import com.bitc.jk.xmis.workflow.dao.WorkflowDAO;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;
import com.bitc.jk.xmis.workflow.model.ProcessWithKeyword;
import com.bitc.jk.xmis.workflow.service.WorkflowService;
import com.bitc.jk.xmis.workflow.util.WorkflowUtil;

public class ProjectServiceImpl implements ProjectService {
	private static Logger logger = Logger.getLogger(ProjectServiceImpl.class);
	private ProjectDAO projectDAO;
	private RuntimeContext workflowContext;
	private DeviceDAO deviceDAO;
	private ContractDAO contractDAO;
	private OrganizationDAO organizationDAO;
	private WorkflowDAO workflowDAO;
	private TodoDAO todoDAO;
	
	private WorkflowService workflowService;

	public WorkflowService getWorkflowService() {
		return workflowService;
	}

	public void setWorkflowService(WorkflowService workflowService) {
		this.workflowService = workflowService;
	}

	public TodoDAO getTodoDAO() {
		return todoDAO;
	}

	public void setTodoDAO(TodoDAO todoDAO) {
		this.todoDAO = todoDAO;
	}

	public WorkflowDAO getWorkflowDAO() {
		return workflowDAO;
	}

	public void setWorkflowDAO(WorkflowDAO workflowDAO) {
		this.workflowDAO = workflowDAO;
	}

	/**
	 * @roseuid 4C1F041F004E
	 */
	public ProjectServiceImpl() {

	}

	public ProjectDAO getProjectDAO() {
		return projectDAO;
	}

	public void setProjectDAO(ProjectDAO aProjectDAO) {
		projectDAO = aProjectDAO;
	}

	public ProjectObj getProjectInfoByID(String projectID) {
		return projectDAO.getProjectInfoByID(projectID);
	}

	/**
	 * @param projectID
	 * @return List
	 * @roseuid 4C1F041F0242
	 */
	public String getDeviceListStoreByID(String projectID) {
		List<DeviceObj> list = projectDAO.getDeviceListByID(projectID);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getProjectTaskByProjectID(String projectID) {
		List<ProjectTaskObj> list = projectDAO
				.getTaskListByProjectID(projectID);
		return WebUtil.buildJsonStoreByList(list);
	}

	/**
	 * @param deviceList
	 * @roseuid 4C1F041F0280
	 */
	public void doSaveDevice(DeviceObj device) {
		if (StringUtils.isEmpty(device.getId())) {
			projectDAO.doSaveDevice(device);
			deviceDAO.saveDeviceInfo(device);
		} else {
			projectDAO.doUpdateDevice(device);
		}
	}

	public void doDeleteDevice(String deviceID) {
		projectDAO.doDeleteDevice(deviceID);
	}

	/**
	 * @param projectObj
	 * @roseuid 4C1F041F036B
	 */
	public void doSaveProjectInfo(ProjectObj projectObj) {
		projectDAO.doSaveProjectInfo(projectObj);
	}

	/**
	 * @param projectObj
	 * @roseuid 4C1F041F03A9
	 */
	public void doUpdateProjectInfo(ProjectObj projectObj) {

	}

	/**
	 * @param projectTaskObj
	 * @throws Exception
	 * @roseuid 4C1F0420000F
	 */
	public void doAssignProjectTask(ProjectTaskObj ptask, String memberJson)
			throws Exception {
		// SimpleEmployee employee = WebUtil.getCurrentEmployee();
		EmployeeObj employee = WebUtil.getCurrentEmployee();
		DepartmentObj dept = null;
		ptask.setAssigedToDept(dept);
		ptask.setGrantor(employee);
		ptask.setLastModifier(employee);
		ptask.setLastModified(DateUtil.getCurrentDateTime());
		ProjectTaskStatus status = new ProjectTaskStatus();
		status.setId(ProjectTaskStatus.NEW);
		ptask.setStatus(status);
		projectDAO.doSaveProjectTask(ptask);
		List<ProjectTaskMember> members = JacksonUtil.readValue(memberJson,
				new TypeReference<List<ProjectTaskMember>>() {
				});
		List<String> ids = new ArrayList<String>();
		for (ProjectTaskMember member : members) {
			member.setProjectTaskId(ptask.getTaskID());
			projectDAO.doSaveProjectTaskMember(member);
			ids.add(String.valueOf(member.getMember().getUserID()));
		}
		ProjectObj project = projectDAO
				.getProjectInfoByID(ptask.getProjectID());
		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		IProcessInstance processInstance = WorkflowUtil.startProcess(
				workflowSession, WorkflowConstants.WF_PROJECT_TASK_ASSIGN,
				String.valueOf(employee.getUserID()));
		WorkflowUtil.addProcessInstanceVariable(processInstance, "projectID",
				ptask.getProjectID());
		WorkflowUtil.addProcessInstanceVariable(processInstance, "projectCode",
				project.getProjectCode());
		WorkflowUtil.addProcessInstanceVariable(processInstance, "projectName",
				project.getProjectName());
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"projectTaskId", ptask.getTaskID());
		WorkflowUtil.specifyActor(workflowSession, ids, "__assignee");
		WorkflowUtil.completeFirstTask(workflowSession, processInstance,
				String.valueOf(employee.getUserID()), null);
		String appendName = " 项目编码 " + project.getProjectCode() + " | 项目名称 "
				+ project.getProjectName();
		this.doUpdateProcessName(processInstance, appendName);
	}

	private void saveProjectTask(ProjectObj project, JSONArray tasks,
			SimpleEmployee grantor) {
		ProjectTaskObj ptask;
		ProjectTaskMember member;
		EmployeeObj currentEmployee = WebUtil.getCurrentEmployee();
		for (int i = 0; i < tasks.size(); i++) {
			JSONObject object = tasks.getJSONObject(i);

			ptask = new ProjectTaskObj();
			ptask.setTaskName(object.getString("taskDesc"));
			DepartmentObj dept = new DepartmentObj();
			dept.setDepartmentID(Integer.parseInt(object.getString("deptId")));
			ptask.setAssigedToDept(dept);
			ptask.setGrantor(grantor);
			ProjectTaskStatus status = new ProjectTaskStatus();
			status.setId(ProjectTaskStatus.NEW);
			ptask.setStatus(status);
			ptask.setProjectID(project.getProjectID());
			ptask.setLastModified(DateUtil.getCurrentDateTime());
			ptask.setLastModifier(currentEmployee);
			projectDAO.doSaveProjectTask(ptask);

			member = new ProjectTaskMember();
			SimpleEmployee emp = new SimpleEmployee(Integer.parseInt(object
					.getString("userId")));
			member.setMember(emp);
			member.setProjectTaskId(ptask.getTaskID());
			member.setTaskDesc(object.getString("taskDesc"));
			int role = object.getInt("role");
			member.setRole(role);
			if (role == 1) {
				project.setManager(emp);
			}
			ptask.setLastModified(DateUtil.getCurrentDateTime());
			ptask.setLastModifier(currentEmployee);
			projectDAO.doSaveProjectTaskMember(member);
		}
	}

	public String getProjectListStore(List<QueryParam> params, PageInfo pageInfo) {
		logger.info("--->  getProjectListStore ");
		List<ProjectObj> projects = projectDAO.getProjectList(params, pageInfo);
		int totalCount = projectDAO.getProjectCount(params, pageInfo);
		return WebUtil.buildJsonStoreByList(projects, totalCount);
	}

	public String getProjectListStoreByInkling(List<QueryParam> params,
			PageInfo pageInfo) {
		logger.info("--->  getProjectListStoreByInkling ");
		List<ProjectObj> projects = projectDAO.getProjectListByInkling(params,
				pageInfo);
		int totalCount = projectDAO.getProjectCountByInkling(params, pageInfo);
		return WebUtil.buildJsonStoreByList(projects, totalCount);
	}

	public String getProjectListStoreByQuery(StandardQuery query,
			PageInfo pageInfo) {
		List<ProjectObj> projects = projectDAO.getProjectListByQuery(query,
				pageInfo);
		// initGLInfo(projects);
		int totalCount = projectDAO.getProjectCountByQuery(query, pageInfo);
		return WebUtil.buildJsonStoreByList(projects, totalCount);
	}

	public void doStartProject(ProjectObj project, SimpleEmployee emp,
			boolean assignToDept, String taskDeptId, String ts,
			String deptTaskDesc, String workItemId) throws Exception {
		ProjectObj p = projectDAO.getProjectInfoByID(project.getProjectID());
		p.setProjectName(project.getProjectName());
		p.setStartDate(project.getStartDate());
		if (project.getExecutiveDept() == null) {
			p.setExecutiveDept(new SimpleDeptObj(Type.GetInt(taskDeptId)));// 分配到部门时候，选择的部门为执行部门
		} else {
			p.setExecutiveDept(project.getExecutiveDept());
		}
		p.setType(project.getType());
		p.setLastModified(DateUtil.getCurrentDateTime());
		p.setLastModifier(WebUtil.getCurrentEmployee());
		if (p.getStatus() != null && p.getStatus().getId() == ProjectStatus.NEW) {
			p.setStatus(new ProjectStatus(ProjectStatus.START));
		}
		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		if (StringUtils.isEmpty(workItemId)) {// 新流程
			IProcessInstance processInstance = WorkflowUtil.startProcess(
					workflowSession, WorkflowConstants.WF_START_PROJECT,
					emp.getUserID() + "");
			WorkflowUtil.addProcessInstanceVariable(processInstance,
					"projectID", p.getProjectID());
			WorkflowUtil.addProcessInstanceVariable(processInstance,
					"projectName", p.getProjectName());
			WorkflowUtil.addProcessInstanceVariable(processInstance,
					"projectCode", p.getProjectCode());
			if (assignToDept) {
				WorkflowUtil.addProcessInstanceVariable(processInstance,
						"deptTaskDesc", deptTaskDesc);// 按部门分配的，任务描述
				// 设置流程实例变量taskDeptId，对应“主任分配”中的Performer Name
				WorkflowUtil.addProcessInstanceVariable(processInstance,
						"taskDeptId", Integer.valueOf(taskDeptId));
			} else {
				assginProjectTask(emp, ts, p, workflowSession, processInstance);
			}
			// 完成第一步骤
			WorkflowUtil.completeFirstTask(workflowSession, processInstance,
					String.valueOf(emp.getUserID()),
					assignToDept ? deptTaskDesc : null);
			String appendName = " 项目编码 " + p.getProjectCode() + " | 项目名称 "
					+ p.getProjectName();
			this.doUpdateProcessName(processInstance, appendName);
		} else {
			IWorkItem workitem = workflowSession.findWorkItemById(workItemId);
			IProcessInstance processInstance = ((TaskInstance) workitem
					.getTaskInstance()).getAliveProcessInstance();
			WorkflowUtil.addProcessInstanceVariable(processInstance,
					"projectID", p.getProjectID());
			WorkflowUtil.addProcessInstanceVariable(processInstance,
					"projectName", p.getProjectName());
			WorkflowUtil.addProcessInstanceVariable(processInstance,
					"projectCode", p.getProjectCode());
			if (assignToDept) {
				// 设置流程实例变量taskDeptId，对应“主任分配”中的Performer Name
				WorkflowUtil.addProcessInstanceVariable(processInstance,
						"taskDeptId", Integer.valueOf(taskDeptId));
			} else {
				assginProjectTask(emp, ts, p, workflowSession, processInstance);
			}
			//自动签收
			workflowService.doClaimTaskList(workItemId, Type.getString(emp.getUserID()));
			WorkflowUtil.completeTask(workflowSession, workItemId,
					emp.getUserID() + "", assignToDept ? deptTaskDesc : null);
		}

	}

	private void assginProjectTask(SimpleEmployee emp, String ts, ProjectObj p,
			IWorkflowSession workflowSession, IProcessInstance processInstance) {
		JSONArray tasks = JSONArray.fromObject(ts);
		// 保存项目任务
		this.saveProjectTask(p, tasks, emp);
		List<String> list = new ArrayList<String>();
		String managerId = null;
		for (int i = 0; i < tasks.size(); i++) {
			JSONObject object = tasks.getJSONObject(i);
			if (object.getInt("role") == 1) {
				managerId = object.getString("userId");
			} else {
				list.add(object.getString("userId"));
			}
		}
		EmployeeObj manager = organizationDAO.getEmployee(Integer
				.parseInt(managerId));
		p.setManager(manager);
		if (manager.getPostList().size() > 0 && p.getExecutiveDept() == null) {
			p.setExecutiveDept(new SimpleDeptObj(manager.getPostList().get(0)
					.getDepartment().getDepartmentID()));
		}
		WorkflowUtil.specifyActor(workflowSession, managerId, "__manager");
		WorkflowUtil.specifyActor(workflowSession, list, "__receive_ids");
		// 设置流程实例变量memeberSize，任务成员数量
		WorkflowUtil.addProcessInstanceVariable(processInstance, "memeberSize",
				list.size());
	}

	public void doDirectorAssignTask(String workItemId, ProjectObj project,
			EmployeeObj emp, String ts, String comment)
			throws WorkflowException {
		ProjectObj p = projectDAO.getProjectInfoByID(project.getProjectID());
		IWorkflowSession wflsession = workflowContext.getWorkflowSession();
		IWorkItem workitem = wflsession.findWorkItemById(workItemId);
		assginProjectTask(emp, ts, p, wflsession,
				((TaskInstance) workitem.getTaskInstance())
						.getAliveProcessInstance());
		//自动签收
		workflowService.doClaimTaskList(workItemId, Type.getString(WebUtil
				.getCurrentEmployee().getUserID() ));
		WorkflowUtil.completeTask(wflsession, workItemId, emp.getUserID() + "",
				comment);
	}

	public void doDeleteProjectSchedulePlan(String id) {
		projectDAO.deleteProjectSchedulePlan(id);
	}

	public void doAcceptProjectTask(SimpleEmployee emp, String workItemId,
			String projectTaskId, String comment) throws WorkflowException {
		IWorkflowSession wflsession = workflowContext.getWorkflowSession();
		//自动签收
		workflowService.doClaimTaskList(workItemId, Type.getString(WebUtil
				.getCurrentEmployee().getUserID() ));
		WorkflowUtil.completeTask(wflsession, workItemId, emp.getUserID() + "",
				comment);
	}
//批处理 未用
	public void doBatchAcceptProjectTask(SimpleEmployee emp, String workItemId,
			String comment) throws WorkflowException {
		List<String> workItemIdsList = TemplateUtil.getListByIds(workItemId,
				",");
		for (int i = 0; i < workItemIdsList.size(); i++) {
			this.doAcceptProjectTask(emp, workItemIdsList.get(i), "", comment);
		}
	}

	public RuntimeContext getWorkflowContext() {
		return workflowContext;
	}

	public void setWorkflowContext(RuntimeContext aWorkflowContext) {
		this.workflowContext = aWorkflowContext;
	}

	public DeviceDAO getDeviceDAO() {
		return deviceDAO;
	}

	public void setDeviceDAO(DeviceDAO deviceDAO) {
		this.deviceDAO = deviceDAO;
	}

	public String getProjectStatus(StandardQuery query) {
		List<ProjectStatus> list = projectDAO.getProjectStatus(query);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getProjectStatusByProjectId(String projectId,
			StandardQuery query) {
		CompositParam projectParam = CompositParam.buildOrCompositParam();
		projectParam.addStandardQuery(
				CompositParam
						.buildAndCompositParam()
						.addStandardQuery(
								SimpleParam.buildSimpleParam("custom", false,
										Type.BOOLEAN, RelationalOperator.EQ))
						.addStandardQuery(
								SimpleParam.buildSimpleParam("id", 0,
										Type.INTEGER, RelationalOperator.GT)))
				.addStandardQuery(
						SimpleParam.buildSimpleParam("projectId", projectId,
								Type.STRING, RelationalOperator.EQ));
		CompositParam query2 = CompositParam.buildAndCompositParam();
		query2.addStandardQuery(projectParam).addStandardQuery(query);
		List<ProjectStatus> list = projectDAO.getProjectStatus(query2);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getDefaultProjectStatus(StandardQuery query) {
		CompositParam defaultQuery = CompositParam
				.buildAndCompositParam()
				.addStandardQuery(
						SimpleParam.buildSimpleParam("custom", false,
								Type.BOOLEAN, RelationalOperator.EQ))
				.addStandardQuery(
						SimpleParam.buildSimpleParam("id", 0, Type.INTEGER,
								RelationalOperator.GT));
		CompositParam query2 = CompositParam.buildAndCompositParam();
		query2.addStandardQuery(defaultQuery).addStandardQuery(query);
		List<ProjectStatus> list = projectDAO.getProjectStatus(query2);
		return WebUtil.buildJsonStoreByList(list);
	}

	public void doUpdateProjectStatus(String projectId, String status,
			String statusDesc, String actualStartDate) throws Exception {
		ProjectObj projectObj = projectDAO.getProjectInfoByID(projectId);
		ProjectStatus s = new ProjectStatus();
		if (StringUtils.isNumeric(status)) {
			s.setId(Integer.parseInt(status));
		} else {
			s.setName(status);
			s.setProjectId(projectId);
			s.setDescription(statusDesc);
			s.setCustom(true);
			projectDAO.doSaveProjectStatus(s);
		}
		projectObj.setStatus(s);
		ProjectSchedulePlan plan = projectDAO.getProjectSchedulePlanByStatusId(
				projectId, s.getId());
		if (plan != null) {
			plan.setActualStartDate(DateUtil.parseDate(actualStartDate,
					"yyyy-MM-dd"));
			projectDAO.saveProjectSchedulePlan(plan);
		}
		// contractDAO.doUpdatePaymentConditionDate(projectObj.getProjectID(),
		// s.getId());
	}

	public int doImportDeviceList(String projectId,
			InputStream excelInputStream, SimpleEmployee entryEmp) {
		Workbook book = null;
		int sum = 0;
		try {
			book = Workbook.getWorkbook(excelInputStream);
			Sheet sheet = book.getSheet(0);
			for (int row = 1; row < sheet.getRows(); row++) {
				Cell[] cells = sheet.getRow(row);
				if (cells.length != 6) {
					continue;
				}
				DeviceObj deviceObj = new DeviceObj();
				deviceObj.setProjectID(projectId);
				String name = cells[0].getContents();
				if (StringUtils.isEmpty(name)) {
					continue;
				}
				deviceObj.setName(name);
				deviceObj.setNumber(Integer.parseInt(cells[1].getContents()));
				deviceObj.setModelType(cells[2].getContents());
				deviceObj.setStandard(cells[3].getContents());
				deviceObj.setWeight(cells[4].getContents());
				deviceObj.setReferencePrice(Type.GetDouble(cells[5]
						.getContents()));
				deviceObj.setCurrency(Currency.buildDefaultCurrency());
				deviceObj.setLastModified(DateUtil.getCurrentDateTime());
				deviceObj.setLastModifier(entryEmp);
				projectDAO.doSaveDevice(deviceObj);
				sum++;
			}
		} catch (Exception e) {
			throw new RuntimeException(e);
		} finally {
			if (book != null) {
				book.close();
			}
		}
		return sum;
	}

	public String getProjectSchedulePlanByQuery(StandardQuery query,
			PageInfo pageInfo) {
		List<ProjectSchedulePlan> list = projectDAO
				.getProjectSchedulePlanByQuery(query, pageInfo);
		return WebUtil.buildJsonStoreByList(list);
	}

	@SuppressWarnings("static-access")
	public void doSaveProjectSchedulePlan(String projectId, String plansJson,
			SimpleEmployee emp) throws ParseException {
		// projectDAO.deleteProjectSchedulePlanByProjectId(projectId);
		JSONArray plans = JSONArray.fromObject(plansJson);
		ProjectSchedulePlan plan;
		Date actDate = new Date(0);
		ProjectSchedulePlan cur = null;
		for (int i = 0; i < plans.size(); i++) {
			JSONObject object = plans.getJSONObject(i);
			plan = new ProjectSchedulePlan();
			plan.setId(JSONUtils.getString(object, "id"));
			plan.setProject(new ProjectObj(projectId));
			plan.setStartDate(JSONUtils.getDate(object, "startDate"));
			plan.setActualStartDate(JSONUtils
					.getDate(object, "actualStartDate"));
			JSONObject statusJson = object.getJSONObject("status");
			ProjectStatus status = (ProjectStatus) object.toBean(statusJson,
					ProjectStatus.class);
			if (status.getId() == null) {
				projectDAO.doSaveProjectStatus(status);
			}
			plan.setStatus(status);
			plan.setLastModifier(emp);
			plan.setLastModified(DateUtil.getCurrentDateTime());
			projectDAO.saveProjectSchedulePlan(plan);
			if (plan.getActualStartDate() != null
					&& plan.getActualStartDate().getTime() > actDate.getTime()) {
				actDate = plan.getActualStartDate();
				cur = plan;
			}
		}
		if (cur != null) {
			ProjectObj projectObj = projectDAO.getProjectInfoByID(projectId);
			projectObj.setStatus(cur.getStatus());
		}
	}

	public void doUpdateProjectSchedulePlan(ProjectSchedulePlan plan) {
		ProjectSchedulePlan schedulePlan = projectDAO
				.getProjectSchedulePlan(plan.getId());
		schedulePlan.setLastModified(DateUtil.getCurrentDateTime());
		schedulePlan.setLastModifier(WebUtil.getCurrentEmployee());
		try {
			BeanUtils.copyNotNullProperties(schedulePlan, plan);
		} catch (Exception e) {
			// 不会发生的异常
		}
	}

	public double getReminderAmt(String projectId) {
		return projectDAO.getReminderAmt(projectId);
	}

	public int doSaveProjectStatus(ProjectStatus status) {
		return projectDAO.doSaveProjectStatus(status);
	}

	public ContractDAO getContractDAO() {
		return contractDAO;
	}

	public void setContractDAO(ContractDAO contractDAO) {
		this.contractDAO = contractDAO;
	}

	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}

	public void setOrganizationDAO(OrganizationDAO organizationDAO) {
		this.organizationDAO = organizationDAO;
	}

	/**
	 * 变更执行部门
	 * 
	 * @throws WorkflowException
	 * @throws KernelException
	 * @throws EngineException
	 */
	public void doUpdateProjectExecutiveDept(String projectID,
			int executiveDeptID, String executiveDeptChangeReason,
			int managerID, String managerChangeReason, String isChangeDept,
			String isChangeManager) throws WorkflowException, EngineException,
			KernelException {
		ProjectObj project = projectDAO.getProjectInfoByID(projectID);
		String projectCode = project.getProjectCode();
		SimpleDeptObj executiveDept = organizationDAO
				.getSimpleDeptObj(executiveDeptID);
		String executiveDeptName = "";
		if (executiveDept != null) {
			executiveDeptName = executiveDept.getDepartmentName();
		}
		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		SimpleEmployee emp = WebUtil.getCurrentEmployee();
		SimpleEmployee manager = organizationDAO.getSimpleEmployee(managerID);
		String managerName = "";
		if (manager != null) {
			managerName = manager.getUserName();
		}
		IProcessInstance processInstance = WorkflowUtil.startProcess(
				workflowSession,
				WorkflowConstants.WF_PROJECT_EXECUTIVE_DEPT_CHANGE,
				emp.getUserID() + "");
		WorkflowUtil.addProcessInstanceVariable(processInstance, "projectID",
				projectID);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "projectName",
				project.getProjectName());
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"executiveDeptID", executiveDeptID);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "projectCode",
				projectCode);
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"executiveDeptName", executiveDeptName);
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"executiveDeptChangeReason", executiveDeptChangeReason);

		WorkflowUtil.addProcessInstanceVariable(processInstance, "managerID",
				managerID);
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"managerChangeReason", managerChangeReason);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "managerName",
				managerName);
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"isChangeDept", Boolean.valueOf(isChangeDept));
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"isChangeManager", Boolean.valueOf(isChangeManager));
		WorkflowUtil.specifyActor(workflowSession, Type.getString(managerID),
				"__manager");
		
		WorkflowUtil.completeFirstTask(workflowSession, processInstance,
				String.valueOf(emp.getUserID()), "");
		// update 任务名字 追加 项目编码、项目名称
		String appendName = " 项目编码 " + projectCode + " | 项目名称 "
				+ project.getProjectName();
		if (Boolean.valueOf(isChangeDept)) {
			appendName = appendName + " | 执行部门变更为" + executiveDeptName;
		}
		if (Boolean.valueOf(isChangeManager)) {
			appendName = appendName + " | 项目经理变更为" + managerName;
		}
		this.doUpdateProcessName(processInstance, appendName);

	}

	/**
	 * 接收任务 变更执行部门
	 */
	public void doAcceptProjectExecutiveDept(String projectID,
			int executiveDeptID, String workItemId) throws WorkflowException {
		SimpleDeptObj executiveDept = organizationDAO
				.getSimpleDeptObj(executiveDeptID);
		ProjectObj project = projectDAO.getProjectInfoByID(projectID);
		project.setExecutiveDept(executiveDept);
		projectDAO.doSaveProjectInfo(project);

		SimpleEmployee emp = WebUtil.getCurrentEmployee();
		IWorkflowSession wflsession = workflowContext.getWorkflowSession();
	//自动签收
		workflowService.doClaimTaskList(workItemId, Type.getString(emp.getUserID()));
		WorkflowUtil.completeTask(wflsession, workItemId, emp.getUserID() + "",
				"");
	}

	/**
	 * 批量 变更执行部门  未使用
	 * 
	 * @param workItemIdList
	 * @throws WorkflowException
	 */
	public void doBatchAcceptProjectExecutiveDept(String workItemId)
			throws WorkflowException {
		List<String> workItemIdsList = TemplateUtil.getListByIds(workItemId,
				",");
		for (int i = 0; i < workItemIdsList.size(); i++) {
			IWorkflowSession wflsession = workflowContext.getWorkflowSession();
			IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
					wflsession, workItemIdsList.get(i));
			String projectID = (String) processInstance
					.getProcessInstanceVariable("projectID");
			int executiveDeptID = (Integer) processInstance
					.getProcessInstanceVariable("executiveDeptID");

			this.doAcceptProjectExecutiveDept(projectID, executiveDeptID,
					workItemIdsList.get(i));
		}
	}

	/**
	 * 接收任务 变更项目经理
	 */
	public void doAcceptProjectManager(String projectID, int managerID,
			String workItemId) throws WorkflowException {
		SimpleEmployee manager = organizationDAO.getSimpleEmployee(managerID);
		ProjectObj project = projectDAO.getProjectInfoByID(projectID);
		project.setManager(manager);
		projectDAO.doSaveProjectInfo(project);

		// 根据项目ID得到所有项目任务ID的列表；
		// 修改项目任务成员表中成员角色是项目经理的的项目成员ID;

		List<ProjectTaskObj> taskObjList = projectDAO
				.getTaskListByProjectID(projectID);
		for (ProjectTaskObj task : taskObjList) {
			for (ProjectTaskMember taskMember : task.getMembers()) {
				if (taskMember.getRole() == 1) {
					taskMember.setMember(new SimpleEmployee(managerID));
					projectDAO.doSaveProjectTaskMember(taskMember);
				}
			}
		}

		SimpleEmployee emp = WebUtil.getCurrentEmployee();
		IWorkflowSession wflsession = workflowContext.getWorkflowSession();
		
		//自动签收
		workflowService.doClaimTaskList(workItemId, Type.getString(emp.getUserID()));
		WorkflowUtil.completeTask(wflsession, workItemId, emp.getUserID() + "",
				"");

	}

	/**
	 * 批量 变更项目经理 未使用
	 */
	public void doBatchAcceptProjectManager(String workItemId)
			throws WorkflowException {
		List<String> workItemIdsList = TemplateUtil.getListByIds(workItemId,
				",");
		for (int i = 0; i < workItemIdsList.size(); i++) {
			IWorkflowSession wflsession = workflowContext.getWorkflowSession();
			IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
					wflsession, workItemIdsList.get(i));
			String projectID = (String) processInstance
					.getProcessInstanceVariable("projectID");
			int managerID = (Integer) processInstance
					.getProcessInstanceVariable("managerID");
			this.doAcceptProjectManager(projectID, managerID,
					workItemIdsList.get(i));
		}

	}

	/**
	 * 启动内部验收流程-提交给总工
	 */
	public void doStartInterCheckToEngineer(String projectID,
			String checkDeptID, String comment) throws WorkflowException,
			EngineException, KernelException {
		ProjectObj project = projectDAO.getProjectInfoByID(projectID);
		String projectCode = project.getProjectCode();
		String projectName = project.getProjectName();

		ProjectStatus status = new ProjectStatus(301);// 301内部验收已经启动
		project.setStatus(status);

		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		EmployeeObj emp = WebUtil.getCurrentEmployee();
		int taskDeptId = emp.getDivisionID();

		// 6\7 manager 8\10 engineer
		// 根据人员ID，判断是否是主任 的方法；是否是总工的方法；
		boolean isEngineer = isEngineer(emp.getUserID());
		boolean isManager = isManager(emp.getUserID());

		IProcessInstance processInstance = WorkflowUtil.startProcess(
				workflowSession, WorkflowConstants.WF_INTERCHECK,
				emp.getUserID() + "");

		WorkflowUtil.addProcessInstanceVariable(processInstance, "projectID",
				projectID);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "projectName",
				projectName);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "projectCode",
				projectCode);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "checkDeptID",
				checkDeptID);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "applicant",
				emp.getUserID());
		WorkflowUtil.addProcessInstanceVariable(processInstance, "empDeptId",
				taskDeptId);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "deptId",
				Integer.parseInt(checkDeptID));

		WorkflowUtil.addProcessInstanceVariable(processInstance, "isEngineer",
				isEngineer);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "isManager",
				isManager);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "comment",
				comment);
		WorkflowUtil.completeFirstTask(workflowSession, processInstance,
				String.valueOf(emp.getUserID()), "");

		// update 任务名字 追加 项目编码、项目名称
		String appendName = " 项目编码 " + projectCode + " | 项目名称 "
				+ project.getProjectName();
		this.doUpdateProcessName(processInstance, appendName);
	}

	/**
	 * 总工 主任审批
	 * 
	 * @throws WorkflowException
	 * @throws KernelException
	 * @throws EngineException
	 */
	public void doManagerRefuseCheck(String isAgree, String workItemId,
			String comment) throws WorkflowException, EngineException,
			KernelException {
		SimpleEmployee emp = WebUtil.getCurrentEmployee();

		boolean flag = true;
		String result = "";
		if ("no".equals(isAgree)) {
			flag = false;
			result = "未通过";
		}

		IWorkflowSession wflsession = workflowContext.getWorkflowSession();

		WorkflowUtil.addProcessInstanceVariable(wflsession, workItemId,
				"isAgree", flag);
		WorkflowUtil.addProcessInstanceVariable(wflsession, workItemId,
				"comment", emp.getUserName() + comment);
		WorkflowUtil.addProcessInstanceVariable(wflsession, workItemId, "goon",
				result);
		//自动签收
		workflowService.doClaimTaskList(workItemId, Type.getString(WebUtil
				.getCurrentEmployee().getUserID() ));
		WorkflowUtil.completeTask(wflsession, workItemId, emp.getUserID() + "",
				comment);
	}

	// 6\7 manager 8\10 engineer
	// 根据人员ID，判断是否是主任 的方法；
	public boolean isEngineer(int userID) {
		boolean isEngineer = false;
		List list = organizationDAO.getPostListByEmpID(userID);
		if (list.size() > 0) {
			for (int i = 0; i < list.size(); i++) {
				Object obj = ((Map) list.get(i)).get("职务");
				if (obj != null && obj != "") {
					short post = (Short) obj;
					if (post == 8 || post == 10) {
						isEngineer = true;
					}
				}
			}
		}
		return isEngineer;
	}

	// 6\7 manager 8\10 engineer
	// 根据人员ID，是否是总工的方法；
	public boolean isManager(int userID) {
		boolean isManager = false;
		List list = organizationDAO.getPostListByEmpID(userID);
		if (list.size() > 0) {
			for (int i = 0; i < list.size(); i++) {
				Object obj = ((Map) list.get(i)).get("职务");
				if (obj != null && obj != "") {
					short post = (Short) obj;
					if (post == 6 || post == 7 || post == 14) {
						isManager = true;
					}
				}
			}
		}
		return isManager;
	}

	/**
	 * 验收员 提交不通过原因
	 * 
	 * @throws WorkflowException
	 * @throws KernelException
	 * @throws EngineException
	 */
	// public void doAcceptorRefuseCheck(String projectID, String applicant,
	// String workItemId, String comment,String checkDeptID) throws
	// WorkflowException, EngineException, KernelException {
	// SimpleEmployee emp = WebUtil.getCurrentEmployee();
	//
	// IWorkflowSession wflsession = workflowContext.getWorkflowSession();
	// //修改其他人的任务状态
	// IWorkItem wi = wflsession.findWorkItemById(workItemId);
	// List<WorkItem> workitems =
	// workflowDAO.getWorkItemsByTaskInstance(wi.getTaskInstance().getId());
	// for (WorkItem item : workitems) {
	// workflowDAO.overTaskInstance(wi.getTaskInstance().getId());
	// //修改workItem表、
	// workflowDAO.overTaskInstanceHistory(wi.getTaskInstance().getId());//修改wf_历史表、
	// //
	// todoDAO.deleteTodoByAppId(Constants.TODO_MODEL_ID,item.getId());//wf_待办表里删除；
	// // workflowDAO.deleteObject(new WorkItemHistory(item.getId()));//wf_历史里删除
	// 即 流程跟踪里删除；
	// }
	//
	// IProcessInstance
	// processInstance=WorkflowUtil.getProcessInstance(wflsession, workItemId);
	// InterCheckApproveInfo approveInfo = new InterCheckApproveInfo();
	// approveInfo.setProcessInstanceID(processInstance.getId());
	// approveInfo.setActorID(Type.getString(emp.getUserID()));
	// approveInfo.setIsApprove(0);
	// approveInfo.setAppId(workItemId);
	// approveInfo.setComments(comment);
	// interCheckDAO.doSaveOrUpdate(approveInfo);
	// WorkflowUtil.addProcessInstanceVariable(processInstance, "checkDeptID",
	// checkDeptID);
	// WorkflowUtil.addProcessInstanceVariable(wflsession, workItemId,
	// "isAgree",
	// false);
	// WorkflowUtil.addProcessInstanceVariable(wflsession, workItemId, "goon",
	// "未通过");
	// WorkflowUtil.addProcessInstanceVariable(wflsession, workItemId,
	// "comment",
	// comment);
	// WorkflowUtil.specifyActor(wflsession, applicant, "__applicant");
	// WorkflowUtil.completeTask(wflsession, workItemId, emp.getUserID() + "",
	// comment);
	// }
	/**
	 * 判断此审批否所有接收任务成员都通过
	 * 
	 * @param workItemId
	 * @return
	 */
	// @SuppressWarnings("unused")
	// private boolean isAllAgree(String workItemId) {
	// @SuppressWarnings("rawtypes")
	// List numsList = workflowDAO.getNumsOfTaskIdByWorkItemId(workItemId);
	// if (numsList != null && numsList.size() > 0) {
	// int agreeNums = (Integer) numsList.get(0) + 1;
	// int nums = (Integer) numsList.get(1);
	// if (nums != 0 && nums == agreeNums) {
	// return true;
	// } else {
	// return false;
	// }
	// } else {
	// return false;
	// }
	// }


	public void doCheckDeptAssignTask(ProjectTaskObj ptask, String memberJson,
			String workItemId, String comment, String divisionManagerID)
			throws JsonParseException, JsonMappingException, IOException,
			EngineException, KernelException, WorkflowException {
		EmployeeObj employee = WebUtil.getCurrentEmployee();
		ptask.setGrantor(employee);
		ptask.setLastModifier(employee);
		ptask.setLastModified(DateUtil.getCurrentDateTime());
		ProjectTaskStatus status = new ProjectTaskStatus();
		status.setId(ProjectTaskStatus.NEW);
		ptask.setStatus(status);
		projectDAO.doSaveProjectTask(ptask);
		List<ProjectTaskMember> members = JacksonUtil.readValue(memberJson,
				new TypeReference<List<ProjectTaskMember>>() {
				});
		List<String> ids = new ArrayList<String>();
		for (ProjectTaskMember member : members) {
			member.setProjectTaskId(ptask.getTaskID());
			member.setRole(3); // 验收部门分配任务，接收任务成员的角色设定为3
			projectDAO.doSaveProjectTaskMember(member);
			ids.add(String.valueOf(member.getMember().getUserID()));
		}
		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
				workflowSession, workItemId);
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"projectTaskId", ptask.getTaskID());
		WorkflowUtil.addProcessInstanceVariable(processInstance, "comment",
				comment);

		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"divisionManagerID", divisionManagerID);// 通知的事业部主任
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"checkManagerID", employee.getUserID());// 验收部门的主任 消息表存数据用的
		int applicant = Type.GetInt(processInstance
				.getProcessInstanceVariable("applicant"));
		WorkflowUtil.addProcessInstanceVariable(processInstance, "applicant",
				applicant);
		WorkflowUtil.specifyActor(workflowSession, ids, "__acceptor");
		//自动签收
		workflowService.doClaimTaskList(workItemId, Type.getString(WebUtil
				.getCurrentEmployee().getUserID() ));
		WorkflowUtil.completeTask(workflowSession, workItemId,
				employee.getUserID() + "", "");
	}

	public void doGoOnInterCheckToEngineer(String workItemId, String comment)
			throws WorkflowException {

		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		EmployeeObj emp = WebUtil.getCurrentEmployee();
		// int taskDeptId = emp.getDivisionID();
		// 6\7 manager 8\10 engineer
		// 根据人员ID，判断是否是主任 的方法；是否是总工的方法；
		boolean isEngineer = isEngineer(emp.getUserID());
		boolean isManager = isManager(emp.getUserID());

		IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
				workflowSession, workItemId);

		WorkflowUtil.addProcessInstanceVariable(processInstance, "isEngineer",
				isEngineer);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "isManager",
				isManager);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "comment",
				comment);
		//自动签收
		workflowService.doClaimTaskList(workItemId, Type.getString(WebUtil
				.getCurrentEmployee().getUserID() ));
		WorkflowUtil.completeTask(workflowSession, workItemId, emp.getUserID()
				+ "", "");
	}

	/**
	 * 验收员审批结果 提交给部门主任
	 */
	public void doTellDeptManager(String comment, String workItemId,
			String isApproved, String checkManagerID) throws WorkflowException,
			EngineException, KernelException {

		SimpleEmployee employee = WebUtil.getCurrentEmployee();

		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
				workflowSession, workItemId);

		WorkflowUtil.addProcessInstanceVariable(processInstance, "isApproved",
				isApproved);

		WorkflowUtil.addProcessInstanceVariable(processInstance, "comment",
				employee.getUserName() + comment);
		WorkflowUtil.specifyActor(workflowSession, checkManagerID,
				"__checkManagerID");// 验收部门的主任--图用，这个验收部门主任是分配给验收员任务的主任，所以是一个人
		//自动签收
		workflowService.doClaimTaskList(workItemId, Type.getString(WebUtil
				.getCurrentEmployee().getUserID() ));
		WorkflowUtil.completeTask(workflowSession, workItemId,
				Type.getString(employee.getUserID()), comment);
	}

	/**
	 * 验收部门主任审批结果 提交给事业部主任
	 */
	public void doTellDivisionDeptManager(String comment, String workItemId,
			String isApproved, String projectID) throws WorkflowException,
			EngineException, KernelException {
		SimpleEmployee employee = WebUtil.getCurrentEmployee();
		String showMessage = "";
		boolean isAgree = false;
		if (isApproved.equals("yes")) {
			showMessage = "通过";
			isAgree = true;
		} else {
			showMessage = "未通过";
		}
		if (isAgree) {
			ProjectObj project = projectDAO.getProjectInfoByID(projectID);
			ProjectStatus status = new ProjectStatus(193);// 193 内部验收通过
			project.setStatus(status);
		}

		// InterCheckApproveInfo approveInfo = new InterCheckApproveInfo();
		// approveInfo.setProcessInstanceID(processInstance.getId());
		// approveInfo.setActorID(Type.getString(employee.getUserID()));
		// approveInfo.setIsApprove(1);
		// approveInfo.setAppId(workItemId);
		// approveInfo.setComments("审批通过");
		// interCheckDAO.doSaveOrUpdate(approveInfo);

		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
		IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
				workflowSession, workItemId);
		int applicant = Type.GetInt(processInstance
				.getProcessInstanceVariable("applicant"));
		int empDeptId = organizationDAO
				.getDivisionDeptIDByDeptID(organizationDAO
						.getDeptIDByUserID(applicant));

		WorkflowUtil.addProcessInstanceVariable(processInstance, "isApproved",
				isApproved);

		WorkflowUtil.addProcessInstanceVariable(workflowSession, workItemId,
				"goon", showMessage);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "empDeptId",
				empDeptId); // 表达式求事业部主任
		WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
				isAgree);
		//自动签收
		workflowService.doClaimTaskList(workItemId, Type.getString(WebUtil
				.getCurrentEmployee().getUserID() ));
		WorkflowUtil.completeTask(workflowSession, workItemId,
				Type.getString(employee.getUserID()), comment);
	}

	@SuppressWarnings("rawtypes")
	public String getInterCheckList(String processInstanceId) {
		List list = projectDAO.getInterCheckList(processInstanceId);
		return WebUtil.buildJsonStoreByList(list, list.size());
	}

	public void doUpdateProcessName(IProcessInstance processInstance,
			String appendName) {
		try {
			String processInstanceId = processInstance.getId();
			ProcessWithKeyword processWithKeyword = new ProcessWithKeyword();
			processWithKeyword.setProcessInstanceId(processInstanceId);
			processWithKeyword.setKeyword(processInstance.getDisplayName()
					+ " " + appendName);
			workflowDAO.doCreateProcessWithKeyword(processWithKeyword);

			int userID = WebUtil.getCurrentEmployee().getUserID();
			workflowDAO.doUpdateWorkHistoryByProcessInstanceId(
					processInstanceId, processInstance.getDisplayName() + "||"
							+ appendName, userID);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	
	/**  
	 *  变更执行部门或经理的再次提交 未使用
	 */
	public void doUpdateProjectExecutiveDeptByReturn(int executiveDeptID,
			String executiveDeptChangeReason, int managerID,
			String managerChangeReason, String workItemId)
			throws WorkflowException {
		SimpleDeptObj executiveDept = organizationDAO
				.getSimpleDeptObj(executiveDeptID);
		String executiveDeptName = "";
		if (executiveDept != null) {
			executiveDeptName = executiveDept.getDepartmentName();
		}
		SimpleEmployee manager = organizationDAO.getSimpleEmployee(managerID);
		String managerName = "";
		if (manager != null) {
			managerName = manager.getUserName();
		}

		IWorkflowSession workflowSession = workflowContext.getWorkflowSession();

		IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
				workflowSession, workItemId);
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"executiveDeptName", executiveDeptName);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "managerID",
				managerID);
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"executiveDeptID", executiveDeptID);
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"executiveDeptChangeReason", executiveDeptChangeReason);
		WorkflowUtil.addProcessInstanceVariable(processInstance,
				"managerChangeReason", managerChangeReason);
		WorkflowUtil.addProcessInstanceVariable(processInstance, "managerName",
				managerName);
		WorkflowUtil.specifyActor(workflowSession, Type.getString(managerID),
				"__manager");
		//自动签收
		workflowService.doClaimTaskList(workItemId, Type.getString(WebUtil
				.getCurrentEmployee().getUserID() ));
		WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
				.getCurrentEmployee().getUserID() + "", "");
	}

	public String getRelationChildContractCodeByID(String contractID) {
		List list = projectDAO.getRelationChildContractCodeByID(contractID);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
	}

	public String getRelationProjectID(Integer contractId) {
		List list= projectDAO.getRelationProjectID(contractId);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getProjectQualificationName(StandardQuery query,
			PageInfo pageInfo) throws Exception {
		// TODO Auto-generated method stub
		List listStore = projectDAO.getProjectQualificationName(query, pageInfo);
		return WebUtil.buildStoreForJson(listStore, "");
	}
}
