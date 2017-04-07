<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="com.minstone.report.ReportBean" %>
<html>
<head>
<title>
报表类型树
</title>
</head>
<%
  String currentUser=request.getRemoteUser();
  ReportBean reportBean=new ReportBean(currentUser);
  String sTypeTree=reportBean.buildReportTypeTree();
%>
<link href="../Common/Toolbar/Css/ToolBar.css" type="text/css" rel="stylesheet">
<link rel="stylesheet" type="text/css" href="../Common/Css/Public.css">
<link rel="stylesheet" type="text/css" href="../Common/Css/Query.css">
<style>
.TreeNode
{
    FONT-SIZE: 1.2em;
    COLOR: black;
    FONT-FAMILY: 宋体;
}
.TreeNodeSelected
{
    FONT-WEIGHT: bolder;
    FONT-SIZE: 1.2em;
    COLOR: black;
    FONT-FAMILY: 宋体;
}
</style>
<script language="javascript" src="../Common/Toolbar/WebToolBar/WebToolBar.js"></script>
<script language="javascript" src="../Common/Include/Comon.js"></script>
<script language="javascript" src="./Tree/tTree.js"></script>
<script language="javascript" src="./Tree/ttreeData.js"></script>
<script language="javascript">
  var oTree=new tTree();
  var ifExpandTree=1;
  <%=sTypeTree%>
  function genTree(){
    var treeContent=oTree.RenderTable(ifExpandTree);
    document.all.item("divReportTypeTree").innerHTML=treeContent;
  }
  function tNodeOnClick()
  {
	var type_code;
        var type_desc;
        var conditionLink;
        var nodetype="0";
	if( StrLastNode != "" )
		document.all.item(StrLastNode).className = "TreeNode";
	StrLastNode=this.nodeName;
	document.all.item(StrLastNode).className = "TreeNodeSelected";
        type_code = this.array.Find("cate_value");
        type_desc = this.array.Find("cate_desc");
        conditionLink=this.array.Find("cate_link");
        nodetype=this.array.Find("cate_type");
        if(nodetype=="1"){
          //alert("Link="+conditionLink+"  node_type="+nodetype+" report_code="+type_code);
          if(conditionLink!=null && conditionLink!="")
            redirectTo(conditionLink,"rpt_code="+type_code);
          else
            openHtmlReport(type_code);
          return;
        }
        else{
          queryReportList(type_code,type_desc);
        }
  }
  <!--
  function displayConditionDlg(id)
  {
  	src = "ShowRptCondition.jsp?rpt_code=" + id;
  	returnV=window.showModalDialog(src,window,"dialogHeight:400px;dialogWidth:620px;status:no;resizable:no;scroll:auto");
  	return returnV;
  }
  //-->
  function openHtmlReport(rpt_code){
    //request.getContextPath()
    //window.open("/report/genreport?aType=HTML&aRetcodeOrName="+rpt_code+"&isOpenByCode=1","","");
	var sWhere=displayConditionDlg(rpt_code);
    if(sWhere!=null && sWhere!=""){
    	window.parent.frames("ReportCenterMain").location="../report/genreport?aType=HTML&aRetcodeOrName="+rpt_code+"&isOpenByCode=1&aWhereClause="+sWhere;
    }
	else{
		window.parent.frames("ReportCenterMain").location="../report/genreport?aType=HTML&aRetcodeOrName="+rpt_code+"&isOpenByCode=1";
	}

  }
  function openExcelReport(rpt_code){
    //window.open("/report/genreport?aType=XLS&aRetcodeOrName="+rpt_code+"&isOpenByCode=1","","");
	var sWhere=displayConditionDlg(rpt_code);
	if(sWhere!=null && sWhere!=""){
    	window.parent.frames("ReportCenterMain").location="../report/genreport?aType=XLS&aRetcodeOrName="+rpt_code+"&isOpenByCode=1&aWhereClause="+sWhere;
	}
	else{
		window.parent.frames("ReportCenterMain").location="../report/genreport?aType=XLS&aRetcodeOrName="+rpt_code+"&isOpenByCode=1";
	}
  }
  function queryReportList(type,type_desc){
    window.parent.frames("ReportCenterMain").location="reportlist.jsp?iType="+type+"&type_desc="+type_desc;
  }
  function redirectTo(url,para){
    window.parent.frames("ReportCenterMain").location=url+"?"+para;
  }
</script>
<body onload="genTree()">
<div id="divReportTypeTree"></div>
</body>
</html>
