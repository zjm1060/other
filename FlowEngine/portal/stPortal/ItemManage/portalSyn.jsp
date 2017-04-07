<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="java.util.*,
                 java.sql.*"%>
<%@ page import="minstone.ItemManage.BaseDbManage"%>

<%

  //String user = request.getRemoteUser();
  String user="";

  String framName = null;
  String bulletinUrl = null;
  String dept =null;
  Connection Conn = null;
  Statement st = null;
  ResultSet rs= null;

  String sqlStr=request.getParameter("sqlStr");;		//用户帐号
  if(sqlStr==null) sqlStr="";
  String result="";  	//执行结果
  if(!sqlStr.equals("")){
	BaseDbManage db=new BaseDbManage(request.getRemoteUser());
    	result=db.sqlPlusCommit(sqlStr);
 }

 %>
<html>
<form name="portalSyn_hs" action="portalSyn.jsp" method="POST">
	<%
		sqlStr="delete from user_modu";
	%>
  <input type="hidden" name="sqlStr" value="<%=sqlStr%>" size="220">
  <br>
	&nbsp;&nbsp;&nbsp;&nbsp;目前系统portal栏目的调整是个性化的，如果系统管理员调整了portal的栏目，应该进行同步，其他用户才与系统管理人员的栏目一致。
	<br>&nbsp;&nbsp;&nbsp;
	<input type="Submit" name="other_funtion" value="开始同步" align="MIDDLE">
</form>
执行结果:<%=result%><br>
</html>
