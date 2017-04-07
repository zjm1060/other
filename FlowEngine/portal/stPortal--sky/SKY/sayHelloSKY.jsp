<%@ page contentType="text/html; charset=gb2312" errorPage="/stPortal/include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,sun.jdbc.rowset.*,suntek.portal.publicLib.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
String m_usercode=(String)session.getAttribute(suntek.security.Security.m_sUser);
String m_username =oManager.getCurrentUserName();
%>
<html>
<head>
<title>问候
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
</title>

<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<link href="oacss.css" rel="stylesheet" type="text/css">

<STYLE type=text/css>.12pt {
	FONT-SIZE: 12pt
}
.10pt {
	FONT-SIZE: 10.5pt
}
.9pt {
	FONT-SIZE: 9pt
}
A:link {
	COLOR: black; TEXT-DECORATION: none
}
A:visited {
	COLOR: black; TEXT-DECORATION: none
}
A:hover {
	COLOR: red; TEXT-DECORATION: none
}
INPUT {
	FONT-SIZE: 9pt
}
SELECT {
	FONT-SIZE: 9pt
}
TABLE {
	FONT-SIZE: 9pt; CURSOR: default
}
.normal {
	BORDER-RIGHT: 0px solid; BORDER-TOP: 0px solid; BORDER-LEFT: 0px solid; BORDER-BOTTOM: 0px solid
}
.over {
	BORDER-RIGHT: #808080 1px solid; BORDER-TOP: #ffffff 1px solid; BORDER-LEFT: #ffffff 1px solid; BORDER-BOTTOM: #808080 1px solid
}
.out {
	BORDER-RIGHT: 0px solid; BORDER-TOP: 0px solid; BORDER-LEFT: 0px solid; BORDER-BOTTOM: 0px solid
}
.down {
	BORDER-RIGHT: #ffffff 1px solid; BORDER-TOP: #808080 1px solid; PADDING-LEFT: 2px; BORDER-LEFT: #808080 1px solid; PADDING-TOP: 2px; BORDER-BOTTOM: #ffffff 1px solid
}
.up {
	BORDER-RIGHT: 0px solid; BORDER-TOP: 0px solid; BORDER-LEFT: 0px solid; BORDER-BOTTOM: 0px solid
}
A {
	FONT-SIZE: 9pt; CURSOR: default
}
FORM {
	MARGIN-TOP: 0px; MARGIN-BOTTOM: 0px
}
</STYLE>

</head>

<body rightmargin="0" bottommargin="0" bgColor="#8199D9" text="#FFFF00" leftmargin="0" topmargin="0">
<div align=center style="width:100%">
<TABLE
      height=18 cellSpacing=0 cellPadding=0 width="90%"
      border=0>
        <TBODY>

        <TR>
          <TD noWrap vAlign=center>
            <P
            style="MARGIN-TOP: 3px; MARGIN-BOTTOM: 3px; MARGIN-LEFT: 10px"><FONT
            color=BLACK><FONT size=2 color="YELLOW">
</FONT><FONT color=white><SPAN><%=m_username%></SPAN>，您好！欢迎登陆到中国水科院财务管理信息系统，请在上方选择您要处理的业务类型。</FONT> 
           </P></TD>

		</TR>
</TBODY>
</TABLE>
</div>
</body>
</html>



