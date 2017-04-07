package com.bitc.jk.xmis.applymanage.dao.impl;

import com.bitc.jk.xmis.applymanage.dao.ApplyManageDao;
import com.bitc.jk.xmis.applymanage.model.*;
import com.bitc.jk.xmis.dao.impl.BaseDAOImpl;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.MessageInfo;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.*;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.response.JsonResponse;
import com.bitc.jk.xmis.web.response.StoreResponse;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementSetter;

import java.math.BigDecimal;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Timestamp;
import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;

public class ApplyManageDaoImpl extends BaseDAOImpl implements ApplyManageDao {

    private JdbcTemplate jt;
    private String sign_no;

    public JdbcTemplate getJt() {
        return jt;
    }

    public void setJt(JdbcTemplate jt) {
        this.jt = jt;
    }

    public String doSaveAppMeetingRoom(ApplyMeetingRoomForm applyMeetingRoom) {
        if (applyMeetingRoom.getId().equals("")) {
            ht.save(applyMeetingRoom);
        } else {
            ht.update(applyMeetingRoom);
        }
        return applyMeetingRoom.getId();
    }

    @SuppressWarnings("rawtypes")
    public List getApplyMeetingRoomFormById(String id) {
        String sql = SQLScript.getApplyMeetingRoomFormById;
        return jt.queryForList(sql, new Object[]{new String(id)});
    }

    @SuppressWarnings("rawtypes")
    public List getAppMeetingRoomFormList(String queryStr, String ordersql) {
        String sql = SQLScript.getAppMeetingRoomFormList;
        if (queryStr != null && !"".equals(queryStr)) {
            queryStr = "'%" + queryStr + "%'";
            String sqlend = "\n where room_no like  " + queryStr + "  \n"
                    + "or  convert(varchar(50), begin_datetime,120) like   "
                    + queryStr + " \n"
                    + "or (RTRIM(h.姓氏)+RTRIM(h.名字)) like    " + queryStr
                    + " \n";
            String sqlresult = sql + sqlend + ordersql;
            return jt.queryForList(sqlresult);
        } else {
            return jt.queryForList(sql + ordersql);
        }

    }

    @SuppressWarnings("rawtypes")
    public List getMeetingRoomCombo() {
        String sql = SQLScript.getMeetingRoomCombo;
        return jt.queryForList(sql);
    }

    public int doHandleAppMeetingRoomStatus(String id, int status_id) {
        String sql = SQLScript.doHandleAppMeetingRoomStatus;
        return jt.update(sql, new Object[]{new Integer(status_id),
                new String(id)});
    }

    @SuppressWarnings("rawtypes")
    public List getMeetingRoomUsingTimeList(String room_id, String form_id) {
        String sql = SQLScript.getMeetingRoomUsingTimeList;
        return jt.queryForList(sql, new Object[]{new String(room_id),
                new String(form_id)});
    }

    public List getApplyMeetingFormChangeCount() {
        String sql = SQLScript.getApplyMeetingFormChangeCount;
        return jt.queryForList(sql);
    }

    @SuppressWarnings("rawtypes")
    public List getAppCarList(String queryStr, String ordersql) {
        String sql = SQLScript.getAppCarList;
        if (queryStr != null && !"".equals(queryStr)) {
            queryStr = "'%" + queryStr + "%'";
            String sqlend = "\n where car_No LIKE " + queryStr
                    + " or convert(varchar(50), begin_datetime,120)  like  "
                    + queryStr + " \n";
            String sqlresult = sql + sqlend + ordersql;
            return jt.queryForList(sqlresult);
        } else {
            return jt.queryForList(sql + ordersql);
        }

    }

    @SuppressWarnings("rawtypes")
    public List getVehicleManagementListToSelectGrid() {
        String sql = SQLScript.getVehicleManagementListToSelectGrid;
        return jt.queryForList(sql);
    }

    public String doSaveAppCars(ApplyCarForm applyCarForm) {
        if (applyCarForm.getForm_id() != null
                && !"".equals(applyCarForm.getForm_id())) {
            ht.update(applyCarForm);
        } else {
            ht.save(applyCarForm);
        }
        return applyCarForm.getForm_id();
    }

    public void doSaveAppCarDetals(String form_id, String car_id) {
        String sql = SQLScript.doSaveAppCarDetals;
        jt.update(sql, new Object[]{new String(form_id), new String(car_id)});

    }

    @SuppressWarnings("rawtypes")
    public List getApplyCarByFormId(String form_id) {
        String sql = SQLScript.getApplyCarByFormId;
        return jt.queryForList(sql, new Object[]{new String(form_id)});
    }

    @SuppressWarnings("rawtypes")
    public List getApplyCarIdsDetalsByFormId(String form_id) {
        String sql = SQLScript.getApplyCarIdsDetalsByFormId;
        return jt.queryForList(sql, new Object[]{new String(form_id)});
    }

    public void doDelCarAppDetails(String form_id) {
        String sql = SQLScript.delCarAppDetails;
        jt.update(sql, new Object[]{new String(form_id)});
    }

    @SuppressWarnings("rawtypes")
    public List getCarUsingTimeList(String car_id, String form_id) {
        String sql = SQLScript.getCarUsingTimeList;
        return jt.queryForList(sql, new Object[]{new String(car_id),
                new String(form_id)});
    }

    @SuppressWarnings("rawtypes")
    public List getApplyOfficialSuppliesListByFormId(String form_id) {
        String sql = SQLScript.getApplyOfficialSuppliesListFormId;
        return jt.queryForList(sql, new Object[]{new String(form_id)});
    }

    @SuppressWarnings("rawtypes")
    public List getOfficialSuppliesTypeCombo() {
        String sql = SQLScript.getOfficialSuppliesTypeCombo;
        return jt.queryForList(sql);
    }

    @SuppressWarnings("rawtypes")
    public List getOfficialSuppliesCombo(String type) {
        String sql = SQLScript.getOfficialSuppliesCombo;
        return jt.queryForList(sql, new Object[]{new String(type)});
    }

    public String doSaveApplyOfficialSuppliesForm(
            ApplyOfficialSuppliesForm applyForm) {
        if (applyForm.getForm_id().equals("")) {
            applyForm.setForm_id(UUIDUtils.nextCode());
            ht.save(applyForm);
        } else {
            ht.save(applyForm);
        }
        return applyForm.getForm_id();
    }

