<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.*" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
          String m_usercode=(String)(request.getRemoteUser());
          String m_username =oManager.getCurrentUserName();
          String m_NodeValue = request.getParameter("HNodeValue");
          int sReturn = oManager.sCanDeletePersonfunc(m_usercode,m_NodeValue);
%>
<SCRIPT type="text/javascript">
<!--

function window_onload()
{
   var s_return = '<%=sReturn%>';
   var bReturn;
   var bReturn;
   if (s_return == 1)
       bReturn=confirm("确定要删除节点["+'<%=request.getParameter("NodeDesc")%>'+"]吗?");
   else
       bReturn=confirm("本功能在模块设置中已经使用,确定要删除节点["+'<%=request.getParameter("NodeDesc")%>'+"]吗?");
   if (bReturn==true)
   {
   	window.DCForm.action="_FunctionModi.jsp?m_Operation=del";
   	window.DCForm.submit();
   }
}
window.attachEvent("onload",window_onload);
//-->

//-->

</SCRIPT>
		<form ACTION="FunctionMain.jsp" METHOD="post" id="DCForm" name="DCForm">
			<input type=hidden name="HNodeValue" value="<%=request.getParameter("HNodeValue")%>">
			<input type=hidden name="HNodeName" value="<%=request.getParameter("HNodeName")%>">
			<input type=hidden name="HNodeDesc" value="<%=request.getParameter("HNodeDesc")%>">
		</form>

