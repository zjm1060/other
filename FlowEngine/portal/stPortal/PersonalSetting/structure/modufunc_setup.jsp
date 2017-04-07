<!--#include file="../../include/Header.jsp"-->
<!--#include file="../../include/CommonUtil.jsp"-->
<!--#include file="../../include/Back_lib.jsp"-->
<!--#include file="../../include/VBFunc.vbs"-->
<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page import="java.util.*"%>
<%@ page import="java.sql.*,jbportal.PortalUtil"%>
<%@ page import="com.minstone.util.database.RecordSetObject"%>

<jsp:useBean id="oSys" scope="session" class="jbportal.classServiceBean" />
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />

<%
    response.setHeader("Cache-Control","no-cache");
    String m_Title = "";
    String m_href = "";
    String m_Description = "";
    String m_href_desc = "";
    String m_AllowMinimize = "";
    String m_Width = "";
    String m_HeightUnitType = "";
    String m_HeightEnabled = "";
    String m_IsVisible = "";
    String m_AllowRemove = "";
    String m_IsIncluded = "";
    String m_WidthEnabled = "";
    String m_WidthUnitType = "";
    String m_Height = "";
    String m_HasFrame = "";
    String m_FrameState = "";
    String m_StrFrameState="";
    String m_RequiresIsolation = "";
    String sCheckedNo = "";
    String sCheckedYes = "";
    RecordSetObject m_Rstemp = null;
    String m_StrZone1 ="";
    String m_StrZone2 ="";
    String m_StrZone3 ="";
    String sOnClick = "";
    String m_StrHeightEnabled1 = "";
    String m_StrHeightEnabled2 ="";
    String m_StrHeightUnitType="";
    String m_StrWidthEnabled1 = "";
    String m_StrWidthEnabled2 ="";
    String m_StrWidthUnitType ="";
    //显示位置定义
    String m_aryDis1[] = {"上方区域", "左方区域"};
    String m_aryDis2[] = {"中间区域","右方区域"};
    String m_aryDis3[] = {"下方区域"};
    String m_aryVal1[] = {"4","1"};
    String m_aryVal2[] = {"3","2"};
    String m_aryVal3[] = {"5"};
    String m_aryImg1[] = {"../../images/topbanner.gif", "../../images/leftcolumn.gif"};
    String m_aryImg2[] = {"../../images/centercolumn.gif", "../../images/rightcolumn.gif"};
    String m_aryImg3[] = {"../../images/bottombanner.gif"};

    //显示状态定义

    String m_aryFrameState1[] = {"展开状态","缩起状态"};
    String m_aryFrameState2[] = {"0","1"};
    String m_aryFrameState3[] = {"../../images/expanded.gif", "../../images/minimized.gif"};

    //定义Select项的内容

    String m_UnitType[] = {"厘米","英寸","毫米","像素","点","百分比"};
    String m_UnitTypeValue[] = {"cm","in","mm","px","pt","%"};
    //------------------
    String m_usercode=(String)request.getRemoteUser();
    String m_username =oManager.getCurrentUserName();
    //得到传递进来的参数
    String m_Command=PortalUtil.getValue(request.getParameter("cmd"));
    String m_ModuTitle = PortalUtil.getValue(request.getParameter("title"));
    String m_func_type = PortalUtil.getValueN(request.getParameter("func_type"));
    String m_func_id =PortalUtil.getValue(request.getParameter("func_id"));
    String m_modu_type =PortalUtil.getValueN(request.getParameter("modu_type"));
    String m_modu_id =PortalUtil.getValue(request.getParameter("modu_id"));
    String HeightUnitType =PortalUtil.getValueN(request.getParameter("HeightUnitType"));
    String WidthUnitType =PortalUtil.getValueN(request.getParameter("WidthUnitType"));
    String m_Zone =PortalUtil.getValueN(request.getParameter("Zone"));
    String m_Source="stportal";
    //如果是更新。
    if(m_Command.equals("update")){
      Hashtable dct_Para = new Hashtable();
      Hashtable dct_Key = new Hashtable();

      dct_Key.put("func_id",m_func_id);
      dct_Key.put("modu_id",m_modu_id);
      dct_Key.put("func_type",m_func_type);
      dct_Key.put("modu_type",m_modu_type);
      dct_Key.put("user_code",m_usercode);
      if (HeightUnitType != null && HeightUnitType.equals("0")){
      	HeightUnitType = "%";
      }
      if (WidthUnitType != null && WidthUnitType.equals("0")){
      	WidthUnitType = "%";
      }
      dct_Para.put("AllowMinimize",PortalUtil.toDouble(request.getParameter("AllowMinimize")));
      dct_Para.put("Width",PortalUtil.toDouble(request.getParameter("Width")));
      dct_Para.put("Zone",PortalUtil.toDouble(m_Zone));
      dct_Para.put("HeightUnitType",HeightUnitType);
      dct_Para.put("HeightEnabled",PortalUtil.toDouble(request.getParameter("HeightEnabled")));
      dct_Para.put("IsVisible",PortalUtil.toDouble(request.getParameter("IsVisible")));
      dct_Para.put("AllowRemove",PortalUtil.toDouble(request.getParameter("AllowRemove")));
      dct_Para.put("IsIncluded",PortalUtil.toDouble(request.getParameter("IsIncluded")));
      dct_Para.put("WidthEnabled",PortalUtil.toDouble(request.getParameter("WidthEnabled")));
      dct_Para.put("WidthUnitType",WidthUnitType);
      dct_Para.put("Height",PortalUtil.toDouble(request.getParameter("Height")));
      dct_Para.put("HasFrame",PortalUtil.toDouble(request.getParameter("HasFrame")));
      dct_Para.put("FrameState",PortalUtil.toDouble(request.getParameter("FrameState")));
      dct_Para.put("RequiresIsolation",PortalUtil.toDouble(request.getParameter("RequiresIsolation")));

      int iReturn = oManager.rsSyncModuFunc(m_modu_id,m_modu_type,m_func_id,m_func_type);
      if(iReturn < 0)
      {
        String Msg = "updateError";
      }
      iReturn = oSys.update(dct_Key,dct_Para,"user_prof",false);
      return;
    }
    //否则
    else{
        m_Rstemp = oManager.rsGetPersonModuFunction(m_usercode, m_modu_id,m_modu_type,m_func_id,m_func_type,m_Source);
        if (m_Rstemp.getRowCount()>0){
          m_Title			=jbportal.PortalUtil.getValue(m_Rstemp.getString("Title",0));
          m_href			=jbportal.PortalUtil.getValue(m_Rstemp.getString("href",0));
          m_Description		=jbportal.PortalUtil.getValue(m_Rstemp.getString("Description",0));
          m_href_desc		        =jbportal.PortalUtil.getValue(m_Rstemp.getString("href_desc",0));
          m_AllowMinimize		=jbportal.PortalUtil.getValue(m_Rstemp.getString("AllowMinimize",0));
          m_Width			=jbportal.PortalUtil.getValueN(m_Rstemp.getString("Width",0));
          m_Zone			=jbportal.PortalUtil.getValueN(m_Rstemp.getString("Zone",0));
          m_HeightUnitType		=jbportal.PortalUtil.getValue(m_Rstemp.getString("HeightUnitType",0));
          m_HeightEnabled		=jbportal.PortalUtil.getValueN(m_Rstemp.getString("HeightEnabled",0));
          m_IsVisible		        =jbportal.PortalUtil.getValue(m_Rstemp.getString("IsVisible",0));
          m_AllowRemove		=jbportal.PortalUtil.getValue(m_Rstemp.getString("AllowRemove",0));
          m_IsIncluded		=jbportal.PortalUtil.getValue(m_Rstemp.getString("IsIncluded",0));
          m_WidthEnabled		=jbportal.PortalUtil.getValueN(m_Rstemp.getString("WidthEnabled",0));
          m_WidthUnitType		=jbportal.PortalUtil.getValue(m_Rstemp.getString("WidthUnitType",0));
          m_Height			=jbportal.PortalUtil.getValueN(m_Rstemp.getString("Height",0));
          m_HasFrame		        =jbportal.PortalUtil.getValue(m_Rstemp.getString("HasFrame",0));
          m_FrameState		=jbportal.PortalUtil.getValue(m_Rstemp.getString("FrameState",0));
          m_RequiresIsolation		=jbportal.PortalUtil.getValueN(m_Rstemp.getString("RequiresIsolation",0));
        }
        else{
          m_func_id="";
        }
        //显示位置
        if (m_Zone.equals("")) m_Zone = "3";
        m_StrZone1 =jbportal.PortalUtil.LArrayRadios("Zone", m_aryDis1, m_aryVal1,m_aryImg1,m_Zone);
        m_StrZone2 =jbportal.PortalUtil.LArrayRadios("Zone", m_aryDis2, m_aryVal2,m_aryImg2,m_Zone);
        m_StrZone3 =jbportal.PortalUtil.LArrayRadios("Zone", m_aryDis3, m_aryVal3,m_aryImg3,m_Zone);

        //预设状态
        if (m_FrameState.equals("")) m_FrameState="0";
        m_StrFrameState =jbportal.PortalUtil.LArrayRadios("FrameState", m_aryFrameState1, m_aryFrameState2,m_aryFrameState3,m_FrameState);

        //固定高度

        if (m_HeightEnabled.equals("") || m_HeightEnabled.equals("0")){
          m_HeightEnabled = "0";
          sCheckedNo = "CHECKED=''";
          sCheckedYes = "";
        }
        else{
          sCheckedYes = "CHECKED=''";
          sCheckedNo = "";
        }
        sOnClick = " onClick='JavaScript:IniHeightEnabled();' ";
        m_StrHeightEnabled1 = "<INPUT TYPE='RADIO' NAME='HeightEnabled' VALUE='1' " + sCheckedYes + sOnClick + " >Yes&nbsp;</INPUT>";
        m_StrHeightEnabled2 = "<INPUT TYPE='RADIO' NAME='HeightEnabled' VALUE='0' " + sCheckedNo + sOnClick + " >No&nbsp;</INPUT>";

        //高度单位
        m_StrHeightUnitType =jbportal.PortalUtil.ArrayOptions(m_UnitTypeValue,m_UnitType,m_HeightUnitType);

        //固定宽度
        if (m_WidthEnabled.equals("") || m_WidthEnabled.equals("0")){
          m_WidthEnabled = "0";
          sCheckedNo = "CHECKED=''";
          sCheckedYes = "";
        }
        else{
          sCheckedYes = "CHECKED=''";
          sCheckedNo = "";
        }
        sOnClick = " onClick='JavaScript:IniWidthEnabled();' ";
        m_StrWidthEnabled1 = "<INPUT TYPE='RADIO' NAME='WidthEnabled' VALUE='1' " + sCheckedYes + sOnClick + " >Yes&nbsp;</INPUT>";
        m_StrWidthEnabled2 = "<INPUT TYPE='RADIO' NAME='WidthEnabled' VALUE='0' " + sCheckedNo + sOnClick + " >No&nbsp;</INPUT>";
        //宽度单位
        m_StrWidthUnitType =jbportal.PortalUtil.ArrayOptions(m_UnitTypeValue,m_UnitType,m_WidthUnitType);
    }
