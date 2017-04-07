<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="baseserverjb.BaseServerClient"%>
<html>
<head>
<title>
清除系统缓存
</title>
</head>
<body bgcolor="#ffffff">
<%
	String actionFlag=request.getParameter("actionFlag");
	if(actionFlag==null) actionFlag="";
	if(!actionFlag.equals("clear")){
%>
<form method="post" name="frmClearCache">
<table width="100%" height="100%">
<tr>
<td width="100%" height="100%" align="center" valign="middle">
<input name="actionFlag" type="hidden" value="clear">
<input name="clearCache" type="submit" value="清除系统缓存">
</td>
<tr>
</table>
</form>
<%
}else{
%>
<table width="100%" height="100%">
<tr>
<td width="100%" height="100%" align="center" valign="middle">
<%
	BaseServerClient bsc=new BaseServerClient();
    bsc.clearCache();
	%>
	清除缓存完成!
	<%
%>
</td>
<tr>
</table>
<%}
%>
</body>
</html>
