<%@ page contentType="text/html; charset=gb2312" errorPage="../../include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,suntek.portal.publicLib.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    //定义常量
    String const_modu_public = "0";
    String const_modu_private = "1";
    String const_func_public = "0";
    String const_func_private = "1";

    jbportal.FieldInfo  finfo=new jbportal.FieldInfo();
    finfo.setDEPTID("modu_id");
    finfo.setDEPTNAME("title");
    finfo.setDEPTPARENTID("para_id");
    finfo.setRECVALUE("modu_id");
    finfo.setTOPVALUE("");
    finfo.setLOWFLAG("low_flag");//是否子节点
    finfo.setSELECTNAME("our_select");
    finfo.setMODUTYPE("modu_type");

    String m_sMenuString="";
    RecordSetObject m_rsAll=null;

    String m_usercode=(String)request.getRemoteUser();
    String m_username =oManager.getCurrentUserName();
    String m_sSQL = "select * from pers_modu where user_code='" + m_usercode + "'" +
           " order by para_id asc,DashBoardOrder asc";
    m_rsAll = oManager.RunQuery("","","","",true,false,m_sSQL);
    try{
      m_sMenuString = jbportal.PortalUtil.FillClass(m_rsAll,"NULL",finfo);
    }
    catch(Exception e){
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

var StrExpandImage2 = "<img src=\"../../images/personalopen.gif\" border=\"0\">";
var StrCloseImage2 = "<img src=\"../../images/personal.gif\" border=\"0\">";
var StrFucntionImage = StrCloseImage2;
var StrBackground="../../images/line-bg.gif";

//当前节点的值
var StrLastNode="";
var StrNewNodeName="新模块";
var StrNewNodeDesc="新模块";

var sMsg="";
var sToolBar;

//结点的右键菜单事件
function tNodeOnContextMenu()
{
	/*
   popupoptions = [
   						new ContextItem("刷新本页",function(){window.location.reload();}),
						new ContextItem("Disabled Item",null,true),
						new ContextSeperator(),
						new ContextItem("打印",function(){window.print()}),
						new ContextItem("查看结点代码",function(){tNodeOnClick();document.body.focus();}),
						new ContextSeperator(),
   						new ContextItem("哈哈",function(){alert('哈哈');document.body.focus();})
   				  ]

   ContextMenu.display(popupoptions)*/
}

//处理节点的点击功能
function GetNewPage(strNodeID){
	//得到节点代码
	var l_moduID;
	l_moduID = strNodeID.substr(0,strNodeID.length-6);
	//0 : 修改 1 :新增
	window.main.location="ModuleDetails.jsp?m_ModuID=" + l_moduID + "&m_Operation=0";
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
	ShowPropertyPage();
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
                        //alert(NodeName);
			document.all.item("HNodeValue").value = NodeName;
			ModuleMain.target="update";
			ModuleMain.action="_TreeAdd.jsp?m_Operation=del";
			ModuleMain.submit ();
			document.all.item("HNodeValue").value = "";
		}

}

//在数据库删除数据后再删除节点
function OnDeleteNode(NodeName)
{
	var oNode,sPara;
	var strNodeID="",i,l_s;

	oNode = oTree.Find(NodeName);

	//判断是否有兄弟节点，如果有则将焦点移动到兄弟节点。
	//如果没有兄弟节点则移动到该节点的父节点上。
	if (oNode.paraNode==null)
		{	strNodeID="";		}
	else
		{
			for (l_s in oNode.paraNode.children.nodes)
			{
				if (l_s!=NodeName){
					strNodeID=l_s;
					break;
					}
			}
			if (strNodeID=="")
				{
					strNodeID=oNode.paraNode.nodeName;
				}
		}
	TrID = NodeName + "_TR";
	document.all.item(TrID).removeNode(true);
	oTree.Remove(NodeName);
	StrLastNode = "";
	oTree.AdjustPic();
	if (strNodeID!=""&&strNodeID!="undefined")
	{	strNodeID=strNodeID+"_Value";}
	SetDBFocus(strNodeID);

}

//给当前节点增加一个子节点
function OnAddNew(){
	//如果当前没有选中节点则返回
	var paraTable,TrID,l_array,oTr,oNode,sReturn,oPara;
	var para_id,modu_id,modu_title,modu_description

	if (StrLastNode=="")
		return;
    //modify by hhy on 2002-06-10
	//SwitchPage("property");
    //end modify by hhy on 2002-06-10

	//假设得到相关的资料,并建立一个新节点
	para_id=StrLastNode.substr(0,StrLastNode.length-6);
	AddToDB(para_id);
}

