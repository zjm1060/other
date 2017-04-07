<%@ page contentType="text/html; charset=gb2312" errorPage="/stPortal/include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,sun.jdbc.rowset.*,com.minstone.util.database.RecordSetObject" %>
<%@ page import="baseserver.*" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
String PROVINCECOMPANY="oa_lasa";
String PORTALNUM = "003";
String m_sPath = "";

String m_usercode=(String)session.getAttribute(com.minstone.security.Security.m_sUser);
if(m_usercode == null)
	m_usercode = request.getRemoteUser();
String m_username =oManager.getCurrentUserName();
baseserverjb.DatabaseInfo databaseInfoLocal = null;

if(databaseInfoLocal == null){
  BaseServerClient lo_BSC = new BaseServerClient("127.0.0.1",7001);

  if(m_usercode  == null || m_usercode.equals("") || m_usercode.equals("guest"))
    m_usercode  = "hhy";
  databaseInfoLocal = lo_BSC.getSystemConnectionStringEx(m_usercode,PORTALNUM);
}
if(databaseInfoLocal == null){
  throw new Exception("tomail.jsp 没法得到数据库连接信息！");
}
//生产机邮件系统的地址格式是：http://10.233.3.2:8880/mail/oa_lasa/usercode.nsf
String m_sArea = databaseInfoLocal.getUser();
//if(m_sArea.toLowerCase().equals(PROVINCECOMPANY))
//  m_sPath = "";
//else
m_sPath = m_sArea + "/";
%>
<SCRIPT type="text/javascript">
<!--
//var m_url="http://10.233.3.2:8880/mail/" +"< %=m_usercode% >"+ ".nsf";
var m_url="http://10.233.3.2:8880/mail/" + "<%=m_sPath%>" + "<%=m_usercode%>"+ ".nsf";
document.location = m_url;
//-->
</SCRIPT>
