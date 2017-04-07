<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="flowengineweb.helper.*" %>
<%@ page import="flowengineweb.*" %>
<%
//取得路径变量
String sCommonPath="";
sCommonPath = SysPara.getCommonPath(request);
String sResPublic  = SysPara.getResPublicPath(request);
String sResPrivate=SysPara.getResPrivatePath(request);
//测试化工作流辅助类。
WorkFlowHelper wfHelper=new WorkFlowHelper();
wfHelper.initialize(request,response,pageContext);
//设置不使用缓存。
wfHelper.setNoCache();
%>
<%
String taskType = "-1";//全部
long readFlag = -1;//全部
if(request.getParameter("taskType") != null)
taskType = request.getParameter("taskType");
if(request.getParameter("readFlag") != null)
readFlag = Integer.parseInt(request.getParameter("readFlag"));
String urlPara = "?taskType=" + taskType + "&readFlag=" + readFlag;
String disType=" disabled";
if (taskType.equals("-1"))
disType="";
String flowName = request.getParameter("c_flowName");
if(flowName == null) flowName = "";
String fieldInfo = request.getParameter("c_field");
if(fieldInfo == null) fieldInfo = "";
//========================================================
String sBatchSend=request.getParameter("batchSend");//不修改任何内容,直接发送所有选中待办.
if(sBatchSend==null) sBatchSend="";
String sBatchMode=request.getParameter("batchMode");
if(sBatchMode==null) sBatchMode="";
String sBatchRead=request.getParameter("batchRead");
if(sBatchRead==null) sBatchRead="";
String sScoreMode=request.getParameter("scoreMode");
String sCanCopy=request.getParameter("canCopy");
String sReadOnly=request.getParameter("readOnly");
String sCanNotice=request.getParameter("canNotice");
String sCanRedirect=request.getParameter("canRedirect");
String disRow=request.getParameter("displayRow");
String sCanDelete=request.getParameter("canDelete");
String sCanReminder=request.getParameter("canReminder");
String sShowOnlyMe=request.getParameter("showOnlyMe");//只显示本人经办过的记录。
String bookmark=wfHelper.readParameter("bookmark","");
if(sShowOnlyMe==null) sShowOnlyMe="0";//缺省是显示所有，该参数实际上对管理员和部门管理员才有效。
if(sCanDelete==null) sCanDelete="1";
if(disRow==null) disRow="";
if(sScoreMode==null) sScoreMode="";
if(sCanCopy==null) sCanCopy="";
if(sReadOnly==null) sReadOnly="";
if(sCanNotice==null) sCanNotice="";
if(sCanRedirect==null) sCanRedirect="";

String flowDir="";
boolean includeSubFlowDir=false;
if(request.getParameter("flowDir")!=null)
flowDir=request.getParameter("flowDir");
if(request.getParameter("includeSubFlowDir")!=null)
includeSubFlowDir=request.getParameter("includeSubFlowDir").trim().equals("1");
String flowFolderHtml = wfHelper.getWorkflowHelper().getFlowFolderHtml();
String hiddenFlowFolder=wfHelper.readParameter("hiddenFlowFolder","");
String folderCode=wfHelper.readParameter("c_flowFolder","");
String instName=wfHelper.readParameter("c_instName","");
String creator=wfHelper.readParameter("c_creator","");


