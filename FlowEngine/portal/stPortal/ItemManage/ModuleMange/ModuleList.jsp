<%@ page contentType="text/html; charset=GBK" %>
<%@ page errorPage="../errorPage.jsp" %>
<%@ page import="minstone.ItemManage.BaseDbManage"%>
<%@ page import="minstone.ItemManage.WorkItem"%>
<%@ page import="oamodule.publiclib.util.RecordSetObject"%>
<%@ page import="oamodule.publiclib.util.RecordSetObject"%>
<%@ page import="java.util.*"%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>模块名称列表</title>
<link rel="stylesheet" type="text/css" href="../Common/Public.css">
</head>
<script>
//查询其他页的数据
function toUrl(num,pagesize){
    //alert(num);
    window.location.href="ModuleList.jsp?pageNo="+num+"&pagesize="+pagesize;
}
</script>
<%
	request.setCharacterEncoding("GBK");
	BaseDbManage db=new BaseDbManage(request.getRemoteUser());
	String workItemCode="";
	String workItemName="";
	String workItemNo="";
	String workItemDesc="";
	String workItemValid="";

	String actionFlag=request.getParameter("actionFlag");
	workItemCode=request.getParameter("workItemCode");
	workItemNo=request.getParameter("workItemNo");
	workItemName=request.getParameter("workItemName");
	workItemDesc=request.getParameter("workItemDesc");
        //每页行数为10条
	String sPageSize = request.getParameter("pagesize");
	if(sPageSize==null) sPageSize="0";
	int pageSize=0;
	pageSize=Integer.parseInt(sPageSize);

	if(actionFlag==null) actionFlag="";
	if(actionFlag.equals("DeleteFlag")){
        	db.delModuleType(workItemCode);
    	}
        //RecordSetObject rsoLiverBed=db.getModuleType();
	//第几页的页码
        String pageStr=(String)request.getParameter("pageNo");
        if(pageStr==null || pageStr.equals(""))
          pageStr="1";
        int pageNo = java.lang.Integer.parseInt(pageStr);
        if(pageNo==0) pageNo=1;
        //每页行数为10条
        if(pageSize==0)   pageSize = 10;
	//得到结果集，并放在数组变量中
        Vector vResult=new Vector();
        vResult=db.getModuleTypeList(pageNo,pageSize);
        //计算翻页参数
        int count=Integer.parseInt((String)vResult.elementAt(0));
        int pageCount=(int)((count-1)/pageSize + 1);
%>
<script language="javascript">
function doDelete(workItemCode){
	var truthBeTold = window.confirm("你真的要删除吗? 单击“确定”继续。单击“取消”停止。");
    if(!truthBeTold) return;
	frmForAction.workItemCode.value=workItemCode;
	frmForAction.actionFlag.value="DeleteFlag";
	frmForAction.submit();
}
function doModify(workItemCode,workItemName,workItemNo,workItemDesc){
	frmForAction.workItemCode.value=workItemCode;
	frmForAction.workItemName.value=workItemName;
	frmForAction.workItemNo.value=workItemNo;
	frmForAction.workItemDesc.value=workItemDesc;
	var strAction="ModuleUpdate.jsp?workItemCode="+workItemCode+"&workItemName="+workItemName+"&workItemDesc="+workItemDesc+"&workItemNo="+workItemNo;
	frmForAction.action=strAction;
	frmForAction.actionFlag.value="UpdateFlag";
	frmForAction.submit();

}
</script>
<body bgcolor="#ffffff">
<%//if(db.isAdmin()){%>
<p class="Btn_common2"><a href="ModuleNew.jsp">新增模块</a></p>
<%//}%>
<table width="100%" cellpadding="3" cellspacing="1" class="boarder">
  <tr class="headline-bg">
    <td width="9%"><div align="center"><strong>序列</strong></div></td>
    <td width="20%"><strong>模块名称</strong></td>
    <td width="10%"><strong>模块编号</strong></td>
    <td width="*"><strong>模块描述</strong></td>
    <td width="11%"><strong>是否生效</strong></td>
    <td width="5%">&nbsp;</td>
    <td width="5%">&nbsp;</td>
  </tr>
  <%
   //第0个元素是记录条数，所以真正的数据从1开始
    for (int i=1;i<vResult.size();i++) {
      Vector vResultRow=new Vector();
      vResultRow=(Vector)vResult.elementAt(i);	//逐行提取，逐个字段显示
      	workItemCode =(String)vResultRow.elementAt(0);
      	workItemName   =(String)vResultRow.elementAt(1);
      	workItemNo   =(String)vResultRow.elementAt(2);
        workItemDesc   =(String)vResultRow.elementAt(3);
      	workItemValid   =(String)vResultRow.elementAt(4);
      	if(workItemCode==null) workItemCode="";
	if(workItemName==null) workItemName="";
	if(workItemNo==null) workItemNo="";
	if(workItemDesc==null) workItemDesc="";
	if(workItemValid==null) workItemValid="1";
  %>
  <tr class="content-bg">
    <td><%=workItemCode%></td>
    <td><%=workItemName%></td>
    <td><%=workItemNo%></td>
    <td><%=workItemDesc%></td>
    <td>
      <%if(workItemValid.equals("1")){
      		out.print("是");}
      	else{
      		out.print("否");}
      %>
    </td>
    <td><div align="center"><img onclick="javascript:doModify('<%=workItemCode%>','<%=workItemName%>','<%=workItemNo%>','<%=workItemDesc%>')" style="cursor:hand;" src="../Common/Images/icon_urge.gif" width="16" height="16" alt="修改"></div></td>
    <td><div align="center"><img onclick="javascript:doDelete('<%=workItemCode%>')" style="cursor:hand;" src="../Common/Images/del_flag.gif" width="16" height="16" alt="删除"></div></td>
  </tr>
  <%} %>
 <tr class="headline-bg">
    <td colspan="2" align="left">第<%=pageNo%>页/共<%=pageCount%>页（共<%=count%>条记录）</td>
    <td colspan="6" align="right">
    <%if (pageNo==1){
      out.print("首页 上一页");
      }else{
  %>
  <a href="javascript:toUrl('1','<%=pageSize%>');" >首页</a>  <a  href="javascript:toUrl('<%=pageNo-1%>','<%=pageSize%>');">上一页</a>
  <% } %>
  <% if (pageNo+1>pageCount){
       out.print ("下一页 尾页");
     }else{
  %>
  <a  href="javascript:toUrl('<%=pageNo+1%>','<%=pageSize%>');">下一页</a> <a  href="javascript:toUrl('<%=pageCount%>','<%=pageSize%>');">尾页</a>
  <% }
  %>
    </td>
    </tr>
</table>
<form name="frmForAction">
<input type="hidden" name="workItemCode" value="">
<input type="hidden" name="workItemName" value="">
<input type="hidden" name="workItemNo" value="">
<input type="hidden" name="workItemDesc" value="">
<input type="hidden" name="actionFlag" value="">
</form>
<h1> </h1>
</body>
</html>

