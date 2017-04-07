<%@page contentType="text/html; charset=utf-8"%>
<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<% 
String params=request.getParameter("params");
%>
<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
    <script type="text/javascript" src="<%=path%>/attendance/extraInfo.js"></script>
    <script type="text/javascript" src="<%=path%>/attendance/missAttendanceLists.js"></script>
    <script type="text/javascript">
 var params='<%=params%>';
    </script>
    <title>
      一级部门领导审批
    </title>

  </head>
  <body>
  
  </body>
</html>

