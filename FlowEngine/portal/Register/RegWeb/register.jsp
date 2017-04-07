<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="java.util.Hashtable,com.suntek.register.outer.Optional" %>
<html>
<head>
<title>外网用户注册</title>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">

<link rel="stylesheet" type="text/css" href="./Common/Css/Public.css">
<script language="javascript" src="./Common/Include/Common.js"></script>
<script language="javascript" src="./Common/Include/i_CheckInput.js"></script>
</head>
<%
    String fullName        = "";
    String userCode        = "";
    String strSex          = "";
    String identityCard    = "";
    String description     = "";
    String phone           = "";
    String mobile          = "";
    String email1          = "";
    String email2          = "";
    String address         = "";
    String pwd		   = "";
    String shortName	   = "";
    String sUrl="";

    Optional opt = new Optional();
    sUrl = opt.getURL();

    String actionFlag=request.getParameter("actionFlag");
    if(actionFlag==null)
       actionFlag="";
    if(!actionFlag.equals("saveIt")){
%>
<script language="javascript">
window.name="win_frmPost";
function doSave(){
  if (CheckAction("./Common/Include/")) {
    frmPost.actionFlag.value="saveIt";
    frmPost.method="post";
    frmPost.submit();
    return true;
  }else{
    return false;
  }
}
</script>
<body rightmargin="0" bottommargin="0"  leftmargin="0" topmargin="0">
<form  name="frmPost" action="register.jsp" target="win_frmPost">
   <div align="center"><br><br><br>
     <table width="90%" cellpadding="0" cellspacing="1">
      <tr><td align="center" valign="center" height="30px" class="headline-bg"><strong>外 网 用 户 注 册</strong></td></tr>
      <tr><td class="boarder">
       <table width="100%" cellpadding="3" cellspacing="1">
         <tr class="content-bg">
           <td width="20%" align="right">登入名：</td>
           <td width="30%" align="left">
               <input type="text" name="userCode" class="TextUtil_input" style="WIDTH: 98%"   autocheck="1"  cannull="0" propname="英文名">
           </td>
           <td width="20%" align="right" class="content-text">姓名：</td>
           <td width="30%" align="left">
               <input type="text"  name="fullName" class="TextUtil_input" style="WIDTH: 100%"   autocheck="1" cannull="0" propname="姓名">
           </td>
          </tr>
         <tr class="content-bg">
           <td width="20%" align="right">密码：</td>
           <td width="30%" align="left">
               <input type="password"  name="pwd" class="TextUtil_input" style="WIDTH: 100%"   autocheck="1" cannull="0" propname="密码">
           </td>
           <td width="20%" align="right">缩写名：</td>
           <td width="30%" align="left">
               <input type="text" name="shortName" class="TextUtil_input" style="WIDTH: 98%"   autocheck="1"  cannull="1" propname="缩写名">
           </td>
          </tr>
          <tr class="content-bg">
            <td width="20%" align="right">性别：</td>
            <td width="20%" align="left">
                <select name="sex" style="WIDTH: 80px"  autocheck="1" cannull="0" propname="性别" >
                    <option value="0" selected>男</option>
                    <option value="1">女</option>
                </select>
             </td>
             <td width="20%" align="right">证件号码： </td>
             <td width="40%" align="left">
                 <input type="text" name="identityCard" class="TextUtil_input" style="width:98%" autocheck="1" datatype = "number"  cannull="1" propname="证件号码">
             </td>
            </tr>
            <tr class="content-bg">
               <td width="20%" align="right">直拨电话：</td>
               <td width="20%" align="left">
                  <input type="text"   name="phone" class="TextUtil_input" style="WIDTH:  100%"  datatype="tel" autocheck="1"  cannull="1" propname="直拨电话" >
               </td>
               <td width="20%" align="right">手机：</td>
               <td width="40%" align="left">
                  <input type="text"  name="mobile"  class="TextUtil_input" style="WIDTH: 97%"  autocheck="1"  cannull="1" propname="手机号码" >
                </td>
             </tr>
             <tr class="content-bg">
                 <td width="20%" align="right" >电子邮件1:</td>
                 <td colspan="3" align="left">
                    <input type="text" name="email1" class="TextUtil_input" style="WIDTH: 98%"  autocheck="1" maxlen="250" cannull="1" propname="电子邮件1" datatype="email">
                 </td>
              </tr>
              <tr class="content-bg">
                 <td width="20%" align="right">电子邮件2:</td>
                 <td colspan="3" align="left">
                     <input type="text" name="email2" class="TextUtil_input" style="WIDTH: 98%"   autocheck="1" maxlen="250" cannull="1" propname="电子邮件2" >
                 </td>
              </tr>
              <tr class="content-bg">
                 <td width="20%" align="right">家庭住址:</td>
                 <td colspan="3" align="left">
                     <input type="text" name="address" class="TextUtil_input" style="WIDTH: 98%"   autocheck="1" maxlen="100" cannull="1" propname="家庭住址" >
                 </td>
            </tr>
            <tr valign="top" class="content-bg">
               <td width="20%" align="right" class="content-text">简介：</td>
               <td width="80%" colspan="3" class="content-text">
                   <textarea rows="2" name="description" style="WIDTH: 98%;height:50px" wrap="PHYSICAL" autocheck="1"  cannull="1" propname="个人简介"></textarea>
               </td>
            </tr>
      </table>
    </td></tr>
    </table>
   </div>
   <br>
   <div align="center">
     <input type="hidden" name="actionFlag" value="">
     <input type="button" name="save"  class="Btn_common" value=" 登 录 " onclick="doEnter()">
     <input type="button" name="save"  class="Btn_common" value=" 注 册 " onclick="doSave()">
     <input type="reset" name="reset"  class="Btn_common" value=" 重 写 ">
  </div>
  </form>
</body>
<%
}else{

    fullName    = request.getParameter("fullName");
    userCode    = request.getParameter("userCode");
    strSex      = request.getParameter("sex");
    identityCard= request.getParameter("identityCard");
    description = request.getParameter("description");
    phone       = request.getParameter("phone");
    mobile      = request.getParameter("mobile");
    email1      = request.getParameter("email1");
    email2      = request.getParameter("email2");
    address     = request.getParameter("address");
    pwd     = request.getParameter("pwd");
    shortName     = request.getParameter("shortName");

    Hashtable userInfo = new Hashtable();

    userInfo.put("full_name", fullName);
    userInfo.put("user_code", userCode);
    userInfo.put("user_sex", strSex);
    userInfo.put("user_iden", identityCard);
    userInfo.put("user_tel", phone);
    userInfo.put("mobi_tel", mobile);
    userInfo.put("e1_mail", email1);
    userInfo.put("e2_mail", email2);
    userInfo.put("user_addr", address);
    userInfo.put("user_pwd", pwd);
    userInfo.put("user_shor", shortName);
    userInfo.put("user_desc", description);

    boolean blnSucc= true;
    boolean blnExist=false;
    if(opt.isExist(userCode)){//用户存在
      System.err.println(userCode+"该用户存在！");
      blnExist = true;
    }else{
	blnSucc=opt.enrollUser(opt.getRegID(),opt.getSRegID(),userInfo);
    }
%>
<body>
<table width="30%" height="100%" border="0" align="center" cellpadding="3" cellspacing="1">
  <tr>
    <td>
        <%
          if(blnExist){
	%>
           <p align="center" class="headline-bg"><%=userCode%> 用户存在！</p>
           <p align="center" class="content-bg"><a style="cursor:hand;" onclick="history.back(-1);">返回注册</a></p>
        <%
          }else{//
            if(!blnSucc){
        %>
               <p align="center" class="headline-bg"><%=userCode%>注册失败!</p>
               <p align="center" class="content-bg"><a style="cursor:hand;" onclick="history.back(-1);">返回注册</a></p>
        <%
            }else{
        %>
               <p align="center" class="headline-bg"><%=userCode%>注册成功！</p>
               <p align="center" class="content-bg"><a href="<%=sUrl%>">登录</a></p>
        <%  }
          }//end if
	%>
     </td>
  </tr>
</table>
</body>
<%}%>
<script>
function doEnter(){
 window.location ="<%=sUrl%>";
}
</script>
</html>

