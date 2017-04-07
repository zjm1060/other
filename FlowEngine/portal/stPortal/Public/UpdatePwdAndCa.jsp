<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="baseserverjb.BaseServerClient" %>
<%
  //取得路径变量
  String sCommonPath="",sPublicPath="";
  sCommonPath ="../Common/";
  BaseServerClient bsc=new BaseServerClient(request.getRemoteUser());
%>
<script type="text/javascript" src="<%=sCommonPath%>Include/Common.js"></script>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<OBJECT ID="SafeEngineCtl" CLASSID="CLSID:B48B9648-E9F0-48A3-90A5-8C588CE0898F" width="0" height="0" border=0 codebase="SafeEngineCOM.cab#Version=1,2,0,0" ></OBJECT>
<%
  String action = request.getParameter("action");
  if(action != null && action.equals("1")){
      String oldPassword = request.getParameter("old_pwd");
      String newPassword = request.getParameter("new_pwd");
      boolean success = false;
      try{success=bsc.getFramework().updatePwd(oldPassword,newPassword);}catch(Exception ex){ex.printStackTrace();}
      if(!success){%>
        <script type="text/javascript">alert("密码错误！");</script>
      <%}else{%>
        <script type="text/javascript">
	//SafeEngineCtl.SEH_InitialSession(31,"com1/usb1", "<%=oldPassword%>", 0, 31, "com1/usb1", "<%=oldPassword%>");
    SafeEngineCtl.SEH_InitialSession(9,"COM1", "<%=oldPassword%>", 0, 9, "COM1", "<%=oldPassword%>");
	if(SafeEngineCtl.ErrorCode!=0)
	{
    		alert("登陆失败，你可能没有将存储CA证书USB设备连接到你的计算机");
	}
	//SafeEngineCtl.SEH_ChangePassword(31,"com1/usb1","<%=oldPassword%>","<%=newPassword%>");
    SafeEngineCtl.SEH_ChangePassword(9,"COM1","<%=oldPassword%>","<%=newPassword%>");
    if(SafeEngineCtl.ErrorCode!=0){
      alert("key的密码修改失败！");
    }
    else{
      alert("密码修改成功！");
      window.location = "/res/oanet.html";
    }
	</script>
      <%}
  }//end if
%>
<script type="text/javascript">
function ChkPwdValid(){
  var oldPwd=document.all.item("old_pwd").value;
  var newPwd=document.all.item("new_pwd").value;
  var confirmPwd=document.all.item("confirm_pwd").value;
  if(oldPwd==null || oldPwd=="" || newPwd==null || newPwd==""){
    alert("密码不能设置为空！");
    return false;
  }
  if(confirmPwd!=newPwd){
    alert("您两次输入的密码不一致，请重新输入！");
    return false;
  }
  if(newPwd.length<6){
	alert("新密码的长度不能小于6位。");
	return false;
  }
  //SafeEngineCtl.SEH_InitialSession(31,"com1/usb1", oldPwd, 0, 31, "com1/usb1", oldPwd);
  SafeEngineCtl.SEH_InitialSession(9,"COM1", oldPwd, 0, 9, "COM1", oldPwd);
  if(SafeEngineCtl.ErrorCode!=0)
  {
  	alert("修改密码前请先将存储CA证书USB设备连接到你的计算机");
    	return false;
  }
  return true;
}
</script>
</head>
<style type="text/css">
Body{
  background-color:white;
  font-size:10.8pt;
  font-family: 宋体;
  margin:0px 0px 0px 0px;  /*top right bottom left*/
  padding:0px 0px 0px 0px;
  scrollbar-face-color: #DEE3E7;                 /*滚动条和上下箭头底座的颜色*/
  scrollbar-highlight-color: #FFFFFF;
  scrollbar-shadow-color: #DEE3E7;               /*不可滚动时箭头的颜色和滚动条右、下边框以及上下箭头底座右、下边框的颜色*/
  scrollbar-3dlight-color: #D1D7DC;              /*滚动条左、上边框和上下箭头底座左、上边框的颜色*/
  scrollbar-arrow-color:  #006699;               /*可滚动时箭头的颜色*/
  scrollbar-track-color: #EFEFEF;                /*滚动条轨道的颜色*/
  scrollbar-darkshadow-color: #98AAB1;           /*滚动条右、下边框和上下箭头底座右、下边框的颜色*/
}
.standardBtn{
  width:60px;
  height:20px;
  padding-top:2px;
  border:1px #939eb2 solid;
  background-image:url('images/standardBtn_bg.gif');
  background-repeat:repeat-x;
  text-align:center;
  cursor:hand;
  font-size:10.8pt;
}
.SetAgent_UtilDIV
{
  width:400px;
  height:190px;
  background-color: #c2ceeb;
  padding:10px;
  border-bottom:1px solid #333366;
  border-top:1px solid #eeeeee;
  border-right:1px solid #333366;
  border-left:1px solid  #eeeeee;
}
.SetAgent_Table
{
  background-color: #e0e7f8;
}
</style>
<body style="overflow:hidden;">
<center>
<table width="100%" style="height:100%;" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="center">
      <div id="Util" class="SetAgent_UtilDIV">
      <fieldset>
      <table width="400" style="height:180;" border="0" cellspacing="0" cellpadding="0" class="SetAgent_Table">
        <form name="form1" action="UpdatePwdAndCa.jsp?action=1" method="post">
        <tr>
          <td style="padding:15px;color:blue;">
            修改密码：
          </td>
        </tr>
        <tr>
          <td style="position:relative;top:-10px;">
            <table width="100%" style="height:70;" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="30%" align="right">
                  旧密码:
                </td>
                <td width="70%" style="position:relative;left:15px;">
                  <input name="old_pwd" id="old_pwd" type="password" style="width:220px;" class="standardInput">
                </td>
              </tr>
              <tr>
                <td width="30%" align="right">
                  新密码:
                </td>
                <td width="70%" style="position:relative;left:15px;">
                  <input name="new_pwd" id="new_pwd" type="password" style="width:220px;" class="standardInput">
                </td>
              </tr>
              <tr>
                <td width="30%" align="right">
                  确认新密码:
                </td>
                <td width="70%" style="position:relative;left:15px;">
                  <input name="confirm_pwd" id="confirm_pwd" type="password" style="width:220px;" class="standardInput">
                </td>
              </tr>
            </table>
          </td>
        </tr>
        <tr>
          <td valign="bottom" align="right" style="padding-right:40px;padding-bottom:30px;">
            <input name="B2" type="submit" value="确认" class="standardBtn" onclick="return ChkPwdValid();">
          </td>
        </tr>
        </form>
      </table>
      </fieldset>
      </div>
    </td>
  </tr>
</table>
</center>
</body>
</html>
