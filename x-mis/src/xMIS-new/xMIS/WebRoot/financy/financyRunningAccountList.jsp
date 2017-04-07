<%@ page language="java" pageEncoding="utf-8"%>
<!DOCTYPE HTML>
<html>
<head>
<%@include file="../jsCss.jsp"%>
<%
	String type=(String)request.getAttribute("type");
%>
</head>
<body>
	<script type="text/javascript">
		var type = '<%=type%>';
	</script>
	<script language="JavaScript" src="<%=path %>/resources/js/TotalPagingToolbar.js"></script>
	<script language="JavaScript" src="<%=path %>/resources/js/SearchField.js"></script>
	<script language="JavaScript" src="<%=path %>/resources/js/InklingSearchField.js"></script>
	<script language="JavaScript" src="<%=path %>/financy/financyRunningAccountListPanel.js"></script>
	<script language="JavaScript" src="<%=path %>/financy/financyRunningAccountList.js"></script>
</body>
</html>