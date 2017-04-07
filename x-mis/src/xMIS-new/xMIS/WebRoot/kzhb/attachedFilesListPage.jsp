<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <script type="text/javascript" src="<%=path %>/resources/js/MultiSearchPanel.js"></script>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
    
    <input type=hidden id="attID" name="attID" value="" />
    <script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/conInfoObjCom.js"></script>
    <script type="text/javascript" src="<%=path%>/kzhb/attachedFilesList.js"></script>
    <title>
    </title>
  </head>
  <body>
  </body>
</html>

