<%@ page language="java" contentType="text/html;charset=utf-8"%>
<%@ taglib uri="/WEB-INF/struts-html.tld" prefix="html"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<!DOCTYPE HTML>
<%
String path = request.getContextPath();
String basePath = request.getScheme() + "://"
+ request.getServerName() + ":" + request.getServerPort()
+ path + "/";

String errMessage = (String) request
.getAttribute(WebKeys.Request_Attribute_Name_LoginFailed);
%>
<html>
  <head>
    <base href="<%=basePath%>">
    <title>
      北京中水科技 监控事业部 综合信息管理系统
    </title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <meta http-equiv="pragma" content="no-cache">
    <meta http-equiv="cache-control" content="no-cache">
    <meta http-equiv="expires" content="0">
    <LINK href="<%=path%>/resources/css/styleCIS.css" type=text/css rel=stylesheet>
  </head>
  <body>
  	<img src="<%=path%>/resources/images/making.gif"></img>
  </body>
</html>
