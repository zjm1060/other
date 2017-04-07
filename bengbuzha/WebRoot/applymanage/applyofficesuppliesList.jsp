<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
   <script type="text/javascript" src="<%=path%>/resources/js/SearchField.js"></script>
       <script type="text/javascript" src="<%=path%>/resources/js/InklingSearchField.js"></script>
       <script type="text/javascript" src="<%=path%>/applymanage/approvalWindow.js"></script>
     
    <script type="text/javascript" src="<%=path%>/applymanage/applyOfficalSuppliesForm.js"></script>
    <script type="text/javascript" src="<%=path%>/applymanage/applyofficesuppliesList.js"></script>
    
    <title>
 	审批列表
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

