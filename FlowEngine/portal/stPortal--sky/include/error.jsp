<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<%@ page import="java.io.*" isErrorPage="true" %>
<HTML>
<HEAD>
  <TITLE>Error!</TITLE>
</HEAD>
<BODY>

<H3>错误: <%=exception%></H3>
<H3>信息: <%=exception.getMessage()%></H3>
<H3>本地信息: <%=exception.getLocalizedMessage()%></H3>
  <%
  StringWriter errorWriter = new StringWriter ();
  PrintWriter errorStream = new PrintWriter (errorWriter);
  exception.printStackTrace (errorStream);
  %>
  <P>堆栈跟踪:
  <BLOCKQUOTE>
    <PRE><%=errorWriter%></PRE>
  </BLOCKQUOTE>
</BODY>
</HTML>
