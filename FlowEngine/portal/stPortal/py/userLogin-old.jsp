<%@page contentType="text/html; charset=gb2312"%>
<%@ page language="java" import="java.util.*,java.sql.*,sun.jdbc.rowset.*,suntek.portal.publicLib.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
//清除ie缓存
	response.setHeader("Cache-Control", "no-store");	//HTTP 1.1
	response.setHeader("Pragma", "no-cache");		//HTTP 1.0
	response.setDateHeader("Expires", 0);			//prevents  caching   at   the   proxy
	String rSrvPort=String.valueOf(request.getServerPort());
	String rSrvName=request.getServerName();
	String targetUrl=(String)session.getAttribute("j_target_url");
	if(targetUrl==null)targetUrl="";
	String rootPath=request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+request.getContextPath();
	if(targetUrl==null)
	targetUrl="";
	if(targetUrl.equals("")){
	  targetUrl=rootPath+"/stPortal/default.jsp";
	}
%>
<%
String m_usercode="";
if(session.getAttribute(suntek.security.Security.m_sUser)!=null){
	m_usercode=(String)session.getAttribute(suntek.security.Security.m_sUser);
}
if(m_usercode==null) m_usercode="";
if(m_usercode.equals("")) m_usercode=request.getRemoteUser();
String m_username =oManager.getCurrentUserName();
%>

<html>
<head>
<title>公众注册</title>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<link href="/STPublish/common/css/style.css" rel="stylesheet" type="text/css"></style>
<script language="javascript">
function keyDown() { 
	//var keycode = String.fromCharCode(event.keyCode);
 	if ( event.keyCode == "13" ){
 	 var userName=document.all.item("j_username");
   var passWord=document.all.item("j_password");
   if(userName.value =="" ) { 	
     	alert("用户不能空!");
     	userName.focus();
     	return;
   }
   if(userName.value !="" && passWord.value ==""){
   		//alert("密码不能空!");     	
    	passWord.focus();
    	return;
    }
   login();
 }  
}
function login()
{
		var userName=document.all.item("j_username");
  	var passWord=document.all.item("j_password");
		if(userName.value !="" && passWord.value !=""){
  	  var ret="";
	       login_server.submit(); //网站登录			
 	  }  else{
 	  	alert("用户名或密码为空!");
 	  } 		
}
function selectAll(id){
	id.select();
}
function getKey(obj){
	if(event.keyCode==13){
		login();
	}
}


function reset(){
	document.all.item("j_password").value="";
	document.all.item("j_username").value="";
}
function regist(){
 window.open("/ApproveWeb/RegWeb/register.jsp");
}
//创建xmlHttp 
  function CreateXMLHttp()
  {
   var xmlHttp = false;
   if(typeof XMLHttpRequest != 'undefined')
   {
    xmlHttp = new XMLHttpRequest();
    return xmlHttp;
   }else if(window.ActiveXObject)
   {
    var IEXMLHttpVersion =["MSXML2.XMLHttp.4.0","MSXML2.XMLHttp.3.0","MSXML2.XMLHttp.5.0","MSXML2.XMLHttp.6.0","MSXML2.XMLHttp","Microsoft.XMLHttp"];
    //循环微软IE版本
    for(var i= 0 ;i < IEXMLHttpVersion.length; i++)
    {
     try
     {
        var xmlHttp = new ActiveXObject(IEXMLHttpVersion[i]);
        return xmlHttp;
     }catch(e)
     {
       //alert(i);alert(xmlHttp);
       xmlHttp= false;
     }
    }      
    }else
    {
     throw new Error("XMLHttp object could be created.");
     xmlHttp= false;
    }
    return xmlHttp;
  }
function loginWithBasic(url,userCode,passWord,sysID){

	  var sysTishi="";
		//alert(url+userCode+passWord);
		var xmlhttp = CreateXMLHttp();// new ActiveXObject("Msxml2.XMLHTTP.3.0");
    //alert(typeof xmlhttp);alert(xmlhttp);
		if( typeof xmlhttp !="object"){
			document.all.item("xmlHttpType").value=typeof xmlhttp;
			if(xmlhttp==false){
			 var url="setupXML.htm";
			 var sFeatures= "dialogHeight:300px;dialogWidth:400px;";
  		 sFeatures = sFeatures + "center:yes;edge:raised;scroll:auto;status:no;unadorned:yes;"
  		 window.showModalDialog(url,window,sFeatures);
			 return false;
			}
		}
		//alert("new");
		xmlhttp.open("GET",url, false,userCode,passWord);
		//alert("open");
		xmlhttp.send();
		//alert("send");
		var book = xmlhttp.status;
		if(book!="200"){
			if(sysID=="1")
				sysTishi="审批管理系统";
			else
				sysTishi="门户网站系统";
			//alert("单次登录失败，返回错误代码："+book+"：门户系统与"+sysTishi+"的用户密码不一致。");
		}
}
</SCRIPT>