    public void doUpdateApplyOfficialSuppliesForm(
            final ApplyOfficialSuppliesForm applyFormParams,
            final String old_form_id, final String old_supplies_id) {
        jt.update(SQLScript.doUpdateApplyOfficialSuppliesForm,
                new PreparedStatementSetter() {

                    public void setValues(PreparedStatement ps)
                            throws SQLException {
                        String form_id = applyFormParams.getForm_id();
                        int priority = applyFormParams.getPriority();
                        String supplies_id = applyFormParams.getSupplies_id();
                        String reason = applyFormParams.getReason();
                        int qty = applyFormParams.getQty();
                        String app_datetime = DateUtil
                                .formatDateTime(applyFormParams
                                        .getApp_datetime());
                        int app_user_id = applyFormParams.getApp_user_id();
                        String remark = applyFormParams.getRemark();
                        ps.setString(1, form_id);
                        ps.setInt(2, priority);
                        ps.setString(3, supplies_id);
                        ps.setString(4, reason);
                        ps.setInt(5, qty);
                        ps.setString(6, app_datetime);
                        ps.setInt(7, app_user_id);
                        ps.setString(8, remark);
                        ps.setString(9, old_form_id);
                        ps.setString(10, old_supplies_id);
                    }
                });

    }

    public void doDeleteApplyOfficialSuppliesForm(String form_id,
                                                  String supplies_id) {
        String sql = SQLScript.doDeleteApplyOfficialSuppliesForm;
        jt.update(sql, new Object[]{new String(form_id),
                new String(supplies_id)});

    }

    @SuppressWarnings("rawtypes")
    public List getOfficalSuppliesByCompositeIds(String form_id,
                                                 String supplies_id) {
        String sql = SQLScript.getOfficalSuppliesByCompositeIds;
        return jt.queryForList(sql, new Object[]{new String(form_id),
                new String(supplies_id)});
    }

    public void doSaveAppFormProcessinstanceIDInfo(String form_id,
                                                   String processInstanceId, int status_id) {
        jt.update(SQLScript.doSaveAppFormProcessinstanceIDInfo, new Object[]{
                new String(form_id), new String(processInstanceId),
                new Integer(status_id)});
    }

    public void doUpdateAppDateTimeByFormId(String form_id,
                                            String applicant_time) {
        jt.update(SQLScript.doUpdateAppDateTimeByFormId, new Object[]{
                new String(applicant_time), new String(form_id)});
    }

    public void doUpdateAppappFormStatus(String processInstanceId, int i) {
        jt.update(SQLScript.doUpdateAppappFormStatus, new Object[]{
                new Integer(i), new String(processInstanceId)});
    }

    @SuppressWarnings("rawtypes")
    public List getApplyOfficialSuppliesListQuery(String queryStr,
                                                  String ordersql) {
        String sql = SQLScript.getApplyOfficialSuppliesListQuery;
        if (queryStr != null && !"".equals(queryStr)) {
            queryStr = "'%" + queryStr + "%'";
            String sqlend = "\n where supplies_name like  " + queryStr + "  \n"
                    + "or convert(varchar(50), app_datetime,120) like   "
                    + queryStr + " \n";
            String sqlresult = sql + sqlend + ordersql;
            return jt.queryForList(sqlresult);
        } else {
            return jt.queryForList(sql + ordersql);
        }

    }

    @SuppressWarnings("rawtypes")
    public List getApplyOfficialSuppliesListSum(String queryStr, String ordersql) {

        String sql = SQLScript.getApplyOfficialSuppliesListSum;
        if (queryStr != null && !"".equals(queryStr)) {
            queryStr = "'%" + queryStr + "%'";
            String sqlend = "\n having RTRIM(h.姓氏) + RTRIM(h.名字) like  "
                    + queryStr + "  \n"
                    + "or convert(varchar(50), app_datetime,120) like   "
                    + queryStr + " \n";
            String sqlresult = sql + sqlend + ordersql;
            return jt.queryForList(sqlresult);
        } else {
            return jt.queryForList(sql + ordersql);
        }

    }

    public String doSaveApplyOfficialSuppliesRepairForm(
            ApplyOfficialSuppliesRepairForm applyForm) {
        if (applyForm.getForm_id().equals("")) {
            applyForm.setForm_id(UUIDUtils.nextCode());
            ht.save(applyForm);
        } else {
            ht.save(applyForm);
        }
        return applyForm.getForm_id();
    }

    public void doUpdateApplyOfficialSuppliesRepairForm(
            final ApplyOfficialSuppliesForm applyFormParams,
            final String old_form_id, final String old_supplies_id) {
        jt.update(SQLScript.doUpdateApplyOfficialSuppliesRepairForm,
                new PreparedStatementSetter() {

                    public void setValues(PreparedStatement ps)
                            throws SQLException {
                        String form_id = applyFormParams.getForm_id();
                        int priority = applyFormParams.getPriority();
                        String supplies_id = applyFormParams.getSupplies_id();
                        String reason = applyFormParams.getReason();
                        int qty = applyFormParams.getQty();
                        String app_datetime = DateUtil
                                .formatDateTime(applyFormParams
                                        .getApp_datetime());
                        int app_user_id = applyFormParams.getApp_user_id();
                        String remark = applyFormParams.getRemark();
                        ps.setString(1, form_id);
                        ps.setInt(2, priority);
                        ps.setString(3, supplies_id);
                        ps.setString(4, reason);
                        ps.setInt(5, qty);
                        ps.setString(6, app_datetime);
                        ps.setInt(7, app_user_id);
                        ps.setString(8, remark);
                        ps.setString(9, old_form_id);
                        ps.setString(10, old_supplies_id);
                    }
                });

    }

    public void doDeleteApplyOfficialSuppliesRepairForm(String form_id,
                                                        String supplies_id) {
        String sql = SQLScript.doDeleteApplyOfficialSuppliesRepairForm;
        jt.update(sql, new Object[]{new String(form_id),
                new String(supplies_id)});

    }

    public void doUpdateAppDateTimeByRepairFormId(String form_id,
                                                  String applicant_time) {
        jt.update(SQLScript.doUpdateAppDateTimeByRepairFormId, new Object[]{
                new String(applicant_time), new String(form_id)});
    }

    @SuppressWarnings("rawtypes")
    public List getApplyOfficialSuppliesRepairList(String form_id) {
        String sql = SQLScript.getApplyOfficialSuppliesRepairListFormId;
        return jt.queryForList(sql, new Object[]{new String(form_id)});
    }

    @SuppressWarnings("rawtypes")
    public List getApplyOfficialSuppliesRepairListSum(String queryStr,
                                                      String ordersql) {

        String sql = SQLScript.getApplyOfficialSuppliesRepairListSum;
        if (queryStr != null && !"".equals(queryStr)) {
            queryStr = "'%" + queryStr + "%'";
            String sqlend = "\n having RTRIM(h.姓氏) + RTRIM(h.名字) like  "
                    + queryStr + "  \n"
                    + "or convert(varchar(50), app_datetime,120) like   "
                    + queryStr + " \n";
            String sqlresult = sql + sqlend + ordersql;
            return jt.queryForList(sqlresult);
        } else {
            return jt.queryForList(sql + ordersql);
        }

    }

    @SuppressWarnings("rawtypes")
    public List getGczlInfoList(String queryStr, String ordersql) {
        String sql = SQLScript.getGczlInfoList;
        return jt.queryForList(sql);

    }

