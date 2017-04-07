<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,com.minstone.util.database.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    //add by hhy
    String const_modu_public = "0";
    String const_modu_private = "1";
    String const_func_public = "0";
    String const_func_private = "1";

    String m_ModuID = "";
    String m_moduType = "-1";
    String m_Title = "";
    String m_Description = "";
    String m_DashBoardOrder = "";
    String m_use_flag = "";
    String m_StrUse_Flag = "";
    String m_typename = "";
    String m_para_id = "";

    RecordSetObject m_Rstemp=null;
    //add end
		m_ModuID = jbportal.PortalUtil.getValue(request.getParameter("m_moduid"));
		if ((request.getParameter("modu_type")!=null)&&(request.getParameter("modu_type").equals("undefined")==false))
			m_moduType=jbportal.PortalUtil.getValue(request.getParameter("modu_type"));


    String m_usercode=(String)request.getRemoteUser();
    String m_username =oManager.getCurrentUserName();
    m_Rstemp = oManager.rsGetUserModu(m_usercode,m_ModuID,m_moduType);
	if (!(m_Rstemp.getRowCount()>0)){
		m_ModuID="";
    }
	else{
		m_Title		= jbportal.PortalUtil.getValue(m_Rstemp.getString("title",0));
		m_Description	= jbportal.PortalUtil.getValue(m_Rstemp.getString("Description",0));
		m_DashBoardOrder	= jbportal.PortalUtil.getValue(m_Rstemp.getString("sort_id",0));
		m_use_flag	= jbportal.PortalUtil.getValueN(m_Rstemp.getString("use_flag",0));
	}
    if (!m_moduType.equals(const_modu_public)){
		if (m_use_flag.equals("0")){
			m_StrUse_Flag = "<INPUT TYPE=CheckBox NAME=use_flag CLASS=CustCheckbox VALUE=1>";
			m_StrUse_Flag = m_StrUse_Flag + "是否允许使用</INPUT>";
    	}
		else{
			m_StrUse_Flag = "<INPUT TYPE=CheckBox NAME=use_flag CLASS=CustCheckbox VALUE=1 CHECKED=>";
			m_StrUse_Flag = m_StrUse_Flag + "是否使用</INPUT>";
	    }
    }
	if (m_moduType.equals(const_modu_public))
		m_typename = "公用模块";
	else
		m_typename = "私人模块";
%>
<html>
<link rel="STYLESHEET" type="text/css" href="../../include/Public.css">

<script type="text/javascript" src="../../include/i_CheckInput.js"></script>
<script type="text/javascript" src="../../include/i_ExpandJScript.js"></script>
<script type="text/javascript" src="../../include/i_LockControls.js"></script>

<script type="text/javascript">
<!--
function Save(){
	//存盘前要验证输入是否正确,如果检验不功过则返回
	if (ValidateInput()==false)
		return;
	ModuleDetails.action = "_ModuleDetailsOperator.jsp";
	ModuleDetails.submit();
}

function GetHelp(){
	alert("抱歉，暂缺!");
}

//检验输入是否合法
function ValidateInput()
{
	var l_strErrDesc;
	l_strErrDesc = Validate_Input();
	if ( l_strErrDesc == ""){
		return true;
	}
	alert(l_strErrDesc);
	return false;
}

//窗体load时的事件
function window_onload()
{
	var sArray,sModuID;

	sModuID='<%=m_ModuID%>';
	if (parent!=null&&parent.menu!=null&&sModuID!="")
		{
		  sArray = new Array();
		  sArray["cate_desc"]= '<%=m_Title%>';
		  sArray['cate_value']= sModuID
		  sArray['cate_para']= '<%=m_para_id%>';
		  sArray['cate_type']= '<%=m_moduType%>';

		  parent.menu.oTree.UpdateEx(sModuID,'<%=m_para_id%>',sArray);
		}
	if (sModuID=="")
		{
			LockControls();
		}
}

window.attachEvent("onload",window_onload);
//-->
</script>

<body leftmargin="0" topmargin="0" rightmargin="0">
	<form ACTION="ModuleDetails.jsp" METHOD="post" id="ModuleDetails" name="ModuleDetails">

		<table width="100%" cellspacing="0" cellpadding="0">
			<tr>

				<td class="TdHeader"><%=m_Title%></td>
			</tr>
		</table>
		<table height=100% valign="middle" align="center">
		<TR><TD>
			<table width="450" Class="Table_Normal" align="center" cellspacing="0" cellpadding="0">
			  <tr class="TrHeader">
			    <td colspan="3">模块管理--<%=m_typename%></td>
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
			  <tr>
			    <td colspan="4">&nbsp;</td>
			  </tr>
			  <tr>
			    <td width="17%">&nbsp;模块名称：</td>
			    <td width="33%">
					<input name="title" class="input_readonly" value="<%=m_Title%>" autocheck="1" maxlen="30" propname="模块名称" cannull="0">
				</td>
			    <td width="17%">&nbsp;显示顺序：</td>
			    <td width="33%"><input name="sort_id" class="input_ReadOnly" Readonly value="<%=m_DashBoardOrder%>" autocheck="1" propname="显示顺序的输入" datatype="1"></td>
			  </tr>
			  <tr>

			    <td width="16%">&nbsp;模块描述：</td>
			    <td colspan="3"><input name="description" class="input_Readonly" value="<%=m_Description%>" style="width:356;"></td>
			  </tr>
			  <tr><td colspan="4">&nbsp;<%=m_StrUse_Flag%></td></tr>
			  <tr><td colspan="4">&nbsp;</td></tr>
			  <tr>
					<td colspan="4" align="right">
						<input type="button" id="ok" name="ok" value="确    定" class="Input_Button" onclick="javascript:Save();">&nbsp;
					</td>
			  </tr>
			  <tr><td colspan="4">&nbsp;</td></tr>
			</table>
		</TR></TD>
	</table>

		<input type="hidden" name="modu_id" value="<%=m_ModuID%>">
		<input type="hidden" name="modu_type" value="<%=m_moduType%>">
	</form>
</body>
</html>
