<%@ page contentType="text/html; charset=GB2312" %>
<%@ page import="java.io.*"%>
<%
 request.setCharacterEncoding("GBK");
 String user=request.getRemoteUser();
 if(user==null ) user="";
 String fun=request.getParameter("fun");
 if(fun==null ) fun="";
%>
<script>
if("<%=fun%>" == "所有"){
	window.location="http://gzg-mail/exchange/";
}
else if ( "<%=fun%>" == "公用文件夹" ) {	
	window.location="http://10.197.32.200/public";
}
else{
	window.location="http://10.197.32.200/exchange/<%=user%>/<%=fun%>/?Cmd=contents";
}
</script>