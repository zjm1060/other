<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<%@ page import="com.minstone.portal.PortalConstants" %>
<%@ page import="com.minstone.util.database.RecordSetObject" %>
<%@ page import="java.util.*,java.sql.*,jbportal.PortalUtil" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
    jbportal.FieldInfo  finfo=new jbportal.FieldInfo();
    String addtype=request.getParameter("addtype");
    String m_sMenuString = "";
    RecordSetObject m_rsAll = null;
    String m_username = "";

    //加入部门列表
    finfo.setDEPTID("FRAM_CODE");
    finfo.setDEPTNAME("DEPT_NAME");
    finfo.setDEPTPARENTID("PARA_CODE");
    finfo.setRECVALUE("DEPT_CODE");
    finfo.setTOPVALUE("");
    finfo.setLOWFLAG("low_flag");//是否子节点
    finfo.setSELECTNAME("our_select");
    finfo.setMODUTYPE("type");

    m_rsAll=oManager.getUserDeptGroupInfo(PortalConstants.DEPARTMENT);
    try
      {
        m_sMenuString = PortalUtil.FillClass(m_rsAll,"NULL",finfo);
      }
      catch(Exception e)
      {
         throw e;
      }

    //加入用户组列表
    finfo.setDEPTID("GROP_CODE");
    finfo.setDEPTNAME("GROP_NAME");
    finfo.setDEPTPARENTID("type");
    finfo.setRECVALUE("GROP_CODE");
    finfo.setTOPVALUE("");
    finfo.setLOWFLAG("low_flag");//是否子节点
    finfo.setSELECTNAME("our_select");
    finfo.setMODUTYPE("type");
    m_rsAll=oManager.getUserDeptGroupInfo(PortalConstants.GROUP);
    try
      {
        m_sMenuString += PortalUtil.FillClass(m_rsAll,"2",finfo);
      }
      catch(Exception e)
      {
         throw e;
      }

    //加入用户列表
    finfo.setDEPTID("USER_CODE");
    finfo.setDEPTNAME("USER_NAME");
    finfo.setDEPTPARENTID("FRAM_CODE");
    finfo.setRECVALUE("USER_CODE");
    finfo.setTOPVALUE("");
    finfo.setLOWFLAG("low_flag");//是否子节点
    finfo.setSELECTNAME("our_select");
    finfo.setMODUTYPE("type");
    m_rsAll=oManager.getUserDeptGroupInfo(PortalConstants.USER);
    try
      {
        m_sMenuString += PortalUtil.FillClass(m_rsAll,"-",finfo);
      }
      catch(Exception e)
      {
         throw e;
      }
    String modu_id=session.getAttribute("modu_id").toString();
    Vector userlist=new Vector();
    session.setAttribute("userlist",userlist);
%>
<html>
<META HTTP-EQUIV="pragma" CONTENT="no-cache">
<title>添加用户权限</title>
<link rel="STYLESHEET" type="text/css" href="../include/tree/css/WebFX-ContextMenu.css">
<link rel="STYLESHEET" type="text/css" href="../include/tree/css/Node.css">
<link rel="STYLESHEET" type="text/css" href="../css/WebToolBar/toolbar.css">
<script type="text/javascript" src="../include/tree/ContextMenu.js"></script>
<script type="text/javascript" src="../include/webtoolbar.js"></script>
<%
//文件tTree.js是有关树模型对象的一些常用的前台方法
	String l_first=null;

	l_first = "oArray = new tArray();" ;
	l_first = l_first + "oArray.Add('cate_desc','组织架构');" ;
	l_first = l_first + "oArray.Add('cate_value','-1');" ;
	l_first = l_first + "oTree.Add('-1',null,oArray);"  ;
	l_first = l_first +" oArray = new tArray();" ;
	l_first = l_first + "oArray.Add('cate_desc','部门/用户列表');" ;
	l_first = l_first + "oArray.Add('cate_value','0');" ;
	l_first = l_first + "oTree.Add('0','-1',oArray);"  ;
	l_first = l_first +" oArray = new tArray();" ;
	l_first = l_first + "oArray.Add('cate_desc','用户组列表');" ;
	l_first = l_first + "oArray.Add('cate_value','2');" ;
	l_first = l_first + "oTree.Add('2','-1',oArray);"  ;
    /*
	l_first = l_first +" oArray = new tArray();" ;
	l_first = l_first + "oArray.Add('cate_desc','用户列表');" ;
	l_first = l_first + "oArray.Add('cate_value','1');" ;
	l_first = l_first + "oTree.Add('1','-1',oArray);"  ;
    */
	l_first = l_first +" oArray = new tArray();" ;
	l_first = l_first + "oArray.Add('cate_desc','EveryOne(所有用户)');" ;
	l_first = l_first + "oArray.Add('cate_value','AllPortalUser');" ;
	l_first = l_first + "oArray.Add('cate_type','2');" ;
	l_first = l_first + "oTree.Add('AllPortalUser','-1',oArray);"  ;

	m_sMenuString = l_first + m_sMenuString;

