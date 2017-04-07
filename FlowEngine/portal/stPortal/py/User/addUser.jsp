<%@ page contentType="text/html; charset=GBK" %>
<html>
<head>
<meta http-equiv="Refresh" CONTENT="2;URL=userConform.jsp">
<script language="JavaScript">
//定义表单输入校验函数
function isOK()
{
	alert("\审核通过！");
	return true;
}
function isError()
{
	alert("\该账号已经审核通过！");
	return true;
}   
   
</script>
</head>
<jsp:useBean id="UserConform" scope="page" class="net.kingsoft.user.UserConform"/>
<%
String mUserName,mPWD;
int i=1;
mUserName=request.getParameter("pUserName");
mPWD=request.getParameter("pPWD");
if(mUserName!=null && !mUserName.equals("") && mPWD!=null &&!mPWD.equals(""))
	i=UserConform.addUser(mUserName,mPWD);
if(i==0)
{
%>
<body onload="isOK();">
<%
}
if(i==1)
{
%>
<body onload="isError();">
<%
}
%>
</html>