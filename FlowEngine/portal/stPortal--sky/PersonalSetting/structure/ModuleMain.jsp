<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="suntek.portal.publicLib.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />

<%
    //定义常量
    int Init = 0;
    String m_sMenuString="";
    RecordSetObject m_rsAll=null;
    //-----------------------//
    String m_usercode=(String)request.getRemoteUser();
    String m_username =oManager.getCurrentUserName();

    String DetailbaordID = jbportal.PortalUtil.getValue(request.getParameter("DetailbaordID"));
    String DetailBoardType =jbportal.PortalUtil.getValueN(request.getParameter("DetailBoardType"));
    String DetailBoardFunc =jbportal.PortalUtil.getValue(request.getParameter("DetailBoardFunc"));
    String DashboardID =jbportal.PortalUtil.getValue(request.getParameter("DashboardID"));
    String DashboardType =jbportal.PortalUtil.getValueN(request.getParameter("DashboardType"));
    String isHasModule="yes";
    oManager.rsSyncAllModu(m_usercode,true);
    m_rsAll = oManager.rsGetPersonAllModu(m_usercode,false);
    try{
      m_sMenuString =jbportal.PortalUtil.FillClassEx(m_rsAll,"-");
      //System.err.println(m_sMenuString);
      if(m_sMenuString==null || m_sMenuString.equals("")){
        isHasModule="no";
      }
    }
    catch(Exception e){
        System.err.print("Error in PersonalSetting/ModuleMain.jsp,Error Is:"+e.getMessage());
        e.printStackTrace(System.err);
        throw e;
    }
%>
<html>
<title>模块管理</title>


<link rel="STYLESHEET" type="text/css" href="../../include/tree/css/WebFX-ContextMenu.css">
<link rel="STYLESHEET" type="text/css" href="../../include/tree/css/Node.css">
<link rel="STYLESHEET" type="text/css" href="../../css/WebToolBar/toolbar.css">
<script LANGUAGE="javascript" src="../../include/tree/ContextMenu.js"></script>
<script LANGUAGE="javascript" src="../../include/tree/genMove.js"></script>
<script LANGUAGE="javascript" src="../../include/webtoolbar.js"></script>
<script LANGUAGE="javascript" src="../../include/tree/tTree.js"></script>
<script LANGUAGE="javascript" src="../../include/tree/PortalTree.js"></script>
<script LANGUAGE="javascript">
<!--
//==这里开始 为了实现树形显示，对树模型对象进行适当的扩充的代码

//图片路径常量
var StrLeafImage = "<img src=\"../../images/line.gif\" border=0>";
var StrLeafImage1 = "<img src=\"../../images/line1.gif\" border=0>";
var StrEmptyImage = "<img src=\"../../images/dot.gif\" border=0 style=\"width:15px\">";

var StrCloseImage = "<img src=\"../../images/add.gif\" border=\"0\">";
var StrExpandImage = "<img src=\"../../images/sub.gif\" border=\"0\">";

var StrCloseImageTop = "<img src=\"../../images/top.gif\" border=\"0\">";
var StrExpandImageTop = "<img src=\"../../images/-top.gif\" border=\"0\">";
var StrCloseImageBottom = "<img src=\"../../images/bottom.gif\" border=\"0\">";
var StrExpandImageBottom = "<img src=\"../../images/-bottom.gif\" border=\"0\">";
var StrCloseImageLeft = "<img src=\"../../images/left.gif\" border=\"0\">";
var StrExpandImageLeft = "<img src=\"../../images/-left.gif\" border=\"0\">";

var StrExpandImage2 = "<img src=\"../../images/open.gif\" border=\"0\">";
var StrCloseImage2 = "<img src=\"../../images/close.gif\" border=\"0\">";
var StrPersonalClose = "<img src=\"../../images/personal.gif\" border=\"0\">";
var StrPersonalOpen = "<img src=\"../../images/personalopen.gif\" border=\"0\">";
var StrBackground="../../images/line-bg.gif";
var StrFucntionImage = StrPersonalClose;
//当前节点的值
var StrLastNode="";
var StrNewNodeName="新模块";
var StrNewNodeDesc="新模块";

