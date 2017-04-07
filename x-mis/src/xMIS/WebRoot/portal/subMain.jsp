<%@ page language="java" import="java.util.*" pageEncoding="gb2312"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>

<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ request.getContextPath() + "/";
	System.out.println("subMain.jsp ---> " + basePath + "\n" + path);

//	String type = (String) request.getParameter("type");
	String type = request
			.getParameter(WebKeys.Request_Attribute_Name_Type) == null ? (String) request
			.getAttribute(WebKeys.Request_Attribute_Name_Type)
			: request.getParameter(WebKeys.Request_Attribute_Name_Type);

	String subType = null;
//	subType = (String) request.getParameter("subType");
	subType = request
	.getParameter(WebKeys.Request_Attribute_Name_SubType) == null ? (String) request
	.getAttribute(WebKeys.Request_Attribute_Name_SubType)
	: request
	.getParameter(WebKeys.Request_Attribute_Name_SubType);


	System.out.println("subMaint.jsp -> type = " + type
			+ "; subType = " + subType);
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
	<head>
		<base href="<%=basePath%>">
		<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
		<script language="JavaScript1.2" src="/xMIS/resources/js/sidebar.js"></script>


	


		<meta http-equiv="pragma" content="no-cache">
		<meta http-equiv="cache-control" content="no-cache">
		<meta http-equiv="expires" content="0">
		<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
		<meta http-equiv="description" content="This is my page">
		<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->

	</head>

	<BODY onresize=setHeightOfFooter() leftMargin=0 topMargin=0
		marginheight="0" marginwidth="0">

		<SCRIPT language=JavaScript>
var contextPath = "/xMIS";

var url_sidebar_img_left = "/xMIS/resources/images/sidebar_left.gif";
var url_sidebar_img_right = "/xMIS/resources/images/sidebar_right.gif";


</SCRIPT>

		<jsp:include page="<%=WebKeys.Include_JSP_File_sideBar %>" />
		<DIV id=sideimg
			style="Z-INDEX: 4; LEFT: 161px; POSITION: absolute; TOP: 130px">
			<IMG style="CURSOR: hand" onclick=clickSideBar();blur() height=39
				alt="expand/collapse the menu"
				src="/xMIS/resources/images/sidebar_left.gif" width=13 border=0
				name=img_sidebar>
		</DIV>
		
		<%
		if (subType != null) {
		%>
		<DIV id=content	style="Z-INDEX: 1; LEFT: 185px; POSITION: absolute; TOP: 130px">
			<jsp:include page="<%=WebKeys.Include_JSP_File_operationPart %>" />
		</DIV>
		
		<%
		}
		%>

	</body>
</html>
