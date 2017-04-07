<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="sun.jdbc.rowset.CachedRowSet" %>
<%@ page import="flowengineforjava.util.BaseToolJB" %>
<%@ page import="com.minstone.util.PublicLib" %>
<%@ page import="com.minstone.util.SQLPara" %>
<%@ page import="baseserverjb.BaseServerClient,com.minstone.util.Base64Encoder" %>
<%@ page import="java.sql.*" %>
<%
//取得路径变量
String userCode=request.getParameter("user_code");
if(userCode==null) userCode="";
%>
<%
//强制页面刷新
response.setHeader("Pragma","No-cache");
response.setHeader("Cache-Control","no-cache");
response.setDateHeader("Expires", 0);


String currentUser = userCode;//request.getRemoteUser();
if (currentUser == null) currentUser = "";
String userName = "Web";
//System.out.println("当前用户：" + currentUser);

Connection cn = null;
CachedRowSet rs = null;
String recordCount="0";
BaseServerClient bsc=new BaseServerClient(currentUser);
try {
  userName=bsc.getFramework().getUserNameFromWizard(currentUser);//取用户中文名称
  String sql = "SELECT count(*) as myCount"
  + " FROM RECI_ENTR_ALL "
  + " WHERE PIIS_TERM = 0 "
  + " 	AND ENTR_STAT = 1 "
  + " 	AND ENTR_ALCO = 0 "
  + " 	AND 1=1 "
  + " 	AND ( ENTR_RECI ='" + userName + "' "
  + " 	  OR EXISTS ( SELECT 1 FROM USER_DATA , SYS_USER "
  + " 	   WHERE RECI_ENTR_ALL.DATA_REFC = USER_DATA.DATA_ID "
  + " 	   AND PROX_USER IN ('" + currentUser + "') "
  + " 	   AND USER_DATA.USER_CODE = SYS_USER.USER_CODE "
  + " 	   AND RECI_ENTR_ALL.ENTR_RECI = SYS_USER.USER_NAME) ) ";

  //System.out.println(userName+"sql语句:"+sql);
  cn =bsc.getDataSource("exoa_ds").getConnection();
  rs = bsc.getRowSet(sql,cn,new SQLPara());
  while (rs.next()) {
    recordCount = rs.getString("myCount");
    if (recordCount == null) recordCount = "0";
  }
}
catch (Exception e) {
  e.printStackTrace();
  System.out.println("获取ODBC数据错误,详细信息为:" + e.getMessage());
}
finally {
  PublicLib.closeCachedRowSet(rs);
  rs=null;
  PublicLib.closeConnection(cn);
  cn=null;
}

out.println("<?xml version=\"1.0\" encoding=\"GB2312\"?>");
out.println("<TaskAwake>");
out.println("\t<TaskList>");
out.println("\t\t<TaskListCount>"+recordCount+"</TaskListCount>");
out.println("\t</TaskList>");
out.println("</TaskAwake>");
%>
