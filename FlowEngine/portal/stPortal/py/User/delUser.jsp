<%@ page contentType="text/html; charset=GBK" %>
<jsp:useBean id="UserConform" scope="page" class="net.kingsoft.user.UserConform"/>
<%

	String mUserName;
	mUserName=request.getParameter("pUserName");

	if(mUserName!=null && !mUserName.equals("") )
		UserConform.delUser(mUserName);
		
%>
<script>
	alert("\该账号已经删除成功！");
	window.returnValue = "ok";
	window.close();
</script>
