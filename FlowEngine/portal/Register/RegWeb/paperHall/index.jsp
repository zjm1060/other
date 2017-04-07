<%@ page contentType="text/html; charset=GBK" %>
<%@ page import="java.util.Hashtable,java.util.Enumeration"%>
<%@ page import="com.suntek.register.outer.Optional"%>
<%
  String curUser= request.getRemoteUser();
  Optional opt = new Optional();
  String regionID=opt.getRegID();
  String flowPath=opt.getFlowPath();
  String FlowDirID = request.getParameter("FlowDirID");
  String FlowDirName =request.getParameter("FlowDirName");
%>
<html>
<head>
 <title>市行政服务在线</title>
 <link href="Css/Public.css" type="text/css" rel="stylesheet">
 <meta http-equiv="Content-Type" content="text/html;charset=GBK">

<script language="JavaScript" type="text/JavaScript">

function createFlowinst(sRecordID){
  <%if(curUser == null){%>
     alert("请登录进行网上申请");
  <%}else{%>
      var w=window.screen.width-10;
      var h=window.screen.height-55;
      var sStyle="width="+w+"px,height="+h+"px,left=0px,top=0px,scrollbars=yes,resizable=yes";
      window.open("<%=flowPath%>/workflow/FlowFrame.jsp?action=0&flowInid="+sRecordID,"",sStyle);

  <%}%>
}
//根据流程目录ID来列出流程
function OpenFlow(DirID,DirName){
    //ClearFormValue();
    var form = document.frmPost;
    var obj = form.FlowDirID;
    obj.value = DirID;
    var obj = form.FlowDirName;
    obj.value = DirName;
    form.submit();
}
</script>

</head>
<body>
  <table cellspacing="0" width="100%" border="0" cellpadding="0">
     <tr>
       <td>
	 <div align="center">
	   <table width="778" border="0" cellspacing="0" cellpadding="0">
            <tr>
		<td><img src="Images/long2.gif" width="125" height="100"></td>
		<td>
                  <object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" codebase="http://download.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,29,0" width="550" height="80">
		    <param name="movie" value="banner.swf">
		    <param name="quality" value="high">
		  </object>
		</td>
		<td><img src="Images/china2.gif" width="95" height="80"></td>
	    </tr>
	   </table>
	 </div>
       </td>
     </tr>
  </table>
  <table cellspacing="0" width="100%" cellpadding="0" border="0">
    <tr>
      <td valign="top">
        <div align="center">
	  <table width="778" border="0" cellspacing="0" cellpadding="0" valign="bottom">
	   <tr>
	      <td width="60">
		<img src="Images/menu_lef.gif" width="60" height="32">
	      </td>
	      <td background="Images/menu_bg.gif">
	        <table width="96%" border="0" align="center" cellpadding="0" cellspacing="0">
 	          <tr>
		    <td width="*" align="center">
		       <a href="#" title="首页"><img src="Images/shouye00.gif" width="72" height="32" border="0"></a>
		    </td>
      		    <td width="0%" valign="top"><img src="Images/menu_lin.gif" width="1" height="32"></td>
                    <td width="*" align="center">
		      <a href="#" title="办事大厅"><img src="Images/banshida.gif" border="0"></a>
                    </td>
                    <td width="0%" valign="top"><img src="Images/menu_lio.gif" width="1" height="32"></td>
                    <td width="*" align="center">
                       <a href="#" title="公告大厅"><img src="Images/gonggaod.gif" border="0"></a>
                    </td>
                    <td width="0%" valign="top"><img src="Images/menu_lio.gif" width="1" height="32"></td>
                    <td width="*" align="center">
                	<a href="#" title="监督大厅"><img src="Images/jianduda.gif" border="0"></a>
                    </td>
                    <td width="0%" valign="top"><img src="Images/menu_lio.gif" width="1" height="32"></td>
                    <td width="*" align="center">
			<a href="#" title="咨询与服务"><img src="Images/zixunyuf.gif" border="0"></a>
                    </td>
                    <td width="0%" valign="top"><img src="Images/menu_lio.gif" width="1" height="32"></td>
                    <td width="*" align="center">
			<a href="#" title="使用指南"><img src="Images/shiyongz.gif" border="0"></a>
                    </td>
		  </tr>
	        </table>
              </td>
	      <td width="59" align="right">
		<img src="Images/menu_rig.gif" width="59" height="32">
	      </td>
	   </tr>
	  </table>

	  <table align="center" cellpadding="0" cellspacing="0" width="778" height="250" border="0">
           <tr>
              <td valign="top" bgcolor="#DBEBFB" width="100%">

