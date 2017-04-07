<!DOCTYPE HTML>
<%@page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@page import="com.bitc.jk.xmis.util.WebUtil"%>
<%@ page contentType="text/html; charset=utf-8"%>
<%
EmployeeObj e=WebUtil.getCurrentEmployee();
%>
<html>
<body style="text-align:center">
<div>正在跳转......</div>
<form action="http://192.169.11.14/login.php" id="f" method="post" style="display:none;">
<input name="username" value="<%=e.getLoginName() %>"/>
<input name="password" value="<%=e.getPassword().trim() %>"/>
</form>
<script type="text/javascript">
document.getElementById('f').submit();
</script>
</body>
</html>

