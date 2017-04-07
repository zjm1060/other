<%@ page language="java" pageEncoding="GB2312" %>
<!DOCTYPE HTML PUBLIC "-//w3c//dtd html 4.0 transitional//en">
<html>
<head>
<title>Lomboz JSP</title>
</head>
<body bgcolor="#FFFFFF">
List session object
<br>
<table border='1'>
<tr bgcolor='yellow'>
<td>session id</td>
<td>user name </td>
<td>what thing </td>
<td>create time </td>
<td>operate</td>
</tr>
<%
/*
  Iterator iterator = minstone.ItemManage.SessionCounter.getActiveSessions();
  while(iterator.hasNext()){
  try{
    HttpSession session1 = (HttpSession)iterator.next();
    out.println("<tr>");
    out.println("<td>" + session1.getId() + "</td>" );
    out.println("<td>" + session1.getAttribute("name") + "</td>" );
    out.println("<td>" + session1.getAttribute("thing") + "</td>" );
    out.println("<td>" + session1.getCreationTime() + "</td>" );
    out.println("<td> <a href='kill.jsp?sessionid=" + session1.getId() +"'>kill </a> </td>" );
    out.println("</tr>");
    System.out.println("list " + session1.getId());
  }catch(Exception ex){
  	ex.printStackTrace();
  	return;
  }
  }
*/
%>
</table></body>
</html>
