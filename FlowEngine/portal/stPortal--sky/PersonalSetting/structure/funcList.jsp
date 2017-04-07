<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,suntek.portal.publicLib.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
      String m_Title = "";
      RecordSetObject m_Rstemp = null;
      String m_StrList = "";
      String ms_ThisURL = "";
      String ms_BackURL = "";
	  String m_ModuID = request.getParameter("modu_id");
      String m_modu_type = request.getParameter("modu_type");
      //得到模块名称
      m_Rstemp = oManager.rsGetModu(m_ModuID);
      if (m_Rstemp.getRowCount()>0)
      {
        m_Title =jbportal.PortalUtil.getValue(m_Rstemp.getString("Title",0));
      }
      String m_usercode=(String)request.getRemoteUser();
      String m_username =oManager.getCurrentUserName();

	  String m_Session_id = "";//Application("SessionID")
	  m_Rstemp = oManager.rsGetPersonFuncList (m_usercode,m_ModuID,m_modu_type,m_Session_id);
      m_StrList = "<Table width=100% Class=Table_List cellspacing=1>";
      m_StrList = m_StrList + "<Tr>";
      m_StrList = m_StrList + "<TD class=3DTD " + " width=20% >功能名称</TD>";
      m_StrList = m_StrList + "<TD class=3DTD " + " width=30% >功能描述</TD>";
      m_StrList = m_StrList + "<TD class=3DTD " + " width=20% >所在区域</TD>";
      m_StrList = m_StrList + "<TD class=3DTD " + " width=30% >链接</TD>";
      m_StrList = m_StrList + "</tr>";
      for(int index=0;index<m_Rstemp.getRowCount();index++){
        m_StrList = m_StrList + "<tr ID=" + jbportal.PortalUtil.getValue(m_Rstemp.getString("func_id",index) + jbportal.PortalUtil.getValueN(m_Rstemp.getString("func_type",index))) + " class=Grid_TR_Normal onmousemove=javascript:mousemove(this); " + " onmouseout=javascript:mouseout(this); " + " onclick=javascript:onClick(this);>";
        int zone = (int)m_Rstemp.getInt("zone",index);
        if (zone==0){zone = 3;}
        m_StrList = m_StrList + "<TD >" + jbportal.PortalUtil.getValue(m_Rstemp.getString("Title",index)) + "</TD>";
        m_StrList = m_StrList + "<TD >" + jbportal.PortalUtil.getValue(m_Rstemp.getString("Description",index)) + "</TD>";
        m_StrList = m_StrList + "<TD >" + jbportal.PortalUtil.getZoneName(zone) + "</TD>";
        m_StrList = m_StrList + "<TD >" + jbportal.PortalUtil.getValue(m_Rstemp.getString("href",index)) + "</TD>";
        m_StrList = m_StrList + "</TR>";
      }
      m_StrList = m_StrList + "</TABLE>";

	  //进行页面的返还处理
	  ms_ThisURL = "funcList.jsp?modu_id=" + m_ModuID + "&modu_type=" + m_modu_type + "&Title=" + m_Title;
	  if (ms_BackURL.equals(""))
		ms_BackURL = "funcList.jsp?modu_id=" + m_ModuID + "&modu_type=" + m_modu_type + "&Title=" + m_Title;

%>

<html>
<link rel="STYLESHEET" type="text/css" href="../../include/Public.css">
<script language="javascript" src="../../include/i_ExpandJScript.js"></script>
<script LANGUAGE="javascript">
<!--
var StrCurrRow;

function GetHelp(){
	alert("抱歉，暂缺!");
}

//窗体load时的事件
function window_onload()
{
	var sArray,sModuID;

}

//当单击某行时的处理
function onClick(item)
{
	//m_NodeVale --func_id
	//m_modu_id -- 模块Id
	//得到func的id 和 func的类型
	var l_func_type,l_func_id;
	if (item==null) {return;}

	l_func_id=item.id;
        //alert(l_func_id);
	l_func_type = l_func_id.substr(l_func_id.length-1,1);
	l_func_id = l_func_id.substr(0,l_func_id.length - 1);
        //alert(l_func_id);
	window.location = "modufunc_modify.jsp?func_id=" + l_func_id +
					  "&func_type=" + l_func_type +
					  "&modu_id=" + '<%=m_ModuID%>' +
					  "&modu_type="+ '<%=m_modu_type%>'+
					  "&title=" + '<%=m_Title%>';
}

//鼠标移动到当前行时的处理
function mousemove(item)
{
	var l_className,l_s;

	//l_className=item.className;

	//if (l_className==null) {return;}

	//l_className=l_className.toLowerCase();

	//if (StrCurrRow!=null)
	//	{
	//		document.all.item(StrCurrRow).className="grid_tr_normal";
	//	}

	item.className="grid_tr_active";

	//StrCurrRow=item.id;
}

function mouseout(item)
{
	item.className="grid_tr_normal";
}

window.attachEvent("onload",window_onload);
//-->
</script>

<body leftmargin="0" topmargin="0" rightmargin="0">
	<form ACTION="ModuleDetails.jsp" METHOD="post" id="ModuleDetails" name="ModuleDetails">

		<table width="100%" cellspacing="0" cellpadding="0" border="0">
			<tr>

				<td class="TdHeader"><%=m_Title%></td>
			</tr>
		</table>
		<table width=100% height=90% valign="top" align="center" border="0">
		<TR><TD>
			<table Class="Table_Normal" align="center" width=100% height=100% cellspacing=0>
				<tr class="TrHeader">
					<td ><%=m_Title%>-功能列表</td>
					<td align="right">
						<table width="30" cellpadding="0" cellspacing="0" border="0">
							<tr><td align="right">
									<img src="../../images/help.gif" align="center" style="cursor=hand;" onclick="javascript:GetHelp();" alt="帮助" WIDTH="16" HEIGHT="14">
								</td>
								<td align="right">
									<img src="../../images/close_no.gif" align="center" alt="退出" WIDTH="16" HEIGHT="14">
								</td>
							</tr>
						</table>
					</td>
				</tr>
				<Tr ><TD colspan="2" valign="top" >
					<div style="width:100%;height:100%;overflow-x:auto;overflow-y:auto;background-color:white">
						<%=m_StrList%>
					</div>
				</TD></tr>
			</table>
		</TR></TD>
		</TABLE>

		<input type="hidden" name="modu_id" value="<%=m_ModuID%>">
		<input type="hidden" name="para_id" value="">
		<input type="hidden" name="modu_type" value="<%=m_modu_type%>">
	</form>
</body>
</html>
