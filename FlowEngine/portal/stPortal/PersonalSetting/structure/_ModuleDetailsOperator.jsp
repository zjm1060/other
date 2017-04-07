<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.Hashtable,jbportal.PortalUtil" %>
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    String modu_id = "";
    String m_moduType = "";
    String use_flag = "";
    int sReturn = 100;
    String Msg = "";
    Hashtable dct_Para = new Hashtable();
    Hashtable dct_Key = new Hashtable();
	modu_id = request.getParameter("modu_id");
    if (modu_id == null)
        modu_id = "";

	m_moduType=request.getParameter("modu_type");
    if (m_moduType == null)
        m_moduType = "";
    use_flag=request.getParameter("use_flag");
    if (use_flag == null)
        use_flag = "";
    String m_usercode=(String)request.getRemoteUser();
    String m_username =oManager.getCurrentUserName();

    dct_Key.put("modu_id",modu_id);
    dct_Key.put("modu_type",m_moduType);
    dct_Key.put("user_code",m_usercode);
    dct_Para.put("use_flag",PortalUtil.toDouble(use_flag));
	sReturn = oSys.update(dct_Key,dct_Para,"user_modu",false);
	if (sReturn >=0)
		response.sendRedirect("ModuleDetails.jsp?m_moduid=" + modu_id + "&modu_type=" + m_moduType);
	else
		Msg = "updateError";
%>
