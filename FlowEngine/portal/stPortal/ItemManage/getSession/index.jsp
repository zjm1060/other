<%@ page contentType="text/html; charset=gb2312" %>
<!-- Copyright (c) 2002 by ObjectLearn. All Rights Reserved. -->
<%
String strName = null;
String strThing = null;
try {
    strName = request.getParameter("name");
    strThing = request.getParameter("thing");
    if ((strName == null) || (strName.length()==0)){
    	throw new Exception("null strName");
    }
    if ((strThing == null) || (strThing.length()==0))
    	throw new Exception("null strThing");
    // add session
    session.setAttribute("name",strName);
    session.setAttribute("thing",strThing);
    response.sendRedirect("display.jsp");
} catch (Exception e) { }
%>
<html>
<head>
<title>Welcome</title>
</head>
<body>
<center>Welcome</center>
<form method='post' >
<table align='center'>
  <tr>
    <td>name:</td>
    <td> <input name='name' type='input'/> </td>
  </tr>
  <tr>
    <td>thing:</td>
    <td> <input name='thing' type='input'/> </td>
  </tr>
  <tr>
    <td align='right'>  </td>
    <td align='right'>
    <button type='submit'>submit</button>
    <button type='reset'>reset</button>
  </td>
  </tr>
</table>
</form>
</body>
</html>


