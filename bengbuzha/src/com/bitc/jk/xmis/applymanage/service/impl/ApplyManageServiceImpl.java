package com.bitc.jk.xmis.applymanage.service.impl;

import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.math.BigDecimal;
import java.net.URLEncoder;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletResponse;

import com.bitc.jk.xmis.util.*;
import org.apache.poi.hssf.usermodel.*;
import org.apache.poi.ss.usermodel.CellStyle;
import org.apache.poi.ss.util.CellRangeAddress;
import org.apache.poi.ss.util.Region;
import org.codehaus.jackson.JsonGenerationException;
import org.fireflow.engine.EngineException;
import org.fireflow.engine.IProcessInstance;
import org.fireflow.engine.IWorkflowSession;
import org.fireflow.engine.RuntimeContext;
import org.fireflow.kernel.KernelException;

import com.bitc.jk.xmis.applymanage.dao.ApplyManageDao;
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
import com.bitc.jk.xmis.applymanage.service.ApplyManageService;
import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.MessageInfo;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.msg.MessageSender;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.dao.WorkflowDAO;
import com.bitc.jk.xmis.workflow.exception.WorkflowException;
import com.bitc.jk.xmis.workflow.model.ProcessWithKeyword;
import com.bitc.jk.xmis.workflow.model.WorkflowApprovalInfo;
import com.bitc.jk.xmis.workflow.service.WorkflowService;
import com.bitc.jk.xmis.workflow.util.WorkflowUtil;

public class ApplyManageServiceImpl implements ApplyManageService {

    private ApplyManageDao applyManageDao;
    private RuntimeContext workflowContext;
    private OrganizationDAO organizationDAOImpl;
    private WorkflowDAO workflowDAO;
    private WorkflowService workflowService;
    private MessageSender<MessageInfo> messageSender;

    public WorkflowService getWorkflowService() {
        return workflowService;
    }

    public void setWorkflowService(WorkflowService workflowService) {
        this.workflowService = workflowService;
    }

    public MessageSender<MessageInfo> getMessageSender() {
        return messageSender;
    }

    public void setMessageSender(MessageSender<MessageInfo> messageSender) {
        this.messageSender = messageSender;
    }

    public WorkflowDAO getWorkflowDAO() {
        return workflowDAO;
    }

    public void setWorkflowDAO(WorkflowDAO workflowDAO) {
        this.workflowDAO = workflowDAO;
    }

    public OrganizationDAO getOrganizationDAOImpl() {
        return organizationDAOImpl;
    }

    public void setOrganizationDAOImpl(OrganizationDAO organizationDAOImpl) {
        this.organizationDAOImpl = organizationDAOImpl;
    }

    public RuntimeContext getWorkflowContext() {
        return workflowContext;
    }

    public void setWorkflowContext(RuntimeContext aWorkflowContext) {
        this.workflowContext = aWorkflowContext;
    }

    public ApplyManageDao getApplyManageDao() {
        return applyManageDao;
    }

    public void setApplyManageDao(ApplyManageDao applyManageDao) {
        this.applyManageDao = applyManageDao;
    }

    public String doSaveAppMeetingRoom(ApplyMeetingRoomForm applyMeetingRoom) {
        return applyManageDao.doSaveAppMeetingRoom(applyMeetingRoom);
    }

    @SuppressWarnings("rawtypes")
    public String getApplyMeetingRoomFormById(String id) {
        List list = applyManageDao.getApplyMeetingRoomFormById(id);
        return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
    }

