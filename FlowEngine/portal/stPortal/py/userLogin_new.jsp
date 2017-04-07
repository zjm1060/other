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
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<META http-equiv=Content-Type content="text/html; charset=gb2312">
<LINK href="userLogin/default.css" type=text/css rel=stylesheet>
<META content="MSHTML 6.00.3790.4186" name=GENERATOR></HEAD>
<BODY>
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
<TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
  <TBODY>
  <TR>
    <TD vAlign=top>
<SCRIPT language=javascript>
function doReset()
{
    window.loginFrm.reset();
}
</SCRIPT>

      <TABLE cellSpacing=0 cellPadding=0 width=778 align=center bgColor=#e4f2fe  border=0>
        <TBODY>
        <TR>
          <TD><BR>
            <TABLE height=344 cellSpacing=0 cellPadding=0 width=498 align=center   background=userLogin/enter_bg.jpg border=0>
              <FORM name=loginFrm  action="<%=targetUrl%>/j_security_check"  method=post>
              <INPUT type=hidden name=signinfo> <INPUT type=hidden  name=cert> 
              <INPUT  type=hidden value=JLoginUser name=action> 
              <TBODY>
              <TR>
                <TD vAlign=top>
                  <TABLE height=315 cellSpacing=0 cellPadding=0 width="100%"  border=0>
                    <TBODY>
                    <TR>
                      <TD width="47%" height=76>&nbsp;</TD>
                      <TD width="53%">&nbsp;</TD></TR>
                    <TR>
                      <TD vAlign=top>
                        <TABLE cellPadding=3 width="100%" border=0>
                          <TBODY>
                          <TR>
                            <TD vAlign=top align=middle colSpan=2></TD></TR>
                          <TR>
                            <TD align=right width="35%">用户名：</TD>
                            <TD width="65%">
                            <input type=text name="j_username" maxlength=20 size=10 class="textfield" onkeydown="javascript:keyDown();" onclick="javascript:selectAll(this);" onfocus="javascript:selectAll(this);"></td>
                            </TD></TR>
                          <TR>
                            <TD align=right>密　码：</TD>
                            <TD>
                            <input name="j_password" type="password"  class="textfield" size=10 maxlength=20 onkeydown="javascript:keyDown();" onclick="javascript:selectAll(this);" onfocus="javascript:selectAll(this);">
                            </TD></TR>
                          <TR>
                            <TD colSpan=2>
                              <TABLE height=25 cellSpacing=0 width="89%" 
                              align=center border=0>
                                <TBODY>
                                <TR>
                                <TD align=middle width="51%">&nbsp;</TD>
                                <TD width="48%">
                                <INPUT title=登录 type=image onclick="javascript:login();"
                                height=17 width=60 
                                src="userLogin/enter_enter.gif" 
                                align=middle border=0 name=submit> 
                                </TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>
                                <table width="90%"  border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td height="30" align="center" valign="bottom" class="aa"><a href="/STPublish/view/ViewRecord.jsp?id=2533" target="new"><br>
              请点击此处,<br>设置本机IE环境的变量。</a></td>
          </tr>
        </table>
                            </TD>
                      <TD vAlign=top>
                        <TABLE cellSpacing=0 cellPadding=0 width="88%" 
                          border=0><TBODY>
                          <TR>
                            <TD 
                              vAlign=top>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;申请者在左侧登录窗口输入用户名称和密码，进入番禺区人民政府审批服务中心。<BR><BR>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;如果您是新用户，请先注册获得用户名<BR>和密码后，再登录本系统</TD></TR></TBODY></TABLE></TD></TR>
                    <TR>
                      <TD 
                        vAlign=top>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;如果您是新用户，请先 
                        <STRONG><A 
                        href="/ApproveWeb/RegWeb/register.jsp"><FONT 
                        color=#ff6600>注册</FONT></A> </STRONG>。 </TD>
                      <TD vAlign=top align=right><A class=JB onclick="window.location='/STPublish/style/lct.htm'">返回</A>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</TD></TR></TBODY></TABLE></TD></TR></FORM></TBODY></TABLE>
            <P>&nbsp;</P></TD></TR></TBODY></TABLE></TD></TR></TBODY></TABLE>


      