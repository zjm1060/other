<%@page import="com.bitc.jk.xmis.model.SimpleEmployee"%>
<%@page import="com.bitc.jk.xmis.util.JSONUtils"%>
<%@page import="com.bitc.jk.xmis.web.SpringInit"%>
<%@page import="com.bitc.jk.xmis.dao.ProjectDAO"%>
<%@page import="com.bitc.jk.xmis.dao.OrganizationDAO"%>
<%@page import="com.bitc.jk.xmis.model.SimpleDeptObj"%>
<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<%@page import="com.bitc.jk.xmis.model.ProjectObj"%>
<%@page import="com.bitc.jk.xmis.util.Type"%>
<!DOCTYPE HTML>
<%
String projectID=request.getParameter("projectID");
ProjectDAO dao=(ProjectDAO)SpringInit.getBean("projectDAOImpl");
ProjectObj project=dao.getProjectInfoByID(projectID);
OrganizationDAO organizationdao=(OrganizationDAO)SpringInit.getBean("organizationDAOImpl");
SimpleDeptObj dept = null;
String executiveDeptIDString = request.getParameter("executiveDeptID");
int executiveDeptID = Type.GetInt(executiveDeptIDString);
if(executiveDeptID != 0){
	dept = organizationdao.getSimpleDeptObj(executiveDeptID);
}
SimpleEmployee manager = null;
String managerIDString = request.getParameter("managerID");
int managerID = Type.GetInt(managerIDString);
if(managerID != 0){
	manager = organizationdao.getSimpleEmployee(managerID);
}
	
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
	<script type="text/javascript" src="<%=path%>/resources/js/conInfoObjCom.js"></script>
	   <script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
	    <script type="text/javascript" src="<%=path%>/organization/OrgTreeChooseField.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/ChooseOrgTreeField.js"></script>
    <script type="text/javascript" src="<%=path%>/project/taskAssignDeptChange.js"></script>
    <script type="text/javascript">
      <%
      if(project!=null){
      	out.print("var project = " + project.toJsonString()+";");
      }
      if(dept!=null){
      	out.print("var dept = " + dept.toJsonString()+";");
      } else {
      	out.print("var dept = null;" );
      }
      if(manager!=null){
      	out.print("var manager = " + manager.toJsonString()+";");
      } else {
      	out.print("var manager = null;" );
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

