<%@ page contentType="text/html; charset=GBK" %>
<jsp:useBean id="CaManager" scope="page" class="net.kingsoft.gdca.CaManager"/>

<%
	//更新证书
	
	
	System.out.println("更新证书更新证书11");
	String cavalue = "", usercode = "";
	if(request.getParameter("userCode") != null)
	{
		usercode = request.getParameter("userCode");
	}
	if(request.getParameter("cavalue") != null)
	{
		cavalue = request.getParameter("cavalue");
	}	
	
	System.out.println(usercode);
	

	System.out.println(usercode);	
	
	
	if(!usercode.equals(""))
	{
		boolean a = CaManager.UpdateUserCa(usercode, cavalue);
		System.out.println(a);
	}
%>

