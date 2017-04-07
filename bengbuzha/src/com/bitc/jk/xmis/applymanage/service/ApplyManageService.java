package com.bitc.jk.xmis.applymanage.service;

import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.math.BigDecimal;
import java.text.ParseException;
import java.util.List;
import java.util.Map;

import com.jspsmart.upload.SmartUploadException;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.codehaus.jackson.JsonGenerationException;
import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.kernel.KernelException;

import com.bitc.jk.xmis.applymanage.model.AbsentForm;
import com.bitc.jk.xmis.applymanage.model.ApplyCarForm;
import com.bitc.jk.xmis.applymanage.model.ApplyMeetingRoomForm;
import com.bitc.jk.xmis.applymanage.model.ApplyOfficialSuppliesForm;
import com.bitc.jk.xmis.applymanage.model.ApplyOfficialSuppliesRepairForm;
import com.bitc.jk.xmis.applymanage.model.BusinessEntertainment;
import com.bitc.jk.xmis.applymanage.model.CeyaInfo;
import com.bitc.jk.xmis.applymanage.model.DocumentAppFormDetail;
import com.bitc.jk.xmis.applymanage.model.DocumentCiculatedInstances;
import com.bitc.jk.xmis.applymanage.model.EarlyPlans;
import com.bitc.jk.xmis.applymanage.model.GczlInfo;
import com.bitc.jk.xmis.applymanage.model.TheEndOfThePlanned;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

public interface ApplyManageService {

	public String doSaveAppMeetingRoom(ApplyMeetingRoomForm applyMeetingRoom);

	public String getApplyMeetingRoomFormById(String id);

	public String getAppMeetingRoomFormList(PageInfo pageInfo, String queryStr);

	public String getMeetingRoomCombo();

	public int doHandleAppMeetingRoomStatus(String id,int status_id);

	public boolean getMeetingRoomUsingTimeList(String type,String room_id,String new_begin_datetime,String new_end_datetime, String form_id);

	public String getApplyMeetingFormTipsList();

	public String getAppCarList(PageInfo pageInfo, String queryStr);

	public String getVehicleManagementListToSelectGrid();

	public String doSaveAppCars(ApplyCarForm applyCarForm, String caridStrs);

	public String getApplyCarByFormId(String form_id, EmployeeObj emp);

	public String getApplyCarIdsDetalsByFormId(String form_id);

	public boolean validBeginEndTime(String now_begin_datetime,
			String now_end_datetime);

	public String getApplyOfficialSuppliesList(String form_id);

	public String getOfficialSuppliesTypeCombo();

	public String getOfficialSuppliesCombo(String type);

	public String doSaveApplyOfficialSuppliesForm(
			ApplyOfficialSuppliesForm applyForm);

	public void doUpdateApplyOfficialSuppliesForm(
			ApplyOfficialSuppliesForm applyForm, String form_id, String supplies_id);

	public void doDeleteApplyOfficialSuppliesForm(String form_id,
			String supplies_id);

	public String getOfficalSuppliesByCompositeIds(String form_id,
			String supplies_id);

	public String doStartApplyOfficialSupplies(String form_id) throws WorkflowException, EngineException, KernelException;

	public String doApprovalTask(String workItemIds, String isAgree,
			String comment) throws WorkflowException;
	public String doApprovalTask2(String workItemIds, String isAgree,
			String comment) throws WorkflowException;

	public void doEndApply(String workItemId,String forceStop, String processInstanceId) throws WorkflowException, EngineException ;
	
	public String doGoOnSubmitApplyOfficialSupplies(
			String workItemId) throws WorkflowException;

	public String getApplyOfficialSuppliesListQuery(PageInfo pageInfo,
			String queryStr);

	public String getApplyOfficialSuppliesListSum(PageInfo pageInfo, String queryStr);

	public String doStartApplyOfficialSuppliesRepair(String form_id) throws EngineException, KernelException, WorkflowException;

	public String getApplyOfficialSuppliesRepairListSum(PageInfo pageInfo,
			String queryStr);
	
	public String getGczlInfoList(PageInfo pageInfo,
			String queryStr);

	public String getApplyOfficialSuppliesRepairList(String form_id);

	public void doUpdateApplyOfficialSuppliesRepairForm(
			ApplyOfficialSuppliesRepairForm applyForm, String form_id_param,
			String supplies_id_param);

	public void doDeleteApplyOfficialSuppliesRepairForm(String form_id,
			String supplies_id);

	public String doSaveApplyOfficialSuppliesRepairForm(
			ApplyOfficialSuppliesRepairForm applyForm);

	public void doEndApplyOfficialSuppliesUpdateNum(String workItemId,
			String form_id, String processInstanceId) throws WorkflowException, EngineException;

	public void updateLastActorToNull(String processInstanceId);

	public String doStartApplyBusinessEntertainment(BusinessEntertainment businessEntertainment)
	throws EngineException, KernelException, WorkflowException ;

	public String getBusinessEntertainmentFormByFormId(String form_id);

