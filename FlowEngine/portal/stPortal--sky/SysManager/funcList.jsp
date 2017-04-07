<%@ page contentType="text/html; charset=gb2312" errorPage="error.jsp"%>
<%@ page language="java" import="java.sql.*,suntek.portal.publicLib.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    String m_StrList=null;
    RecordSetObject m_RsModu = null;
    String m_Title = null;
    RecordSetObject m_Rstemp = null;
	String m_ModuID = request.getParameter("modu_id");

	//得到模块名称
    m_RsModu = oManager.rsGetModu(m_ModuID);
    if (m_RsModu.getRowCount()>0)
    {
      m_Title = jbportal.PortalUtil.getValue(m_RsModu.getString("Title",0));
    }
	m_Rstemp = oManager.rsGetFuncList(m_ModuID);
	//进行页面的返还处理
	//Dim ms_BackURL,ms_ThisURL
	String ms_ThisURL = "funcList.jsp?modu_id=" + m_ModuID;
	String ms_BackURL = "funcList.jsp?modu_id=" + m_ModuID;



%>

<html>
<link rel="STYLESHEET" type="text/css" href="../include/Public.css">
<script language="javascript" src="../include/i_ExpandJScript.js"></script>
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

	window.location = "modufunc_modify.jsp?m_NodeValue=" + item.id +
					  "&m_modu_id=" + '<%=m_ModuID%>';
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
		<table width="100%" height="90%" valign="top" align="center" border="0">
		<tr><td>
			<table Class="Table_Normal" align="center" width="100%" height="100%" cellspacing="0">
				<tr class="TrHeader">
					<td ><%=m_Title%>-功能列表</td>
					<td align="right">
						<table width="30" cellpadding="0" cellspacing="0" border="0">
						<tr><td align="right">
						<img src="../images/help.gif" align="center" style="cursor=hand;" onclick="javascript:GetHelp();" alt="帮助" WIDTH="16" HEIGHT="14"></td>
						<td align="right">
						<img src="../images/close_no.gif" align="center" alt="退出" WIDTH="16" HEIGHT="14"></td>
						</tr>
				</table>
					</td>
				</tr>
				<tr><td colspan="2" valign="top">
					<div style="width:100%;height:100%;overflow-x:auto;overflow-y:auto;background-color:white">
					<Table width=100% Class=Table_List cellspacing=1>
                    <%
						out.println("<Tr>");
						out.println("<TD class=3DTD " + " width=20% >功能名称</TD>");
						out.println("<TD class=3DTD " + " width=30% >功能描述</TD>");
						out.println("<TD class=3DTD " + " width=20% >所在区域</TD>");
						out.println("<TD class=3DTD " + " width=30% >链接</TD>");
						out.println("</Tr>");

						for(int index=0;index<m_Rstemp.getRowCount();index++){
       						out.println("<tr ID=" + jbportal.PortalUtil.getValue(m_Rstemp.getString("func_id",index)) + " class=Grid_TR_Normal onmousemove=javascript:mousemove(this); " + " onmouseout=javascript:mouseout(this); " + " onclick=javascript:onClick(this);>");
							out.println("<TD >" + jbportal.PortalUtil.getValue(m_Rstemp.getString("Title",index)) + "</TD>");
							out.println("<TD >" + jbportal.PortalUtil.getValue(m_Rstemp.getString("Description",index)) + "</TD>");
							out.println("<TD >" + jbportal.PortalUtil.getZoneName(Integer.parseInt(m_Rstemp.getString("zone",index))) + "</TD>");
							out.println("<TD >" + jbportal.PortalUtil.getValue(m_Rstemp.getString("href",index)) + "</TD>");
							out.println("</TR>");
						}

					%>
					</TABLE>
					</div>
				</td></tr>
			</table>
		</tr></td>
		</table>
		<%
		//保存隐藏变量
		%>
		<input type="hidden" name="modu_id" value="<%=m_ModuID%>">
		<input type="hidden" name="para_id" value="">
	</form>
</body>
</html>

