<%@page contentType="text/html; charset=gb2312"%>
<%@ page language="java" import="java.util.*,java.sql.*,sun.jdbc.rowset.*,suntek.portal.publicLib.RecordSetObject" %>
<%@ page import="baseserverjb.BaseServerClient,baseserverjb.DatabaseInfo,
java.util.Hashtable,java.sql.*,java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.minstone.util.PublicLib" %>
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
 
	String m_usercode="";
	if(session.getAttribute(suntek.security.Security.m_sUser)!=null){
		m_usercode=(String)session.getAttribute(suntek.security.Security.m_sUser);
	}
	if(m_usercode==null) m_usercode="";
	if(m_usercode.equals("")) m_usercode=request.getRemoteUser();
	String m_username =oManager.getCurrentUserName();
%>
<BODY>
<META http-equiv=Content-Type content="text/html; charset=gb2312">
<META content="MSHTML 6.00.3790.4186" name=GENERATOR></HEAD>
<LINK href="userLogin/default.css" type=text/css rel=stylesheet>
<link href="/STPublish/common/css/style.css" rel="stylesheet" type="text/css"></style>
<script language="javascript">
function keyDown() { 
	//var keycode = String.fromCharCode(event.keyCode);
 	if ( event.keyCode == "13" ){
 	 var userName=document.all.item("j_username");
   var passWord=document.all.item("j_password");
   
   if(userName.value =="" ) { 	
     	//alert("会员不能空!");
     	userName.focus();
     	return;
   }
   if(userName.value !="" && passWord.value ==""){
   		document.all.item("j_password").focus();
    	return;
    }
   if(userName.value !="" && passWord.value !=""){ 
    login();
   }
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
 	  	alert("会员号或密码为空!");
 	  } 		
}
function selectAll(id){
	id.select();
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
			//alert("单次登录失败，返回错误代码："+book+"：门户系统与"+sysTishi+"的会员号或密码不一致。");
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

        
<TABLE cellSpacing=0 cellPadding=0 width=778 align=center bgColor=#e4f2fe  border=0 class="TD">
  <TBODY>
  <TR>
    <TD><BR>
      <FORM id="login_server" name="login_server" action="<%=targetUrl%>/j_security_check" method="post"  target=_top>
      <TABLE height=344 cellSpacing=0 cellPadding=0 width=498 align=center   background="userLogin/enter_bg.jpg" border=0>
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
                <%if(m_usercode.equals("guest")){%>
                  <TABLE cellPadding=3 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD vAlign=top align=middle colSpan=2></TD></TR>
                    <TR>
                      <TD align=right width="35%" class="fontsize">会员号：</TD>
                      <TD width="65%">
                       <input type=text name="j_username" maxlength="20" size="14" class="textfield" onkeydown="javascript:keyDown();" onclick="javascript:selectAll(this);" onfocus="javascript:selectAll(this);"></td>
                      </TD></TR>
                    <TR>
                      <TD align=right class="fontsize">密　码：</TD>
                      <TD>
                        <input name="j_password" type="password"  class="textfield" size="14" maxlength=20 onkeydown="javascript:keyDown();" onclick="javascript:selectAll(this);" onfocus="javascript:selectAll(this);">
                      </TD></TR>
											<SCRIPT language="javascript">
											 document.all.item("j_username").focus();
											</SCRIPT>
       						  <TR>
                      <TD colSpan="2" align="center">
                        <table width="100%" border="0" cellspacing="0" cellpadding="0">
					                <tr>
					                  <!--<td width="10%" align="right"><a href="javascript:regist();"><img src="register.gif" width="44" height="21" border="0"></a></td>-->
					                  <td width="30%" align="center"><a href="javascript:login();"><img src="login.jpg" width="44" height="21" border="0"></a></td>
					                  <td width="30%" align="left"><a href="javascript:reset()"><img src="cancer.jpg" width="44" height="21" border="0"></a></td>
					                </tr>
					              </table>
                       </TD>
                     </TR>
                     </TBODY>
                   </TABLE>
                   <%}else{
                    String currentUser=request.getRemoteUser();
                    String mobile="";
                    String tel="";
                   	Connection Conn = null;
										try{
											Conn = BaseServerClient.getConnection(currentUser);
											String sql = " select mobi_tel,user_tel from base_user t "
											      +" where user_code='"+currentUser+"' ";
											Statement stm = Conn.createStatement();
											ResultSet rs = stm.executeQuery(sql);
											if(rs.next()){
											  mobile=rs.getString("mobi_tel");
											  if(mobile==null) mobile="";
											  tel=rs.getString("user_tel");
											  if(tel.equals("")) tel="";
											}
											rs.close();
											rs = null;
											stm.close();
											stm = null;
										}catch(Exception ex){
										  ex.printStackTrace();
										} finally{
										  PublicLib.closeConnection(Conn);
										  Conn=null;
										}
                   %>
									<TABLE cellPadding=3 width="100%" border=0>
                    <TBODY>
                    <TR>
                      <TD vAlign=top align="middle" colSpan="2"></TD></TR>
                    <TR>
                      <TD align=right width="35%"></TD>
                      <TD width="65%">
                      </TD></TR>
                    <TR>
                      <TD width="100%" align="center" colspan="2" class="fontsize">
                       <br>欢迎<font color=red> <%=m_username%><p>已经登录本系统。</p></font>
                       <br><font color=red><p style="cursor:hand" onclick="window.location='index.jsp'">返回首页</p></font>
                      </TD></TR>
       						  <TR>
                      <TD colSpan=2>
                      </TD>
                     </TR>
                     </TBODY>
                   </TABLE>
                   <%if(mobile.equals("") || tel.equals("")){%>
                   <script language="javascript">
                     alert("你的手机号码、电话号码等信息还没填上，为了方便短信提醒，请在\"客户服务-个人信息-联系方式\"这里填写。");
                   </script>
										<%}}%>
   <table width="90%"  border="0" cellspacing="0" cellpadding="0">
    <tr>
      <td height="30" align="center" valign="bottom" class=""><a href="/STPublish/view/ViewRecord.jsp?id=2533" target="new"><br>
        请点击此处,<br>设置本机IE环境的变量。</a></td>
    </tr>
  </table>
    </TD>
		<TD vAlign=top>
			<TABLE cellSpacing=0 cellPadding=0 width="88%"  border=0><TBODY>
				<TR>
				  <TD  vAlign=top  class="fontsize">
				    &nbsp;&nbsp;&nbsp;&nbsp;申请者在左侧登录窗口输入会员号和密码，进入番禺区人民政府审批服务中心。
				    <BR><BR>&nbsp;&nbsp;&nbsp;&nbsp;<span style="color:red">如果您想登记为新会员，请先注册，然后到审批中心102室开通并领取上网登记卡，再登录本系统。
				    </span>
				    <br>番禺审批大厅地址:番禺区市桥清河东路319号区行政办公中心 西副楼  电话：020-84690909
				  </TD>
				</TR>
			</TABLE>
		</TD>
		</TR>
		<TR>
		 <TD vAlign=top class="fontsize">&nbsp;&nbsp;&nbsp;&nbsp;如果您是新会员，请先  <STRONG>
		  <A  href="/ApproveWeb/RegWeb/register.jsp?noPwd=1">
		  <FONT color=#ff6600>注册</FONT></A> </STRONG>
		 </TD>
		 <TD vAlign=top align=right class="fontsize">
		   <A style="cursor:hand" onclick="window.location='loginFlow.jsp'">办事流程图</A>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
		 </TD>
		</TR>
		</TBODY></TABLE></TD></TR></TBODY></TABLE>
       <P>&nbsp;</P></TD></TR></TBODY></TABLE>
      </FORM>
      </TD></TR></TBODY></TABLE>

</BODY>