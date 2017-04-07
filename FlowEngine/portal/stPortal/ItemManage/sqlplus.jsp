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
  String sqlStr="",procedure="";
  String result="";  	//执行结果
  Connection Conn = null;
  Statement st = null;
  ResultSet rs= null;
  BaseDbManage db=new BaseDbManage(request.getRemoteUser());

  //如果页面直接带存储过程的参数 如 sqlplus.jsp?procedure=....
  procedure=request.getParameter("procedure");	//存储过程名字
  if(!procedure.equals("")){
     result=db.sqlPlusProcedure(procedure);
     out.println(result);
  }else{ //如果页面没有参数，就用界面的方式进行
  String sqlType=request.getParameter("sqlplusType");
  if(sqlType.equals("1")){//sql，包括select、update、delete
  	sqlStr=request.getParameter("sqlStr");		//用户帐号
  	if(sqlStr==null) sqlStr="";
  	if(!sqlStr.equals("")){
		result=db.sqlPlusCommit(sqlStr);
  	}
 }
 if(sqlType.equals("2")){//存储过程
  	procedure=request.getParameter("sqlStr");	//存储过程名字
  	if(procedure==null) procedure="";
  	if(!procedure.equals("")){
    		result=db.sqlPlusProcedure(procedure);
  	}
 }

 %>
<html>
<form name="sqlplus" action="sqlplus.jsp" method="POST">
   <table width="95%" border="1" cellspacing="0" cellpadding="2" align="CENTER" bgcolor="" bordercolor="">
     <tr>
	<td>
	<input type="radio" name="sqlplusType" value="1" align="MIDDLE">sql语句
	<input type="radio" name="sqlplusType" value="2" align="MIDDLE">存储过程
	</td>
     </tr>
     <tr>
	<td>
	<input type="text"   name="sqlStr" value="<%=sqlStr%>" size="120"><br>
	<input type="Submit" name="other_funtion" value="提交sql语句" align="MIDDLE">
	<input type="Reset" value="清除" align="MIDDLE">
	</td>
     </tr>
    </table>
</form>
执行结果:<%=result%><br>
</html>
<%}%>