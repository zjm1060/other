<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="flowengineweb.*" %>
<jsp:useBean id="myBaseServer" scope="session" class="flowengineweb.BaseServerBean" />
<%
  //取得路径变量
  String sCommonPath="",sPublicPath="";
  sCommonPath = SysPara.getCommonPath(request);
  sPublicPath = SysPara.getPublicPath(request);
%>
<script language="javascript" src="<%=sCommonPath%>Include/Common.js"></script>
<%
  //强制页面刷新
  response.setHeader("Pragma","No-cache");
  response.setHeader("Cache-Control","no-cache");
  response.setDateHeader("Expires", 0);
  myBaseServer.initialize(pageContext);
%>
<%
  String action = request.getParameter("action");
  if(action != null && action.equals("1")){
      String oldPassword = request.getParameter("old_pwd");
      String newPassword = request.getParameter("new_pwd");
      boolean success = myBaseServer.updatePwd(oldPassword,newPassword);
      if(!success)
        out.print("<script language=\"javascript\">AlertWindow(\"" + sPublicPath + "\",\"密码错误！\")</script>");
      else
        out.print("<script language=\"javascript\">SucceedWindow(\"" + sPublicPath + "\",\"密码修改成功！\");window.location = \"UpdatePwd.jsp\";</script>");
  }//end if
%>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=GBK">
<title>修改密码</title>
<link rel="stylesheet" type="text/css" href="<%=sCommonPath%>Css/Public.css">
<script language="javascript">
function ChkPwdValid()
{
  if (form1.confirm_pwd.value !=form1.new_pwd.value)
  {
    AlertWindow("<%=sPublicPath%>","您两次输入的密码不一致，请重新输入！");
    return false;
  }
  if (form1.old_pwd.value==""||form1.new_pwd.value==""||form1.old_pwd.value.indexOf(" ")!=-1||form1.new_pwd.value.indexOf(" ")!=-1)
  {
    AlertWindow("<%=sPublicPath%>","输入的密码不能为空，且不能包含空格，请重新输入！");
    return false;
  }
  
  
  return true;
}
</script>
</head>
<body scroll="no">
<table width="100%" height="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td align="center">
      <div id="Util" class="SetAgent_UtilDIV">
      <fieldset>
      <table width="400" height="180" border="0" cellspacing="0" cellpadding="0" class="SetAgent_Table">
        <form name="form1" action="UpdatePwd.jsp?action=1" method="post">
        <tr>
          <td style="padding:15px;color:blue;">
            修改密码：
          </td>
        </tr>
        <tr>
          <td style="position:relative;top:-10px;">
            <table width="100%" height="70" border="0" cellspacing="0" cellpadding="0">
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
</body>
</html>