var sMsg="";
var sToolBar="property";

//结点的右键菜单事件
function tNodeOnContextMenu()
{

}
//重新排序
function ChangeNodeIndex(a_node)
{
	var l_strchild,l_oNode;
	var l_SQL;
	l_SQL="";
	if (a_node.paraNode!=null)
		{

			for (l_strchild in a_node.paraNode.children.nodes)
			{
				l_oNode = a_node.paraNode.children.nodes[l_strchild];
				l_SQL += l_oNode.nodeName + "," + l_oNode.Index() + ";"
			}
		ModuleMain.HOrder.value = l_SQL;
		ModuleMain.target="update";
		ModuleMain.action="_AdjustOrder.jsp";
		ModuleMain.submit ();
		ModuleMain.HOrder.value = "";
		SetDBFocus(a_node.nodeName+"_Value");
		}

}

/*删除当前选中的节点
*/
function OnDelete(){
		var TrID,NodeName,NodeDesc;
		var oNode,bReturn;

		if ( StrLastNode != "" && StrLastNode != "0_Value" ){

			//得到当前行的ID
			NodeName = StrLastNode.substr(0,StrLastNode.length-6);
			//得到当前的节点
			oNode = oTree.Find(NodeName)
			//得到节点的描述
			NodeDesc = oNode.array.Find('cate_desc')
			//判断当前节点是否有子节点，如果有则不允许删除
			if (oNode.children.count > 0 )
				{
					alert("请先删除节点["+NodeDesc+"]的子节点。");
					return;
				}
			bReturn=confirm("确定要删除节点["+NodeDesc+"]吗?");
			if (bReturn!=true)
				{
					return;
				}
			//从数据库删除
			document.all.item("HNodeValue").value = NodeName;
			ModuleMain.target="update";
			ModuleMain.action="_TreeAdd.jsp?m_Operation=del";
			ModuleMain.submit ();
			document.all.item("HNodeValue").value = "";
		}

}

//删除用户的个性化模块设置，恢复成缺省的模块设置
function RestoreModule()
{
	//询问用户
	bReturn=confirm("确定要恢复成缺省的模块设置吗?如果选择是，模块的顺序将恢复成缺省的状态！");
	//执行删除
	if( bReturn )
	{
		ModuleMain.target="";
		ModuleMain.action="_RestoreModule.jsp";
		ModuleMain.submit ();
	}

}

//恢复当前模块的的缺省排版设置
function RestoreLayout()
{
	//如果是个人模块，则不需要恢复
	var l_NodeValue,oNode,l_modutype;
	var l_desc;

	if (StrLastNode=="" || StrLastNode=="0_Value") { alert("请先选择具体的模块");return;}

	l_NodeValue = StrLastNode.substr(0,StrLastNode.length-6);

	oNode = oTree.Find(l_NodeValue);
	l_modutype = oNode.array.Find("cate_type");
	l_NodeValue = oNode.array.Find("cate_value");

	if( 1 == l_modutype )
	{
		alert("当前模块是个人模块，不能恢复排版！");
		return;
	}
	bReturn=confirm("确定要恢复当前模块的缺省排版设置吗?如果选择是，当前模块的排版将恢复成缺省的状态！");
	//执行恢复
	if( bReturn )
	{
		ModuleMain.target="main";
		ModuleMain.action="_RestoreLayout.jsp?modu_id=" + l_NodeValue +
						  "&modu_type=" + l_modutype;
		ModuleMain.submit ();
	}

}



//给当前节点增加一个子节点
function OnAddNew(){
	//如果当前没有选中节点则返回
	var paraTable,TrID,l_array,oTr,oNode,sReturn,oPara;
	var para_id,modu_id,modu_title,modu_description

	if (StrLastNode=="")
		return;
	//假设得到相关的资料,并建立一个新节点
	para_id=StrLastNode.substr(0,StrLastNode.length-6);
	//
	AddToDB(para_id);
}

