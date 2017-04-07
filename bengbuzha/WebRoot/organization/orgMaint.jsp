<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<!DOCTYPE HTML>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";

List aList = (List)session.getAttribute("testList");

%>

<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'orgMaint.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->

  </head>
  
  <body>
    This is my JSP page. <br>
    
    <%
    for(int i=0; i<aList.size(); i++){ 
    	out.println(aList.get(i));
    } 
    %>
  </body>
</html>
