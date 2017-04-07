<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%
	String path = request.getContextPath();
%>
<table width="100%">
	<tr>
		<TH class=large noWrap valign="bottom" colSpan=2>
		<H2><font color=blue><%=((EmployeeObj) session
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA))
					.getDivisionName()%></font></H2>
		</TH>
	</tr>
	<tr>
		<TH class=large noWrap><font color="#0000FF">欢迎 <%=((EmployeeObj) session
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA))
					.getUserName()%></font></TH>
		<TH class=large noWrap><a href="<%=path + "/logout.do"%>"> 注销</a></TH>
	</tr>
</table>