//生成公文类别
String flowListHtml = wfHelper.getWorkflowHelper().getFlowListHtml(hiddenFlowFolder);
%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">

  <link rel="stylesheet" type="text/css" href="<%=sResPublic%>Css/Public.css">
  <link rel="stylesheet" type="text/css" href="<%=sResPrivate%>Css/WorkFlow.css">
  <script type="text/javascript">
  function AdvancedSearch()
  {
    var feature = "dialogWidth:705px;dialogHeight:285px;center:yes;status:no";
    var path = "../AdvancedSearch.jsp?c_flowName=<%=flowName%>";
    var sReturn = showModalDialog(path,"",feature);
    if (sReturn != "" && sReturn != "undefined" &&  sReturn != undefined)
    {
      sArr = sReturn.split("##");
      if (sArr.length == 10)
      {
        document.all.item("c_instName").value = sArr[0];
        <%if(flowName.equals("")){%>
        document.all.item("c_flowName").value = sArr[1];
        <%}else{%>
        document.all.item("c_flowName").value = "<%=flowName%>";
        <%}%>
        document.all.item("c_beginDateLower").value = sArr[2];
        document.all.item("c_beginDateHigher").value = sArr[3];
        document.all.item("c_endDateLower").value = sArr[4];
        document.all.item("c_endDateHigher").value = sArr[5];
        document.all.item("c_creator").value = sArr[6];
        document.all.item("c_sender").value = sArr[7];
        document.all.item("c_flowFolder").value = sArr[8];
        document.all.item("c_docSeq").value = sArr[9];
        document.forms("TaskList").submit();
      }
    }
  }

  function Search()
  {
    document.all.item("c_beginDateLower").value = "";
    document.all.item("c_beginDateHigher").value = "";
    document.all.item("c_endDateLower").value = "";
    document.all.item("c_endDateHigher").value = "";
    document.all.item("c_docSeq").value = "";
    document.forms("TaskList").submit();
  }
  function initWindow()
  {
    obj = document.all.item("c_flowFolder");
    for (i=0;i<obj.length;i++)
    {
      if (obj.options[i].value == "<%=folderCode%>")
      {
        obj.options[i].selected = true;
        break;
      }
    }
  }
  function changeFlowFolder()
  {
    obj = document.all.item("c_flowFolder");
    for (i=0;i<obj.length;i++)
    {
      if (obj.options[i].selected)
      {
        temp = obj.options[i].value;
        break;
      }
    }
    document.all.item("hiddenFlowFolder").value=temp;
    document.forms("TaskList").action="TaskListUp.jsp?<%=urlPara%>";
    document.forms("TaskList").target="_self";
    document.forms("TaskList").submit();
  }
  </script>
  </head>
  <body style="overflow:hidden;" class="PanelFlat" onload="javascript:initWindow();">
  <table width="100%" border="0" cellspacing="0" cellpadding="0" style="height:70px;padding-left:5px;padding-right:5px;">
    <tr>
      <td width="100%" valign="top" class="Underline_Td" style="padding-top:10px;padding-bottom:10px;">
        <table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="78%" valign="top">
              <fieldset>
                <legend>模糊查询</legend>
                <table width="100%" align="center" border="0" cellspacing="0" cellpadding="0" style="padding-bottom:5px;">
                  <form target="taskListFrameDown" name="TaskList" id="TaskList" action="TaskListDown.jsp<%=urlPara%>" method="post">
                  <tr>
                    <td valign="top" align="center">
                      <table width="100%">
                        <tr>
                          <td align="right">
                          目录：
                          </td>
                          <td align="left">
                            <select name="c_flowFolder" style="width:170px;" onchange="javascript:changeFlowFolder();"><%=flowFolderHtml%></select>
                          </td>
                          <td align="right">
                          类别：
                          </td>
                          <td align="left">
                            <select name="c_flowName" style="width:170px;"><%=flowListHtml%></select>
                          </td>
                        </tr>
                        <tr>
                          <td align="right">
                          名称：
                          </td>
                          <td align="left">
                            <input type="text" value="<%=instName%>" name="c_instName" size="25" style="width:170px;" title="请输入名称模糊查询条件" class="standardInput">
                          </td>
                          <td align="right">
                          经办人：
                          </td>
                          <td align="left">
                            <input type="text" name="c_creator" value="<%=creator%>" size="25" style="width:170px;" title="请输入经办人模糊查询条件" class="standardInput">
                          </td>
                        </tr>
                      </table>
                      <input type="hidden" name="c_beginDateLower">
                      <input type="hidden" name="c_beginDateHigher">
                      <input type="hidden" name="c_endDateLower">
                      <input type="hidden" name="c_endDateHigher">
                      <input type="hidden" name="c_sender">
                      <input type="hidden" name="c_docSeq">
                      <input type="hidden" name="c_field" value="<%=fieldInfo%>">
                      <input type="hidden" name="batchMode" value="<%=sBatchMode%>">
                      <input type="hidden" name="scoreMode" value="<%=sScoreMode%>">
                      <input type="hidden" name="canCopy" value="<%=sCanCopy%>">
                      <input type="hidden" name="batchRead" value="<%=sBatchRead%>">
                      <input type="hidden" name="batchSend" value="<%=sBatchSend%>">
                      <input type="hidden" name="flowDir" value="<%=flowDir%>">
                      <input type="hidden" name="includeSubFlowDir" value="<%=includeSubFlowDir?"1":"0"%>">
                      <input type="hidden" name="readOnly" value="<%=sReadOnly%>">
                      <input type="hidden" name="canNotice" value="<%=sCanNotice%>">
                      <input type="hidden" name="canRedirect" value="<%=sCanRedirect%>">
                      <input type="hidden" name="displayRow" value="<%=disRow%>">
                      <input type="hidden" name="canDelete" value="<%=sCanDelete%>">
                      <input type="hidden" name="canReminder" value="<%=sCanReminder%>">
                      <input type="hidden" name="showOnlyMe" value="<%=sShowOnlyMe%>">
                      <input type="hidden" name="hiddenFlowFolder" value="<%=hiddenFlowFolder%>">
                      <input type="hidden" name="bookmark" value="<%=bookmark%>">
                    </td>
                    <td width="100px" align="center">
                      <input type="button" value="查　询" class="standardBtn" onclick="javascript:Search();">
                    </td>
                  </tr>
                  </form>
                </table>
              </fieldset>
            </td>
            <td width="2%" valign="top">
            &nbsp;
            </td>
            <td width="20%" valign="top">
              <fieldset>
                <legend>高级查询</legend>
                <table width="100%" height="50px" align="center" border="0" cellspacing="0" cellpadding="0" style="padding-bottom:2px;">
                  <tr>
                    <td width="100%" valign="middle" align="center">
                      <input type="button" value="查　询" class="standardBtn" onclick="javascript:AdvancedSearch();">
                    </td>
                  </tr>
                    <tr>
                        <td>
                            提示：查询2014年12月31日以前的公文，请设置“公文创建日期小于”条件
                        </td>
                    </tr>
                </table>
              </fieldset>
            </td>
          </tr>
        </table>
      </td>
    </tr>
  </table>
  </body>
</html>
