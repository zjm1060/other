<%@ page contentType="text/html; charset=GBK" %>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>Redister</title>
<link href="common/style.css" rel="stylesheet" type="text/css">
<script language="JavaScript">

function isCharsInBag (s, bag)
{ 
	var i,c;
	for (i = 0; i < s.length; i++)
	{ 
		c = s.charAt(i);//字符串s中的字符
		if (bag.indexOf(c) < 0) 
			return c;
	}
	return "";
}

function ischinese(s)
{
	var errorChar;
	var badChar = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	errorChar = isCharsInBag( s, badChar);
	if (errorChar != "" )
	{
		return false;
	}
	return true;
}
	

//定义表单输入校验函数
function isValid(){
     	
	if((form1.CODE.value.length>20))
	{	
	    alert("\登录帐户长度超过限制！");
		document.form1.CODE.focus();
		return false;
	}
	
	if(!ischinese(form1.CODE.value))   
	{
		alert("\登录帐户请输入字母数字组合！");
		document.form1.CODE.focus();
		return false;
	}

	if(form1.CODE.value=="")
	{
		document.form1.CODE.focus();
		alert("\请输入登陆帐户！");
		return false;
	}
	if( form1.pwd.value =="" ) 
	{
        alert("\请输入密码！");
        document.form1.pwd.focus();
        return false;
    }
	
	if((form1.pwd.value.length<5)||(form1.pwd.value.length>20))
	{//密码为5－20之间的数字或者字母
		alert("\密码长度不符合要求，请重新输入密码！");
		document.form1.pwd.focus();
		return false;
	}
	if( form1.repwd.value =="" ) 
	{
        alert("\请输入确认密码！");
        document.form1.repwd.focus();
        return false;
    }
	if(form1.pwd.value!=form1.repwd.value)
	{
		alert("\两次输入的密码不一致！");
		document.form1.repwd.focus();
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

<body>
<br>
<br>
<br>
<form method="post" action="registerServer.jsp" name="form1" onSubmit="return isValid();">
<table width="600"  border="0" align="center" cellpadding="0" cellspacing="0" style="BORDER-COLLAPSE: collapse">
  <tr>
    <td colspan="2"><table width="100%"  border="0" cellspacing="0" cellpadding="0">
      <TR>
            <TD vAlign=top width=7><IMG height=22 alt="" src="images/rnavleft.gif" width=10></TD>
            <TD width="100%" class=SubTitleRed2><img src="images/arrow_write.gif" width="15" height="15" border="0" align="absmiddle"> 用户注册 - 请仔细填写以下栏目，打*号的栏目必须填写。</TD>
            <TD vAlign=top width=7><IMG height=22 alt="" src="images/rnavright.gif" width=10></TD>
        </TR>
    </table></td>
  </tr>

  <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 登录帐户 <font color="red">*</font></td>
    <td class="Tabcell"><input name="CODE" type="text" class="textfield2" id="CODE" size="20" maxlength="20">
    0~20位 a-z;A-Z;0-9的字母数字组合 </td>
  </tr>
    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 密码 <font color="red">*</font></td>
    <td class="Tabcell"><input name="pwd" type="password" class="textfield2" id="pwd" size="20" maxlength="20">
5~20位 a-z;A-Z;0-9的字母数字组合 </td>
  </tr>
    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 确认密码 <font color="red">*</font></td>
    <td class="Tabcell"><input name="repwd" type="password" class="textfield2" id="repwd" size="20" maxlength="20"></td>
  </tr>
    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 个人用户姓名/企业名称 <font color="red">*</font></td>
    <td class="Tabcell"><input name="name" type="text" class="textfield2" id="name" size="50" maxlength="40"></td>
	</tr>
	    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 个人/法人身份证号码 <font color="red">*</font></td>
    <td class="Tabcell"><input name="identify" type="text" class="textfield2" id="identify" size="30" maxlength="40"></td>
  </tr>

  <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 电子邮件 <font color="red">*</font></td>
    <td class="Tabcell"><input name="mail" type="text" class="textfield2" id="mail" size="30" maxlength="30"></td>
  </tr>
    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 地址 <font color="red">*</font></td>
    <td class="Tabcell"><input name="address" type="text" class="textfield2" id="address" size="50" maxlength="100"></td>
  </tr>

    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 固定电话 <font color="red">*</font></td>
    <td class="Tabcell"><input name="tel" type="text" class="textfield2" id="tel" size="30" maxlength="20">
    </td>
  </tr>
    <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 移动电话</td>
    <td class="Tabcell"><input name="mobile" type="text" class="textfield2" id="mobile" size="30" maxlength="15"></td>
  </tr>
      <tr align="left">
    <td width="30%" class="Tabcell"  style="COLOR: black; BACKGROUND-COLOR: #f1f1f1"> 备注</td>
    <td class="Tabcell"><textarea name="remark" cols="50" rows="6" class="textfield2" id="remark"></textarea></td>
  </tr>
      <tr align="right">
    <td colspan="2" class="Tabcell">
	<input name="button" type="submit" class="GoBt" id="button" value="注册" >	
	<input name="button" type="reset" class="GoBt" id="button" value="取消" >	
	</td>
    </tr>
</table>
</form>
<script language="javascript">
	document.all.item("CODE").focus();
</script>
</body>
</html>
