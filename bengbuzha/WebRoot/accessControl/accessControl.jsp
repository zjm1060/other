<%@page contentType="text/html; charset=utf-8"%>
<!DOCTYPE HTML>
<%
	EmployeeObj emp = (EmployeeObj) request.getSession().getAttribute(
		WebKeys.Session_Attribute_Name_UserDNA);
%>>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
    <script type="text/javascript" src="<%=path %>/resources/js/SearchField.js"></script>
    <script type="text/javascript" src="<%=path %>/resources/js/InklingSearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
    <script type="text/javascript" src="<%=path%>/accessControl/accessControl.js"></script>
   	<script type="text/javascript">
   	var empId = "<%=emp.getUserID()%>";
   	var empDivisionId = "<%=emp.getDivisionID()%>";
   	</script>
    <title>
      北京中水科技 X-MIS
    </title>
  </head>
  <body>
  </body>
</html>