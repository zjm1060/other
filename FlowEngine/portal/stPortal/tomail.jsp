<%@ page contentType="text/html; charset=gb2312" errorPage="/stPortal/include/error.jsp"%>
<%@ page import="baseserver.*" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
String PROVINCECOMPANY="HBOA";
String PORTALNUM = "002";
String m_sPath = "";

String m_usercode=request.getRemoteUser();
String m_username =oManager.getCurrentUserName();
baseserverjb.DatabaseInfo databaseInfoLocal = null;

if(databaseInfoLocal == null){
  BaseServerClient lo_BSC = new BaseServerClient("127.0.0.1",9081);

  if(m_usercode  == null || m_usercode.equals("") || m_usercode.equals("guest"))
    m_usercode  = "qijun";
  databaseInfoLocal = lo_BSC.getSystemConnectionStringEx(m_usercode,PORTALNUM);
}
if(databaseInfoLocal == null){
  throw new Exception("tomail.jsp 没法得到数据库连接信息！");
}

String m_sArea = databaseInfoLocal.getUser();
if(m_sArea.toLowerCase().equals(PROVINCECOMPANY))
  m_sPath = "";
else
  m_sPath = m_sArea + "/";
%>
<SCRIPT type="text/javascript">
var m_url="http://mssmail.hb.unicom:8800/mail/" +"<%=m_usercode%>"+ ".nsf";
document.location = m_url;
</SCRIPT>
