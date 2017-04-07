<%@ page import="java.util.Date" %>
<%@ page import="java.text.SimpleDateFormat" %>
<%@ page import="java.util.TimeZone" %>
<%@ page import="java.util.Enumeration" %>
<%@ page import="java.util.Properties" %>
<%@ page contentType="text/html; charset=GBK" %>
<%
    Date date = new Date();
    SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    final Properties p = System.getProperties();
    final Enumeration e = p.keys();
    while (e.hasMoreElements())
    {
        final String prt = (String) e.nextElement();
        final String prtvalue = System.getProperty(prt);
        out.println(prt + ":" + prtvalue);
        out.print("<br>");
    }
%>
<%=sdf.format(date)%>
<br>
<%=TimeZone.getDefault().getDisplayName()%>

