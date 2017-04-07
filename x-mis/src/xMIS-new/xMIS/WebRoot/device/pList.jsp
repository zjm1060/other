<!DOCTYPE HTML>
<html>
  <head>
  <title>采购清单列表</title>
    <%@include file="../jsCss.jsp"%>
     <script type="text/javascript" src="<%=path%>/resources/js/SearchField.js"></script>
   <script type="text/javascript" src="<%=path%>/resources/js/InklingSearchField.js"></script>
   <script type="text/javascript" src="<%=path%>/resources/js/ChooseField.js"></script>
   <script type="text/javascript" src="<%=path%>/resources/js/GridRowEditForm.js"></script>
  	 <script type="text/javascript" src="<%=path%>/resources/js/GroupSummary.js"></script>
  	 <script type="text/javascript" src="<%=path%>/resources/js/RowExpander.js">
    </script>
   	 <script type="text/javascript" src="<%=path%>/device/PurchasingList.js"></script>
    <script type="text/javascript" src="<%=path%>/device/pList.js"></script>
     <%
    com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"com.bitc.xmis.device.PurchasingList.request");
  	%>
  </head>
  <body>
  </body>
</html>
