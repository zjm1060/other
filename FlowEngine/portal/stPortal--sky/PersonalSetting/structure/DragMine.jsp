<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="jbportal.PortalUtil" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    String const_modu_public = "0";
    String const_modu_private = "1";
    String const_func_public = "0";
    String const_func_private = "1";

    String m_StrpNode = "";
    String ZoneTop = "4";
    String m_StrLeftBar = "";
    String m_modu_id = request.getParameter("modu_id");
    String m_modu_type = request.getParameter("modu_type");
    String s_SessionID = "";//Application("SessionID")

    String m_usercode=(String)request.getRemoteUser();
    String m_username =oManager.getCurrentUserName();

    int m_IfNew = oManager.sGetDBContext(m_usercode,m_modu_id,m_modu_type);
    String [] m_Str1 =PortalUtil.GetPositionTop(oManager,m_usercode,"4",m_StrpNode,m_modu_id,m_modu_type,s_SessionID);
    String m_StrTop   = m_Str1 [0];
    m_StrpNode = m_Str1 [1];

    String [] m_Str2 = PortalUtil.GetPositionTop(oManager,m_usercode,"3",m_StrpNode,m_modu_id,m_modu_type,s_SessionID);
        String m_StrMiddle= m_Str2 [0];
        m_StrpNode = m_StrpNode + m_Str2 [1];

    String [] m_Str3 = PortalUtil.GetPositionTop(oManager,m_usercode,"1",m_StrpNode,m_modu_id,m_modu_type,s_SessionID);
        String m_StrLeft  = m_Str3 [0];
        m_StrpNode = m_StrpNode + m_Str3 [1];

    String [] m_Str4 = PortalUtil.GetPositionTop(oManager,m_usercode,"2",m_StrpNode,m_modu_id,m_modu_type,s_SessionID);
        String m_StrRight = m_Str4 [0];
        m_StrpNode = m_StrpNode + m_Str4 [1];

    String [] m_Str5 = PortalUtil.GetPositionTop(oManager,m_usercode,"5",m_StrpNode,m_modu_id,m_modu_type,s_SessionID);
        String m_StrBottom = m_Str5 [0];
        m_StrpNode  = m_StrpNode + m_Str5 [1];

    m_StrLeftBar = PortalUtil.GetLeftBar(oManager,m_usercode,s_SessionID ,m_modu_id,m_modu_type);

%>

<link REL="stylesheet" HREF="../../include/drag/cust.css"></link>
<link REL="stylesheet" HREF="../../include/Public.css"></link>
<script language="JavaScript" src="../../include/drag/layout.js"></script>
<script language="JavaScript" src="../../include/drag/Drag.js"></script>

