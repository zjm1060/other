<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ page import="java.sql.*"%>
<%@ page import="javax.sql.*"%>
<%@ page import="javax.naming.*"%>
<!DOCTYPE HTML>　　
<html>
  　　
  <head>
    　　
    <title>
      tomcat数据库连接池技术测试
    </title>
    　　
  </head>
  　　
  <body>
    <br>
    　　
    <%
    Class.forName("com.microsoft.sqlserver.jdbc.SQLServerDriver");
    Connection conn = null;
    conn=DriverManager.getConnection("jdbc:sqlserver://192.169.11.149:1433;DatabaseName=xmis","xmis-uat","xmis");
    Statement stmt = conn.createStatement();
    PreparedStatement ps = conn.prepareStatement("select * from con_合同");
    ResultSet rs = ps.executeQuery();
    while (rs.next()) {
      out.println(rs.getInt(1) + "< br>");
      out.println(rs.getString(2) + "< br>");
      out.println(rs.getString(3) + "< br>");
    }
    rs.close();
    stmt.close();
    conn.close();
    out.println("测试成功");
    %>
    　　
  </body>
  　　
</html>