    public void doUpdateOfficeSuppliesDetailsNum(String supplies_id, int qty) {
        jt.update(SQLScript.doUpdateOfficeSuppliesDetailsNum, new Object[]{
                new Integer(qty), new String(supplies_id)});

    }

    public void doSaveOrUpdateBusinessEntertainment(
            BusinessEntertainment businessEntertainment) {
        if (businessEntertainment.getForm_id() != null
                && !"".equals(businessEntertainment.getForm_id())) {
            ht.update(businessEntertainment);
        } else {
            ht.save(businessEntertainment);
        }
    }

    @SuppressWarnings("rawtypes")
    public List getBusinessEntertainmentFormByFormId(String form_id) {
        String sql = SQLScript.getBusinessEntertainmentFormByFormId;
        return jt.queryForList(sql, new Object[]{new String(form_id)});
    }

    public void doUpdateBusinessEntertainmentWorkedId(int workedId,
                                                      String form_id) {
        jt.update(SQLScript.doUpdateBusinessEntertainmentWorkedId,
                new Object[]{new Integer(workedId), new String(form_id)});

    }

    public void doUpdateSignNoByFormId(String form_id, String sign_No) {
        jt.update(SQLScript.doUpdateSignNoByFormId, new Object[]{
                new String(sign_No), new String(form_id)});
    }

    @SuppressWarnings("rawtypes")
    public List getBusinessEntertainmentFormList(String queryStr,
                                                 String ordersql) {
        String sql = SQLScript.getBusinessEntertainmentFormList;
        if (!"".equals(queryStr)) {
            return jt.queryForList(sql + queryStr + ordersql);
        } else {
            return jt.queryForList(sql + ordersql);
        }

    }

    public void doSaveOrUpdateAbsentForm(AbsentForm absentForm) {
        if (absentForm.getForm_id() != null
                && !"".equals(absentForm.getForm_id())) {
            ht.update(absentForm);
        } else {
            ht.save(absentForm);
        }
    }

    public int isInRoles(String empId, int role_flag) {
        String sql = "SELECT COUNT(1) FROM  xmis.T_Dept_Leader_Mapping  WHERE role_flag="
                + role_flag + " and user_id=" + empId;
        return jt.queryForInt(sql);
    }

    @SuppressWarnings("rawtypes")
    public List getAbsentFormByFormId(String form_id) {
        String sql = SQLScript.getAbsentFormByFormId;
        return jt.queryForList(sql, new Object[]{new String(form_id)});
    }

    public AbsentForm getAbsentFormById(String form_id) {
        return (AbsentForm) ht.get(AbsentForm.class, form_id);
    }

    @SuppressWarnings("rawtypes")
    public List getAttachmentListByAbsentFormId(String form_id) {
        String sql = SQLScript.getAttachmentListByAbsentFormId;
        return jt.queryForList(sql, new Object[]{new String(form_id)});
    }

    @SuppressWarnings("rawtypes")
    public List getAbsentFormList(String queryStr, String ordersql) {
        String sql = SQLScript.getAbsentFormList;

        if (queryStr != null && !"".equals(queryStr)) {
            queryStr = "'%" + queryStr + "%'";
            String sqlend = "\n where (RTRIM(p.姓氏)+RTRIM(p.名字)) like "
                    + queryStr + " \n";
            String sqlresult = sql + sqlend + ordersql;
            return jt.queryForList(sqlresult);
        } else {
            return jt.queryForList(sql + ordersql);
        }

    }

    @SuppressWarnings("rawtypes")
    public List getDocumentTypeInfoCombo() {
        String sql = SQLScript.getDocumentTypeInfoCombo;
        return jt.queryForList(sql);
    }

    public String doSaveDocumentAppFormDetail(DocumentAppFormDetail applyForm) {
        if ("".equals(applyForm.getForm_id())) {
            ht.save(applyForm);
        } else {
            ht.update(applyForm);
        }
        return applyForm.getForm_id();
    }

    @SuppressWarnings("rawtypes")
    public List getDocumentAppFormDetailByFormId(String form_id) {
        String sql = SQLScript.getDocumentAppFormDetailByFormId;
        return jt.queryForList(sql, new Object[]{new String(form_id)});
    }

    public void doUpdateDeptArroval(int userID, String comment,
                                    String approval_datetime, String form_id) {
        String sql = SQLScript.doUpdateDeptArroval;
        jt.update(sql, new Object[]{new Integer(userID), new String(comment),
                new String(approval_datetime), new String(form_id)});
    }

    public void doUpdateSpecArroval(int userID, String comment,
                                    String approval_datetime, String form_id) {
        String sql = SQLScript.doUpdateSpecArroval;
        jt.update(sql, new Object[]{new Integer(userID), new String(comment),
                new String(approval_datetime), new String(form_id)});

    }

    public void doUpdatedirecArroval(int userID, String comment,
                                     String approval_datetime, String form_id) {

        String appYear = approval_datetime.substring(0, 4);
        String sqlStr = "SELECT CASE WHEN  Max(file_no) IS NULL THEN 1 ELSE MAX(file_no)+1 END AS FILE_no from T_Document_App_Form_Detail where file_flag= (select file_flag from xmis.T_Document_App_Form_Detail where form_id= '"
                + form_id
                + "')  and CONVERT(VARCHAR(4), app_direc_approved_datetime, 120) = '"
                + appYear + "'";
        int file_no = jt.queryForInt(sqlStr);

        String sql = SQLScript.doUpdatedirecArroval;
        jt.update(sql, new Object[]{new Integer(userID), new String(comment),
                new String(approval_datetime), new String(form_id)});

    }

    public void doInsertDocumentCirculatedInstances(
            final DocumentCiculatedInstances instancesObj) {
        String sql = "INSERT INTO T_Document_Circulated_Instances VALUES (?,?,?,?,?,?)";
        jt.update(sql, new PreparedStatementSetter() {

            @Override
            public void setValues(PreparedStatement p) throws SQLException {
                p.setString(1, instancesObj.getInstanc_id());
                p.setString(2, instancesObj.getInstanc_id());
                p.setInt(3, instancesObj.getFrom_id());
                p.setTimestamp(4,
                        Timestamp.valueOf(DateUtil.formatDate(
                                instancesObj.getFrom_datetime(),
                                "yyyy-MM-dd MM:mm:ss")));
                p.setInt(5, instancesObj.getTo_id());
                p.setString(6, instancesObj.getComments());

            }
        });
    }

    @SuppressWarnings("rawtypes")
    public List getDocumentCirculatedInstancesList(String form_id) {
        String sql = SQLScript.getDocumentCirculatedInstancesList;
        return jt.queryForList(sql, new Object[]{new String(form_id)});
    }