<form action="index.jsp" method="post" name="frmPost">
<table width="778" border="0" cellpadding="0" cellspacing="0" align="center">
   <tr valign="top">

      <td width="182">
          <table width="100%" border="0" cellspacing="0" cellpadding="0">
            <tr height="45" valign="middle">
               <td background='Images/jiaose_1.gif' align="center"><b>单&nbsp;位&nbsp;名&nbsp;称</b></td>
             </tr>
             <%
		Hashtable hshDir = opt.getFlowDir(regionID);
		Enumeration enum = hshDir.keys();
		String name="";
		String name2="";
		String value="";
		String value2="";
                while(enum.hasMoreElements()){
		  name=(String)enum.nextElement();
                  value=(String)hshDir.get(name);
		  name2=(String)enum.nextElement();
                  value2=(String)hshDir.get(name2);
             %>
              <tr>
               <td background="Images/jiaose_2.gif">
		  <table border="0" cellspacing="0" cellpadding="0" width="100%">
                    <tr height="26">
                      <td nowrap >
			 <a href="javascript:OpenFlow('<%=name%>','<%=value%>');">
                           <img src='Images/point800.gif' width="3" height="3" border="0">
                           <%=value%>
                         </a>
                      </td>
                      <td nowrap>
			 <a href="javascript:OpenFlow('<%=name2%>','<%=value2%>');" >
                           <img src='Images/point800.gif' width="3" height="3" border="0">
                           <%=value2%>
                         </a>
                      </td>
                     </tr>
		  </table>
               </td>
             </tr>
             <%}%>
             <tr height="5">
               <td background='Images/jiaose_13.gif'></td>
              </tr>
          </table>
	</td>

	<td>
           <table width="98%" border="0" cellpadding="0" cellspacing="0" align="center">
              <tr>
                 <td height="30" background='Images/jiaose_8.gif' valign="bottom">
                     <font color="#045FB0">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;您现在的位置：办事大厅&nbsp;&gt;&gt;&nbsp;所有部门&nbsp;&gt;&gt;</font>
                  </td>
              </tr>
            </table>

            <table border="0" cellspacing="0" cellpadding="0" height="5">
		<tr><td></td></tr>
            </table>

            <table width="98%" border="0" cellpadding="0" cellspacing="0" align="center">
		<tr>
                  <td>
                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;请输入查询的关键字
                    <input type="text" name="queryValue" value="" size="27" class="input1" onkeypress="javascript:if(event.keyCode==13) return doQuery_P_fb13eb9f26_10000();">
                    <select name="queryCondition" style="width:120px">
                        <option value=""></option>
                        <option value="1">审批事项编号</option>
                        <option value="2">审批事项名称</option>
                        <option value="3">审批部门</option>
                    </select>
                     <INPUT type="image" src='Images/search20.gif' width="73" height="21" border="0" name="eventSubmit_doQuery" id="doQuery" align="absmiddle" onclick="javascript:return doQuery_P_fb13eb9f26_10000();">
                   </td>
                   <td width="20"></td>
		</tr>
            </table>

            <table border="0" cellspacing="0" cellpadding="0" height="10">
		<tr><td></td></tr>
            </table>

            <table width="98%" border="0" cellpadding="1" cellspacing="1" align="center" bgcolor="#ADADAD">
                <tr height="26" bgcolor="#7CC2F1" style="font-size:9pt;font-family:宋体;color:#FFFFFF">
                    <td width="15%" align=center>审批事项编号</td>
                    <td width="51%" align=center>审批事项名称</td>
                    <td width="24%" align=center>受理部门</td>
                    <td width="10%" align=center>网上办理</td>
                </tr>
		<%
                  if( FlowDirID!=null){
                   Hashtable hshFlow =opt.getFlowbyDir(regionID,FlowDirID);
                   Enumeration enumFlow = hshFlow.keys();
		   String nameFlow="";
                   String valueFlow="";
                   while(enumFlow.hasMoreElements()){
		     nameFlow=(String)enumFlow.nextElement();
		     valueFlow =(String)hshFlow.get(nameFlow);
		%>
               <tr height="26" bgcolor="#DDEBF9" style="font-size:9pt;font-family:宋体;color:#000000">
		   <td align="center"><%=nameFlow%></td>
		   <td align="center"><%=valueFlow%></td>
		   <td align="center"><%=FlowDirName%></td>
		   <td align="center">
		     <a href="#" onclick="javascript:createFlowinst(<%=nameFlow%>);">
                        <img src='Images/icon6000.gif' width="16" height="15" border="0">
                      </a>

	            </td>
		</tr>
		<%
                   }
		 }
		%>
            </table>
        </td>
   </tr>

</table>
<input type="hidden" name="FlowDirID" value="">
<input type="hidden" name="FlowDirName" value="">
</form>

</td>
</tr>
</table>

</div>
</td>
</tr>
</table>

</body>
</html>

