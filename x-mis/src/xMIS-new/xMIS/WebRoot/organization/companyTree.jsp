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
			if (currentUserID == -1 || currentUserID == 605 || currentUserID == 614) {
				isAdmin = true;
			}
		%>
		<style type="text/css">
			.x-tree-node-italic-red span {color:#CC0000 !important;}
			.x-tree-node-italic-green span {color:green !important;}
		</style>
		<script type="text/javascript">
			var isAdmin= <%=isAdmin%>;
			var currentUserID = <%=currentUserID%>;
		</script>
		<script type="text/javascript" src="<%=path %>/resources/js/MultiSelect.js"></script>
		<script type="text/javascript" src="<%=path %>/crm/AddressChooseField.js"></script>
		<script type="text/javascript" src="<%=path %>/resources/js/SearchField.js"></script>
    	<script type="text/javascript" src="<%=path %>/resources/js/InklingSearchField.js"></script>
		<script type="text/javascript" src="<%=path %>/organization/companyTree.js"></script>
<script type="text/javascript">
	Ext.onReady(function(){
		var panels=buildPanel(1);
		new Ext.Viewport({
			layout : 'border',
			items : [panels[0], panels[1]]
		});
	});
</script>
	</head>
	<body>

		
	</body>
</html>
