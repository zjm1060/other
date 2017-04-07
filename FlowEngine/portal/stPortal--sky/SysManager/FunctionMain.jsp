<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<%@ page language="java" import="java.util.*,java.sql.*,suntek.portal.publicLib.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    jbportal.FieldInfo  finfo=new jbportal.FieldInfo();
    finfo.setDEPTID("func_id");
    finfo.setDEPTNAME("title");
    finfo.setDEPTPARENTID("modu_id");
    finfo.setRECVALUE("func_id");
    finfo.setTOPVALUE("");
    finfo.setLOWFLAG("low_flag");//是否子节点
    finfo.setSELECTNAME("our_select");
    finfo.setMODUTYPE("Modu_Type");

    RecordSetObject m_rsAll=null;
    String m_sMenuString="";
    String m_sSQL = "(SELECT modu_def.modu_id AS func_id, modu_def.para_id AS modu_id," +
                          "modu_def.Title,modu_def.DashBoardOrder AS seq,0 AS modu_type" +
                          " FROM  modu_def Where para_id is null " +
                          ") union (Select func_id,modu_id,Title,0 as seq,1 as modu_type from func_def) order by modu_id asc,seq asc";
	m_rsAll = oManager.RunQuery("","","","",true,false,m_sSQL);
    try{
      m_sMenuString = jbportal.PortalUtil.FillClass(m_rsAll,"NULL",finfo);
    }
    catch(Exception e){
      throw e;
    }
%>
<html>
<title>功能管理</title>
<link rel="STYLESHEET" type="text/css" href="../include/tree/css/WebFX-ContextMenu.css">
<link rel="STYLESHEET" type="text/css" href="../include/tree/css/Node.css">
<link rel="STYLESHEET" type="text/css" href="../css/WebToolBar/toolbar.css">
<script LANGUAGE="javascript" src="../include/webtoolbar.js"></script>
<script LANGUAGE="javascript" src="../include/tree/ContextMenu.js"></script>
<script LANGUAGE="javascript" src="../include/tree/tTree.js"></script>
<script LANGUAGE="javascript" src="../include/tree/PortalTree.js"></script>
<script LANGUAGE="javascript">
<!--
//图片路径常量
var StrLeafImage = "<img src=\"../images/line.gif\" border=0>";
var StrLeafImage1 = "<img src=\"../images/line1.gif\" border=0>";
var StrEmptyImage = "<img src=\"../images/dot.gif\" border=0 style=\"width:15px\">";

var StrCloseImage = "<img src=\"../images/add.gif\" border=\"0\">";
var StrExpandImage = "<img src=\"../images/sub.gif\" border=\"0\">";

var StrCloseImageTop = "<img src=\"../images/top.gif\" border=\"0\">";
var StrExpandImageTop = "<img src=\"../images/-top.gif\" border=\"0\">";
var StrCloseImageBottom = "<img src=\"../images/bottom.gif\" border=\"0\">";
var StrExpandImageBottom = "<img src=\"../images/-bottom.gif\" border=\"0\">";
var StrCloseImageLeft = "<img src=\"../images/left.gif\" border=\"0\">";
var StrExpandImageLeft = "<img src=\"../images/-left.gif\" border=\"0\">";

var StrExpandImage2 = "<img src=\"../images/open.gif\" border=\"0\">";
var StrCloseImage2 = "<img src=\"../images/close.gif\" border=\"0\">";
var StrFucntionImage = "<img src=\"../images/function.gif\" border=\"0\">";
var StrBackground="../images/line-bg.gif";
var StrNewNodeName="新功能";
var StrNewNodeDesc="新功能";

var StrStatus="";
var sMsg="";

//处理节点的点击功能
function GetNewPage(strNodeID){
	//得到节点代码
	var l_NodeValue,oNode;

	l_NodeValue = strNodeID.substr(0,strNodeID.length-6);

	//0 : 修改 1 :新增
	window.main.location="FunctionDetails.jsp?m_NodeValue=" + l_NodeValue + "&m_Status=" + StrStatus ;
}


