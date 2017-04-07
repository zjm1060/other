<%@page import="com.bitc.jk.xmis.util.JSONUtils"%>
<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<%@page import="com.bitc.jk.xmis.model.ProjectObj"%>
<%@page import="com.bitc.jk.xmis.web.SpringInit"%>
<%@page import="com.bitc.jk.xmis.dao.ProjectDAO"%>
<%@page import="com.bitc.jk.xmis.dao.OrganizationDAO"%>
<%@page import="com.bitc.jk.xmis.model.SimpleDeptObj"%>
<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.JSONSerializer"%>

<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
    <%
		com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"request");
    
    String projectID=request.getParameter("projectID");
    ProjectDAO dao=(ProjectDAO)SpringInit.getBean("projectDAOImpl");
    ProjectObj project=dao.getProjectInfoByID(projectID);
    SimpleDeptObj dept = project.getExecutiveDept();
 
    
	%>
	<script type="text/javascript">
	<%if(dept ==null){
		%>
		var dept = null;
		<% } else {%>var dept= <%=dept.toJsonString()%>;//执行部门
		<%}%>

	</script>
			<script type="text/javascript" src="<%=path %>/resources/js/SearchField.js"></script>
    		<script type="text/javascript" src="<%=path %>/resources/js/InklingSearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreeChooseField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Draft.js"></script>
    <script type="text/javascript" src="<%=path%>/workflow/js/TodoInfoPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/kinetic-v4.0.0.min.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/Workflow.js"></script>
    <script type="text/javascript" src="<%=path%>/project/startProject.js">
    </script>
    <title>
      北京中水科技 X-MIS
    </title>
  </head>
  <body>
  </body>
</html>

