<%@page language="java" contentType="text/html;charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<%@page import="com.bitc.jk.xmis.util.WebKeys"%>
<jsp:useBean id="loginForm" scope="session" class="com.bitc.jk.xmis.web.form.LoginForm" />
<!DOCTYPE HTML>
<%
String path = request.getContextPath();
if("/".equals(path)){
	path="";
}
String basePath = request.getScheme() + "://"
+ request.getServerName() + ":" + request.getServerPort()
+ path + "/";

String errMessage = (String) request
.getAttribute(WebKeys.Request_Attribute_Name_LoginFailed);
%>
<html>
  <head>
    <title>
      蚌埠闸工程管理处  综合信息管理系统
    </title>
    <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE9" />
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <meta http-equiv="pragma" content="no-cache">
    <meta http-equiv="cache-control" content="no-cache">
    <meta http-equiv="expires" content="0">
    <link rel="shortcut icon" href="<%=path%>/favicon.ico" type="image/x-icon"/>
    <style type="text/css">
      input.text { width: 125px; height: 20px; border: 1px; border-width: 0px; color: #000; background-color:#fff
      !important; font-family: Arial, '宋体'; font-weight: bold; padding: 3 4 4 3 !important; font-size: 16px; } body {
      text-align: center; margin: 0 0 0 0; background-image: url('resources/images/login-back.jpg'); margin: 0 0 0 0; }
      #login { margin: 200px auto 0 auto; background-image: url('resources/images/login-m.png'); background-repeat:
      no-repeat; } .label { width: 85px; font-size: 20px; font-family: Tahoma; } #errmsg { color: red; font-size: 14px;
      font-family: Tahoma; } #bottom { margin: 0 auto 0 auto; background-image:
      url('resources/images/login-bottom.png'); background-repeat: no-repeat; height: 330px; width: 100%; position:
      absolute; left:0px; bottom: 0px; z-index: -1 } #sbbtn{ width: 95px; height: 65px; cursor: pointer; }
    </style>
    <script type="text/javascript">
      function checkForm(form){
        if(form.loginName.value==""){
          setMsg("请输入用户名！");
          return false;
        }
        return true;
      }
      function setMsg(msg){
        document.getElementById("errmsg").innerHTML=msg;
      }
    </script>
  </head>
  <body onload="javascript:document.getElementById('loginName').focus();">

    <div id="login" style="width: 623px; height: 140px;text-align:left;">
      <form action="<%=path%>/loginAction.do" method="POST" onsubmit="return checkForm(this)" id = "userLogin">
        <input type=hidden name="uri" value="<%=request.getParameter(WebKeys.RequestKey_Original_URI)%>" />
        <input 
            type=hidden
            name="queryStr"
            value="<%=request.getParameter(WebKeys.RequestKey_Original_QueryString)%>"
        />
        <div style="position: relative; top: 50px; left: 115px; width: 100px;">
        <input id="loginName" type="text" name="loginName" value="" class="text"/>
        </div>
        <div style="position: relative; top: 66px; left: 115px; width: 100px;">
        <input id= "password" type="password" name="password" value="" class="text" />
        </div>
        <div 
            id="btndiv"
            style="position: relative; top: 0px; left: 270px; width: 95px; height: 65px; filter: alpha(Opacity = 0); -moz-opacity: 0; opacity: 0; background-color: #ffffff;">
          <input id="sbbtn" type="submit" value=" " />
        </div>
        <div style="position: relative; top: 0px; left: 115px; width: 100px;">
	        <span id="errmsg">
	          <%=errMessage == null ? "" : errMessage%>
	        </span>
        </div>
         <div style="padding-left:100px;font-size:12px;" title="今天之内不用再登录">
        	<input type="checkbox" name="remember" />记住我的登录状态
        </div>
      </form>
      <div style="padding-left:10px;font-size:18px;color:yellow;">
        	提示：<br>
        	建议使用IE浏览器，其他浏览器可能无法正常使用本系统的某些功能！
      </div>
    </div>
    <div id="bottom">
    </div>
    <script type="text/javascript">
    	ua = navigator.userAgent.toLowerCase(),
    		check = function(r){
        	return r.test(ua);
    	}
    	isOpera = check(/opera/);
    	isIE = !isOpera && check(/msie/);
    	if(!isIE){
    		alert("您使用的不是IE浏览器,强烈建议使用IE浏览器！");
    	}
    </script>
  </body>
</html>
