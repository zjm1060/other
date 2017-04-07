<%@ page language="java" pageEncoding="GB2312" %>
<!DOCTYPE HTML PUBLIC "-//w3c//dtd html 4.0 transitional//en">
<html>
<head>
<title>Lomboz JSP</title>
</head>
<body bgcolor="#FFFFFF">
<%
       if(session.isNew()!=true){
              session.invalidate();
       }
       response.sendRedirect("index.jsp");
%>
</body>
</html>
