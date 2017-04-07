<%@ page contentType="text/html; charset=GBK" %>
<%
 String currentUser=request.getRemoteUser();
 if(currentUser==null) currentUser="";
%>
<html>
<head>
<title>广州市番禺区人民政府审批服务中心</title>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312">
<!--Fireworks MX 2004 Dreamweaver MX 2004 target.  Created Sun Feb 17 09:56:21 GMT+0800 2008-->
<style type="text/css">
body {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size:12px;
	background-image: url(images/bg.gif);
	margin-left: 0px;
	margin-top: 50px;
	margin-right: 0px;
	margin-bottom: 0px;
}

body,td,th {
	font-family: Verdana, Arial, Helvetica, sans-serif;
	font-size:12px;
}

  .button {
    color:#ffffff;
	font-size:14px;
	text-decoration:none; }   
  a.button:visited {
    color:#ffffff;
	text-decoration:none;
	font-size:14px;}   
  a.button:hover {
    color:#320D6D;
	text-decoration:none;
	text-decoration:underline;
	font-size:14px;}  

	 
</style>
</head>





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



<body bgcolor="#ffffff"><div align="center">
<table border="0" cellpadding="0" cellspacing="0" width="777">
<!-- fwtable fwsrc="番禺登录.png" fwbase="login.png" fwstyle="Dreamweaver" fwdocid = "1143676283" fwnested="0" -->
  <tr>
   <td><img src="images/spacer.gif" width="9" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="13" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="142" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="14" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="72" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="95" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="95" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="95" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="95" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="95" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="5" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="47" height="1" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="1" height="1" border="0" alt=""></td>
  </tr>

  <tr>
   <td colspan="12"><img name="login_r1_c1" src="images/login_r1_c1.png" width="777" height="79" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="1" height="79" border="0" alt=""></td>
  </tr>
  <tr>
   <td rowspan="14"><img name="login_r2_c1" src="images/login_r2_c1.png" width="9" height="383" border="0" alt=""></td>
   <td colspan="3" rowspan="2" bgcolor="#4CC5F7">
   
   <div align="center">
              <SCRIPT language=JavaScript>
