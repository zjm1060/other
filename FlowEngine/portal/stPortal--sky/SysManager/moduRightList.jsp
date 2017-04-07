<%@ page contentType="text/html; charset=gb2312" errorPage="error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,suntek.portal.publicLib.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    String m_StrList=null;
    RecordSetObject m_RsModu = null;
    String m_Title = null;
    RecordSetObject m_Rstemp = null;
    String m_ModuID = request.getParameter("modu_id");
    session.setAttribute("modu_id",m_ModuID);
    int usercount=0;

    //得到模块名称
    m_RsModu = oManager.rsGetModu(m_ModuID);
    if (m_RsModu.getRowCount()>0)
    {
      m_Title = jbportal.PortalUtil.getValue(m_RsModu.getString("Title",0));
    }
	m_Rstemp = oManager.getModuRightList(m_ModuID);
	m_StrList = "<Table width=100% Class=Table_List cellspacing=1>";
	m_StrList = m_StrList + "<Tr>";
	m_StrList = m_StrList + "<TD align=center class=3DTD " + " width=40% >用户名</TD>";
	m_StrList = m_StrList + "<TD align=center class=3DTD " + " width=60% >权限描述</TD>";
	m_StrList = m_StrList + "</Tr>";

	for(int index=0;index<m_Rstemp.getRowCount();index++){
          m_StrList = m_StrList +  "<tr>";
          String uname=jbportal.PortalUtil.getValue(m_Rstemp.getString("user_name",index));
          String ucode=jbportal.PortalUtil.getValue(m_Rstemp.getString("user_code",index));
          String utype=jbportal.PortalUtil.getValue(m_Rstemp.getString("user_type",index));
          m_StrList = m_StrList + "<TD align=left><input type=checkbox name=removeuc value=\""+ucode+"^"+utype+"\">" + uname + "</TD>";
          if (jbportal.PortalUtil.getValue(m_Rstemp.getString("deny_use",index)).equals("1"))
            m_StrList = m_StrList + "<TD align=center>" +"<input type=\"radio\" value=\"yes\" name=\"privilege"+usercount+"\">允许使用&nbsp;&nbsp;&nbsp;<input type=\"radio\" name=\"privilege"+usercount+"\" checked value=\"no\">禁止使用"+"</TD>";
          if (jbportal.PortalUtil.getValue(m_Rstemp.getString("allow_use",index)).equals("1"))
            m_StrList = m_StrList + "<TD align=center>" +"<input type=\"radio\" value=\"yes\" checked name=\"privilege"+usercount+"\">允许使用&nbsp;&nbsp;&nbsp;<input type=\"radio\" name=\"privilege"+usercount+"\" value=\"no\">禁止使用"+"</TD>";
          m_StrList =m_StrList + "<input type=hidden name=user_code"+usercount+" value=\""+jbportal.PortalUtil.getValue(m_Rstemp.getString("user_code",index))+"\">";
          m_StrList =m_StrList + "<input type=hidden name=user_type"+usercount+" value=\""+jbportal.PortalUtil.getValue(m_Rstemp.getString("user_type",index))+"\">";
          m_StrList = m_StrList + "</TR>";
          usercount++;
	}

	m_StrList = m_StrList + "</TABLE>";

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
function AddClick(){
  returnvalue=window.showModalDialog("AddUserRight.jsp?addtype=modu","","dialogHeight:400px;dialogWidth=677px;center:yes;edge:raised;scroll:no;status:yes;unadorned:yes;");
  if (returnvalue==1)
    window.location="moduRightList.jsp?modu_id=<%=m_ModuID%>";
}
function OkClick(){
  ModuRightList.operator.value="ok";
}
function RemoveClick(){
  ModuRightList.operator.value="remove";
}
//窗体load时的事件
function window_onload()
{
	var sArray,sModuID;
}
window.attachEvent("onload",window_onload);
//-->
</script>

<body leftmargin="0" topmargin="0" rightmargin="0">
	<form ACTION="_ModuRightOperator.jsp" METHOD="post" id="ModuRightList" name="ModuRightList">
		<table width="102%" cellspacing="0" cellpadding="0" border="0">
			<tr>

				<td class="TdHeader"><%=m_Title%></td>
			</tr>
		</table>
		<table width="102%" height="80%" valign="top" align="center" border="0">
		<tr><td>
			<table Class="Table_Normal" align="center" width="100%" height="100%" cellspacing="0">
				<tr class="TrHeader">
					<td ><%=m_Title%>-权限列表</td>
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
						<%=m_StrList%>
					</div>
				</td></tr>
			</table>
		</tr></td>
		</table>
		<%
		//保存隐藏变量
		%>
		<input type="hidden" name="modu_id" value="<%=m_ModuID%>">
		<input type="hidden" name="usercount" value="<%=usercount%>">
                <input type="hidden" name="operator" value="">
                <table width="100%">
                  <tr>
                    <td width="100%" align=right>
                        <input type="button" id="add" name="add" value="添加权限" class="Input_Button" onclick="javascript:AddClick();">
                    	<input type="submit" id="add" name="remove" value="删除选中" class="Input_Button" onclick="javascript:RemoveClick();">
                    	<input type="submit" id="ok" name="ok" value="保存修改" class="Input_Button" onclick="javascript:OkClick();">
                    </td>
                  </tr>
                </table>
	</form>
</body>
</html>