<style type="text/css">
<!--
.aa {
	font-size: 12px;
}
-->
</style>
</head>
<BODY BGCOLOR=#FFFFFF LEFTMARGIN=0 TOPMARGIN=0 MARGINWIDTH=0 MARGINHEIGHT=0 rightmargin="0" bottommargin="0">
<%if(m_usercode.equals("guest")){%>
<TABLE WIDTH="184" HEIGHT="190" BORDER="0" CELLPADDING="0" CELLSPACING="0" class="aa">
  <TR>
    <TD> <IMG SRC="/STPublish/style/images/login_1.jpg" WIDTH="100%" HEIGHT=27 ALT=""></TD>
  </TR>
  <TR>
    <TD width="100%" height="100%" background="/STPublish/style/images/login_2.jpg">
    <FORM id="login_server" name="login_server" action="<%=targetUrl%>/j_security_check" method="post"  target=_top>
        <br>
        <table width="100%" border="0" cellpadding="0" cellspacing="0">
          <tr align="left" class="subTab">
            <td height="10" valign="bottom" class="aa"><font color="006B12"><strong>用　户：</strong></font>
              <input type=text name="j_username" maxlength=20 size=10 class="textfield" onkeydown="javascript:keyDown();" onclick="javascript:selectAll(this);" onfocus="javascript:selectAll(this);"></td>
          </tr>
          <tr align="left" class="subTab">
            <td height="10" valign="bottom" class="aa"><font color="006B12"><strong>密　码：</strong></font>
              <input name="j_password" type="password"  class="textfield" size=10 maxlength=20 onkeydown="javascript:keyDown();" onclick="javascript:selectAll(this);" onfocus="javascript:selectAll(this);">
              <input type="hidden" value="" name="CA">
            </td>
          </tr>
          <tr align="left">
            <td height="30" valign="bottom">
              <table width="90%" border="0" cellspacing="0" cellpadding="0">
                <tr>
                  <td width="10%" align="right"><a href="javascript:regist();"><img src="register.gif" width="44" height="21" border="0"></a></td>
                  <td width="10%" align="center"><a href="javascript:login();"><img src="login.jpg" width="44" height="21" border="0"></a></td>
                  <td width="10%" align="left"><a href="javascript:reset()"><img src="cancer.jpg" width="44" height="21" border="0"></a></td>
                </tr>
              </table></td>
          </tr>
        </table>
        <table width="90%"  border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td height="30" align="center" valign="bottom" class="aa"><a href="/STPublish/view/ViewRecord.jsp?id=2533" target="new"><br>
              请点击此处,<br>设置本机IE环境的变量。</a></td>
          </tr>
        </table>
      </form> </TD>
  </TR>
  <tr>
		<table width="100%" border="0" cellspacing="0" cellpadding="0">
			<tr>
			<td><img src="images/space.jpg" width="1" height="7"></td>
			</tr>
		</table>
  </TR>
</TABLE>
<SCRIPT language=javascript>
	document.all.item("j_username").focus();
</SCRIPT>
<%}else{%>
<TABLE WIDTH="184" HEIGHT="190" BORDER=0 CELLPADDING=0 CELLSPACING=0>
  <TR>
    <TD> <IMG SRC="/STPublish/style/images/login_1.jpg" WIDTH="100%" HEIGHT="27" ALT=""></TD>
  </TR>
  <TR>
    <TD width="100%" height="100%" background="/STPublish/style/images/login_2.jpg">
      <table width="100%" height="100%" border="0" cellpadding="0" cellspacing="0">
          <tr>
          <td align="center"> <br>
            欢迎<font color=red> <%=m_username%></font>
<p>登录</p><p>番禺区人民政府审批中心系统</p>
          </td></tr>
        </table>
        <table width="100%"  border="0" cellspacing="0" cellpadding="0">
          <tr align="right">
            <td height="30" align="right" valign="bottom" class="aa"><a href="/STPublish/view/ViewRecord.jsp?id=2533" target="new"><br>
              请点击此处<br>
              设置本客户端IE环境的变量。</a></td>
          </tr>
        </table>
    </TD>
  </TR>
  <tr>
		<table width="100%" border="0" cellspacing="0" cellpadding="0">
			<tr>
			<td><img src="images/space.jpg" width="1" height="7"></td>
			</tr>
		</table>
  </TR>
</TABLE>
<%}%>

</BODY>
<input type="hidden" name="j_forwardurl" size="9" value="<%=targetUrl%>">
</HTML>