//向数据库添加一条记录,返回新的记录的NodeValue
function AddToDB(a_paraid)
{

	if (a_paraid == '0') {a_paraid='';}
	document.all.item("HNodeValue").value = a_paraid;
	document.all.item("HNodeName").value = StrNewNodeName;
	document.all.item("HNodeDesc").value = StrNewNodeDesc;
	ModuleMain.target="update";
	ModuleMain.action="_TreeAdd.jsp?m_Operation=add";
	ModuleMain.submit ();
	document.all.item("HNodeValue").value = "";
	document.all.item("HNodeName").value = "";
	document.all.item("HNodeDesc").value = "";
}

//增加一个新的节点
function AddNewNode(a_NewNodeID)
{
	//如果当前没有选中节点则返回
	var paraTable,TrID,l_array,oTr,oNode,sReturn,oPara;
	var para_id,modu_id,modu_title,modu_description

	if (StrLastNode=="")
		return;

	//假设得到相关的资料,并建立一个新节点
	para_id=StrLastNode.substr(0,StrLastNode.length-6);

	//


	//得到新的节点号
	modu_id=a_NewNodeID;
	modu_title=StrNewNodeName;
	modu_description=StrNewNodeDesc;


	l_array = new tArray();
	l_array.Add('cate_desc', modu_description);
	l_array.Add('cate_value',modu_id);
	l_array.Add('cate_para',para_id);

	sReturn=oTree.Add(modu_id,para_id,l_array);


	//得到当前tr对象
	TrID= para_id  + "_tr";

	//paraTable=document.all.item(TrID).parentElement.parentElement;
	//先在外面的大Table增加一个<TR>
	paraTable=document.all.item(para_id+"_childrentable");

	oTr=paraTable.insertRow();
	oTr.id = modu_id+"_tr";

	//得到当前节点
	oNode = oTree.Find(modu_id);

	//在当前节点下增加一个子节点，默认为newModule
	GetTD1(oNode,oTr);

	SetDBFocus(modu_id + "_Value");
	paraTable.style.display="";

	oTree.AdjustPic();
}

//如果新增或者删除发生错误则显示错误
function MsgError(a_Error)
{
	alert('错误'+ a_Error);
}
//-->
</script>


<script LANGUAGE="javascript">
<!--
var oTree,ifExpandTree = 1;
oTree = new tTree();
oArray = new tArray();
oArray.Add('cate_desc','<%=m_username%>界面调整');
oArray.Add('cate_value','0');
oTree.Add('0',null,oArray);
<%=m_sMenuString%>


//set the user css
function CSSSetting()
{
	window.showModalDialog("CSSSetting.jsp","","dialogHeight:150px;center:yes;edge:raised;scroll:no;status:no;unadorned:yes;");
}
//结点的OnClick事件
function tNodeOnClick()
{
	var elem = window.event.srcElement;
	var strNodeID;

	strNodeID=elem.id;

        //alert('function=SetDBFocus()');
	SetDBFocus(strNodeID);
}


//处理节点的点击功能
function GetNewPage(strNodeID){
	//得到节点代码
	var l_NodeValue,oNode,l_modutype;
	var l_desc;
	l_NodeValue = strNodeID.substr(0,strNodeID.length-6);

	//0 : 修改 1 :新增
	if ( parent != null )
	{
		oNode = oTree.Find(l_NodeValue);
		l_modutype = oNode.array.Find("cate_type");
		l_NodeValue = oNode.array.Find("cate_value");
		l_desc = oNode.array.Find("cate_desc");


		window.main.location="ModuleDetails.jsp?m_moduid=" + l_NodeValue +
							  "&modu_type=" + l_modutype + "&Title=" + l_desc;
	}
}


function ShowNode(a_NodeValue)
{

	var l_NodeValue = a_NodeValue.substr(0,a_NodeValue.length-6);

	oNode = oTree.Find(l_NodeValue);

        //alert("function=SetDBFocus()");
	SetDBFocus(a_NodeValue );

	oTemp = oNode.paraNode;
	while ( !(oTemp.nodeName == "0") )
	{
		l_tb = document.all.item(oNode.nodeName + '_childrentable');
		l_dis = l_tb.style.display;
		if ( l_dis == 'none' )
		{
			oTemp.HideTable();
		}
		oTemp = oTemp.paraNode;
	}
}

