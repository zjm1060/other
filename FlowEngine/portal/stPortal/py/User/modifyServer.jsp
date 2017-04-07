<%@ page contentType="text/html; charset=GBK" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<jsp:useBean id="userfile" scope="page" class="net.kingsoft.user.Userfile"/>

<html>
<jsp:setProperty name="userfile" property="FULL_NAME" param="name"/>
<jsp:setProperty name="userfile" property="USER_ADDR" param="address"/>
<jsp:setProperty name="userfile" property="USER_IDEN" param="identify"/>
<jsp:setProperty name="userfile" property="USER_TEL" param="tel"/>
<jsp:setProperty name="userfile" property="MOBI_TEL" param="mobile"/>
<jsp:setProperty name="userfile" property="USER_DESC" param="remark"/>
<jsp:setProperty name="userfile" property="E_MAIL" param="mail"/>

<%	
	String m_usercode = (String)session.getAttribute(suntek.security.Security.m_sUser);
	String m_username = oManager.getCurrentUserName();

    userfile.setUSER_CODE(m_usercode);
    int regReturn = 1;
	String err;
	
	//判断是否是默认用户
	if(!m_usercode.equals("guest"))
	{
		regReturn = userfile.updateUser();
		//返回码等于0，修改成功
		err="modify_result.jsp?regReturn="+Integer.toString(regReturn);
	}
	else
	{
		//返回码等于123，身份检查错误
		err="modify_result.jsp?regReturn=123";
	}
		
	
%>
	<jsp:forward page="<%=err%>"/>
</html>

