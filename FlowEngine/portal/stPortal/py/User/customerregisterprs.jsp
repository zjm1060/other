<%@page contentType="text/html; charset=gb2312"%>
<html>
<head>
<link href="/stPortal/images/new.css" rel="stylesheet" type="text/css">
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>客户资料登记表</title>
<script type="text/javascript">


function MM_jumpMenu(selObj){

	if(selObj.options[0].selected){
		document.all.item("e_enterprise_type").value="";
	}

	if(selObj.options[1].selected){
		document.all.item("e_enterprise_type").value="民营企业";
	}

	if(selObj.options[2].selected){
		document.all.item("e_enterprise_type").value="外资企业";
	}


}
function checkAccount(){
    	var account=document.forms["form1"].account;
    	if(account.value!=""){
      		document.frames["accountexists"].location.replace("accountexists.do?account="+e_account.value);
    	}
    	else{
      		account.focus();
    	}
  	}

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

function isNum(s)
{
	var errorChar;
	var badChar = "0123456789xX.";
	errorChar = isCharsInBag( s, badChar);
	if (errorChar != "" )
	{
		return false;
	}
	return true;
}

function isPhoneNum(s)
{
	var errorChar;
	var badChar = "0123456789-";
	errorChar = isCharsInBag( s, badChar);
	if (errorChar != "" )
	{
		return false;
	}
	return true;
}


//定义表单输入校验函数
function isValid(){

	if(form1.e_account.value=="")
	{//帐号为空
		document.form1.e_account.focus();
		alert("\请输入登陆帐户！");
		return false;
	}
	if((form1.e_account.value.length>20))
	{ //帐号大于
	    alert("\登录帐户长度不能大于20！");
		document.form1.e_account.focus();
		return false;
	}
	if(form1.e_account.value.length < 5)
	{//帐号少于
	    alert("\登录帐户长度不能小于5！");
		document.form1.e_account.focus();
		return false;
	}
	if(!ischinese(form1.e_account.value))
	{//帐号内容
		alert("\登录帐户请输入字母数字组合！");
		document.form1.e_account.focus();
	return false;
	}

	if( form1.e_password.value =="" )
	{//密码为空
        alert("\请输入密码！");
        document.form1.e_password.focus();
        return false;
    }
	if((form1.e_password.value.length<5)||(form1.e_password.value.length>20))
	{//密码为5－20之间的数字或者字母
		alert("\密码长度不符合要求，请重新输入密码！");
		document.form1.e_password.focus();
		return false;
	}
	if( form1.re_e_password.value =="" )
	{//确认码为空
        alert("\请输入确认密码！");
        document.form1.re_e_password.focus();
        return false;
    }
	if(form1.e_password.value!=form1.re_e_password.value)
	{//密码确认
		alert("\输入密码与确认密码不一致！");
		document.form1.re_e_password.focus();
		return false;
	}
	if((form1.e_name.value.length>40))
	{//企业名称长度
	    alert("\企业名称长度不能超过40！");
		document.form1.e_name.focus();
		return false;
	}
	if(form1.e_name.value=="")
	{//企业名称为空
		document.form1.e_name.focus();
		alert("\请输入企业名称！");
		return false;
	}
	if(form1.e_address.value=="")
	{//地址为空
		alert("\请输入地址！");
		document.form1.e_address.focus();
		return false;
	}
	if((form1.e_address.value.length>200))
	{//地址长度
	    alert("\地址度超过限制！");
		document.form1.e_address.focus();
		return false;
	}
	if(form1.e_post_code.value=="")
	{//邮编格式
	    alert("\邮编不能为空！");
		document.form1.e_post_code.focus();
		return false;
	}
	if(!isNum(form1.e_post_code.value))
	{//邮编格式
	    alert("\邮编必须是数字！");
		document.form1.e_post_code.focus();
		return false;
	}
	if(form1.e_phone.value=="")
	{//固定电话为空
		alert("\请输入固定电话号码！");
		document.form1.e_phone.focus();
		return false;
	}
	if(!isPhoneNum(form1.e_phone.value))
	{//固定电话格式
		alert("\输入的固定电话号码格式不对！");
		document.form1.e_phone.focus();
		return false;
	}
	if(form1.e_phone.value.length>20)
	{//固定电话长度
	    alert("\固定电话号码长度超过限制！");
		document.form1.e_phone.focus();
		return false;
	}
//	if(form1.e_org_code.value=="")
//	{//组织结构代码为空
//	    alert("\请输入组织机构代码！");
//		document.form1.e_org_code.focus();
//		return false;
//	}
//	if(form1.e_enterprise_type.value==""){

	//    	alert("\请选择企业类型！");
	//		return false;
	//}
	//if(form1.e_register_fund.value=="")
	//{
	//    alert("\请输入注册资金！");
	//	document.form1.e_register_fund.focus();
	//	return false;
	//}
	//if(!isNum(form1.e_register_fund.value))
	//{
	//    alert("\注册资金必须是数字！");
	//	document.form1.e_register_fund.focus();
	//	return false;
	//}
	//if(form1.e_artificial_person.value=="")
	//{
	//    alert("\请输入法人代表名称！");
	//	document.form1.e_artificial_person.focus();
	//	return false;
	//}


	if(form1.e_commission_name.value=="")
	{
	    alert("\请输入代办人姓名！");
		document.form1.e_commission_name.focus();
		return false;
	}
	if(form1.e_commission_identity.value=="")
	{
	    alert("\请填入代办人身份证号码！");
		document.form1.e_commission_identity.focus();
		return false;
	}
	if(!isNum(form1.e_commission_identity.value))
	{
	    alert("\代办人身份证号码必须是数字！");
		document.form1.e_commission_identity.focus();
		return false;
	}
	if(form1.e_commission_mobile.value==""){
	    alert("\代办人手机不能为空！");
		document.form1.e_commission_mobile.focus();
		return false;
	}
	if(!isNum(form1.e_commission_mobile.value))
	{
	   	alert("\代办人手机类型必须为数字！");
		document.form1.e_commission_mobile.focus();
		return false;
	}
	if(form1.e_mail.value=="")
	{
		alert("\请输入电子邮件地址！");
		document.form1.e_mail.focus();
		return false;
	}
	if((form1.e_mail.value.indexOf('@',0)==-1)||(form1.e_mail.value.indexOf('.',0)==-1)||(form1.e_mail.value.length<6))
	{
		alert("\请您输入合法的电子邮件地址！");
		document.form1.e_mail.focus();
		return false;
	}

}


	function submitForm(){

	if(isValid()!=false){

		document.form1.CODE.value=document.form1.e_account.value;
		document.form1.pwd.value=document.form1.e_password.value;
		document.form1.name.value=document.form1.e_name.value;
		document.form1.identify.value=document.form1.e_pidentity_card.value;
		document.form1.tel.value=document.form1.e_phone.value;
		document.form1.mobile.value=document.form1.e_mobile_phone.value;
		document.form1.mail.value=document.form1.e_mail.value;
		document.form1.address.value=document.form1.e_address.value;
		document.form1.remark.value=document.form1.e_remark.value;
		document.form1.submit();
	}

}
	function resetButton(){

		document.form1.e_account.value="";
		document.form1.e_password.value="";
		document.form1.re_e_password.value="";
		document.form1.e_name.value="";
		document.form1.e_address.value="";
		document.form1.e_post_code.value="";
		document.form1.e_phone.value="";
		document.form1.e_org_code.value="";
		document.form1.e_register_fund.value="";
		document.form1.e_artificial_person.value="";
		document.form1.e_commission_name.value="";
		document.form1.e_commission_identity.value="";
		document.form1.e_commission_mobile.value="";
		document.form1.e_mail.value="";

	}

	function checkUserCode(){

	    var userCode	=document.all.item("e_account").value;
		if(userCode!=""){
			if(userCode.indexOf(" ")!=-1){
				alert("帐号中不得包含空格，请重新输入！");
			}else{
				var path="checkUserCode.jsp?userCode="+userCode;
			   	var strFeatures = "dialogWidth:300px;dialogHeight:200px;center:yes;status:no;scroll:no";
			   	var msg=window.showModalDialog(path,window,strFeatures);
			}
		}
}




  </script>
