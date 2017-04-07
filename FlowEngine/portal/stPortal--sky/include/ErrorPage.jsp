<%@ page contentType="text/html;charset=gb2312" %>
<html>
<head>
<title>JSP页面错误捕获</title>
<head>

<%
Integer statusCode = null;
String message = null;
Exception excep = null; 
Throwable t = null; //for Exceptions thrown by another JSP

//try to get the HTTP status code OR the Exception thrown by a Servlet
statusCode = (Integer) request.getAttribute("javax.servlet.error.status_code");
message = (String) request.getAttribute("javax.servlet.error.message");
excep = (Exception) request.getAttribute("javax.servlet.error.exception_type");

String problem, explanation;

//try to get the exception thrown by a JSP (note: the implicit "exception" could be used here instead)
t = (Throwable) request.getAttribute("javax.servlet.jsp.jspException");

if(statusCode != null)
{
message = (message == null ? "No description available." : message);

problem = "HTTP Status Code - " +statusCode.intValue();
explanation = message;
}

//process an exception thrown by a Servlet (not a JSP)
else if(excep != null)
{
problem = "An Exception was thrown while trying to process your request";
explanation = excep.toString();
}

//process an exception thrown by another JSP (note: here is where a test using the implicit "exception"
//such as exception != null, followed by explanation = exception.getMessage() could be used instead).

else if(t != null)
{
problem = "An Exception was thrown while trying to process your request";
explanation = t.toString();
}

else
{
problem = "Unknown";
explanation = "The exact cause of the error could not be determined";
}
%>

<body bgcolor=#ffffff>
<div style="margin: 2%">
<p class="errorText">
发生了以下的错误：
</p>

<p></p>


<table border=1 width="96%" cellspacing=2 cellpadding=6 bgcolor=#ffcc66>
<tr>
<th>问题描述</th>
<th>问题解释</th>
</tr>

<tr>
<td align="center" bgcolor=#ffffee class="normalText">
<%=problem%>
</td>

<td bgcolor=#ffffee class="normalText">
<%=explanation%>
</td>
</tr>

</table> 
<!--
<p>Go to the <a href="<%//=request.getContextPath() + "/admin"%>">admin UI for <%//=request.getContextPath().substring(1)%></a>
-->
</div>
</body>

</html> 