%>
<script type="text/javascript" src="../include/tree/tTree.js"></script>
<script type="text/javascript">
<!--
//==这里开始 为了实现树形显示，对树模型对象进行适当的扩充的代码

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

var StrExpandImage2 = "<img src=\"../images/personalopen.gif\" border=\"0\">";
var StrCloseImage2 = "<img src=\"../images/personal.gif\" border=\"0\">";
var StrFucntionImage = "<img src=\"../images/personal.gif\" border=\"0\">";
var StrBackground="../images/line-bg.gif";
//当前节点的值
var StrLastNode="";
var StrNewNodeName="新功能";
var StrNewNodeDesc="新功能";

var StrStatus="";
var sMsg="";
//--树结点类-------------------------------------------------------------

function tNode(a_NodeName,a_ParaName,a_ParaNode,a_tAry,a_Tree)
{
	this.nodeName = a_NodeName; //结点代码
	this.paraName = a_ParaName; //结点父代码
	this.paraNode = a_ParaNode; //结点的父结点
	this.array = a_tAry; //其他参数，例如结点的描述等，放在一个tArray增强型数组里
	this.tree = a_Tree; //结点所在的tTree类树对象
	this.children = new tArray(); //结点的儿子集合，是一个tArray类对象

	this.Update = tNodeUpdate; //更新一个结点的方法
	this.toString = tNodetoString; //Debug用的打出结点信息的方法
	this.isLeaf = tNodeisLeaf; //判断本结点是否叶子结点的方法
	this.Layer = tNodeLayer; //返回本结点在树中的第几层，从0开始计数
	this.Index = tNodeIndex; //返回在同一层中本结点是第几个结点，从0开始计数
	this.BrotherCount = tNodeBrotherCount; //返回同一层中有多少个兄弟结点，包括自己

	//上面的是通用的，下面的是为了实现树形显示而写的
	this.RenderTable = tNodeRenderTable;
	this.HideTable = tNodeHideTable;
	this.ExpandImage = tNodeExpandImage;
	this.ExpandNode = tExpandNode;
	this.OnClick = tNodeOnClick;
	this.OnContextMenu = tNodeOnContextMenu;
	this.VLine = tNodeVLine;
	this.AdjustPic = tNodeAdjustPic;
}

/*
function tNodeVLine()
功能：调整一个结点的前面应该放什么虚线style
输入：
输出：
*/
function tNodeVLine()
{
	var l_td = document.all.item(this.nodeName + "_vline");
	if ( l_td == null ){
		return;
	}
	if (!( this.paraNode != null
			&& this.paraNode.Index() == ( this.paraNode.BrotherCount() -1 )))
	{
		l_td.style.backgroundPositionX = "4px";
		l_td.style.backgroundImage = "url(../images/line-bg.gif)";
		l_td.style.backgroundRepeat = "repeat-y";
	}
}

