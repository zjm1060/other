<%@ page contentType="text/html; charset=gb2312" %>
<%@ page import="com.zctek.thridinterface.CustomerOperation" %>
<%@ page import="com.zctek.iaas.projectrecord.crm.dao.Pcustomer" %>

<%

	String usercode;
	usercode=request.getParameter("pUserName");

	  String id=""; //用户代码
    String e_name="";//申请人
	  String e_phone="";//单位电话
    String e_address="";//地址
    String e_post_code="";//邮编
    String e_org_code="";//组织机构代码
	  String e_enterprise_type="";//企业类型
    String e_register_fund="";//注册资金 投资规模
    String e_artificial_person="";//法人代表
    String e_mail="";//电子邮件
    String e_commission_name="";//代办人姓名
	  String e_commission_identity="";//代办人身份证
    String e_commission_phone="";//代办人电话
    String e_commission_mobile="";//代办人手机
    String pno="";//项目唯一编号
    String e_remark="";//备注
	  String NoNum="";//项目查询项

	CustomerOperation customerOperation=new CustomerOperation();
	Pcustomer pcustomer=customerOperation.getPcustomers(usercode);

	if(pcustomer!=null){
			//System.out.println("#==="+pcustomer.getId());
			id=pcustomer.getId();
			e_name=pcustomer.getE_name();
			e_phone=pcustomer.getE_phone();
			e_address=pcustomer.getE_address();
			e_post_code=pcustomer.getE_post_code();
			e_org_code=pcustomer.getE_org_code();
			e_enterprise_type=pcustomer.getE_enterprise_type();
			e_register_fund=pcustomer.getE_register_fund();
			e_artificial_person=pcustomer.getE_artificial_person();
			e_mail=pcustomer.getE_mail();
			e_commission_name=pcustomer.getE_commission_name();
			e_commission_identity=pcustomer.getE_commission_identity();
			e_commission_phone=pcustomer.getE_commission_phone();
			e_commission_mobile=pcustomer.getE_commission_mobile();
			e_remark=pcustomer.getE_remark();
		}

%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>用户资料</title>
<link href="/stPortal/images/new.css" rel="stylesheet" type="text/css">
<script>

function closeWin(){

			window.close();
}

function setUserCA(userCode){
			
			bfun1();
			
			var sn=document.all.item("sn").value;
	 		var cavalue=sn;

		if(cavalue!=("")){
			
			document.all.userCA.src="UserCa.jsp?userCode="+userCode+"&cavalue="+ cavalue;
			
		}
	 		
			

	}
</script>


