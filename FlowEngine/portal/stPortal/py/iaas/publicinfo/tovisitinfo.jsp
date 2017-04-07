<%@ page contentType="text/html; charset=gb2312" errorPage="/stPortal/include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,sun.jdbc.rowset.*,suntek.portal.publicLib.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
String m_usercode=(String)session.getAttribute(suntek.security.Security.m_sUser);
String m_username =oManager.getCurrentUserName();
System.out.println("fdfdfdsfdsfff");
System.out.println("dfdsfsdafsdhfdsh...."+m_usercode);
System.out.println("fdfdfdsfdsfff"+m_username);
%> 
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title></title>
</head>
<body>
<script>
var accounts="<%=m_username%>";
if(accounts=="投诉科科长"){

window.location="../../FlowEngineWeb/publicinfo/visitInfofind.do?modify=6&IsSearch=no&account=<%=m_username%>";
}else{
window.location="../../FlowEngineWeb/publicinfo/visitInfo.do?IsOne=yes&modify=6&account=<%=m_username%>";}

</script>

</body>
</html>