//调整结点的加碱号、虚线、文件夹图片
function tNodeAdjustPic(a_expand)
{
	var l_tb = document.all.item(this.nodeName + '_childrentable');
	var l_expand = document.all.item(this.nodeName + "_expand");
	l_dis = l_tb.style.display;

	l_folder = document.all.item(this.nodeName + '_folder');

	l_expand.style.cursor = ( this.children.count > 0 ) ? "hand" : "";

	if ( l_dis == '' && this.children.count > 0 )
	{
		l_expand.innerHTML = this.ExpandImage(1);
		if (this.array.Find('ExpandImage') != null ) //如果有自定义的展开图片就用
			l_folder.innerHTML = this.array.Find('ExpandImage');
		else //否则用缺省的展开图片
			l_folder.innerHTML = StrExpandImage2;
	}else{
		l_expand.innerHTML = this.ExpandImage(0);
		if (this.array.Find('CloseImage') != null ) //如果有自定义的收缩图片就用
			l_folder.innerHTML = this.array.Find('CloseImage');
		else //否则用缺省的收缩图片
			if (this.array.Find("cate_type") =="0")
				{l_folder.innerHTML = StrCloseImage2;}
			else
				{l_folder.innerHTML = StrFucntionImage;}
	}

	this.VLine();
}

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

//结点的OnClick事件
function tNodeOnClick()
{
	var elem = window.event.srcElement;
	var strNodeID;

	strNodeID=elem.id;
	SetDBFocus(strNodeID);
    if (strNodeID!="" && strNodeID!="2^undefined^用户组列表"&&strNodeID!="0^undefined^部门/用户列表"&&strNodeID!="1^undefined^用户列表"&&strNodeID!="-1^undefined^组织架构"){
      url="AddUserList.jsp?addtype=<%=request.getParameter("addtype")%>";
      url=url+"&amp;adduser="+strNodeID;
      url=URLEncoding(url);
      window.userlist.location=url;
    }
}