    @SuppressWarnings("rawtypes")
    public String getTitleByFormId(String form_id) {
        String sql = SQLScript.getTitleByFormId;
        List list = jt.queryForList(sql, new Object[]{new String(form_id)});
        if (list != null && list.size() > 0) {
            Map map = (Map) list.get(0);
            return Type.getString(map.get("title"));
        } else {
            return "";
        }
    }

    @SuppressWarnings("rawtypes")
    public List getDocumentAppFormDetailList(String queryStr, String ordersql) {
        String sql = SQLScript.getDocumentAppFormDetailList;
        if (queryStr != null && !"".equals(queryStr)) {
            queryStr = "'%" + queryStr + "%'";
            String sqlend = "\n where title like  " + queryStr + "  \n"
                    + "or  convert(varchar(50), app_datetime,120) like   "
                    + queryStr + " \n"
                    + "or (RTRIM(app_user.姓氏)+RTRIM(app_user.名字)) like    "
                    + queryStr + " \n" + " or summary like  " + queryStr
                    + " \n" + " or document_type_name like  " + queryStr
                    + " \n";
            String sqlresult = sql + sqlend + ordersql;
            return jt.queryForList(sqlresult);
        } else {
            return jt.queryForList(sql + ordersql);
        }
    }

    @Override
    public void doSaveOrUpdateMonthPlan(EarlyPlans earlyPlans) {
        if (earlyPlans.getFormId() != null
                && !"".equals(earlyPlans.getFormId())) {
            ht.update(earlyPlans);
        } else {
            ht.save(earlyPlans);
            TheEndOfThePlanned endOfThePlanned = new TheEndOfThePlanned();
            endOfThePlanned.setFormId(earlyPlans.getFormId());
            endOfThePlanned.setExcutedComments("");
            ht.save(endOfThePlanned);
        }
    }

    @Override
    public List getMonthPlanByFormId(String form_id) {
        String sql = SQLScript.getMonthPlanByFormId;
        return jt.queryForList(sql, new Object[]{new String(form_id)});
    }

    @Override
    public void doSaveOrUpdateEndOfMonthPlan(TheEndOfThePlanned endOfThePlanned) {
        if (endOfThePlanned.getFormId() != null
                && !"".equals(endOfThePlanned.getFormId())) {
            ht.update(endOfThePlanned);
        } else {
            ht.save(endOfThePlanned);
        }

    }

    @Override
    public void doUpdateMonthPlanComments(final EarlyPlans earlyPlans) {
        String sql = "update T_Workplan_Form_Detail_EarlyPlans set plan_assign_comments = ? ,"
                + " plan_assigned_id = ? ,"
                + "plan_assigned_date = ? "
                + " where form_id =?";
        jt.update(sql, new PreparedStatementSetter() {

            @Override
            public void setValues(PreparedStatement p) throws SQLException {
                p.setString(1, earlyPlans.getPlanAssignComments());
                p.setInt(2, earlyPlans.getPlanAssignedId());
                p.setDate(3, Type.getDate(earlyPlans.getPlanAssignedDate()));
                p.setString(4, earlyPlans.getFormId());
            }
        });

    }

    @Override
    public void doUpdateMonthPlanComments(
            final TheEndOfThePlanned endOfThePlanned) {
        String sql1 = "update T_Workplan_Form_Detail_EndOfPlanned set excuted_assigned_comments = ? ,"
                + " excuted_assigned_id = ? ,"
                + "excute_assigned_date = ? "
                + " where form_id =?";
        jt.update(sql1, new PreparedStatementSetter() {

            @Override
            public void setValues(PreparedStatement p) throws SQLException {
                p.setString(1, endOfThePlanned.getExcutedAssignedComments());
                p.setInt(2, endOfThePlanned.getExcutedAssignedId());
                p.setDate(3,
                        Type.getDate(endOfThePlanned.getExcuteAssignedDate()));
                p.setString(4, endOfThePlanned.getFormId());
            }
        });

    }

    public int doSaveGczlInfo(GczlInfo info) {
        if (info.getTab_seed() != 0) {
            ht.update(info);
        } else {
            int maxID = jt.queryForInt(SQLScript.getMaxIDFromGczlInfo) + 1;
            String unit_code = (String) jt.queryForMap(
                    SQLScript.getUnitCodeFormUnitInfo).get("unit_code");
            info.setUnit_code(unit_code);
            info.setTab_seed(maxID);
            ht.save(info);
        }
        return info.getTab_seed();
    }

    public void doDeleteGczlInfoById(int tab_seed) {
        String sql = SQLScript.doDeleteGczlInfoById;
        jt.update(sql, new Object[]{tab_seed});

    }

    @SuppressWarnings("rawtypes")
    public List getGczlInfoListWithQuery(String b_date, String e_date) {
        String sql = SQLScript.getGczlInfoListWithQuery;
        return jt.queryForList(sql, new Object[]{new String(b_date),
                new String(e_date)});
    }

    @SuppressWarnings("rawtypes")
    public List getCurrentAndNextAbsentInfo() {
        String sql = SQLScript.getCurrentAndNextAbsentInfo;
        return jt.queryForList(sql);
    }

    @Override
    public List getMonthPlanList(PageInfo pageInfo) {
        String sql = SQLScript.getMonthPlanList;
        return jt.queryForList(sql);
    }

    @Override
    public List getMonthPlanListByDate(PageInfo pageInfo, String workMonth) {
        String sql = SQLScript.getMonthPlanListByDate;
        return jt.queryForList(sql, new Object[]{workMonth.substring(0, 7)});
    }

    @Override
    public List getMonthPlanListByPart(PageInfo pageInfo, String department) {
        String sql = SQLScript.getMonthPlanListByPart;
        return jt.queryForList(sql, new Object[]{department});
    }

    @Override
    public List getMonthPlanListByPartAndDate(PageInfo pageInfo,
                                              String department, String workMonth) {
        String sql = SQLScript.getMonthPlanListByPartAndDate;
        return jt.queryForList(sql,
                new Object[]{department, workMonth.substring(0, 7)});
    }

    @Override
    public void doSaveFormIdAndProId(final String form_id, final String id) {
        String sql = SQLScript.doSaveFormIdAndProId;
        jt.update(sql, new PreparedStatementSetter() {

            @Override
            public void setValues(PreparedStatement p) throws SQLException {
                p.setString(1, form_id);
                p.setString(2, id);
            }
        });

    }

    public List getAbsentList() {
        String sql = SQLScript.getAbsentList;
        return jt.queryForList(sql);
    }

    public List getColumnNameCombo() {
        String sql = SQLScript.getColumnNameCombo;
        return jt.queryForList(sql);
    }

