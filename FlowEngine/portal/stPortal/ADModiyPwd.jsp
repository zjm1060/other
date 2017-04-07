<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="java.util.*,
                 java.sql.*"
%>
<%
	String userName=request.getRemoteUser();
	if(userName==null) userName="";
%>

<script>
	var w=window.screen.width-10;
	var h=window.screen.height-55;
	var sStyle="width="+w+"px,height="+h+"px,left=0px,top=0px,scrollbars=yes,resizable=yes";
	window.location="http://10.197.32.200/commdeal/ChangePwd.asp?userName=<%=userName%>";
</script>