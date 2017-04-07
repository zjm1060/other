<%@ page contentType="text/html; charset=GBK" %>
<%
 String currentUser=request.getRemoteUser();
 if(currentUser==null) currentUser="";
%>
<html>
<head>
<title>广州市番禺区人民政府审批服务中心</title>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<!--Fireworks MX 2004 Dreamweaver MX 2004 target.  Created Thu Apr 18 16:18:45 GMT+0800 2002-->
<style type="text/css">
<!--
body {
	margin-left: 0px;
	margin-top: 0px;
	margin-right: 0px;
	margin-bottom: 0px;
	background-color: #02C9FF;
}
-->
</style></head>

<script language="JavaScript" type="text/JavaScript">
<!--
function MM_swapImgRestore() { //v3.0
  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
}

function MM_preloadImages() { //v3.0
  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
}

function MM_findObj(n, d) { //v4.01
  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
  if(!x && d.getElementById) x=d.getElementById(n); return x;
}

function MM_swapImage() { //v3.0
  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
}
//-->
</script>
<body onLoad="MM_preloadImages('images/menu__r1_c1.png','images/button1_.png','images/button2_.png','images/button3_.png','images/button4_.png','images/button5_.png','images/menu__r1_c4.png','images/menu__r1_c6.png','images/menu__r1_c8.png','images/menu__r1_c10.png')">
<table width="1002" border="0" align="center" cellpadding="0" cellspacing="0">
<!-- fwtable fwsrc="login.png" fwbase="login.png" fwstyle="Dreamweaver" fwdocid = "536205801" fwnested="0" -->
  <tr>
   <td><img src="images/spacer.gif" width="192" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="810" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="1" height="1" border="0" alt=""></td>
  </tr>

  <tr>
    <td width="192" height="319" rowspan="2" align="right" valign="center" background="images/login_r1_c.png">
	     <table width="100%"  border="0" align="right" cellpadding="0" cellspacing="1">
	     <%if(currentUser.equals("guest") || currentUser.equals("admin_public")){%>
		     <tr>
		       <td height="39" align="right" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0015&DashboardType=0'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('photo1','','images/button1_.png',1)"><img src="images/button1.png" width="181" height="34" border="0" name="photo1"></a></td>
		     </tr>
		     <tr>
		       <td height="39" align="right" onClick="javascript:window.location='/stPortal/py/userLogin.jsp'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('photo2','','images/button2_.png',1)"><img src="images/button2.png" width="181" height="34" border="0" name="photo2"></a></td>
		     </tr>
		     
		     <tr>
		       <td height="39" align="right" onClick="javascript:window.location='/ApproveWeb/RegWeb/register.jsp?noPwd=1'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('photo5','','images/button5_.png',1)"> <img src="images/button5.png" width="181" height="34" border="0" name="photo5"></a></td>
		     </tr>
		   <%}else{%>
		   	  <tr>
		       <td height="39" align="right" onClick="javascript:window.location='/stPortal/py/userLogin.jsp'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('photo2','','images/button2_.png',1)"><img src="images/button2.png" width="181" height="34" border="0" name="photo2"></a></td>
		     <br><br><br><br><br><br>
		     </tr>
		     <tr></tr>
		     <tr></tr>
		     <tr></tr>
		     <tr></tr>
		     <tr>
		       <td height="39" align="right" onClick="javascript:window.location='/ApproveWeb/RegWeb/register.jsp?noPwd=1'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('photo5','','images/button5_.png',1)"> <img src="images/button5.png" width="181" height="34" border="0" name="photo5"></a></td>
		     </tr>
		   <%}%>  
	     </table>
   </td>
   
   
   <td width="810" height="128" background="images/login_r1_c2.png">&nbsp;</td>
   <td><img src="images/spacer.gif" width="1" height="128" border="0" alt=""></td>
  </tr>
  <tr>
   <td><img name="login_r2_c2" src="images/login_r2_c2.png" width="810" height="191" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="1" height="191" border="0" alt=""></td>
  </tr>
  <tr>
   <td height="223" colspan="2" background="images/login_r3_c1.png">&nbsp;</td>
   <td><img src="images/spacer.gif" width="1" height="223" border="0" alt=""></td>
  </tr>
  <tr>
   <td height="58" colspan="2" valign="top" background="images/login_r4_c1.png"><table width="900"  border="0" align="center" cellpadding="0" cellspacing="0">
   
   	 <%if(currentUser.equals("guest") || currentUser.equals("admin_public")){%>
     <tr>
       <td align="center" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0024&DashboardType=0'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image1','','images/menu__r1_c1.png',1)"><img src="images/menu_r1_c1.png" width="159" height="51" border="0" name="Image1"></a></td>
       <td align="center" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0025&DashboardType=0'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image2','','images/menu__r1_c4.png',1)"><img src="images/menu_r1_c4.png" width="159" height="51" border="0" name="Image2"></a></td>
       <td align="center" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0003&DashboardType=0'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image3','','images/menu__r1_c6.png',1)"><img src="images/menu_r1_c8.png" width="159" height="51" border="0" name="Image3"></a></td>
       <td align="center" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0004&DashboardType=0'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image4','','images/menu__r1_c8.png',1)"><img src="images/menu_r1_c10.png" width="159" height="51" border="0" name="Image4"></a></td>
       <td align="center" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0029&DashboardType=0'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image5','','images/menu__r1_c10.png',1)"><img src="images/menu_r1_c1_r4.png" width="159" height="51" border="0" name="Image5"></a></td>
     </tr>
	 
	 <%}else{%>
	 
	       <TR align="center">
           <td align="center" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0036&DashboardType=0'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image1','','images/menu4_.png',1)"><img src="images/menu4.png" width="159" height="51" border="0" name="Image1"></a></td>
             
                   <td align="center" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0004&DashboardType=0'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image1','','images/menu__r1_c10.png',1)"><img src="images/menu_r1_c10.png" width="159" height="51" border="0" name="Image1"></a></td>
                   
                   <td align="center" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0039&DashboardType=0'"><a href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image1','','images/menu5_.png',1)"><img src="images/menu5.png" width="159" height="51" border="0" name="Image1"></a></td>
	      <%}%>
   </table></td>
   <td><img src="images/spacer.gif" width="1" height="58" border="0" alt=""></td>
  </tr>
  <tr>
   <td colspan="2"><img name="login_r5_c1" src="images/login_r5_c1.png" width="1002" height="38" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="1" height="38" border="0" alt=""></td>
  </tr>
</table>
</body>
</html>


<%if(currentUser.equals("guest") || currentUser.equals("admin_public")){%>
<script>
  var w=window.screen.width-200;
	var h=window.screen.height-255;
	var sStyle="width="+w+"px,height="+h+"px,left=80px,top=80px,scrollbars=yes,resizable=yes";
	var sStyle="width="+w+"px,height="+h+"px,left=80px,top=80px,scrollbars=yes,resizable=yes";
	window.open("/STPublish/view/ViewRecord.jsp?id=2541","",sStyle);
</script>
<%}%>