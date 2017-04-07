<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="java.util.*" %>
<%@ page import="sun.jdbc.rowset.CachedRowSet" %>
<%@ page import="flowengineweb.*" %>
<%@ page import="flowengineforjava.util.PublicLib" %>

<%@ page import="flowengineweb.helper.WorkFlowHelper" %>
<%
  //long startTime = new Date().getTime();
  //取得路径变量
  String curUser=FlowWebBean.getCurrentUser(request);
  FlowWebBean fwb=new FlowWebBean(curUser);
  String userFullName=fwb.getUserName(curUser);
  String userPosiName=fwb.getPosiNameOfUser(curUser);
  String sCommonPath="",sImagesPath="",sPublicPath="";
  sCommonPath = SysPara.getCommonPath(request);
  sImagesPath = SysPara.getImagesPath(request);
  sPublicPath = SysPara.getPublicPath(request);
  String sResPublic  = SysPara.getResPublicPath(request);
  //测试化工作流辅助类。
  WorkFlowHelper wfHelper=new WorkFlowHelper();
  wfHelper.initialize(request,response,pageContext);
  //设置不使用缓存。
  wfHelper.setNoCache();
%>
<%
  CachedRowSet countRs = null;
  CachedRowSet taskRs = null;
  try{
  //初始化
  String Per_instName="30",Per_docSeqField="13",Per_flowName="15",Per_stepName="13",Per_UserName="11",Per_receDate="18";
  boolean isAdmin = wfHelper.isAdmin();
  boolean isDeptAdmin=fwb.isDeptAdmin();
  String taskType = wfHelper.readParameter("taskType","-1");//全部
  long readFlag =wfHelper.readParameterInt("readFlag",-1);//全部
  String creator = wfHelper.readParameter("c_creator","");
  String beginDateLower = wfHelper.readParameter("c_beginDateLower","");
  String beginDateHigher = wfHelper.readParameter("c_beginDateHigher","");
  String endDateLower = wfHelper.readParameter("c_endDateLower","");
  String endDateHigher = wfHelper.readParameter("c_endDateHigher","");
  String flowName = wfHelper.readParameter("c_flowName","");
  String instName = wfHelper.readParameter("c_instName","");
  String sender = wfHelper.readParameter("c_sender","");
  String flowFolder = wfHelper.readParameter("c_flowFolder","");
  String docSeq = wfHelper.readParameter("c_docSeq","");
  String fieldInfo =wfHelper.readParameter("c_field","");
  String AwokeFlag = wfHelper.readParameter("hiddenAwokeFlag","down");
  String FinsNameFlag = wfHelper.readParameter("hiddenFinsNameFlag","down");
  String FlowNameFlag = wfHelper.readParameter("hiddenFlowNameFlag","down");
  String StepNameFlag = wfHelper.readParameter("hiddenStepNameFlag","down");
  String PeriNameFlag = wfHelper.readParameter("hiddenPeriNameFlag","down");
  String ReceDateFlag = wfHelper.readParameter("hiddenReceDateFlag","down");
  String DocSeqFlag = wfHelper.readParameter("hiddenDocSeqFlag","down");
  String OrderFlag = wfHelper.readParameter("hiddenOrder","");
  String bookmark=wfHelper.readParameter("bookmark","");
  String orderFieldParaName="TaskListApprove_"+bookmark+"_OrderField";
  String orderTypeParaName="TaskListApprove_"+bookmark+"_OrderType";
  String orderType="";
  if(OrderFlag.trim().length()==0){
    try{
      OrderFlag=wfHelper.getBaseServerClientJB().getPersonalParameter(orderFieldParaName,"",false);
      orderType=wfHelper.getBaseServerClientJB().getPersonalParameter(orderTypeParaName,"",false);
    }
    catch(Exception ex){
      ex.printStackTrace();
    }
  }
  else{
    orderType=wfHelper.readParameter(OrderFlag,"down");
  }
  if(OrderFlag.trim().length()>0){
    wfHelper.getBaseServerClientJB().setPersonalParameter(orderFieldParaName,OrderFlag);
    wfHelper.getBaseServerClientJB().setPersonalParameter(orderTypeParaName,orderType);
  }
  //关于个人归类
  String sClassID = "";
  sClassID = request.getParameter("sClassID");
  if (sClassID == null || sClassID.equals(""))
    sClassID = "";
  //关于排序
  String sOrder = "";
  if (OrderFlag.equals("hiddenAwokeFlag"))
  {
    sOrder=wfHelper.genOrderByField(orderType,"A.sys_urge");
  }
  else if (OrderFlag.equals("hiddenFinsNameFlag"))
  {
    sOrder=wfHelper.genOrderByField(orderType,"A.fins_name");
  }
  else if (OrderFlag.equals("hiddenFlowNameFlag"))
  {
    sOrder=wfHelper.genOrderByField(orderType,"T.item_name");
  }
  else if (OrderFlag.equals("hiddenStepNameFlag"))
  {
    if((isAdmin||isDeptAdmin) && taskType.equals("-1")){
      sOrder=wfHelper.genOrderByField(orderType,"B.step_name");
    }else{
      sOrder=wfHelper.genOrderByField(orderType,"B2.step_name");
    }
  }
  else if (OrderFlag.equals("hiddenPeriNameFlag"))
  {
    sOrder=wfHelper.genOrderByField(orderType,"D.full_name");
  }
  else if (OrderFlag.equals("hiddenReceDateFlag"))
  {
    if(!((isAdmin||isDeptAdmin) && taskType.equals("-1"))){
      sOrder=wfHelper.genOrderByField(orderType,"C.rece_date");
    }else{
      sOrder=wfHelper.genOrderByField(orderType,"A.inst_crda");
    }
  }
  else if (OrderFlag.equals("hiddenDocSeqFlag"))
  {
    sOrder=wfHelper.genOrderByField(orderType,"A.doc_seq");
  }
  else{
    if(taskType.equals("0") || (taskType.equals("3") && readFlag==0) || (taskType.equals("4") && readFlag==0)) sOrder = "A.sys_urge desc,";
    if((isAdmin||isDeptAdmin) && taskType.equals("-1"))
    sOrder += "A.inst_crda desc";
    else
    sOrder += "C.rece_date desc";
  }

  String condition = "";
  int pageNo = 1;//第一页
  if (taskType.equals("-1"))
  {
    Per_instName="33";
    Per_docSeqField="16";
    Per_flowName="17";
    Per_stepName="16";
    Per_receDate="18";
  }
  String urlPara = "&taskType=" + taskType + "&readFlag=" + readFlag;
  String urlPara1 = "?taskType=" + taskType + "&readFlag=" + readFlag + "&sClassID=" + sClassID;

  if(request.getParameter("pageNo") != null)
      pageNo = Integer.parseInt(request.getParameter("pageNo"));
  if(!fieldInfo.equals("")){
    Vector oVTEveryCase=new Vector();
    String sqlStr="";
    int iIndex=0;
    String strJoin="";
    int iCount=-1;
    int iStart=0;
    iCount=fieldInfo.indexOf(" and ",iStart);
    if(iCount!=-1){
      strJoin=" and ";
      while(iCount!=-1){
        String firstCase=fieldInfo.substring(iStart,iCount);
        oVTEveryCase.add(iIndex,firstCase);
        iIndex=iIndex+1;
        iStart=iCount+5;
        iCount=fieldInfo.indexOf(" and ",iStart);
      }
      String lastCase=fieldInfo.substring(iStart,fieldInfo.length());
      oVTEveryCase.add(iIndex,lastCase);
    }else{
      iCount=fieldInfo.indexOf(" or ",iStart);
      if(iCount!=-1){
        strJoin=" or ";
        while(iCount!=-1){
          String firstCase=fieldInfo.substring(iStart,iCount);
          oVTEveryCase.add(iIndex,firstCase);
          iIndex=iIndex+1;
          iStart=iCount+4;
          iCount=fieldInfo.indexOf(" or ",iStart);
        }
        String lastCase=fieldInfo.substring(iStart,fieldInfo.length());
        oVTEveryCase.add(iIndex,lastCase);
      }
    }
    String arrOperate[]={"=","<>"," like "," not like "};
    if(iIndex>0){
      for(int e=0;e<oVTEveryCase.size();e++){
        String temp=(String)oVTEveryCase.get(e);
        int iS=0;
        int iC=-1;
        String urlFieldName="";
        String urlFieldValue="";
        String strOperate="";
        for(int f=0;f<arrOperate.length;f++){
          strOperate=arrOperate[f];
          iC=temp.indexOf(strOperate,iS);
          if(iC!=-1){
            urlFieldName=temp.substring(iS,iC);
            iS=iC+strOperate.length();
            urlFieldValue=temp.substring(iS,temp.length());
            break;
          }
        }
        if((strOperate.equals(" like ")) || (strOperate.equals(" not like "))){
          urlFieldValue="%"+urlFieldValue+"%";
        }
        if(!urlFieldName.equals("")){
          //如果手工加了前缀，则不加上AA前缀。
          boolean hasPrefix=urlFieldName.indexOf(".")!=-1;
          //如果不需要自动加上“'”
          String semicolon="'";
          if(urlFieldValue.startsWith("[")){
			if(urlFieldValue.endsWith("]")){
				urlFieldValue=urlFieldValue.substring(1,urlFieldValue.length()-1);
				semicolon="";
			}
            else{
				System.err.println("c_field参数指定了非法的值");
				out.println("<font color=red>c_field参数指定了非法的值</font>");
            }
          }
          if(sqlStr.equals(""))
            sqlStr=(hasPrefix?"(":"(AA.")+urlFieldName+strOperate+semicolon+urlFieldValue+semicolon+")";
          else
            sqlStr=sqlStr+strJoin+(hasPrefix?"(":"(AA.")+urlFieldName+strOperate+semicolon+urlFieldValue+semicolon+")";
        }
      }
    }else{
      int iS=0;
      int iC=-1;
      String urlFieldName="";
      String urlFieldValue="";
      String strOperate="";
      for(int g=0;g<arrOperate.length;g++){
        strOperate=arrOperate[g];
        iC=fieldInfo.indexOf(strOperate,iS);
        if(iC!=-1){
          urlFieldName=fieldInfo.substring(iS,iC);
          iS=iC+strOperate.length();
          urlFieldValue=fieldInfo.substring(iS,fieldInfo.length());
          break;
        }
      }
      if((strOperate.equals(" like ")) || (strOperate.equals(" not like "))){
        urlFieldValue="%"+urlFieldValue+"%";
      }
      if(!urlFieldName.equals("")){
        //如果手工加了前缀，则不加上AA前缀。
        boolean hasPrefix=urlFieldName.indexOf(".")!=-1;
		//如果不需要自动加上“'”
          String semicolon="'";
          if(urlFieldValue.startsWith("[")){
			if(urlFieldValue.endsWith("]")){
				urlFieldValue=urlFieldValue.substring(1,urlFieldValue.length()-1);
				semicolon="";
			}
            else{
				System.err.println("c_field参数指定了非法的值");
				out.println("<font color=red>c_field参数指定了非法的值</font>");
            }
          }
        sqlStr=(hasPrefix?"(":"(AA.")+urlFieldName+strOperate+semicolon+urlFieldValue+semicolon+")";
      }
    }
    //System.out.println("sqlStr="+sqlStr);
    condition = sqlStr;
  }
  if(isAdmin && taskType.equals("-1")) {
      countRs = wfHelper.getWorkflowHelper().getAdminTaskCount(condition, taskType, readFlag, creator, beginDateLower, beginDateHigher, endDateLower, endDateHigher, flowName, instName, flowFolder, docSeq, sOrder, sClassID);
  }else {
      countRs = wfHelper.getWorkflowHelper().getTaskCount(condition, taskType, readFlag, creator, beginDateLower, beginDateHigher, endDateLower, endDateHigher, flowName, instName, sender, flowFolder, docSeq, sOrder, sClassID);
  }
  int pageSize = 10;
  String tmpPageSize = wfHelper.getSysParaValue("WF/PAGESIZE");
  if(tmpPageSize != null && !tmpPageSize.trim().equals(""))
    pageSize = Integer.parseInt(tmpPageSize);
  int recordCount = countRs.size();
  int bookMark = pageSize*(pageNo-1);
  if(bookMark > 0)
    countRs.absolute(bookMark);
  int count = 0;
  String flowInids = "";
  while(countRs.next() && count<pageSize){
    if(!flowInids.equals(""))
       flowInids += ",";
    flowInids += "'" + countRs.getString("flow_inid") + "'";
    count++;
  }
  if(flowInids.equals(""))
    condition = "1=2";
  else{
    condition = "A.flow_inid in (" + flowInids + ")";
  }
  taskRs = wfHelper.getWorkflowHelper().getTasksEx(condition,taskType,readFlag,creator,beginDateLower,beginDateHigher,endDateLower,endDateHigher,flowName,instName,sender,flowFolder,docSeq,sOrder,sClassID);
  String para = "";
  int i = 0;
  int iColSpan=6;
  if (!taskType.equals("-1"))
    iColSpan=7;
  StringBuffer sListHtml = new StringBuffer(30000);
  //关于附件
  String sAttachList = "";
  if(!flowInids.equals(""))
    sAttachList = wfHelper.getWorkflowHelper().getAttachFlow(flowInids);
  String[] sArrAttachList=null;
  if (sAttachList != null && !sAttachList.equals(""))
  {
    sArrAttachList = PublicLib.split(sAttachList,";");
  }

  String flowInstList = "";
  while(taskRs.next()){
    String userName = "";
    String userTrueName = "";
    String sUserNameLink = "";
    String flowInid = taskRs.getString("flow_inid");
    String flowInst = ";" + flowInid + ";";
    if(flowInstList.indexOf(flowInst) != -1)
        continue;
    flowInstList += flowInst;
    //关于紧急公文
    String sUrgeImagesName = sImagesPath + "Blank.gif";
    int iUrgeFile=taskRs.getInt("sys_urge");
    if(iUrgeFile > 0)
      sUrgeImagesName = sImagesPath + "UrgeFile.gif";
    //关于附件
    String sAttachImagesName = sImagesPath + "Blank.gif";
    if (sArrAttachList != null && sArrAttachList.length > 0)
    {
      for (int m=0;m<sArrAttachList.length;m++)
      {
        if (sArrAttachList[m].equals(flowInid))
        {
          sAttachImagesName = sImagesPath + "Attach.gif";
          break;
        }
      }
    }
    //关于知会
    String sNoticeImagesName = sImagesPath + "Blank.gif";
    String sStepType = taskRs.getString("step_type");
    if(sStepType.equals("3"))
      sNoticeImagesName = sImagesPath + "Notice.gif";
    String stepInid = taskRs.getString("step_inco");
    String docSeqField = taskRs.getString("doc_seq");
    if (docSeqField == null)
      docSeqField = "";
    String s_flowName = taskRs.getString("flow_name");
    if(s_flowName == null)
      s_flowName = "&nbsp;";
    String ifReload = "0";
    if (taskType.equals("0") || taskType.equals("2") || (taskType.equals("3") && readFlag == 0) || (taskType.equals("4") && readFlag == 0))
      ifReload = "1";
    para = "action=0&flowInid=" +  flowInid + "&stepInid=" + stepInid + "&ifReload=" + ifReload;
    String stepName = taskRs.getString("step_name");
    if(stepName == null)
      stepName = "&nbsp;";
    String s_instName = taskRs.getString("fins_name");
    if ((s_instName == null) || (s_instName.equals("")))
      s_instName = "<空>";
    String receDate = "";
    if(isAdmin && taskType.equals("-1"))//管理员取步骤实例创建时间
       receDate = taskRs.getString("crea_date");
    else//普通用户取经办记录接受时间
       receDate = taskRs.getString("rece_date");
    if(receDate != null && !receDate.equals("")){
        int pos = receDate.lastIndexOf(":");
        receDate = receDate.substring(0,pos);
    }else{
        receDate = "";
    }

    //关于个人信息
    userName = taskRs.getString("peri_man");
    if(userName != null && !userName.equals(""))
    {
      userTrueName = taskRs.getString("peri_name");
      if(userTrueName == null || userTrueName.equals(""))
        userTrueName = userName;
      sUserNameLink = "<span onclick=\"javascript:UserInfo('"+userName+"')\">" + userTrueName + "</span>";
    }
    String dealMan = taskRs.getString("deal_man");
    String href = "";
    if(SysPara.FlowFrame_OpenMode == 0)
      href = "onclick=\"javascript:HrefLink('0','../../../FlowFrame.jsp?"+para+"');\"";
    else if(SysPara.FlowFrame_OpenMode == 1)
      href = "onclick=\"javascript:HrefLink('1','../../../FlowFrame.jsp?"+para+"');\"";
    String tail = "";
    if(SysPara.FlowTrace_OpenMode == 0)
    {
      if(SysPara.FlowTrace_ShowMode == 0)
        tail = "onclick=\"javascript:HrefLink('0','../../../FlowTrace2.jsp?flowInid="+flowInid+"');\"";
      else if(SysPara.FlowTrace_ShowMode == 1)
        tail = "onclick=\"javascript:HrefLink('0','../../../FlowTrace2.jsp?flowInid="+flowInid+"');\"";
    }
    else if(SysPara.FlowTrace_OpenMode == 1)
    {
      if(SysPara.FlowTrace_ShowMode == 0)
        tail = "onclick=\"javascript:HrefLink('1','../../../FlowTrace2.jsp?flowInid="+flowInid+"');\"";
      else if(SysPara.FlowTrace_ShowMode == 1)
        tail = "onclick=\"javascript:HrefLink('1','../../../FlowTrace2.jsp?flowInid="+flowInid+"');\"";
    }
    String checkboxValue = flowInid + "," + stepInid + "," + dealMan;
    int j = i + 1;
    String sTempClassName = (i%2==0)?"data_Tr1":"data_Tr2";
    sListHtml.append("<tr index='" + j + "' class='" + sTempClassName + "' onclick='javascript:listTrClick(this);'>");
    sListHtml.append("<td width='30' flag='hiddenCheckboxTd' class='hiddenCheckboxTd'><input type='checkbox' id='hiddenCheckbox' name='hiddenCheckbox' value='" + checkboxValue + "' class='hiddenCheckbox' onclick='javascript:listCheckboxClick();'></td>");
    sListHtml.append("<td width='30' class='data_Td_char'><span " + tail + "><img src='" + sImagesPath + "search.gif' border='0' alt='流转痕迹'></span></td>");
    sListHtml.append("<td width='30' class='data_Td_char'><img src='" + sUrgeImagesName + "' border='0'></td>");
    sListHtml.append("<td width='"+Per_instName+"%' class='data_Td_char' style='padding-left:0px;' title='" + s_instName + "'><img src='" + sAttachImagesName + "' border='0' align='top'><img src='" + sNoticeImagesName + "' border='0' align='top'><span " + href + ">" + s_instName + "</span>&nbsp;</td>");
    sListHtml.append("<td width='"+Per_stepName+"%' class='data_Td_char' title='" + stepName + "'><span " + tail + ">" + stepName + "</span>&nbsp;</td>");
    if (!taskType.equals("-1")){
      //sListHtml.append("<td width='"+Per_UserName+"%' class='data_Td_char' title='" + userTrueName + "'>" + sUserNameLink + "&nbsp;</td>");
      sListHtml.append("<td width='"+Per_UserName+"%' class='data_Td_char' title='" + userTrueName + "'>"+ userTrueName+"&nbsp;</td>");
    }
    sListHtml.append("<td width='"+Per_receDate+"%' class='data_lastTd_char' title='" + receDate + "'>" + receDate + "&nbsp;</td>");
    sListHtml.append("</tr>");
    i++;
  }//end while
  int pageCount = (recordCount+pageSize-1)/pageSize;
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
<script type="text/javascript" src="<%=sCommonPath%>Include/TaskListDown.js"></script>
<script type="text/javascript">
var gPublicPath="<%=sPublicPath%>";
var gImagesPath="<%=sImagesPath%>";
var gUrl="TaskListDownApprove.jsp?pageNo=<%=pageNo%><%=urlPara%>";
var gUrl1="ListByType.jsp<%=urlPara1%>";
//分页
function gotoPage(pageNo){
  if(pageNo != null)
    document.all.item("c_page").value=pageNo;
  if(checkPage(gPublicPath,"<%=pageCount%>") == false)
    return;
  document.fmMain.action = "TaskListDownApprove.jsp?pageNo=" + document.all.item("c_page").value + "<%=urlPara%>";
  document.fmMain.submit();
}
//公文名称的超链
function HrefLink(sFlag,sPath){
  var w=window.screen.width-10;
  var h=window.screen.height-40;
  if (sFlag == "0"){
    var sStyle="left=0,top=0,width="+w+"px,height="+(h-20)+"px,left=0px,top=0px,scrollbars=yes";
    window.open(sPath,"",sStyle);
  }
  if (sFlag == "1"){
    var sStyle="dialogWidth:"+w+"px;dialogHeight:"+h+"px;center:yes;status:no";
    showModalDialog(sPath,"",sStyle);
    sArr = sPath.split("?");
    if ((sArr[0] == "FlowFrame.jsp") && ((<%=taskType%> == 0) || (<%=taskType%> == 2) || ((<%=taskType%> == 3) && (<%=readFlag%> == 0)) || ((<%=taskType%> == 4) && (<%=readFlag%> == 0))))
      window.location.reload();
  }
}
function HrefLink_ex(sPath){
  var W = window.screen.width;
  var H = window.screen.height-30;
  var splashWin = window.open(sPath,null,"fullscreen=1,toolbar=0,location=0,directories=0,status=0,menubar=0,scrollbars=0,resizable=0");
  splashWin.resizeTo( Math.ceil( W ), Math.ceil( H ) );
  splashWin.moveTo(0,0);
  splashWin.focus();
}
//window的初始函数
function initWindow(){
  //根据分辨率调整大小
  var oTable = document.all.item("setTable");
  var screenW = window.screen.width;
  oTable.style.width = "100%";
  //关于排序
  if ("<%=OrderFlag%>" == "hiddenFinsNameFlag")
    initDownOrUp("FinsNameFlag","<%=orderType%>");
  if ("<%=OrderFlag%>" == "hiddenFlowNameFlag")
    initDownOrUp("FlowNameFlag","<%=orderType%>");
  if ("<%=OrderFlag%>" == "hiddenStepNameFlag")
    initDownOrUp("StepNameFlag","<%=orderType%>");
  if ("<%=OrderFlag%>" == "hiddenPeriNameFlag")
    initDownOrUp("PeriNameFlag","<%=orderType%>");
  if ("<%=OrderFlag%>" == "hiddenReceDateFlag")
    initDownOrUp("ReceDateFlag","<%=orderType%>");
  if ("<%=OrderFlag%>" == "hiddenDocSeqFlag")
    initDownOrUp("DocSeqFlag","<%=orderType%>");
  //产生ButtonBar
  ButtonBar_addItem("button_noticeIcon","/FlowEngineWeb/workflow/Images/btn_notice_normal.gif","","","知会"," onclick=\"javascript:buttonItemClick('button_noticeIcon');\" onmouseover=\"javascript:buttonItemOver('button_noticeIcon');\" onmouseout=\"javascript:buttonItemOut('button_noticeIcon');\"","13","70","23");
  <%if(taskType.equals("0")){%>
    ButtonBar_addItem("button_redirectIcon","/FlowEngineWeb/workflow/Images/btn_redirect_normal.gif","","","转交"," onclick=\"javascript:buttonItemClick('button_redirectIcon');\" onmouseover=\"javascript:buttonItemOver('button_redirectIcon');\" onmouseout=\"javascript:buttonItemOut('button_redirectIcon');\"","5","70","23");
  <%}%>
  <%if((!isAdmin) && (taskType.equals("1"))){%>
    ButtonBar_addItem("button_withdrawIcon","/FlowEngineWeb/workflow/Images/btn_back_normal.gif","","","回撤"," onclick=\"javascript:buttonItemClick('button_withdrawIcon');\" onmouseover=\"javascript:buttonItemOver('button_withdrawIcon');\" onmouseout=\"javascript:buttonItemOut('button_withdrawIcon');\"","5","70","23");
  <%}%>
  <%if(taskType.equals("-1") || (taskType.equals("0") && readFlag == 1)){%>
    ButtonBar_addItem("button_deleteIcon","/FlowEngineWeb/workflow/Images/btn_del_normal.gif","","","删除"," onclick=\"javascript:buttonItemClick('button_deleteIcon');\" onmouseover=\"javascript:buttonItemOver('button_deleteIcon');\" onmouseout=\"javascript:buttonItemOut('button_deleteIcon');\"","5","70","23");
  <%}%>
  <%if(taskType.equals("-1")){%>
    ButtonBar_addItem("button_getIcon","/FlowEngineWeb/workflow/Images/btn_get_normal.gif","","","归类"," onclick=\"javascript:buttonItemClick('button_getIcon');\" onmouseover=\"javascript:buttonItemOver('button_getIcon');\" onmouseout=\"javascript:buttonItemOut('button_getIcon');\"","5","70","23");
    //ButtonBar_addItem("button_copyIcon","/FlowEngineWeb/workflow/Images/btn_copy_normal.gif","","","复制"," onclick=\"javascript:buttonItemClick('button_copyIcon');\" onmouseover=\"javascript:buttonItemOver('button_copyIcon');\" onmouseout=\"javascript:buttonItemOut('button_copyIcon');\"","5","70","23");
  <%}%>
  //ButtonBar_addItem("button_scoreIcon","/FlowEngineWeb/workflow/Images/btn_score_normal.gif","","","评价"," onclick=\"javascript:buttonItemClick('button_scoreIcon');\" onmouseover=\"javascript:buttonItemOver('button_scoreIcon');\" onmouseout=\"javascript:buttonItemOut('button_scoreIcon');\"","5","70","23");
  //createButtonBar("buttonUtil");
  //列表的初始设置
  initList();
}
function openPage(sPath){
	var w=window.screen.width-10;
  	var h=window.screen.height-80;
  	var feature = "left=0,top=0,width="+w+",height="+h+",status=no,toolbar=no,menubar=no,location=no,resizable=yes,scrollbars=yes";
  	window.open(sPath,'',feature)
}
</script>
</head>
<style type="text/css">
<!--
.style1 {
	color: #3941AD;
	font-weight: bold;
}
-->
</style>
<body class="listBody" style="overflow:auto;" onload="javascript:initWindow();">
<table id="setTable" width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr><td><span class="style1"><%=userFullName%><%=userPosiName==null?"":userPosiName%>，您好！现有以下事项等待您审批！</span></td></tr>
  <!--开始列表部分-->
  <tr>
    <td>
      <table id="listTable" width="100%" border="0" cellspacing="0" cellpadding="0" class="listTable">
        <tr class="head_Tr">
          <td width="30" class="head_firstTd">
            <input type="checkbox" id="AllorNone" name="AllorNone" title="全选" onclick="javascript:listAllorNone();">
          </td>
          <td width="30" class="head_Td">
            <img  alt="" src="<%=sImagesPath%>search.gif" border="0">
          </td>
          <td width="30" class="head_Td" style="cursor:pointer;" onclick="javascript:DownOrUp('hiddenAwokeFlag','<%=AwokeFlag%>');">
            <img  alt="" src="<%=sImagesPath%>UrgeFile.gif" border="0">
          </td>
          <td width="*" class="head_Td" style="cursor:pointer;" onclick="javascript:DownOrUp('hiddenFinsNameFlag','<%=FinsNameFlag%>');">
            <table width="100%" cellspacing="0" cellpadding="0" border="0">
              <tr>
                <td width="50%" align="right">
                  主题
                </td>
                <td width="40%" align="left">
                  名称
                </td>
                <td width="10%" align="right">
                  <img name="FinsNameFlag" alt="" id="FinsNameFlag" src="<%=sImagesPath%>down.gif" border="0">&nbsp;
                </td>
              </tr>
            </table>
          </td>
          <td width="<%=Per_stepName%>%" class="head_Td" style="cursor:pointer;" onclick="javascript:DownOrUp('hiddenStepNameFlag','<%=StepNameFlag%>');">
            <table width="100%" cellspacing="0" cellpadding="0" border="0">
              <tr>
                <td width="50%" align="right">
                  公文
                </td>
                <td width="40%" align="left">
                  状态
                </td>
                <td width="10%" align="right">
                  <img name="StepNameFlag" alt="" id="StepNameFlag" src="<%=sImagesPath%>down.gif" border="0">&nbsp;
                </td>
              </tr>
            </table>
          </td>
          <%if(!taskType.equals("-1")){%>
          <td width="<%=Per_UserName%>%" class="head_Td" style="cursor:pointer;" onclick="javascript:DownOrUp('hiddenPeriNameFlag','<%=PeriNameFlag%>');">
            <table width="100%" cellspacing="0" cellpadding="0" border="0">
              <tr>
                <td width="50%" align="right">
                  发件
                </td>
                <td width="40%" align="left">
                  人
                </td>
                <td width="10%" align="right">
                  <img name="PeriNameFlag"  alt="" id="PeriNameFlag" src="<%=sImagesPath%>down.gif" border="0">&nbsp;
                </td>
              </tr>
            </table>
          </td>
          <%}%>
          <td width="<%=Per_receDate%>%" class="head_Td" style="cursor:pointer;" onclick="javascript:DownOrUp('hiddenReceDateFlag','<%=ReceDateFlag%>');">
            <table width="100%" cellspacing="0" cellpadding="0" border="0">
              <tr>
                <td width="50%" align="right">
                  接收
                </td>
                <td width="40%" align="left">
                  时间
                </td>
                <td width="10%" align="right">
                  <img name="ReceDateFlag"  alt="" id="ReceDateFlag" src="<%=sImagesPath%>down.gif" border="0">&nbsp;
                </td>
              </tr>
            </table>
          </td>
        </tr>
        <%=sListHtml.substring(0)%>
        <form name="fmMain" method="post" action="TaskListDownApprove.jsp" onsubmit="return isSubmit(<%=pageCount%>);">
        <tr>
          <td colspan="<%=iColSpan%>">
            <table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr class="tail_Tr">
                <td class="tail_firstTd">
                  <span class="ListUtil_inputSpan">第<%=pageNo%>页/共<%=pageCount%>页（共<%=recordCount%>条记录）&nbsp;&nbsp;</span>
                </td>
                <td class="tail_lastTd">
                  <%
                    if(pageNo > 1){
                      out.print("<input type='button' class='standardBtn' value='首  页' onclick='javascript:gotoPage(1);'>&nbsp;&nbsp;");
                      out.print("<input type='button' class='standardBtn' value='上一页' onclick='javascript:gotoPage(" + (pageNo-1) + ");'>&nbsp;&nbsp;");
                    }
                    if(pageCount > 1 && pageNo < pageCount){
                      out.print("<input type='button' class='standardBtn' value='下一页' onclick='javascript:gotoPage(" + (pageNo+1) + ");'>&nbsp;&nbsp;");
                      out.print("<input type='button' class='standardBtn' value='尾  页' onclick='javascript:gotoPage(" + pageCount + ");'>&nbsp;&nbsp;");
                    }
                  %>
                  &nbsp;&nbsp;
                  <span class="ListUtil_inputSpan">
                    查看第
                    <input type="text" name="c_page" class="ListUtil_input">
                    页&nbsp;
                  </span>
                  <input type='button' class='standardBtn' value="查  看" onclick="javascript:gotoPage(null)">
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
                </td>
              </tr>
            </table>
          </td>
        </tr>
        </form>
      </table>
    </td>
  </tr>
   <!--ButtonBar
  <tr>
    <td id="buttonUtil" name="buttonUtil" class="buttonUtil">
    </td>
  </tr>
  -->
  <tr>
	<td>
		<br>
		<br>
		<INPUT name=declare onclick="javascript:openPage('../../../worklist/TaskList.jsp?taskType=1')"; style="BORDER-RIGHT: #033771 1px solid; BORDER-TOP: #033771 1px solid; FONT-SIZE: 9pt; BORDER-LEFT: #033771 1px solid; BORDER-BOTTOM: #033771 1px solid; BACKGROUND-COLOR: #d4e2ef" type=button value="我想查看已审批事项">
	</td>
  </tr>
</table>
<table style="position:absolute;top:0px;left:0px;width:0px;height:0px;" >
  <form id="SubUtilForm" action="">
  <tr>
    <td width="0">
      <iframe id="SubUtil" name="SubUtil" marginWidth="0" marginHeight="0" frameBorder="0" style="position:absolute;top:0px;left:0px;width:0px;height:0px;">
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
<%  }catch(Exception e){
   System.out.println(e.getMessage());
}finally{
  SysPara.closeCachedRowSet(countRs);
  countRs = null;
  SysPara.closeCachedRowSet(taskRs);
  taskRs = null;
  //long endTime = new Date().getTime();
  //System.out.println("打开TaskList为：" + (endTime-startTime));
}
%>
