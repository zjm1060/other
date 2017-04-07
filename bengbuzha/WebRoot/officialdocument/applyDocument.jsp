<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*,com.bitc.jk.xmis.util.Type,net.sf.json.JSONObject"%>
<%
if(Type.GetInt(request.getParameter("isApp"))== 1 ){
	String form_id = request.getParameter("form_id");
	String step = request.getParameter("step");
	String processTaskId = request.getParameter("processTaskId");
	String processInstanceId = request.getParameter("processInstanceId");
	
	JSONObject json=new JSONObject();
	json.element("form_id", form_id);
	json.element("step", step);
	json.element("processTaskId", processTaskId);
	json.element("processInstanceId", processInstanceId);
	json.element("attachmentTypeID", 18);
	json.element("formURL","/applyManageMaint.do?method=getDocumentAppFormDetailByFormId");
	json.element("attachmentURL","fileUpload.do?method=getAttachmentList");
	json.element("approvalURL","/workflow.do?method=getApprovalInfos");
	json.element("docBodyURL","/fileUpload.do?type=11&form_id="+form_id+"&fileName=正文");
	response.setContentType("application/json;charset=UTF-8");
	response.getWriter().write(json.toString());
	
}else{
%>
<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
     <script type="text/javascript" src="<%=path%>/resources/js/Draft.js"></script>
        <script type="text/javascript" src="<%=path%>/applymanage/approvalWindow.js"></script>
		    <script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
		    <script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
    		<script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
			<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
			<script type="text/javascript" src="<%=path%>/officialdocument/applyDocument.js"></script>
    <title>
 	 公文流转
    </title>
     <%
    com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"request");
    %>
         <script type="text/javascript">
 
    </script>
    	
  </head>
  <body>
  
  </body>
</html>

<%} %>



