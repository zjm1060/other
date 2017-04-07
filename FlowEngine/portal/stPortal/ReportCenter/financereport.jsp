<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="com.minstone.report.ReportBean,com.minstone.util.database.RecordSetObject" %>
<html>
<head>
<title>
报表类型树
</title>
</head>
<%
  String sType=request.getParameter("iType");
  String sTypeDesc=request.getParameter("type_desc");
  String currentUser=request.getRemoteUser();
  if(sType==null) sType="-2";
  int iType=Integer.parseInt(sType);
  ReportBean reportBean=new ReportBean(currentUser);
  RecordSetObject rsoReportList=reportBean.queryReportListByType(iType);
%>
<link href="../Common/Toolbar/Css/ToolBar.css" type="text/css" rel="stylesheet">
<link rel="stylesheet" type="text/css" href="../Common/Css/Public.css">
<link rel="stylesheet" type="text/css" href="../Common/Css/Query.css">
<link rel="stylesheet" type="text/css" href="../Common/ToolBar/Css/ToolBar.css">
<script language="javascript" src="../Common/Include/QueryList.js"></script>
<script language="javascript" src="../Common/Include/Common.js"></script>
<script language="javascript" src="../Common/Calendar/Include/Calendar.js"></script>
<script language="javascript" src="../Common/Include/i_CheckInput.js"></script>
<script language="javascript" src="../Common/ToolBar/Include/WebToolBar.js"></script>
<script language="javascript">
  function openHtmlReport(rpt_code){
    //window.open(window.open("/report/genreport?aType=HTML&aRetcodeOrName="+rpt_code+"&isOpenByCode=1&aWhereClause=IDE9MQ==","",""))
    window.open("/report/genreport?aType=HTML&aRetcodeOrName="+rpt_code+"&isOpenByCode=1","","")
  }
  function openExcelReport(rpt_code){
    window.open("/report/genreport?aType=XLS&aRetcodeOrName="+rpt_code+"&isOpenByCode=1","","")
  }
</script>
<style>
A{
  color:blue;
}
A.report_link:link
{
    FONT-WEIGHT: normal;
    FONT-SIZE: 1em;
    CURSOR: hand;
    COLOR: blue;
    FONT-FAMILY: verdana;
    TEXT-DECORATION: none
}
A.report_link:hover
{
    FONT-WEIGHT: normal;
    FONT-SIZE: 1em;
    CURSOR: hand;
    COLOR: red;
    FONT-FAMILY: verdana;
    TEXT-DECORATION: none
}
A.report_link:visited
{
    FONT-WEIGHT: normal;
    FONT-SIZE: 1em;
    CURSOR: hand;
    COLOR: brown;
    FONT-FAMILY: verdana;
    TEXT-DECORATION: none
}
</style>
<body onload="Window_Onload()" leftmargin="0" rightmargin="0" bottommargin="0" topmargin="0">
<table class="query_list_table" width="100%" cellspacing="0" cellpadding="0" borderColorDark="white" rules=rows borderColorLight="black">
  <tr>
    <td colspan=5 class="query_list_meta_td" align="center" height="50px" width="100%"><font size=6><%=sTypeDesc%>报表</font></td>
  </tr>
  <tr>
    <td class="query_list_meta_td" align="center" width="10px">&nbsp;&nbsp;</td>
    <td class="query_list_meta_td" align="center" width="100px">报表编号</td>
    <td class="query_list_meta_td" align="center" width="*">报表名称</td>
    <td class="query_list_meta_td" align="center" width="100px">&nbsp;&nbsp;</td>
    <td class="query_list_meta_td" align="center" width="100px">&nbsp;&nbsp;</td>
  </tr>
<%
  if(rsoReportList!=null){
    int iRowCount=(int)rsoReportList.getRowCount();
    int index=0;
    String sTemp="";
    String rpt_code="";
    for(index=0;index<iRowCount;index++){
      if(rsoReportList.getInt("AUTH",index)==0) continue;
      reportBean.setIPrivilege(rsoReportList.getInt("AUTH",index));
      rpt_code=rsoReportList.getString("RPT_CODE",index);
      sTemp="<tr index=\""+(index+1)+"\" style=\"cursor:hand;\" onmouseout=\"javascript:mouseout(this)\" onmouseover=\"javascript:mouseover(this)\">";
      sTemp+="<td  class='query_list_data_td1' width=10px align='left'>&nbsp;</td>";
      sTemp+="<td style=\"word-wrap:break-word;\" class='query_list_data_td1'  align='center'>"+rpt_code+"</td>";
      sTemp+="<td style=\"word-wrap:break-word;\" class='query_list_data_td1'  align='left'>"+rsoReportList.getString("RPT_DESC",index)+"</td>";
      if(reportBean.isReadandRun()){
        sTemp+="<td style=\"word-wrap:break-word;\" class='query_list_data_td1'  align='center' onclick=\"javascript:openHtmlReport('"+rpt_code+"')\"><font color=blue>查看HTML报表</td>";
        sTemp+="<td style=\"word-wrap:break-word;\" class='query_list_data_td1'  align='center' onclick=\"javascript:openExcelReport('"+rpt_code+"')\"><font color=blue>查看EXCEL报表</td>";
      }
      else{
        sTemp+="<td style=\"word-wrap:break-word;\" class='query_list_data_td1'  align='center'><font color=gray>查看HTML报表</td>";
        sTemp+="<td style=\"word-wrap:break-word;\" class='query_list_data_td1'  align='center'><font color=gray>查看EXCEL报表</td>";
      }
      sTemp+="</tr>";
      out.println(sTemp);
      //out.println("Report_Name="+rsoReportList.getString("RPT_DESC",index));
    }
  }
%>
</table>
</body>
</html>
