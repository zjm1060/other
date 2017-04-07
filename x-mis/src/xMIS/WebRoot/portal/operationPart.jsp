<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>

<%
	String subType = request
	.getParameter(WebKeys.Request_Attribute_Name_SubType) == null ? (String) request
	.getAttribute(WebKeys.Request_Attribute_Name_SubType)
	: request
	.getParameter(WebKeys.Request_Attribute_Name_SubType);

	System.out.println("operationPart.jsp -> subType = " + subType + "//" + request.getRequestURI());
	
%>

<!-- DIV id=content	style="Z-INDEX: 1; LEFT: 185px; POSITION: absolute; TOP: 130px"-->

	<%if(WebKeys.orgMaintURI.equals(request.getAttribute(WebKeys.RequestKey_Original_URI))){ %>
		<jsp:include page="<%=WebKeys.Include_JSP_File_org_subMain %>"></jsp:include>
	<%}else if(WebKeys.contractMainURI.equals(request.getAttribute(WebKeys.RequestKey_Original_URI))){ %>
		<jsp:include page="<%=WebKeys.Include_JSP_File_contract_subMain %>"></jsp:include>
	<%}else{ %>
		<jsp:include page="<%=WebKeys.Include_JSP_File_building %>" />
	<%} %>
<!-- /DIV-->