//设置节点的焦点
function SetDBFocus(strNodeID)
{
	if (StrLastNode != "")
	{
			document.all.item(StrLastNode).style.fontWeight = 'normal';
	}

	document.all.item(strNodeID).style.fontWeight = 'bold';
        StrLastNode = strNodeID;

	if (StrLastNode=="" || StrLastNode=="0_Value")
		SwitchPage( "property" );	 //Switch to the first page
        //alert("function=ShowPropertyPage()");
	ShowPropertyPage();
}

//select which page to show, property? function list? or layout page
function SwitchPage( sNewToolBar )
{
	if ( sToolBar != "" )
		SetCheck( sToolBar,false );
	SetCheck( sNewToolBar, true );
	sToolBar = sNewToolBar;
	ShowPropertyPage();
}

function ShowPropertyPage()
{
	if( sToolBar == "property")
	{
		if( StrLastNode != "")
        {
        	GetNewPage( StrLastNode);
        }
		else
        {
			window.main.location = "ModuleDetails.jsp";
        }
	}
	else if(sToolBar == "funclist")
	{
		GetFuncList();
	}
	else
	{
                //alert("function=Layout()");
		Layout();
	}
}
//==这里结束 为了实现树形显示，对树模型对象进行适当的扩充的代码
function Layout()
{
	var l_moduID,oNode,l_modutype,l_desc;
	var l_allowpersonize;

	if (StrLastNode=="" || StrLastNode=="0_Value") { alert("请先选择具体的模块");return;}

	l_moduID = StrLastNode.substr(0,StrLastNode.length-6);

	//判断是否允许个性化
	if ( parent != null  )
	{
		oNode = oTree.Find(l_moduID);
		l_modutype = oNode.array.Find("cate_type");

		l_moduID = oNode.array.Find("cate_value");
                //alert('l_modutype='+ l_modutype + ';l_moduID=' + l_moduID);
		l_desc = oNode.array.Find("cate_desc");
		l_allowpersonize = oNode.array.Find("cate_allowpersonize");

		if (l_allowpersonize==0)
		{
			alert("模块(" + l_desc + ")不允许被个性化!" );
			return;
		}

		window.main.location="DragAdmin.jsp?modu_id=" + l_moduID + "&modu_type=" + l_modutype;
	}

}

//得到对应的功能列表
function GetFuncList()
{
	var l_moduID,l_nodeName,oNode;
	var l_NodeValue,l_desc,l_modutype,l_allowpersonize;

	if (StrLastNode=="" || StrLastNode=="0_Value") { alert("请先选择具体的模块");return;}
	l_moduID = StrLastNode.substr(0,StrLastNode.length-6);

	//0 : 修改 1 :新增
	if ( parent != null )
	{
		oNode = oTree.Find(l_moduID);
		l_modutype = oNode.array.Find("cate_type");
		l_NodeValue = oNode.array.Find("cate_value");
		l_desc = oNode.array.Find("cate_desc");
		l_allowpersonize = oNode.array.Find("cate_allowpersonize");

		if (l_allowpersonize==0)
		{
			alert("模块(" + l_desc + ")不允许被个性化!" );
			return;
		}
        window.main.location="funcList.jsp?modu_id=" + l_NodeValue +
							 "&modu_type="+l_modutype;
	}

}
function window_onload()
{
	var oNode;
	ShowTree();
	DivResize();

	//展开树的第一级
	oNode = oTree.Find('0');
	if (oNode!=null){
		oNode.HideTable();
	}

	if( m_isHasModel=="yes" && m_DetailBoardID != "" )
	{
		if (m_DetailBoardFunc=='funclist')
		{
			ShowNode( m_DashBoardID + "-" + m_DashBoardType +"_Value");
			SwitchPage(m_DetailBoardFunc);
		}
		else
		{
			ShowNode( m_DetailBoardID + "-" + m_DetailBoardType +"_Value");
			SwitchPage("property");
		}
	}
	else
		SwitchPage("property");
}
function ShowTree()
{

	ifExpandTree = ( ifExpandTree == 1 ) ? 0 : 1; //切换是否展开所有结点的标志
	document.all.item('divRender').innerHTML = oTree.RenderTable(ifExpandTree);
//	document.all.item('divDebug').innerText = oTree.toString();
}

