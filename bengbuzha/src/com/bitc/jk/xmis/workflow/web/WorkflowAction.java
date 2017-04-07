package com.bitc.jk.xmis.workflow.web;

import java.io.InputStream;
import java.sql.Date;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import net.sf.json.JSONObject;

import org.apache.commons.beanutils.BeanUtils;
import org.apache.commons.fileupload.DiskFileUpload;
import org.apache.commons.fileupload.FileItem;
import org.apache.commons.fileupload.FileUpload;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.apache.struts.action.ActionForm;
import org.apache.struts.action.ActionForward;
import org.apache.struts.action.ActionMapping;
import org.apache.struts.actions.DispatchAction;
import org.fireflow.engine.definition.WorkflowDefinition;
import org.fireflow.model.WorkflowProcess;
import org.fireflow.model.io.Dom4JFPDLParser;

import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.ExtjsAjaxResponse;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;
import com.bitc.jk.xmis.workflow.model.Attention;
import com.bitc.jk.xmis.workflow.model.AuthorizeInfo;
import com.bitc.jk.xmis.workflow.model.WorkItemHistory;
import com.bitc.jk.xmis.workflow.service.AttentionService;
import com.bitc.jk.xmis.workflow.service.WorkflowService;

public class WorkflowAction extends DispatchAction {

	private static final Logger logger = Logger.getLogger(WorkflowAction.class);

	private WorkflowService workflowService;

	private AttentionService attentionService;

	/**
	 * 签收工作项
	 */
	public ActionForward claim(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String workItemId = request.getParameter("workItemId");
		String userId = String
				.valueOf(WebUtil.getCurrentEmployee().getUserID());
		ExtjsAjaxResponse message = new ExtjsAjaxResponse();
		try {
			workflowService.doClaimTaskList(workItemId, userId);
			// workflowService.doClaimTask(workItemId, userId);
			message.setSuccess(true);
			message.setMessageTitle("成功");
			message.setMessage("任务签收成功");
		} catch (WorkflowException e) {
			logger.error(e.getMessage());
			message.setSuccess(false);
			message.setMessage(e.getMessage());
			message.setMessageTitle("任务签收失败");
		}
		WebUtil.returnResponse(response, message.toJsonString());
		return null;
	}

	/**
	 * 拒收工作项
	 */
	public ActionForward reject(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String workItemId = request.getParameter("workItemId");
		String comments = request.getParameter("comments");
		String userId = String
				.valueOf(WebUtil.getCurrentEmployee().getUserID());
		ExtjsAjaxResponse message = new ExtjsAjaxResponse();
		try {
			workflowService.doRejectTask(workItemId, userId, comments);
			message.setSuccess(true);
			message.setMessageTitle("成功");
			message.setMessage("任务拒收成功");
		} catch (WorkflowException e) {
			logger.error(e.getMessage());
			message.setSuccess(false);
			message.setMessage(e.getMessage());
			message.setMessageTitle("任务拒收失败");
		}

		WebUtil.returnResponse(response, message.toJsonString());
		return null;
	}

	/**
	 * 拒收工作项
	 */
	public ActionForward withdraw(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String workItemId = request.getParameter("workItemId");
		String comments = request.getParameter("comments");
		String userId = String
				.valueOf(WebUtil.getCurrentEmployee().getUserID());
		ExtjsAjaxResponse message = new ExtjsAjaxResponse();
		try {
			workflowService.doWithdrawTask(workItemId, userId, comments);
			message.setSuccess(true);
			message.setMessageTitle("成功");
			message.setMessage("任务拒收成功");
		} catch (WorkflowException e) {
			logger.error(e.getMessage());
			message.setSuccess(false);
			message.setMessage(e.getMessage());
			message.setMessageTitle("任务拒收失败");
		}

		WebUtil.returnResponse(response, message.toJsonString());
		return null;
	}

