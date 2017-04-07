<%@ page contentType="text/html;charset=GBK" %>
<%@ page import="shecasafeapi.SafeEngine" %>
<%@ page import="baseserverjb.BaseServerClient" %>
<%@ page import="java.sql.*" %>
<%
//=====================================================
  String actionFlag=request.getParameter("actionFlag");
  if(actionFlag==null) actionFlag="";
  if(!actionFlag.equals("login")){
%>
<html>
<head>
<title>广州市番禺区审批行政服务中心</title>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<!--//证书控件-->
<object classid="clsid:6DD0675D-C9E8-4FB2-BB64-E443B269D4CE" id="CAOCX" codebase="SZD31AX.ocx"></object>
<style type="text/css">
<!--
.font {  font-size: 18px; color: #000000; text-decoration: none}
.font1 {  font-size: 18px; color: red; text-decoration: none}
.font2 {  font-size: 12px; color: black; text-decoration: none}
.GoBt {
	BORDER-RIGHT: #000000 1px solid; BORDER-TOP: #000000 1px solid; FONT: 9pt Verdana,Tahoma,宋体; BORDER-LEFT: #000000 1px solid; WIDTH: 40px; CURSOR: hand; COLOR: #000000; MARGIN-RIGHT: 10px; BORDER-BOTTOM: #000000 1px solid
}
.input {  border: #868685; border-style: solid; border-top-width: 1px; border-right-width: 1px; border-bottom-width: 1px; border-left-width: 1px}
-->
</style>
<script language="VBScript">
function GetCaNo() 
	dim vart
	dim data
	ret = CAOCX.Get_serialno(vart)
	GetCaNo= vart
end function
</script>
<SCRIPT LANGUAGE=javascript>
<!--
function login(){
	login_server.submit();
}
function loginSubmit(){
	if(!checkCaKey()) return;
	else{
		 var caKey=GetCaNo();
		 if(caKey!=""){
		 	document.all.item("CAKeyUser").value=caKey;
		 	document.all.item("j_password").value=document.all.item("CAKeyPwd").value;
		 	document.all.item("actionFlag").value="login";
		  login();
		}else{
		 alert("读取key信息为空，请插入key硬件。");
	 }
  }
}
function reset(){
    login_server.reset();
}

function keyDown() { 
	//var keycode = String.fromCharCode(event.keyCode);
 	if ( event.keyCode == "13" ){
 	 var userName=document.all.item("j_username");
   var passWord=document.all.item("CAKeyPwd");
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
   GetCaNo();
 }  
}
function keyDownCa() { 
	if ( event.keyCode == "13" ){
   if(!checkCaKey()){
   	 return;
   }else{
    loginSubmit();
  }
 }  
}
function checkCaKey(){
	 var passWord=document.all.item("CAKeyPwd");
   if(passWord.value ==""){
   		alert("密码不能空!");     	
    	passWord.focus();
    	return false;
   }else return true;
  
}
function selectAll(id){
	id.select();
}
//创建xmlHttp 
  function CreateXMLHttp()
  {
   var xmlHttp = false;
   if(typeof XMLHttpRequest != 'undefined'){
    xmlHttp = new XMLHttpRequest();
    return xmlHttp;
   }else if(window.ActiveXObject){
    var IEXMLHttpVersion =["MSXML2.XMLHttp.3.0","MSXML2.XMLHttp.4.0","MSXML2.XMLHttp.5.0","MSXML2.XMLHttp.6.0","MSXML2.XMLHttp","Microsoft.XMLHttp"];
    //循环微软IE版本
    for(var i= 0 ;i < IEXMLHttpVersion.length; i++){
     try{
        var xmlHttp = new ActiveXObject(IEXMLHttpVersion[i]);
        return xmlHttp;
     }catch(e){
       //alert(i);alert(xmlHttp);
       xmlHttp= false;
     }
    }      
    }else{
     throw new Error("XMLHttp object could be created.");
     xmlHttp= false;
    }
    return xmlHttp;
  }
function loginWithBasic(url,userCode,passWord,sysID){
	  var sysTishi="";
		var xmlhttp = CreateXMLHttp();// new ActiveXObject("Msxml2.XMLHTTP.3.0");
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
		xmlhttp.open("GET",url, false,userCode,passWord);
		xmlhttp.send();
		var book = xmlhttp.status;
		if(book!="200"){
			alert("单次登录失败，返回错误代码："+book+",可能两个系统的密码不一致。");
		}

}
//-->
</SCRIPT>
</head>

<body bgcolor="#FFFFFF" text="#000000" leftmargin="0" topmargin="0">
<form name="login_server" method="post" action="/Login/enterCA.jsp" id="login_server" >
<table width="100%" border="0" cellspacing="0" cellpadding="0" height="100%">
  <tr>
    <td height="59" background="./img/top_bg.gif" colspan="2">&nbsp;</td>
  </tr>
  <tr>
    <td bgcolor="#F2F2F1" height="90"><img src="./img/logo-py.gif" width="642" height="74"></td>
    <td bgcolor="#F2F2F1" height="90">&nbsp;</td>
  </tr>
  <tr bgcolor="#F2F2F1">
    <td bgcolor="#E5E4E4" background="./img/logocenter.jpg" height="107" colspan="2">&nbsp;</td>
  </tr>
   <tr bgcolor="#E5E4E4">
    <td colspan="2" height="30">
      <div align="right" class="font">&copy;广州市番禺区人民政府审批服务中心 All Right Reserved </div>
    </td>
  </tr>
  <tr bgcolor="#E5E4E4" valign="top">
    <td colspan="2">
      <div align="center"> 
          <p><span class="font"><strong><font color="#333333">用户名:</font></strong></span> 
            <input type="text" name="j_username" value="" class="input" onkeydown="javascript:keyDown();" onclick="javascript:selectAll(this);" onfocus="javascript:selectAll(this);">
            <input type="hidden" name="j_password" value=""  class="input" >
            <input type="hidden" name="CAKeyUser" value=""  class="input" >
            <span class="font"><strong><font color="#333333">密码:</font></strong></span> 
            <input type="password" value="" name="CAKeyPwd" class="input" onkeydown="javascript:keyDownCa();" onclick="javascript:selectAll(this);" onfocus="javascript:selectAll(this);">
            <input type="button" name="Submit"  value="登 录" class="GoBt" onclick="javascript:loginSubmit();">
            <input type="button"  name="Submit2" value="重 置" class="GoBt">
            <input type="hidden" name="actionFlag" value="">
      
          </p> 
        </div>
        <div align="right" class="font1">&copy;行政审批新系统 广州明动软件有限公司  </div>
        <div align="right" class="font2">技术支持电话: 13318863148 (江生) 、13751838997 (徐生)、13726011534(陈生)</div>
    </td>
  </tr>

  <tr>
    <td height="177" background="./img/bottom_bg.gif" colspan="2"><img src="./img/bottom_bg.gif" width="3" height="177">
    </td>
  </tr>
</table>

</form>
</body>
</html>
<SCRIPT language=javascript>
	document.all.item("j_username").focus();
</SCRIPT>
<%}else{
 boolean caErr=false;
 String CAKeyUser=request.getParameter("CAKeyUser");
 String j_username=request.getParameter("j_username");
 String j_password=request.getParameter("j_password");
 if(j_username==null) j_username="";
 if(j_password==null) j_password="";
 System.out.println("ca认证后的 CAKeyUser"+CAKeyUser);
 System.out.println("ca认证后的 j_password"+j_password);
 //if(j_username.equals("")) j_username="test01";
 //if(j_password.equals("")) j_password="test01";
 
 
  Connection conn=null;
  PreparedStatement pstm=null;
  String tempUser="";
  try{
    if(CAKeyUser!=null){
      tempUser=baseserverjb.BaseServerClient.getUserbyCASN(CAKeyUser);
      if(tempUser==null) tempUser="";
      if(!tempUser.equals(j_username)){
        out.println("<script language='javascript'>alert('输入用户有误，请重新输入！');</script>");
        caErr=true;
      }
    }else{
       out.println("<script language='javascript'>alert('没有插入key硬件，或者信息有误，请与管理员联系！');</script>");
       caErr=true;
    }
  }
  catch(Exception ex){
    ex.printStackTrace();
  }
  finally{
    if(pstm!=null){
      try{pstm.close();}catch(Exception ex000001){}
    }
    pstm=null;
    if(conn!=null){
      try{conn.close();}catch(Exception exxxxxx){}
    }
    conn=null;
  }
  if(!caErr){
%>
<OBJECT CLASSID="CLSID:0DC9CA0C-ED42-4C87-A335-1B0BD043FC70" id="PSIAtv"  VIEWASTEXT ></OBJECT>
<form name="login_server2" onSubmit= method="post" action="j_security_check" id="login_server" >
	<input name="j_username" type="hidden" value="<%=j_username%>" class="defaultinput" size="124" tabindex="1">
	<input name="j_password" type="hidden" value="<%=j_password%>" class="defaultinput" size="124" tabindex="2">
</form>
 
<SCRIPT LANGUAGE="JAVASCRIPT">
  login_server2.submit();
</script>
<%}else{%>
  <SCRIPT LANGUAGE="JAVASCRIPT">
  window.location.href="./enterCA.jsp";
</script>
<%}
}%>