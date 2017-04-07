<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="oamodule.report.ReportBean" %>
<%@ page import="oamodule.publiclib.*"%>
<%@ page import="oamodule.report.Base64Encoder" %>
<%@ page import="java.util.Date" %>

<html>
<head>
<title>条件选择的报表</title>
</head>
<%
  String sAction=request.getParameter("actionType");
  String sOpenInNew=request.getParameter("cbOpenInNew");
  String cbUnitAll=request.getParameter("cbUnitAll");
  if(sOpenInNew==null) sOpenInNew="0";
  boolean isOpenInNewWin=(Integer.parseInt(sOpenInNew)==1);
  if(sAction==null) sAction="";
  if(sAction.equals("")){
    String sType=request.getParameter("iType");
    String sTypeDesc=request.getParameter("type_desc");
    String sReportCode=request.getParameter("rpt_code");
    String currentUser=request.getRemoteUser();
    if(sType==null) sType="-2";
    int iType=Integer.parseInt(sType);
    ReportBean reportBean=new ReportBean(currentUser);

%>
<script language="javascript" src="./ReportCenter/include/ReportFunction.js"></script>
<script language="javascript" src="../Common/Calendar/Include/Calendar.js"></script>
<link rel="stylesheet" type="text/css" href="../Common/Css/Public.css">
<style>
.Btn_common{
	height:20px;
	padding-top:1px;
	padding-left: 1px;
	border:1px solid #000000;
	text-align:center;
	cursor:hand;
	font-size: 9pt;
}
.Text_input{
 border:1px #859bb5 solid;
}
</style>
<script language="javascript">
  function openReport(){
    var iGenType=getRadioSelectedValue("genType");
    if(iGenType=="1")
      openExcelReport("<%=sReportCode%>");
    else
      openHtmlReport("<%=sReportCode%>");
  }
  function getRadioSelectedValue(name){
    var radios=document.all.item(name);
    var oValue=null;
    if(radios!=null){
      if(radios.length>0){
        for(i=0;i<radios.length;i++){
          if(radios[i].checked){
            oValue=radios[i].value;
            break;
          }
        }
      }
      else{
        if(radios.checked)
          oValue=radios.value;
      }
    }
    return oValue;
  }
//日期选择
function beforeSelectDate(id)
{
  var calPath ="../Common/Calendar/Include/calendar1.htm";
  calendar(id,calPath);
}
//是否全选
function ifSelectAll(id)
{ var unit=document.all.item("cbUnit");
  if(id.checked)
    //alert("全选");
    for(i=0;i<unit.length;i++){
      unit[i].checked=true;
    }
  else
    //alert("不全选");
    for(i=0;i<unit.length;i++){
      unit[i].checked=false;
    }
}
//选择年、月
  function setDateSelect() {
    dNow = new Date();
    iScrap = dNow.getFullYear();
    iMonth =dNow.getMonth();
    
    //清空原来的设置
    queryInputForm.year.options.length=0;
    queryInputForm.month.options.length=0; 
    for (i=-20; i<10; i++)
    {
      objNewOpt = document.createElement("OPTION");
      objNewOpt.text = (iScrap - i);
      objNewOpt.value = (iScrap - i);
      if(i==0) objNewOpt.selected=true;
      queryInputForm.year.options.add(objNewOpt);
    }
    for(i=0;i<=11;i++)
    {
      objNewOpt = document.createElement("OPTION");
      objNewOpt.text = (i+1);
      objNewOpt.value = (i+1);
      if(i==iMonth+0) objNewOpt.selected=true;
      queryInputForm.month.options.add(objNewOpt);
    }
    for(i=1;i<=4;i++)
    {
      objNewOpt = document.createElement("OPTION");
      objNewOpt.text = (i);
      objNewOpt.value = (i);
      //alert((dNow.getMonth()+1)%4+1);
      if(i==(dNow.getMonth()+1)%4+1) objNewOpt.selected=true;
      queryInputForm.quarter.options.add(objNewOpt);
    }
  }
  //选择季度
function  selQuarter(){
  var quarter=document.all.item("quarter").value;
  var nowdate=new Date();

  if(quarter!=null){
  	document.all.item("dateFrom").value=nowdate.getFullYear()+"-"+formatMonth((quarter*3-2))+"-1";
  	document.all.item("dateTo").value=nowdate.getFullYear()+"-"+formatMonth((quarter*3))+"-31";
  }	
}
//根据季度格式化月份
function formatMonth(value){
	var mystr=value+"";
	//alert(mystr.length);
	if(mystr.length<2)
	  mystr="0"+mystr;
	return mystr;
}
//选择时间类型
function selGenType(value){
	if(value=="2")
		selQuarter();
	else{
		document.all.item("dateFrom").value="";
  	document.all.item("dateTo").value="";
	}
}
</script>
<body leftmargin="0" rightmargin="0" bottommargin="0" topmargin="0" onload="setDateSelect()">
  <form id="queryInputForm" action="conSMS.jsp" method="POST">
    <table width="99%" height="100%" border="0" align="center">
      <tr>
      <td width="100%" height="100%"><div align="center"><font size="6"><strong>短信统计查询</strong></font></div>
        <br>
        <fieldset>
        <legend>请输入查询条件：</legend>
        <table width="100%" border="0" cellspacing="1"  id="whereClause"> 
          <tr ><tr >
            	<td align="right" rowspan='2' bgcolor="#CCCC99"><B></B>
	        	  </td>
	            <td align="left" bgcolor="#CCCCCC">方式:
                <input type="radio" name="genDateType" checked value="" onclick="selGenType('')">不限时间
                  <input type="radio" name="genDateType" value="0" onclick="selGenType('0')">年
                <input type="radio" name="genDateType" value="1" onclick="selGenType('1')">月
                <input type="radio" name="genDateType" value="2" onclick="selGenType('2')">季度
                <input type="radio" name="genDateType" value="3" onclick="selGenType('3')">年月日
                </td>
              </tr>
              <tr>
	            <td align="left" bgcolor="#CCCCCC">
	            	年份<select  name="year" >
	            	</select>
	            	月份<select  name="month" >
	            	</select>
	            	季度<select  name="quarter" OnChange="selQuarter()" >
	            	</select>
	             </td>
          </tr>
          <tr>
            <td align="right" bgcolor="#CCCC99"><strong>发送时间:</strong></td>
            <td align="center" bgcolor="#CCCCCC">
              <div align="left"><strong>从</strong>
                <input readonly name="dateFrom" type="text" id="dateFrom" class="AreaUtil_input" onClick="javascript:beforeSelectDate(this);">
                <b>到</b>
                <input readonly name="dateTo" type="text" id="dateTo" class="AreaUtil_input" onClick="javascript:beforeSelectDate(this);">
              </div></td>
          </tr>
          <tr>
            <td align="right" bgcolor="#CCCC99"><B>接收状态:</B> </td>
            <td align="left" bgcolor="#CCCCCC">
              <input type="radio" name="sendStat" checked value="">全部
              <input type="radio" name="sendStat" value="0">待发
              <input type="radio" name="sendStat" value="1">成功
              <input type="radio" name="sendStat" value="2">失败
              <input type="radio" name="sendStat" value="3">号码空或者有误
            </td>
          </tr>
          <tr>
            <td align="right" bgcolor="#CCCC99"><B>短信内容:</B> </td>
            <td align="left" bgcolor="#CCCCCC">
              <input type="text" name="smsContent" value="" style="width:100%" class="Text_input">
            </td>
          </tr>
          <tr>
            <td align="right" bgcolor="#CCCC99"><B>生成类型:</B> </td>
            <td align="left" bgcolor="#CCCCCC">
              <input type="radio" name="genType" checked value="0">
              HTML格式
              <input type="radio" name="genType" value="1">
              EXCEL格式 </td>
          </tr>
          <tr>
            <td align="right" colspan=2><input name="cbOpenInNew" type="checkbox" id="cbOpenInNew" value="1">
              在新窗口打开报表</td>
          </tr>
          <tr>
            <td align="right" colspan=2> <br> <input type="hidden" name="actionType" value="openReport">
              <input type="hidden" name="rpt_code" value="<%=sReportCode%>"> 
              <input type="submit" class="Btn_common" name="btnOK" value="确定" id="btnOK">
              &nbsp; <input type="reset" class="Btn_common" name="btnCancel" value="重填" id="btnCancel">
              <br> </td>
          </tr>
        </table>
        </fieldset>
      </td>
      <tr>
    </table>
  </form>
</body>
</html>
<%}else{%>
<script language="javascript">
  function openHtmlReport(rpt_code,sWhere){
    <%if(isOpenInNewWin){%>
    window.open("/report/genreport?aType=HTML&aRetcodeOrName="+rpt_code+"&isOpenByCode=1&aWhereClause="+sWhere,"","");
    <%}else{%>
	document.location="/report/genreport?aType=HTML&aRetcodeOrName="+rpt_code+"&isOpenByCode=1&aWhereClause="+sWhere;
    <%}%>
  }
  function openExcelReport(rpt_code,sWhere){
    <%if(isOpenInNewWin){%>
    window.open("/report/genreport?aType=XLS&aRetcodeOrName="+rpt_code+"&isOpenByCode=1&aWhereClause="+sWhere,"","");
    <%}else{%>
	document.location="/report/genreport?aType=XLS&aRetcodeOrName="+rpt_code+"&isOpenByCode=1&aWhereClause="+sWhere;
    <%}%>
  }
</script>
<%
  String sShowType=request.getParameter("showType");
  String sRptCode=request.getParameter("rpt_code");
  String sGenType=request.getParameter("genType");
  String sSmsContent=request.getParameter("smsContent");
  String sSendStat=request.getParameter("sendStat");
  String[] sUnit=request.getParameterValues("cbUnit");
  
  String sDateyear=request.getParameter("year");  	//年
  String sDatemonth=request.getParameter("month");	//月      
  String dateFrom=request.getParameter("dateFrom"); //季度与年月日
  String dateTo=request.getParameter("dateTo");
  
  String sDateType=request.getParameter("genDateType");
  if(sDateType.equals("0")){
  	dateFrom=sDateyear+"-"+"1-1";
  	dateTo=sDateyear+"-"+"12-31";
  }else  if(sDateType.equals("1")){
    Date sDObj = new Date();
    int iYear=sDObj.getYear()+1900;
    String sYear="";
  	if(sDateyear==null || sDateyear.equals("")){
  		dateFrom=sYear.valueOf(iYear)+"-"+sDatemonth+"-1";//默认为当年 
  		dateTo=sYear.valueOf(iYear)+"-"+sDatemonth+"-31";
  	}else{
			dateFrom=sDateyear+"-"+sDatemonth+"-1";
			dateTo=sDateyear+"-"+sDatemonth+"-31";
  	}
  }
  
  String sWhere="(1=1)";
  String sTemp="";
  String currentUser=request.getRemoteUser();
  Base64Encoder base64=new Base64Encoder();
  //处理单位条件
  if(sUnit!=null && sUnit.length>0){
    if(sWhere.equals("(1=1)")) sWhere="";
    StringBuffer unitList=new StringBuffer("");
    for(int i=0;i<sUnit.length;i++){
		if(i==0)
        	unitList.append("'").append(sUnit[i]).append("'");
		else
			unitList.append(",'").append(sUnit[i]).append("'");
    }
    sTemp=unitList.toString();
    if(sTemp.length()>0){
      if(sWhere.equals(""))
		sWhere="UNIT_CODE IN("+sTemp+")";
      else
        sWhere=sWhere +" AND UNIT_CODE IN("+sTemp+")";
    }
  }
  //短信内容
  if(sSmsContent==null) sSmsContent="";
  if(!sSmsContent.trim().equals("")){
    sWhere=sWhere +" AND sms_msg like '%"+sSmsContent+"%'";
  }
  
  //接收状态
  if(sSendStat==null) sSendStat="";
  if(!sSendStat.trim().equals("")){
    sWhere=sWhere +" AND send_stat = '"+sSendStat+"'";
  }
  //发送人
  if(!DBService.isAdmin(currentUser))
   sWhere=sWhere +" AND send_user = '"+currentUser+"'";
  
  //处理日期条件
  if(dateFrom==null) dateFrom="";
  if(dateTo==null) dateTo="";
  sTemp="";
  if(!dateFrom.trim().equals("") ){
	dateFrom=java.sql.Date.valueOf(dateFrom)+"";
    sTemp="to_char(SEND_TIME,'YYYY-MM-DD')>='"+dateFrom+"'";
  }
 //结束的日期
  if(!dateTo.trim().equals("") ){
	   //验证是否有这天为本月最后一天
     String tempdateTo=tempdateTo=java.sql.Date.valueOf(dateFrom)+"";
     String dateTo1=dateTo.substring(0,dateTo.lastIndexOf("-")+1);
     String dateTo11=dateTo.substring(dateTo.lastIndexOf("-")+1,dateTo.length());
     String temp="",dateTo2="";
     if(dateTo11.length()==2){ //如果最后的日期不是2位，就不要判断
	     for(int i=31;i>=28;--i){	
	        dateTo2=dateTo1+java.lang.String.valueOf(i);
					temp =java.sql.Date.valueOf(dateTo2)+"";
					if(dateTo2.equals(temp)) {
						dateTo=dateTo2;
						break;
					}
	     }
     }    
    dateTo=java.sql.Date.valueOf(dateTo)+"";  //转换为"yyyy-mm-dd"格式
    if(sTemp.trim().equals(""))
    	sTemp="to_char(SEND_TIME,'YYYY-MM-DD')<='"+dateTo+"'";
    else
		sTemp+=" AND to_char(SEND_TIME,'YYYY-MM-DD')<='"+dateTo+"'";
  }
  if(!sTemp.equals("")){
  	if(sWhere.equals(""))
  		sWhere="("+sTemp+")";
  	else
    	sWhere=sWhere+" AND ("+sTemp+")";
  }
  System.err.println("sWhere="+sWhere);
  sWhere=base64.encode(sWhere);
  //System.err.println("sWhere="+sWhere);
  base64=null;
%>
  <table width="100%" height="100%" border="0">
    <tr>
      <td align="center"><b><font color="red" size="6">正在生成报表请稍候......</font></b></td>
    </tr>
  </table>
<%
  out.println("<script language=\"javascript\">");
  if(sGenType.equals("0")){
    out.println("openHtmlReport('"+sRptCode+"','"+sWhere+"')");
  }
  else{
    out.println("openExcelReport('"+sRptCode+"','"+sWhere+"')");
  }
  out.println("</script>");
%>
  </body>
  </html>
<%}%>