	public ActionForward openForm(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String workItemId = request.getParameter("workItemId");
		String userId = String
				.valueOf(WebUtil.getCurrentEmployee().getUserID());
		try {
			String uri = workflowService.getFormUri(workItemId, userId);
			return new ActionForward(uri);
		} catch (WorkflowException e) {
			logger.error("打开待办任务错误", e);
			return error(mapping, form, request, response, "打开待办任务错误",
					e.getMessage());
		}

	}

	public ActionForward getWfCalendar(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String year = request.getParameter("year");
		String month = request.getParameter("month");
		String store = workflowService.doGetWfCalendarStore(
				Integer.parseInt(year), Integer.parseInt(month));
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward saveHoliday(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String year = request.getParameter("year");
		String month = request.getParameter("month");
		String[] holiday = request.getParameterValues("holiday");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			workflowService.doSaveHoliday(year, month, holiday);
			ajaxResponse.setSuccess(true);
		} catch (Exception e) {
			logger.error(e.getMessage());
			ajaxResponse.setSuccess(false);
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
		return null;
	}

//	public ActionForward getTodoList(ActionMapping mapping, ActionForm form,
//			HttpServletRequest request, HttpServletResponse response)
//			throws Exception {
//		SimpleEmployee employee = WebUtil.getCurrentEmployee(request
//				.getSession(false));
//		PageInfo pageInfo = WebUtil.generatePageInfo(request);
//		String state = request.getParameter("state");
//		logger.info("-------//getTodoList----state=" + state);
//		StandardQuery stateQuery = null;
//		if (StringUtils.isEmpty(state)) {
//			stateQuery = SimpleParam.buildSimpleParam("state", 10,
//					Type.INTEGER, RelationalOperator.LT);
//		} else {
//			stateQuery = SimpleParam
//					.buildSimpleParam("state", Integer.valueOf(state),
//							Type.INTEGER, RelationalOperator.EQ);
//			
//			
//		}
//		// 模糊查询
//		StandardQuery queryyInkling = WebUtil
//				.generateStandardQueryByInkling(request);
//		StandardQuery query = CompositParam
//				.buildAndCompositParam()
//				.addStandardQuery(
//						SimpleParam.buildSimpleParam("userID",
//								employee.getUserID() + "", Type.STRING,
//								RelationalOperator.EQ))
//				.addStandardQuery(
//						SimpleParam.buildSimpleParam("isVisible", 1,
//								Type.INTEGER, RelationalOperator.EQ))
//				.addStandardQuery(stateQuery).addStandardQuery(queryyInkling);
//		String store = workflowService.getTodoListByQuery(query, pageInfo);
//		WebUtil.returnResponse(response, store);
//		return null;
//	}

	public ActionForward getTodoInfo(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String todoId = request.getParameter("todoId");
		String store = workflowService.getTodoInfo(todoId);
		WebUtil.returnResponse(response, store);
		return null;
	}

	private ActionForward error(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response,
			String title, String msg) throws Exception {
		request.setAttribute("errorTitle", title);
		request.setAttribute("errorMsg", msg);
		return mapping.findForward("error");
	}

	public ActionForward getAttention(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		Attention attention = new Attention();
		BeanUtils.populate(attention, request.getParameterMap());
		if (!StringUtils.isEmpty(attention.getId())) {
			attention = attentionService.getAttentionById(attention.getId());
		} else {
			attention = attentionService.findAttentionByExample(attention);
		}
		if (attention == null) {
			attention = new Attention();
		}
		ajaxResponse.setSuccess(true);
		ajaxResponse.addAttribute("attention", attention);
		WebUtil.returnResponse(response, ajaxResponse.toString());
		return null;
	}

	public ActionForward payAttention(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		Attention attention = new Attention();
		BeanUtils.populate(attention, request.getParameterMap());
		boolean isAttention = Boolean.parseBoolean(request
				.getParameter("isAttention"));
		if (!isAttention) {
			attention.setCreateTime(DateUtil.getCurrentDateTime());
		}
		try {
			attentionService.doPayAttention(attention, isAttention);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessage("操作成功。");
			ajaxResponse.setMessageTitle("提示");
			ajaxResponse.addAttribute("attention", attention);
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("添加关注失败！请联系管理员。<br/>" + e.getMessage());
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward getAttentionList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		SimpleEmployee employee = WebUtil.getCurrentEmployee(request
				.getSession(false));
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		StandardQuery query = SimpleParam.buildSimpleParam("userID",
				employee.getUserID() + "", Type.STRING, RelationalOperator.EQ);
		String store = attentionService
				.getAttentionListByQuery(query, pageInfo);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public ActionForward getWorkItemHistoryList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String queryJson = request.getParameter("queryJson");
		WorkItemHistory example = null;
		if (queryJson != null) {
			example = (WorkItemHistory) JSONObject.toBean(
					JSONObject.fromObject(queryJson), WorkItemHistory.class);
		}
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		StandardQuery querys = CompositParam.buildForExample(example);

		logger.info("---------//getWorkItemHistoryList----querys=" + querys);
		String agentId = request.getParameter("agentId");
		String agentState = request.getParameter("agentState");
		StandardQuery agentQuery = null; 
		if (agentId != null && !agentId.equals("") && agentState != null
				&& !agentState.equals("")) {
			agentQuery = CompositParam
					.buildAndCompositParam()
					.addStandardQuery(
							SimpleParam.buildSimpleParam("agent.userID",
									Type.getInteger(agentId), Type.INTEGER,
									RelationalOperator.EQ))
					.addStandardQuery(
							SimpleParam.buildSimpleParam("state",
									Type.getInteger(agentState), Type.INTEGER,
									RelationalOperator.EQ));
			querys = CompositParam.buildOrCompositParam()
					.addStandardQuery(querys).addStandardQuery(agentQuery);
		}
		StandardQuery queryyInkling = WebUtil
				.generateStandardQueryByInkling(request);
		StandardQuery query = CompositParam.buildAndCompositParam()
				.addStandardQuery(querys).addStandardQuery(queryyInkling);
		String store = workflowService.getWorkItemHistoryListStore(query,
				pageInfo);
		WebUtil.returnResponse(response, store);
		return null;
	}

	public void getHaveDoneHistoryListStore(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String itemState = request.getParameter("itemState");
		logger.info("---------//getHaveDoneHistoryListStore----itemState="
				+ itemState);

		EmployeeObj employee = WebUtil.getCurrentEmployee();
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		StandardQuery querys = CompositParam.buildAndCompositParam()
		// .addStandardQuery(
		// SimpleParam.buildSimpleParam("任务人ID",
		// employee.getUserID() + "", Type.STRING,
		// RelationalOperator.EQ))
				.addStandardQuery(
						SimpleParam.buildSimpleParam("状态 ",
								Type.GetInt(itemState), Type.INTEGER,
								RelationalOperator.EQ));
		StandardQuery queryOR = CompositParam
				.buildOrCompositParam()
				.addStandardQuery(
						SimpleParam.buildSimpleParam("任务人ID",
								employee.getUserID() + "", Type.STRING,
								RelationalOperator.EQ))
				.addStandardQuery(
						SimpleParam.buildSimpleParam("代理人ID",
								employee.getUserID() + "", Type.STRING,
								RelationalOperator.EQ));

		StandardQuery queryyInkling = WebUtil
				.generateStandardQueryByInkling(request);
		StandardQuery query = CompositParam.buildAndCompositParam()
				.addStandardQuery(querys).addStandardQuery(queryyInkling)
				.addStandardQuery(queryOR);
		String store = workflowService.getHaveDoneHistoryListStore(query,
				pageInfo);
		WebUtil.returnResponse(response, store);
	}

	public void getTodoListSQL(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String state = request.getParameter("state");
		logger.info("---------//getTodoListSQL----itemState=" + state);

		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		StandardQuery querys = null;
		if (StringUtils.isEmpty(state)) {
			querys = SimpleParam.buildSimpleParam("state", 10,
					Type.INTEGER, RelationalOperator.LT);
		} else if("0".equals(state) || "1".equals(state)) {
			querys = CompositParam.buildOrCompositParam().addStandardQuery(
					SimpleParam.buildSimpleParam(
							"state",
//							"todo.状态 ",
							0, Type.INTEGER,
							RelationalOperator.EQ)).addStandardQuery(
									SimpleParam.buildSimpleParam(
											"state",
//											"todo.状态 ",
											1, Type.INTEGER,
											RelationalOperator.EQ));
		} else {
			querys = SimpleParam
					.buildSimpleParam("state", Integer.valueOf(state),
							Type.INTEGER, RelationalOperator.EQ);
			
			
		}
		StandardQuery queryyInkling = WebUtil
				.generateStandardQueryByInkling(request);
		StandardQuery query = CompositParam.buildAndCompositParam()
				.addStandardQuery(querys).addStandardQuery(queryyInkling);
		String store = "";
		//用于手机端的APP
		if(Type.GetInt(request.getParameter("isApp"))== 1 ){
			store = workflowService.getTodoListSQLByApp(query, pageInfo);
		}else{
			store = workflowService.getTodoListSQL(query, pageInfo);
		}
		WebUtil.returnResponse(response, store);
	}

	public ActionForward getProcessDefinitionXML(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String processId = request.getParameter("processId");
		String xml = null;
		if (processId == null) {
			String processInstanceId = request
					.getParameter("processInstanceId");
			xml = workflowService
					.getProcessDefinitionXMLByInstanceId(processInstanceId);
		} else {
			xml = workflowService.getProcessDefinitionXML(processId, null);
		}
		WebUtil.returnResponseForXML(response, xml);
		return null;
	}

	public ActionForward getHistoryTrace(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String processInstanceId = request.getParameter("processInstanceId");
		String store = workflowService.getHistoryTrace(processInstanceId);
		WebUtil.returnResponse(response, store);
		return null;
	}

	/**
	 * 获取候选任务人
	 */
	public ActionForward getCandidate(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String expression = request.getParameter("expression");
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			List<SimpleEmployee> employees = workflowService
					.getCandidate(expression);
			ajaxResponse.setSuccess(true);
			ajaxResponse.addAttribute("employees", employees);
		} catch (Exception e) {
			logger.error(e.getMessage());
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("错误");
			ajaxResponse.setMessage("获取候选任务人出错。<br/>" + e.getMessage());
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
		return null;
	}

	@SuppressWarnings("unchecked")
	public ActionForward deployProcess(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		String directPath = request.getSession().getServletContext()
				.getRealPath("/");
		EmployeeObj emp = WebUtil.getCurrentEmployee(request.getSession(false));
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		WorkflowDefinition workflowdef = new WorkflowDefinition();
		try {
			if (!FileUpload.isMultipartContent(request)) {
				throw new WorkflowException("上传文件出错！");
			}
			DiskFileUpload fu = new DiskFileUpload();
			fu.setSizeThreshold(2 * 1024 * 1024);// 2M
			fu.setRepositoryPath(directPath + "temp");
			List<FileItem> fileItems = fu.parseRequest(request);
			for (FileItem fileItem : fileItems) {
				if (fileItem.isFormField()) {
					continue;
				}
				InputStream in = fileItem.getInputStream();
				Dom4JFPDLParser parser = new Dom4JFPDLParser();
				WorkflowProcess workflowProcess = parser.parse(in);
				workflowdef.setWorkflowProcess(workflowProcess);
				in.close();
				break;
			}
			workflowdef.setUploadUser(String.valueOf(emp.getUserID()));
			Date currentDateTime = DateUtil.getCurrentDateTime();
			workflowdef.setState(true);
			workflowdef.setUploadTime(currentDateTime);
			workflowdef.setPublishUser(String.valueOf(emp.getUserID()));
			workflowdef.setPublishTime(currentDateTime);
			workflowService.doDeploy(workflowdef);
			ajaxResponse.setSuccess(true);
			ajaxResponse.setMessageTitle("成功");
			ajaxResponse.setMessage("流程发布成功！");
		} catch (Exception e) {
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessageTitle("操作失败");
			ajaxResponse.setMessage("发布流程失败！<br/>" + e.getMessage());
		}
		response.getWriter().write(ajaxResponse.toJsonString());
		return null;
	}

	public ActionForward getDeployedProcessList(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) throws Exception {
		String store = workflowService.getDeployedProcessList();
		WebUtil.returnResponse(response, store);
		return null;
	}

	public WorkflowService getWorkflowService() {
		return workflowService;
	}

	public void setWorkflowService(WorkflowService workflowService) {
		this.workflowService = workflowService;
	}

	public AttentionService getAttentionService() {
		return attentionService;
	}

	public void setAttentionService(AttentionService attentionService) {
		this.attentionService = attentionService;
	}

	/**
	 * 供管理员查看的流程列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 */
	public void getProcessInstanceList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);

		String processname = request.getParameter("PROCESS_ID");
		if (!StringUtils.isEmpty(processname)
				&& !StringUtils.isEmpty(processname.trim())
				&& !"-1".equals(processname)) {
			processname = processname.trim();
			query = CompositParam
					.buildAndCompositParam()
					.addStandardQuery(
							SimpleParam.buildSimpleParam("PROCESS_ID",
									processname, Type.STRING,
									RelationalOperator.EQ))
					.addStandardQuery(query);
		}

		String store = workflowService.getProcessInstanceList(query, pageInfo);
		WebUtil.returnResponse(response, store);
	}

