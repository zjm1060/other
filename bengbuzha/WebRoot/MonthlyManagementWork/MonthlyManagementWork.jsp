<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
		<script type="text/javascript" src="<%=path%>/MonthlyManagementWork/MonthPickerPlugin.js"></script>
		    <script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
        <script type="text/javascript" src="<%=path%>/applymanage/approvalWindow.js"></script>
		<script type="text/javascript" src="<%=path%>/MonthlyManagementWork/monthPlanForm.js"></script>
		<script type="text/javascript" src="<%=path%>/askForLeave/DateValidateOfAbsentFormT.js"></script>
		    <script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
		<script type="text/javascript" src="<%=path%>/MonthlyManagementWork/MonthlyManagementWork.js"></script>
    <title>
 	 工作月报计划
    </title>
     <%
    com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"request");
    %>
         <script type="text/javascript">
     var type='use_type';
    </script>
    
  </head>
  <body>
  
  </body>
</html>





