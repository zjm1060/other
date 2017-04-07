package com.bitc.jk.xmis.dao;


import java.util.List;

import com.bitc.jk.xmis.model.MissAttendanceInfos;
import com.bitc.jk.xmis.model.MissAttendanceObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface MissAttendanceDAO {

	void doCreateMissAttendanceInfos(MissAttendanceInfos missAttendanceInfos);

	String doCreateMissAttendanceObj(MissAttendanceObj missAttendanceObj);

	void doUpdateMissAttendanceInfos(MissAttendanceInfos missAttendanceInfos);

	//根据任务ID、接收人ID、创建日期 查找 满足条件的“统计的一条待办”
	Boolean isExistMissAttendanceList(String todoModel, String actorId,String createTime);

	@SuppressWarnings("rawtypes")
	List getMissAttendanceList(StandardQuery query, PageInfo pageInfo,String todoIdsQuery)  throws Exception  ;

	void doUpdateMissAttendanceObj(MissAttendanceObj missAttendanceObj);
	
	public String getValueFromParamTableByName(String name);

//	Boolean isExistMissAttendanceListByEmpId(String todoModel,
//			String actorId, String createTime);

	@SuppressWarnings("rawtypes")
	List getMissAttendanceListByEmpId(String isOfficialTravel,
			String todoModelList, String wokItemId, String isTodo,
			PageInfo pageInfo)  throws Exception ;
	@SuppressWarnings("rawtypes")
	public List getMissAttendanceListByQuery(StandardQuery query, PageInfo pageInfo, String dateFirst, String dateLast) ;

	@SuppressWarnings("rawtypes")
	List getMissAttendanceInfos(String workItemId);

	MissAttendanceInfos getMissAttendanceInfoById(String infoId);

	void doDeleteMissAttendanceInfos(MissAttendanceInfos info);

	MissAttendanceObj getMissAttendanceObjById(String missAttendanceId);

	void doDeleteMissAttendanceObj(MissAttendanceObj missAttendanceObj);

	boolean isExistMissAttendanceRecord(String infoId, int year, int month);

	void doUpdateMissAttendanceRecordState(String infoId, int year, int month,
			int state );

	void doCreateMissAttendanceRecord(String infoId, int year, int month,
			int state );

	List getApprovalTaskListByQuery(StandardQuery query, PageInfo pageInfo);

	public List<MissAttendanceObj> getMissAttendanceId(String processInstanceId);

	//得到中水科技所有人的邮箱地址
	public List<String> getAllMyCompEmailList();
}
