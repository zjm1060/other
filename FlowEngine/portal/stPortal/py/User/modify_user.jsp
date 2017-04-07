<%@ page contentType="text/html; charset=GBK" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
"http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>用户资料修改</title>
<link href="common/style.css" rel="stylesheet" type="text/css">
<script language="JavaScript">
<!--定义表单输入校验函数-->
function isValid(){
     	
	if((form1.CODE.value.length>20))
	{	
	    alert("\帐户长度超过限制！");
		document.form1.CODE.focus();
		return false;
	}   

	if(form1.CODE.value=="")
	{
		document.form1.CODE.focus();
		alert("\请输入登陆帐户！");
		return false;
	}
	if((form1.name.value.length>40))
	{	
	    alert("\姓名/企业名称长度超过限制！");
		document.form1.name.focus();
		return false;
	}   
	if(form1.name.value=="")
	{
		document.form1.name.focus();
		alert("\请输入个人用户姓名/企业名称！");
		return false;
	}
	if(form1.identify.value=="")
	{
		document.form1.identify.focus();
		alert("\请输入个人/法人身份证号码！");
		return false;
	}
	if((form1.identify.value.length>40))
	{	
	    alert("\个人/法人身份证号码长度超过限制！");
		document.form1.identify.focus();
		return false;
	}
	if(form1.mail.value=="")
	{
		alert("\请输入电子邮件地址！");
		document.form1.mail.focus();
		return false;
	}
	
	if((form1.mail.value.indexOf('@',0)==-1)||(form1.mail.value.indexOf('.',0)==-1)||(form1.mail.value.length<6))
	{
		alert("\请您输入合法的电子邮件地址！");
		document.form1.mail.focus();
		return false;
	}
	if(form1.address.value=="")
	{
		alert("\请输入地址！");
		document.form1.address.focus();
		return false;
	}

	if((form1.address.value.length>200))
	{	
	    alert("\地址度超过限制！");
		document.form1.address.focus();
		return false;
	}
	if(form1.tel.value=="")
	{
		alert("\请输入固定电话号码！");
		document.form1.tel.focus();
		return false;
	}
	
	if((form1.tel.value.length>20))
	{	
	    alert("\电话号码长度超过限制！");
		document.form1.tel.focus();
		return false;
	}
	
	if((form1.remark.value.length>200))
	{	
	    alert("\备注长度超过限制！");
		document.form1.remark.focus();
		return false;
	}
}
</script>
</head>
<jsp:useBean id="userfile" scope="session" class="net.kingsoft.user.Userfile"/>
<!--接收上级页面传递的用户名-->
<!--提取并存放用户信息以备显示-->
<%
String m_usercode = (String)session.getAttribute(suntek.security.Security.m_sUser);
String m_username = oManager.getCurrentUserName();
String err = "";

//判断是否是默认用户
if(!m_usercode.equals("guest"))
{
	userfile.setUSER_CODE(m_usercode);
	userfile.selectUser();
}
else
{
	//返回码等于123，身份检查错误
	err="modify_result.jsp?regReturn=123";
%>
	<jsp:forward page="<%=err%>"/>
<%	
}
%>

<!--使用其属性显示数据库原有内容-->


<body>
<form method="post" action="modifyServer.jsp" name="form1" onSubmit="return isValid();">
<table width="600"  border="0" align="center" cellpadding="0" cellspacing="0" style="BORDER-COLLAPSE: collapse">
  <tr>
    <td colspan="2"><table width="100%"  border="0" cellspacing="0" cellpadding="0">
      <TR>
            <TD vAlign=top width=7><IMG height=22 alt="" src="images/rnavleft.gif" width=10></TD>
            <TD width="100%" class=SubTitleRed2><img src="images/arrow_write.gif" width="15" height="15" border="0" align="absmiddle"> 用户注册资料修改 - 请仔细填写以下栏目，打*号的栏目必须填写。</TD>
            <TD vAlign=top width=7><IMG height=22 alt="" src="images/rnavright.gif" width=10></TD>
        </TR>
    </table></td>
  </tr>

  <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 登录帐户</td>
    <td class="Tabcell"><input name="CODE" type="text" class="textfield2" id="CODE" size="30" maxlength="20" value="<%=userfile.getUSER_CODE()%>" readonly></td>
  </tr>
    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 个人用户姓名/企业名称*</td>
    <td class="Tabcell"><input name="name" type="text" class="textfield2" id="name" size="50" maxlength="40" value="<%=userfile.getFULL_NAME()%>"></td>
	</tr>
	    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 个人/法人身份证号码*</td>
    <td class="Tabcell"><input name="identify" type="text" class="textfield2" id="identify" size="30" maxlength="40" value="<%=userfile.getUSER_IDEN()%>"></td>
  </tr>

  <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 电子邮件*</td>
    <td class="Tabcell"><input name="mail" type="text" class="textfield2" id="mail" size="30" maxlength="30" value="<%=userfile.getE_MAIL()%>"></td>
  </tr>
    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 地址*</td>
    <td class="Tabcell"><input name="address" type="text" class="textfield2" id="address" size="50" maxlength="100" value="<%=userfile.getUSER_ADDR()%>"></td>
  </tr>

    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 固定电话*</td>
    <td class="Tabcell"><input name="tel" type="text" class="textfield2" id="tel" size="30" maxlength="20" value="<%=userfile.getUSER_TEL()%>">
    </td>
  </tr>
    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 移动电话</td>
    <td class="Tabcell"><input name="mobile" type="text" class="textfield2" id="mobile" size="30" maxlength="15" value="<%=userfile.getMOBI_TEL()%>"></td>
  </tr>
      <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 备注</td>
    <td class="Tabcell"><textarea name="remark" cols="50" rows="6" class="textfield2" id="remark"><%=userfile.getUSER_DESC()%></textarea></td>
  </tr>
      <tr align="right">
    <td colspan="2" class="Tabcell">
	<input name="button" type="submit" class="GoBt" id="button" value="修改" >	
	<input name="button" type="reset" class="GoBt" id="button" value="取消" >	
	</td>
    </tr>
</table>
</form>
</body>
</html>
