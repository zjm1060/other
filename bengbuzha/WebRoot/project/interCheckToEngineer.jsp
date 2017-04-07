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
<%@page import="com.bitc.jk.xmis.model.ProjectTaskObj"%>
<%@page import="com.bitc.jk.xmis.util.Type"%>
<%@page import="com.bitc.jk.xmis.workflow.model.Todo"%>
<%@page import="com.bitc.jk.xmis.workflow.dao.TodoDAO"%>
<!DOCTYPE HTML>
<%
String projectID=request.getParameter("projectID");
ProjectDAO dao=(ProjectDAO)SpringInit.getBean("projectDAOImpl");
ProjectObj project=dao.getProjectInfoByID(projectID);
OrganizationDAO organizationdao=(OrganizationDAO)SpringInit.getBean("organizationDAOImpl");
SimpleDeptObj dept = null;
String checkDeptIDString = request.getParameter("checkDeptID");
int checkDeptID = Type.GetInt(checkDeptIDString);
if(checkDeptID != 0){
	dept = organizationdao.getSimpleDeptObj(checkDeptID);
}
String applicantString = request.getParameter("applicant");
int applicant = Type.GetInt(applicantString);
ProjectTaskObj ptask= null;	
if(request.getParameter("projectTaskId")!=null){
	ptask=dao.getProjectTask(request.getParameter("projectTaskId"));	
}

String workItemsId = request.getParameter("workItemId");
//求发送人 
Todo todo = null;
if(workItemsId != null && !workItemsId.equals("")){
	TodoDAO todoDAO = (TodoDAO)SpringInit.getBean("todoDAO");
	todo = todoDAO.findTodoByAppId("wf-task",workItemsId);
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
	<script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
	<script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
	<script type="text/javascript" src="<%=path%>/organization/OrgTreeChooseField.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/ChooseOrgTreeField.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/GridRowEditForm.js"></script>
    <script type="text/javascript" src="<%=path%>/project/interCheckToEngineer.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/MultiSelect.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/ItemSelector.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Attention.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
    <script type="text/javascript" src="<%=path%>/project/DeviceImportWin.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/NavButtonPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
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
      if(ptask!=null){
    	  out.print("var projectTask= "+JSONUtils.toJSON(ptask,true)+";");
      } else {
      	out.print("var projectTask = null;" );
      }
      if(todo!=null){
    	  out.print("var a = "+JSONUtils.toJSON(todo,true)+";" );
      } else {
    	  out.print("var a = 3;" );
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

