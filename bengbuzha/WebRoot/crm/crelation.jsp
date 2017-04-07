<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<!DOCTYPE HTML>
<html>
	<head>

		<title>companyTree page</title>

		<%@include file="../jsCss.jsp"%>
		<%
			boolean isAdmin = false;
			int currentUserID = ((EmployeeObj) request.getSession()
					.getAttribute(WebKeys.Session_Attribute_Name_UserDNA))
					.getUserID();
			if (currentUserID == -1 || currentUserID == 605) {
				isAdmin = true;
			}
		%>
		<style type="text/css">
			.x-grid3-row-over .x-grid3-cell-inner {
				font-weight: bold;
				color:'#545BED'
			}
		</style>
		<script type="text/javascript">
			var isAdmin= <%=isAdmin%>;
			var currentUserID = <%=currentUserID%>;
		</script>
		<script type="text/javascript" src="<%=path %>/resources/js/SearchField.js"></script>
    		<script type="text/javascript" src="<%=path %>/resources/js/InklingSearchField.js"></script>
    		<script type="text/javascript" src="<%=path %>/crm/crelation.js"></script>
	</head>
	<body>

		
	</body>
</html>
