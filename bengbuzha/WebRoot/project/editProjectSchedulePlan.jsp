<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<%@page import="com.bitc.jk.xmis.model.ProjectObj"%>
<%
ProjectObj project=(ProjectObj)request.getAttribute("project");
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
    <script type="text/javascript" src="<%=path%>/resources/js/NavButtonPanel.js"></script>
    <script type="text/javascript" src="<%=path%>/project/ProjectSchedule.js"></script>
    <script type="text/javascript" src="<%=path%>/project/editProjectSchedulePlan.js"></script>
    <%
    com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"reuqest");
    %>
    <title>
      北京中水科技 X-MIS
    </title>
  </head>
  <body>
  </body>
</html>