<script LANGUAGE="javascript">
<!--


	var oBuffer,oAllFunc;
	oBuffer = new dBuffer;
	oAllFunc = new cAllFunc;
	<%=m_StrLeftBar%>
	<%=m_StrpNode%>

	//当按取消的时候返回上一页。
	function Exit(){
      window.location.reload();
	}
	//进行存盘处理
	function Save()
	{
		//将oBuffer中的节点切换成相应的字串
		var l_s,oNode;
		var l_value,l_sql,l_where;
		var l_modu_id,l_func_type,l_func_id;
		l_value ="";
		l_sql ="";
		l_where="";

		for (l_s in oBuffer.nodes)
		{
			oNode = oBuffer.nodes[l_s];
			l_func_id = oNode.Func_id;

			l_func_type = l_func_id.substr(l_func_id.length-1,1);
			l_func_id = l_func_id.substr(0,l_func_id.length - 2);

			l_where = " where modu_id = '" + '<%=m_modu_id%>' + "'" ;
			l_where+= " and modu_type = " + '<%=m_modu_type%>' ;
			l_where+= " and func_id = '" + l_func_id + "'" ;
			l_where+= " and func_type = " + l_func_type  ;
			l_where+= " and user_code='" + '<%=m_usercode%>' + "'";

			//新增的处理
			switch (oNode.Status)
			{	case 'new':
					l_sql = " Insert Into user_prof " +
							" (user_code, modu_id, func_id, modu_type, func_type, IsIncluded, IsVisible, " +
							"HasFrame, AllowRemove, AllowMinimize, Zone, FrameState, HeightEnabled, Height, " +
							"HeightUnitType, WidthEnabled, Width, WidthUnitType, RequiresIsolation, " +
							"DashBoardOrder, RefreshInterval ) " ;

					l_sql += " (Select '" + '<%=m_usercode%>' + "','" +
							 '<%=m_modu_id%>' + "' , func_id," + '<%=m_modu_type%>' + "," + l_func_type + "," +
							 " IsIncluded, IsVisible, HasFrame, AllowRemove, AllowMinimize, " + oNode.Zone + ", FrameState, HeightEnabled, Height, " +
							 " HeightUnitType, WidthEnabled, Width, WidthUnitType, RequiresIsolation, " +
							 oNode.Position + ", RefreshInterval  "  ;

					if (l_func_type=='<%=const_func_private%>')
						{
							l_sql += " from  pers_func " +
									 " Where func_id = '" + l_func_id + "'and " +
									 "		 user_code = '" + '<%=m_usercode%>' + "');";
						}
					else
						{
							l_sql += " from  func_def " +
									 " Where func_id = '" + l_func_id + "');";
						}

						break;
				case 'delete':
						//如果是新增又是delete则不处理
						if ('<%=m_IfNew%>'!= '0')
						{
							//如果是新增又是show,则从modu_func添加
							l_sql = "Delete from user_prof " + l_where + ";";
						}
						else
							l_sql = "";
						break;
				case 'show':
						if ('<%=m_IfNew%>'=='0')
						{
							//如果是新增又是show,则从modu_func添加
							l_sql = " Insert Into user_prof " +
							" (user_code, modu_id, func_id, modu_type, func_type, IsIncluded, IsVisible, " +
							"HasFrame, AllowRemove, AllowMinimize, Zone, FrameState, HeightEnabled, Height, " +
							"HeightUnitType, WidthEnabled, Width, WidthUnitType, RequiresIsolation, " +
							"DashBoardOrder, RefreshInterval ) " ;

							l_sql += " (Select '" + '<%=m_usercode%>' + "','" +
							 '<%=m_modu_id%>' + "' , func_id," + '<%=m_modu_type%>' + "," + l_func_type + "," +
							 " IsIncluded, IsVisible, HasFrame, AllowRemove, AllowMinimize, " + oNode.Zone + ", FrameState, HeightEnabled, Height, " +
							 " HeightUnitType, WidthEnabled, Width, WidthUnitType, RequiresIsolation, " +
							 oNode.Position + ", RefreshInterval  "  ;

							l_sql += " from  modu_func " +
									 " Where func_id = '" + l_func_id + "'and " +
									 "modu_id = '" + '<%=m_modu_id%>' + "');";

						}
						else
						{
							l_sql = "Update user_prof set zone=" + oNode.Zone +
									",DashBoardOrder=" + oNode.Position + " " + l_where + ";";
						}
						break;
				default:
					break;
			}
			if ( l_sql != "" )
				l_value += l_sql;
		}
		saveToServer.partLayout.value = l_value;
		saveToServer.modu_id.value  = '<%=m_modu_id%>';
		saveToServer.modu_type.value  = '<%=m_modu_type%>';
		saveToServer.action = "_DragOperator.jsp";
		saveToServer.submit();
	}

	function window_onload()
	{
		var l_return;
		var l_selvalue;
		//建立下拉框显示模块
		l_return = GetModu();
		if (l_return!=""){
			document.all.item("LeftBarSelect").innerHTML=l_return;
			document.all.item("moduSelect").style.posWidth=160;
			l_return = moduSelect.options(moduSelect.selectedIndex).value;
			}
		else
			{
			  return;
			}

		//显示对应的各模块功能
		l_return = GetFunction(l_return);
		if (l_return!=""){
			document.all.item("LeftBar").innerHTML=l_return;
			//document.all.item("inFloder").innerHTML=l_return;
        }
	}
  window.attachEvent("onload",window_onload);
//-->
</script>

