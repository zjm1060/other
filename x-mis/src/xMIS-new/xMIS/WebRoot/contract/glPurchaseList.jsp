<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<!DOCTYPE HTML>

<script language="javascript">



function showGLPurchaseList(contractAbb, contractID,type){
	showGLPurchaseListWindow(contractAbb, contractID,type);
}



</script>
<html>
  <head>
    <title>My JSP 'paymentList.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<%
	String path = request.getContextPath();
	if("/".equals(path)){
		path="";
	}
%>
	<script language="JavaScript" src="<%=path %>/contract/glPurchaseList.js"></script>

  </head>
</html>