//向数据库添加一条记录,返回新的记录的NodeValue
function AddToDB(a_paraid)
{

	if (a_paraid == '0') {a_paraid='';}
	document.all.item("HNodeValue").value = a_paraid;
	document.all.item("HNodeName").value = StrNewNodeName;
	document.all.item("HNodeDesc").value = StrNewNodeDesc;
	//ModuleMain.target="_blank";
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
//==这里结束 为了实现树形显示，对树模型对象进行适当的扩充的代码
	function Layout()
	{
	var l_moduID;

		if (StrLastNode=="" || StrLastNode=="0_Value") { alert("请先选择具体的模块");return;}
	l_moduID = StrLastNode.substr(0,StrLastNode.length-6);
	//0 : 修改 1 :新增
	window.main.location="../structure/DragAdmin.jsp?modu_id=" + l_moduID + "&modu_type=" + '<%=const_modu_private%>';

	}

	//得到对应的功能列表
	function GetFuncList()
	{
	var l_moduID,l_nodeName,oNode;
	var l_Nodetype,l_modu_type,l_desc

	if (StrLastNode=="" || StrLastNode=="0_Value") { alert("请先选择具体的模块");return;}
	l_moduID = StrLastNode.substr(0,StrLastNode.length-6);
	//0 : 修改 1 :新增

	oNode = oTree.Find(l_moduID);
	l_desc = oNode.array.Find("cate_desc");

	window.main.location="../structure/funcList.jsp?modu_id=" + l_moduID +
						 "&modu_type=" + '<%=const_modu_private%>' +
						 "&Title=" + l_desc;

	}

//-->
</script>

<script LANGUAGE="javascript">
<!--
var oTree,ifExpandTree = 1;
oTree = new tTree();
oArray = new tArray();
oArray.Add('cate_desc','<%=m_username%>个人模块');
oArray.Add('cate_value','0');
oTree.Add('0',null,oArray);
<%=m_sMenuString%>


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
			GetNewPage( StrLastNode);
		else
			window.main.location = "ModuleDetails.jsp";
	}
	else if(sToolBar == "funclist")
	{
		GetFuncList();
	}
	else
	{
		Layout();
	}
}

function window_onload()
{
	var oNode;
//	ContextMenu.intializeContextMenu(); //初始化右键菜单，只需要在页面onload的时候调用一次
	ShowTree();
	DivResize();

	SwitchPage("property");

	//展开树的第一级
	oNode = oTree.Find('0');
	if (oNode!=null){
		oNode.HideTable();
	}
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

var m_DetailBoardID = '<%=request.getParameter("DetailbaordID")%>';
var m_DetailBoardType = '<%=request.getParameter("DetailBoardType")%>';
window.onresize=DivResize;

window.attachEvent("onload",window_onload);
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
						<td>个人模块管理</td>
						<td width="20"><img id="funclist" alt="功能列表" src="../../images/go.gif" class="image" onclick="JavaScript:GetFuncList();" WIDTH="16" HEIGHT="16"></td>
						<td width="20"><img id="go" alt="设置功能" src="../../images/showtree.gif" class="image" onclick="JavaScript:Layout();" WIDTH="16" HEIGHT="16"></td>
						<td width="20"><img id="Refresh" alt="刷新" src="../../images/refresh.gif" class="image" onclick="JavaScript:RefreshPage()" WIDTH="11" HEIGHT="11"></td>
						<td width="20"><img id="AddNew" alt="新增" src="../../images/Plus.gif" onclick="JavaScript:OnAddNew();" class="image" WIDTH="11" HEIGHT="11"></td>
						<td width="20"><img id="Delete" alt="删除" src="../../images/minus.gif" onclick="JavaScript:OnDelete();" class="image" WIDTH="11" HEIGHT="11"></td>
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
						AddToolBarItemEx("refresh","../../images/refresh.gif","","","刷新","60","1","RefreshPage()");
						AddToolBarItemEx("addnew","../../images/createfolder.gif","","","新增模块","60","1","OnAddNew()");
						AddToolBarItemEx("delete","../../images/delete.gif","","","删除模块","60","0","OnDelete()");
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
