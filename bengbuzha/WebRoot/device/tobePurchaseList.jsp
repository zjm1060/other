<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp"%>
    <script type="text/javascript" src="<%=path%>/resources/js/SearchField.js"></script>
   <script type="text/javascript" src="<%=path%>/resources/js/InklingSearchField.js"></script>
   	 <script type="text/javascript" src="<%=path%>/resources/js/GroupSummary.js"></script>
   	 <script type="text/javascript" src="<%=path%>/device/PurchasingList.js"></script>
    <script type="text/javascript" src="<%=path%>/device/tobePurchaseList.js"></script>
     <%
    com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"com.bitc.xmis.device.PurchaseRequisition.request");
  	%>
  </head>
  <body>
  </body>
</html>
