//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\web\\action\\ProjectAction.java

package com.bitc.jk.xmis.web.action;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.URLEncoder;
import java.sql.Date;
import java.util.Iterator;
import java.util.List;

import javax.servlet.ServletContext;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.commons.beanutils.ConvertUtils;
import org.apache.commons.beanutils.converters.DateConverter;
import org.apache.commons.fileupload.DiskFileUpload;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;
import org.fireflow.engine.EngineException;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.DeviceObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.ProjectObj;
import com.bitc.jk.xmis.model.ProjectSchedulePlan;
import com.bitc.jk.xmis.model.ProjectStatus;
import com.bitc.jk.xmis.model.ProjectTaskObj;
import com.bitc.jk.xmis.model.ProjectType;
import com.bitc.jk.xmis.model.SimpleDeptObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.service.FileUploadDownloadService;
import com.bitc.jk.xmis.service.ImportExcelService;
import com.bitc.jk.xmis.service.LoginService;
import com.bitc.jk.xmis.service.ProjectService;
import com.bitc.jk.xmis.service.SysService;
import com.bitc.jk.xmis.service.impl.FileUploadDownloadServiceImpl;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.SpringInit;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;

public class ProjectAction extends DispatchAction {
	private static Logger logger = Logger.getLogger(ProjectAction.class);

	private ProjectService projectService;

	private SysService sysService;

	private LoginService loginService;
	private ImportExcelService importExcelService;

	public ImportExcelService getImportExcelService() {
		return importExcelService;
	}

	public void setImportExcelService(ImportExcelService importExcelService) {
		this.importExcelService = importExcelService;
	}

	/**
	 * @roseuid 4C1F042200DA
	 */
	public ProjectAction() {

	}

	public ProjectService getProjectService() {
		return projectService;
	}

	public void setProjectService(ProjectService aProjectService) {
		projectService = aProjectService;
	}

	public void getProjectListByUser() {

	}
	
	public SysService getSysService() {
		return sysService;
	}
	
	public void setSysService(SysService sysService) {
		this.sysService = sysService;
	}

	public LoginService getLoginService() {
		return loginService;
	}

	public void setLoginService(LoginService loginService) {
		this.loginService = loginService;
	}
	
	public ActionForward getProjectInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		
		EmployeeObj userObj = (EmployeeObj) request.getSession()
		.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
		
