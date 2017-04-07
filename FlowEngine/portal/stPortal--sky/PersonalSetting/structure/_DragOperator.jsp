<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    String m_partLayout = request.getParameter("partLayout");
    String m_modu_type = request.getParameter("modu_type");
	String m_modu_id = request.getParameter("modu_id");
	int sReturn = oManager.UpdatePartLayout(m_partLayout);
	response.sendRedirect("DragMine.jsp?modu_id=" + m_modu_id + "&modu_type=" + m_modu_type);
%>