<%@ page language="java" pageEncoding="GB2312" %>
<%@ page import="minstone.ItemMange.SessionCounter.* %>
<html>
<head>
<title></title>
</head>
<% // kill the session
try {
    String strSid = request.getParameter("sessionid");
    HttpSession session1 = minstone.ItemMange.SessionCounter.getSession(strSid);
    if (session1!=null){
    	session1.invalidate();
    }
} catch (Exception e) {
	e.printStackTrace();
}
response.sendRedirect("session.jsp");
%>
</html>