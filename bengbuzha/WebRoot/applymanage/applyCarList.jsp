<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<%@page import="com.bitc.jk.xmis.util.WebKeys"%>
<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreeChooseField.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
   <script type="text/javascript" src="<%=path%>/resources/js/SearchField.js"></script>
       <script type="text/javascript" src="<%=path%>/resources/js/InklingSearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/ChooseOrgTreeField.js"></script>
     
    <script type="text/javascript" src="<%=path%>/applymanage/officialVehiclesGridToSelect.js"></script>
    <script type="text/javascript" src="<%=path%>/applymanage/applyCarFormPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/applymanage/applyCarList.js"></script>
    		<script type="text/javascript">
			var ROLE= <%=WebKeys.APPLY_CAR_ROLE%>;
			  var isApplyPerson = com.bitc.xmis.existRole(__emp.roles,ROLE);
		</script>
    <title>
 	审批列表
    </title>

 
 
  </head>
  <body>
  
  </body>
</html>