</head>
<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0" rightmargin="0" bottommargin="0">
<iframe id="userCA" name="userCA" marginWidth="0" marginHeight="0" frameBorder="0" style="position:absolute;top:0px;left:0px;width:0px;height:0px;" src="UserCa.jsp"></iframe>
<OBJECT id=aax classid="clsid:E2558853-EEC6-4696-8836-D5B7C37FDBE9" codebase="../ActiveX/AuthnetAX.ocx"  width=0 height=0 align=center hspace=0 vspace=0 >
</OBJECT>
<table width="100%" height="100%" border="0" cellpadding="0" cellspacing="0">
  <tr>
    <td align="center" valign="top">
      <table width="650" border="0" cellpadding="0" cellspacing="0">
        <tr>
          <td align="center"> <p>&nbsp;</p>
            <table width="95%" border="0" align="center" cellpadding="3" cellspacing="1" bgcolor="#CCCCCC" class="zy1">
              <input name="modify"  type="hidden" value="1"/>
                <input name="e_type" type="hidden" value="单位">
                
              <tr align="left"> 
                <td height="25" colspan="2" class="topback"><font color="#FFFFFF"><strong><font color="#FFFFFF"><strong><img src="/stPortal/images/folder_inbox.gif" width="16" height="16" hspace="5" align="absmiddle"></strong></font>企业客户资料登记表</strong></font></td>
                </tr>
                
              <tr align="center"> 
                <td height="23" colspan="2" bgcolor="#f6f6f3"><strong><font color="#990000">为了保证顺利提交网上资料，请准确填写以下相关资料!</font></strong></td>
                </tr>
                
              <tr> 
                <td height="23" align="left" bgcolor="#FFFFFF">申请帐号:<br>
                  <font color="#666666">&nbsp; </font> </td>
                  
                <td width="493" height="23" bgcolor="#FFFFFF"> 
                  <input name="e_account" type="text" size="20" maxlength="20" value="<%=usercode%>" readonly>
                  <input type="button" value="绑定UserKey" onclick="setUserCA('<%=usercode%>');">
                  <input type="hidden" name="sn">
                </td>
                </tr>
                
              <!--tr> 
                <td width="143" height="23" align="left" bgcolor="f6f6f3"> 密 码:<br>
                </td>
                  
                <td height="23" bgcolor="f6f6f3"> 
                  <input name="e_password" type="password" size="20" maxlength="20" >
                  </td>
                </tr-->
                
              <tr> 
                <td height="23" align="left" bgcolor="#FFFFFF"> 企业名称:<br>
                </td>
                  
                <td height="23" bgcolor="#FFFFFF"> 
                  <input name="e_name" type="text" size="20" maxlength="40" value="<%=e_name%>" readonly>
                </td>
                </tr>
                
              <tr> 
                <td height="23" align="left" bgcolor="#f6f6f3"> 地址:<br>
                </td>
                  
                <td height="23" bgcolor="#f6f6f3"> 
                  <input name="e_address" type="text" size="50" maxlength="200" value="<%=e_address%>" readonly>
                  </td>
                </tr>
                
              <tr> 
                <td height="23" align="left" bgcolor="#FFFFFF"> 邮编:</td>
                  
                <td height="23" bgcolor="#FFFFFF"> 
                  <input name="e_post_code" type="text" size="20" maxlength="6" value="<%=e_post_code%>" readonly>
                  </td>
                </tr>
                
              <tr> 
                <td height="23" align="left" bgcolor="f6f6f3"> 固定电话:</td>
                  
                <td height="23" bgcolor="f6f6f3"> 
                  <input name="e_phone" type="text" size="20" maxlength="20" value="<%=e_phone%>" readonly>
                  </td>
                </tr>
				
              <tr> 
                <td height="23" align="left" bgcolor="#FFFFFF"> 组织机构代码:</td>
                  
                <td height="23" bgcolor="#FFFFFF"> 
                  <input name="e_org_code" type="text" size="20" maxlength="20" value="<%=e_org_code%>" readonly></td>
                </tr>
                
              <tr> 
                <td height="23" align="left" bgcolor="f6f6f3"> 企业类型:</td>
                  
                <td height="23" bgcolor="f6f6f3"> 
                  <input type="text" name="e_enterprise_type" value="<%=e_enterprise_type%>" readonly>
                </tr>
                
              <tr> 
                <td height="23" align="left" bgcolor="#FFFFFF"> 注册资金/投资规模:</td>
                  
                <td height="23" bgcolor="#FFFFFF"> 
                  <input name="e_register_fund" type="text" size="20" maxlength="20" value="<%=e_register_fund%>" readonly> 万元（人民币）
                  </td>
                </tr>
                
              <tr> 
                <td height="23" align="left" bgcolor="f6f6f3"> 法人代表名称:</td>
                  
                <td height="23" bgcolor="f6f6f3"> 
                  <input name="e_artificial_person" type="text" size="20" maxlength="20" value="<%=e_artificial_person%>" readonly></td>
                </tr>
                
              <tr> 
                <td height="23" align="left" bgcolor="#FFFFFF"> 代办人姓名:</td>
                  
                <td height="23" bgcolor="#FFFFFF"> 
                  <input name="e_commission_name" type="text" size="20" maxlength="20" value="<%=e_commission_name%>" readonly>
                  </td>
                </tr>
                
              <tr> 
                <td height="23" align="left" bgcolor="f6f6f3"> 代办人身份证号:</td>
                  
                <td height="23" bgcolor="f6f6f3"> 
                  <input name="e_commission_identity" type="text" size="20" maxlength="20" value="<%=e_commission_identity%>" readonly>
                  </td>
                </tr>
                
              <tr> 
                <td height="23" align="left" bgcolor="#FFFFFF"> 代办人手机:</td>
                  
                <td height="23" bgcolor="#FFFFFF"> 
                  <input name="e_commission_mobile" type="text" size="20" maxlength="20" value="<%=e_commission_mobile%>" readonly ></td>
                </tr>
                
              <tr> 
                <td height="23" align="left" bgcolor="f6f6f3"> 代办人电子邮箱:</td>
                  
                <td height="23" bgcolor="f6f6f3"> 
                  <input name="e_mail" type="text" size="20" maxlength="20" value="<%=e_mail%>" readonly >
                  </td>
                </tr>
                
              <tr align="center" bgcolor="#FFFFFF"> 
                <td height="40" colspan="2"> 
                  <table width="20%" height="25" border="0" cellpadding="0" cellspacing="0">
                      <tr align="center">
                        
                      <td width="50%" height="25"><span onclick="javascript:closeWin()"> 
                        <input name="button" type="button" onclick="window.close()" value="关闭">
                        </span></td>
                      </tr>
                    </table>
                </td>
                </tr>
            </table>

     </table>
    </td>
  </tr>
</table>
</body>
<script language="VBScript">
dim vart//输出值
dim data

function bfun1() 
	

  			ret = aax.GET_SERIALNO(vart)
  			
  			if ret <> 0 then
					msgbox("请插入CA卡！")
					exit function
				end if
				
				document.all.item("sn").value=vart

				
	
end function

</script>

</html>