    @SuppressWarnings({"rawtypes", "unchecked"})
    public String getAppMeetingRoomFormList(PageInfo pageInfo, String queryStr) {
        String ordersql = WebUtil.getOrderSqlByPageInfo(pageInfo);
        List list = applyManageDao
                .getAppMeetingRoomFormList(queryStr, ordersql);
        List notValidList = new ArrayList();
        for (int i = 0; i < list.size(); i++) {
            Map map = (Map) list.get(i);
            int status_id = Type.getInteger(map.get("status_id"));
            String begin_datetime = Type.getString(map.get("begin_datetime"));
            String end_datetime = Type.getString(map.get("end_datetime"));
            String nowdatetime = DateUtil.formatDateTime(DateUtil
                    .getCurrentDateTime());
            if (status_id == 1 || status_id == 2 || status_id == 3) {
                boolean isOverdue = isDateBefore(begin_datetime, nowdatetime);
                // 处理已过期状态
                if (isOverdue) {
                    map.put("status_name", "已过期");
                }

                boolean isInCurrentDate = isInCurrentDate(begin_datetime);
                // 显示当天过期
                if (isOverdue && !isInCurrentDate) {
                    notValidList.add(list.get(i));
                }
            }
            if (status_id == 3) {
                boolean isInuse = (isDateBefore(begin_datetime, nowdatetime))
                        && (isDateBefore(nowdatetime, end_datetime));
                // 处理使用中状态
                if (isInuse) {
                    map.put("status_name", "使用中");
                }
                boolean isInCurrentDate = isInCurrentDate(begin_datetime);
                // 显示当天使用中的
                if (isInuse && !isInCurrentDate) {
                    notValidList.add(list.get(i));
                }
            }
            if (status_id == 4) {
                boolean isOverdue = (isDateBefore(end_datetime, nowdatetime));
                // 删除今天之前的撤销
                if (isOverdue) {
                    notValidList.add(list.get(i));
                }
            }
        }
        list.removeAll(notValidList);

        List aList = getListWithPageInfo(list, pageInfo);
        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
                list.size());
    }

    @SuppressWarnings({"unchecked", "rawtypes"})
    public List getListWithPageInfo(List list, PageInfo pageInfo) {
        List resultList = new ArrayList();
        int limit = pageInfo.getLimit();
        int start = pageInfo.getStart();
        int size = list.size();
        int limitSize = start + limit;
        if (limitSize > size) {
            limitSize = size;
        }
        if (limit == 0 || list.size() < limit) {
            return list;
        } else {
            for (int i = start; i < limitSize; i++) {
                resultList.add(list.get(i));
            }
            return resultList;
        }

    }

    @SuppressWarnings("rawtypes")
    public String getMeetingRoomCombo() {
        List list = applyManageDao.getMeetingRoomCombo();
        return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
    }

    public int doHandleAppMeetingRoomStatus(String id, int status_id) {
        return applyManageDao.doHandleAppMeetingRoomStatus(id, status_id);
    }

    // 判断时间date1是否在时间date2之前
    public boolean isDateBefore(String date1, String date2) {
        try {
            DateFormat df = DateFormat.getDateTimeInstance();
            return df.parse(date1).before(df.parse(date2));
        } catch (ParseException e) {
            return false;
        }
    }

    public boolean isInCurrentDate(String date1) {
        if (DateUtil.formatDate(Type.getDate(date1)).equals(
                Type.getString(DateUtil.getCurrentDate()))) {
            return true;
        }
        return false;
    }

    @SuppressWarnings("rawtypes")
    public boolean getMeetingRoomUsingTimeList(String type, String params_id,
                                               String new_begin_datetime, String new_end_datetime, String form_id) {

        boolean isInuse = false;
        List list = null;
        if (type.equals("car")) {
            if (params_id != null && !"".equals(params_id)) {
                if (params_id.indexOf(",") > -1) {
                    String[] caridsArray = params_id.split(",");
                    for (int i = 0; i < caridsArray.length; i++) {
                        list = applyManageDao.getCarUsingTimeList(
                                caridsArray[i], form_id);
                        for (int i1 = 0; i1 < list.size(); i1++) {
                            Map map = (Map) list.get(i1);
                            String begin_datetime = Type.getString(map
                                    .get("begin_datetime"));
                            String end_datetime = Type.getString(map
                                    .get("end_datetime"));
                            if ((isDateBefore(begin_datetime,
                                    new_begin_datetime) && isDateBefore(
                                    new_begin_datetime, end_datetime))
                                    || (isDateBefore(begin_datetime,
                                    new_end_datetime) && isDateBefore(
                                    new_end_datetime, end_datetime))
                                    || (isDateBefore(new_begin_datetime,
                                    begin_datetime) && isDateBefore(
                                    end_datetime, new_end_datetime))) {
                                return true;
                            } else {
                                continue;
                            }
                        }
                    }
                } else {
                    list = applyManageDao.getCarUsingTimeList(params_id,
                            form_id);
                }
            }
            list = applyManageDao.getCarUsingTimeList(params_id, form_id);
        } else {
            list = applyManageDao.getMeetingRoomUsingTimeList(params_id,
                    form_id);
        }
        for (int i = 0; i < list.size(); i++) {
            Map map = (Map) list.get(i);
            String begin_datetime = Type.getString(map.get("begin_datetime"));
            String end_datetime = Type.getString(map.get("end_datetime"));
            if ((isDateBefore(begin_datetime, new_begin_datetime) && isDateBefore(
                    new_begin_datetime, end_datetime))
                    || (isDateBefore(begin_datetime, new_end_datetime) && isDateBefore(
                    new_end_datetime, end_datetime))
                    || (isDateBefore(new_begin_datetime, begin_datetime) && isDateBefore(
                    end_datetime, new_end_datetime))) {
                return true;
            } else {
                continue;
            }
        }
        return isInuse;
    }

    public String getApplyMeetingFormTipsList() {
        List list = applyManageDao.getApplyMeetingFormChangeCount();
        return WebUtil.buildStoreForJsonWithoutPageInfo(list);
    }

    @SuppressWarnings({"rawtypes", "unchecked"})
    public String getAppCarList(PageInfo pageInfo, String queryStr) {
        String ordersql = WebUtil.getOrderSqlByPageInfo(pageInfo);
        List list = applyManageDao.getAppCarList(queryStr, ordersql);
        List notValidList = new ArrayList();
        for (int i = 0; i < list.size(); i++) {
            Map map = (Map) list.get(i);
            int status_id = Type.getInteger(map.get("status_id"));
            String begin_datetime = Type.getString(map.get("begin_datetime"));
            String end_datetime = Type.getString(map.get("end_datetime"));
            String nowdatetime = DateUtil.formatDateTime(DateUtil
                    .getCurrentDateTime());
            if (status_id == 1 || status_id == 2 || status_id == 3) {
                boolean isOverdue = isDateBefore(begin_datetime, nowdatetime);
                // 处理已过期状态
                if (isOverdue) {
                    map.put("status_name", "已过期");
                }

                boolean isInCurrentDate = isInCurrentDate(begin_datetime);
                // 显示当天过期
                if (isOverdue && !isInCurrentDate) {
                    notValidList.add(list.get(i));
                }
            }
            if (status_id == 3) {
                boolean isInuse = (isDateBefore(begin_datetime, nowdatetime))
                        && (isDateBefore(nowdatetime, end_datetime));
                // 处理使用中状态
                if (isInuse) {
                    map.put("status_name", "使用中");
                }
                boolean isInCurrentDate = isInCurrentDate(begin_datetime);
                // 显示当天使用中的
                if (isInuse && !isInCurrentDate) {
                    notValidList.add(list.get(i));
                }
            }
            if (status_id == 4) {
                boolean isOverdue = (isDateBefore(end_datetime, nowdatetime));
                // 删除今天之前的撤销
                if (isOverdue) {
                    notValidList.add(list.get(i));
                }
            }
        }
        list.removeAll(notValidList);
        List aList = getListWithPageInfo(list, pageInfo);
        return WebUtil.buildJsonStoreByList(aList);
    }

    @SuppressWarnings("rawtypes")
    public String getVehicleManagementListToSelectGrid() {
        List list = applyManageDao.getVehicleManagementListToSelectGrid();
        return WebUtil.buildJsonStoreByList(list);
    }

    public String doSaveAppCars(ApplyCarForm applyCarForm, String caridStrs) {
        String return_id = applyManageDao.doSaveAppCars(applyCarForm);
        if (!"".equals(applyCarForm.getForm_id())) {
            applyManageDao.doDelCarAppDetails(applyCarForm.getForm_id());
        }
        if (caridStrs != null && !"".equals(caridStrs)) {
            if (caridStrs.indexOf(",") > -1) {
                String[] caridsArray = caridStrs.split(",");
                for (int i = 0; i < caridsArray.length; i++) {
                    applyManageDao
                            .doSaveAppCarDetals(return_id, caridsArray[i]);
                }
            } else {
                applyManageDao.doSaveAppCarDetals(return_id, caridStrs);
            }
        }

        return return_id;
    }

    @SuppressWarnings("rawtypes")
    public String getApplyCarByFormId(String form_id, EmployeeObj emp) {
        List list = applyManageDao.getApplyCarByFormId(form_id);
        boolean isApplyPerson = false;
        for (int i = 0; i < emp.getRoles().size(); i++) {
            if (emp.getRoles().get(i) == -22) {
                isApplyPerson = true;
                break;
            }
        }
        //List notValidList = new ArrayList();
        for (int i = 0; i < list.size(); i++) {
            Map map = (Map) list.get(i);
            map.put("isApplyPerson", isApplyPerson);
            int status_id = Type.getInteger(map.get("status_id"));
            String begin_datetime = Type.getString(map.get("begin_datetime"));
            String end_datetime = Type.getString(map.get("end_datetime"));
            String nowdatetime = DateUtil.formatDateTime(DateUtil
                    .getCurrentDateTime());
            if (status_id == 1 || status_id == 2 || status_id == 3) {
                boolean isOverdue = isDateBefore(begin_datetime, nowdatetime);
                // 处理已过期状态
                if (isOverdue) {
                    map.put("status_name", "已过期");
                }
            }
            if (status_id == 3) {
                boolean isInuse = (isDateBefore(begin_datetime, nowdatetime))
                        && (isDateBefore(nowdatetime, end_datetime));
                // 处理使用中状态
                if (isInuse) {
                    map.put("status_name", "使用中");
                }
            }
        }


        //	EmployeeObj emp = (EmployeeObj) request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
        return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
    }

    @SuppressWarnings("rawtypes")
    public String getApplyCarIdsDetalsByFormId(String form_id) {
        String strs = "";
        List list = applyManageDao.getApplyCarIdsDetalsByFormId(form_id);
        if (list != null && list.size() > 0) {
            for (int i = 0; i < list.size(); i++) {
                Map map = (Map) list.get(i);
                String car_id = Type.getString(map.get("car_id"));
                strs += car_id + ",";
            }
            return strs.substring(0, strs.length() - 1);
        } else {
            return "";
        }

    }

    public boolean validBeginEndTime(String now_begin_datetime,
                                     String now_end_datetime) {
        return isDateBefore(now_begin_datetime, now_end_datetime);
    }

    @SuppressWarnings("rawtypes")
    public String getApplyOfficialSuppliesList(String form_id) {
        List list = applyManageDao
                .getApplyOfficialSuppliesListByFormId(form_id);
        return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
    }

    @SuppressWarnings("rawtypes")
    public String getOfficialSuppliesTypeCombo() {
        List list = applyManageDao.getOfficialSuppliesTypeCombo();
        return WebUtil.buildJsonStoreByList(list);
    }

    @SuppressWarnings("rawtypes")
    public String getOfficialSuppliesCombo(String type) {
        List list = applyManageDao.getOfficialSuppliesCombo(type);
        return WebUtil.buildJsonStoreByList(list);
    }

    public String doSaveApplyOfficialSuppliesForm(
            ApplyOfficialSuppliesForm applyForm) {
        String return_id = applyManageDao
                .doSaveApplyOfficialSuppliesForm(applyForm);
        return return_id;
    }

    public void doUpdateApplyOfficialSuppliesForm(
            ApplyOfficialSuppliesForm applyFormParams, String old_form_id,
            String old_supplies_id) {
        String form_id = applyFormParams.getForm_id();
        String supplies_id = applyFormParams.getSupplies_id();
        int priority = applyFormParams.getPriority();
        int qty = applyFormParams.getQty();
        String reason = applyFormParams.getReason();
        int app_user_id = applyFormParams.getApp_user_id();
        Date app_datetime = applyFormParams.getApp_datetime();
        String remark = applyFormParams.getRemark();

        ApplyOfficialSuppliesForm applyForm = new ApplyOfficialSuppliesForm();

        applyForm.setForm_id(form_id);
        applyForm.setSupplies_id(supplies_id);
        applyForm.setPriority(priority);
        applyForm.setQty(qty);
        applyForm.setReason(reason);
        applyForm.setApp_user_id(app_user_id);
        applyForm.setApp_datetime(app_datetime);
        applyForm.setRemark(remark);

        applyManageDao.doUpdateApplyOfficialSuppliesForm(applyForm,
                old_form_id, old_supplies_id);
    }

    public void doDeleteApplyOfficialSuppliesForm(String form_id,
                                                  String supplies_id) {
        applyManageDao.doDeleteApplyOfficialSuppliesForm(form_id, supplies_id);
    }

    @SuppressWarnings("rawtypes")
    public String getOfficalSuppliesByCompositeIds(String form_id,
                                                   String supplies_id) {
        List list = applyManageDao.getOfficalSuppliesByCompositeIds(form_id,
                supplies_id);
        return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
    }

    public String doStartApplyOfficialSupplies(String form_id)
            throws WorkflowException, EngineException, KernelException {
        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
        EmployeeObj emp = WebUtil.getCurrentEmployee();
        int deptID = emp.getDepartmentID();
        Calendar calendar = Calendar.getInstance();
        java.util.Date nowDate = new java.util.Date(calendar.getTimeInMillis());
        String applicant_time = DateUtil.formatDateTime(nowDate);
        // 提交后，做申请时间的修改；流程实例ID、状态的保存(用于“办公用品申领情况查询”)；
        applyManageDao.doUpdateAppDateTimeByFormId(form_id, applicant_time);

        String empId = String.valueOf(emp.getUserID());

        IProcessInstance processInstance = WorkflowUtil.startProcess(
                workflowSession, WorkflowConstants.WF_APPLYOFFICIAL_SUPPLIES,
                empId);

        List DutyLevel = applyManageDao.getDutyLevel(empId);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "isDeptManager",
                false);
        for (int i = 0; i < DutyLevel.size(); i++) {
            int level = Type.GetInt(((Map) DutyLevel.get(i)).get("角色ID"));
            if (level == -5) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                        true);
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isDeptManager",
                        true);
            } else if (level == -6) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isChargeLeader",
                        true);
            } else if (level == -17) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isMagenagerLeader",
                        true);
            }
        }
        WorkflowUtil.addProcessInstanceVariable(processInstance, "form_id",
                form_id);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "applicant",
                empId);
        // WorkflowUtil.addProcessInstanceVariable(processInstance,
        // "deptmanager_role", WebKeys.APPLY_DEPTMANAGER_ROLE);
        // WorkflowUtil.addProcessInstanceVariable(processInstance,
        // "chargeleader_role", WebKeys.APPLY_CHARGELEADER_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "officedirector_role", WebKeys.APPLY_OFFICEDIRECTOR_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "suppliesmanager_role", WebKeys.APPLY_SUPPLIESMANAGER_ROLE);

        WorkflowUtil.addProcessInstanceVariable(processInstance, "deptID",
                deptID);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "applicant_name", emp.getUserName());
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "applicant_time", applicant_time);

        WorkflowUtil.completeFirstTask(workflowSession, processInstance, empId,
                "");
        applyManageDao.doSaveAppFormProcessinstanceIDInfo(form_id,
                processInstance.getId(), 0);
        String appendName = emp.getUserName() + "办公用品领用申请";
        this.doUpdateProcessName(processInstance, appendName);
        return processInstance.getId();
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
//2014-09-15 在tasklistenerEvent里修改
//			int userID = WebUtil.getCurrentEmployee().getUserID();
//			workflowDAO.doUpdateWorkHistoryByProcessInstanceId(
//					processInstanceId, processInstance.getDisplayName() + "||"
//					+ appendName, userID);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public String doApprovalTask(String workItemId, String isAgree,
                                 String comment) throws WorkflowException {
        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(WebUtil.getCurrentEmployee().getUserID()));
        SimpleEmployee emp = WebUtil.getCurrentEmployee();

        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
        IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
                workflowSession, workItemId);
        WorkflowApprovalInfo approvalInfo = new WorkflowApprovalInfo();
        approvalInfo.setApprover(emp);
        approvalInfo.setApprovedTime(DateUtil.getCurrentDateTime());
        approvalInfo.setIsApproved(Type.GetInt(isAgree));
        approvalInfo.setAdvice(comment);
        approvalInfo.setWorkItemID(workItemId);
        approvalInfo.setProcessinstanceID(processInstance.getId());
        workflowDAO.doSaveWorkflowApprovalInfo(approvalInfo);

        WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                "0".equals(isAgree) ? true : false);
        WorkflowUtil.completeTask(workflowSession, workItemId, Type
                        .getString(emp.getUserID()),
                "0".equals(isAgree) ? ("[审批通过] " + comment)
                        : ("[审批未通过] " + comment));

        return processInstance.getId();
    }

    public String doApprovalTask2(String workItemId, String isAgree,
                                  String comment) throws WorkflowException {
        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(WebUtil.getCurrentEmployee().getUserID()));
        SimpleEmployee emp = WebUtil.getCurrentEmployee();

        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
        IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
                workflowSession, workItemId);
        WorkflowApprovalInfo approvalInfo = new WorkflowApprovalInfo();
        approvalInfo.setApprover(emp);
        approvalInfo.setApprovedTime(DateUtil.getCurrentDateTime());
        approvalInfo.setIsApproved(Type.GetInt(isAgree));
        approvalInfo.setAdvice(comment);
        approvalInfo.setWorkItemID(workItemId);
        approvalInfo.setProcessinstanceID(processInstance.getId());
        workflowDAO.doSaveWorkflowApprovalInfo(approvalInfo);

        WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree2",
                "0".equals(isAgree) ? true : false);
        WorkflowUtil.completeTask(workflowSession, workItemId, Type
                        .getString(emp.getUserID()),
                "0".equals(isAgree) ? ("[审批通过] " + comment)
                        : ("[审批未通过] " + comment));

        return processInstance.getId();
    }

    public void doEndApply(String workItemId, String forceStop,
                           String processInstanceId) throws WorkflowException, EngineException {
        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
        IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
                workflowSession, workItemId);
        if (!"".equals(forceStop)) {
            WorkflowUtil.addProcessInstanceVariable(processInstance,
                    "isForceStop", true);// 用于被退回的链接
        }
        // 修改此表单的状态
        applyManageDao.doUpdateAppappFormStatus(processInstanceId, 1);
        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(WebUtil.getCurrentEmployee().getUserID()));
        WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
                .getCurrentEmployee().getUserID() + "", "");

    }

    public String doGoOnSubmitApplyOfficialSupplies(String workItemId)
            throws WorkflowException {
        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();

        IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
                workflowSession, workItemId);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                true);// 再次提交被默认同意
        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(WebUtil.getCurrentEmployee().getUserID()));
        WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
                .getCurrentEmployee().getUserID() + "", "");
        return processInstance.getId();

    }

    @SuppressWarnings("rawtypes")
    public String getApplyOfficialSuppliesListQuery(PageInfo pageInfo,
                                                    String queryStr) {
        String ordersql = WebUtil.getOrderSqlByPageInfo(pageInfo);
        List list = applyManageDao.getApplyOfficialSuppliesListQuery(queryStr,
                ordersql);
        List aList = getListWithPageInfo(list, pageInfo);
        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
                list.size());
    }

    @SuppressWarnings("rawtypes")
    public String getApplyOfficialSuppliesListSum(PageInfo pageInfo,
                                                  String queryStr) {
        String ordersql = WebUtil.getOrderSqlByPageInfo(pageInfo);
        List list = applyManageDao.getApplyOfficialSuppliesListSum(queryStr,
                ordersql);
        List aList = getListWithPageInfo(list, pageInfo);
        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
                list.size());
    }

    @SuppressWarnings("rawtypes")
    public String getDocumentAppFormDetailList(PageInfo pageInfo,
                                               String queryStr) {
        String ordersql = WebUtil.getOrderSqlByPageInfo(pageInfo);
        List list = applyManageDao.getDocumentAppFormDetailList(queryStr,
                ordersql);
        List aList = getListWithPageInfo(list, pageInfo);
        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
                list.size());
    }

    // 维修申请

    public String doStartApplyOfficialSuppliesRepair(String form_id)
            throws EngineException, KernelException, WorkflowException {
        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
        EmployeeObj emp = WebUtil.getCurrentEmployee();
        int deptID = emp.getDepartmentID();
        Calendar calendar = Calendar.getInstance();
        java.util.Date nowDate = new java.util.Date(calendar.getTimeInMillis());
        String applicant_time = DateUtil.formatDateTime(nowDate);
        // 提交后，做申请时间的修改；流程实例ID、状态的保存(用于“办公用品申领情况查询”)；
        applyManageDao.doUpdateAppDateTimeByRepairFormId(form_id,
                applicant_time);

        String empId = String.valueOf(emp.getUserID());

        IProcessInstance processInstance = WorkflowUtil.startProcess(
                workflowSession,
                WorkflowConstants.WF_APPLYOFFICIAL_SUPPLIES_REPAIR, empId);

        List DutyLevel = applyManageDao.getDutyLevel(empId);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "isDeptManager",
                false);
        for (int i = 0; i < DutyLevel.size(); i++) {
            int level = Type.GetInt(((Map) DutyLevel.get(i)).get("角色ID"));
            if (level == -5) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                        true);
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isDeptManager",
                        true);
            } else if (level == -6) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                        true);
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isChargeLeader",
                        true);
            } else if (level == -17) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                        true);
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isMagenagerLeader",
                        true);
            }
        }
        WorkflowUtil.addProcessInstanceVariable(processInstance, "form_id",
                form_id);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "applicant",
                empId);
        // WorkflowUtil.addProcessInstanceVariable(processInstance,
        // "deptmanager_role", WebKeys.APPLY_DEPTMANAGER_ROLE);
        // WorkflowUtil.addProcessInstanceVariable(processInstance,
        // "chargeleader_role", WebKeys.APPLY_CHARGELEADER_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "officedirector_role", WebKeys.APPLY_OFFICEDIRECTOR_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "suppliesmanager_role", WebKeys.APPLY_SUPPLIESMANAGER_ROLE);

        WorkflowUtil.addProcessInstanceVariable(processInstance, "deptID",
                deptID);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "applicant_name", emp.getUserName());
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "applicant_time", applicant_time);

        WorkflowUtil.completeFirstTask(workflowSession, processInstance, empId,
                "");
        applyManageDao.doSaveAppFormProcessinstanceIDInfo(form_id,
                processInstance.getId(), 0);
        String appendName = emp.getUserName() + "办公用品维修申请";
        this.doUpdateProcessName(processInstance, appendName);
        return processInstance.getId();
    }

    @SuppressWarnings("rawtypes")
    public String getApplyOfficialSuppliesRepairListSum(PageInfo pageInfo,
                                                        String queryStr) {
        String ordersql = WebUtil.getOrderSqlByPageInfo(pageInfo);
        List list = applyManageDao.getApplyOfficialSuppliesRepairListSum(
                queryStr, ordersql);
        List aList = getListWithPageInfo(list, pageInfo);
        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
                list.size());
    }

    @SuppressWarnings("rawtypes")
    public String getGczlInfoList(PageInfo pageInfo,
                                  String queryStr) {
        String ordersql = WebUtil.getOrderSqlByPageInfo(pageInfo);
        List list = applyManageDao.getGczlInfoList(
                queryStr, ordersql);
        List aList = getListWithPageInfo(list, pageInfo);
        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
                list.size());
    }

    @SuppressWarnings("rawtypes")
    public String getGczlInfoListWithGroupQuery(PageInfo pageInfo,
                                                String queryStr, StandardQuery query) {
        List list = applyManageDao.getGczlInfoListWithGroupQuery(
                queryStr, pageInfo, query);
        return WebUtil.buildStoreForJson(list, "");
    }

    @SuppressWarnings("rawtypes")
    public String getApplyOfficialSuppliesRepairList(String form_id) {
        List list = applyManageDao.getApplyOfficialSuppliesRepairList(form_id);
        return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
    }

    public void doUpdateApplyOfficialSuppliesRepairForm(
            ApplyOfficialSuppliesRepairForm applyFormParams,
            String old_form_id, String old_supplies_id) {
        String form_id = applyFormParams.getForm_id();
        String supplies_id = applyFormParams.getSupplies_id();
        int priority = applyFormParams.getPriority();
        int qty = applyFormParams.getQty();
        String reason = applyFormParams.getReason();
        int app_user_id = applyFormParams.getApp_user_id();
        Date app_datetime = applyFormParams.getApp_datetime();
        String remark = applyFormParams.getRemark();

        ApplyOfficialSuppliesForm applyForm = new ApplyOfficialSuppliesForm();

        applyForm.setForm_id(form_id);
        applyForm.setSupplies_id(supplies_id);
        applyForm.setPriority(priority);
        applyForm.setQty(qty);
        applyForm.setReason(reason);
        applyForm.setApp_user_id(app_user_id);
        applyForm.setApp_datetime(app_datetime);
        applyForm.setRemark(remark);

        applyManageDao.doUpdateApplyOfficialSuppliesRepairForm(applyForm,
                old_form_id, old_supplies_id);
    }

    public void doDeleteApplyOfficialSuppliesRepairForm(String form_id,
                                                        String supplies_id) {
        applyManageDao.doDeleteApplyOfficialSuppliesRepairForm(form_id,
                supplies_id);
    }

    public String doSaveApplyOfficialSuppliesRepairForm(
            ApplyOfficialSuppliesRepairForm applyForm) {
        String return_id = applyManageDao
                .doSaveApplyOfficialSuppliesRepairForm(applyForm);
        return return_id;
    }

    public void doEndApplyOfficialSuppliesUpdateNum(String workItemId,
                                                    String form_id, String processInstanceId) throws WorkflowException,
            EngineException {
        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
        // 修改此表单的状态
        applyManageDao.doUpdateAppappFormStatus(processInstanceId, 1);
        // 修改会议室表的数量
        doUpdateOfficeSuppliesDetailsNum(form_id);
        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(WebUtil.getCurrentEmployee().getUserID()));
        WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
                .getCurrentEmployee().getUserID() + "", "");
    }

    @SuppressWarnings("rawtypes")
    private void doUpdateOfficeSuppliesDetailsNum(String form_id) {
        List formList = applyManageDao
                .getApplyOfficialSuppliesListByFormId(form_id);
        for (int i = 0; i < formList.size(); i++) {
            Map map = (Map) formList.get(i);
            String supplies_id = Type.getString(map.get("supplies_id"));
            int qty = Type.GetInt(map.get("qty"));
            applyManageDao.doUpdateOfficeSuppliesDetailsNum(supplies_id, qty);
        }

    }

    public void updateLastActorToNull(String processInstanceId) {
        // 历史表的当前执行人清空（发邮件时候不需要此方法）
        workflowDAO.updateLastActorToNull(processInstanceId);
    }

    public String doStartApplyBusinessEntertainment(
            BusinessEntertainment businessEntertainment)
            throws EngineException, KernelException, WorkflowException {
        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
        EmployeeObj emp = WebUtil.getCurrentEmployee();
        int deptID = emp.getDepartmentID();
        String empId = String.valueOf(emp.getUserID());
        applyManageDao
                .doSaveOrUpdateBusinessEntertainment(businessEntertainment);
        String form_id = businessEntertainment.getForm_id();

        IProcessInstance processInstance = WorkflowUtil.startProcess(
                workflowSession,
                WorkflowConstants.WF_APPLY_BUSINESS_ENTERTAINMENT, empId);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "isDeptManager",
                false);
        List DutyLevel = applyManageDao.getDutyLevel(empId);
        for (int i = 0; i < DutyLevel.size(); i++) {
            int level = Type.GetInt(((Map) DutyLevel.get(i)).get("角色ID"));
            if (level == -5) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                        true);
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isDeptManager",
                        true);
            } else if (level == -6) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                        true);
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isChargeLeader",
                        true);
            } else if (level == -17) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                        true);
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isMagenagerLeader",
                        true);
            }
        }
        WorkflowUtil.addProcessInstanceVariable(processInstance, "form_id",
                form_id);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "applicant",
                empId);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "applicantName", emp.getUserName());
        WorkflowUtil.addProcessInstanceVariable(processInstance, "mealsDate",
                DateUtil.formatDate(businessEntertainment.getMeals_date()));
        // WorkflowUtil.addProcessInstanceVariable(processInstance,
        // "deptmanager_role", WebKeys.APPLY_DEPTMANAGER_ROLE);
        // WorkflowUtil.addProcessInstanceVariable(processInstance,
        // "chargeleader_role", WebKeys.APPLY_CHARGELEADER_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "logisticsleader_role", WebKeys.APPLY_LOGISTICSLEADER_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "canteenexecutor_role", WebKeys.APPLY_CANTEENEXECUTOR_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "magenagerleader_role", WebKeys.APPLY_MAGENAGERLEADER_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "officemanager_role", WebKeys.APPLY_SUPPLIESMANAGER_ROLE);

        WorkflowUtil.addProcessInstanceVariable(processInstance, "deptID",
                deptID);

        WorkflowUtil.completeFirstTask(workflowSession, processInstance, empId,
                "");
        applyManageDao.doSaveAppFormProcessinstanceIDInfo(form_id,
                processInstance.getId(), 0);
        String appendName = emp.getUserName()
                + DateUtil.formatDate(businessEntertainment.getMeals_date())
                + "业务招待申报";
        this.doUpdateProcessName(processInstance, appendName);
        return processInstance.getId();
    }

    @SuppressWarnings("rawtypes")
    public String getBusinessEntertainmentFormByFormId(String form_id) {
        List list = applyManageDao
                .getBusinessEntertainmentFormByFormId(form_id);
        return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
    }

    public void doForceStopApplyBusinessEntertainment(String workItemId,
                                                      String forceStop, String processInstanceId,
                                                      String businessEntertentWorkedId, String form_id)
            throws WorkflowException, EngineException {
        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
        IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
                workflowSession, workItemId);
        if (!"".equals(forceStop)) {
            WorkflowUtil.addProcessInstanceVariable(processInstance,
                    "isForceStop", true);// 用于被退回的链接
        }
        // 修改此表单的状态
        applyManageDao.doUpdateBusinessEntertainmentWorkedId(
                Type.GetInt(businessEntertentWorkedId), form_id);
        // 修改此表单的状态
        applyManageDao.doUpdateAppappFormStatus(processInstanceId, 1);
        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(WebUtil.getCurrentEmployee().getUserID()));
        WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
                .getCurrentEmployee().getUserID() + "", "");

    }

    public void doGoOnSubmitBusinessEntertainment(
            BusinessEntertainment businessEntertainment, String workItemId)
            throws WorkflowException {
        applyManageDao
                .doSaveOrUpdateBusinessEntertainment(businessEntertainment);

        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();

        IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
                workflowSession, workItemId);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                true);// 再次提交被默认同意
        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(WebUtil.getCurrentEmployee().getUserID()));
        WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
                .getCurrentEmployee().getUserID() + "", "");

    }

    public void doUpdateApplyBusinessEntertainmentByCanteen(
            BusinessEntertainment businessEntertainment, String workItemId)
            throws WorkflowException {
        applyManageDao
                .doSaveOrUpdateBusinessEntertainment(businessEntertainment);

        boolean isOver = false;
        if (businessEntertainment.getTotal_amt().compareTo(
                businessEntertainment.getMeals_fee_budget()) == 1) {
            isOver = true;
        }

        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();

        IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
                workflowSession, workItemId);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "isOver",
                isOver);//
        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(WebUtil.getCurrentEmployee().getUserID()));
        WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
                .getCurrentEmployee().getUserID() + "", "");
    }

    public void doUpdateSignNoByFormId(String form_id, String sign_No) {
        applyManageDao.doUpdateSignNoByFormId(form_id, sign_No);
    }

    @SuppressWarnings("rawtypes")
    public String getBusinessEntertainmentFormList(String queryStr,
                                                   PageInfo pageInfo) {
        String ordersql = WebUtil.getOrderSqlByPageInfo(pageInfo);
        List list = applyManageDao.getBusinessEntertainmentFormList(queryStr,
                ordersql);
        List aList = getListWithPageInfo(list, pageInfo);
        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList, list.size());
    }

    // 请假
    public String doStartAskForLeave(AbsentForm absentForm)
            throws WorkflowException, EngineException, KernelException {
        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
        EmployeeObj emp = WebUtil.getCurrentEmployee();
        int deptID = emp.getDepartmentID();
        String empId = String.valueOf(emp.getUserID());

        applyManageDao.doSaveOrUpdateAbsentForm(absentForm);
        String form_id = absentForm.getForm_id();
        int days = getDaysFromStr(absentForm.getPlan_begin_date(),
                absentForm.getPlan_end_date());
        int limit1 = 1, limit3 = 3, limit5 = 5;

        IProcessInstance processInstance = WorkflowUtil.startProcess(
                workflowSession, WorkflowConstants.WF_ASK_FOR_LEAVE, empId);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "form_id",
                form_id);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "days", days);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "limit1", limit1);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "limit3", limit3);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "limit5", limit5);


        boolean ischgleader = isInRoles(empId, 1);
        boolean isdptleader = isInRoles(empId, 0);

        WorkflowUtil.addProcessInstanceVariable(processInstance, "ischgleader",
                ischgleader);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "isdptleader",
                isdptleader);

        WorkflowUtil.addProcessInstanceVariable(processInstance, "applicantID",
                empId);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "applicantName", emp.getUserName());
        // WorkflowUtil.addProcessInstanceVariable(processInstance,
        // "deptmanager_role", WebKeys.APPLY_DEPTMANAGER_ROLE);
        // WorkflowUtil.addProcessInstanceVariable(processInstance,
        // "chargeleader_role", WebKeys.APPLY_CHARGELEADER_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "magenagerleader_role", WebKeys.APPLY_MAGENAGERLEADER_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "officemanager_role", WebKeys.APPLY_OFFICEDIRECTOR_ROLE);

        WorkflowUtil.addProcessInstanceVariable(processInstance, "deptID",
                deptID);

        WorkflowUtil.completeFirstTask(workflowSession, processInstance, empId,
                "");
        applyManageDao.doSaveAppFormProcessinstanceIDInfo(form_id,
                processInstance.getId(), 0);
        String appendName = emp.getUserName() + "请假申请";
        this.doUpdateProcessName(processInstance, appendName);
        return processInstance.getId();
    }

    private boolean isInRoles(String empId, int roleId) {
        int num = applyManageDao.isInRoles(empId, roleId);
        if (num > 0) {
            return true;
        }
        return false;
    }

    public void doGoOnSubmitAskForLeave(AbsentForm absentForm, String workItemId)
            throws WorkflowException {
        applyManageDao.doSaveOrUpdateAbsentForm(absentForm);

        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();

        IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
                workflowSession, workItemId);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                true);// 再次提交被默认同意
        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(WebUtil.getCurrentEmployee().getUserID()));
        WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
                .getCurrentEmployee().getUserID() + "", "");

    }

    public void doUpdateAbsentForm(AbsentForm absentForm, String workItemId)
            throws WorkflowException {
        AbsentForm form = applyManageDao.getAbsentFormById(absentForm
                .getForm_id());
        Calendar calendar = Calendar.getInstance();
        Date nowDateTime = new java.util.Date(calendar.getTimeInMillis());
        form.setActual_begin_date(absentForm.getActual_begin_date());
        form.setActual_end_date(absentForm.getActual_end_date());
        form.setLeavetime(nowDateTime);
        form.setActual_sum(absentForm.getActual_sum());
        applyManageDao.doSaveOrUpdateAbsentForm(form);

        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();

        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(WebUtil.getCurrentEmployee().getUserID()));
        WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
                .getCurrentEmployee().getUserID() + "", "");
    }

    @SuppressWarnings("rawtypes")
    public String getAbsentFormByFormId(String form_id) {
        List list = applyManageDao.getAbsentFormByFormId(form_id);
        return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
    }

    /**
     * 两个日期间隔的天数
     *
     * @param date1结束
     * @param date2   开始
     * @return
     */
    public static long getDaysByDateTime(String date1, String date2) {
        DateFormat df = new SimpleDateFormat("yyyy-MM-dd");
        long days = 0;
        try {
            Date d1 = df.parse(date1);
            Date d2 = df.parse(date2);
            long diff = d2.getTime() - d1.getTime();
            days = diff / (1000 * 60 * 60 * 24);
        } catch (ParseException e) {
            e.printStackTrace();
        }
        return days;

    }

    /**
     * 两个日期间隔的小时数
     *
     * @param date1 结束
     * @param date2 开始
     * @return public static long getHoursByDateTime(String date1, String date2)
     * { DateFormat df = new SimpleDateFormat("yyyy-MM-dd"); long hours
     * = 0; try { Date d1 = df.parse(date1); Date d2 = df.parse(date2);
     * long diff = d1.getTime() - d2.getTime(); long days = diff / (1000
     * * 60 * 60 * 24); hours = (diff / (1000 * 60 * 60) - days * 24);
     * System.out.println(days); System.out.println(hours); } catch
     * (ParseException e) { e.printStackTrace(); } return hours;
     * <p/>
     * }
     */

    public String getAbsentDaysSum(String plan_begin_date,
                                   String plan_end_date, int plan_begin_hour, int plan_end_hour) {
        String result = "";
        int am_begin = Type.getInteger(WebKeys.AM_BEGIN);
        int am_end = Type.getInteger(WebKeys.AM_END);
        int pm_begin = Type.getInteger(WebKeys.PM_BEGIN);
        int pm_end = Type.getInteger(WebKeys.PM_END);
        int workdays = am_end - am_begin + pm_end - pm_begin;

        long diffDays = getDaysByDateTime(plan_begin_date, plan_end_date);
        if (diffDays == 0) {// 同一个日期：工作的小时数
            result = getAmHoursInSameDay(plan_begin_hour, plan_end_hour) + "小时";
        } else { // 相差多天以上：首日小时数+末日小时数+间隔的天数
            int hours = getAmHoursInDiffDay(plan_begin_hour, plan_end_hour);
            if (hours > workdays) {
                int add_days = hours / workdays;
                int add_hours = hours % workdays;
                hours = add_hours;
                diffDays = diffDays + add_days;
            }
            if (diffDays != 1) {
                result = (diffDays - 1) + "天";
            }
            if (hours != 0) {
                result += hours + "小时";
            }

        }

        return result;
    }

    public String getAbsentDaysSum(Date dateBegin, Date dateEnd) {
        SimpleDateFormat sdfDate = new SimpleDateFormat("yyyy-MM-dd");// 设置日期格式
        SimpleDateFormat sdfHour = new SimpleDateFormat("HH");// 设置日期格式

        String plan_begin_date = sdfDate.format(dateBegin);
        String plan_end_date = sdfDate.format(dateEnd);
        int plan_begin_hour = Type.getInteger(sdfHour.format(dateBegin));
        int plan_end_hour = Type.getInteger(sdfHour.format(dateEnd));

        String str = getAbsentDaysSum(plan_begin_date, plan_end_date,
                plan_begin_hour, plan_end_hour);
        return str;
    }

    public int getDaysFromStr(Date dateBegin, Date dateEnd) {
        SimpleDateFormat sdfDate = new SimpleDateFormat("yyyy-MM-dd");// 设置日期格式
        SimpleDateFormat sdfHour = new SimpleDateFormat("HH");// 设置日期格式

        String plan_begin_date = sdfDate.format(dateBegin);
        String plan_end_date = sdfDate.format(dateEnd);
        int plan_begin_hour = Type.getInteger(sdfHour.format(dateBegin));
        int plan_end_hour = Type.getInteger(sdfHour.format(dateEnd));

        String str = getAbsentDaysSum(plan_begin_date, plan_end_date,
                plan_begin_hour, plan_end_hour);

        if (str.indexOf("天") > -1) {
            return Type.getInteger(str.substring(0, str.indexOf("天")));
        } else if (str.indexOf("时") > -1) {
            return 1;
        } else {
            return 0;
        }

    }

    /**
     * 同一个日期：工作的小时数
     *
     * @param plan_begin_hour
     * @param plan_end_hour
     * @return
     */
    public static int getAmHoursInSameDay(int plan_begin_hour, int plan_end_hour) {
        int result = 0;
        int am_begin = Type.getInteger(WebKeys.AM_BEGIN);
        int am_end = Type.getInteger(WebKeys.AM_END);
        int pm_begin = Type.getInteger(WebKeys.PM_BEGIN);
        int pm_end = Type.getInteger(WebKeys.PM_END);
        int relax = pm_begin - am_end;

        if (plan_begin_hour < am_begin) {
            plan_begin_hour = am_begin;
        }

        if (plan_end_hour >= pm_end) {
            plan_end_hour = pm_end;
        }

        if (plan_begin_hour >= am_end && plan_begin_hour < pm_begin) {
            plan_begin_hour = pm_begin;
        }

        if (plan_end_hour >= am_end && plan_end_hour < pm_begin) {
            plan_end_hour = am_end;
        }

        if (plan_begin_hour >= pm_end || plan_end_hour <= am_begin) {
            return 0;
        } else if ((plan_begin_hour >= am_begin && plan_end_hour <= am_end)
                || (plan_begin_hour >= pm_begin && plan_end_hour <= pm_end)) {
            result = plan_end_hour - plan_begin_hour;
        } else if (plan_begin_hour >= am_begin && plan_begin_hour <= am_end
                && plan_end_hour >= pm_begin && plan_end_hour <= pm_end) {
            result = plan_end_hour - plan_begin_hour - relax;
        }

        return result;
    }

    public static int getAmHoursInDiffDay(int plan_begin_hour, int plan_end_hour) {
        int b_result = 0;
        int e_result = 0;

        int am_begin = Type.getInteger(WebKeys.AM_BEGIN);
        int am_end = Type.getInteger(WebKeys.AM_END);
        int pm_begin = Type.getInteger(WebKeys.PM_BEGIN);
        int pm_end = Type.getInteger(WebKeys.PM_END);

        int relax = pm_begin - am_end;

        if (plan_begin_hour < am_begin) {
            plan_begin_hour = am_begin;
        }

        if (plan_end_hour >= pm_end) {
            plan_end_hour = pm_end;
        }

        // 开始时间 到下班时间
        if (plan_begin_hour >= pm_begin) {
            b_result = pm_end - plan_begin_hour;
        } else if (plan_begin_hour >= am_end) {
            b_result = pm_end - pm_begin;
        } else if (plan_begin_hour >= am_begin) {
            b_result = pm_end - plan_begin_hour - relax;
        }

        // 上班时间 到结束时间
        if (plan_end_hour <= am_end) {
            e_result = plan_end_hour - am_begin;
        } else if (plan_end_hour <= pm_begin) {
            e_result = plan_end_hour - am_begin;
        } else if (plan_end_hour <= pm_end) {
            e_result = plan_end_hour - am_begin - relax;
        }

        return b_result + e_result;
    }

    @SuppressWarnings({"rawtypes", "unchecked"})
    public String getAbsentFormList(PageInfo pageInfo, String queryStr) {
        String ordersql = WebUtil.getOrderSqlByPageInfo(pageInfo);
        List list = applyManageDao.getAbsentFormList(queryStr, ordersql);
        for (int i = 0; i < list.size(); i++) {
            Map map = (Map) list.get(i);
            String leavetime = Type.getString(map.get("leavetime"));
            String absentDiffStr = Type.getString(map.get("plan_sum"));
            String absentDiffStr1 = Type.getString(map.get("actual_sum"));
            if (leavetime == null || "".equals(leavetime)) {
                map.put("diffstr", absentDiffStr);
            } else {
                map.put("diffstr", absentDiffStr1);
            }
        }
        List aList = getListWithPageInfo(list, pageInfo);
        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
                list.size());
    }

    @SuppressWarnings("rawtypes")
    public String getAttachmentListByAbsentFormId(String form_id) {
        List list = applyManageDao.getAttachmentListByAbsentFormId(form_id);
        return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
    }

    @SuppressWarnings("rawtypes")
    public String getDocumentTypeInfoCombo() {
        List list = applyManageDao.getDocumentTypeInfoCombo();
        return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
    }

    public String doSaveDocumentAppFormDetail(DocumentAppFormDetail applyForm) {
        String form_id = applyManageDao.doSaveDocumentAppFormDetail(applyForm);
        return form_id;
    }

    @SuppressWarnings("rawtypes")
    public String getDocumentAppFormDetailByFormId(String form_id) {
        List list = applyManageDao.getDocumentAppFormDetailByFormId(form_id);
        return WebUtil.buildStoreForJsonWithoutPageInfo(list);
    }

    public String doStartApplyDocumentAppFormDetail(String form_id, int type)
            throws WorkflowException, EngineException, KernelException {
        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
        EmployeeObj emp = WebUtil.getCurrentEmployee();
        int deptID = emp.getDepartmentID();
        // Calendar calendar = Calendar.getInstance();
        // java.util.Date nowDate = new
        // java.util.Date(calendar.getTimeInMillis());
        // String applicant_time = DateUtil.formatDateTime(nowDate);
        // // 提交后，做申请时间的修改；流程实例ID、状态的保存(用于“办公用品申领情况查询”)；
        // applyManageDao.doUpdateAppDateTimeByFormId(form_id, applicant_time);

        String empId = String.valueOf(emp.getUserID());

        IProcessInstance processInstance = WorkflowUtil.startProcess(
                workflowSession,
                WorkflowConstants.WF_APPLY_DOCUMENT_CIRCULATION, empId);
        List DutyLevel = applyManageDao.getDutyLevel(empId);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "isDeptManager",
                false);
        for (int i = 0; i < DutyLevel.size(); i++) {
            int level = Type.GetInt(((Map) DutyLevel.get(i)).get("角色ID"));
            if (level == -5) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                        true);
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isDeptManager",
                        true);
            } else if (level == -6) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                        true);
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isChargeLeader",
                        true);
            } else if (level == -17) {
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isAgree",
                        true);
                WorkflowUtil.addProcessInstanceVariable(processInstance, "isMagenagerLeader",
                        true);
            }
        }

        WorkflowUtil.addProcessInstanceVariable(processInstance, "type",
                type);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "form_id",
                form_id);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "applicant",
                empId);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "magenagerleader_role", WebKeys.APPLY_MAGENAGERLEADER_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "officemanager_role", WebKeys.DOCUMENT_DOWNER_ROLE);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "document_upper_role", WebKeys.DOCUMENT_UPPER_ROLE);

        WorkflowUtil.addProcessInstanceVariable(processInstance, "deptID",
                deptID);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "applicant_name", emp.getUserName());

        WorkflowUtil.completeFirstTask(workflowSession, processInstance, empId,
                "");
        applyManageDao.doSaveAppFormProcessinstanceIDInfo(form_id,
                processInstance.getId(), 0);
        String appendName = emp.getUserName() + "申请公文流转";
        this.doUpdateProcessName(processInstance, appendName);
        return processInstance.getId();
    }

    public String doApprovalTaskWithFormInfo(String workItemId, String isAgree,
                                             String comment, String step, String form_id) throws WorkflowException {

        if (isAgree.equals("0")) {
            int userID = WebUtil.getCurrentEmployee().getUserID();
            Calendar calendar = Calendar.getInstance();
            java.util.Date approval_datetime = new java.util.Date(
                    calendar.getTimeInMillis());
            String approvalDate = DateUtil.formatDate(approval_datetime, "yyyy-MM-dd HH:mm:ss");
            String text = "同意";
            // 保存审批人信息到业务的表单页面
            if (step.equals("ApplyDocumentCirculation.DeptManagerApproval.Task")) {
                applyManageDao.doUpdateDeptArroval(userID, text, approvalDate, form_id);
            }
            if (step.equals("ApplyDocumentCirculation.ChargeLeaderApproval.Task")) {
                applyManageDao.doUpdateSpecArroval(userID, text, approvalDate, form_id);
            }
            if (step.equals("ApplyDocumentCirculation.OfficeDirectorApproval.Task")) {
                applyManageDao.doUpdatedirecArroval(userID, text, approvalDate, form_id);
            }
        }

        return doApprovalTask(workItemId, isAgree, comment);
    }

    public String doSubmitAfterUpdateApplyDocumentAppFormDetail(
            DocumentAppFormDetail applyForm, String workItemId) throws WorkflowException {
        applyManageDao.doSaveDocumentAppFormDetail(applyForm);
        return doGoOnSubmitApplyOfficialSupplies(workItemId);
    }

    public void doInsertDocumentCirculatedInstances(
            DocumentCiculatedInstances instancesObj, String to_id_str) {
        String[] arrays = to_id_str.split(",");
        for (int i = 0; i < arrays.length; i++) {
            instancesObj.setTo_id(Type.GetInt(arrays[i]));
            applyManageDao.doInsertDocumentCirculatedInstances(instancesObj);
            String title = applyManageDao.getTitleByFormId(instancesObj.getForm_id());
            MessageInfo message = new MessageInfo();
            message.setTitle(title);
            SimpleEmployee sender = organizationDAOImpl.getSimpleEmployee(Type.GetInt(instancesObj.getFrom_id()));
            message.setSender(sender);
            SimpleEmployee receiver = organizationDAOImpl.getSimpleEmployee(Type.GetInt(instancesObj.getTo_id()));
            message.setReceiver(receiver);
            message.setContent("officialdocument/applyDocumentCirculation.jsp?&read=true&form_id=" + instancesObj.getForm_id());
            message.setSendTime(instancesObj.getFrom_datetime());
            message.setState(1);
            messageSender.doSendMessage(message);
        }

    }

    @SuppressWarnings("rawtypes")
    public String getDocumentCirculatedInstancesList(String form_id) {
        List list = applyManageDao.getDocumentCirculatedInstancesList(form_id);
        return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
    }

    @Override
    public String doStartMonthPlan(EarlyPlans earlyPlans)
            throws WorkflowException, EngineException, KernelException {
        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();
        EmployeeObj emp = WebUtil.getCurrentEmployee();
        int deptID = emp.getDepartmentID();
        String empId = String.valueOf(emp.getUserID());

        applyManageDao.doSaveOrUpdateMonthPlan(earlyPlans);
        String form_id = earlyPlans.getFormId();

        IProcessInstance processInstance = WorkflowUtil.startProcess(
                workflowSession, WorkflowConstants.WF_Monthly_Management_Work, empId);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "form_id",
                form_id);
        WorkflowUtil.addProcessInstanceVariable(processInstance, "applicantID",
                empId);
        WorkflowUtil.addProcessInstanceVariable(processInstance,
                "applicantName", emp.getUserName());
        WorkflowUtil.addProcessInstanceVariable(processInstance, "deptID",
                deptID);

        WorkflowUtil.completeFirstTask(workflowSession, processInstance, empId,
                "");
        String appendName = emp.getUserName() + "部门月初计划申请";
        this.doUpdateProcessName(processInstance, appendName);
        applyManageDao.doSaveFormIdAndProId(form_id, processInstance.getId());
        return processInstance.getId();
    }

    @Override
    public void doGoOnSubmitMonthPlan(EarlyPlans earlyPlans, String workItemId)
            throws WorkflowException {
        applyManageDao.doSaveOrUpdateMonthPlan(earlyPlans);

        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();


        IProcessInstance processInstance = WorkflowUtil.getProcessInstance(
                workflowSession, workItemId);
        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(WebUtil.getCurrentEmployee().getUserID()));
        WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
                .getCurrentEmployee().getUserID() + "", "");
    }

    @Override
    public String getMonthPlanByFormId(String form_id) {
        List list = applyManageDao.getMonthPlanByFormId(form_id);
        return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
    }

    @Override
    public void doUpdateMonthPlanForm(TheEndOfThePlanned endOfThePlanned,
                                      String workItemId) throws WorkflowException {
        EmployeeObj emp = WebUtil.getCurrentEmployee();
        int deptID = emp.getDepartmentID();
        String empId = String.valueOf(emp.getUserID());
        applyManageDao.doSaveOrUpdateEndOfMonthPlan(endOfThePlanned);

        IWorkflowSession workflowSession = workflowContext.getWorkflowSession();

        // 自动签收
        workflowService.doClaimTaskList(workItemId,
                Type.getString(emp.getUserID()));
        WorkflowUtil.completeTask(workflowSession, workItemId, WebUtil
                .getCurrentEmployee().getUserID() + "", "");
    }

    @Override
    public void doUpdateMonthPlanComments(EarlyPlans earlyPlans) {
        // TODO Auto-generated method stub
        applyManageDao.doUpdateMonthPlanComments(earlyPlans);
    }

    @Override
    public void doUpdateMonthPlanComments(TheEndOfThePlanned endOfThePlanned) {
        // TODO Auto-generated method stub
        applyManageDao.doUpdateMonthPlanComments(endOfThePlanned);
    }

    @Override
    public String getMonthPlanList(PageInfo pageInfo, String department,
                                   String workMonth) {

        if (department == null && workMonth == null) {


            List list = applyManageDao.getMonthPlanList(pageInfo);
            List alist = getListWithPageInfo(list, pageInfo);

            return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist, list.size());
        } else if (Type.GetInt(department) == -1 || "".equals(department)) {
            if (workMonth == null || "".equals(workMonth)) {
                List list = applyManageDao.getMonthPlanList(pageInfo);
                List alist = getListWithPageInfo(list, pageInfo);

                return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist, list.size());
            } else {
                List list = applyManageDao.getMonthPlanListByDate(pageInfo, workMonth);
                List alist = getListWithPageInfo(list, pageInfo);
                return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist, list.size());
            }

        } else {
            if (workMonth == null || "".equals(workMonth)) {
                List list = applyManageDao.getMonthPlanListByPart(pageInfo, department);
                List alist = getListWithPageInfo(list, pageInfo);
                return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist, list.size());
            } else {
                List list = applyManageDao.getMonthPlanListByPartAndDate(pageInfo, department, workMonth);
                List alist = getListWithPageInfo(list, pageInfo);
                return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist, list.size());
            }
        }

    }


    public int doSaveGczlInfo(GczlInfo info) {
        return applyManageDao.doSaveGczlInfo(info);
    }

    public void doDeleteGczlInfoById(int tab_seed) {
        applyManageDao.doDeleteGczlInfoById(tab_seed);
    }

    public String getGczlInfoListWithQuery(String b_date,
                                           String e_date) {
        List list = applyManageDao.getGczlInfoListWithQuery(b_date, e_date
        );

        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(list,
                list.size());
    }

    public String getCurrentAndNextAbsentInfo() {
        List list = applyManageDao.getCurrentAndNextAbsentInfo();
        return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
    }

    @Override
    public String getAbsentList() {
        List list = applyManageDao.getAbsentList();
        return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
    }

    @Override
    public String getAskForLeaveList(PageInfo pageInfo, String hrName, String date) {
        String ordersql = WebUtil.getOrderSqlByPageInfo(pageInfo);
        List list = applyManageDao.getAskForLeaveList(hrName, ordersql, date);
        List aList = getListWithPageInfo(list, pageInfo);
        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
                list.size());
    }

    public String getColumnNameCombo() {
        List list = applyManageDao.getColumnNameCombo();
        return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
    }

    public String getCeYaColumnNameCombo() {
        List list = applyManageDao.getCeYaColumnNameCombo();
        return WebUtil.buildStoreForJsonWithoutPageInfoWithDateFormat(list);
    }

    @Override
    public String getGczlInfoListComparison(String b_year, String e_year,
                                            String month) {
        List list = applyManageDao.getGczlInfoListComparison(b_year, e_year, month
        );

        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(list,
                list.size());
    }

    @Override
    public String getGczlInfoExcretion(String b_date, String e_date, PageInfo pageInfo) {

        List list = applyManageDao.getGczlInfoExcretion(b_date, e_date
        );
        List aList = getListWithPageInfo(list, pageInfo);
        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,
                list.size());
    }

    @Override
    public String getExcretionSum(String b_year, String e_year) {
        List list = applyManageDao.getExcretionSum(b_year,
                e_year);
        String sum = ((Map) list.get(0)).get("sum").toString();
        return sum;
    }

    @Override
    public String getGczlInfoListForFlood(String year, String flood) {
        List list = applyManageDao.getGczlInfoListForFlood(year, flood
        );

        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(list,
                list.size());
    }

    @Override
    public String getGczlInfoListForFloodGates(String b_date, String e_date,
                                               String floodGates) {
        List list = applyManageDao.getGczlInfoListForFloodGates(b_date, e_date, floodGates
        );

        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(list,
                list.size());
    }

    @Override
    public String getGczlInfoEveryMonthLevel(String b_date, String e_date,
                                             String type) {
        List list = applyManageDao.getGczlInfoEveryMonthLevel(b_date, e_date, type
        );

        return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(list,
                list.size());
    }

    @Override
    public String getMptpCodeList() {
        List list = applyManageDao.getMptpCodeList();
        return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
    }

    @Override
    public String getMptCodeList(String mptp_code) {
        List list = applyManageDao.getMptCodeList(mptp_code);
        return WebUtil.buildStoreForJsonWithoutPageInfo(list, "");
    }

    @Override
    public int doSavecyInfo(CeyaInfo info) {
        return applyManageDao.doSavecyInfo(info);
    }

    @Override
    public void doDeletecyInfoById(int tab_seed) {
        applyManageDao.doDeletecyInfoById(tab_seed);

    }

    public String getCyInfoListWithGroupQuery(PageInfo pageInfo,
                                              String queryStr, StandardQuery query) {
        List list = applyManageDao.getCyInfoListWithGroupQuery(
                queryStr, pageInfo, query);
        return WebUtil.buildStoreForJson(list, "");
    }

    @Override
    public BigDecimal getTubo_high_degree(String mptp_code, String mpt_name) {
        // TODO Auto-generated method stub
        return applyManageDao.getTubo_high_degree(mptp_code, mpt_name);
    }

    @Override
    public String getFileFlagList() {
        return applyManageDao.getFileFlagList().toJsonString();
    }

    @Override
    public int getSign_No(String year) {
        // TODO Auto-generated method stub
        return applyManageDao.getSign_No(year);
    }

    @Override
    public void dosendHuiQianMessage(String indexStr, String formId, String type) {
        applyManageDao.dosendHuiQianMessage(indexStr, formId, type);
    }

    @Override
    public void doupdateHuiQianMessage(String isAgree, String formId, String type) {
        applyManageDao.doupdateHuiQianMessage(isAgree, formId, type);
    }

    @Override
    public String getHQ_nameByFormId(String formId, String type) {
        return applyManageDao.getHQ_nameByFormId(formId, type);
    }

    @Override
    public String getDoDayReportData(String datetime) throws IOException {
        Map<String, String> data = applyManageDao.getDoDayReportData(datetime);
        return JacksonUtil.getJsonString(data);
    }

    @Override
    public void saveDoDayReportData(List<Map<String, String>> list) throws Exception {
        Map<String, String> map = null;
        for (int i = 0; i < list.size(); i++) {
            map = list.get(i);
            applyManageDao.saveDoDayReportData(map);
        }
    }

    @Override
    public String queryDoDayReportData(String startDate, String endDate) throws IOException {
        List<Map<String, String>> list = applyManageDao.queryDoDayReportData(startDate, endDate);
        return JacksonUtil.getJsonString(list);
    }

    @Override
    public String queryzmDayReportData(String currentDate) throws IOException, ParseException {
        Map map = applyManageDao.queryzmDayReportData(currentDate);
        return JacksonUtil.getJsonString(map);
    }

    @Override
    public void exportzmDayReportData(HttpServletResponse response, String currentDate) throws Exception {
		Map map = applyManageDao.queryzmDayReportData(currentDate);
        Map llMap = (Map) map.get("liuliang");
        Map swMap = (Map) map.get("shuiwei");
        Map kdMap = (Map) map.get("kaidu");
        HSSFWorkbook workbook = new HSSFWorkbook();
        HSSFSheet sheet = workbook.createSheet(currentDate + "_日报表");
        sheet.addMergedRegion(new CellRangeAddress(0, 0, 0, 25));
        sheet.addMergedRegion(new CellRangeAddress(1, 1, 0, 1));
        sheet.addMergedRegion(new CellRangeAddress(2, 2, 0, 1));
        sheet.addMergedRegion(new CellRangeAddress(3, 3, 0, 1));
        sheet.addMergedRegion(new CellRangeAddress(4, 4, 0, 1));
        sheet.addMergedRegion(new CellRangeAddress(5, 16, 0, 0));
        sheet.addMergedRegion(new CellRangeAddress(17, 44, 0, 0));
        HSSFCellStyle style = workbook.createCellStyle(); // 样式对象
        style.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);// 垂直
        style.setAlignment(HSSFCellStyle.ALIGN_CENTER);// 水平
        HSSFCellStyle titleStyle = workbook.createCellStyle();
        titleStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);// 垂直
        titleStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);// 水平
        HSSFRow head = sheet.createRow(0);
        HSSFCell cell = head.createCell(0);
        HSSFFont font = workbook.createFont();
        font.setFontHeightInPoints((short) 36);
        font.setBoldweight(HSSFFont.BOLDWEIGHT_BOLD);
        font.setFontName("华文新魏");
        titleStyle.setFont(font);
        cell.setCellStyle(titleStyle);
        cell.setCellValue("蚌埠闸闸门控制系统日报表");
        HSSFCellStyle headerStyle = workbook.createCellStyle();
        headerStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);// 垂直
        headerStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);// 水平
        font = workbook.createFont();
        font.setFontName("宋体");
        font.setFontHeightInPoints((short) 11);
        font.setBoldweight(HSSFFont.BOLDWEIGHT_BOLD);
        headerStyle.setFont(font);
        String[] names = {"ling","yi", "er", "san", "si", "wu", "liu", "qi", "ba", "jiu", "shi",
                "shiyi", "shier", "shisan", "shisi", "shiwu", "shiliu", "shiqi", "shiba", "shijiu", "ershi",
                "eryi", "erer", "ersan"};
        HSSFRow row = sheet.createRow(1);
        for (int i = 0; i < 24; i++) {
            cell = row.createCell(i + 2);
            cell.setCellStyle(headerStyle);
            cell.setCellValue(i + "时");
        }
        HSSFRow ll = sheet.createRow(2);
        HSSFRow shang_shuiwei = sheet.createRow(3);
        HSSFRow xia_shuiwei = sheet.createRow(4);
        cell = ll.createCell(0);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("流量");
        for(int i=0;i<24;i++){
            cell = ll.createCell(i+2);
//            cell.setCellStyle(headerStyle);
            cell.setCellValue(llMap.get(names[i])==null?"":String.valueOf(llMap.get(names[i])));
        }
        cell = shang_shuiwei.createCell(0);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("上游水位");
        for(int i=0;i<24;i++){
            cell = shang_shuiwei.createCell(i+2);
//            cell.setCellStyle(headerStyle);
            cell.setCellValue(swMap.get("shangyou_"+names[i])==null?"":String.valueOf(swMap.get("shangyou_"+names[i])));
        }
        cell = xia_shuiwei.createCell(0);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("下游水位");
        for(int i=0;i<24;i++){
            cell = xia_shuiwei.createCell(i+2);
//            cell.setCellStyle(headerStyle);
            cell.setCellValue(swMap.get("xiayou_"+names[i])==null?"":String.valueOf(swMap.get("xiayou_"+names[i])));
        }

        HSSFRow zhamen = null;
        for(int i=1;i<=12;i++){
            zhamen = sheet.createRow(i+4);
            if(i==1){
                cell = zhamen.createCell(0);
                cell.setCellStyle(headerStyle);
                cell.setCellValue("新闸");
            }
            cell = zhamen.createCell(1);
            cell.setCellStyle(headerStyle);
            cell.setCellValue(i+"#孔");
            for(int j=0;j<24;j++){
                cell = zhamen.createCell(j+2);
//                cell.setCellStyle(headerStyle);
                cell.setCellValue(kdMap.get("new_"+i+"_"+names[j])==null?"":String.valueOf(kdMap.get("new_"+i+"_"+names[j])));
            }
        }
        for(int i=1;i<=28;i++){
            zhamen = sheet.createRow(i+16);
            if(i==1){
                cell = zhamen.createCell(0);
                cell.setCellStyle(headerStyle);
                cell.setCellValue("老闸");
            }
            cell = zhamen.createCell(1);
            cell.setCellStyle(headerStyle);
            cell.setCellValue(i+"#孔");
            for(int j=0;j<24;j++){
                cell = zhamen.createCell(j+2);
//                cell.setCellStyle(headerStyle);
                cell.setCellValue(kdMap.get("old_"+i+"_"+names[j])==null?"":String.valueOf(kdMap.get("old_"+i+"_"+names[j])));
            }
        }
        response.reset();
        response.setHeader("Cache-Control", "no-cache");
        response.setHeader("Pragma", "no-cache");
        response.setDateHeader("Expires", -1);
        response.setContentType("application/vnd.ms-excel");
        response.setHeader("Content-Disposition", "attachment;filename=" + URLEncoder.encode("蚌埠闸闸门控制系统日报表.xls", "UTF-8"));
        OutputStream out = response.getOutputStream();
        workbook.write(out);
        out.close();
        workbook.close();
    }

    @Override
    public void exportdoDayReportData(HttpServletResponse response, String startDate, String endDate) throws Exception {
        List<Map<String, String>> list = applyManageDao.queryDoDayReportData(startDate, endDate);
        HSSFWorkbook workbook = new HSSFWorkbook();
        HSSFSheet sheet = workbook.createSheet("闸门控制系统报表");
        sheet.addMergedRegion(new CellRangeAddress(0, 0, 0, 10));
        sheet.addMergedRegion(new CellRangeAddress(1, 2, 0, 0));
        sheet.addMergedRegion(new CellRangeAddress(1, 2, 1, 1));
        sheet.addMergedRegion(new CellRangeAddress(1, 1, 2, 4));
        sheet.addMergedRegion(new CellRangeAddress(1, 1, 5, 7));
        sheet.addMergedRegion(new CellRangeAddress(1, 2, 8, 8));
        sheet.addMergedRegion(new CellRangeAddress(1, 2, 9, 9));
        sheet.addMergedRegion(new CellRangeAddress(1, 2, 10, 10));

        HSSFCellStyle style = workbook.createCellStyle(); // 样式对象
        style.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);// 垂直
        style.setAlignment(HSSFCellStyle.ALIGN_CENTER);// 水平
        HSSFCellStyle titleStyle = workbook.createCellStyle();
        titleStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);// 垂直
        titleStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);// 水平
        HSSFRow head = sheet.createRow(0);
        HSSFCell cell = head.createCell(0);
        HSSFFont font = workbook.createFont();
        font.setFontHeightInPoints((short) 36);
        font.setBoldweight(HSSFFont.BOLDWEIGHT_BOLD);
        font.setFontName("华文新魏");
        titleStyle.setFont(font);
        cell.setCellStyle(titleStyle);
        cell.setCellValue("蚌埠闸闸门控制系统报表");
        HSSFCellStyle headerStyle = workbook.createCellStyle();
        headerStyle.setVerticalAlignment(HSSFCellStyle.VERTICAL_CENTER);// 垂直
        headerStyle.setAlignment(HSSFCellStyle.ALIGN_CENTER);// 水平
        font = workbook.createFont();
        font.setFontName("宋体");
        font.setFontHeightInPoints((short) 11);
        font.setBoldweight(HSSFFont.BOLDWEIGHT_BOLD);
        headerStyle.setFont(font);
        HSSFRow row = sheet.createRow(1);
        cell = row.createCell(0);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("调度指令时间");
        cell = row.createCell(1);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("调度员");
        cell = row.createCell(2);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("新闸");
        cell = row.createCell(5);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("老闸");
        cell = row.createCell(8);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("上游水位");
        cell = row.createCell(9);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("下游水位");
        cell = row.createCell(10);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("流量");
        row = sheet.createRow(2);
        cell = row.createCell(2);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("开启孔数");
        cell = row.createCell(3);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("开度");
        cell = row.createCell(4);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("开启孔号");
        cell = row.createCell(5);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("开启孔数");
        cell = row.createCell(6);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("开度");
        cell = row.createCell(7);
        cell.setCellStyle(headerStyle);
        cell.setCellValue("开启孔号");
        Map<String,String> map = null;
        String[] names={"doTime","doName","new_kongshu","new_kaidu","new_konghao","old_kongshu","old_kaidu","old_konghao","shang_shuiwei","xia_shuiwei","liuliang"};
        for (int i = 0; i < list.size(); i++) {
            row = sheet.createRow(i + 3);
            map = list.get(i);
            for(int j=0;j<names.length;j++){
                cell = row.createCell(j);
                cell.setCellValue(map.get(names[j])==null?"":String.valueOf(map.get(names[j])));
            }
        }
        response.reset();
        response.setHeader("Cache-Control", "no-cache");
        response.setHeader("Pragma", "no-cache");
        response.setDateHeader("Expires", -1);
        response.setContentType("application/vnd.ms-excel");
        response.setHeader("Content-Disposition", "attachment;filename=" + URLEncoder.encode("蚌埠闸闸门控制系统报表.xls", "UTF-8"));
        OutputStream out = response.getOutputStream();
        workbook.write(out);
        out.close();
        workbook.close();
    }

    @Override
    public String getTopReportData(String startDate,String endDate) throws Exception {
        Map<String, String> map = applyManageDao.getTopReportData(startDate,endDate);
        return JacksonUtil.getJsonString(map);
    }

    @Override
    public String doCreateDdWord(String queryDate) throws Exception {
        String[] query = queryDate.split(",");
        List<Map<String,String>> list = new ArrayList();
        list.add(applyManageDao.getDoDayReportData(DateUtil.formatDate(new Date(), "yyyy-MM-dd")+" 08:00:00"));
        Map<String,String> map = null;
        for (int i=0;i<query.length;i++) {
            if(query[i]!= null && !"".equals(query[i])){
                map = applyManageDao.getDoDayReportData(query[i]);
                map.put("sum_ll",applyManageDao.doCreateDdWord(query[i]));
                map.put("datetime",query[i]);
                list.add(map);
            }
        }
        return JacksonUtil.getJsonString(list);
    }

    @Override
    public void deleteDoDayReportData(String queryDate, String queryPerson) {
        applyManageDao.deleteDoDayReportData(queryDate,queryPerson);
    }
}
