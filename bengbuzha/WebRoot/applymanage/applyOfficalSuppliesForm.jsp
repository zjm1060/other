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
     
    <script type="text/javascript" src="<%=path%>/applymanage/applyOfficalSuppliesForm.js"></script>
    
    <title>
 	审批列表
    </title>

     <script type="text/javascript">
     var isApplyPerson = com.bitc.xmis.existRole(__emp.roles,200);
    </script>
 
  </head>
  <body>
  
  </body>
</html>