%>
<html>
<link rel="STYLESHEET" type="text/css" href="../../include/Public.css">

<script type="text/javascript" src="../../include/i_CheckInput.js"></script>
<script type="text/javascript" src="../../include/i_ExpandJScript.js"></script>
<script type="text/javascript" src="../../include/i_LockControls.js"></script>

<script type="text/javascript">
<!--
function preSave(){
  arrZone=document.all.item("Zone");
  var i=0;
  var selectedIndex=0;
  if(document.all.item("FrameState")[0].checked){
    document.all.item("hidden_framestate").value=document.all.item("FrameState")[0].value;
  }
  else{
    document.all.item("hidden_framestate").value=document.all.item("FrameState")[1].value;
  }
  for(i=0;i<arrZone.length;i++){
    if(arrZone.item(i).checked){
      selectedIndex=i;
      break;
    }
  }

  document.all.item("hidden_zone").value=arrZone.item(selectedIndex).value;
}
function Save(){
	//存盘前要验证输入是否正确,如果检验不功过则返回
	if (ValidateInput()==false)
		return;
    var strURL;
	var xhreq;
    preSave();
	xhreq = new ActiveXObject("Microsoft.XMLHTTP");
	strUrl = "modufunc_setup.jsp?cmd=update";
    strUrl += "&func_id=" +document.all.item("func_id").value;
    strUrl += "&modu_id=" +document.all.item("modu_id").value;
    strUrl += "&func_type=" +document.all.item("func_type").value;
    strUrl += "&modu_type=" +document.all.item("modu_type").value;
    if (document.all.item("AllowMinimize").checked==true)
      strUrl += "&AllowMinimize=1";
    else
      strUrl += "&AllowMinimize=0";
    strUrl += "&Width=" +document.all.item("Width").value;
    strUrl += "&Zone=" +document.all.item("hidden_zone").value;
    strUrl += "&HeightUnitType=" +document.all.item("HeightUnitType").value;
    strUrl += "&HeightEnabled=" +document.all.item("hidden_heightenabled").value;
    if (document.all.item("IsVisible").checked==true)
      strUrl += "&IsVisible=1";
    else
      strUrl += "&IsVisible=0";
    if (document.all.item("AllowRemove").checked==true)
      strUrl += "&AllowRemove=1";
    else
      strUrl += "&AllowRemove=0";
    if (document.all.item("IsIncluded").checked==true)
      strUrl += "&IsIncluded=1";
    else
      strUrl += "&IsIncluded=0";
    strUrl += "&WidthEnabled=" +document.all.item("hidden_widthenabled").value;
    strUrl += "&WidthUnitType=" +document.all.item("WidthUnitType").value;
    strUrl += "&Height=" +document.all.item("Height").value;
    if (document.all.item("HasFrame").checked==true)
      strUrl += "&HasFrame=1";
    else
      strUrl += "&HasFrame=0";
    strUrl += "&FrameState=" +document.all.item("hidden_framestate").value;
    strUrl += "&RequiresIsolation=" +document.all.item("RequiresIsolation").value;
    //alert(strUrl);
	xhreq.open("POST", strUrl, false);

	xhreq.send();
	// If we got "OK" back, then consider the post successful.
	if("OK" == xhreq.statusText)
        window.returnValue="success";
	else{
		alert( xhreq.statusText);
        window.returnValue="cancel";
    }
    window.close();
}

