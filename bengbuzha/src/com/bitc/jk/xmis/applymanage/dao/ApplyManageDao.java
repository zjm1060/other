package com.bitc.jk.xmis.applymanage.dao;

import java.math.BigDecimal;
import java.text.ParseException;
import java.util.List;
import java.util.Map;

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
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.JsonResponse;

public interface ApplyManageDao {

	public String doSaveAppMeetingRoom(ApplyMeetingRoomForm applyMeetingRoom);

	@SuppressWarnings("rawtypes")
	public List getApplyMeetingRoomFormById(String id);

	@SuppressWarnings("rawtypes")
	public List getAppMeetingRoomFormList(String queryStr, String ordersql);

	@SuppressWarnings("rawtypes")
	public List getMeetingRoomCombo();

	public int doHandleAppMeetingRoomStatus(String id,int status_id);

	@SuppressWarnings("rawtypes")
	public List getMeetingRoomUsingTimeList(String room_id,String form_id);

	public List getApplyMeetingFormChangeCount();

	
	@SuppressWarnings("rawtypes")
	public List getAppCarList(String queryStr, String ordersql);
	
	@SuppressWarnings("rawtypes")
	public List getVehicleManagementListToSelectGrid();

	public String doSaveAppCars(ApplyCarForm applyCarForm);

	public void doSaveAppCarDetals(String return_id, String string);

	@SuppressWarnings("rawtypes")
	public List getApplyCarByFormId(String form_id);

	@SuppressWarnings("rawtypes")
	public List getApplyCarIdsDetalsByFormId(String form_id);

	public void doDelCarAppDetails(String form_id);

	@SuppressWarnings("rawtypes")
	public List getCarUsingTimeList(String car_id,String form_id);

	@SuppressWarnings("rawtypes")
	public List getApplyOfficialSuppliesListByFormId(String form_id);

	@SuppressWarnings("rawtypes")
	public List getOfficialSuppliesTypeCombo();

	@SuppressWarnings("rawtypes")
	public List getOfficialSuppliesCombo(String type);

	public String doSaveApplyOfficialSuppliesForm(
			ApplyOfficialSuppliesForm applyForm);

	public void doUpdateApplyOfficialSuppliesForm(
			ApplyOfficialSuppliesForm applyForm, String old_form_id, String old_supplies_id);

	public void doDeleteApplyOfficialSuppliesForm(String form_id,
			String supplies_id);

	@SuppressWarnings("rawtypes")
	public List getOfficalSuppliesByCompositeIds(String form_id,
			String supplies_id);

	public void doSaveAppFormProcessinstanceIDInfo(String form_id, String id,
			int i);

	public void doUpdateAppDateTimeByFormId(String form_id, String applicant_time);

	public void doUpdateAppappFormStatus(String processInstanceId, int i);

	@SuppressWarnings("rawtypes")
	public List getApplyOfficialSuppliesListQuery(String queryStr,
			String ordersql);

	@SuppressWarnings("rawtypes")
	public List getApplyOfficialSuppliesListSum(String queryStr, String ordersql);

	public String doSaveApplyOfficialSuppliesRepairForm(
			ApplyOfficialSuppliesRepairForm applyForm);

	public void doUpdateApplyOfficialSuppliesRepairForm(
			ApplyOfficialSuppliesForm applyForm, String old_form_id,
			String old_supplies_id);

	public void doDeleteApplyOfficialSuppliesRepairForm(String form_id,
			String supplies_id);

	public void doUpdateAppDateTimeByRepairFormId(String form_id,
			String applicant_time);

	@SuppressWarnings("rawtypes")
	public List getApplyOfficialSuppliesRepairList(String form_id);

	@SuppressWarnings("rawtypes")
	public List getApplyOfficialSuppliesRepairListSum(String queryStr,
			String ordersql);
	@SuppressWarnings("rawtypes")
	public List getGczlInfoList(String queryStr,
			String ordersql);

	public void doUpdateOfficeSuppliesDetailsNum(String supplies_id, int qty);

	public void doSaveOrUpdateBusinessEntertainment(
			BusinessEntertainment businessEntertainment);

	@SuppressWarnings("rawtypes")
	public List getBusinessEntertainmentFormByFormId(String form_id);

	public void doUpdateBusinessEntertainmentWorkedId(int getInt, String form_id);

	public void doUpdateSignNoByFormId(String form_id, String sign_No);

