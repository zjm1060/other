<%@ page language="java" import="java.util.*" pageEncoding="GBK"%>
<%@taglib prefix="c" uri="http://java.sun.com/jstl/core"%>
<%
String rootpath = request.getContextPath();
String me_code=request.getRemoteUser();
request.setAttribute("me_code",me_code);
%>
<HTML>
<HEAD>
<link rel="stylesheet" type="text/css" href="<%=rootpath%>/stPortal/Common/Css/IM.css">
<SCRIPT LANGUAGE="JavaScript">
<!--
  //加载记录
  function onLoad()
  {
  reShow();
	  window.parent.parent.getToButton();
  }
  //每隔5分钟刷新记录
  function reShow()
  {
	  window.parent.parent.getRcvMsg();
  }
//-->
</SCRIPT>
</HEAD>
<BODY onload="reShow();onLoad();setInterval('reShow()',300000);" leftMargin="0" topMargin="0" scroll="no">
<div style="border: black 1px solid;width:100%;height:170px;overflow-y:auto;overflow:no;">
 <c:forEach begin="0" items="${newmsgs}" step="1" var="newmsg">
 <c:choose>
 <c:when test="${me_code eq newmsg.senderCode}">
 <font color="#242424">
 </c:when> 
 <c:otherwise>
 	<font color="#6B6B6B">
 </c:otherwise>
 </c:choose>
      [<c:out value="${newmsg.sendtime}"/> ] <c:out value="${newmsg.sender}"/> 说：</font>
 <c:choose>
 <c:when test="${me_code eq newmsg.senderCode}">
 	<font color="#5D478B">
 </c:when> 
 <c:otherwise>
 	<font color="#8B4C39">
 </c:otherwise>
 </c:choose>
  <c:out value="${newmsg.contenet}"/></font>
  <br>
</c:forEach>
</div>
</BODY>
</HTML>
