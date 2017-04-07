<!DOCTYPE HTML>
<%@page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@page import="com.bitc.jk.xmis.util.WebUtil"%>
<%@page import="com.bitc.jk.xmis.util.TemplateUtil"%>
<%@page import="com.bitc.jk.xmis.util.Type"%>
<%@ page contentType="text/html; charset=utf-8"%>
<%
EmployeeObj e=WebUtil.getCurrentEmployee();
Object userIDObj = e.getUserID();
int userIDInt = Type.GetInt(userIDObj);
String userIDStr = TemplateUtil.getUserIDStr(userIDInt);
%>
<html>
<body style="text-align:center">
<div>正在跳转......</div>
<form action="http://192.169.11.27/zentao/user-login.html" id="myform" method="post" style="display:none;">
<input name="account" value="<%=userIDStr %>"/>
<input name="password" value="222222"/>
<!-- 
<input name="password" value="<%=e.getPassword().trim() %>"/>
 -->
</form>
<script type="text/javascript">
document.getElementById('myform').submit();
</script>
</body>
</html>

