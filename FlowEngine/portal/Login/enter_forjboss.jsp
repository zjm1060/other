<%@ page contentType="text/html;charset=GBK" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>湖北移动统一信息平台欢迎您！</title>
<link href="/res/css/public.css" rel="stylesheet" type="text/css">
<style type="text/css">
<!--
-->
</style>
<%
	//response.setHeader("Cache-Control","no-cache");
	String submitCount=(String)session.getAttribute("submitCount");
	/*
    java.util.Enumeration headers=request.getHeaderNames();
    while(headers.hasMoreElements()){
		String headerName=(String)headers.nextElement();
        System.err.println(headerName+"="+request.getHeader(headerName));
    }
	*/
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
<SCRIPT type="text/javascript">
<!--
function login(){
  login_server.submit();
}
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
<table width="778" height="100%" border="0" align="center" cellpadding="0" cellspacing="0">
<form name="login_server" method="post" action="/Login/login.jsp" id="login_server" >
  <tr>
    <td valign="middle"> <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
        <tr>
          <td width="340" align="center" valign="top" bgcolor="#F4F0E9"> <table width="100%" height="411" border="0" cellpadding="0" cellspacing="0">
              <tr bgcolor="#FFFFFF">
                <td width="46" height="58">&nbsp;</td>
                <td>&nbsp;</td>
              </tr>
              <tr>
                <td valign="top" class="blocktopbg"><img src="/res/images/block_topleft.gif"></td>
                <td width="292" class="blocktopbg"></td>
              </tr>
              <%
          	if(iSubCount!=0){
          		out.println("<tr align=\"center\">");
	     		out.println("<td colspan=\"2\" width=\"100%\" height=\"14\" class=\"blocktopbg\" ><font color=\"#FF0000\">您的帐号或者密码不匹配，请重新输入！</font></td>");
	  		out.println("</tr>");
	  	}
	      %>
              <tr>
                <td valign="top" class="blockrightbg">&nbsp;</td>
                <td align="center" valign="top"> <br> <img src="/res/images/title.gif"><br>
                  <br> <br> <br> <br> <br> <table width="100%" border="0" cellspacing="0" cellpadding="0">
                    <tr>
                      <td width="68" height="34">用户名</td>
                      <td width="232"><input name="j_username" type="text" class="default_input"></td>
                    </tr>
                    <tr>
                      <td height="35">密码</td>
                      <td><input name="j_password" type="password" class="default_input"></td>
                    </tr>
                    <tr>
                      <td height="100">&nbsp;</td>
                      <td><input type="image" src="/res/images/enter.gif" onclick="javascript:checkUserPwd();" border="0" >
                        <input type="image" src="/res/images/reset.gif" onclick="javascript:reset();" border="0" ></td>
                        <input type="hidden" name="j_forwardurl" size="9" value="<%=targetUrl%>">
                    </tr>
                  </table></td>
              </tr>
              <tr>
                <td height="10" valign="bottom" class="blockrightbg"><img src="/res/images/block_underleft.gif"></td>
                <td valign="top">&nbsp;</td>
              </tr>
            </table></td>
          <td width="438" valign="top"> <table height="411" border="0" cellpadding="0" cellspacing="0">
              <tr>
                <td><img src="/res/images/circle_onleft.jpg"><img src="/res/images/circle_onright.jpg"></td>
                <td>&nbsp;</td>
              </tr>
              <tr>
                <td><img src="/res/images/circle_underleft.jpg"><img src="/res/images/circle_underright.jpg"></td>
                <td>&nbsp;</td>
              </tr>
            </table></td>
        </tr>
      </table>
      <table width="654" border="0" cellspacing="0" cellpadding="0">
        <tr>
          <td width="18" class="welcomleft">&nbsp;</td>
          <td width="438" class="welcomebg"><p>
              <marquee  border="0" align="bottom" scrollamount="2"  scrolldelay="10" behavior="scroll"  >
              欢迎光临湖北移动统一信息平台！
              </marquee>
            </p></td>
          <td width="198" class="welcomeright">&nbsp;</td>
        </tr>
      </table> </td>
  </tr>
  </FORM>
</table>
</body>
</html>
