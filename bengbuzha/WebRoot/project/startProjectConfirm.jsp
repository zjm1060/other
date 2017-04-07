<%@page import="com.bitc.jk.xmis.util.JSONUtils"%>
<%@page import="com.bitc.jk.xmis.model.ProjectTaskObj"%>
<%@page import="com.bitc.jk.xmis.web.SpringInit"%>
<%@page import="com.bitc.jk.xmis.dao.ProjectDAO"%>
<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<%@page import="com.bitc.jk.xmis.model.ProjectObj"%>
<!DOCTYPE HTML>
<%
String projectID=request.getParameter("projectID");
ProjectDAO dao=(ProjectDAO)SpringInit.getBean("projectDAOImpl");
ProjectObj project=dao.getProjectInfoByID(projectID);
%>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
    		<script type="text/javascript" src="<%=path %>/resources/js/SearchField.js"></script>
    		<script type="text/javascript" src="<%=path %>/resources/js/InklingSearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/workflow/js/TodoInfoPanel.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/kinetic-v4.0.0.min.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/Workflow.js"></script>
	    <script type="text/javascript" src="<%=path%>/project/ProjectTaskPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/project/startProjectConfirm.js"></script>
    <script type="text/javascript">
      <%
      if(project!=null){
      	out.print("var project = " + project.toJsonString()+";");
      }
      %>
    </script>
    <%
    com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"request");
    %>
    <title>
      北京中水科技 X-MIS
    </title>
  </head>
  <body>
  </body>
</html>