    @Override
    public List getAskForLeaveList(String hrName, String ordersql, String date) {
        String sql = SQLScript.getAskForLeaveList;
        String sql1 = SQLScript.getAskForLeaveListByFullYear;
        if (date.indexOf("-") < 0) {
            if (hrName != null && !"".equals(hrName)) {
                String sqlresult = sql1
                        + "   WHERE RTRIM(hr.姓氏) + RTRIM(hr.名字) LIKE '%"
                        + hrName + "%' " + ordersql;
                return jt.queryForList(sqlresult, new Object[]{date, date,
                        date, date, date, date, date, date, date, date, date,
                        date, date, date, date, date});
            } else {
                return jt.queryForList(sql1 + ordersql, new Object[]{date,
                        date, date, date, date, date, date, date, date, date,
                        date, date, date, date, date, date});
            }
        } else {
            if (hrName != null && !"".equals(hrName)) {
                String sqlresult = sql
                        + "   WHERE RTRIM(hr.姓氏) + RTRIM(hr.名字) LIKE '%"
                        + hrName + "%' " + ordersql;
                return jt.queryForList(sqlresult, new Object[]{date, date,
                        date, date, date, date, date, date, date, date, date,
                        date, date, date, date, date});
            } else {
                return jt.queryForList(sql + ordersql, new Object[]{date,
                        date, date, date, date, date, date, date, date, date,
                        date, date, date, date, date, date});
            }
        }
    }