		String projectID = request.getParameter("projectID");
		logger.info("getProjectInfo//projectId:" + projectID);
		ProjectObj project;
		try {
			project = projectService.getProjectInfoByID(projectID);
			
			
			project.getMainContract().setContractOpType(
					loginService.getContractOpType(userObj,
							project.getMainContract()));		
			
			
		} catch (Exception e) {
			throw e;
		}
		request.setAttribute("project", project);
		String isAgree = request.getParameter("isAgree");
		request.setAttribute("isHiddenByIsAgree", isAgree);
		return mapping.findForward("projectInfo");
	}

	@SuppressWarnings("unchecked")
	public ActionForward getProjectList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String show = request.getParameter("show");
		if (show != null) {
			return mapping.findForward("projectList");
		}
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		List<StandardQuery> querys = (List<StandardQuery>) request
				.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);
		CompositParam query = CompositParam.buildAndCompositParam();
		query.addStandardQuery(
				CompositParam.buildOrCompositParam().addStandardQuery(querys))
				.addStandardQuery(
						SimpleParam.buildSimpleParam("mainContract.contractID",
								null, Type.INTEGER, RelationalOperator.ISNULL));
		StandardQuery query2 = WebUtil.generateStandardQueryByInkling(request);
		if (query2 == null) {
			query2 = WebUtil.generateStandardQueryByMulit(request, sysService);
		}
		query.addStandardQuery(query2);
		String store = projectService.getProjectListStoreByQuery(query,
				pageInfo);
		WebUtil.returnResponse(response, store);
		return null;
	}

	@SuppressWarnings("unchecked")
	public ActionForward getProjectListStore(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		List<StandardQuery> querys = (List<StandardQuery>) request
				.getAttribute(WebKeys.Request_Attribute_Name_AccessRight_StandardQuery);
		CompositParam query = CompositParam.buildAndCompositParam();
		query.addStandardQuery(CompositParam.buildOrCompositParam()
				.addStandardQuery(querys));
		StandardQuery query2 = WebUtil.generateStandardQueryByInkling(request);
		if (query2 != null) {
		} else {
			query2 = WebUtil.generateStandardQueryByMulit(request, sysService);
		}
		query.addStandardQuery(query2);
		String store = projectService.getProjectListStoreByQuery(query,
				pageInfo);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getDeviceByProjectID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String projectID = request.getParameter("projectID");
		if (StringUtils.isEmpty(projectID)) {
			String message = "getDeviceByProjectId//查询设备列表时提供的项目ID为空";
			logger.error(message);
			throw new Exception(message);
		}
		String store = projectService.getDeviceListStoreByID(projectID);
		response.getWriter().print(store);
		return null;
	}

	public ActionForward getProjectTaskByProjectID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String projectID = request.getParameter("projectID");
		if (StringUtils.isEmpty(projectID)) {
			String message = "getProjectTaskByProjectID//查询项目任务时提供的项目ID为空";
			logger.error(message);
			throw new Exception(message);
		}
		String store = projectService.getProjectTaskByProjectID(projectID);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getProjectStatusStore(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String searchName = request.getParameter("searchName");
		StandardQuery query = null;
		if (!StringUtils.isEmpty(searchName)) {
			query = SimpleParam.buildSimpleParam("name", searchName,
					Type.STRING, RelationalOperator.LIKE);
		}
		String store = projectService.getProjectStatus(query);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getProjectStatusStoreByProjectId(
			ActionMapping mapping, ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String projectId = request.getParameter("projectId");
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String store = projectService.getProjectStatusByProjectId(projectId,
				query);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getDefaultProjectStatus(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		String store = projectService.getDefaultProjectStatus(query);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward saveDevice(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		DeviceObj device = new DeviceObj();
		BeanUtils.populate(device, request.getParameterMap());
		int currencyId = Type.GetInt(request.getParameter("currencyId"));
		Currency currency = new Currency(currencyId);
		SimpleEmployee emp = WebUtil.getCurrentEmployee(request.getSession());
		device.setCurrency(currency);
		device.setLastModifier(emp);
		device.setLastModified(DateUtil.getCurrentDateTime());
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			projectService.doSaveDevice(device);
			ajaxResponse.setSuccess(true);
			
			ajaxResponse.setMessage("保存成功");
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.addAttribute("id", device.getId());
		} catch (Exception e) {
			logger.error("保存设备失败。" + e.getMessage());
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("保存失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward deleteDevice(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String deviceId = request.getParameter("id");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			projectService.doDeleteDevice(deviceId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
			ajaxResponse.setMessageTitle("提示");
		} catch (Exception e) {
			logger.error("删除设备失败。" + e.getMessage());
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除失败！");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	/**
	 * 启动项目 参数中包含show=true时，跳转到项目启动页面
	 */
	public ActionForward startProject(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		if (!StringUtils.isEmpty(request.getParameter("show"))) {
			String projectIDs = request.getParameter("projectID");
			if (!StringUtils.isEmpty(projectIDs)) {
				ProjectObj obj = projectService.getProjectInfoByID(projectIDs);
				request.setAttribute("project", obj);
			}
			return mapping.findForward("show");
		}
		EmployeeObj emp = WebUtil.getCurrentEmployee(request.getSession(false));
		ProjectObj project = buildProject(request, emp);
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String step = request.getParameter("step");
		String workItemId = request.getParameter("workItemId");
		if (StringUtils.isEmpty(step)
				|| "StartProject.TaskAssignment.AssignTask".equals(step)) {
			String mode = request.getParameter("mode");// 任务分配的方式，分配到部门为1
			String ts = request.getParameter("tasks");// 分配到人员时，任务分配信息
			String taskDeptId = request.getParameter("taskDept");// 分配到部门时的部门ID
			String deptTaskDesc = request.getParameter("deptTaskDesc");
			boolean assignToDept = "1".equals(mode);
			try {
				projectService.doStartProject(project, emp, assignToDept,
						taskDeptId, ts, deptTaskDesc, workItemId);
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessage("提交成功");
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.addAttribute("projectID", project.getProjectID());
			} catch (Exception e) {
				logger.error("项目启动失败。", e);
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessage("项目启动失败");
				ajaxResponse.setMessageTitle("失败");
			}
		} else if ("StartProject.DirectorAssign.AssignTask".equals(step)) {
			String ts = request.getParameter("tasks");
			String comment = request.getParameter("comment");
			projectService.doDirectorAssignTask(workItemId, project, emp, ts,
					comment);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("提交成功");
			ajaxResponse.setMessageTitle("成功");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
		return null;
	}


	public ActionForward updateProjectStatus(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String projectId = request.getParameter("projectId");
		String status = request.getParameter("status");
		String statusDesc = request.getParameter("statusDesc");
		String actualStartDate = request.getParameter("actualStartDate");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			projectService.doUpdateProjectStatus(projectId, status, statusDesc,
					actualStartDate);
			ajaxResponse.setSuccess(true);
		} catch (Exception e) {
			logger.error("设置项目状态失败。" + e.getMessage());
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("设置项目状态失败。\n" + e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward addProjectTask(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		ProjectTaskObj ptask = new ProjectTaskObj();
		ptask.setTaskName(request.getParameter("taskName"));
		ptask.setProjectID(request.getParameter("projectID"));
		
		String memberJson = request.getParameter("members");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("保存成功");
			ajaxResponse.setMessageTitle("成功");
			projectService.doAssignProjectTask(ptask, memberJson);
		} catch (Exception e) {
			logger.error("项目任务保存失败。",  e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("项目任务保存失败");
			ajaxResponse.setMessageTitle("失败");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	private ProjectObj buildProject(HttpServletRequest request, EmployeeObj emp) {
		String projectId = request.getParameter("projectID");
		String projectName = request.getParameter("projectName");
		String projectTypeID = request.getParameter("projectTypeID");
		String projectStartDate = request.getParameter("projectStartDate");
		String executiveDeptId = request.getParameter("executiveDept");

		ProjectObj project = new ProjectObj();
		if (!StringUtils.isEmpty(projectId)) {
			project.setProjectID(projectId);
		}
		project.setProjectName(projectName);
		ProjectType type = new ProjectType();
		type.setId(Type.GetInt(projectTypeID));
		project.setType(type);
		project.setStartDate(Date.valueOf(projectStartDate));
		if (StringUtils.isNotEmpty(executiveDeptId)) {
			project.setExecutiveDept(new SimpleDeptObj(Integer
					.parseInt(executiveDeptId)));
		}
		return project;
	}

	/**
	 * 项目启动流程，项目成员确认任务
	 */
	public ActionForward acceptProjectTask(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String workItemId = request.getParameter("workItemId");
		String projectTaskId=request.getParameter("projectTaskId");
		SimpleEmployee user = WebUtil.getCurrentEmployee(request
				.getSession(false));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			projectService.doAcceptProjectTask(user, workItemId,projectTaskId, null);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功完成任务");
			ajaxResponse.setMessage("任务已经完成。");
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("错误");
			ajaxResponse.setMessage("发生错误，无法完成任务。<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}
	/** 批量 项目成员确认任务  未使用
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void batchAcceptProjectTask(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String workItemId = request.getParameter("workItemId");
		SimpleEmployee user = WebUtil.getCurrentEmployee(request
				.getSession(false));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			projectService.doBatchAcceptProjectTask(user, workItemId, null);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功完成任务");
			ajaxResponse.setMessage("任务已经完成。");
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("错误");
			ajaxResponse.setMessage("发生错误，无法完成任务。<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}

	public ActionForward downloadTemplateFile(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		try {
			String directPath = request.getSession().getServletContext()
					.getRealPath("/");
			File file = new File(directPath + "/project/DevideListTemplate.xlt");
			response.setContentType("application/x-msdownload");
			response.addHeader("Content-Disposition", "attachment;filename="
					+ URLEncoder.encode("设备清单模板.xlt", "utf-8"));
			FileInputStream fis = new FileInputStream(file);
			OutputStream os = response.getOutputStream();
			int b = 0;
			while (fis != null && (b = fis.read()) != -1) {
				os.write(b);
			}
			os.flush();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	/** 设备列表的导入
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 * @throws Exception
	 */
	@SuppressWarnings("rawtypes")
	public ActionForward importDeviceList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		
		String directPath = request.getSession().getServletContext()
				.getRealPath("/");
		ServletContext servletContext = request.getSession()
				.getServletContext();
		String directPathLocal = (String) servletContext
				.getAttribute("application.attachedPath")+ request.getParameter("type") + "\\" ;
		SimpleEmployee emp = WebUtil.getCurrentEmployee(request.getSession());
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			DiskFileUpload fu = new DiskFileUpload();
			fu.setSizeThreshold(1024 * 1024);
			fu.setRepositoryPath(directPath + "temp");
			fu.setHeaderEncoding("utf-8");
			List fileItems = fu.parseRequest(request);
			Iterator iter = fileItems.iterator();
			while (iter.hasNext()) {
				FileItem item = (FileItem) iter.next();
				// 忽略其他不是文件域的所有表单信息
				if (!item.isFormField()) {
					String name = item.getName();
					long size = item.getSize();
					String contentType = item.getContentType();
					if (!"application/vnd.ms-excel".equals(contentType)) {
						throw new RuntimeException("文件类型错误。请上传由模板创建的文件。并确认上传的文件不是被打开状态。");
					}
					if ((name == null || name.equals("")) && size == 0)
						continue;
					// 保存上传的文件到指定的目录
			
					String[] names = StringUtils.split(name, "\\"); // 对原来带路径的文件名进行分割
					name = names[names.length - 1];
					String path = directPathLocal
							+ name;
					item.write(new File(path));

					FileUploadDownloadService service = (FileUploadDownloadServiceImpl) SpringInit
							.getBean("fileUploadDownloadServiceImpl",
									FileUploadDownloadServiceImpl.class);

					AttachmentFile attFile = new AttachmentFile(Type
							.getInteger(request.getParameter("type"))
							.intValue());// 2,4,5,6

					java.io.File ioFile = new java.io.File(path);

					InputStream inStream = new FileInputStream(ioFile);

					// int fileSize = file.getSize();
					int fileSize = (int) ioFile.length();
					byte[] content = new byte[fileSize];
					attFile.setUploadID(emp.getUserID());
					attFile.setDesc(request.getParameter("desc"));
					attFile.setFileName(name);
					attFile.setContent(content);
					attFile.setAttachmentTypeID(Type.GetInt(request
							.getParameter("attachmentTypeID")));
					attFile.setRefID(Type.getString(request
							.getParameter("REFID")));
					if (Type.getInteger(request.getParameter("type"))
							.intValue() == WebKeys.Upload_Application_KzhbFile_Type) {
						attFile.setAttachmentTypeID(Type.GetInt(-1));
						attFile.setRefID(Type.getString(-1));
					}
					attFile.setKeyWords(request.getParameter("keywords"));

					attFile.setFilePath(directPathLocal);

					attFile.setFileSize(fileSize);
					attFile.setFis(inStream);
					int attID = service.doSaveAttachmentGetAttacheId(attFile);// 保存
					if (attID==0) {
						throw new RuntimeException("保存附件失败。");
					}
					List<String[]> sheetList = importExcelService.getSheetInfos(path);
					boolean flag = importExcelService.doSaveDeviceSheetImportInfo(attID,sheetList);
					if (!flag) {
						throw new RuntimeException("保存上传文件的工作表信息失败。");
					}
				}
			}

			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功");
			ajaxResponse.setMessage("上传成功！请在“设备清单导入文件”列表上单击选择文件做导入处理~");
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("导入失败！<br/>" + e.getMessage());
		}
		response.getWriter().write(ajaxResponse.toJsonString());
		return null;
	}
	/** 原导入设备清单excel 到项目
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 * @throws Exception
	 */
	
/*	@SuppressWarnings("rawtypes")
	public ActionForward importDeviceList2(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String directPath = request.getSession().getServletContext()
				.getRealPath("/");
		SimpleEmployee emp = WebUtil.getCurrentEmployee(request.getSession());
		String projectId = request.getParameter("projectId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		int count = 0;
		try {
			DiskFileUpload fu = new DiskFileUpload();
			fu.setSizeThreshold(1024 * 1024);
			fu.setRepositoryPath(directPath + "temp");
			List fileItems = fu.parseRequest(request);
			Iterator iter = fileItems.iterator();
			while (iter.hasNext()) {
				FileItem item = (FileItem) iter.next();
				// 忽略其他不是文件域的所有表单信息
				if (!item.isFormField()) {
					String name = item.getName();
					long size = item.getSize();
					String contentType = item.getContentType();
					if (!"application/vnd.ms-excel".equals(contentType)) {
						throw new RuntimeException("文件类型错误。请上传由模板创建的文件。");
					}
					if ((name == null || name.equals("")) && size == 0)
						continue;
					// 保存上传的文件到指定的目录
					String[] names = StringUtils.split(name, "\\"); // 对原来带路径的文件名进行分割
					name = names[names.length - 1];
					// item.write(new File(directPath + "temp/" + name));
					InputStream in = item.getInputStream();
					count = projectService.doImportDeviceList(projectId, in,
							emp);
					in.close();
				}
			}
		
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功");
			ajaxResponse.setMessage("成功导入" + count + "条数据！");
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("导入失败！<br/>" + e.getMessage());
		}
		response.getWriter().write(ajaxResponse.toJsonString());
		return null;
	}
*/
	public ActionForward getProjectSchedulePlan(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String projectId = request.getParameter("projectId");
		StandardQuery query = SimpleParam.buildSimpleParam("project.id",
				projectId, Type.STRING, RelationalOperator.EQ);
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = projectService.getProjectSchedulePlanByQuery(query,
				pageInfo);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward saveProjectSchedulePlan(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String projectId = request.getParameter("projectId");
		String plans = request.getParameter("plans");
		SimpleEmployee emp = WebUtil.getCurrentEmployee();
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			projectService.doSaveProjectSchedulePlan(projectId, plans, emp);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功");
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("保存项目进度计划失败！", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("保存失败！<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward deleteProjectSchedulePlan(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String id = request.getParameter("projectSchedulePlanId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			projectService.doDeleteProjectSchedulePlan(id);
			ajaxResponse.setSuccess(true);
		} catch (Exception e) {
			logger.error("保存项目进度计划失败！", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("删除失败！<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward saveProjectStatus(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		ProjectStatus status = new ProjectStatus();
		BeanUtils.populate(status, request.getParameterMap());
		if (status.getId() == 0) {
			status.setId(null);
		}
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			int id = projectService.doSaveProjectStatus(status);
			ajaxResponse.addAttribute("id", id);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功");
			ajaxResponse.setMessage("保存成功！");
		} catch (Exception e) {
			logger.error("保存项目状态失败！", e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("失败");
			ajaxResponse.setMessage("保存失败！<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	/**
	 * 获取项目目前应催款
	 */
	public ActionForward getReminderAmt(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String projectId = request.getParameter("projectId");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		double reminderAmt = projectService.getReminderAmt(projectId);
		ajaxResponse.setSuccess(true);
		ajaxResponse.addAttribute("reminderAmt", reminderAmt);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward updateProjectSchedulePlan(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		ProjectSchedulePlan plan = new ProjectSchedulePlan();
		DateConverter dateConverter = new DateConverter();
		dateConverter.setPattern("yyyy-MM-dd");
		ConvertUtils.register(dateConverter, java.util.Date.class);
		BeanUtils.populate(plan, request.getParameterMap());
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		projectService.doUpdateProjectSchedulePlan(plan);
		ajaxResponse.setSuccess(true);
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}
	
	
	/**
	 * 提交变更执行部门
	 */
	public ActionForward updateProjectExecutiveDept(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String projectID = request.getParameter("projectID");
		
		int executiveDeptID = Type.GetInt(request.getParameter("executiveDeptID"));
		String executiveDeptChangeReason = request.getParameter("executiveDeptChangeReason");
		
		int managerID = Type.GetInt(request.getParameter("managerID"));
		String managerChangeReason = request.getParameter("managerChangeReason");
		String isChangeDept = "false";
		String isChangeManager = "false";
		if(executiveDeptID != 0){
			isChangeDept = "true";
		}
		if(managerID != 0){
			isChangeManager = "true";
		}
		
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			if (!StringUtils.isEmpty(projectID)) {
				ProjectObj obj = projectService.getProjectInfoByID(projectID);
				request.setAttribute("project", obj);
			}
			projectService.doUpdateProjectExecutiveDept(projectID,executiveDeptID,executiveDeptChangeReason,
					managerID,managerChangeReason,isChangeDept,isChangeManager);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("提交成功");
			ajaxResponse.setMessageTitle("成功");
		}catch(EngineException e){                      //此流程只有修改执行部门的主任时候用到了表达式
			logger.error("提交变更报错",e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>变更后的执行部门没有主任级角色的任务人，请联系管理员~");
			ajaxResponse.setMessageTitle("失败");
		} catch (Exception e) {
			logger.error("提交变更报错",e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>"+e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
		return null;
	}
		
	
	/**
	 * 被退回后提交变更执行部门
	 */
	public ActionForward updateProjectExecutiveDeptByReturn(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String projectID = request.getParameter("projectID");
		String workItemId = request.getParameter("workItemId");
		
		int executiveDeptID = Type.GetInt(request.getParameter("executiveDeptID"));
		String executiveDeptChangeReason = request.getParameter("executiveDeptChangeReason");
		
		int managerID = Type.GetInt(request.getParameter("managerID"));
		String managerChangeReason = request.getParameter("managerChangeReason");
		
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			if (!StringUtils.isEmpty(projectID)) {
				ProjectObj obj = projectService.getProjectInfoByID(projectID);
				request.setAttribute("project", obj);
			}
			projectService.doUpdateProjectExecutiveDeptByReturn(executiveDeptID,executiveDeptChangeReason,
					managerID,managerChangeReason,workItemId);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("提交成功");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("提交变更报错",e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>"+e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}
		
		WebUtil.returnResponse(response, ajaxResponse.toString());
		return null;
	}
	
	
	/**
	 * 接收任务   变更执行部门
	 */
	public ActionForward getExecutiveDeptChangeNotice(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		
		String projectID = request.getParameter("projectID");
		int excecutiveDeptID = Type.GetInt(request.getParameter("executiveDeptID"));
		int managerID = Type.GetInt(request.getParameter("managerID"));
		
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String workItemId = request.getParameter("workItemId");
		if(excecutiveDeptID !=0){
			try {
				projectService.doAcceptProjectExecutiveDept(projectID,excecutiveDeptID,workItemId );
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.setMessage("变更执行部门任务已经完成。");
			} catch (Exception e) {
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessageTitle("错误");
				ajaxResponse.setMessage("发生错误，无法完成任务。<br/>" + e.getMessage());
			}
		}
		if(managerID !=0){
			try {
				projectService.doAcceptProjectManager(projectID,managerID,workItemId );
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.setMessage("变更项目经理已经完成。");
			} catch (Exception e) {
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessageTitle("错误");
				ajaxResponse.setMessage("发生错误，无法完成任务。<br/>" + e.getMessage());
			}
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}
	/** 批量 接收项目经理变更通知 未使用
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void batchAcceptProjectManager(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String workItemId = request.getParameter("workItemId");
			try {
				projectService.doBatchAcceptProjectManager(workItemId );
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.setMessage("变更项目经理已经完成。");
			} catch (Exception e) {
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessageTitle("错误");
				ajaxResponse.setMessage("发生错误，无法完成任务。<br/>" + e.getMessage());
			}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	
	/** 批量变更 未使用
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void getBatchExecutiveDeptChangeNotice(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String workItemId = request.getParameter("workItemId");
			try {
				projectService.doBatchAcceptProjectExecutiveDept(workItemId );
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessageTitle("成功");
				ajaxResponse.setMessage("变更执行部门任务已经完成。");
			} catch (Exception e) {
				ajaxResponse.setSuccess(false);
				ajaxResponse.setMessageTitle("错误");
				ajaxResponse.setMessage("发生错误，无法完成任务。<br/>" + e.getMessage());
			}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	/** 批量变更 未使用
	 */
	public void getBatchProjectManager(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String workItemId = request.getParameter("workItemId");
		try {
			projectService.doBatchAcceptProjectManager(workItemId );
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功");
			ajaxResponse.setMessage("变更项目经理已经完成。");
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("错误");
			ajaxResponse.setMessage("发生错误，无法完成任务。<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
	
		 
	/**
	 * 启动内部验收流程  
	 */
	public void startInterCheckToEngineer(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String projectID = request.getParameter("projectID");
		String checkDeptID = request.getParameter("checkDeptID");
		String comment = request.getParameter("comment");
		
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			if (!StringUtils.isEmpty(projectID)) {
				ProjectObj obj = projectService.getProjectInfoByID(projectID);
				request.setAttribute("project", obj);
			}
			projectService.doStartInterCheckToEngineer(projectID,checkDeptID,comment);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("提交成功");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("提交报错",e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>"+e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	
	/**
	 *  总工 、主任审批
	 */
	public ActionForward managerCheck(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		
		String isAgree = request.getParameter("isAgree");
		String comment = request.getParameter("comment");
		
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		String workItemId = request.getParameter("workItemId");
		try {
			projectService.doManagerRefuseCheck(isAgree,workItemId,comment);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功");
			ajaxResponse.setMessage("任务已经完成。");
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("错误");
			ajaxResponse.setMessage("发生错误，无法完成任务。<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}	

	
	/**
	 *   验收部门分配任务给成员  提交
	 */
	public void checkDeptAssignTask(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String workItemId = request.getParameter("workItemId");
		String projectID = request.getParameter("projectID");
		String comment = request.getParameter("comment");
		String divisionManagerID = request.getParameter("divisionManagerID");
		
		ProjectTaskObj ptask = new ProjectTaskObj();
		ptask.setTaskName(request.getParameter("taskName"));
		ptask.setProjectID(request.getParameter("projectID"));
		
		String memberJson = request.getParameter("members");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			if (!StringUtils.isEmpty(projectID)) {
				ProjectObj obj = projectService.getProjectInfoByID(projectID);
				request.setAttribute("project", obj);
			}
			projectService.doCheckDeptAssignTask(ptask, memberJson, workItemId,comment,divisionManagerID);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("提交成功");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("提交报错",e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>"+e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}
		
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	 
	/**
	 * 继续内部验收流程  
	 */
	public void goOnInterCheckToEngineer(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String projectID = request.getParameter("projectID");
		String workItemId = request.getParameter("workItemId");
		String comment = request.getParameter("comment");
		
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			if (!StringUtils.isEmpty(projectID)) {
				ProjectObj obj = projectService.getProjectInfoByID(projectID);
				request.setAttribute("project", obj);
			}
			projectService.doGoOnInterCheckToEngineer(workItemId,comment);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("提交成功");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("提交报错",e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>"+e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}

		WebUtil.returnResponse(response, ajaxResponse.toString());
	}
	

	/**启动内部验收流程的页面
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @return
	 * @throws Exception
	 */
	public ActionForward showPage(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String goOn = request.getParameter("isAgree");
		String projectID = request.getParameter("projectID");
		logger.info("getProjectInfo//projectId:" + projectID);
		ProjectObj project;
		try {
			project = projectService.getProjectInfoByID(projectID);
		} catch (Exception e) {
			throw e;
		}
		request.setAttribute("project", project);
		if(goOn != null ){
			return mapping.findForward("interCheckToEngineer");
		} else {
			return mapping.findForward("projectInfo");
		}
	}
	
	/**
	 *  验收员审批结果     提交给部门主任   
	 */
	public void tellDeptManager(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String workItemId = request.getParameter("workItemId");
		String projectID = request.getParameter("projectID");
		String isApproved = request.getParameter("isApprove");
		
		String comment = request.getParameter("comment");
		String checkManagerID = request.getParameter("checkManagerID");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			if (!StringUtils.isEmpty(projectID)) {
				ProjectObj obj = projectService.getProjectInfoByID(projectID);
				request.setAttribute("project", obj);
			}
			projectService.doTellDeptManager(comment,workItemId,isApproved,checkManagerID);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("提交成功");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("提交报错",e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>"+e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}
		
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}	
	
	
	/**
	 *  验收部门主任审批结果     提交给事业部主任   
	 */
	public void tellDivisionDeptManager(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
	throws Exception {
		String workItemId = request.getParameter("workItemId");
		String projectID = request.getParameter("projectID");
		String isApproved = request.getParameter("isApprove");
		
		String comment = request.getParameter("comment");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			if (!StringUtils.isEmpty(projectID)) {
				ProjectObj obj = projectService.getProjectInfoByID(projectID);
				request.setAttribute("project", obj);
			}
			projectService.doTellDivisionDeptManager(comment, workItemId,isApproved,projectID);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("提交成功");
			ajaxResponse.setMessageTitle("成功");
		} catch (Exception e) {
			logger.error("提交报错",e);
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("提交失败<br/>"+e.getMessage());
			ajaxResponse.setMessageTitle("失败");
		}
		
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}	
	
	public void getInterCheckList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String processInstanceId = request.getParameter("processInstanceId");
		String store = projectService.getInterCheckList(processInstanceId);
		WebUtil.returnResponse(response, store);
	}	
	/**
	 * 查询项目ID
	 * */
	public void getRelationProjectID(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		Integer contractId = Integer.valueOf(request.getParameter("contractId"));
		String store = projectService.getRelationProjectID(contractId);
		WebUtil.returnResponse(response, store);
	}
	
	
	public void getRelationChildContractCodeByID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response){
		String contractID = request.getParameter("contractID");
		String store = projectService.getRelationChildContractCodeByID(contractID);
		WebUtil.returnResponse(response, store);
	}
	
	
	public ActionForward getProjectQualificationName(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception{
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		String store = "";
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);
		store = projectService.getProjectQualificationName(query,pageInfo);
		WebUtil.returnResponse(response, store);
		return null;
	}
	
	
	public void delSheetInfoByAttID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response){
		String attID = request.getParameter("attID");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			importExcelService.delSheetInfoByAttID(Type.GetInt(attID));
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("删除成功！");
			ajaxResponse.setMessageTitle("提示");
		} catch (Exception e) {
			logger.error("删除失败。" + e.getMessage());
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("删除失败！");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
	}
}
