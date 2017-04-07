<%@ page contentType="text/html;charset=GBK" %>
<%@ page import="shecasafeapi.SafeEngine" %>
<%@ page import="baseserverjb.BaseServerClient" %>
<%@ page import="java.sql.*" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
  <title>欢迎登录柳州市电子政务系统!</title>
  <style type="text/css">
  <!--
  BODY
  {
  font-family:"宋体";
  font-size: 12px;
  background-color:#3B97FF;
  margin:0px 0px 0px 0px;
  scrollbar-face-color:DCE0E2;
  scrollbar-shadow-color:687888;
  scrollbar-highlight-color:ffffff;
  scrollbar-3dlight-color:687888;
  scrollbar-darkshadow-color:DCE0E2;
  scrollbar-track-color:F4F4F0;
  scrollbar-arrow-color:6E7E88;
  overflow-x:auto;
  }
  td
  {font-size:12px;}
  .defaultinput {
  font-size: 12px;
  height: 18px;
  width: 120px;
  border:1px solid #000000;
  }
  .Leftbg1 {
  background-image: url(/Login/images/left1.jpg);
  background-repeat: no-repeat;
  background-position: left top;
  width: 360px;
  height: 450px;
  vertical-align: top;
  }
  .Rightbg1 {
  background-image: url(/Login/images/right1.jpg);
  background-repeat: no-repeat;
  background-position: left bottom;
  padding-top:260px;
  vertical-align: top;
  padding-left: 120px;
  }
  .Enterword1 {
  font-weight: bold;
  font-size: 13px;
  }
  -->
  </style>
  <%!
  public String[] genCARandom(int PriKeyDevType,
  String strPrivKeyDevParam,
  String strPriKeyPass,
  int lPriKeyTimeout,
  int CertChainDevType,
  String strCertChainDevParam,
  String strCertChainPass,
  int CertDevType,
  String strCertDevParam,
  String strCertPass,
  javax.servlet.jsp.JspWriter out){
    String strRandom=null;
    String strServerCert=null;
    shecasafeapi.SafeEngine myobj=null;
    try{
      myobj = new shecasafeapi.SafeEngine();
      int i=myobj.shecaInitEnviroment(PriKeyDevType,strPrivKeyDevParam, strPriKeyPass, lPriKeyTimeout, CertChainDevType, strCertChainDevParam, strCertChainPass);
      if (i!=0){
        out.println("<script>alert(\"初始化safeengine错误:"+i+"\");</script>");
      }
      else{
        //获取服务器证书
        strServerCert = myobj.shecaGetSelfCertificate(CertDevType, strCertDevParam, strCertPass);
        if (myobj.getErrorCode()!=0){
          out.println("<script>alert(\"读取服务器证书错误\");</script>");
        }
        else{
          //产生随机数
          java.util.Random RandNum = new java.util.Random();
          int RandomData = RandNum.nextInt(1000000);
          strRandom = ""+ RandomData;
        }
      }
    }
    catch(Exception ex){
      ex.printStackTrace();
    }
    finally{
      if(myobj!=null){
        myobj.shecaClearEnviroment();
        myobj=null;
      }
      String[] sRet=new String[2];
      sRet[0]=strRandom;
      sRet[1]=strServerCert;
      return sRet;
    }
  }
  public boolean checkKey(
  int invokeType,
  int PriKeyDevType,
  String strPrivKeyDevParam,
  String strPriKeyPass,
  int lPriKeyTimeout,
  int CertChainDevType,
  String strCertChainDevParam,
  String strCertChainPass,
  int CertDevType,
  String strCertDevParam,
  String strCertPass,
  javax.servlet.jsp.JspWriter out,
  javax.servlet.http.HttpServletRequest request,
  javax.servlet.http.HttpSession session
  ){
    boolean bCaPass=false;
    shecasafeapi.SafeEngine myobj=null;
    String postfix="";
    if(invokeType==2) postfix="2";
    try{
      myobj = new shecasafeapi.SafeEngine();
      int i=myobj.shecaInitEnviroment(PriKeyDevType,strPrivKeyDevParam, strPriKeyPass, lPriKeyTimeout, CertChainDevType, strCertChainDevParam, strCertChainPass);
      if (i!=0){
        //out.println("<script>alert(\"初始化safeengine错误:"+i+"\");</script>");
        System.err.print("初始化safeengine错误："+i);
      }
      String strServerCert  = myobj.shecaGetSelfCertificate(CertDevType, strCertDevParam, strCertPass);
      if (myobj.getErrorCode() != 0){
        //out.println("<script>alert(\"获取服务器证书错误："+myobj.getErrorCode()+"\");</script>");
        System.err.print("获取服务器证书错误："+myobj.getErrorCode());
      }
      else{
        String strClientCert = request.getParameter("sCert");
        String strSigned = request.getParameter("sSign");
        String strEnvelope = request.getParameter("sEnvelope");
        //验证客户端证书
        i=myobj.shecaVerifyCertificate(strClientCert);
        if (i!=0){
          //out.println("<script>alert(\"验证客户端证书错误：\");</script>");
          System.err.println("验证客户端证书错误："+i);
        }
        else{
          //解密数字信封
          String strData = myobj.shecaEnvelope(2, strEnvelope, strServerCert);
          if (myobj.getErrorCode() != 0){
            out.println("<script>alert(\"解数字信封错误："+myobj.getErrorCode()+"\");</script>");
            System.err.println("解数字信封错误："+myobj.getErrorCode());
          }
          else{
            //验证客户端签名
            i=myobj.shecaVerifySignData(strData, 3, strSigned, strClientCert);
            if (i != 0){
              System.err.println("<script>alert(\"验证签名错误："+i+"\");</script>");
              System.err.println("验证签名错误："+i);
            }
            else{
              if (!(session.getAttribute("random"+postfix).equals(strData))){
                System.err.println("随机数错误strData is "+strData+"\nsession is "+session.getAttribute("random"+postfix));
              }
              else{
                //CA证书验证通过
                bCaPass=true;
              }
            }			// end if VerifySignData
          }				//end if Envelope
        }					//end if VerifyCertificate
      }						//end if GetSelfCertificate
    }							//end if InitEnviroment
    catch(Exception ex){
      ex.printStackTrace();
    }
    finally{
      if(myobj!=null){
        myobj.shecaClearEnviroment();
        myobj=null;
      }
      return bCaPass;
    }
  }
  %>
  <%
  if (request.getProtocol().compareTo("HTTP/1.0") == 0)
  response.setHeader("Pragma", "no-cache");
  else if (request.getProtocol().compareTo("HTTP/1.1") == 0)
  response.setHeader("Cache-Control", "no-cache");
  response.setDateHeader("Expires", 0);

  String username=""; //隐藏用户名
  //begin ca
  String userDir = System.getProperty("user.dir");
  String fileSeparator = System.getProperty("file.separator");
  String path=userDir+fileSeparator+"CA"+fileSeparator;
  String path2=userDir+fileSeparator+"CA2"+fileSeparator;
  String strServerCert1 = "";
  String strServerCert2 = "";
  int i = 0;

  String strCertChainDevParam2="COM1";
  String strCertDevParam2="COM1";
  String strCertChainPass2="615e8502";
  //设备类型，9--usb，2--文件
  int PriKeyDevType=9;
  String strPrivKeyDevParam="COM1";
  String strPrivKeyDevParam2="COM1";
  String strPriKeyPass="123456";
  int lPriKeyTimeout=0;
  //设备类型，9--usb，2--文件
  int CertChainDevType=9;
  String strCertChainDevParam="COM1";
  String strCertChainPass="123456";
  //String strCertChainPass2="615e8502";
  //设备类型，9--usb，2--文件
  int CertDevType=9;
  String strCertDevParam="COM1";
  String strCertPass="123456";
  String strCertPass2="615e8502";
  String strPriKeyPass2="615e8502";
  boolean isUsb=false;
  if(!isUsb){
    //设备类型，9--usb，2--文件
    PriKeyDevType=2;
    strPrivKeyDevParam=path+"userkey.key";
    strPrivKeyDevParam2=path2+"userkey.key";
    strPriKeyPass="123456";
    strPriKeyPass2="615e8502";
    lPriKeyTimeout=1000;
    //设备类型，9--usb，2--文件
    CertChainDevType=2;
    strCertChainDevParam=path+"CertChain.spc";
    strCertChainDevParam2=path2+"CertChain.spc";
    strCertChainPass="123456";
    strCertChainPass2="615e8502";
    //设备类型，9--usb，2--文件
    CertDevType=2;
    strCertDevParam=path+"usercert.der";
    strCertDevParam2=path2+"usercert.der";
    strCertPass="123456";
    strCertPass2="615e8502";
  }
  //=====================================================
  String actionFlag=request.getParameter("actionFlag");
  if(actionFlag==null) actionFlag="";
  String strRandom1;
  String strRandom2;
  if(!actionFlag.equals("login")){
    //产生随机数
    String[] sReturn1,sReturn2;
    sReturn1=genCARandom(PriKeyDevType,strPrivKeyDevParam, strPriKeyPass, lPriKeyTimeout, CertChainDevType, strCertChainDevParam, strCertChainPass,CertDevType, strCertDevParam, strCertPass,out);
    sReturn2=genCARandom(PriKeyDevType,strPrivKeyDevParam2, strPriKeyPass2, lPriKeyTimeout, CertChainDevType, strCertChainDevParam2, strCertChainPass2,CertDevType, strCertDevParam2, strCertPass2,out);
    strRandom1=sReturn1[0];
    strRandom2=sReturn2[0];
    strServerCert1=sReturn1[1];
    strServerCert2=sReturn2[1];
    session.setAttribute("random",strRandom1);
    session.setAttribute("random2",strRandom2);

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
    <SCRIPT type="text/javascript">
    function clearSession(){
      try{
        //如果不是刷新窗口则是关闭窗口。
        if(IECloseMonitor.isWindowClosing()){
          xhreq = new ActiveXObject("Microsoft.XMLHTTP");
          xhreq.open("GET", "/portallogout?windowclose=1", false);
          xhreq.setRequestHeader("Content-Type","text/html");
          xhreq.send();
          var returnValue= xhreq.statusText;
        }
      }catch(e){}
    }
    <!--
    function login(){
      document.all.item("actionFlag").value="login";
      login_server.submit();
    }
    function reset(){
      login_server.reset();
    }
    function window_onload(){
      document.all.item("pwd").focus();
    }
    function checkUserPwd(){
      if(doTest()){
        login();
      }
    }
    function keydownLogin(){
      if(event.keyCode=="13")
      checkUserPwd();
    }
    //-->
    var popUpWin=0;
    function popUpWindow(URLStr, left, top, width, height)
    {
      if(popUpWin)
      {
        if(!popUpWin.closed) popUpWin.close();
      }
      popUpWin = open(URLStr, 'popUpWin', 'toolbar=no,location=no,directories=no,status=no,menub ar=no,scrollbar=no,resizable=no,copyhistory=yes,width='+width+',height='+height+',left='+left+', top='+top+',screenX='+left+',screenY='+top+'');
    }


    </SCRIPT>
    <body onunload="javascript:clearSession();" onload="javascript:popUpWindow('notice1.html',0,0,300,343);">
    <OBJECT id="IECloseMonitor"
    classid="clsid:2BA5FD98-D118-469C-868F-351BC58EA0AA"
    codeBase="IECloseMonitor.dll">
    width="0"
    height="0"
    </OBJECT>



    <div id="Layer1" style="position:absolute; width:286px; height:71px; z-index:1; left: 0px; top: 4px;">
      <p><strong><font size="4">　用户服务电话:2859413</font></strong></p>
      <p><strong><font size="4">　<a href="notice.html" target="_blank">关于上海市CA中心广西<br>
        分中心证书受理点搬迁的通知</a></font></strong></p>
        <SPAN
        title=柳州OA办公自动化系统 style="CURSOR: hand"
        onclick="javascript:window.external.addFavorite('http://10.18.1.17/','柳州OA办公自动化系统')"><FONT
          color=#ffffff size="4">加入收藏</FONT></SPAN>
    </div>

    <OBJECT ID="SafeEngineCtl" CLASSID="CLSID:B48B9648-E9F0-48A3-90A5-8C588CE0898F" width="0" height="0" border=0 codebase="SafeEngineCOM.cab#Version=1,2,0,0" ></OBJECT>
    <form name="login_server" method="post" action="/Login/enter.jsp" id="login_server" >
    <table width="100%" style="height:100%;" border="0" cellpadding="0" cellspacing="0">
      <tr>
        <td align="center" valign="middle"><table width="781" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td class="Leftbg1"><div id="Layer1" style="position:absolute; z-index:1;">

</div></td>
<td class="Rightbg1"><table width="250" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td colspan="3"><input name="username" type="hidden" class="enterword1 value="<%=username%>"></td>
  </tr>
  <tr>
    <td  align="right" height="40" class="enterword1">证书密码:</td>
    <td >
      <input type="hidden" name="actionFlag" value="">
      <input type=hidden name=sCert value="">
      <input type=hidden name=sSign value="">
      <input type=hidden name=sEnvelope value="">
      <input type=password name=pwd class="defaultinput" onkeydown=keydownLogin();>
    </td>
  </tr>
  <tr>
    <td height="60" style="padding-left:80px;">
      <A  href="userbook.zip" target=_blank><strong><font size="+1">驱动</strong></A></td>
      <td ><input type="image" src="/Login/images/enter.gif" onclick="javascript:checkUserPwd();">
        &nbsp;<input type="image" src="/Login/images/reset.gif" onclick="javascript:reset();" ></td>
  </tr>
    </table></td>
          </tr>
</table></td>
      </tr>
</table>
    </form>
    <script type="text/javascript" src="ca.js"></script>
    <script type="text/javascript">
    function toStr(code){
      return printStatus(code)+" !ErrorCode="+code;
    }
    function doTest()
    {
      /* Change the path and password below */
      strpassword=document.login_server.pwd.value;
      if(strpassword==null || strpassword==""){
        alert("请输入密码");
        document.login_server.pwd.focus();
        return false;
      }

      /* 初始化明华usb */
      SafeEngineCtl.SEH_InitialSession(9,"COM1", strpassword, 0, 9, "COM1", strpassword);
      if(SafeEngineCtl.ErrorCode!=0)
      {
        alert("登陆失败,"+toStr(SafeEngineCtl.ErrorCode));
        return false;
      }
      /* 获取自己证书 */
      strCert = SafeEngineCtl.SEH_GetSelfCertificate(9,"COM1",strpassword);
      if(SafeEngineCtl.ErrorCode!=0)
      {
        alert("SEH_GetSelfCertificate Error. Return:" + toStr(SafeEngineCtl.ErrorCode));
        return false;
      }
      document.login_server.sCert.value = strCert;
      /*获取序列号?用户名?*/
      var certUserName=SafeEngineCtl.SEH_GetCertDetail(strCert,2);//2为序列号，17为用户中文名
      //alert(certUserName);
      document.login_server.username.value=certUserName;

      /*
      var userName=document.login_server.username.value;
      if(certUserName!=userName){
        alert("用户名与证书用户名不匹配！");
        return false;
      }
      */
      /* 验证服务器证书 */
      SafeEngineCtl.SEH_VerifyCertificate("<%=strServerCert1%>");
      var ff=1;
      if(SafeEngineCtl.ErrorCode!=0)
      {
        ff=0;
        SafeEngineCtl.SEH_VerifyCertificate("<%=strServerCert2%>");
        if(SafeEngineCtl.ErrorCode!=0)
        {
          alert("SEH_VerifyCertificate Error. Return:" +toStr(SafeEngineCtl.ErrorCode));
          SafeEngineCtl.SEH_ClearSession();
          return false;
        }
      }

      /* 签名随机数 */
      if(ff==1){
        strSigned = SafeEngineCtl.SEH_SignData("<%=strRandom1%>", 3);
      }
      else{
        strSigned = SafeEngineCtl.SEH_SignData("<%=strRandom2%>", 3);
      }
      if(SafeEngineCtl.ErrorCode!=0)
      {

        alert("SEH_SignData Error. Return:" + toStr(SafeEngineCtl.ErrorCode));
        SafeEngineCtl.SEH_ClearSession();
        return false;
      }

      document.login_server.sSign.value = strSigned;

      /* 用服务器证书加密随机数 */
      if(ff==1){
        strEnvelope = SafeEngineCtl.SEH_Envelope(1, "<%=strRandom1%>", "<%=strServerCert1%>");
      }
      else{
        strEnvelope = SafeEngineCtl.SEH_Envelope(1, "<%=strRandom2%>", "<%=strServerCert2%>");
      }
      if(SafeEngineCtl.ErrorCode!=0)
      {
        alert("SEH_Envelope(Enc) Error. Return:" + toStr(SafeEngineCtl.ErrorCode));
        SafeEngineCtl.SEH_ClearSession();
        return false;
      }
      document.login_server.sEnvelope.value = strEnvelope;

      /* 释放 */
      SafeEngineCtl.SEH_ClearSession();
      return true;
    }
    </script>
    <%}else{
    boolean bCaPass1=checkKey(1,PriKeyDevType,strPrivKeyDevParam, strPriKeyPass, lPriKeyTimeout, CertChainDevType, strCertChainDevParam, strCertChainPass,CertDevType, strCertDevParam, strCertPass,out,request,session);
    boolean bCaPass2=checkKey(2,PriKeyDevType,strPrivKeyDevParam2, strPriKeyPass2, lPriKeyTimeout, CertChainDevType, strCertChainDevParam2, strCertChainPass2,CertDevType, strCertDevParam2, strCertPass2,out,request,session);
    boolean bCaPass=bCaPass1 || bCaPass2;
    if(bCaPass){
      String loginPwd=request.getParameter("pwd");
      String loginUser=request.getParameter("username");
      String checkAddr="10.18.1.19";
      //String checkAddr="192.168.1.19";
      Connection conn=null;
      PreparedStatement pstm=null;
      try{
        if(loginUser!=null){
          loginUser=baseserverjb.BaseServerClient.getUserbyCASN(loginUser);
        }
        session.setAttribute("ca",loginPwd);
        conn=baseserverjb.BaseServerClient.getWizardConnection();
        String caSn=request.getParameter("username");
        if(caSn==null) caSn="";
        String sqlClause = " select u_password from users where ca_sn=?";
        pstm=conn.prepareStatement(sqlClause);
        pstm.setString(1,caSn);
        ResultSet rs= pstm.executeQuery();
        if(rs.next()){
          loginPwd = rs.getString("u_password");
        }
        rs.close();
        rs=null;
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
      %>
      <body>
      <!--
      <iframe id="ddddd"></iframe>
      -->
      <form name="formLogin" method="post" action="j_security_check">
      <input name="j_password" type="hidden" value="<%=loginPwd%>">
      <input name="j_username" type="hidden" value="<%=loginUser%>">
      </form>
      </body>
</html>
<SCRIPT type="text/VBScript">
Function URLEncoding(vstrIn)
strReturn = ""
For t = 1 To Len(vstrIn)
ThisChr = Mid(vStrIn,t,1)
If Abs(Asc(ThisChr)) < &HFF Then
strReturn = strReturn & ThisChr
Else
innerCode = Asc(ThisChr)
If innerCode < 0 Then
innerCode = innerCode + &H10000
End If
Hight8 = (innerCode And &HFF00)\ &HFF
Low8 = innerCode And &HFF
strReturn = strReturn & "%" & Hex(Hight8) & "%" & Hex(Low8)
End If
Next
URLEncoding = strReturn
End Function
</SCRIPT>
<script type="text/javascript">
function loginWithForm(url,formData){
  xhreq = new ActiveXObject("Microsoft.XMLHTTP");
  strUrl =url;
  formData =URLEncoding(formData);
  xhreq.open("POST", strUrl, false);
  xhreq.setRequestHeader("Content-Type","application/x-www-form-urlencoded");
  xhreq.send(formData);
  var returnValue= xhreq.statusText;
}
</script>
<script type="text/javascript">
  var userName="<%=loginUser%>";
  var pwd="<%=loginPwd%>";
  loginWithForm("http://10.18.1.19:8001/j_security_check","j_username="+userName+"&j_password="+pwd);
  //document.formLogin.target="ddddd";
  document.formLogin.submit();
  //document.location="/stPortal/default.jsp";
  </script>
  <%}else{
  response.sendRedirect("/Login/enter.jsp");
}
}
%>