	public void getProcessInstanceComb(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		try {
			List list = workflowService.getProcessInstanceComb();
			String store = WebUtil.buildJsonStoreByList(list);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void getTaskList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		try {
			PageInfo pageInfo = WebUtil.generatePageInfo(request);
			String task_ID = request.getParameter("task_ID");
			// StandardQuery query = WebUtil
			// .generateStandardQueryByInkling(request);
			SimpleEmployee employee = WebUtil.getCurrentEmployee(request
					.getSession(false));
			StandardQuery query = CompositParam
					.buildAndCompositParam()
					.addStandardQuery(
							SimpleParam.buildSimpleParam("用户ID",
									employee.getUserID() + "", Type.STRING,
									RelationalOperator.EQ))
					.addStandardQuery(
							SimpleParam.buildSimpleParam("task_ID", task_ID,
									Type.STRING, RelationalOperator.EQ));
			String store = workflowService.getTaskListByQuery(query, pageInfo);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public ActionForward getTileAndTaskIDByUserID(ActionMapping mapping,
			ActionForm form, HttpServletRequest request,
			HttpServletResponse response) {
		try {
			SimpleEmployee employee = WebUtil.getCurrentEmployee(request
					.getSession(false));
			Map<String, String> map = workflowService
					.getTileAndTaskIDByUserID(employee.getUserID());
			String titles = map.get("titles");
			String taskIds = map.get("taskIds");
			// String titles = "接收#确认任务/添加项目成员#确认任务";
			// String taskIds =
			// "ProjectTaskAssign.Accept.Accept#StartProject.Manager.confirm#StartProject.memberConfirm.confirm";

			request.setAttribute("t1", titles);
			request.setAttribute("t2", taskIds);

		} catch (Exception e) {
			e.printStackTrace();
		}
		return mapping.findForward("myTasks");
	}

	public void getApprovalInfos(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response) {
		try {
			String processInstanceId = request
					.getParameter("processInstanceId");
			String store = workflowService.getApprovalInfos(processInstanceId);
			WebUtil.returnResponse(response, store);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 获取授权信息列表
	 * 
	 * @param mapping
	 * @param form
	 * @param request
	 * @param response
	 * @throws Exception
	 */
	public void getAuthorizeList(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		PageInfo pageInfo = WebUtil.generatePageInfo(request);
		SimpleEmployee employee = WebUtil.getCurrentEmployee(request
				.getSession(false));

		StandardQuery query = WebUtil.generateStandardQueryByInkling(request);

		StandardQuery querys = null;
		if (employee.getUserID() == -1) {
			querys = CompositParam.buildAndCompositParam().addStandardQuery(
					query);

		} else {
			StandardQuery queryOr = CompositParam.buildOrCompositParam().addStandardQuery(
					SimpleParam.buildSimpleParam("authorizerID",
							employee.getUserID(), Type.INTEGER,
							RelationalOperator.EQ)).addStandardQuery(
									SimpleParam.buildSimpleParam("authorizeeID",
											employee.getUserID(), Type.INTEGER,
											RelationalOperator.EQ));
			querys = CompositParam
					.buildAndCompositParam()
					.addStandardQuery(queryOr)
					.addStandardQuery(query);

		}

		String store = workflowService.getAuthorizeList(querys, pageInfo);
		WebUtil.returnResponse(response, store);
	}

	public void cancelAuthorize(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		boolean result = false;
		try {
			String id = request.getParameter("id");
			result = workflowService.doCancelAuthorizeById(id);
			if (result) {
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessage("撤销成功！");
				ajaxResponse.setMessageTitle("提示");
			} else {
				ajaxResponse.setSuccess(true);
				ajaxResponse.setMessage("撤销失败");
				ajaxResponse.setMessageTitle("提示");
			}
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("撤销失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

	public void addAuthorize(ActionMapping mapping, ActionForm form,
			HttpServletRequest request, HttpServletResponse response)
			throws Exception {
		ExtjsAjaxResponse ajaxResponse = new ExtjsAjaxResponse();
		try {
			String id = request.getParameter("id");
			String authorizeeID = request.getParameter("authorizeeID");
			String authorizerID = request.getParameter("authorizerID");
			String process_ID = request.getParameter("process_ID");
			String availableDate = request.getParameter("availableDate");
			String result = request.getParameter("results");
			String comment = request.getParameter("comment");
			AuthorizeInfo info = new AuthorizeInfo();
			
			info.setId(id);
			info.setProcessId(process_ID);
			info.setAuthorizee(new SimpleEmployee(Type.GetInt(authorizeeID)));
			info.setAuthorizer(new SimpleEmployee(Type.GetInt(authorizerID)));
			info.setAvailableDate(Type.getDate(availableDate));
			info.setResult(result);
			info.setComment(comment);
			info.setCancelDate(Type.getDate(request.getParameter("cancelDate")));
			if(request.getParameter("createDate")!=null &&!"".equals(request.getParameter("createDate"))){
				info.setCreateDate(DateUtil.parseDateTime(request.getParameter("createDate")));
			}
			
			boolean isSuccess = workflowService.doAddAuthorize(info);
			ajaxResponse.setSuccess(true);
			if(isSuccess){
				ajaxResponse.setMessage("授权成功！");
			}else{
				ajaxResponse.setMessage("此次授权无效！被授权的流程不允许二次授权");
			}
			ajaxResponse.setMessageTitle("提示");
		} catch (Exception e) {
			e.printStackTrace();
			ajaxResponse.setSuccess(false);
			ajaxResponse.setMessage("授权失败");
			ajaxResponse.setMessageTitle("提示");
		}
		WebUtil.returnResponse(response, ajaxResponse.toString());
	}

}