//设置节点的焦点
function SetDBFocus(strNodeID)
{
	if (StrLastNode != "")
	{
		//try{
			document.all.item(StrLastNode).style.fontWeight = 'normal';
		//}
	}
	document.all.item(strNodeID).style.fontWeight = 'bold';
	StrLastNode = strNodeID;
	GetNewPage(strNodeID);
}
/*删除当前选中的节点
*/
function OnDelete(){
                //假设得到相关的资料,并建立一个新节点
                para_id=StrLastNode.substr(0,StrLastNode.length-6);
                  //alert(para_id);
                //判断节点类型是否是0--表示是模块
                oNode=oTree.Find(para_id);
                  //alert(para_id);
                if (oNode==null){return;}

                sModutype = oNode.array.Find("cate_type");

                  //alert(para_id);
                if (sModutype=="0") {alert("对不起，不能删除模块！"); return;}
		var TrID,NodeName,NodeDesc;
		var oNode,bReturn;

		if (StrLastNode != ""&&StrLastNode!="0_Value"){

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
			//从数据库删除
			document.all.item("HNodeValue").value = NodeName;
			document.all.item("NodeDesc").value = NodeDesc;
			ModuleMain.target="update";
			ModuleMain.action="_DelConfirm.jsp?m_Operation=del";
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
	var para_id,modu_id,modu_title,modu_description,sModutype

	if (StrLastNode=="")
		return;
	if (StrLastNode.substr(0,2) == '0_'){
		alert("控制台不能新增模块，请选择下面的模块！");
		return;
	}

	//假设得到相关的资料,并建立一个新节点
	para_id=StrLastNode.substr(0,StrLastNode.length-6);
          //alert(para_id);
	//判断节点类型是否是0--表示是模块
	oNode=oTree.Find(para_id);
          //alert(para_id);
	if (oNode==null){return;}

	sModutype = oNode.array.Find("cate_type");

          //alert(para_id);
	if (sModutype=="1") {alert("对不起，只能在模块下添加功能！"); return;}
	//
	AddToDB(para_id);
}

//向数据库添加一条记录,返回新的记录的NodeValue
function AddToDB(a_paraid)
{
	document.all.item("HNodeValue").value = a_paraid;
	document.all.item("HNodeName").value = StrNewNodeName;
	document.all.item("HNodeDesc").value = StrNewNodeDesc;
	ModuleMain.target="update";
	ModuleMain.action="_FunctionModi.jsp?m_Operation=add";
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
	//得到新的节点号
	modu_id=a_NewNodeID;
	modu_title=StrNewNodeName;
	modu_description=StrNewNodeDesc;


	l_array = new tArray();
	l_array.Add('cate_desc', modu_description);
	l_array.Add('cate_value',modu_id);
	l_array.Add('cate_para',para_id);
	l_array.Add('cate_type',"1");

	sReturn=oTree.Add(modu_id,para_id,l_array);


	//得到当前tr对象
	TrID= para_id  + "_tr";

	//paraTable=document.all.item(TrID).parentElement.parentElement;
	//先在外面的大Table增加一个<TR>
	paraTable=document.all.item(para_id+"_childrentable");

	oTr=paraTable.insertRow();
	oTr.id = modu_id+"_tr";
          //alert('id=' + modu_id);
	//得到当前节点
	oNode = oTree.Find(modu_id);

	//在当前节点下增加一个子节点，默认为newModule
	GetTD1(oNode,oTr);

	StrStatus="true";
	SetDBFocus(modu_id + "_Value");
	StrStatus="";
	paraTable.style.display="";

	oTree.AdjustPic();
}

//如果新增或者删除发生错误则显示错误
function MsgError(a_Error)
{
	alert('错误'+ a_Error);
}
//取得公用功能的权限列表。
function GetFuncRightList(){
  var l_moduID,l_nodeName,oNode;
  if (StrLastNode=="" || StrLastNode=="0_Value") { alert("请先选择具体的功能。");return;}
  //假设得到相关的资料,并建立一个新节点
  para_id=StrLastNode.substr(0,StrLastNode.length-6);
    //alert(para_id);
  //判断节点类型是否是0--表示是模块
  oNode=oTree.Find(para_id);
  if (oNode==null){return;}
  sModutype = oNode.array.Find("cate_type");
  if (sModutype=="0") {alert("对不起，请先选择具体的功能！"); return;}

  l_moduID = StrLastNode.substr(0,StrLastNode.length-6);
  window.main.location="funcRightList.jsp?func_id=" + l_moduID ;
}
function GetTD1(oNode,oTr)
{
	var l_name,l_node,l_return = "";
	var oTD,l_HTML;

	//虚线竖线
	l_node = oNode.paraNode;
	if ( l_node != null )
	{
		//插入一个TD
		oTD = oTr.insertCell();
		oTD.width = "18px";
		if ( document.all.item(oNode.nodeName + "_vline") != null )
		{
			alert(document.all.item(oNode.nodeName + "_vline").outerHTML);
		}
		oTD.id = oNode.nodeName + "_vline";
	}

	oTD=oTr.insertCell();

	oTD.width="4px";
	oTD.innerHTML="&nbsp;"

	oTD=oTr.insertCell();

	l_HTML = GetTD3(oNode);
	oTD.innerHTML=l_HTML;
}

//==这里结束 为了实现树形显示，对树模型对象进行适当的扩充的代码

//-->
</script>


<script LANGUAGE="javascript">
<!--
var oTree,ifExpandTree = 1;
oTree = new tTree();
oArray = new tArray();
oArray.Add('cate_desc','控制台');
oArray.Add('cate_value','0');
oArray.Add('cate_type',"0")
oTree.Add('0',null,oArray);
<%=m_sMenuString%>

function window_onload()
{
	var oNode;
//	ContextMenu.intializeContextMenu(); //初始化右键菜单，只需要在页面onload的时候调用一次
	ShowTree();
	DivResize();

	//展开树的第一级
	oNode = oTree.Find('0');
	if (oNode!=null){
		oNode.HideTable();
	}

	window.main.location="FunctionDetails.jsp";
}

function ShowTree()
{

	ifExpandTree = ( ifExpandTree == 1 ) ? 0 : 1; //切换是否展开所有结点的标志
	document.all.item('divRender').innerHTML = oTree.RenderTable(ifExpandTree);
}

function DivResize(){
	//改变显示区的大小
	document.all.item('divRender').style.height = document.body.offsetHeight -10;
}

window.onresize=DivResize;

window.attachEvent("onload",window_onload);
//-->



</script>
<!--在这里Base的作用是设置缺省target -->
<link rel="STYLESHEET" type="text/css" href="../include/Public.css">

<body leftmargin="0" topmargin="0">
<table border="1" height="100%">
<tr>
	<td height="100%" valign="top">
		<form ACTION="ModuleMain.jsp" METHOD="post" id="ModuleMain" name="ModuleMain">
			<!--div id="TitleDiv" style="height:12">  By Wu yangfei 2001/11/20
				<table class="menutitle" width="100%" cellspacing="0" cellpadding="0" border="0">
					<tr>
						<td>功能管理</td>
						<td width="20"><img id="Refresh" alt="刷新" src="../images/refresh.gif" class="image" onclick="JavaScript:RefreshPage()" WIDTH="11" HEIGHT="11"></td>
						<td width="20"><img id="AddNew" alt="新增" src="../images/Plus.gif" onclick="JavaScript:OnAddNew();" class="image" WIDTH="11" HEIGHT="11"></td>
						<td width="20"><img id="Delete" alt="删除" src="../images/minus.gif" onclick="JavaScript:OnDelete();" class="image" WIDTH="11" HEIGHT="11"></td>
					</tr>
				</table>
			</div-->

			<div id="divDebug"></div>
			<div id="divRender" style="width:200;overflow-x:auto;overflow-y:auto;"></div>
			<input type="hidden" name="HNodeValue" value>
			<input type="hidden" name="HNodeName" value>
			<input type="hidden" name="HNodeDesc" value>
			<input type=hidden name="NodeDesc" value>
		</form>
	</td>

	<td width="100%">
		<table border="1" width="100%" height="100%">
			<tr>
				<td>
					<div id="toolbar"></div>
					<script language="javascript">
						SetToolBarHeight(40);
						AddToolBarItemEx("refresh","../images/refresh.gif","","","刷新","60","0","RefreshPage()");
                        AddToolBarItemEx("funcrightlist","../images/go.gif","","","权限","60","0","GetFuncRightList()");
						AddToolBarItemEx("addnew","../images/createfolder.gif","","","新增功能","60","1","OnAddNew()");
						AddToolBarItemEx("delete","../images/delete.gif","","","删除功能","60","0","OnDelete()");
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