	@SuppressWarnings("rawtypes")
	public List getBusinessEntertainmentFormList(String queryStr,
			String ordersql);

	public void doSaveOrUpdateAbsentForm(AbsentForm absentForm);

	public int isInRoles(String empId, int roleId);

	@SuppressWarnings("rawtypes")
	public List getAbsentFormByFormId(String form_id);

	public AbsentForm  getAbsentFormById(String form_id);
	@SuppressWarnings("rawtypes")
	public List getAttachmentListByAbsentFormId(String form_id);
	@SuppressWarnings("rawtypes")
	public List getAbsentFormList(String queryStr, String ordersql);

	@SuppressWarnings("rawtypes")
	public List getDocumentTypeInfoCombo();

	public String doSaveDocumentAppFormDetail(DocumentAppFormDetail applyForm);

	@SuppressWarnings("rawtypes")
	public List getDocumentAppFormDetailByFormId(String form_id);

	public void doUpdateDeptArroval(int userID, String comment,String approvalDate,
			String form_id);

	public void doUpdateSpecArroval(int userID, String comment,String approvalDate,
			String form_id);

	public void doUpdatedirecArroval(int userID, String comment,String approvalDate,
			String form_id);

	public void doInsertDocumentCirculatedInstances(
			DocumentCiculatedInstances instancesObj);

	public List getDocumentCirculatedInstancesList(String form_id);

	public String getTitleByFormId(String form_id);

	public List getDocumentAppFormDetailList(String queryStr, String ordersql);

	public void doSaveOrUpdateMonthPlan(EarlyPlans earlyPlans);

	public List getMonthPlanByFormId(String form_id);

	public void doSaveOrUpdateEndOfMonthPlan(TheEndOfThePlanned endOfThePlanned);

	public void doUpdateMonthPlanComments(EarlyPlans earlyPlans);

	public void doUpdateMonthPlanComments(TheEndOfThePlanned endOfThePlanned);

	public int doSaveGczlInfo(GczlInfo info);

	public void doDeleteGczlInfoById(int tab_seed);

	public List getGczlInfoListWithQuery(String b_date, String e_date);

	public List getCurrentAndNextAbsentInfo();

	public List getMonthPlanList(PageInfo pageInfo);

	public List getMonthPlanListByDate(PageInfo pageInfo, String appDate);

	public List getMonthPlanListByPart(PageInfo pageInfo, String department);

	public List getMonthPlanListByPartAndDate(PageInfo pageInfo,
			String department, String appDate);

	public void doSaveFormIdAndProId(String form_id, String id);

	public List getAbsentList();

	public List getAskForLeaveList(String queryStr, String ordersql, String date);

	public List getColumnNameCombo();

	public List getGczlInfoListWithGroupQuery(String queryStr, PageInfo pageInfo, StandardQuery query);

	public List getGczlInfoListComparison(String b_year, String e_year,
			String month);

	public List getGczlInfoExcretion(String b_date, String e_date);

	public List getExcretionSum(String b_year, String e_year);

	public List getGczlInfoListForFlood(String year, String flood);

	public List getGczlInfoListForFloodGates(String b_date, String e_date,
			String floodGates);

	public List getGczlInfoEveryMonthLevel(String b_date, String e_date,
			String type);

	public List getMptpCodeList();

	public List getMptCodeList(String mptp_code);

	public int doSavecyInfo(CeyaInfo info);

	public void doDeletecyInfoById(int tab_seed);

	public List getCyInfoListWithGroupQuery(String queryStr, PageInfo pageInfo,
			StandardQuery query);

	public BigDecimal getTubo_high_degree(String mptp_code, String mpt_name);

	public List getCeYaColumnNameCombo();

	public JsonResponse getFileFlagList();

	public List getDutyLevel(String empId);

	public int getSign_No(String year);

	public void dosendHuiQianMessage(String indexStr, String formId, String type);

	public void doupdateHuiQianMessage(String isAgree, String formId, String type);

	public String getHQ_nameByFormId(String formId, String type);


	Map<String,String> getDoDayReportData(String datetime);

	void saveDoDayReportData(Map<String, String> map) throws Exception;

	List<Map<String,String>> queryDoDayReportData(String startDate, String endDate);

	Map queryzmDayReportData(String currentDate) throws ParseException;

	Map<String,String> getTopReportData(String startDate,String endDate) throws ParseException;

	String doCreateDdWord(String queryDate);

	void deleteDoDayReportData(String queryDate, String queryPerson);
}
