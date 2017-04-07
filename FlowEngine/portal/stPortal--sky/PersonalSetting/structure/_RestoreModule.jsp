<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
	oManager.RestoreModuleSetting(); //»Ö¸´Ä£¿éÈ±Ê¡ÉèÖÃ
    response.sendRedirect("ModuleMain.jsp");
%>