<body onmousedown="Document_OnMouseDown();" onload="CleanUpTables();">
	<div ID="divMouse" STYLE="display:none" onmousemove="DivMouse_OnMouseMove();" onmouseup="DivMouse_OnMouseUp();"></div>
	<table width="100%" height="100%" border="0" cellspacing="0" cellpadding="0" id="OuttermostContentBodyTable" >
		<tr>
			<td valign="Top" height=70% width="150" class="3DTD">
				<table border="0" width="100%" Class="LeftBarTable" height="100%">
					<tr><td id="LeftBarSelect" align="center"></td></tr>
					<!--
					<tr><td align="right"><img src="../../images/up100.gif" style="cursor:hand;" onclick="javaScript:miny();" WIDTH="16" HEIGHT="16"></td></tr>
					<tr><td align="right"><img src="../../images/down.gif" style="cursor:hand;" onclick="javaScript:addy();" WIDTH="16" HEIGHT="16"></td></tr>
                    -->
					<tr><td><div id="inFloder" style="overflow:auto;position:absolute;left:7;top:28;width:160px;height:280px;z-index:0;clip:rect(0 160 280 0);">
								<table BORDER="0" VALIGN="TOP" CLASS="LayoutFuncZones">
									<tr>
										<td VALIGN="TOP" id="LeftBar" class="LayoutLeftBar">
										</td>
									</tr>
								</table>
							</div>
						</td></tr>
					<tr><td height="100%"></td></tr>
					<tr><td bgcolor="#f0f0f0">删除区</td></tr>
					<tr><td class="LayoutDelZone" height="30" width="100%" id="Del">&nbsp;</td></tr>
				</table>

			</td>
		  <td>
		 	<table CLASS="LayoutZonesContainer" BORDER="0" CELLSPACING="0" CELLPADDING="0">
		 		<tr>
				<td>
					<table CLASS="LayoutZones" BORDER="0" CELLSPACING="4" CELLPADDING="2">
						<tr>
							<td CLASS="LayoutZoneTop" ID="tdZoneTop" COLSPAN="3">
								 <%=m_StrTop%>
								&nbsp;
							</td>
						</tr>
						<tr>
							<td CLASS="LayoutZoneLeft" ID="tdZoneLeft">
								<%=m_StrLeft%>
								&nbsp;
							</td>
							<td CLASS="LayoutZoneCenter" ID="tdZoneCenter">
								<%=m_StrMiddle%>
								&nbsp;
							</td>
							<td CLASS="LayoutZoneRight" ID="tdZoneRight">
								<%=m_StrRight%>
								&nbsp;
							</td>
						</tr>
						<tr>
							<td CLASS="LayoutZoneBottom" ID="tdZoneBottom" COLSPAN="3">
								<%=m_StrBottom%>
								&nbsp;
							</td>
						</tr>
					</table>
				</td>
				</tr>
			</table>
			<br />
		 </td>
		</tr>
		<tr>
			 <TD></TD>
			 <td class="CustButtonSectionRight" >
				<table>
					<tr>
					<td>
						<form METHOD="POST" ID="saveToServer" ACTION="layout.jsp">
							<input class="CustButton" TYPE="button" id="submit1" name="submit1" value="确    定" onclick="javascript:Save();">
							<input class="CustButton" TYPE="button" id="button1" name="button1" value="取    消" onclick="javascript:Exit();">
							<input TYPE="hidden" NAME="partLayout" ID="partLayout" VALUE />
							<input TYPE="hidden" NAME="DashboardID" />
							<input TYPE="hidden" NAME="modu_id" />
							<input TYPE="hidden" NAME="modu_type" />
						</form>
					</td>
					</tr>
				</table>
			</td>
		</tr>
	</table>

		<table CLASS="LayoutIBeam" ID="tblIBeam" CELLPADDING="0" CELLSPACING="0" BORDER="0" height=0>
			<tr CLASS="LayoutIBeamTR"><td></td></tr>
			<tr CLASS="LayoutIBeamTR">
				<td CLASS="LayoutIBeamTDTop"></td>
			</tr>
			<tr CLASS="LayoutIBeamTR">
				<td CLASS="LayoutIBeamTDBottom"></td>
			</tr>
			<tr CLASS="LayoutIBeamTR"><td></td></tr>
		</table>
</body>

