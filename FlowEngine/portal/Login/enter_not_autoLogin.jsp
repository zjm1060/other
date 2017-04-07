<%@ page contentType="text/html;charset=GBK" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<title>自动办公系统.</title>
<style type="text/css">
<!--
.underline {
	background-image: url(images/banner_underline.gif);
	background-repeat: repeat-x;
	height: 13px;
}
td {
	font-size: 12px;
}
.mbg1 {
	background-image: url(images/mbg1.jpg);
	background-repeat: no-repeat;
	height: 367px;
	width: 343px;
	padding-top: 70px;
}
.input {
	border: 1px solid #000000;
	background-color: #FFFFFF;
	font-size: 12px;
	height: 18px;
}
-->
</style>
<style type="text/css">
<!--
.lbg1 {
	background-image: url(images/leftbg1.jpg);
	width: 225px;
	background-position: left top;
	vertical-align: top;
}
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
<SCRIPT LANGUAGE="JAVASCRIPT">

<!--
function keyUp() { 
　　 var keycode = event.keyCode 
　　 var realkey = String.fromCharCode(event.keyCode)
		 if ( keycode == "13")
　　   checkUserPwd(); 
} 

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



<br></br>
<br></br>
<body background="images/bg.gif" leftmargin="0" topmargin="4" marginwidth="0" marginheight="0" scroll="no" >
<tr>
<table width="780"  border="0" align="center" cellpadding="0" cellspacing="0" bgcolor="#FFFFFF">
	<form name="login_server" method="post" action='<%= response.encodeURL("j_security_check") %>' id="login_server" >
  <tr>
    <td align="center" VALIGN="MIDDLE" style="border:1px solid #BAB9B9;">
<table width="780" border="0" align="center" cellpadding="0" cellspacing="0">
        <!--<tr> 
          <td height="70" valign="top">
          	<img SRC="images/topbanner.gif">
          </td>
        </tr>-->
        <tr> 
          <td class="underline">&nbsp;</td>
        </tr>
        <tr> 
          <td><table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>               
                <td width="225" valign="top" class="lbg1">
                <!---
                <object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,29,0" width="225" height="367">
                    <param name="movie" value="left.swf">
                    <param name="quality" value="high">
										<param name=wmode value=transparent>
                    <embed src="left.swf" quality="high" pluginspage="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash" width="225" height="367"></embed>
                </object>
                --->
                </td>                
                <td class="mbg1"><table width="100%" border="0" cellpadding="0" cellspacing="0">
                    <tr> 
                      <td width="130" height="34">&nbsp;</td>
                      <td width="70">用户名：</td>
                      <td><input name="j_username" type="text" class="default_input" onKeyUp="javascript:keyUp();"> </td>
                      </td>
                    </tr>
                    <tr> 
                      <td height="35">&nbsp;</td>
                      <td>密码：</td>
                      <td><input name="j_password" type="password" class="default_input" onKeyUp="javascript:keyUp();"></td>
                    </tr>
                    <tr> 
                      <td height="55">&nbsp;</td>
                      <td>&nbsp;</td>
                      <td><a href='javascript:checkUserPwd();'><img src='images/enter.gif' border='0' hspace="3"></a>
                      <a href='javascript:reset();'><img src='images/reset.gif' border='0' hspace="3"></a>
                      </td>
                    </tr>
                  </table>
                  <table width="57%" border="0" align="right" cellpadding="0" cellspacing="0">
                    <tr> 
                      <td align="left"> <table width="185" border="0" cellspacing="0" cellpadding="0">
                          <tr> 
                            <td> <marquee  border="0" align="bottom" scrollamount="2"  scrolldelay="10" behavior="scroll">
                              <font color="#FF6600">欢迎使用协同工作平台系统！</font> 
                              </marquee></td>
                          </tr>
                        </table></td>
                    </tr>
                  </table></td>
                <td valign="top"><img src="images/rbg1.jpg" width="212" height="367"></td>
              </tr>
            </table></td>
        </tr>
      </table></td>
  </tr>
</table>
</body>
</html>
