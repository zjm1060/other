<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="sun.jdbc.rowset.CachedRowSet" %>
<%@ page import="flowengineforjava.util.BaseToolJB" %>
<%@ page import="com.minstone.util.PublicLib" %>
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
  Statement st = null;
  ResultSet rs = null;
  String recordCount="0";

  try {//取用户中文名称
    cn = BaseServerClient.getConnection(currentUser);
    String sql3 = " SELECT FULL_NAME FROM BASE_USER WHERE USER_CODE ='";
    sql3 = sql3 + currentUser + "'";
    //out.println("sql3："+sql3);
    st = cn.createStatement();
    rs = st.executeQuery(sql3);
    if (rs.next()) {
      userName = rs.getString(1);
    }
    rs.close();
    rs = null;
    st.close();
    st = null;
  } catch (SQLException ex) {
    ex.printStackTrace();
    throw new Exception("取用户中文名称失败！错误为：" + ex.getMessage());
  } finally {
    if (cn != null)
      cn.close();
    cn = null;
  }
  
  try {
    //String dbUrl = "jdbc:oracle:thin:@EXOA:1521:oanet"; //oracle
    String dbUrl = "jdbc:odbc:EXOA";

    //String dbDriver="oracle.jdbc.driver.OracleDriver"; //oracle
    String dbDriver = "sun.jdbc.odbc.JdbcOdbcDriver";

    String dbUser = "sa";
    String dbPassWord = "excellence";

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

    Class.forName(dbDriver);
    cn = DriverManager.getConnection(dbUrl, dbUser, dbPassWord);
    st = cn.createStatement();
    rs = st.executeQuery(sql);
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
    try {
      if (rs != null) rs.close();
      if (st != null) st.close();
      if (cn != null) cn.close();
    } catch (Exception e) {
    }
  }
  
  out.println("<?xml version=\"1.0\" encoding=\"GB2312\"?>");
  out.println("<TaskAwake>");
  out.println("\t<TaskList>");
  out.println("\t\t<TaskListCount>"+recordCount+"</TaskListCount>");
  out.println("\t</TaskList>");
  out.println("</TaskAwake>");
%>
