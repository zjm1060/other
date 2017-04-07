package com.bitc.jk.xmis.workflow.util;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.apache.log4j.Logger;
import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.IWorkItem;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.impl.TaskInstance;
import org.fireflow.engine.taskinstance.DynamicAssignmentHandler;
import org.fireflow.kernel.KernelException;
import org.fireflow.model.FormTask;
import org.wltea.expression.ExpressionEvaluator;
import org.wltea.expression.datameta.Variable;

import com.bitc.jk.xmis.util.TemplateUtil;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;

public class WorkflowUtil {

	private static Logger logger = Logger.getLogger(WorkflowUtil.class);

	/**
	 * 启动流程
	 * 
	 * @param workflowSession
	 *            工作流会话对象
	 * @param processName
	 *            流程定义的名称
	 * @param userId
	 *            启动人
	 * @return 流程实例
	 * @throws WorkflowException
	 */
	public static IProcessInstance startProcess(
			IWorkflowSession workflowSession, String processName, String userId)
			throws WorkflowException {
		IProcessInstance processInstance;
		try {
			processInstance = workflowSession.createProcessInstance(
					processName, userId);
			processInstance.run();
			return processInstance;
		} catch (Exception e) {
			throw new WorkflowException("", e);
		}
	}

	/**
	 * 添加流程实例变量
	 * 
	 * @param processInstance
	 *            流程实例
	 * @param key
	 *            键
	 * @param value
	 *            值
	 */
	public static void addProcessInstanceVariable(
			IProcessInstance processInstance, String key, Object value) {
		processInstance.setProcessInstanceVariable(key, value);
	}

	public static void addProcessInstanceVariable(IWorkflowSession session,
			String workItemId, String key, Object value) {
		addProcessInstanceVariable(getProcessInstance(session, workItemId),
				key, value);
	}

	public static void addProcessInstanceVariables(
			IProcessInstance processInstance, Map<String, Object> variables) {
		Set<String> keySet = variables.keySet();
		for (Iterator<String> it = keySet.iterator(); it.hasNext();) {
			String key = it.next();
			addProcessInstanceVariable(processInstance, key, variables.get(key));
		}
	}

	public static IProcessInstance getProcessInstance(IWorkflowSession session,
			String workItemId) {
		IWorkItem item = session.findWorkItemById(workItemId);
		return ((TaskInstance) item.getTaskInstance())
				.getAliveProcessInstance();
	}

	/**
	 * 为下一步指定接收人
	 * 
	 * @param workflowSession
	 *            工作流会话对象
	 * @param actorIds
	 *            接收人ID List
	 * @param performName
	 *            流程定义里在FormTask上设置的Performer Name属性
	 */
	public static void specifyActor(IWorkflowSession workflowSession,
			List<String> actorIds, String performName) {
		DynamicAssignmentHandler handler = new DynamicAssignmentHandler();
		handler.setActorIdsList(actorIds);
		handler.setNeedClaim(true);
		workflowSession.setAttribute(performName, handler);
	}

	/**
	 * @param workflowSession
	 *            工作流会话对象
	 * @param actorId
	 *            接收人ID
	 * @param performName
	 *            流程定义里在FormTask上设置的Performer Name属性
	 */
	public static void specifyActor(IWorkflowSession workflowSession,
			String actorId, String performName) {
		List<String> list = new ArrayList<String>();
		list.add(actorId);
		specifyActor(workflowSession, list, performName);
	}

	/**
	 * 完成流程的第一个节点的任务
	 * 
	 * @param workflowSession
	 *            工作流会话
	 * @param processInstance
	 *            流程实例
	 * @param actorId
	 *            任务人
	 * @throws EngineException
	 * @throws KernelException
	 */
	public static void completeFirstTask(IWorkflowSession workflowSession,
			IProcessInstance processInstance, String actorId, String comment)
			throws EngineException, KernelException {
		List<IWorkItem> items = workflowSession.findMyTodoWorkItems(actorId);
		for (IWorkItem item : items) {
			if (processInstance.getId().equals(
					item.getTaskInstance().getProcessInstanceId())) {
				item.claim();
				item.complete(comment);
				break;
			}
		}
	}

