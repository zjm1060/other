<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%

	//调整次序
	int sReturn = -1;
	String m_SQL=request.getParameter("HOrder");
    if (m_SQL == null)
        m_SQL = "";
    String m_usercode=(String)request.getRemoteUser();
    String m_username =oManager.getCurrentUserName();
    m_SQL=m_SQL.replace('-',',');
	sReturn = oManager.AdjustUserModuOrder(m_usercode,m_SQL);
	if (sReturn >=0){
    }
	else
		out.println("htmRenderErrorMessage \"调整次序发生错误，请刷新后再试！\",\"");
%>