function GetHelp(){
	alert("抱歉，帮助暂缺!");
}

function Exit()
{
  window.returnValue="cancel"
  window.close();
}
//检验输入是否合法
function ValidateInput()
{
	var l_strErrDesc;
	l_strErrDesc = Validate_Input();
	if ( l_strErrDesc == ""){
		return true;
	}
	//alert(l_strErrDesc);
	return false;
}

//窗体load时的事件
function window_onload()
{
	var sArray,sFuncID;
	var sZone;
	sFuncID='<%=m_func_id%>';
	IniHeightEnabled();
	IniWidthEnabled();
	IniCheckded();
	if (sFuncID=="")
		{
			if ('<%=m_Source%>'=="stportal")
			{
			alert("记录不存在");
			window.close();

			return;}
			else
			{LockControls();}
		}
}

function IniHeightEnabled()
{
	//1、判断是否允许高度以及单位
	var bEnabled;
	bEnabled = document.all.item("HeightEnabled")[1].checked;  //yes
	//改Disabled属性
	document.all.item("Height").disabled=bEnabled;
	document.all.item("HeightUnitType").disabled=bEnabled;
	//改bgcolor属性
	if (bEnabled==true){
		document.all.item("Height").style.backgroundColor="#c0c0c0";
		document.all.item("HeightUnitType").style.backgroundColor="#c0c0c0";
        document.all.item("hidden_heightenabled").value="0";
	}
	else
	{
		document.all.item("Height").style.backgroundColor=document.all.item("Height").currentStyle.getAttribute("backgroundColor",2);
		document.all.item("HeightUnitType").style.backgroundColor=document.all.item("HeightUnitType").currentStyle.getAttribute("backgroundColor",2);
        document.all.item("hidden_heightenabled").value="1";
	}
}

