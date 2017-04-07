<%@ page contentType="text/html; charset=GBK" %>
<html>
<jsp:useBean id="userfile" scope="page" class="net.kingsoft.user.Userfile"/>
<jsp:setProperty name="userfile" property="USER_CODE" param="CODE"/>
<jsp:setProperty name="userfile" property="USER_PWD" param="pwd"/>
<jsp:setProperty name="userfile" property="FULL_NAME" param="name"/>
<jsp:setProperty name="userfile" property="USER_ADDR" param="address"/>
<jsp:setProperty name="userfile" property="USER_IDEN" param="identify"/>
<jsp:setProperty name="userfile" property="USER_TEL" param="tel"/>
<jsp:setProperty name="userfile" property="MOBI_TEL" param="mobile"/>
<jsp:setProperty name="userfile" property="E_MAIL" param="mail"/>
<jsp:setProperty name="userfile" property="USER_DESC" param="remark"/>
<%
    int regReturn = 1;
	String err;
	regReturn = userfile.insertUser();
	
	//返回码等于0，注册成功
	err="re_result.jsp?regReturn="+Integer.toString(regReturn);
%>
	<jsp:forward page="<%=err%>"/>

</html>

