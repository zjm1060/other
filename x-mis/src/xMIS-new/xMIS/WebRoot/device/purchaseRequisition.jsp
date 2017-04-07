<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp"%>
   
  	<script type="text/javascript" src="<%=path%>/resources/js/GridRowEditForm.js"></script>
  	 <script type="text/javascript" src="<%=path%>/resources/js/RowExpander.js"></script>
   	 <script type="text/javascript" src="<%=path%>/device/Purchase.js"></script>
    <script type="text/javascript" src="<%=path%>/device/pr.js"></script>
     <%
    com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"com.bitc.xmis.device.PurchaseRequisition.request");
  	%>
  </head>
  <body>
  </body>
</html>
