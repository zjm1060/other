<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="baseserverjb.BaseServerClient" %>
<%
  //取得路径变量
  String sCommonPath="",sPublicPath="";
  sCommonPath ="/stPortal/Common/";
  sPublicPath ="/stPortal/Public/";
  BaseServerClient bsc=new BaseServerClient(request.getRemoteUser());
  String sFromFilter=request.getParameter("fromFilter");
  boolean isFromFilter=sFromFilter!=null && sFromFilter.trim().equals("1");
%>
<script type="text/javascript" src="<%=sCommonPath%>Include/Common.js"></script>
<%
  String action = request.getParameter("action");
  if(action != null && action.equals("1")){
      String oldPassword = request.getParameter("old_pwd");
      String newPassword = request.getParameter("new_pwd");
      boolean success =false;
      try{
		success=bsc.getFramework().updatePwd(oldPassword,newPassword);
      }
      catch(Exception ex){
		ex.printStackTrace();
		%>
        <script type="text/javascript">
			AlertWindow("<%=sPublicPath%>","对不起，密码修改失败,出错原因:"+ex.getMessage()+"！")
        </script>
		<%
      }
      if(!success){
		%>
        <script type="text/javascript">
		AlertWindow("<%=sPublicPath%>","对不起，密码修改失败,可能是你的旧密码不正确！")
		</script>
		<%
      }else{
		%>
        <script type="text/javascript">
			SucceedWindow("<%=sPublicPath%>","密码修改成功！");
		<%if(isFromFilter){%>
        	window.location ="/stPortal/default.jsp";
		<%}else{%>
			window.location ="/res/oanet.html";
		<%}%>
		</script>
		<%
      }
  }//end if
  else{
  %>
  <html>
  <head>
  <meta http-equiv="Content-Type" content="text/html; charset=GBK">
    <script type="text/javascript">
    function checkPwd(){
      var oldPwd=document.all.item("old_pwd").value;
      var newPwd=document.all.item("new_pwd").value;
      var confirmPwd=document.all.item("confirm_pwd").value;

      return true;
    }
    </script>
    <script type="text/javascript">
    function ChkPwdValid(){
      var oldPwdCtrl=document.all.item("old_pwd");
      var newPwdCtrl=newPwd=document.all.item("new_pwd");
      var confirmPwdCtrl=document.all.item("confirm_pwd");
      var oldPwd=oldPwdCtrl.value;
      var newPwd=newPwdCtrl.value;
      var confirmPwd=confirmPwdCtrl.value;
      turnBorderNone(oldPwdCtrl);
      turnBorderNone(newPwdCtrl);
      turnBorderNone(confirmPwdCtrl);
      if(oldPwd==null || oldPwd==""){
        AlertWindow("<%=sPublicPath%>","对不起，旧密码不能设置为空，请重新输入！");
        turnBorderRedAndFocus(oldPwdCtrl);
        return false;
      }
      if(newPwd==null || newPwd==""){
        AlertWindow("<%=sPublicPath%>","对不起，新密码不能设置为空，请重新输入！");
        turnBorderRedAndFocus(newPwdCtrl);
        confirmPwdCtrl.value="";
        return false;
      }
      if(oldPwd==newPwd){
        AlertWindow("<%=sPublicPath%>","对不起，在更改密码时，系统要求新密码不能于旧密码一致！");
        turnBorderRedAndFocus(newPwdCtrl);
        confirmPwdCtrl.value="";
        return false;
      }
      if(newPwd.length<6){
        AlertWindow("<%=sPublicPath%>","对不起，新密码的长度不能小于6位，请重新输入！");
        turnBorderRedAndFocus(newPwdCtrl);
        confirmPwdCtrl.value="";
        return false;
      }
      if(confirmPwd!=newPwd){
        AlertWindow("<%=sPublicPath%>","对不起，您输入的确认密码与新密码不一致，请重新输入！");
        turnBorderRedAndFocus(newPwdCtrl);
        confirmPwdCtrl.value="";
        return false;
      }
      var re =/\d/;
      var hasDigital=re.test(newPwd);
      re=/[A-Za-z]/;
      var hasLetter=re.test(newPwd);
      if(hasDigital && hasLetter){
        return true;
      }
      else{
        AlertWindow("<%=sPublicPath%>","密码须由数字、字母组成，请重新输入！");
        turnBorderRedAndFocus(newPwdCtrl);
        confirmPwdCtrl.value="";
        return false;
      }
      return true;
    }
    function turnBorderNone(ctr){
      if(ctr!=null){
        try{
          //ctr.style.borderRight="black solid 1px";
          //ctr.style.borderTop="black solid 1px";
          //ctr.style.borderLeft="black solid 1px";
          //ctr.style.borderBottom="black solid 1px"
        }
        catch(e){
        }
      }
    }
    function turnBorderRedAndFocus(ctr){
      if(ctr!=null){
        try{
          //ctr.style.borderRight="red solid 2px";
          //ctr.style.borderTop="red solid 2px";
          //ctr.style.borderLeft="red solid 2px";
          //ctr.style.borderBottom="red solid 2px";
        }
        catch(e){
        }
        ctr.value="";
        ctr.focus();
      }
    }
    function setDefaultFocus(){
      var oldPwdCtrl=document.all.item("old_pwd");
      oldPwdCtrl.focus();
    }
    </script>
    </head>
    <style type="text/css">
    .body{
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
    .standardInput{
    border-right:solid 1px;
    border-top:solid 1px;
    border-left:solid 1px;
    border-bottom:solid 1px;
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
    <body  class="body" onload="javascript:setDefaultFocus();" style="overflow:hidden;">
    <table width="100%"  border="0" cellspacing="0" cellpadding="0" style="height:100%;">
      <tr>
        <td align="center">
          <div id="Util" class="SetAgent_UtilDIV">
            <fieldset>
              <table width="400" style="height:180px;" border="0" cellspacing="0" cellpadding="0" class="SetAgent_Table">
                <form name="form1" action="/stPortal/Public/UpdatePwd.jsp?action=1&fromFilter=<%=isFromFilter?1:0%>" method="post">
                <tr>
                  <td style="padding:15px;color:blue;">
                  修改密码：
                  </td>
                </tr>
                <tr>
                  <td>
                    <table width="100%" style="height:70px;" border="0" cellspacing="0" cellpadding="0">
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
                <%if(isFromFilter){%>
                <tr><td><span style="color:red;font-size:10.8pt;font-family: 宋体;"><b>&nbsp;&nbsp;&nbsp;&nbsp;您的密码已经超过90天未修改，根据《西藏移动统一信息平台系统用户管理规定v1.0》，必须修改密码。密码必须六位或以上，且由字母和数字混合组成！</b></span><td></tr>
                  <%}%>
                </form>
              </table>
            </fieldset>
          </div>
        </td>
      </tr>
    </table>
    </body>
  </html>
  <%}%>
