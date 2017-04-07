<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="java.util.*" %>
<%@ page import="java.sql.Timestamp" %>
<%@ page import="sun.jdbc.rowset.CachedRowSet" %>
<%@ page import="flowengineforjava.structure.WFConstants" %>
<%@ page import="flowengineforjava.structure.PagingData" %>
<%@ page import="flowengineweb.*" %>
<%@ page import="com.minstone.util.PublicLib" %>
<%@ page import="flowengineweb.helper.WorkFlowHelper" %>
<%
    long startTime = System.currentTimeMillis();
//取得路径变量
    String sCommonPath = "", sImagesPath = "", sPublicPath = "";
    sCommonPath = SysPara.getCommonPath(request);
    sImagesPath = SysPara.getImagesPath(request);
    sPublicPath = SysPara.getPublicPath(request);
    String sResPublic = SysPara.getResPublicPath(request);
//测试化工作流辅助类。
    WorkFlowHelper wfHelper = new WorkFlowHelper();
    wfHelper.initialize(request, response, pageContext);
//设置不使用缓存。
    wfHelper.setNoCache();
%>
<%
    CachedRowSet taskRs = null;
    PagingData pagingData = null;
    long afterInit, afterPagingData, afterTasksEx=0L;
    try {
        //初始化
        FlowWebBean fwb = new FlowWebBean(wfHelper.getCurrentUser());
        boolean bBatchMode = false;
        boolean bBatchRead = false;
        boolean bBatchSend = false;
        boolean bScoreMode = false;
        boolean bCanCopy = false;
        boolean bCanNotice = false;
        boolean bCanRedirect = false;
        boolean bCanDelete = false;
        boolean bNeedShowOverTime = false;
        boolean bCanReminder = false;
        boolean bShowOnlyMe = false;
        boolean bIntegrationRtx = wfHelper.getSysParaValue("WF/SUPPORT_RTX").equals("1");//是否集成RTX
        String sBatchMode = wfHelper.readParameter("batchMode", "");
        String sBatchSend = wfHelper.readParameter("batchSend", "");//不修改任何内容,直接发送所有选中待办.
        String sBatchRead = wfHelper.readParameter("batchRead", "");
        String sScoreMode = wfHelper.readParameter("scoreMode", "");
        String sCanCopy = wfHelper.readParameter("canCopy", "");
        String sReadOnly = wfHelper.readParameter("readOnly", "");
        String sCanNotice = wfHelper.readParameter("canNotice", "");
        String sCanRedirect = wfHelper.readParameter("canRedirect", "");
        String sCanDelete = wfHelper.readParameter("canDelete", "");
        String sCanReminder = wfHelper.readParameter("canReminder", "");
        String sShowOnlyMe = wfHelper.readParameter("showOnlyMe", "0");//只显示本人经办过的记录,缺省是显示所有，该参数实际上对管理员和部门管理员才有效。。
        int iDisRow = wfHelper.readParameterInt("displayRow", -1);

        bCanDelete = sCanDelete.trim().equalsIgnoreCase("1");
        bBatchMode = sBatchMode.trim().equals("1");
        bBatchRead = sBatchRead.trim().equals("1");
        bBatchSend = sBatchSend.trim().equals("1");
        bScoreMode = sScoreMode.equals("1");
        bCanCopy = sCanCopy.equals("1");
        bCanNotice = sCanNotice.trim().equals("1");
        bCanRedirect = sCanRedirect.trim().equals("1");
        bCanReminder = sCanReminder.trim().equals("1");
        bShowOnlyMe = sShowOnlyMe.trim().equals("1");

        boolean isAdmin = wfHelper.isAdmin() && !bShowOnlyMe;//如果是bShowOnlyMe为真的话，该用户就作为普通用户使用。
        boolean isDeptAdmin = fwb.isDeptAdmin() && !bShowOnlyMe;//如果是bShowOnlyMe为真的话，该用户就作为普通用户使用。
        String taskType = wfHelper.readParameter("taskType", "-1");//全部
        long readFlag = wfHelper.readParameterInt("readFlag", -1);//全部
        String flowDir = wfHelper.readParameter("flowDir", "");
        boolean includeSubFlowDir = wfHelper.readParameter("includeSubFlowDir", "").trim().equals("1");
        String creator = wfHelper.readParameter("c_creator", "");
        String beginDateLower = wfHelper.readParameter("c_beginDateLower", "");
        String beginDateHigher = wfHelper.readParameter("c_beginDateHigher", "");
        String endDateLower = wfHelper.readParameter("c_endDateLower", "");
        String endDateHigher = wfHelper.readParameter("c_endDateHigher", "");
        String flowName = wfHelper.readParameter("c_flowName", "");
        String instName = wfHelper.readParameter("c_instName", "");
        String sender = wfHelper.readParameter("c_sender", "");
        String flowFolder = wfHelper.readParameter("c_flowFolder", "");
        String docSeq = wfHelper.readParameter("c_docSeq", "");
        String fieldInfo = wfHelper.readParameter("c_field", "");
        String AwokeFlag = wfHelper.readParameter("hiddenAwokeFlag", "down");
        String FinsNameFlag = wfHelper.readParameter("hiddenFinsNameFlag", "down");
        String FlowNameFlag = wfHelper.readParameter("hiddenFlowNameFlag", "down");
        String StepNameFlag = wfHelper.readParameter("hiddenStepNameFlag", "down");
        String PeriNameFlag = wfHelper.readParameter("hiddenPeriNameFlag", "down");
        String ReceDateFlag = wfHelper.readParameter("hiddenReceDateFlag", "down");
        String DocSeqFlag = wfHelper.readParameter("hiddenDocSeqFlag", "down");
        String OrderFlag = wfHelper.readParameter("hiddenOrder", "");
        String bookmark = wfHelper.readParameter("bookmark", "");
        String orderFieldParaName = "TaskList_" + bookmark + "_OrderField";
        String orderTypeParaName = "TaskList_" + bookmark + "_OrderType";
        String orderType = "";
        if (OrderFlag.trim().length() == 0) {
            try {
                OrderFlag = wfHelper.getBaseServerClientJB().getPersonalParameter(orderFieldParaName, "", false);
                orderType = wfHelper.getBaseServerClientJB().getPersonalParameter(orderTypeParaName, "", false);
            } catch (Exception ex) {
                ex.printStackTrace();
            }
        } else {
            orderType = wfHelper.readParameter(OrderFlag, "down");
        }
        if (OrderFlag.trim().length() > 0) {
            wfHelper.getBaseServerClientJB().setPersonalParameter(orderFieldParaName, OrderFlag);
            wfHelper.getBaseServerClientJB().setPersonalParameter(orderTypeParaName, orderType);
        }
        taskType = taskType.trim();
        bNeedShowOverTime = (taskType.equals("0")) || (taskType.equals("3") && readFlag == 0);
        //关于个人归类
        String sClassID = "";
        sClassID = request.getParameter("sClassID");
        if (sClassID == null || sClassID.equals(""))
            sClassID = "";
        //关于排序
        String sOrder = "";
        if (OrderFlag.equals("hiddenAwokeFlag")) {
            sOrder = wfHelper.genOrderByField(orderType, "A.sys_urge");
        } else if (OrderFlag.equals("hiddenFinsNameFlag")) {
            sOrder = wfHelper.genOrderByField(orderType, "A.fins_name");
        } else if (OrderFlag.equals("hiddenFlowNameFlag")) {
            sOrder = wfHelper.genOrderByField(orderType, "T.item_name");
        } else if (OrderFlag.equals("hiddenStepNameFlag")) {
            if ((isAdmin || isDeptAdmin) && taskType.equals("-1")) {
                sOrder = wfHelper.genOrderByField(orderType, "B.step_name");
            } else {
                sOrder = wfHelper.genOrderByField(orderType, "B2.step_name");
            }
        } else if (OrderFlag.equals("hiddenPeriNameFlag")) {
            sOrder = wfHelper.genOrderByField(orderType, "D.full_name");
        } else if (OrderFlag.equals("hiddenReceDateFlag")) {
            if (!((isAdmin || isDeptAdmin) && taskType.equals("-1"))) {
                sOrder = wfHelper.genOrderByField(orderType, "C.rece_date");
            } else {
                sOrder = wfHelper.genOrderByField(orderType, "B.crea_date");
            }
        } else if (OrderFlag.equals("hiddenDocSeqFlag")) {
            sOrder = wfHelper.genOrderByField(orderType, "A.doc_seq");
        } else {
            if (taskType.equals("0") || (taskType.equals("3") && readFlag == 0) || (taskType.equals("4") && readFlag == 0))
                sOrder = "A.sys_urge desc,";
            if ((isAdmin || isDeptAdmin) && taskType.equals("-1"))
                sOrder += "B.crea_date desc";
            else
                sOrder += "C.rece_date desc";
        }

        afterInit = System.currentTimeMillis();
        String condition = "";
        String urlPara = "&taskType=" + taskType + "&readFlag=" + readFlag;
        String urlPara1 = "?taskType=" + taskType + "&readFlag=" + readFlag + "&sClassID=" + sClassID;
        int pageNo = wfHelper.readParameterInt("pageNo", 1);//第一页
        boolean bWordBreak = wfHelper.isWordBreak();
        int pageSize = wfHelper.getPageSize(iDisRow);
        //=============================================
        condition = fwb.genCFieldCondition(fieldInfo, flowDir, includeSubFlowDir);
        //=============================================
        boolean doAsAdmin = (isAdmin || isDeptAdmin) && taskType.equals("-1");
        if (doAsAdmin) {
            if (isAdmin) {
                pagingData = wfHelper.getWorkflowHelper().getAdminTaskCount(condition, taskType, readFlag, creator, beginDateLower, beginDateHigher, endDateLower, endDateHigher, flowName, instName, flowFolder, docSeq, sOrder, sClassID, pageNo, pageSize);
            } else if (isDeptAdmin) {
                pagingData = wfHelper.getWorkflowHelper().getDeptAdminTaskCount(condition, taskType, readFlag, creator, beginDateLower, beginDateHigher, endDateLower, endDateHigher, flowName, instName, flowFolder, docSeq, sOrder, sClassID, pageNo, pageSize);
            }
        } else {
            pagingData = wfHelper.getWorkflowHelper().getTaskCount(condition, taskType, readFlag, creator, beginDateLower, beginDateHigher, endDateLower, endDateHigher, flowName, instName, sender, flowFolder, docSeq, sOrder, sClassID, pageNo, pageSize);
        }
        afterPagingData = System.currentTimeMillis();

        long recordCount = pagingData.getTotalRow();
        int count = 0;
        StringBuffer sListHtml = new StringBuffer(30000);
        String para = "";
        int i = 0;
        int iColSpan = 9;
        if (!taskType.equals("-1")) {
            iColSpan = 10;
        }
        //关于附件
        String sAttachList = "";
        //如果没有记录不处理。
        if (recordCount > 0) {
            String flowInids = "";
            while (pagingData.getPagingData().next() && count < pageSize) {
                if (!flowInids.equals(""))
                    flowInids += ",";
                flowInids += "'" + pagingData.getPagingData().getString("flow_inid") + "'";
                count++;
            }
            if (flowInids.equals(""))
                condition = "1=2";
            else {
                condition = "A.flow_inid in (" + flowInids + ")";
            }

            if (doAsAdmin)
                taskRs = wfHelper.getWorkflowHelper().getAdminTasksEx(condition, taskType, readFlag, creator, beginDateLower, beginDateHigher, endDateLower, endDateHigher, flowName, instName, sender, flowFolder, docSeq, sOrder, sClassID);
            else
                taskRs = wfHelper.getWorkflowHelper().getTasksEx(condition, taskType, readFlag, creator, beginDateLower, beginDateHigher, endDateLower, endDateHigher, flowName, instName, sender, flowFolder, docSeq, sOrder, sClassID);

            afterTasksEx = System.currentTimeMillis();

            if (!flowInids.equals(""))
                sAttachList = wfHelper.getWorkflowHelper().getAttachFlow(flowInids);
            String[] sArrAttachList = null;
            if (sAttachList != null && !sAttachList.equals("")) {
                sArrAttachList = PublicLib.split(sAttachList, ";");
            }

            String flowInstList = "";
            //long bTime=System.currentTimeMillis();
            boolean displayTrace = isAdmin;
            while (taskRs.next()) {
                String userName = "";
                String userTrueName = "";
                String sUserNameLink = "";
                String flowInid = taskRs.getString("flow_inid");
                String stepCode = taskRs.getString("step_code");
                String flowId = taskRs.getString("flow_id");
                String stepInid = taskRs.getString("step_inco");
                if (!isAdmin) {
                    MiscStruct ms = fwb.getStepDefMiscStruct(flowId, stepInid);
                    displayTrace = ms.getTraceButton() == 1;
                }
                String flowInst = ";" + flowInid + ";";
                if (flowInstList.indexOf(flowInst) != -1)
                    continue;
                flowInstList += flowInst;
                //关于紧急公文
                String sUrgeImagesName = sImagesPath + "Blank.gif";
                int iUrgeFile = taskRs.getInt("sys_urge");
                if (iUrgeFile > 0)
                    sUrgeImagesName = sImagesPath + "UrgeFile.gif";
                //关于附件
                String sAttachImagesName = sImagesPath + "Blank.gif";
                //是否有附件
                boolean hasAttach = false;
                if (sArrAttachList != null && sArrAttachList.length > 0) {
                    for (int m = 0; m < sArrAttachList.length; m++) {
                        if (sArrAttachList[m].equals(flowInid)) {
                            sAttachImagesName = sImagesPath + "Attach.gif";
                            hasAttach = true;
                            break;
                        }
                    }
                }
                //关于知会
                String sNoticeImagesName = sImagesPath + "Blank.gif";
                String sStepType = taskRs.getString("step_type");
                //是否知会
                boolean isNotice = false;
                if (sStepType.equals("3")) {
                    isNotice = true;
                    sNoticeImagesName = sImagesPath + "Notice.gif";
                }
                String docSeqField = taskRs.getString("doc_seq");
                if (docSeqField == null)
                    docSeqField = "";
                String s_flowName = taskRs.getString("flow_name");
                String dealIndx = taskRs.getString("deal_indx");
                if (s_flowName == null)
                    s_flowName = "&nbsp;";
                String ifReload = "0";
                if (taskType.equals("0") || taskType.equals("2") || (taskType.equals("3") && readFlag == 0) || (taskType.equals("4") && readFlag == 0))
                    ifReload = "1";
                para = "action=0&flowInid=" + flowInid + "&stepInid=" + stepInid + "&dealIndx=" + dealIndx + "&ifReload=" + ifReload + "&readOnly=" + sReadOnly;

                String stepName = taskRs.getString("step_name");
                if (stepName == null)
                    stepName = "&nbsp;";
                String s_instName = taskRs.getString("fins_name");
                if ((s_instName == null) || (s_instName.equals("")))
                    s_instName = "<空>";
                String receDate = "";
                Timestamp receDateTime = taskRs.getTimestamp("rece_date");
                double dealDelay = taskRs.getDouble("deal_delay");
                double totalDelay = taskRs.getDouble("total_delay");
                boolean isDealOverTime = totalDelay < 0 || totalDelay < 0;
                isDealOverTime = isDealOverTime && bNeedShowOverTime;
                if ((isAdmin || isDeptAdmin) && taskType.equals("-1")) {//管理员取步骤实例创建时间
                    receDate = taskRs.getString("crea_date");
                } else {//普通用户取经办记录接受时间
                    receDate = receDateTime == null ? "" : receDateTime.toString();
                }
                if (receDate != null && !receDate.equals("")) {
                    int pos = receDate.lastIndexOf(":");
                    receDate = receDate.substring(0, pos);
                } else {
                    receDate = "";
                }

                //关于个人信息
                userName = taskRs.getString("peri_man");
                if (userName != null && !userName.equals("")) {
                    userTrueName = taskRs.getString("peri_name");
                    if (userTrueName == null || userTrueName.equals(""))
                        userTrueName = userName;
                    String rtxTag = "";
                    if (bIntegrationRtx)
                        rtxTag = "<img align=absbottom width=16 height=16 src=\"../rtx/images/blank.gif\" onload=\"javascript:RAP('" + userName + "');\" showOffline>";
                    sUserNameLink = rtxTag + "<span onclick=\"javascript:UserInfo('" + userName + "')\">" + userTrueName + "</span>";
                }
                String dealMan = taskRs.getString("deal_man");
                if (dealMan == null) dealMan = "";
                String href = "";
                if (SysPara.FlowFrame_OpenMode == 0)
                    href = "onclick=\"javascript:HrefLink('0','../FlowFrame.jsp?" + para + "');\"";
                else if (SysPara.FlowFrame_OpenMode == 1)
                    href = "onclick=\"javascript:HrefLink('1','../FlowFrame.jsp?" + para + "');\"";
                String tail = "";
                //如果需要显示流程痕迹，才生产显示痕迹的连接。
                if (displayTrace) {
                    if (SysPara.FlowTrace_OpenMode == 0) {
                        if (SysPara.FlowTrace_ShowMode == 0)
                            tail = "onclick=\"javascript:HrefLink('0','../FlowTrace2.jsp?flowInid=" + flowInid + "');\"";
                        else if (SysPara.FlowTrace_ShowMode == 1)
                            tail = "onclick=\"javascript:HrefLink('0','../FlowTrace2.jsp?flowInid=" + flowInid + "');\"";
                    } else if (SysPara.FlowTrace_OpenMode == 1) {
                        if (SysPara.FlowTrace_ShowMode == 0)
                            tail = "onclick=\"javascript:HrefLink('1','../FlowTrace2.jsp?flowInid=" + flowInid + "');\"";
                        else if (SysPara.FlowTrace_ShowMode == 1)
                            tail = "onclick=\"javascript:HrefLink('1','../FlowTrace2.jsp?flowInid=" + flowInid + "');\"";
                    }
                }
                String checkboxValue = flowInid + "," + stepInid + "," + dealMan + "," + dealIndx;
                //System.err.println("checkboxValue="+checkboxValue);
                int j = i + 1;
                String sDataTdChar = bWordBreak ? "data_Td_char" : "data_Td_char_No_Break";
                String sFlowInstNameTd = bWordBreak ? (isDealOverTime ? "data_Td_char_OverTime" : "data_Td_char") : (isDealOverTime ? "data_Td_char_No_Break_OverTime" : "data_Td_char_No_Break");
                String sTempClassName = (i % 2 == 0) ? "data_Tr1" : "data_Tr2";
                sListHtml.append("<tr index='" + j + "' class='" + sTempClassName + "' onclick='javascript:listTrClick(this);'>");
                sListHtml.append("<td width='25' flag='hiddenCheckboxTd' class='hiddenCheckboxTd'><input type='checkbox' id='hiddenCheckbox' name='hiddenCheckbox' value='" + checkboxValue + "' class='hiddenCheckbox' onclick='javascript:listCheckboxClick();'></td>");
                sListHtml.append("<td width='16' class='" + sDataTdChar + "' style='text-align:center'>" + (displayTrace ? "<span " + tail + "><img src='" + sImagesPath + "search.gif' border='0' alt='点击查看流转痕迹'></span>" : "&nbsp;") + "</td>");
                sListHtml.append("<td width='16' class='" + sDataTdChar + "' style='text-align:center'><img src='" + sUrgeImagesName + "' border='0' alt='紧急'></td>");
                sListHtml.append("<td width='15' class='" + sDataTdChar + "' style='text-align:center'>" + (hasAttach ? "<img src='" + sAttachImagesName + "' border='0' alt='有附件'>" : "") + "</td>");
                sListHtml.append("<td width='15' class='" + sDataTdChar + "' style='text-align:center'>" + (isNotice ? "<img src='" + sNoticeImagesName + "' border='0' alt='知会'>" : "") + "</td>");
                sListHtml.append("<td " + href + " width='*' class='" + sFlowInstNameTd + "' style='padding-left:0px;' title='" + s_instName + (isDealOverTime ? "（待办超时，请尽快办理）" : "") + "'>");
                sListHtml.append(s_instName + "&nbsp;</td>");
                //sListHtml.append("<td width='130' class='"+sDataTdChar+"' title='" + docSeqField + "'>" + docSeqField + "&nbsp;</td>");
                sListHtml.append("<td width='130' class='" + sDataTdChar + "' title='" + s_flowName + "'>" + s_flowName + "&nbsp;</td>");
                sListHtml.append("<td width='100' class='" + sDataTdChar + "' title='" + stepName + "'><span " + tail + ">" + stepName + "</span>&nbsp;</td>");
                if (!taskType.equals("-1")) {
                    sListHtml.append("<td width='80' class='" + sDataTdChar + "' title='" + userTrueName + "'>" + sUserNameLink + "&nbsp;</td>");
                }
                sListHtml.append("<td width='130' class='data_lastTd_char' title='" + receDate + "'>" + receDate + "&nbsp;</td>");
                sListHtml.append("</tr>");
                i++;
            }//end while
        }
        long pageCount = (recordCount + pageSize - 1) / pageSize;
        while (i < pageSize) {
            int j = i + 1;
            String sDataTdChar = bWordBreak ? "data_Td_char" : "data_Td_char_No_Break";
            String sFlowInstNameTd = bWordBreak ? "data_Td_char" : "data_Td_char_No_Break";
            String sTempClassName = (i % 2 == 0) ? "data_Tr1" : "data_Tr2";
            sListHtml.append("<tr index='" + j + "' class='" + sTempClassName + "'>");
            sListHtml.append("<td width='25' flag='hiddenCheckboxTd' class='hiddenCheckboxTd'>&nbsp;</td>");
            sListHtml.append("<td width='16' class='" + sDataTdChar + "'>&nbsp;</td>");
            sListHtml.append("<td width='16' class='" + sDataTdChar + "'>&nbsp;</td>");
            sListHtml.append("<td width='15' class='" + sDataTdChar + "'>&nbsp;</td>");
            sListHtml.append("<td width='15' class='" + sDataTdChar + "'>&nbsp;</td>");
            sListHtml.append("<td  class='" + sFlowInstNameTd + "' style='padding-left:0px;' title=''>&nbsp;</td>");
            //sListHtml.append("<td  class='"+sDataTdChar+"' title=''>&nbsp;</td>");
            sListHtml.append("<td  class='" + sDataTdChar + "' title=''>&nbsp;</td>");
            sListHtml.append("<td  class='" + sDataTdChar + "' title=''>&nbsp;</td>");
            if (!taskType.equals("-1")) {
                sListHtml.append("<td width='80' class='" + sDataTdChar + "' title=''>&nbsp;</td>");
            }
            sListHtml.append("<td width='130' class='data_lastTd_char' title=''>&nbsp;</td>");
            sListHtml.append("</tr>");
            i++;
        }
        long endTime = System.currentTimeMillis();
%>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=GBK">
    <link rel="stylesheet" type="text/css" href="<%=sResPublic%>Css/Public.css">
    <link rel="stylesheet" type="text/css" href="<%=sResPublic%>Css/QueryList.css">
    <link rel="stylesheet" type="text/css" href="<%=sResPublic%>Css/ToolBar.css">
    <script type="text/javascript" src="<%=sResPublic%>Script/QueryList.js"></script>
    <script type="text/javascript" src="<%=sResPublic%>Script/MenuAndButtonbar.js"></script>
    <script type="text/javascript" src="<%=sCommonPath%>Include/Common.js"></script>
    <script type="text/JScript.Encode" src="../rtx/js/browinfo.js"></script>
    <script type="text/JScript.Encode" src="../rtx/js/rtxint.js"></script>
    <script type="text/javascript">
        //这是传递给TaskListDown.js的参数,请不要删除,且应该放在引用TaskListDown.js的前面.
        var prePath = "../";
        var startTime='<%=startTime%>';
        var afterInit='<%=afterInit%>';
        var afterPagingData='<%=afterPagingData%>';
        var afterTasksEx='<%=afterTasksEx%>';
        var endTime='<%=endTime%>';
    </script>
    <script type="text/javascript" src="<%=sCommonPath%>Include/TaskListDown.js"></script>
    <script type="text/javascript">
        var gPublicPath = "<%=sPublicPath%>";
        var gImagesPath = "<%=sImagesPath%>";
        var gUrl = "TaskListDown.jsp?pageNo=<%=pageNo%><%=urlPara%>";
        var gUrl1 = "ListByType.jsp<%=urlPara1%>";
        //分页
        function gotoPage(pageNo) {
            if (pageNo != null)
                document.all.item("c_page").value = pageNo;
            if (checkPage(gPublicPath, "<%=pageCount%>") == false)
                return;
            document.fmMain.action = "TaskListDown.jsp?pageNo=" + document.all.item("c_page").value + "<%=urlPara%>";
            document.fmMain.submit();
        }
        //公文名称的超链
        function HrefLink(sFlag, sPath) {
            var w = window.screen.width - 10;
            var h = window.screen.height - 40;
            if (sFlag == "0") {
                var sStyle = "left=0,top=0,width=" + w + "px,height=" + (h - 20) + "px,left=0px,top=0px,scrollbars=yes";
                window.open(sPath, "", sStyle);
            }
            var taskType =<%=taskType%>;
            var readFlag =<%=readFlag%>;
            if (sFlag == "1") {
                var sStyle = "dialogWidth:" + w + "px;dialogHeight:" + h + "px;center:yes;status:no";
                showModalDialog(sPath, "", sStyle);
                sArr = sPath.split("?");
                if ((sArr[0] == "FlowFrame.jsp")
                        && ((taskType == 0) || (taskType == 2) || ((taskType == 3) && (readFlag == 0)) || ((taskType == 4) && (readFlag == 0)))) {
                    window.location.reload();
                }
            }
        }
        function HrefLink_ex(sPath) {
            var W = window.screen.width;
            var H = window.screen.height - 30;
            var splashWin = window.open(sPath, null, "fullscreen=1,toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=0,resizable=0");
            splashWin.resizeTo(Math.ceil(W), Math.ceil(H));
            splashWin.moveTo(0, 0);
            splashWin.focus();
        }
        //window的初始函数
        function initWindow() {
            //根据分辨率调整大小
            var oTable = document.all.item("setTable");
            var screenW = window.screen.width;
            oTable.style.width = "1005px";
            if (screenW > 1024) {
                oTable.style.width = "100%";
            }
            //关于排序
            if ("<%=OrderFlag%>" == "hiddenFinsNameFlag")
                initDownOrUp("FinsNameFlag", "<%=orderType%>");
            if ("<%=OrderFlag%>" == "hiddenFlowNameFlag")
                initDownOrUp("FlowNameFlag", "<%=orderType%>");
            if ("<%=OrderFlag%>" == "hiddenStepNameFlag")
                initDownOrUp("StepNameFlag", "<%=orderType%>");
            if ("<%=OrderFlag%>" == "hiddenPeriNameFlag")
                initDownOrUp("PeriNameFlag", "<%=orderType%>");
            if ("<%=OrderFlag%>" == "hiddenReceDateFlag")
                initDownOrUp("ReceDateFlag", "<%=orderType%>");
            if ("<%=OrderFlag%>" == "hiddenDocSeqFlag")
                initDownOrUp("DocSeqFlag", "<%=orderType%>");
            var lImagePath = "/FlowEngineWeb/workflow/Images/";
            //产生ButtonBar
            <%if(bCanNotice){%>
            addSplit();
            ButtonBar_addItem("button_noticeIcon", lImagePath + "btn_notice_normal.gif", "", "", "知会", genBS("button_noticeIcon"), "0", "70", "23");
            <%}%>
            <%if(taskType.equals("3") && bBatchRead){%>
            addSplit();
            ButtonBar_addItem("button_batchReadIcon", lImagePath + "btn_batch_normal.gif", "", "", "批量阅毕", genBS("button_batchReadIcon"), "0", "100", "23");
            <%}%>
            <%if(taskType.equals("0")){%>
            <%if(bCanRedirect){%>
            addSplit();
            ButtonBar_addItem("button_redirectIcon", lImagePath + "btn_redirect_normal.gif", "", "", "转交", genBS("button_redirectIcon"), "0", "70", "23");
            <%}%>
            <%if(bBatchMode){%>
            ButtonBar_addItem("button_batchIcon", lImagePath + "btn_batch_normal.gif", "", "", "批处理", genBS("button_batchIcon"), "0", "70", "23");
            <%}%>
            <%if(bBatchSend){%>
            addSplit();
            ButtonBar_addItem("button_batchSendIcon", lImagePath + "btn_batch_normal.gif", "", "", "批量发送", genBS("button_batchSendIcon"), "0", "100", "23");
            <%}%>
            <%if(bBatchRead){%>
            addSplit();
            ButtonBar_addItem("button_batchReadIcon", lImagePath + "btn_batch_normal.gif", "", "", "批量阅毕", genBS("button_batchReadIcon"), "0", "100", "23");
            <%}%>
            <%}%>
            <%if((!isAdmin) && (taskType.equals("1"))){%>
            addSplit();
            ButtonBar_addItem("button_withdrawIcon", lImagePath + "btn_back_normal.gif", "", "", "回撤", genBS("button_withdrawIcon"), "0", "70", "23");
            <%}%>
            <%if(bCanDelete || taskType.equals("-1") || (taskType.equals("0") && readFlag == 1)){%>
            addSplit();
            ButtonBar_addItem("button_deleteIcon", lImagePath + "btn_del_normal.gif", "", "", "删除", genBS("button_deleteIcon"), "0", "70", "23");
            <%}%>
            <%if(taskType.equals("-1")){%>
            addSplit();
            ButtonBar_addItem("button_getIcon", lImagePath + "btn_get_normal.gif", "", "", "归类", genBS("button_getIcon"), "0", "70", "23");
            <%if(bCanCopy){%>
            addSplit();
            ButtonBar_addItem("button_copyIcon", lImagePath + "btn_copy_normal.gif", "", "", "复制", genBS("button_copyIcon"), "0", "70", "23");
            <%}%>
            <%}%>
            <%if(bScoreMode){%>
            addSplit();
            ButtonBar_addItem("button_scoreIcon", lImagePath + "btn_score_normal.gif", "", "", "评价", genBS("button_scoreIcon"), "0", "70", "23");
            <%}%>
            <%if(bCanReminder){%>
            addSplit();
            ButtonBar_addItem("button_reminderIcon", lImagePath + "btn_batch_normal.gif", "", "", "催办", genBS("button_reminderIcon"), "0", "70", "23");
            <%}%>
            createButtonBar("buttonUtil");
            //列表的初始设置
            initList();
        }
    </script>
</head>
<body class="listBody" style="overflow:auto;" onload="javascript:initWindow();">
<table id="setTable" width="100%" border="0" cellspacing="0" cellpadding="0">
    <!--ButtonBar-->
    <tr>
        <td id="buttonUtil" class="buttonUtil">
        </td>
    </tr>
    <!--开始列表部分-->
    <tr>
        <td style="padding-left:1px;">
            <table id="listTable" width="100%" border="0" cellspacing="0" cellpadding="0" class="listTable">
                <tr class="head_Tr">
                    <td width="25" class="head_firstTd">
                        <input type="checkbox" id="AllorNone" name="AllorNone" title="全选"
                               onclick="javascript:listAllorNone();">
                    </td>
                    <td width="16" class="head_Td">
                    </td>
                    <td width="16" class="head_Td" style="cursor:pointer;"
                        onclick="javascript:DownOrUp('hiddenAwokeFlag','<%=AwokeFlag%>');">
                    </td>
                    <td width="15" class="head_Td" style="cursor:pointer;">
                    </td>
                    <td width="15" class="head_Td" style="cursor:pointer;">
                    </td>
                    <td width="*" class="head_Td" style="cursor:pointer;"
                        onclick="javascript:DownOrUp('hiddenFinsNameFlag','<%=FinsNameFlag%>');">
                        名称<img alt="" name="FinsNameFlag" id="FinsNameFlag" src="<%=sImagesPath%>down.gif" border="0">&nbsp;
                    </td>
                    <!--
              <td width="130" class="head_Td" style="cursor:pointer;" onclick="javascript:DownOrUp('hiddenDocSeqFlag','<%=DocSeqFlag%>');">
                公文字号<img alt="" name="DocSeqFlag" id="DocSeqFlag" src="<%=sImagesPath%>down.gif" border="0">&nbsp;
              </td>
              -->
                    <td width="130" class="head_Td" style="cursor:pointer;"
                        onclick="javascript:DownOrUp('hiddenFlowNameFlag','<%=FlowNameFlag%>');">
                        公文类别<img alt="" name="FlowNameFlag" id="FlowNameFlag" src="<%=sImagesPath%>down.gif" border="0">&nbsp;
                    </td>
                    <td width="100" class="head_Td" style="cursor:pointer;"
                        onclick="javascript:DownOrUp('hiddenStepNameFlag','<%=StepNameFlag%>');">
                        公文状态<img alt="" name="StepNameFlag" id="StepNameFlag" src="<%=sImagesPath%>down.gif" border="0">&nbsp;
                    </td>
                    <%if (!taskType.equals("-1")) {%>
                    <td width="80" class="head_Td" style="cursor:pointer;"
                        onclick="javascript:DownOrUp('hiddenPeriNameFlag','<%=PeriNameFlag%>');">
                        发件人<img alt="" name="PeriNameFlag" id="PeriNameFlag" src="<%=sImagesPath%>down.gif" border="0">&nbsp;
                    </td>
                    <%}%>
                    <td width="130" class="head_Td" style="cursor:pointer;"
                        onclick="javascript:DownOrUp('hiddenReceDateFlag','<%=ReceDateFlag%>');">
                        接收时间<img alt="" name="ReceDateFlag" id="ReceDateFlag" src="<%=sImagesPath%>down.gif" border="0">&nbsp;
                    </td>
                </tr>
                <%=sListHtml.substring(0)%>
                <form name="fmMain" method="post" action="TaskListDown.jsp" onsubmit="return isSubmit(<%=pageCount%>);">
                    <tr>
                        <td colspan="<%=iColSpan%>">
                            <table width="100%" border="0" cellspacing="0" cellpadding="0">
                                <tr class="tail_Tr">
                                    <td class="tail_firstTd">
                                        <span class="ListUtil_inputSpan">第<%=pageNo%>页/共<%=pageCount%>页（共<%=recordCount%>条记录）&nbsp;&nbsp;</span>
                                    </td>
                                    <td class="tail_lastTd">
                                        <%
                                            if (pageNo > 1) {
                                                out.print("<input type='button' class='standardBtn' value='首  页' onclick='javascript:gotoPage(1);'>&nbsp;&nbsp;");
                                                out.print("<input type='button' class='standardBtn' value='上一页' onclick='javascript:gotoPage(" + (pageNo - 1) + ");'>&nbsp;&nbsp;");
                                            }
                                            if (pageCount > 1 && pageNo < pageCount) {
                                                out.print("<input type='button' class='standardBtn' value='下一页' onclick='javascript:gotoPage(" + (pageNo + 1) + ");'>&nbsp;&nbsp;");
                                                out.print("<input type='button' class='standardBtn' value='尾  页' onclick='javascript:gotoPage(" + pageCount + ");'>&nbsp;&nbsp;");
                                            }
                                        %>
                                        &nbsp;&nbsp;
                    <span class="ListUtil_inputSpan">
                    查看第
                    <input type="text" name="c_page" class="ListUtil_input">
                    页&nbsp;
                    </span>
                                        <input type='button' class='standardBtn' value="查  看"
                                               onclick="javascript:gotoPage(null)">
                                        &nbsp;&nbsp;
                                        <input type="hidden" name="c_instName" value="<%=instName%>">
                                        <input type="hidden" name="c_flowName" value="<%=flowName%>">
                                        <input type="hidden" name="c_beginDateLower" value="<%=beginDateLower%>">
                                        <input type="hidden" name="c_beginDateHigher" value="<%=beginDateHigher%>">
                                        <input type="hidden" name="c_endDateLower" value="<%=endDateLower%>">
                                        <input type="hidden" name="c_endDateHigher" value="<%=endDateHigher%>">
                                        <input type="hidden" name="c_creator" value="<%=creator%>">
                                        <input type="hidden" name="c_sender" value="<%=sender%>">
                                        <input type="hidden" name="c_flowFolder" value="<%=flowFolder%>">
                                        <input type="hidden" name="c_docSeq" value="<%=docSeq%>">
                                        <input type="hidden" name="c_field" value="<%=fieldInfo%>">
                                        <input type="hidden" name="hiddenAwokeFlag" value="<%=AwokeFlag%>">
                                        <input type="hidden" name="hiddenFinsNameFlag" value="<%=FinsNameFlag%>">
                                        <input type="hidden" name="hiddenFlowNameFlag" value="<%=FlowNameFlag%>">
                                        <input type="hidden" name="hiddenStepNameFlag" value="<%=StepNameFlag%>">
                                        <input type="hidden" name="hiddenPeriNameFlag" value="<%=PeriNameFlag%>">
                                        <input type="hidden" name="hiddenReceDateFlag" value="<%=ReceDateFlag%>">
                                        <input type="hidden" name="hiddenDocSeqFlag" value="<%=DocSeqFlag%>">
                                        <input type="hidden" name="hiddenOrder" value="<%=OrderFlag%>">
                                        <input type="hidden" name="sClassID" value="<%=sClassID%>">
                                        <input type="hidden" name="batchMode" value="<%=sBatchMode%>">
                                        <input type="hidden" name="scoreMode" value="<%=sScoreMode%>">
                                        <input type="hidden" name="canCopy" value="<%=sCanCopy%>">
                                        <input type="hidden" name="readOnly" value="<%=sReadOnly%>">
                                        <input type="hidden" name="batchRead" value="<%=bBatchRead?1:0%>">
                                        <input type="hidden" name="batchSend" value="<%=bBatchSend?1:0%>">
                                        <input type="hidden" name="canNotice" value="<%=sCanNotice%>">
                                        <input type="hidden" name="canRedirect" value="<%=sCanRedirect%>">
                                        <input type="hidden" name="flowDir" value="<%=flowDir%>">
                                        <input type="hidden" name="includeSubFlowDir"
                                               value="<%=includeSubFlowDir?"1":"0"%>">
                                        <input type="hidden" name="displayRow" value="<%=iDisRow%>">
                                        <input type="hidden" name="canDelete" value="<%=sCanDelete%>">
                                        <input type="hidden" name="canReminder" value="<%=sCanReminder%>">
                                        <input type="hidden" name="showOnlyMe" value="<%=sShowOnlyMe%>">
                                        <input type="hidden" name="bookmark" value="<%=bookmark%>">
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                </form>
            </table>
        </td>
    </tr>
</table>
<table style="position:absolute;top:0px;left:0px;width:0px;height:0px;">
    <form id="SubUtilForm" action="">
        <tr>
            <td width="0">
                <iframe id="SubUtil" name="SubUtil" marginWidth="0" marginHeight="0" frameBorder="0"
                        style="position:absolute;top:0px;left:0px;width:0px;height:0px;">
                </iframe>
            </td>
            <td>
                <input type="hidden" name="subStructXml" value="">
            </td>
        </tr>
    </form>
</table>
</body>
</html>
<%
    } catch (Exception e) {
        System.out.println(e.getMessage());
    } finally {
        if (pagingData != null) {
            pagingData.clear();
            pagingData = null;
        }
        SysPara.closeCachedRowSet(taskRs);
        taskRs = null;
    }
%>
