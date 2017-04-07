<%@ page import="suntek.security.*"%>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="run" scope="session" class="jbportal.runBean"/>
<jsp:useBean id="CaManager" scope="page" class="net.kingsoft.gdca.CaManager"/>
<jsp:useBean id="framework" scope="application" class="net.kingsoft.user.Framework"/>

<%!
  final static String PORTALURL="t3://localhost:7001";
  final static String CSSPATHBASE="/res/cssfunc/";
  String m_username=null;
  String m_password=null;
%>
<%
    m_username=request.getParameter("j_username");
    m_password=request.getParameter("j_password");
    if(m_username==null) m_username="";
    if(m_password==null) m_password="";
	// 外网用户， 没有登陆资格
	if(framework.isOuterUser(m_username)) {
		%>
		<jsp:forward page="LoginError.htm" />
		<%
	}
    session.setAttribute("PORTALURL",PORTALURL);
    session.setAttribute("CSSPATHBASE",CSSPATHBASE);
    session.setAttribute(Security.m_sUser,m_username);
    session.setAttribute(Security.m_sPwd,m_password);
    //初始化JAVABEAN
    run.setEJB_URL(PORTALURL);
    run.setEJB_User(m_username);
    run.setEJB_Pwd(m_password);
    //强行断开EJB连接
    run.setRunRemoteObject(null);

    oManager.setEJB_URL(PORTALURL);
    oManager.setEJB_User(m_username);
    oManager.setEJB_Pwd(m_password);
    //强行断开EJB连接
    oManager.setSysManagerRemoteObject(null);

    oSys.setEJB_URL(PORTALURL);
    oSys.setEJB_User(m_username);
    oSys.setEJB_Pwd(m_password);
    //强行断开EJB连接
    oSys.setClassServiceRemoteObject(null);
    //初始化JAVABEAN结束
%>
<html>
<head>
<script language="javascript">
function window_onload()
{
  document.checksec.submit();
}
</script>
</head>
<body onload="javascript:window_onload();">
<form name="checksec" method="post" action="j_security_check">
  <input type="hidden" name="j_username" value="<%=m_username%>">
  <input type="hidden" name="j_password" value="<%=m_password%>">
</form>
</body>
</html>