function IniWidthEnabled()
{
	//1、判断是否允许高度以及单位
	var bEnabled;
	bEnabled = document.all.item("WidthEnabled")[1].checked;  //yes
	//改Disabled属性
	document.all.item("Width").disabled=bEnabled;
	document.all.item("WidthUnitType").disabled=bEnabled;
	//改bgcolor属性
	if (bEnabled==true){
		document.all.item("Width").style.backgroundColor="#c0c0c0";
		document.all.item("WidthUnitType").style.backgroundColor="#c0c0c0";
        document.all.item("hidden_widthenabled").value="0";
	}
	else
	{
		document.all.item("Width").style.backgroundColor=document.all.item("Width").currentStyle.getAttribute("backgroundColor",2);
		document.all.item("WidthUnitType").style.backgroundColor=document.all.item("WidthUnitType").currentStyle.getAttribute("backgroundColor",2);
        document.all.item("hidden_widthenabled").value="1";
	}
}

//初始化CheckBox
function IniCheckded()
{
	if ('<%=m_IsVisible%>'==0)
		{document.all.item("IsVisible").checked=false;}
	else
		{document.all.item("IsVisible").checked=true;}

	if ('<%=m_AllowRemove%>'==0)
		{document.all.item("AllowRemove").checked=false;}
	else
		{document.all.item("AllowRemove").checked=true;}

	if ('<%=m_IsIncluded%>'==0)
		{document.all.item("IsIncluded").checked=false;}
	else
		{document.all.item("IsIncluded").checked=true;}

	if ('<%=m_HasFrame%>'==0)
		{document.all.item("HasFrame").checked=false;}
	else
		{document.all.item("HasFrame").checked=true;}

	if ('<%=m_AllowMinimize%>'==0)
		{document.all.item("AllowMinimize").checked=false;}
	else
		{document.all.item("AllowMinimize").checked=true;}

	if ('<%=m_RequiresIsolation%>'==0)
		{document.all.item("RequiresIsolation").checked=false;}
	else
		{document.all.item("RequiresIsolation").checked=true;}
}
window.attachEvent("onload",window_onload);

