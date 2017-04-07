<%@ page contentType="text/html;charset=GBK" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>中国水科院财务管理信息系统欢迎您！</title>
<link href="/res/css/public.css" rel="stylesheet" type="text/css">
<style type="text/css">
<!--
-->
</style>
<%
	String username=request.getParameter("username");
	String password=request.getParameter("password");
	//response.setHeader("Cache-Control","no-cache");
	String submitCount=(String)session.getAttribute("submitCount");
	if(submitCount==null)
		submitCount="0";
	int iSubCount=Integer.parseInt(submitCount);
        String rSrvPort=String.valueOf(request.getServerPort());
        String rSrvName=request.getServerName();
	String targetUrl=(String)session.getAttribute("j_target_url");
	if(targetUrl==null)
          targetUrl="";
    String protocol=request.getProtocol();
    if(protocol==null) protocol="";
    int index=protocol.indexOf("/");
    if(index>0){
      protocol=protocol.substring(0,index);
    }
	if(targetUrl.equals("")){
          targetUrl=protocol+"://"+request.getServerName()+":"+request.getServerPort()+request.getContextPath()+"/stPortal/default.jsp";
	}

%>
</head>

<SCRIPT LANGUAGE="JAVASCRIPT">
function login(){
  window.location="/j_security_check?
j_username=wuhy&j_password=iwhr6154&j_forwardurl=HTTP%3A%2F%2F192.168.8.7%3A6001%2FstPortal%2Fdefault.jsp";
}


<!--

function reset(){
    login_server.reset();
}
function window_onload(){
    document.all.item("j_username").focus();
}
function checkUserPwd(){
    var userName=document.all.item("j_username").value;
    var pwd=document.all.item("j_password").value;
    var re=/[']|["]/;
    if(userName==null || userName==""){
      alert("用户名不能为空！");
      document.all.item("j_username").focus();
      return false;
    }
    if(re.test(userName)){
    	alert("用户名不能含有'或者\"号");
        document.all.item("j_username").focus();
    	return false;
    }
    if(re.test(pwd)){
    	alert("密码中不能含有'或者\"号");
        document.all.item("j_password").focus();
    	return false;
    }
    login();
}
//-->
</SCRIPT>
<body leftmargin="0" topmargin="0" marginwidth="0" marginheight="0" scroll="no">
<form name="login_server" method="post" action="<%=request.getContextPath()%>/j_security_check" id="login_server" >
<table width="778" height="100%" border="0" align="center" cellpadding="0" cellspacing="0">
<tr><td align="center">
<table width="445"  border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td width="445" height="292" background="/Login/Images/back5.GIF"><table width="100%" height="254"  border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td width="37%" height="242" rowspan="2">&nbsp;</td>
        <td width="63%" height="96">
        <%if(iSubCount!=0){%>
	     		<br><br><br><br><font color=\"#FF0000\">您的帐号或者密码不匹配，请重新输入</font>
	  	<%}%></td>
      </tr>
      <tr>
        <td height="158"><table width="100%" height="119"  border="0" cellpadding="0" cellspacing="0">
          <tr>
            <td width="37%" height="30"><table width="100%" height="28"  border="0" cellpadding="0" cellspacing="0">
              <tr>
                <td width="46%">&nbsp;</td>
                <td width="54%"><span class="style1">用 户：</span></td>
              </tr>
            </table></td>
            <td colspan="2"><INPUT class=style1  
                  maxLength=16 size=16 name=j_username type="text" value="<%= targetUrl%>"></td>
          </tr>
          <tr>
            <td height="28"><table width="100%" height="28"  border="0" cellpadding="0" cellspacing="0">
              <tr>
                <td width="46%">&nbsp;</td>
                <td width="54%"><span class="style1">密 码：</span></td>
              </tr>
            </table></td>
            <td colspan="2"><input class=style1  type=password 
                  maxlength=16 size=16 name=j_password value="<%= password%>"></td>
          </tr>
          <tr>
            <td height="55"><input id="as" value="asda" type="submit"></td>
            <td width="58%"><div align="center"><input type="image"  src="/Login/Images/chongzhi.gif" style="cursor:hand" onclick="javascript:reset();" width="82" height="20"></div></td>
			<input type="hidden" name="j_forwardurl" size="9" value="">
            <td width="5%">&nbsp;</td>
          </tr>
        </table></td>
      </tr>
    </table></td>
  </tr>
</table>
</td></tr>
</table>
</FORM>
</body>

</html>