    @SuppressWarnings("rawtypes")
    public List getGczlInfoListWithGroupQuery(String queryStr,
                                              PageInfo pageInfo, StandardQuery query) {
        String sql = SQLScript.getGczlInfoListWithGroupQuery;
        SQLGenerator generator = null;
        if (queryStr != null && !("").equals(queryStr)) {
            generator = new SQLGenerator(sql
                    + " select * into #result  from #temp where " + queryStr);
        } else {
            generator = new SQLGenerator(sql
                    + " select * into #result  from #temp    ");

        }
        if (query != null) {
            query.addToSQLGenerator(generator);
        }

        String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
        Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
                .getLimit() == 0)) ? new Object[]{} : new Object[]{
                new Integer(pageInfo.getStart() + 1),
                new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit())};
        return jt.queryForList(sqlStr, args);
    }

    @Override
    public List getGczlInfoListComparison(String b_year, String e_year,
                                          String month) {
        List list = new ArrayList();
        for (int i = Type.GetInt(e_year); i >= Type.GetInt(b_year); i--) {
            String date = i + "-" + month;
            String sql = SQLScript.getGczlInfoListComparison;
            List list1 = jt.queryForList(sql, new Object[]{date});
            // for(int index=0;index<list1.size();index++){
            // list.add(list1.get(index));
            // }
            list.addAll(list1);
        }
        return list;
        // String sql = SQLScript.getGczlInfoListComparison;
        // String date = b_year+"-"+month;
        // return jt.queryForList(sql, new Object[] { date});
    }

    @Override
    public List getGczlInfoExcretion(String b_date, String e_date) {
        String sql = SQLScript.getGczlInfoExcretion;
        return jt.queryForList(sql, new Object[]{b_date, e_date});
    }

    @Override
    public List getExcretionSum(String b_year, String e_year) {
        String sql = SQLScript.getExcretionSum;
        return jt.queryForList(sql, new Object[]{b_year, e_year});
    }

    @Override
    public List getGczlInfoListForFlood(String year, String flood) {
        int isNot = Type.GetInt(flood);
        List list = null;
        if (isNot == 0) {
            String sql = SQLScript.getGczlInfoListForFloodNot;
            list = jt.queryForList(sql, new Object[]{year});
        } else if (isNot == 1) {
            String sql = SQLScript.getGczlInfoListForFlood;
            list = jt.queryForList(sql, new Object[]{year});
        }
        return list;
    }

    @Override
    public List getGczlInfoListForFloodGates(String b_date, String e_date,
                                             String floodGates) {
        int isNot = Type.GetInt(floodGates);
        List list = null;
        if (isNot == 0) {
            String sql = SQLScript.getGczlInfoListForFloodGatesNot;
            list = jt.queryForList(sql, new Object[]{b_date, e_date});
        } else if (isNot == 1) {
            String sql = SQLScript.getGczlInfoListForFloodGates;
            list = jt.queryForList(sql, new Object[]{b_date, e_date});
        }
        return list;
    }

    @Override
    public List getGczlInfoEveryMonthLevel(String b_date, String e_date,
                                           String type) {
        int isNot = Type.GetInt(type);
        List list = null;
        if (isNot == 1) {
            String sql = SQLScript.getGczlInfoEveryMonthMaxLevel;
            list = jt.queryForList(sql, new Object[]{b_date, e_date});
        } else if (isNot == 2) {
            String sql = SQLScript.getGczlInfoEveryMonthMinLevel;
            list = jt.queryForList(sql, new Object[]{b_date, e_date});
        } else if (isNot == 3) {
            String sql = SQLScript.getGczlInfoEveryMonthAvgLevel;
            list = jt.queryForList(sql, new Object[]{b_date, e_date});
        }
        return list;
    }

    @Override
    public List getMptpCodeList() {
        String sql = SQLScript.getMptpCodeList;
        return jt.queryForList(sql);
    }

    @Override
    public List getMptCodeList(String mptp_code) {
        String sql = SQLScript.getMptCodeList;
        return jt.queryForList(sql, new Object[]{mptp_code});
    }

    @Override
    public int doSavecyInfo(CeyaInfo info) {
        if (info.getTab_seed() != 0) {
            List list = jt.queryForList(SQLScript.getMpt_codeFromCyInfo,
                    new Object[]{info.getMpt_name(), info.getMptp_code()});
            String mpt_code = "";
            if (list != null && list.size() > 0) {
                Map map = (Map) list.get(0);
                mpt_code = (String) map.get("mpt_code");
            }
            info.setMpt_code(mpt_code);
            ht.update(info);
        } else {
            int maxID = jt.queryForInt(SQLScript.getMaxIDFromCyInfo) + 1;
            List list = jt.queryForList(SQLScript.getMpt_codeFromCyInfo,
                    new Object[]{info.getMpt_name(), info.getMptp_code()});
            String mpt_code = "";
            if (list != null && list.size() > 0) {
                Map map = (Map) list.get(0);
                mpt_code = (String) map.get("mpt_code");
            }
            String unit_code = (String) jt.queryForMap(
                    SQLScript.getUnitCodeFormUnitInfo).get("unit_code");
            info.setUnit_code(unit_code);
            info.setMpt_code(mpt_code);
            info.setTab_seed(maxID);
            ht.save(info);
        }
        return info.getTab_seed();
    }

    @Override
    public void doDeletecyInfoById(int tab_seed) {
        String sql = SQLScript.doDeleteCyInfoById;
        jt.update(sql, new Object[]{tab_seed});

    }

    public List getCyInfoListWithGroupQuery(String queryStr, PageInfo pageInfo,
                                            StandardQuery query) {
        String sql = SQLScript.getCyInfoListWithGroupQuery;
        SQLGenerator generator = null;
        if (queryStr != null && !("").equals(queryStr)) {
            generator = new SQLGenerator(sql
                    + " select * into #result  from #temp where " + queryStr);
        } else {
            generator = new SQLGenerator(sql
                    + " select * into #result  from #temp    ");

        }
        if (query != null) {
            query.addToSQLGenerator(generator);
        }

        String sqlStr = WebUtil.generateSQLStr(generator.getSQL(), pageInfo);
        Object[] args = (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
                .getLimit() == 0)) ? new Object[]{} : new Object[]{
                new Integer(pageInfo.getStart() + 1),
                new Integer(pageInfo.getStart() + 1 + pageInfo.getLimit())};
        return jt.queryForList(sqlStr, args);
    }

    @Override
    public BigDecimal getTubo_high_degree(String mptp_code, String mpt_name) {
        String sql = "select tubo_high_degree from [xmis].[gczl_measure_press_tube_dict] where mptp_code = ? and mpt_name = ?";
        List list = jt.queryForList(sql, new Object[]{mptp_code,
                new String(mpt_name)});
        if (list != null && list.size() > 0) {
            Map map = (Map) list.get(0);
            return (BigDecimal) map.get("tubo_high_degree");
        }
        return null;
    }

    public List getCeYaColumnNameCombo() {
        String sql = SQLScript.getCeYaColumnNameCombo;
        return jt.queryForList(sql);
    }

    @Override
    public JsonResponse getFileFlagList() {
        String sql = " SELECT  DISTINCT file_flag FROM xmis.T_Document_App_Form_Detail WHERE file_flag IS NOT NULL";

        List result = jt.queryForList(sql);
        int size = result.size();
        StoreResponse response = new StoreResponse();
        response.setSuccess(true);
        response.setTotalCount(size);
        response.setRows(result);
        return response;
    }

    @Override
    public List getDutyLevel(String empId) {
        String sql = "select 角色ID from [bbz].[xmis].[sec_人员角色] WHERE [人员ID] = ?";
        return jt.queryForList(sql, new Object[]{empId});
    }

    @Override
    public int getSign_No(String year) {
        // TODO Auto-generated method stub
        String sql = SQLScript.getSign_No;
        int sign_no = jt.queryForInt(sql, new Object[]{year});
        return sign_no;
    }

    @Override
    public void dosendHuiQianMessage(String indexStr, String formId, String type) {
        // TODO Auto-generated method stub

        String[] receivers = (indexStr.substring(0, indexStr.length() - 1))
                .split(",");
        EmployeeObj emp = WebUtil.getCurrentEmployee();
        Calendar calendar = Calendar.getInstance();
        java.util.Date nowDate = new java.util.Date(calendar.getTimeInMillis());
        String app_time = DateUtil.formatDateTime(nowDate);
        for (int i = 0; i < receivers.length; i++) {
            int receiverId = Type.GetInt(receivers[i]);
            MessageInfo info = new MessageInfo();
            EmployeeObj receiver = new EmployeeObj();
            receiver.setUserID(receiverId);
            info.setReceiver(receiver);
            info.setSender(emp);
            info.setContent("officialdocument/applyDocument.jsp?isAgree=false&type="
                    + type + "&form_id=" + formId);
            info.setSendTime(nowDate);
            info.setState(1);
            info.setTitle(emp.getUserName() + "要与您公文会签");
            ht.save(info);
            // String sql =
            // "insert into [bbz].[xmis].[msg_消息] (接收人,发送人,标题,内容,发送时间,是否已读) values('"+str+"','"
            // + emp.getUserID() +
            // "','公文会签','officialdocument/applyDocument.jsp?&form_id="+formId+"','"+app_time+"','1')";
            // jt.update(sql);
        }

    }

    public void doupdateHuiQianMessage(final String isAgree,
                                       final String formId, final String type) {
        // TODO Auto-generated method stub

        String sql = "update [bbz].[xmis].[msg_消息] set 内容  = ? where 接收人= ? and 内容 = ?";
        final EmployeeObj emp = WebUtil.getCurrentEmployee();
        int count = jt.update(sql, new PreparedStatementSetter() {

            public void setValues(PreparedStatement ps) throws SQLException {

                ps.setString(1, "officialdocument/applyDocument.jsp?isAgree="
                        + isAgree + "&type=" + type + "&form_id=" + formId);
                ps.setInt(2, emp.getUserID());
                ps.setString(3,
                        "officialdocument/applyDocument.jsp?isAgree=false&type="
                                + type + "&form_id=" + formId);
            }
        });
        if (count > 0) {
            String getNotagreeCount = "select * from [bbz].[xmis].[msg_消息] WHERE 内容   ='officialdocument/applyDocument.jsp?isAgree=false&type="
                    + type + "&form_id=" + formId + "'";
            List notagreeCountList = jt.queryForList(getNotagreeCount);
            if (notagreeCountList.size() < 1) {
                String getSender = "select 发送人 from [bbz].[xmis].[msg_消息] where 内容   like '%form_id="
                        + formId + "' group by 发送人";
                List senderList = jt.queryForList(getSender);
                int receiverId = Type.GetInt(((Map) senderList.get(0))
                        .get("发送人"));
                Calendar calendar = Calendar.getInstance();
                java.util.Date nowDate = new java.util.Date(
                        calendar.getTimeInMillis());
                MessageInfo info = new MessageInfo();
                EmployeeObj receiver = new EmployeeObj();
                receiver.setUserID(receiverId);
                info.setReceiver(receiver);
                info.setSender(receiver);
                info.setContent("officialdocument/applyDocument.jsp?continueEdit=true&type="
                        + type + "&form_id=" + formId);
                info.setSendTime(nowDate);
                info.setState(1);
                info.setTitle("公文会签已同意");
                ht.save(info);
            }
        }
    }

    @Override
    public String getHQ_nameByFormId(String formId, String type) {
        String sql = "SELECT  DISTINCT RTRIM(hr.姓氏)+RTRIM(hr.名字) as HQ_name  FROM  "
                + "[bbz].[xmis].[msg_消息] msg  "
                + "LEFT OUTER JOIN xmis.hr_人员 hr ON msg.接收人 = hr.人员ID  WHERE 接收人!= 发送人 AND 内容 LIKE "
                + "'%form_id=" + formId + "'";
        List list = jt.queryForList(sql);
        if (list.size() > 0) {
            String hQ_name = "";
            for (int i = 0; i < list.size(); i++) {
                hQ_name += ((Map) list.get(i)).get("HQ_name") + "    ";
            }
            return hQ_name;
        }
        return "";
    }

    @Override
    public Map<String, String> getDoDayReportData(String datetime) {
        String sql = "select open_value,gate_type,gate_name,gate_liuliang from [bbz].[xmis].[gczl_闸门开度流量表] where open_datetime = ?";
        String sql1 = "select 上游水位 as shangyou,下游水位 as xiayou from [bbz].[xmis].[gczl_水文站上下水位数据] where 观测时间 = ? ";
        List<Map<String, String>> list = jt.queryForList(sql, new String[]{datetime});
        List<Map<String, String>> list1 = jt.queryForList(sql1, new String[]{datetime});
        Map<String, String> data = new HashMap<>();
        Map map = null;
        int new_open_count = 0;
        int old_open_count = 0;
        float new_open_value = 0;
        float old_open_value = 0;
        String new_open_konghao = "";
        String old_open_konghao = "";
        float liuliang = 0;
        for (int i = 0; i < list.size(); i++) {
            map = list.get(i);
            if (map.get("open_value") != null && Float.parseFloat(map.get("open_value").toString()) > 0.1) {
                liuliang = liuliang + Float.parseFloat(map.get("gate_liuliang").toString() == null ? "0" : map.get("gate_liuliang").toString());
                if ("12".equals(map.get("gate_type").toString())) {
                    new_open_count++;
                    new_open_value = new_open_value + Float.parseFloat(map.get("open_value").toString());
                    String name = map.get("gate_name").toString();
                    new_open_konghao = new_open_konghao + name.substring(name.indexOf("孔闸") + 2, name.indexOf("号孔")) + "，";
                } else {
                    old_open_count++;
                    old_open_value = old_open_value + Float.parseFloat(map.get("open_value").toString());
                    String name = map.get("gate_name").toString();
                    old_open_konghao = old_open_konghao + name.substring(name.indexOf("孔闸") + 2, name.indexOf("号孔")) + "，";
                }
            }
        }
        new_open_konghao = "".equals(new_open_konghao) ? "" : new_open_konghao.substring(0, new_open_konghao.length() - 1);
        old_open_konghao = "".equals(old_open_konghao) ? "" : old_open_konghao.substring(0, old_open_konghao.length() - 1);
        if (list1.size() == 0) {
            data.put("shang_shuiwei", "");
            data.put("xia_shuiwei", "");
        } else {
            data.put("shang_shuiwei", list1.get(0).get("shangyou"));
            data.put("xia_shuiwei", list1.get(0).get("xiayou"));
        }
        DecimalFormat df = new DecimalFormat("#0.000");
        data.put("new_kongshu", new_open_count + "");
        data.put("old_kongshu", old_open_count + "");
        data.put("new_kaidu", df.format(new_open_count == 0 ? 0 : (new_open_value / new_open_count)));
        data.put("old_kaidu", df.format(old_open_count == 0 ? 0 : (old_open_value / old_open_count)));
        data.put("new_konghao", new_open_konghao);
        data.put("old_konghao", old_open_konghao + "");
        data.put("liuliang", liuliang + "");
        return data;
    }

    @Override
    public void saveDoDayReportData(Map<String, String> map) throws Exception {
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String checkSql = "select count(*) from gczl_闸门控制系统报表 where doTime = ?";
        String lastRecord = "select top 1 doTime,liuliang from gczl_闸门控制系统报表 where doTime<? order by doTime desc";
        String updateSql = "update gczl_闸门控制系统报表 set doName = ? ,currentll =? where doTime = ?";
        String insertSql = "insert into gczl_闸门控制系统报表(doTime,doName,new_kongshu,new_kaidu,new_konghao,old_kongshu,old_kaidu,old_konghao,shang_shuiwei,xia_shuiwei,liuliang,currentll) values (?,?,?,?,?,?,?,?,?,?,?,?)";
        int i = jt.queryForInt(checkSql, new String[]{map.get("doTime")});
        String doTime = map.get("doTime");
        Map rs = jt.queryForMap(lastRecord, new String[]{map.get("doTime")});
        String lastDoTime = String.valueOf(rs.get("doTime"));
        float lastLiuliang = Float.valueOf(String.valueOf(rs.get("liuliang")));
        float current_ll = (dateFormat.parse(doTime).getTime() - dateFormat.parse(lastDoTime).getTime()) / 1000 * lastLiuliang;
        if (i > 0) {
            jt.update(updateSql, new String[]{map.get("doName"), current_ll + "", map.get("doTime")});
        } else {
            jt.update(insertSql, new String[]{map.get("doTime"), map.get("doName"), map.get("new_kongshu"), map.get("new_kaidu"), map.get("new_konghao"), map.get("old_kongshu"), map.get("old_kaidu"), map.get("old_konghao"), map.get("shang_shuiwei"), map.get("xia_shuiwei"), map.get("liuliang"), current_ll + ""});
        }
    }

    @Override
    public List<Map<String, String>> queryDoDayReportData(String startDate, String endDate) {
        String sql = "select CONVERT(varchar(100),doTime, 20) as doTime,doName,new_kongshu,new_kaidu,new_konghao,old_kongshu,old_kaidu,old_konghao,shang_shuiwei,xia_shuiwei,liuliang from gczl_闸门控制系统报表 where doTime >=? and doTime <= ? order by doTime desc";
        List<Map<String, String>> list = jt.queryForList(sql, new String[]{startDate, endDate});
        return list;
    }

    @Override
    public Map queryzmDayReportData(String currentDate) throws ParseException {
        DateFormat df = DateFormat.getDateInstance();
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
        String dateValue = dateFormat.format(df.parse(currentDate));
        StringBuffer sb = new StringBuffer();
        for (int i = 0; i < 24; i++) {
            sb.append("'");
            sb.append(dateValue);
            sb.append(" ");
            sb.append(i);
            sb.append(":00:00'");
            sb.append(",");
        }
        String[] names = {"ling", "yi", "er", "san", "si", "wu", "liu", "qi", "ba", "jiu", "shi",
                "shiyi", "shier", "shisan", "shisi", "shiwu", "shiliu", "shiqi", "shiba", "shijiu", "ershi",
                "eryi", "erer", "ersan"};
        String sw = "select 上游水位 as shangyou,下游水位 as xiayou,DATEPART(hh,观测时间) as datehour  from [bbz].[xmis].[gczl_水文站上下水位数据]  where 观测时间 in (" + sb.substring(0, sb.length() - 1) + ") order by 观测时间 asc";
        List<Map<String, String>> swList = jt.queryForList(sw);
        Map map = null;
        String name = null;
        String dateHour = null;
        Map<String, Float> swMap = new HashMap<>();
        for (int i = 0; i < swList.size(); i++) {
            map = swList.get(i);
            dateHour = names[Integer.parseInt(String.valueOf(map.get("datehour")))];
            if (map.get("shangyou") != null) {
                swMap.put("shangyou_" + dateHour, Float.parseFloat(String.valueOf(map.get("shangyou"))));
            } else {
                swMap.put("shangyou_" + dateHour, 0f);
            }
            if (map.get("xiayou") != null) {
                swMap.put("xiayou_" + dateHour, Float.parseFloat(String.valueOf(map.get("xiayou"))));
            } else {
                swMap.put("xiayou_" + dateHour, 0f);
            }
        }
        String ll = "select open_value,gate_type,gate_name,gate_liuliang,DATEPART(hh,open_datetime) as datehour  from [bbz].[xmis].[gczl_闸门开度流量表] where open_datetime in (" + sb.substring(0, sb.length() - 1) + ")";
        List<Map<String, String>> lllist = jt.queryForList(ll);
        Map<String, String> kaiduMap = new HashMap<>();

        Map<String, Float> llMap = new HashMap<>();
        for (int i = 0; i < lllist.size(); i++) {
            map = lllist.get(i);
            name = String.valueOf(map.get("gate_name"));
            name = name.substring(name.indexOf("孔闸") + 2, name.indexOf("号孔"));
            dateHour = names[Integer.parseInt(String.valueOf(map.get("datehour")))];
            name = name + "_" + dateHour;
            if (llMap.get(dateHour) == null) {
                llMap.put(dateHour, Float.parseFloat(String.valueOf(map.get("gate_liuliang"))));
            } else {
                llMap.put(dateHour, llMap.get(dateHour) + Float.parseFloat(String.valueOf(map.get("gate_liuliang"))));
            }
            if ("12".equals(String.valueOf(map.get("gate_type")))) {
                kaiduMap.put("new_" + name, String.valueOf(map.get("open_value")));
            } else {
                kaiduMap.put("old_" + name, String.valueOf(map.get("open_value")));
            }
        }
        Map<String, Object> result = new HashMap();
        result.put("liuliang", llMap);
        result.put("shuiwei", swMap);
        result.put("kaidu", kaiduMap);
        return result;
    }

    @Override
    public Map<String, String> getTopReportData(String startDate, String endDate) throws ParseException {
        Calendar calendar = Calendar.getInstance();
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd");
        SimpleDateFormat dateFormat1 = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String currentDate = dateFormat1.format(calendar.getTime());
        if (endDate != null && !"".equals(endDate)) {
            currentDate = endDate;
        }
        calendar.add(Calendar.DAY_OF_MONTH, -1);
        String yestoday = dateFormat.format(calendar.getTime()) + " 08:00:00";
        if (startDate != null && !"".equals(startDate)) {
            yestoday = startDate;
        }
        String sql = "select open_value,gate_type,gate_name,gate_liuliang,CONVERT(varchar(100),open_datetime, 20) as dotime from [bbz].[xmis].[gczl_闸门开度流量表] where open_datetime>=? and open_datetime<= ?  order by open_datetime desc";
        String sql1 = "select 上游水位 as shangyou,下游水位 as xiayou,CONVERT(varchar(100),观测时间, 20) as dotime from [bbz].[xmis].[gczl_水文站上下水位数据] where 观测时间>=? and 观测时间<=? and 上游水位 is not null and 下游水位 is not null order by 观测时间 desc";
        List<Map<String, String>> kdll = jt.queryForList(sql, new String[]{yestoday, currentDate});
        List<Map<String, String>> shuiwei = jt.queryForList(sql1, new String[]{yestoday, currentDate});
        Map data = new HashMap<>();
        Map map = null;
        float liuliang = 0;
        Map<Object, Object> llMap = new TreeMap();
        DecimalFormat df = new DecimalFormat("#0.000");
        String new_open_konghao = "";
        String old_open_konghao = "";
        int new_open_count = 0;
        int old_open_count = 0;
        float new_open_value = 0;
        float old_open_value = 0;
        for (int i = 0; i < kdll.size(); i++) {
            map = kdll.get(i);
            liuliang = Float.parseFloat(map.get("gate_liuliang").toString() == null ? "0" : map.get("gate_liuliang").toString());
            if (llMap.get(map.get("dotime")) == null) {
                llMap.put(map.get("dotime"), df.format(liuliang));
            } else {
                float tmp = Float.parseFloat(String.valueOf(llMap.get(map.get("dotime")))) + liuliang;
                llMap.put(map.get("dotime"), df.format((float) tmp));
            }
            if (i < 40 && map.get("open_value") != null && Float.parseFloat(map.get("open_value").toString()) > 0.1) {
                if ("12".equals(map.get("gate_type").toString())) {
                    new_open_count++;
                    String name = map.get("gate_name").toString();
                    new_open_value = new_open_value + Float.parseFloat(map.get("open_value").toString());
                    new_open_konghao = new_open_konghao + name.substring(name.indexOf("孔闸") + 2, name.indexOf("号孔")) + ",";
                } else {
                    old_open_count++;
                    String name = map.get("gate_name").toString();
                    old_open_value = old_open_value + Float.parseFloat(map.get("open_value").toString());
                    old_open_konghao = old_open_konghao + name.substring(name.indexOf("孔闸") + 2, name.indexOf("号孔")) + ",";
                }
            }
        }
        new_open_konghao = "".equals(new_open_konghao) ? "" : new_open_konghao.substring(0, new_open_konghao.length() - 1);
        old_open_konghao = "".equals(old_open_konghao) ? "" : old_open_konghao.substring(0, old_open_konghao.length() - 1);
        Map sswMap = new TreeMap();
        Map xswMap = new TreeMap();
        for (int i = 0; i < shuiwei.size(); i++) {
            map = shuiwei.get(i);
            sswMap.put(map.get("dotime"), map.get("shangyou") == null ? "0" : map.get("shangyou"));
            xswMap.put(map.get("dotime"), map.get("xiayou") == null ? "0" : map.get("xiayou"));
        }
        data.put("liuliang", llMap);
        data.put("shangyou", sswMap);
        data.put("xiayou", xswMap);
        data.put("new_kaidu", df.format(new_open_count == 0 ? 0 : (new_open_value / new_open_count)));
        data.put("old_kaidu", df.format(old_open_count == 0 ? 0 : (old_open_value / old_open_count)));
        data.put("new_open_konghao", new_open_konghao);
        data.put("old_open_konghao", old_open_konghao);
        return data;
    }

    @Override
    public String doCreateDdWord(String queryDate) {
        String sql = "select currentll as sum_ll from [bbz].[xmis].[gczl_闸门控制系统报表] where doTime = ?";
        Map rs = jt.queryForMap(sql, new String[]{queryDate});
        DecimalFormat df = new DecimalFormat("#0.00");
        if (rs != null && rs.get("sum_ll") != null) {
            double value = Double.valueOf(String.valueOf(rs.get("sum_ll"))) / 10000;
            return df.format(value);
        }
        return "0.00";
    }

    @Override
    public void deleteDoDayReportData(String queryDate, String queryPerson) {
        if (queryPerson == null || "".equals(queryPerson)) {
            String sql = "delete from gczl_闸门控制系统报表 where doTime =? and (doName is null or doName = '')";
            jt.update(sql, new String[]{queryDate});
        } else {
            String sql = "delete from gczl_闸门控制系统报表 where doTime =? and doName = ?";
            jt.update(sql, new String[]{queryDate, queryPerson});
        }

    }
}
