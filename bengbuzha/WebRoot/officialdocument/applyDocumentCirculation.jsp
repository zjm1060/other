<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*,com.bitc.jk.xmis.util.Type,net.sf.json.JSONObject"%>
<%
if(Type.GetInt(request.getParameter("isApp"))== 1 ){
	String form_id = request.getParameter("form_id");
	JSONObject json=new JSONObject();
	json.element("form_id", form_id);
	json.element("formURL","/applyManageMaint.do?method=getDocumentAppFormDetailByFormId");	
	json.element("attachmentURL","fileUpload.do?method=getAttachmentList");
	json.element("updateState","msgMaint.do?method=updateMessageInfoState");
	json.element("transferListURL","applyManageMaint.do?method=getDocumentCirculatedInstancesList");
	  json.element("getRecordList","applyManageMaint.do?method=getDocumentCirculatedInstancesList");
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
    <script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
		     <script type="text/javascript" src="<%=path%>/organization/OrgTreePanel.js"></script>
    <script type="text/javascript" src="<%=path%>/organization/OrgTreeChooseField.js"></script>
       <script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
    		<script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
			<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
			<script type="text/javascript" src="<%=path%>/officialdocument/applyDocumentCirculation.js"></script>
 
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




