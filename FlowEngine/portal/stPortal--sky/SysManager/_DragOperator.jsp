<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
	String m_Msg="";
    String m_partLayout = request.getParameter("partLayout");
	//得到传入的参数
	String m_modu_id = request.getParameter("modu_id");
    if(m_modu_id==null)
        m_modu_id = "";
	//根据传入的语句编写相应的SQL语句
	int sReturn = oManager.UpdatePartLayout(m_partLayout);
	response.sendRedirect("DragMine.jsp?modu_id=" + m_modu_id);
%>