	/**
	 * 获取待办标题。格式为：【流程DisplayName】[节点 DisplayName]任务Displayname
	 * 
	 * @param workItem
	 *            工作项
	 * @return 格式化的待办标题
	 * @throws EngineException
	 */
	public static String getTodoTitle(IWorkItem workItem)
			throws EngineException {
		TaskInstance taskInstance = (TaskInstance) workItem.getTaskInstance();
		IProcessInstance processInstance = taskInstance
				.getAliveProcessInstance();
		String titleExp = taskInstance.getTask().getExtendedAttributes()
				.get(Constants.TODO_TITLE_EXP_KEY);
		String displayName = taskInstance.getDisplayName();
		if (titleExp != null) {
			displayName = executeTitle(processInstance, titleExp);
		}
		return new StringBuilder().append("【")
				.append(processInstance.getDisplayName()).append("】")
				.append("[")
				.append(taskInstance.getActivity().getDisplayName())
				.append("]").append(displayName).toString();
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	public static String executeTitle(IProcessInstance processInstance,
			String titleExp) {
		Map vars = processInstance.getProcessInstanceVariables();
		List variables = new ArrayList();
		for (Iterator it = vars.keySet().iterator(); it.hasNext();) {
			Object key = it.next();
			variables
					.add(Variable.createVariable(key.toString(), vars.get(key)));
		}
		return (String) ExpressionEvaluator.evaluate(titleExp, variables);
	}

	/**
	 * 获取工作项默认的FormUri
	 * 
	 * @param workItem
	 *            工作项
	 * @param processInstance
	 *            流程实例，用于获取流程实例变量，这些变量有可能用于Uri
	 * @return
	 * @throws WorkflowException
	 */
	public static String getDefaultFormUri(IWorkItem workItem,
			IProcessInstance processInstance) throws WorkflowException {
		try {
			FormTask task = (FormTask) workItem.getTaskInstance().getTask();
			String uri = null;
			String defaultView = task.getDefaultView();
			if (FormTask.VIEWFORM.equals(defaultView)) {
				uri = task.getViewForm().getUri();
			} else if (FormTask.EDITFORM.equals(defaultView)) {
				uri = task.getEditForm().getUri();
			} else if (FormTask.LISTFORM.equals(defaultView)) {
				uri = task.getListForm().getUri();
			}
			return formatUri(processInstance, task, uri);
		} catch (Exception e) {
			throw new WorkflowException(e);
		}
	}

	public static String getEditFormUri(IWorkItem workItem,
			IProcessInstance process) throws WorkflowException {
		try {
			FormTask task = (FormTask) workItem.getTaskInstance().getTask();
			String uri = task.getEditForm().getUri();
			return formatUri(process, task, uri);
		} catch (Exception e) {
			throw new WorkflowException(e);
		}
	}

	public static String getViewFormUri(IWorkItem workItem,
			IProcessInstance process) throws WorkflowException {
		try {
			FormTask task = (FormTask) workItem.getTaskInstance().getTask();
			String uri = task.getViewForm().getUri();
			return formatUri(process, task, uri);
		} catch (Exception e) {
			throw new WorkflowException(e);
		}
	}

	public static String getListFormUri(IWorkItem workItem,
			IProcessInstance process) throws WorkflowException {
		try {
			FormTask task = (FormTask) workItem.getTaskInstance().getTask();
			String uri = task.getListForm().getUri();
			return formatUri(process, task, uri);
		} catch (Exception e) {
			throw new WorkflowException(e);
		}
	}

	/**
	 * 格式化Uri，将其中大括号{}中的变量替换为实际的值
	 * 
	 * @param process
	 * @param task
	 * @param uri
	 * @return
	 * @throws Exception
	 */
	@SuppressWarnings("unchecked")
	private static String formatUri(IProcessInstance process, FormTask task,
			String uri) throws Exception {
		if (uri == null) {
			return "";
		}
		StringBuilder builder = new StringBuilder(uri);
		if (uri.indexOf('?') < 0) {
			builder.append("?");
		} else {
			builder.append("&");
		}
		builder.append("processTaskId={" + Constants.DEFAULT_FORMURI_VAR_TASKID)
				.append("}&processInstanceId={"
						+ Constants.DEFAULT_FORMURI_VAR_PROCESSINSTANCEID)
				.append("}");
		uri = builder.toString();
		Map<String, Object> map = process.getProcessInstanceVariables();
		map.put(Constants.DEFAULT_FORMURI_VAR_TASKID, task.getId());
		map.put(Constants.DEFAULT_FORMURI_VAR_PROCESSINSTANCEID,
				process.getId());
		return TemplateUtil.format(uri, map);
	}

	/**
	 * 批量处理任务 uri里的参数值
	 * 
	 * @param process
	 * @param task
	 * @param uri
	 * @return
	 * @throws Exception
	 */
	@SuppressWarnings({ "unused", "unchecked" })
	private static Map<String, Object> myFormatUri(IProcessInstance process,
			FormTask task) throws Exception {
		Map<String, Object> map = process.getProcessInstanceVariables();
		map.put(Constants.DEFAULT_FORMURI_VAR_TASKID, task.getId());
		map.put(Constants.DEFAULT_FORMURI_VAR_PROCESSINSTANCEID,
				process.getId());
		return map;
	}

	/**
	 * 完成流程工作项
	 * 
	 * @param wflsession
	 * @param todoDAO
	 * @param workItemId
	 * @param userId
	 *            用户ID
	 * @throws WorkflowException
	 */
	public static void completeTask(IWorkflowSession wflsession,
			String workItemId, String userId, String comment)
			throws WorkflowException {
		IWorkItem wi = wflsession.findWorkItemById(workItemId);
		if (wi == null) {
			throw new WorkflowException("完成任务出错，不存在该任务（" + workItemId + "）");
		}
		logger.info("----------completeTask------------"+wi.getState());
		if (wi.getState() != IWorkItem.RUNNING) {
			throw new WorkflowException("完成任务出错，任务（" + workItemId
					+ "）可能已经完成，或者尚未签收。");
		}
//		if (!userId.equals(wi.getActorId())) {
//			throw new WorkflowException("完成任务出错，该任务（" + workItemId
//					+ "）的任务人不是当前用户");
//		}
		try {
			wi.complete(comment);
			if (logger.isInfoEnabled()) {
				logger.info("完成任务项成功//WorkItemId:" + workItemId + "//userId:"
						+ userId);
			}
		} catch (EngineException e) {
			throw new WorkflowException(e);
		} catch (KernelException e) {
			throw new WorkflowException(e);
		}

	}
	public static void jumpToTask(IWorkflowSession wflsession,
			String workItemId, String userId, String comment)
			throws WorkflowException {
		IWorkItem wi = wflsession.findWorkItemById(workItemId);
		if (wi == null) {
			throw new WorkflowException("完成任务出错，不存在该任务（" + workItemId + "）");
		}
		if (wi.getState() != IWorkItem.RUNNING) {
			throw new WorkflowException("完成任务出错，任务（" + workItemId
					+ "）可能已经完成，或者尚未签收。");
		}
//		if (!userId.equals(wi.getActorId())) {
//			throw new WorkflowException("完成任务出错，该任务（" + workItemId
//					+ "）的任务人不是当前用户");
//		}
		try {
			wi.jumpTo("MissAttence.ApplyForAtendance");
			if (logger.isInfoEnabled()) {
				logger.info("任务跳转//WorkItemId:" + workItemId + "//userId:"
						+ userId);
			}
		} catch (EngineException e) {
			throw new WorkflowException(e);
		} catch (KernelException e) {
			throw new WorkflowException(e);
		}

	}
}
