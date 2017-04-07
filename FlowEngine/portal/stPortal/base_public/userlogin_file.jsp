<%@ page contentType="text/html;charset=GBK" %>
<%@ page import="com.minstone.fileLogin"%>
  <script type="text/VBScript" src="getUserFile_VB.js"></script>
  <%
  //清除ie缓存
  response.setHeader("Cache-Control", "no-store");	//HTTP 1.1
  response.setHeader("Pragma", "no-cache");		//HTTP 1.0
  response.setDateHeader("Expires", 0);			//prevents  caching   at   the   proxy
  %>
  <%
  request.setCharacterEncoding("GBK");
  String actionFlag = request.getParameter("actionFlag");
  if(actionFlag==null) actionFlag="";
  String newLoginFlag = request.getParameter("newLoginFlag");
  if(newLoginFlag==null) newLoginFlag="";

  if(actionFlag.equals("fileLoginFlag")){
    String user = request.getParameter("j_username");
    String pwd = request.getParameter("j_password");
    String newPwd="",newPwdBase="",oldPwd="";
    String driverS=request.getParameter("driveSource");
    if(driverS==null) driverS="A:";
    boolean ifOk=false;
    fileLogin fl=new fileLogin();
    if(newLoginFlag.equals("newLogin")){
      //System.out.println("newLogin:"+newLoginFlag);
      oldPwd=request.getParameter("oldPwd");
      newPwd=pwd;
      try{
        //System.out.println(user+oldPwd+newPwd);
        if(fl.updatePwd(user,oldPwd, newPwd)){
          //System.out.println("更新成功！");
          %>
          <html>
          <head>
          </head>
          <body >
          <form name="getFileLogin3" id="getFileLogin3" method="post" action="j_security_check">
          <input type="hidden" name="j_username" value="<%=user%>">
          <input type="hidden" name="j_password" value="<%=pwd%>">
          </form>
          </body>
          </html>
          <script type="text/javascript">
          	//alert("newlogin:<%=user%><%=pwd%>");
	          document.getFileLogin3.submit();
          </script>
          
          <%}
          }catch (Exception ex){
            System.out.println( user+"更新新密码到数据库失败!");
          }
        }
        else{
          try{
            //System.out.println("解密前用户与密码："+user+","+pwd+","+driverS);
            user=fl.getFromBASE64(user);
            pwd=fl.getFromBASE64(pwd);
            oldPwd=pwd;

            //System.out.println("解密后用户与密码："+user+","+oldPwd+","+driverS);
            try{
              if(fl.updatePwd(user,oldPwd, oldPwd)){ //验证是否可以写入数据库
              	ifOk = true;
              }else{%>
              	<script type="text/javascript">
	            	  alert("请检查用户、密码输入的正确性。");
	            	  document.location="/stPortal/default.jsp";
	              </script>
	              <%
            	}
              //System.out.println( user+"验证是否可以写入数据库成功!");
            }catch (Exception ex){
              System.out.println( user+"验证是否可以写入数据库失败!");
            }

            if(ifOk){
              newPwd=fl.getStrRandom(99999999);
              pwd=newPwd;  //用新密码登陆，并且把新密码加密之后，写入软盘中
              newPwdBase=fl.getBASE64(newPwd);
              %>
              <html>
              <head>
              <script type="text/javascript">
              function window_onload2(){
                document.getFileLogin2.target="_top"
                var ifok="-1";
                var s="<%=newPwdBase%>";
                ifok=writeIntoFile("<%=driverS%>","<%=newPwdBase%>","WriteKey","");  //vbscript写key文件
                if(ifok=="-1"){
                  alert("新密码写入key盘失败，请联系管理员检查IE安全级别的设置！");
                  document.getFileLogin2.action="/stPortal/default.js";
                }else{
                  var allResult=getfileContent();
                  var s1=allResult.split(";");
                  s1=s1[1];
                  if(s!=s1){
                    alert("校认写入密钥盘的新密码不正确，请联系管理员检查IE安全级别的设置。");
                    document.getFileLogin2.action="/stPortal/default.js";
                  }else{
                    //提交认证，并且修改数据库
                    document.getFileLogin2.action="userlogin_file.jsp";
                    document.getFileLogin2.actionFlag.value="fileLoginFlag";
                    document.getFileLogin2.newLoginFlag.value="newLogin";
                  }
                }

                document.getFileLogin2.submit();
              }
              </script>
              </head>
              <body onload="javascript:window_onload2();">
              <form name="getFileLogin2" id="getFileLogin2" method="post" action="j_security_check">
              <input type="hidden" name="j_username" value="<%=user%>">
              <input type="hidden" name="j_password" value="<%=pwd%>">
              <input type="hidden" name="oldPwd" value="<%=oldPwd%>">
              <input type="hidden" name="actionFlag" value="">
              <input type="hidden" name="newLoginFlag" value="">
              </form>
              </body>
              </html>
              <%
              }              
            }catch (Exception ex){%>
              <script type="text/javascript">
	            	  alert("密钥盘内容有错误，请与管理员联系，重新制作。"+"<%=ex%>");
	            	  document.location="/stPortal/default.jsp";
	            </script>
            <%}
          }
        }
        else{
          %>

          <HTML><HEAD><TITLE>用户登录。</TITLE>
            <META http-equiv=Content-Type content="text/html; charset=gb2312">
              <META content="Microsoft.Net" name=Creator>
                <link href="../css/style.css" rel="stylesheet" type="text/css">

                <SCRIPT type="text/javascript">
                function login()
                {
                  var allResult=ifOkDriverFile();  //vbscript取key文件
                  if(allResult=="-1") {
                    alert("读取密钥盘有问题，请确定:密钥盘是否已经插入、是否写保护;密钥文件是否读写正确。或者联系管理员检查你的机器环境设置。");
                    return;
                  }else{
                    allResult=getfileContent();
                    if(allResult=="-1") {
                      alert("读取密钥盘内容失败，请确定是否插入密钥盘.");
                      return;
                    }
                    var result=allResult.split(";");
	                  document.all.item("j_username").value=result[0];
	                  document.all.item("j_password").value=result[1];
	
	                  if(result[0]=="") {
	                    alert("读取密钥盘用户内容失败，请确定密钥盘的用户文件是否正确.");
	                    return;
	                  }
	                  if(result[1]=="") {
	                    alert("读取密钥盘密码内容失败，请确定密钥盘的密码文件是否正确！");
	                    return;
	                  }
                  }
                  
                  document.login_server.action="userlogin_file.jsp";
                  document.login_server.actionFlag.value="fileLoginFlag";
                  document.login_server.driveSource.value=result[2];
                  document.login_server.target="_top"

                  document.login_server.submit();
                }

                function keyDown() {
                  //var keycode = String.fromCharCode(event.keyCode);
                  if ( event.keyCode == "13" ){
                    var oUserName=document.all.item("j_username");
                    var oPwd=document.all.item("j_password");
                    if(oUserName.value =="" ) {
                      alert("用户不能空!");
                      oUserName.focus();
                    }
                    if(oUserName.value !="" && oPwd.value ==""){
                      //alert("密码不能空!");
                      oPwd.focus();
                    }
                    if(oUserName.value !="" && oPwd.value !="")
                    login();
                  }
                }
                </SCRIPT>

                <META content="MSHTML 6.00.3790.2491" name=></HEAD>
                <BODY  margin="2" scroll="no">
                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                    <TBODY>
                      <TR>
                        <TD class=SubTitle align=left width="80%">
                          <IMG height=15 src="../images/arrow_login.gif" width=15 align=absMiddle border=0>
                            用户登录</TD></TR>
                            <TR vAlign=top>
                              <TD class=subWebTab align=left colSpan=3>
                                <FORM id="login_server" name="login_server" action="j_security_check" method="post"  target="_top">
                                  <TABLE cellSpacing=0 cellPadding=0 width="100%" border=0>
                                    <TBODY>
                                      <TR class="subTab" align="middle" style="visibility:hidden">
                                        <TD vAlign="bottom" height="30">
                                          用户： <INPUT class="textfield" maxLength="20" value="" size="16" id="j_username" name="j_username" onkeydown="javascript:keyDown();"></TD></TR>
                                          <TR align=right>
                                            <TD vAlign="bottom" height="30" >
                                              <SPAN align="LEFT" style="COLOR:RED;FONT:12pt">请插入密钥盘，点击登录。</SPAN>
                                              <!--<INPUT class=GoBt id=button onclick="openregister()" type=button value=注册 name=button target="_top">-->
                                                <!--<INPUT class="GoBt" id="button" onclick="javascript:login();" type="button" value="登录" name="button">-->
                                                  <INPUT class="GoBt" id="button" type="reset" value="取消" name="button" style="visibility:hidden">
                                                    <input type="hidden" name="actionFlag" value="">
                                                    <input type="hidden" name="driveSource" value="">

                                            </TD></TR>
                                            <TR class="subTab" align="middle" style="visibility:">
                                              <TD vAlign="bottom" height="30">
                                                <INPUT class="GoBtPicture" id="button" onclick="javascript:login();" type="button" value="" name="button">
                                              </TD></TR>
                                              <TR class="subTab" align="middle" style="visibility:">
                                                <TD vAlign="bottom" height="30">
                                                </TD></TR>
                                                <TR>
                                                  <TD colspan=3 class=subTab vAlign=bottom align=left height="" style="FONT:10pt">
                                                    <A href="/STPublish/view/ViewRecord.jsp?id=2518" target="_blank">
                                                      为更好办公，请设置环境。</A></TD>
                                                </TR>
                                                <TR class="subTab" align="middle" style="visibility:hidden">
                                                  <TD vAlign="bottom" height="30">
                                                    密码： <INPUT class="textfield" type="password" value="" maxLength="20" size="16" id="j_password" name="j_password" onkeydown="javascript:keyDown();"> </TD></TR>

                                    </TBODY></TABLE></FORM>

                                                  </TD></TR></TBODY></TABLE>
                                                  <SCRIPT language=javascript>
                                                  //隐藏则不能使用focus
                                                  //document.all.item("j_username").focus();
                                                  </SCRIPT>
                                                  </BODY></HTML>
                                                  <%}%>
