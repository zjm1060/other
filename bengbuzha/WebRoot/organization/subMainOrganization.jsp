<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>

<%
	String subType = request
	.getParameter(WebKeys.Request_Attribute_Name_SubType) == null ? (String) request
	.getAttribute(WebKeys.Request_Attribute_Name_SubType)
	: request
	.getParameter(WebKeys.Request_Attribute_Name_SubType);

	System.out.println("operationPart.jsp -> subType = " + subType);
%>
	<%
	if (subType.equals(WebKeys.Request_Attribute_Name_SubType_departmentTree)) {
	%>
	<jsp:include flush="true" page="<%=WebKeys.Include_JSP_File_departmentTree %>"></jsp:include>
	<%
	}
	%>