<link href="css/common.css" rel="stylesheet" type="text/css">
</head>

<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0" rightmargin="0" bottommargin="0">
<table width="100%" height="100%" border="0" cellpadding="0" cellspacing="0">
  <tr>
    <td align="center"><br>
      <table width="650" border="0" cellpadding="0" cellspacing="0">
        <tr>
          <td align="center"> <table width="650" border="0" align="center" cellpadding="3" cellspacing="1" bgcolor="#669966" class="zy1">
              <form method="post" action="registerServer.jsp" name="form1" focus="e_account">
                <input name="modify"  type="hidden" value="1"/>
                <input name="e_type" type="hidden" value="单位">
                <tr bgcolor="#CCFFCC">
                  <td height="25" colspan="2" align="center"><font color="#006600" size="3"><strong>客户资料登记表</strong></font></td>
                </tr>
                <tr align="center" bgcolor="#F6f6f3">
                  <td height="25" colspan="2"><strong><font color="#990000">为了保证顺利提交网上资料，请准确填写以下相关资料!</font></strong></td>
                </tr>
                <tr bgcolor="#FFFFFF">
                  <td height="25" align="left">申请帐号:<br> <font color="#666666">
                    5~20位a-z;A-Z;0-9的字母数字组合 </font> </td>
                  <td height="25"> <input name="e_account" type="text" size="20" maxlength="20">
                    <font color="#FF0000">*</font>　　 
                    <input name="button" type="button" onClick="checkUserCode();" value="检测帐号">
                  </td>
                </tr>
                <tr bgcolor="f6f6f3">
                  <td width="211" height="25" align="left"> 密 码:<br> <font color="#666666">5~20位
                    a-z;A-Z;0-9的字母数字组合</font> </td>
                  <td height="25"> <input name="e_password" type="password" size="20" maxlength="20">
                    <font color="#FF0000">*</font> </td>
                </tr>
                <tr bgcolor="#FFFFFF">
                  <td width="211" height="25" align="left"> 确认密码:<br> <font color="#FF0000"><font color="#666666">5~20位
                    a-z;A-Z;0-9的字母数字组合</font> </font> </td>
                  <td height="25"> <input name="re_e_password" type="password" size="20" maxlength="20">
                    <font color="#FF0000">*</font> </td>
                </tr>
                <tr bgcolor="f6f6f3">
                  <td height="25" align="left"> 企业名称:<br> <font color="#666666">请输入您的企业名称</font>
                  </td>
                  <td height="25"> <input name="e_name" type="text" size="20" maxlength="40">
                    <font color="#FF0000">*</font> </td>
                </tr>
                <tr bgcolor="#FFFFFF">
                  <td height="25" align="left"> 地址:<br> <font color="#666666">请输入您有效的联系地址</font><font color="red">&nbsp;</font>
                  </td>
                  <td height="25"> <input name="e_address" type="text" size="50" maxlength="200">
                    <font color="#FF0000">*</font> </td>
                </tr>
                <tr bgcolor="f6f6f3">
                  <td height="25" align="left"> 邮编:</td>
                  <td height="25"> <input name="e_post_code" type="text" size="20" maxlength="6">
                    <font color="#FF0000">*</font> </td>
                </tr>
                <tr bgcolor="#FFFFFF">
                  <td height="25" align="left"> 固定电话:</td>
                  <td height="25"> <input name="e_phone" type="text" size="20" maxlength="20">
                    <font color="#FF0000">*</font> </td>
                </tr>
                <tr bgcolor="#f6f6f3">
                  <td height="25" align="left"> 组织机构代码:</td>
                  <td height="25"> <input name="e_org_code" type="text" size="20" maxlength="20">
                   </td>
                </tr>
                <tr bgcolor="#FFFFFF">
                  <td height="25" align="left"> 企业类型:</td>
                  <td height="25"> <input type="hidden" name="e_enterprise_type" >
                    <select name="select_enterprise_type" onChange="MM_jumpMenu(this)">
                      <option selected>------请选择------</option>
                      <option>民营企业</option>
                      <option>外资企业</option>
                    </select>
                    </td>
                </tr>
                <tr bgcolor="#F6f6f3">
                  <td height="25" align="left"> 注册资金/投资规模:</td>
                  <td height="25"> <input name="e_register_fund" type="text" size="20" maxlength="20">
                    万元（人民币） </td>
                </tr>
                <tr bgcolor="#FFFFFF">
                  <td height="25" align="left"> 法人代表名称:</td>
                  <td height="25"> <input name="e_artificial_person" type="text" size="20" maxlength="20">
                    </td>
                </tr>
                <tr bgcolor="#F6f6f3">
                  <td height="25" align="left"> 代办人姓名:</td>
                  <td height="25"> <input name="e_commission_name" type="text" size="20" maxlength="20">
                    <font color="#FF0000">*</font> </td>
                </tr>
                <tr bgcolor="#FFFFFF">
                  <td height="25" align="left"> 代办人身份证号:</td>
                  <td height="25"> <input name="e_commission_identity" type="text" size="20" maxlength="20">
                    <font color="#FF0000">*</font> </td>
                </tr>
                <tr bgcolor="#F6f6f3">
                  <td height="25" align="left"> 代办人手机:</td>
                  <td height="25"> <input name="e_commission_mobile" type="text" size="20" maxlength="20">
                    <font color="#FF0000">*</font></td>
                </tr>
                <tr bgcolor="#FFFFFF">
                  <td height="25" align="left"> 代办人电子邮箱:</td>
                  <td height="40" bgcolor="#FFFFFF">
                    <input name="e_mail" type="text" size="20" maxlength="20">
                    <font color="#FF0000">*</font> </td>
                </tr>
                <tr align="center" bgcolor="f6f6f3">
                  <td height="40" colspan="2"> <table width="20%" height="25" border="0" cellpadding="0" cellspacing="0">
                      <tr align="center">
                        <td width="50%" height="25"><a href="javascript:submitForm()"><img src="./images/1.GIF" width="50" height="25" border="0"></a></td>
                        <td width="50%" height="25"><a href="javascript:resetButton()"><img src="./images/2.GIF" width="50" height="25" border="0"></a></td>
                      </tr>
                    </table></td>
                </tr>
                  <input name="e_pidentity_card" type="hidden" size="20" value="">
                  <input name="e_mobile_phone" type="hidden" size="20" value="">
                  <input name="e_commission_phone" type="hidden" size="20" value="">
                  <input name="e_remark" type="hidden" value="">
                <input type="hidden" name="CODE" value="">
                <input type="hidden" name="pwd" id="pwd" value="">
                <input type="hidden" name="name" id="name" value="">
                <input type="hidden" name="identify" id="identify" value="">
                <input type="hidden" name="mail" id="mail" value="">
                <input type="hidden" name="address" id="address" value="">
                <input type="hidden" name="tel" id="tel" value="">
                <input type="hidden" name="mobile" id="mobile" value="">
                <input type="hidden" name="remark" id="remark" value="">
              </form>
            </table>

     </table></td></tr>

</table>
</body>
</html>