	public void doForceStopApplyBusinessEntertainment(String workItemId, String forceStop,
			String processInstanceId, String businessEntertentWorkedId, String form_id) throws WorkflowException, EngineException;

	public void doGoOnSubmitBusinessEntertainment(
			BusinessEntertainment businessEntertainment, String workItemId) throws WorkflowException;

	public void doUpdateApplyBusinessEntertainmentByCanteen(
			BusinessEntertainment businessEntertainment, String workItemId) throws WorkflowException;

	public void doUpdateSignNoByFormId(String form_id, String sign_No);

	public String getBusinessEntertainmentFormList(String queryStr,
			PageInfo pageInfo);

	public String doStartAskForLeave(AbsentForm absentForm) throws WorkflowException, EngineException, KernelException;

	public void doGoOnSubmitAskForLeave(AbsentForm absentForm, String workItemId) throws WorkflowException;

	public void doUpdateAbsentForm(AbsentForm absentForm, String workItemId) throws WorkflowException;

	public String getAbsentFormByFormId(String form_id);

	public String getAbsentDaysSum(String plan_begin_date,
			String plan_end_date, int plan_begin_hour, int plan_end_hour);

	public String getAbsentFormList(PageInfo pageInfo, String queryStr);

	public String getAttachmentListByAbsentFormId(String form_id);

	public String getDocumentTypeInfoCombo();

	public String doSaveDocumentAppFormDetail(DocumentAppFormDetail applyForm);

	public String getDocumentAppFormDetailByFormId(String form_id);

	public String doStartApplyDocumentAppFormDetail(String form_id, int type) throws WorkflowException, EngineException, KernelException;

	public String doApprovalTaskWithFormInfo(String workItemId, String isAgree,
			String comment, String step, String form_id) throws WorkflowException;

	public String doSubmitAfterUpdateApplyDocumentAppFormDetail(
			DocumentAppFormDetail applyForm, String workItemId) throws WorkflowException;

	public void doInsertDocumentCirculatedInstances(
			DocumentCiculatedInstances instancesObj, String to_id_str);

	public String getDocumentCirculatedInstancesList(String form_id);

	public String getDocumentAppFormDetailList(PageInfo pageInfo,
			String queryStr);

	public String doStartMonthPlan(EarlyPlans earlyPlans) throws WorkflowException, EngineException, KernelException;

	public void doGoOnSubmitMonthPlan(EarlyPlans earlyPlans, String workItemId)throws WorkflowException;

	public String getMonthPlanByFormId(String form_id);

	public void doUpdateMonthPlanForm(TheEndOfThePlanned endOfThePlanned,
			String workItemId) throws WorkflowException;

	public void doUpdateMonthPlanComments(EarlyPlans earlyPlans);

	public void doUpdateMonthPlanComments(TheEndOfThePlanned endOfThePlanned);

	public int doSaveGczlInfo(GczlInfo info);

	public void doDeleteGczlInfoById(int tab_seed);

	public String getGczlInfoListWithQuery( String b_date,
			String e_date);

	public String getCurrentAndNextAbsentInfo();

	public String getMonthPlanList(PageInfo pageInfo, String deptId,
			String appDate);

	public String getAbsentList();

	public String getAskForLeaveList(PageInfo pageInfo, String queryStr, String date);

	public String getColumnNameCombo();

	public String getGczlInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query);

	public String getGczlInfoListComparison(String b_year, String e_year,
			String month);

	public String getGczlInfoExcretion(String b_date, String e_date, PageInfo pageInfo);

	public String getExcretionSum(String b_year, String e_year);

	public String getGczlInfoListForFlood(String year, String flood);

	public String getGczlInfoListForFloodGates(String b_date, String e_date,
			String floodGates);

	public String getGczlInfoEveryMonthLevel(String b_date, String e_date,
			String type);

	public String getMptpCodeList();

	public String getMptCodeList(String mptp_code);

	public int doSavecyInfo(CeyaInfo info);

	public void doDeletecyInfoById(int tab_seed);

	public String getCyInfoListWithGroupQuery(PageInfo pageInfo,
			String queryStr, StandardQuery query);

	public BigDecimal getTubo_high_degree(String mptp_code, String mpt_name);

	public String getCeYaColumnNameCombo();

	public String getFileFlagList();

	public int getSign_No(String year);

	public void dosendHuiQianMessage(String indexStr, String formId, String type);

	public void doupdateHuiQianMessage(String isAgree, String formId, String type);

	public String getHQ_nameByFormId(String formId, String type);


	String getDoDayReportData(String datetime) throws IOException;

	void saveDoDayReportData(List<Map<String, String>> list) throws Exception;

	String queryDoDayReportData(String startDate, String endDate) throws IOException;

	String queryzmDayReportData(String currentDate) throws IOException, ParseException;

	void exportzmDayReportData(HttpServletResponse response,String currentDate) throws Exception;

	void exportdoDayReportData(HttpServletResponse response, String startDate, String endDate) throws Exception;

	String getTopReportData(String startDate,String endDate) throws Exception;

	String doCreateDdWord(String queryDate) throws Exception;

	void deleteDoDayReportData(String queryDate, String queryPerson);
}
