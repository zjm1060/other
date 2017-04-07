<%@ page contentType="text/html; charset=GBK" %>
<html>
<head>
<title>Lomboz JSP</title>
</head>
<body bgcolor="#FFFFFF">
<%
    if (session.isNew()==true){
            response.sendRedirect("index.jsp");
    }
    out.println("name: "+ session.getAttribute("name") + "<br>");
    out.println("thing: "+ session.getAttribute("thing") + "<br>");
    out.println("session id: " + session.getId() + "<br>");
    out.println("create time: " + session.getCreationTime() );
    %>
</body>
</html>
