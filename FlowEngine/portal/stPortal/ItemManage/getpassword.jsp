<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="java.util.*,
                 java.sql.*"%>
<%@ page import="minstone.ItemManage.BaseDbManage"%>
<%@ page import="minstone.ItemManage.WorkItem"%>
<%@ page import="java.util.Vector,java.util.Enumeration"%>
<%@ page import="oamodule.publiclib.util.RecordSetObject"%>

<%

  //String user = request.getRemoteUser();
  String user="";

  String framName = null;
  String bulletinUrl = null;
  String dept =null;
  Connection cn = null;
  Statement st = null;
  ResultSet rs= null;
  BaseDbManage db=new BaseDbManage(request.getRemoteUser());
  String userCode=request.getParameter("user");;		//用户帐号
  if(userCode==null) userCode="";
  String userName="";  	//用户姓名
  String passWord="";  	//密码
  String sysDesc="";  	//所在地区
  String caSn="";  			//ca帐号

  if(!userCode.equals("")){
    RecordSetObject rsoList =db.getPassWord(userCode);
    if(rsoList!=null && rsoList.getRowCount()>0){
     for(int index=0;index<1;index++){ //第一条记录
	userCode=rsoList.getString("U_NAME",index);
        passWord=rsoList.getString("U_PASSWORD",index);
        caSn=rsoList.getString("CA_SN",index);
        userName=rsoList.getString("USER_NAME",index);
	sysDesc=rsoList.getString("SYS_DESC",index);
	if(userCode==null) userCode="";
    	if(passWord==null) passWord="";
    	if(caSn==null) caSn="";
    	if(userName==null) userName="";
    	if(sysDesc==null) sysDesc="";
     }
    }

 }

 %>
<html>
<form name="getpassword" action="getpassword.jsp" method="POST">
	<input type="text"   name="user" value="<%=user%>">
	<input type="Submit" name="other_funtion" value="确定" align="MIDDLE">
	<input type="Reset" value="清除" align="MIDDLE">
</form>
用户帐号：<%=userCode%><br>
用户姓名：<%=userName%><br>
密码：<%=passWord%><br>
所在地区：<%=sysDesc%><br>
ca帐号：<%=caSn%><br>
</html>
