</html>

<%@ page language="java" pageEncoding="utf-8"%>
<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <!-- script type="text/javascript" src="<%=path%>/resources/js/Draft.js"></script-->
    <script type="text/javascript" src="<%=path%>/applymanage/approvalWindow.js"></script>
		    <script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
		    <script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
    		<script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
			<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
			<script type="text/javascript" src="<%=path%>/officialdocument/uploadDocument.js"></script>
<title>Insert title here</title>
</head>

<body>
 <%
    com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"request");
    %>
</body>
</html>