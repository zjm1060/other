<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*,com.bitc.jk.xmis.util.Type,net.sf.json.JSONObject"%>
<%
if(Type.GetInt(request.getParameter("isApp"))== 1 ){
	String form_id = request.getParameter("form_id");
	String step = request.getParameter("step");
	String workItemId = request.getParameter("workItemId");
	String processInstanceId = request.getParameter("processInstanceId");
	
	JSONObject json=new JSONObject();
	json.element("form_id", form_id);
	json.element("step", step);
	json.element("workItemId", workItemId);
	json.element("processInstanceId", processInstanceId);
	json.element("formURL","/applyManageMaint.do?method=getBusinessEntertainmentFormByFormId");
	json.element("attachmentURL","fileUpload.do?method=getAttachmentList");
	json.element("approvalURL","/workflow.do?method=getApprovalInfos");
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
        <script type="text/javascript" src="<%=path%>/applymanage/approvalWindow.js"></script>
		<script type="text/javascript" src="<%=path%>/businessEntertainment/businessEntertainmentForm.js"></script>
		<script type="text/javascript" src="<%=path%>/businessEntertainment/applyBusinessEntertainmentForm.js"></script>
    <title>
 	 业务招待
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

<%} %>