//处理节点的点击功能
function GetNewPage(strNodeID){
	//得到节点代码
	var l_NodeValue,oNode;

	l_NodeValue = strNodeID.substr(0,strNodeID.length-6);

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


/*
function tNodeHideTable()
功能：处理点击一个结点前面的加号、减号的事件
输入：
输出：
*/
function tNodeHideTable()
{
  var l_tb,l_dis,l_expand,l_folder;
  l_tb = document.all.item(this.nodeName + '_childrentable');
  l_dis = l_tb.style.display;
  l_expand = document.all.item(this.nodeName + '_expand');
  l_folder = document.all.item(this.nodeName + '_folder');
  if ( l_dis == 'none' )
  {
    this.ExpandNode();
    l_tb.style.display = '';
    l_expand.innerHTML = this.ExpandImage(1,0);
    l_folder.innerHTML = StrExpandImage2;
  }else{
    l_tb.style.display = 'none';
    l_expand.innerHTML = this.ExpandImage(0,0);
    if (this.array.Find("cate_type")=="0")
    {l_folder.innerHTML = StrCloseImage2;}
    else
    {l_folder.innerHTML = StrFucntionImage;}
  }
}

/*
function tNodeExpandImage(a_expand)
功能：判断一个结点的前面应该放哪个加号、减号的图片
输入：
	a_expand：本结点是否展开状态，1：展开，0：收缩
输出：
	应该放的图片的字符串
*/
function tNodeExpandImage(a_expand)
{
	//加减号图片
	if ( this.children.count > 0 ) //非叶子结点
	{
		if ( this.Layer() == 0 && this.BrotherCount() == 1 ) //第一层中只有一个结点
			return (a_expand == 1) ? StrExpandImage : StrCloseImage;
		else if ( this.Index() == 0 && this.Layer() == 0 ) //第一层的第一个结点
			return (a_expand == 1) ? StrExpandImageTop : StrCloseImageTop;
		else if ( this.Index() == this.BrotherCount() - 1 ) //一层中的最后一个结点
			return (a_expand == 1) ? StrExpandImageBottom : StrCloseImageBottom;
		else
			return (a_expand == 1) ? StrExpandImageLeft : StrCloseImageLeft; //一般的结点
	}else{ //叶子结点
		if ( this.Index() == this.BrotherCount() - 1 ) //一层中的最后一个结点
			return StrLeafImage1;
		else
			return StrLeafImage; //非一层中的最后一个结点
	}
}

/*
function tNodeRenderTable()
功能：构造一个结点的界面，返回html字符串
输入：
输出：
	本结点的html字符串
*/
function tNodeRenderTable()
{
	var l_name,l_node,l_return = "";
	l_return += "<tr id='" + this.nodeName + "_tr'>\n";

	//虚线竖线
	l_node = this.paraNode;
	if ( l_node != null )
	{
		l_return += "<td id='" + this.nodeName + "_vline' style=\"width:18px;";
	if ( ! ( this.paraNode != null
			&& this.paraNode.Index() == ( this.paraNode.BrotherCount() -1 )
			)
		)
	{
		l_return += "background-position-x:4px;background-repeat:repeat-y;background-image:url("+StrBackground+");";
		l_return += "\">";
		l_return +="&nbsp;</td>\n";
	}
	else
	{
		l_return += "\">&nbsp;</td>\n";
	}
	}

	l_return += "<td style=\"width:4px\"></td>\n";

	l_return += "<td>\n";

	l_return += "<table border=0 cellspacing=0 cellpadding=0>";
	l_return += "<tr >";

	l_return += "<td style=\"width:0%\">";
	l_return += "<span id='" + this.nodeName + "_expand'";
	l_return += " onclick=\"javascript:oTree.Find('" + this.nodeName + "').HideTable();\"";

	//加减号图片
	if ( this.children.count > 0 ) //非叶子结点
	{
		l_return += " style=\"cursor:hand;width:18px\">";
	}else{ //叶子结点
		l_return += " style=\"width:18px\">";
	}
	l_return += this.ExpandImage(this.tree.ShowOnRender);
	l_return += "</span>";
	l_return += "</td >";

	l_return += "<td style=\"width:0%\">";

	//文件夹图片
	l_return += "<span id='" + this.nodeName + "_folder'>";
	if (this.array.Find('cate_type')==0)
	{	l_return += StrCloseImage2;}
	else
	{	l_return += StrFucntionImage;}
	l_return += "</span>";

	l_return += "</td>";
	l_return += "<td align=left>";

	//分类的描述
	l_return += "<div nodeName=\"" + this.nodeName + "\"";
	l_return += " onmousedownex=\"javascript:oTree.Find('" + this.nodeName + "').onmousedownex();\"";
	l_return += " onmouseupex=\"javascript:oTree.Find('" + this.nodeName + "').onmouseupex();\"";
	l_return += " id='" + this.nodeName + "_Move'";
	l_return += " class=moveme style=\"position:relative;\">";
	l_return += "<input id='" + this.nodeName+"^"+this.array.Find('cate_type')+"^"+this.array.Find('cate_desc') + "' type=text readonly style=\"height:13px;cursor:hand;border:0\"";
	l_return += " onclick=\"javascript:oTree.Find('" + this.nodeName + "').OnClick();\"";
	l_return += " oncontextmenu=\"javascript:oTree.Find('" + this.nodeName + "').OnContextMenu()\"";
	l_return += " class=\"node\"";
	l_return += " value=\"";
	l_return += this.array.Find('cate_desc');
	l_return += "\"></input>";
	l_return += "</div>";

	l_return += "</td>";

	l_return += "</tr>";

	//子分类的table
	l_return += "<tr>";

	l_return += "<td colspan=3>";
	l_return += "<table border=0 cellspacing=0 cellpadding=0 width=\"100%\"";
	if ( this.tree.ShowOnRender == 0 )
		l_return += " style=\"display:none\"";
	l_return += " id='" + this.nodeName + "_childrentable'>\n";
    if(this.tree.ShowOnRender==1 && this.Layer()==0){
      for ( l_name in this.children.nodes )
      {
        l_node = this.children.nodes[l_name];
        l_return += l_node.RenderTable();
      }
    }
	l_return += "</table>\n";
	l_return += "</td>";
	l_return += "</tr>";
	l_return += "</table>";
	//结束
	l_return += "</td>\n";
	l_return += "</tr>\n";
	return l_return;
}
/**
 * 生成子节点
 * @return
 */
function tExpandNode(){
  var l_return = "";
  var l_ctbl = document.all.item(this.nodeName + '_childrentable');
  if(!this.isRender){
    l_return += "<table border=0 cellspacing=0 cellpadding=0 width=\"100%\"";
    l_return += " id='" + this.nodeName + "_childrentable'>\n";

    for ( l_name in this.children.nodes )
    {
            l_node = this.children.nodes[l_name];
            l_return += l_node.RenderTable();
    }

    l_return += "</table>\n";

    l_ctbl.outerHTML = l_return;
    l_ctbl.style.display = '';
    this.isRender = true;
  }
}
//--树类-------------------------------------------------------------

function tTree() //构造树类
{
	this.nodes = new tArray(); //树的结点集合
	this.count = tTreeCount; //树的结点个数
	this.Add = tAddNode; //增加一个结点的方法
	this.Find = tFindNode; //根据代码找到一个结点的方法
	this.Remove = tRemoveNode; //删除一个结点的方法
	this.Update = tUpdateNode; //更新一个结点的方法
	this.toString = tTreetoString; //用于Debug时，返回树的信息的方法

//上面的是通用的，下面的是为了实现树形显示而写的
	this.ShowOnRender = 0; //缺省树的开始状态是不展开的
	this.RenderTable = tRenderTable;
	this.AdjustPic = tAdjustPic;
	this.UpdateEx = tUpdateNodeEx;
	this.ExpandNode = tExpandNode;
}

function tUpdateNodeEx(a_nodeName,a_paraName,a_ary)
{
	var l_tAry = new tArray(), l_s;
	for ( l_s in a_ary )
	{
		l_tAry.Add( l_s, a_ary[l_s] );
	}
	this.Update(a_nodeName,a_paraName,l_tAry);
}

function tAdjustPic()
{
	var l_node;
	for ( l_node in this.nodes.nodes )
	{
		this.nodes.Find(l_node).AdjustPic();
	}
}

/*
function tRenderTable(a_ShowOnRender)
功能：构造一棵树的界面，返回html字符串
输入：
	a_ShowOnRender：是否展开整棵树，1：展开，0：收缩
输出：
	本树的html字符串
*/
function tRenderTable(a_ShowOnRender)
{
	var l_name,l_node,l_return = "";

	this.ShowOnRender = a_ShowOnRender;

	l_return += "<table ID=oTable border=0 cellspacing=0 cellpadding=0>\n";
	for ( l_name in this.nodes.nodes )
	{
		l_node = this.nodes.nodes[l_name];
		if ( l_node.paraNode != null )
			continue;
		l_return += l_node.RenderTable();
	}
	l_return += "</table>\n";
	return l_return;
}


//==这里结束 为了实现树形显示，对树模型对象进行适当的扩充的代码

//-->
</script>


<script type="text/javascript">
<!--
var oTree,ifExpandTree = 1;
oTree = new tTree();
<%=m_sMenuString%>

function window_onload()
{
	var oNode;
//	ContextMenu.intializeContextMenu(); //初始化右键菜单，只需要在页面onload的时候调用一次
	ShowTree();
	DivResize();

	//展开树
	oNode = oTree.Find('-1');
	if (oNode!=null){
		oNode.HideTable();
	}
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
<SCRIPT type="text/VBScript">
Function URLEncoding(vstrIn)
  strReturn = ""
  For t = 1 To Len(vstrIn)
  ThisChr = Mid(vStrIn,t,1)
  If Abs(Asc(ThisChr)) < &HFF Then
  strReturn = strReturn & ThisChr
  Else
  innerCode = Asc(ThisChr)
  If innerCode < 0 Then
  innerCode = innerCode + &H10000
  End If
  Hight8 = (innerCode And &HFF00)\ &HFF
  Low8 = innerCode And &HFF
  strReturn = strReturn & "%" & Hex(Hight8) & "%" & Hex(Low8)
  End If
  Next
  URLEncoding = strReturn
  End Function
</script>
<link rel="STYLESHEET" type="text/css" href="../include/Public.css">

<body leftmargin="0" topmargin="0">
<table border="1" height="100%" width=100%>
<tr>
	<td height="100%" width=30% valign="top">
          <div id="divDebug"></div>
          <div id="divRender" style="width:230;overflow-x:auto;overflow-y:auto;"></div>
	</td>
        <td valign="top" width=70%>
          <iframe name="userlist" FRAMEBORDER=1 width=100% height=100% src="AddUserList.jsp">
          </iframe>
        </td>
</tr>
</table>

</body>
</html>


