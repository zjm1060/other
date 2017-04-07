<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%

	String m_modu_id = request.getParameter("modu_id");
        if(m_modu_id==null)
            m_modu_id = "";
        String l_modu_type = request.getParameter("modu_type");
        if (l_modu_type==null)
            l_modu_type = "";
	oManager.RestoreLayout(m_modu_id,l_modu_type);  //»Ö¸´Ä£¿éÈ±Ê¡ÉèÖÃ
    response.sendRedirect("DragAdmin.jsp?modu_id=" + m_modu_id + "&modu_type=" + l_modu_type);
%>
