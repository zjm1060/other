<%@ page contentType="text/html; charset=GBK" %>
<%@ page errorPage="../errorPage.jsp" %>
<%@ page import="minstone.ItemManage.BaseDbManage"%>
<%@ page import="java.util.Vector,java.util.Enumeration"%>
<%@ page import="oamodule.publiclib.util.RecordSetObject"%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>查询结果列表</title>
<link rel="stylesheet" type="text/css" href="./Common/Public.css">
</head>
<script>
//查询其他页的数据
function toUrl(num,pagesize,tableName){
    //alert(num);
    window.location.href="queryList.jsp?pageNo="+num+"&pagesize="+pagesize+"&tableName="+tableName;
}
</script>
<%
	request.setCharacterEncoding("GBK");
	BaseDbManage db=new BaseDbManage(request.getRemoteUser());
	String columnPercent ="",columnList="";
	String sTableName=request.getParameter("tableName");
	if(sTableName==null) sTableName="";
	if(!sTableName.equals("")){
	String contentList="";//通用的查询结果
	String actionFlag=request.getParameter("actionFlag");
	if(actionFlag==null) actionFlag="";
	if(actionFlag.equals("DeleteFlag")){
        	System.out.println("删除记录 ");
    	}
        //每页行数为10条
	String sPageSize = request.getParameter("pagesize");
	if(sPageSize==null) sPageSize="0";
	int pageSize=0;
	pageSize=Integer.parseInt(sPageSize);
	//RecordSetObject rsoLiverBed=db.getDocSeq();
	//第几页的页码
        String pageStr=(String)request.getParameter("pageNo");
        if(pageStr==null || pageStr.equals(""))
          pageStr="1";
        int pageNo = java.lang.Integer.parseInt(pageStr);
        if(pageNo==0) pageNo=1;
        //每页行数为10条
        if(pageSize==0)   pageSize = 10;
	//得字段名称
	Vector vTableColumn=new Vector();
        vTableColumn =db.getQueryTable(sTableName);
        int columnSize=vTableColumn.size();
        //得到结果集，并放在数组变量中
        Vector vResult=new Vector();
        vResult=db.getQueryList(sTableName,pageNo,pageSize);
        //计算翻页参数
        int count=Integer.parseInt((String)vResult.elementAt(0));
        int pageCount=(int)((count-1)/pageSize + 1);
%>
<script language="javascript">
function doDelete(workItemCode){
	var truthBeTold = window.confirm("你真的要删除吗? 单击“确定”继续。单击“取消”停止。");
    if(!truthBeTold) return;
	frmQueryList.workItemCode.value=workItemCode;
	frmQueryList.actionFlag.value="DeleteFlag";
	frmQueryList.submit();
}
function doModify(workItemCode){
	frmQueryList.workItemCode.value=workItemCode;
	var strAction="DocSeqUpdate.jsp?workItemCode="+workItemCode;
	frmQueryList.action=strAction;
	frmQueryList.actionFlag.value="UpdateFlag";
	frmQueryList.submit();
}
function openList(flowInid){
	var w=window.screen.width-10;
	var h=window.screen.height-55;
	var sStyle="width="+w+"px,height="+h+"px,left=0px,top=0px,scrollbars=yes,resizable=yes";
	var sUrl="/FlowEngineWeb/workflow/FlowFrame.jsp?action=0&flowInid="+flowInid;
	window.open(sUrl,"",sStyle);
}
</script>
<body bgcolor="#ffffff">
<table width="100%" cellpadding="3" cellspacing="1" class="boarder">
  <tr class="headline-bg">
   <%
    for (int i=0;i<vTableColumn.size();i++) {
        //逐行提取，逐个字段显示
      	columnList =(String)vTableColumn.elementAt(i);
      	int length=columnList.length();
      	if(contentList==null) contentList="";
	else{
	 columnPercent =columnList.substring( length-2,length);
  	 if(db.toInt(columnPercent,0)==0) { //如果没有数字作为百分比，即分配20％
         	columnPercent=columnPercent.valueOf(100/(vTableColumn.size()))+"%" ;
	 	columnList=columnList;
         }else{
		columnPercent=columnPercent+"%";
	 	columnList=columnList.substring(0, length-2);
         }
	}
        //System.out.println(columnPercent+columnList);
   %>
	<td width="<%=columnPercent%>"><div align="center"><strong><%=columnList%></strong></div></td>
   <% }%>
  </tr>
  <%
   //第0个元素是记录条数，所以真正的数据从1开始
    for (int i=1;i<vResult.size();i++) {
      Vector vResultRow=new Vector();
      vResultRow=(Vector)vResult.elementAt(i);	//逐行提取，逐个字段显示
  %>
  <tr class="content-bg">
  	<%
  	  for (int j=0;j<vTableColumn.size();j++) {
      		contentList =(String)vResultRow.elementAt(j);
      		if(contentList==null) contentList="";
  	%>
    <td><a style="cursor:hand;" onclick="javacript:openList()"><%=contentList%></a></td>
    <% }%>
    </tr>
    <%} %>
 <tr class="headline-bg">
    <td colspan="1" align="left">第<%=pageNo%>页/共<%=pageCount%>页（共<%=count%>条记录）</td>
    <td colspan="<%=columnSize-1%>" align="right">
    <%if (pageNo==1){
      out.print("首页 上一页");
      }else{
  %>
  <a href="javascript:toUrl('1','<%=pageSize%>','<%=sTableName%>');" >首页</a>  <a  href="javascript:toUrl('<%=pageNo-1%>','<%=pageSize%>','<%=sTableName%>');">上一页</a>
  <% } %>
  <% if (pageNo+1>pageCount){
       out.print ("下一页 尾页");
     }else{
  %>
  <a  href="javascript:toUrl('<%=pageNo+1%>','<%=pageSize%>','<%=sTableName%>');">下一页</a> <a  href="javascript:toUrl('<%=pageCount%>','<%=pageSize%>','<%=sTableName%>');">尾页</a>
  <% }
  %>
    </td>
    </tr>
</table>
<form name="frmQueryList">
<input type="hidden" name="actionFlag" value="">
</form>
<h1> </h1>
<%} else
	out.print("参数 tableName 不为空");
%>
</body>
</html>