<!-- Begin
function Year_Month(){ 
var now = new Date(); 
var yy = now.getYear(); 
var mm = now.getMonth()+1; 
var cl = '<font color="#000000">';
return(cl +yy + '年' + mm + '月</font>'); 
}function Date_of_Today(){ 
var now = new Date(); 
var cl = '<font color="#000000"><b>';
if (now.getDay() == 0) cl = '<font color="#FF0000"><b>'; 
if (now.getDay() == 6) cl = '<font color="#008000"><b>'; 
return(cl +now.getDate() + '</font></b>'); 
}function Day_of_Today(){ 
var day = new Array(); 
day[0] = "星期日"; 
day[1] = "星期一"; 
day[2] = "星期二"; 
day[3] = "星期三"; 
day[4] = "星期四"; 
day[5] = "星期五"; 
day[6] = "星期六"; 
var now = new Date(); 
var cl = '<font color="#000000">';
if (now.getDay() == 0) cl = '<font color="#FF0000">'; 
if (now.getDay() == 6) cl = '<font color="#008000">'; 

return(cl +day[now.getDay()] + '</font>'); 
}function CurentTime(){ 
var now = new Date(); 
var hh = now.getHours(); 
var mm = now.getMinutes(); 
var ss = now.getTime() % 60000; 
ss = (ss - (ss % 1000)) / 1000; 
var clock = hh+':'; 
if (mm < 10) clock += '0'; 
clock += mm+':'; 
if (ss < 10) clock += '0'; 
clock += ss; 
return(clock); 
}function refreshCalendarClock(){ 
document.all.calendarClock1.innerHTML = Year_Month(); 
document.all.calendarClock2.innerHTML = Date_of_Today(); 
document.all.calendarClock3.innerHTML = Day_of_Today(); 
document.all.calendarClock4.innerHTML = CurentTime(); 
}document.write('<table border="0" cellpadding="0" cellspacing="0" width="66" bgcolor="#C0C0C0" height="70">');
document.write('<tr><td valign="top" width="100%" height="100%">');
document.write('<table border="0" bordercolor="#000000" cellpadding="0" cellspacing="0" width="63" bgcolor="#FFFF99" height="67">');
document.write('<tr><td align="center" width="100%" height="100%" >');
document.write('<font id="calendarClock1" style="font-family:宋体;font-size:9pt;line-height:120%"> </font><br>');
document.write('<font id="calendarClock2" style="font-family:Arial;font-size:18pt;line-height:120%"> </font><br>');
document.write('<font id="calendarClock3" style="font-family:宋体;font-size:9pt;line-height:120%"> </font><br>');
document.write('<font id="calendarClock4" style="color:#000000;font-family:宋体;font-size:9pt;line-height:120%"><b> </b></font>');
document.write('</td></tr></table>');
document.write('</td></tr></table>');
refreshCalendarClock()
setInterval('refreshCalendarClock()',1000);
// End -->
            </SCRIPT>
      </div> 
   
   
   </td>
   <td colspan="8"><img name="login_r2_c5" src="images/login_r2_c5.png" width="599" height="113" border="0" alt=""></td>
   <td><img src="images/spacer.gif" width="1" height="113" border="0" alt=""></td>
  </tr>
  <tr>
   <td height="269" width="599" colspan="8" rowspan="13" background="images/bg21.png">
   <div align="center" style="padding-top:100px;margin-top:20px; ">
     <TABLE width="82%" align="center" class=list_table>
       <TBODY>
        <%if(currentUser.equals("guest") || currentUser.equals("admin_public")){%>
         <TR align="center">
           <TD style="font-size:13px;color:white;font-weight: bold;" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0024&DashboardType=0'"> 
             <a href="#"><img src="images/button1_r1_c1.png" width="75" height="75" border=0></a></A>  <BR>
             <a href="#" class="button">在线咨询</a></TD>
           <TD style="font-size:13px;color:white;font-weight: bold;" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0025&DashboardType=0'">
             <a href="#"><IMG src="images/button1_r1_c3.png" width="75" height="75"   border=0></a></A>  <BR>
             <a href="#" class="button">业务投诉 </a></TD>
           <TD style="font-size:13px;color:white;font-weight: bold;" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0003&DashboardType=0'" >
             <a href="#"><IMG src="images/button1_r11_c9.png" width="75" height="75"  border=0></a></A>  <BR>
             <a href="#" class="button">办事指南</A> </TD>
           <TD style="font-size:13px;color:white;font-weight: bold;" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0004&DashboardType=0'">
             <a href="#"><IMG src="images/button1_r11_c8.png" width="75" height="75"   border=0></a></A>  <BR>
             <a href="#" class="button">网上受理</A> </TD>
           <TD style="font-size:13px;color:white;font-weight: bold;" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0029&DashboardType=0'"> 
             <a href="#"><img src="images/button1_r1_c5.png" width="75" height="75" border="0"></a>  <BR>
             <a href="#" class="button">智能查询</A> </TD>
         </TR>
         
	      <%}else{%>
	 
	       <TR align="center">
           <TD style="font-size:13px;color:white;font-weight: bold;" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0036&DashboardType=0'"> 
             <a href="#"><img src="images/button1_r1_c1.png" width="75" height="75" border=0></a></A>  <BR>
             <a href="#" class="button">网上预约</a></TD>
             
           <!--<TD style="font-size:13px;color:white;font-weight: bold;" onclick="javascript:window.location='/stPortal/default.jsp?DashboardID=0039&DashboardType=0'">
             <a href="#"><IMG src="images/button1_r1_c3.png" width="75" height="75"   border=0></a></A>  <BR>
             <a href="#" class="button">业务投诉 </a></TD> 
           <TD style="font-size:13px;color:white;font-weight: bold;" onclick="javascript:window.location='/stPortal/default.jsp?DashboardID=0004&DashboardType=0'" >
             <a href="#"><IMG src="images/button1_r11_c9.png" width="75" height="75"  border=0></a></A>  <BR>
             <a href="#" class="button">办事指南</A> </TD>-->
             
           <TD style="font-size:13px;color:white;font-weight: bold;" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0004&DashboardType=0'">
             <a href="#"><IMG src="images/button1_r11_c8.png" width="75" height="75"   border=0></a></A>  <BR>
             <a href="#" class="button">网上受理</A> </TD>
			 
           <TD style="font-size:13px;color:white;font-weight: bold;" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0039&DashboardType=0'"> 
             <a href="#"><img src="images/button1_r1_c5.png" width="75" height="75" border="0"></a>  <BR>
             <a href="#" class="button">客户服务</A> </TD>
         </TR>
	      <%}%>
       </TBODY>
     </TABLE> 
     </div>
   </td>
   <td><img src="images/spacer.gif" width="1" height="18" border="0" alt=""></td>
  </tr><br>
  <%if(currentUser.equals("guest") || currentUser.equals("admin_public")){%>
		  <tr>
		   <td colspan="3" rowspan="12" valign="top" bgcolor="#4CC5F7"><table width="150" border="0" align="center" cellpadding="0" cellspacing="0">
		     <tr>
		       <td height="40" align="center" onClick="javascript:window.location='/stPortal/default.jsp?DashboardID=0015&DashboardType=0'"><a  href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image1','','images/butt_r1_c1.png',1)"><img src="images/button_r1_c1.png" name="Image1" width="142" height="22" border="0" alt=""></a></td>
     </tr>
     <tr>
       <td height="40" align="center" onClick="javascript:window.location='/stPortal/py/userLogin.jsp'">
	    <a  href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image2','','images/butt_r3_c1.png',1)">
        <img src="images/button_r3_c1.png" name="Image2" width="142" height="22" border="0" alt=""> </a>	   </td>
     </tr>
     <br><br>
     <!--<tr>
       <td height="40" align="center">
	    <a  href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image3','','images/butt_r5_c1.png',1)">
        <img src="images/button_r5_c1.png" name="Image3" width="142" height="22" border="0" alt=""> </a>	   
	   </td>
     </tr>
     <tr>
       <td height="40" align="center">
	   <a  href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image4','','images/butt_r8_c1.png',1)">
        <img src="images/button_r8_c1.png" name="Image4" width="142" height="22" border="0" alt=""> </a>		
		</td>
     </tr>-->
     <tr>
       <td height="40" align="center" onClick="javascript:window.location='/ApproveWeb/RegWeb/register.jsp?noPwd=1'">
	    <a  href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image5','','images/butt_r10_c1.png',1)">
        <img src="images/button_r10_c1.png" name="Image5" width="142" height="22" border="0" alt=""> </a>	   </td>
     </tr>
   </table></td>
   
   <%}else{%>
     <tr>
		   <td colspan="3" rowspan="12" valign="top" bgcolor="#4CC5F7"><table width="150" border="0" align="center" cellpadding="0" cellspacing="0">
     <tr>
       <td height="40" align="center" onClick="javascript:window.location='/stPortal/py/userLogin.jsp'">
	    <a  href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image2','','images/butt_r3_c1.png',1)">
        <img src="images/button_r3_c1.png" name="Image2" width="142" height="22" border="0" alt=""> </a>	   
	   </td>
     </tr>
     <br>
     <tr>
       <td height="40" align="center" onClick="javascript:window.location='/ApproveWeb/RegWeb/register.jsp?noPwd=1'">
	    <a  href="#" onMouseOut="MM_swapImgRestore()" onMouseOver="MM_swapImage('Image5','','images/butt_r10_c1.png',1)">
        <img src="images/button_r10_c1.png" name="Image5" width="142" height="22" border="0" alt=""> </a>	   
	   </td>
     </tr>
   </table></td>
   <%}%>
   
   <td><img src="images/spacer.gif" width="1" height="22" border="0" alt=""></td>
  </tr>
  <tr>
   <td><img src="images/spacer.gif" width="1" height="17" border="0" alt=""></td>
  </tr>
  <tr>
   <td><img src="images/spacer.gif" width="1" height="22" border="0" alt=""></td>
  </tr>
  <tr>
   <td><img src="images/spacer.gif" width="1" height="18" border="0" alt=""></td>
  </tr>
  <tr>
   <td><img src="images/spacer.gif" width="1" height="13" border="0" alt=""></td>
  </tr>
   <tr>
   <td><img src="images/spacer.gif" width="1" height="9" border="0" alt=""></td>
  </tr>
  <tr>
   <td><img src="images/spacer.gif" width="1" height="17" border="0" alt=""></td>
  </tr>
  <tr>
   <td><img src="images/spacer.gif" width="1" height="22" border="0" alt=""></td>
  </tr>
  <tr>
   <td><img src="images/spacer.gif" width="1" height="17" border="0" alt=""></td>
  </tr>
  <tr>
   <td><img src="images/spacer.gif" width="1" height="22" border="0" alt=""></td>
  </tr>
  <tr>
   <td><img src="images/spacer.gif" width="1" height="56" border="0" alt=""></td>
  </tr>
  <tr>
    <td><img src="images/spacer.gif" width="1" height="17" border="0" alt=""></td>
  </tr>  
</table>
</div>
<table border="0" cellpadding="0" cellspacing="0" width="100%">
<tr >
   <td align="center">
    <div align="center" style="font-size: 12px;color:">
	    (建议使用IE6.0以上、1024×768像素浏览)<br>
			审批事项上网热线：84690905 技术支持热线：84690905<br>
			粤ICP备05094920号 <br>
 		</div>
   </td>
  </tr>
</table>
</body>
</html>

<script>
  var w=window.screen.width-200;
	var h=window.screen.height-255;
	var sStyle="width="+w+"px,height="+h+"px,left=80px,top=80px,scrollbars=yes,resizable=yes";
	var sStyle="width="+w+"px,height="+h+"px,left=80px,top=80px,scrollbars=yes,resizable=yes";
	window.open("/STPublish/view/ViewRecord.jsp?id=2541","",sStyle);
</script>
 
 
 
 