<%@ page import="bitc.sz.wzEnv.util.WebUtil" %>
<%--
  Created by IntelliJ IDEA.
  User: zsz
  Date: 2016/2/19
  Time: 8:43
  To change this template use File | Settings | File Templates.
--%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<%
  String path = WebUtil.getContextPath(request);
%>
<script>
  var __path = '<%=path%>'
</script>
<html>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<head>
  <script type="text/javascript" src="extjs/ext-all-debug.js"></script>
  <link rel="stylesheet" type="text/css" href="extjs/resources/css/ext-all-neptune.css">
  <link rel="stylesheet" type="text/css" href="resources/css/icon.css">
  <script type="text/javascript" src="extjs/locale/ext-locale-zh_CN.js"></script>
  <script type="text/javascript" src="js/Util.js"></script>
  <script type="text/javascript" src="js/login.js"></script>
    <title>登录</title>
</head>
<body>
<style type="text/css">
  body{
    background: url("<%=path%>/resources/image/user_all_bg.gif")  #226cc5 repeat-x 50% top !important;
  }
</style>
</body>
</html>
