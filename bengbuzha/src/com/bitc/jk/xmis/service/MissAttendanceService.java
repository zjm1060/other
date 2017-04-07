package com.bitc.jk.xmis.service;

import java.io.IOException;
import java.util.Date;

import org.codehaus.jackson.JsonParseException;
import org.codehaus.jackson.map.JsonMappingException;
import org.fireflow.engine.EngineException;
import org.fireflow.kernel.KernelException;

import com.bitc.jk.xmis.model.MissAttendanceInfos;
import com.bitc.jk.xmis.model.MissAttendanceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;

public interface MissAttendanceService {

	String doStartMissAttence(MissAttendanceObj missAttendanceObj,
			String memberJson) throws WorkflowException, JsonParseException, JsonMappingException, IOException, EngineException, KernelException;

	public String getMissAttendanceList (StandardQuery query, PageInfo pageInfo, String todoIdsQuery)  throws Exception  ;

	public void doCreateTimerTask(String needId,String commandBeanName);

	public Date getExecuteTime();

	void doTimerTask(String wi) throws WorkflowException;

	String getMissAttendanceListByEmpId(String isOfficialTravel,
			String todoModelList, String wokItemId, String isTodo,
			PageInfo pageInfo) throws Exception;

	public String doApprovalTaskList(String workItemIds, String isAgree, String comment) throws WorkflowException, EngineException;
	/**删除统计待办
	 * @param workItemId
	 */
	public void deleteTodoCountByTodoId(String todoId);
	public String getMissAttendanceListByQuery(StandardQuery query, PageInfo pageInfo, String dateFirst, String dateLast) ;

	void doWarnInfo();

	String getMissAttendanceInfos(String workItemId);

	String doUpdateMissAttendanceInfo(MissAttendanceInfos info);
	void doDeleteMissAttendanceInfo( String info );

	MissAttendanceInfos getMissAttendanceInfoById(String infoId);

    public	String doGoOnSubmitMissAttence(String missAttendanceId,String workItemId) throws WorkflowException;

	void doEndMissAttence(String workItemId,String missAttendanceId) throws WorkflowException, EngineException;

	String doSaveMissAttence(String missAttendanceId, MissAttendanceObj missAttendanceObj,
			MissAttendanceInfos info);

	void doUpdateMissAttendanceRecordState(String infoId,int year, int month,int state);

	String getApprovalTaskListByQuery(StandardQuery query, PageInfo pageInfo);

//	void doResignTask(String workItemId) throws WorkflowException;

	public MissAttendanceObj getMissAttendanceObj(String processInstanceId);

//	public void doResignTasks(String workItemIds ) throws WorkflowException ;

}