//-->
</script>
<body leftmargin="0" topmargin="0" rightmargin="0" >
<form ACTION="modufunc_setup.jsp" METHOD="post" id="modufunc_setup" name="modufunc_setup">
  <div align="center">
  <TABLE width="100%" height="100%" cellpadding="0" cellspacing="0" class="Table_Normal" align=center>
      <TR>
        <TD colspan="2" WIDTH=100%>
              <TABLE width="100%" cellpadding="0" cellspacing="0" class="InnerTable" align=center>
                <tr align=center>
                    <TD>
                    <TABLE WIDTH="100%" CELLPADDING="0" CELLSPACING="0" border="0" Class="InnerTable">
                        <TR>
                            <TD CLASS="CustSectionRight" width="100%" colspan="3">
                            <fieldset style="padding:6px;width:100%">
                                <legend>
                                    <span id="usersetting">主设定</span>
                                </legend>
                                <TABLE WIDTH="100%" class="InnerTable">
                                    <TR>
                                        <TD CLASS="CustSectionRight"><input type="label" style="width:30" class="Input_Label" Value="名称:" id=label2 name=label2></TD>
                                        <TD CLASS="CustSectionRight" >
                                            <input type="text" maxlength="100" name="Title" style="width:180; overflow:hidden" class="input_ReadOnly" readonly  value=<%=m_Title%>  >
                                        </TD>
                                        <TD CLASS="CustSectionRight"><input type="label" style="width:30" class="Input_Label" Value="描述:" id=label1 name=label1></TD>
                                        <TD CLASS="CustSectionRight" >
                                            <TEXTAREA NAME="Description" ROWS="1" STYLE="width:180" class="input_ReadOnly" readonly ><%=m_Description%></TEXTAREA>
                                        </TD>
                                    </TR>
                                    <TR>
                                        <TD CLASS="CustCheckboxText" >链接:</TD>
                                        <TD CLASS="CustCheckboxText" colspan="3" >
                                            <INPUT TYPE="text" NAME="href" STYLE="width:420;overflow:hidden" class="input_ReadOnly" readonly value=<%=m_href%>>
                                        </TD>
                                    </TR>
                                    <TR>
                                        <TD CLASS="CustCheckboxText" >链接说明:</TD>
                                        <TD CLASS="CustCheckboxText" Colspan="3" >
                                            <INPUT TYPE="TEXT" NAME="href_desc" STYLE="width:420;overflow:hidden" class="input_ReadOnly"  readonly value=<%=m_href_desc%>>
                                        </TD>
                                    </TR>
                                </TABLE>
                            </fieldset>
                            </TD>
                        </TR>
                        <TR>
                            <TD CLASS="CustSectionRight" width="50%" valign="TOP">
                            <fieldset style="padding:4px;width:100%;height:200">
                                <legend>
                                    <span id="usersetting" >位置设定</span>
                                </legend>
                                <TABLE WIDTH="100%" class="InnerTable">
                                    <TR>
                                        <TD class="CustCheckbox">
                                            <INPUT TYPE="CHECKBOX" NAME="IsIncluded" VALUE=1 >启用
                                        </TD>
                                        <TD class="CustCheckbox">
                                            <INPUT TYPE="CHECKBOX" NAME="IsVisible" VALUE=1 >显示内容
                                        </TD>
                                        <TD class="CustCheckbox">
                                            <INPUT TYPE="CHECKBOX" NAME="HasFrame" VALUE=1 >显示控制行
                                        </TD>
                                    </TR>
                                    <TR>
                                        <TD CLASS="CustCheckbox">
                                            <INPUT TYPE="CHECKBOX" NAME="AllowRemove" VALUE=1>可关闭
                                        </TD>
                                        <TD class="CustCheckbox">
                                            <INPUT TYPE="CHECKBOX" NAME="AllowMinimize" VALUE=1>可最小化
                                        </TD>
                                        <TD class="CustCheckbox">
                                        </TD>
                                    </TR>
                                </TABLE>
                                显示位置:
                                <TABLE class="InnerTable" width="100%">
                                  <%=m_StrZone1%>
                                  <%=m_StrZone2%>
                                  <%=m_StrZone3%>
                                </TABLE>
                            </fieldset>
                        </TD>
                        <TD CLASS="CustSectionRight" width="50%" valign="TOP">
                            <fieldset style="padding:4px;width:100%;height:200">
                                <legend>
                                    <span id="usersetting" >其他设定</span>
                                </legend>
                                预设状态
                                <TABLE class="InnerTable">
                                    <%=m_StrFrameState%>
                                </TABLE>
                                固定高度:
                                <BR />
                                <TABLE class="InnerTable">
                                    <TR>
                                        <TD>
                                            <%=m_StrHeightEnabled1%>
                                        </TD>
                                        <TD>
                                            <%=m_StrHeightEnabled2%>
                                        </TD>
                                        <TD>
                                            <INPUT TYPE="text" NAME="Height" STYLE="WIDTH:.5IN;OVERFLOW:HIDDEN" class="input_Normal"  autocheck="1" datatype="1" propname="高度" value=<%=m_Height%>>
                                        </TD>
                                        <TD>
                                            <SELECT NAME="HeightUnitType" SIZE="1" SYTLE="WIDTH:.5IN" class="DrawSelect">
                                                <%=m_StrHeightUnitType%>
                                            </SELECT>
                                        </TD>
                                    <TR>
                                </TABLE>
                                固定宽度:
                                <BR />
                                <TABLE CLASS="InnerTable">
                                    <TR>
                                        <TD>
                                            <%=m_StrWidthEnabled1%>
                                        </TD>
                                        <TD>
                                            <%=m_StrWidthEnabled2%>
                                        </TD>
                                        <TD>
                                            <INPUT TYPE="text" NAME="Width" STYLE="WIDTH:.5IN;OVERFLOW:HIDDEN" class="input_Normal"  autocheck="1" datatype="1" propname="宽度" value=<%=m_Width%>>
                                        </TD>
                                        <TD>
                                            <SELECT NAME="WidthUnitType" SIZE="1" SYTLE="WIDTH:.5IN" class="DrawSelect">
                                                <%=m_StrWidthUnitType%>
                                            </SELECT>
                                        </TD>
                                    </TR>
                                <TABLE>
                                    <TR>
                                        <TD CLASS="CustCheckbox">
                                            <INPUT TYPE="CHECKBOX" NAME="RequiresIsolation" VALUE=1 >
                                        </TD>
                                        <TD CLASS="CustCheckboxText">
                                        自动滚动
                                        </TD>
                                    </TR>
                                </TABLE>
                            </fieldset>
                        </TD>
                    </TR>
                    <TR>
                        <TD class="CustButtonSectionRight" NOWRAP="" colspan="4" align="right">
                            <INPUT class="Input_Button" TYPE="Button"  value="确  定"  id=Button1 name=Button1 onclick="javascript:Save();">
                            <INPUT class="Input_Button" TYPE="Button"  value="取  消"  id=Button1 name=Button1 onclick="javascript:Exit();">
                        </TD>
                        <TD CLASS="CustSpacer">&nbsp;</TD>
                    </TR>
                </TABLE>
          </TD>
      </TR>
</TABLE>
</div>
<input type="hidden" name="func_id" value=<%=m_func_id%>>
<input type="hidden" name="modu_id" value=<%=m_modu_id%>>
<input type="hidden" name="func_type" value=<%=m_func_type%>>
<input type="hidden" name="modu_type" value=<%=m_modu_type%>>
<input type="hidden" name="hidden_zone" value=<%=m_Zone%>>
<input type="hidden" name="hidden_framestate" value=<%=m_FrameState%>>
<input type="hidden" name="hidden_widthenabled" value=<%=m_WidthEnabled%>>
<input type="hidden" name="hidden_heightenabled" value=<%=m_HeightEnabled%>>
</form>
</body>
</html>
