<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.*" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    String m_Msg = "";
    Hashtable dct_Para = new Hashtable();
    Hashtable dct_Key = new Hashtable();
    int sReturn = -1;
	//调整次序
	String m_SQL=request.getParameter("HOrder");
    if (m_SQL == null)
        m_SQL = "";
    String m_usercode=(String)request.getRemoteUser();
    String m_username =oManager.getCurrentUserName();
	sReturn = oManager.AdjustPersonModuOrder(m_usercode,m_SQL);
	if (sReturn >=0){
    }
	else
		out.println("htmRenderErrorMessage \"调整次序发生错误，请刷新后再试！\",\"");
%>
