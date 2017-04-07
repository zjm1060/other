<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<!DOCTYPE HTML>
<%

EmployeeObj emp = (EmployeeObj) request.getSession().getAttribute(
		WebKeys.Session_Attribute_Name_UserDNA);

boolean decisionFlag = emp.hasDecisionRole();
boolean readOnly = emp.isReadOnly();
%>
<html>
	<head>
		<%@include file="../jsCss.jsp"%>
		
		<style type="text/css">
			.x-grid3-row-over .x-grid3-cell-inner {
				font-weight: bold;
				color:'#545BED'
			}
		</style>
		<script type="text/javascript" src="<%=path %>/resources/js/SearchField.js"></script>
    		<script type="text/javascript" src="<%=path %>/resources/js/InklingSearchField.js"></script>
    		
    		  <script type="text/javascript" src="<%=path %>/materials/materials.js"></script>
    		  <script type="text/javascript">
	var decisionFlag = <%=decisionFlag%>;
	var readOnly = <%=readOnly%>;
</script>
	</head>
	<body>

		
	</body>
</html>