function DivResize(){
	//改变显示区的大小
	document.all.item('divRender').style.height = document.body.offsetHeight -10;
}

var m_DetailBoardID = '<%=DetailbaordID%>';
var m_DetailBoardType = '<%=DetailBoardType%>';

var m_DetailBoardFunc = '<%=DetailBoardFunc%>';
var m_DashBoardID  ='<%=DashboardID%>';
var m_DashBoardType = '<%=DashboardType%>';
var m_isHasModel = '<%=isHasModule%>';

window.onresize=DivResize;

window.attachEvent("onload",window_onload);
//-->
//-->



</script>
<!--在这里Base的作用是设置缺省target -->
<link rel="STYLESHEET" type="text/css" href="../../include/Public.css">

<body leftmargin="0" topmargin="0">
<table border="1" height="100%">
<tr>
	<td height="100%" valign="top">
		<form ACTION="ModuleMain.jsp" METHOD="post" id="ModuleMain" name="ModuleMain">
			<!--div id="TitleDiv" style="height:12">
				<table class="menutitle" width="100%" cellspacing="0" cellpadding="0" border="0">
					<tr>
						<td>模块管理</td>
						<td width="20"><img id="funclist" alt="功能列表" src="../../images/go.gif" class="image" onclick="JavaScript:GetFuncList();" WIDTH="16" HEIGHT="16"></td>
						<td width="20"><img id="go" alt="设置功能" src="../../images/showtree.gif" class="image" onclick="JavaScript:Layout();" WIDTH="16" HEIGHT="16"></td>
						<td width="20"><img id="Refresh" alt="刷新" src="../../images/refresh.gif" class="image" onclick="JavaScript:RefreshPage()" WIDTH="11" HEIGHT="11"></td>
					</tr>
				</table>
			</div-->

			<div id="divDebug"></div>
			<div id="divRender" style="width:200;overflow-x:auto;overflow-y:auto;"></div>
			<input type=hidden name="HNodeValue" value="">
			<input type=hidden name="HNodeName" value="">
			<input type=hidden name="HNodeDesc" value="">
			<input type=hidden name="HOrder" value="">
		</form>
	</td>

	<td width="100%">
		<table border="1" width="100%" height="100%">
			<tr>
				<td>
					<div id="toolbar"></div>
					<script language="javascript">
						SetToolBarHeight(40);
						AddToolBarItemEx("property","../../images/properties_h.gif","","","属性","60","0","SwitchPage('property')");
						AddToolBarItemEx("funclist","../../images/go.gif","","","功能列表","60","0","SwitchPage('funclist')");
						AddToolBarItemEx("layout","../../images/showtree.gif","","","排版","60","0","SwitchPage('layout')");
						AddToolBarItemEx("restoremodule","../../images/undo.gif","","","模块恢复","60","1","RestoreModule()");
						AddToolBarItemEx("restorelayout","../../images/dbprops.gif","","","排版恢复","60","0","RestoreLayout()");
						AddToolBarItemEx("refresh","../../images/refresh.gif","","","刷新","60","1","RefreshPage()");
						//AddToolBarItemEx("navlink","../../images/navlink.gif","","","界面风格","60","1","CSSSetting()");
						window.document.getElementById("toolbar").innerHTML=GenToolBarV();
					</script>
				</td>
			</tr>
			<tr height="100%">
				<td>
					<iframe name="main" FRAMEBORDER=1 width=100% height=100% src="">
					</iframe>
					<iframe name="update" width=0 height=0 src="">
					</iframe>
				</td>
			</tr>
		</table>
	</td>

</tr>
</table>
</body>
</html>
