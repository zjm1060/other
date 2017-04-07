<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<%@page import="com.bitc.jk.xmis.model.ProjectObj"%>
<!DOCTYPE HTML>
<%
ProjectObj project=(ProjectObj)request.getAttribute("project");
String checkDeptIDString = request.getParameter("checkDeptID");
String processInstanceId=request.getParameter("processInstanceId");
String isHiddenByIsAgree = (String)request.getAttribute("isHiddenByIsAgree");

%>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
    <script type="text/javascript" src="<%=path%>/resources/js/SearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/InklingSearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/workflow/js/TodoInfoPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/device/DeviceGridPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/device/ProjectDevicePanelGrid.js"></script>
    <script type="text/javascript" src="<%=path%>/project/ProjectDevicePanel.js"></script>
    <script type="text/javascript" src="<%=path%>/project/ProjectTaskPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/MultiSelect.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/ItemSelector.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Attention.js"></script>
    <script type="text/javascript" src="<%=path%>/project/DeviceImportWin.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/NavButtonPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/project/ProjectSchedule.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/GridRowEditForm.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreeChooseField.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/ChooseOrgTreeField.js"></script>
	<script type="text/javascript" src="<%=path%>/contract/confirmPayment.js"></script>
	<script type="text/javascript" src="<%=path%>/project/estimatedPaymentPanel.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/kinetic-v4.0.0.min.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/Workflow.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/device/DeviceImportPanelGrid.js"></script>
    <script type="text/javascript" src="<%=path%>/project/projectInfo.js"></script>
    <script type="text/javascript">
      <%
      if(project!=null){
      	out.print("var project = " + project.toJsonString()+";");
      	if(isHiddenByIsAgree!=null){
      		out.print("var isHiddenByIsAgree = "+isHiddenByIsAgree+";");
      	}else {
      		out.print("var isHiddenByIsAgree = null;");
      	}
      }
      out.print("var checkDeptIDString = "+checkDeptIDString+";");
      %>
    </script>
    <%
    com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"com.bitc.xmis.ProjectInfo.request");
    %>
    <title>
      北京中水科技 X-MIS
    </title>
  </head>
  <body>
  </body>
</html>

