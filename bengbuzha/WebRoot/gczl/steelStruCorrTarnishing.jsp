<%@ page language="java" pageEncoding="utf-8"%>
<!DOCTYPE HTML>
<html>
  <head>
    <%@include file="../jsCss.jsp" %>
    <script type="text/javascript" src="<%=path%>/resources/js/SearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/InklingSearchField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/ChooseOrgTreeField.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/conInfoObjCom.js"></script>
   <script type="text/javascript" src="<%=path%>/gczl/steelStruCorrTarnishingSearch.js"></script>
   <script type="text/javascript" src="<%=path%>/gczl/steelStruCorrTarnishing.js"></script>
<title>Insert title here</title>
</head>
<body>
 <%
    com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,"request");
    %>
</body>
</html>