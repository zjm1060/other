<%@ page contentType="text/html; charset=gb2312" errorPage="/stPortal/include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,sun.jdbc.rowset.*,suntek.portal.publicLib.RecordSetObject" %>
<%@ page import="baseserver.*" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
String PROVINCECOMPANY="base_gov";
String PORTALNUM = "003";
String m_sPath = "";

String m_usercode=(String)session.getAttribute(suntek.security.Security.m_sUser);
String m_username =oManager.getCurrentUserName();
DatabaseInfo databaseInfoLocal = null;

if(databaseInfoLocal == null){
  BaseServerClient lo_BSC = new BaseServerClient("127.0.0.1",7001);

  if(m_usercode  == null || m_usercode.equals("") || m_usercode.equals("guest"))
    m_usercode  = "test2";
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
<SCRIPT LANGUAGE=javascript>
<!--
//var m_url="http://10.233.3.2:8880/mail/" +"< %=m_usercode% >"+ ".nsf";
var m_url="http://192.168.0.254/mail/"+ "<%=m_usercode%>"+ ".nsf";
document.location = m_url;
//-->
</SCRIPT>
