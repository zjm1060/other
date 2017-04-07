<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<HTML>
<HEAD>
<TITLE>系统管理</TITLE>
</HEAD>
<%
    String m_modu_id = "";
    String m_modu_type = "";
	m_modu_id=request.getParameter("modu_id");
	m_modu_type=request.getParameter("modu_type");
%>

<SCRIPT type="text/javascript">
<!--
	function Exit()
	{
		window.location = 'aa.jsp'
	}
//-->
</SCRIPT>

<!--页面内容 start-->


  <frameset rows="*" cols="*,0" frameborder="1" BORDER="1" FRAMESPACING="1" BORDERCOLOR="#9999CC">
	<frame src="DragMine.jsp?modu_id=<%=m_modu_id%>&modu_type=<%=m_modu_type%>" name="main" scrolling="YES">
	<frame src="" name="update" >
</frameset>
</HTML>

