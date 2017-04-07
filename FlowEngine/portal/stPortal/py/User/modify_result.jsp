<%@ page contentType="text/html; charset=GBK" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>Redister-Infomation</title>
<link href="common/style.css" rel="stylesheet" type="text/css">
</head>

<body>
<%
String m_result;
m_result=request.getParameter("regReturn");
%>
<table width="400"  border="0" align="center" cellpadding="0" cellspacing="0" style="BORDER-COLLAPSE: collapse">
  <tr>
    <td colspan="2"><table width="100%"  border="0" cellspacing="0" cellpadding="0">
      <TR>
            <TD vAlign=top width=7><IMG height=22 alt="" src="images/rnavleft.gif" width=10></TD>
            <TD width="100%" class=SubTitleRed2><img src="images/arrow_write.gif" width="15" height="15" border="0" align="absmiddle"> 注册信息</TD>
            <TD vAlign=top width=7><IMG height=22 alt="" src="images/rnavright.gif" width=10></TD>
        </TR>
    </table></td>
  </tr>

  <tr align="left">
    <td height="160" colspan="2" align="center" style="BORDER: #cccccc 1px solid; COLOR: black; BACKGROUND-COLOR: #f1f1f1">
    <%
    	if(m_result.equals("0"))
    	{
    %>
    用户资料修改成功！
    <%
    	}
    	else if(m_result.equals("51"))
    	{
    %>
	数据库错误
	<%
		}
		else if(m_result.equals("52"))
		{
	%>
	密码或用户名错误
	<%
		}
		else if(m_result.equals("53"))
		{
	%>
	数据库引擎错误
	<%
		}
		else if(m_result.equals("120"))
		{
	%>
	用户存在
	<%
		}
		else if(m_result.equals("123"))
		{
	%>
	身份证检查错误
	<%
		}
		else
		{
		System.out.println(m_result);
	%>
	未知错误
	<%
		}
	%>
    </td>
  </tr>
  <tr align="left">
  <%
  	if(m_result.equals("0"))
    {
  %>
    <td height="30" colspan="2" align="center" style="BORDER: #cccccc 1px solid; COLOR: black; BACKGROUND-COLOR: #f1f1f1">你可以登录并提交、查询您的审批请求。 <a href="modify_user.jsp">返回..</a></td>
  <%
  	}
  	else
  	{
  %>
    <td height="30" colspan="2" align="center" style="BORDER: #cccccc 1px solid; COLOR: black; BACKGROUND-COLOR: #f1f1f1"> <a href="modify_user.jsp">返回修改..</a></td>
  <%
  	}
  %>
  </tr>
</table>

</body>
</html>
