<link rel="STYLESHEET" type="text/css" href="css/WebFX-ContextMenu.css">
<SCRIPT type="text/javascript" src="ContextMenu.js"></SCRIPT>
<%
//文件tTree.js是有关树模型对象的一些常用的前台方法
%>
<SCRIPT type="text/javascript" src="tTree.js"></SCRIPT>
<script type="text/javascript">
<!--
//==这里开始 为了实现树形显示，对树模型对象进行适当的扩充的代码

//图片路径常量
var StrLeafImage = "<img src=\"images/line.gif\" border=0>";
var StrLeafImage1 = "<img src=\"images/line1.gif\" border=0>";
var StrEmptyImage = "<img src=\"images/dot.gif\" border=0 style=\"width:15px\">";

var StrCloseImage = "<img src=\"images/add.gif\" border=\"0\">";
var StrExpandImage = "<img src=\"images/sub.gif\" border=\"0\">";

var StrCloseImageTop = "<img src=\"images/top.gif\" border=\"0\">";
var StrExpandImageTop = "<img src=\"images/-top.gif\" border=\"0\">";
var StrCloseImageBottom = "<img src=\"images/bottom.gif\" border=\"0\">";
var StrExpandImageBottom = "<img src=\"images/-bottom.gif\" border=\"0\">";
var StrCloseImageLeft = "<img src=\"images/left.gif\" border=\"0\">";
var StrExpandImageLeft = "<img src=\"images/-left.gif\" border=\"0\">";

var StrExpandImage2 = "<img src=\"images/open.gif\" border=\"0\"><span style=\"width:5px\"></span>";
var StrCloseImage2 = "<img src=\"images/close.gif\" border=\"0\"><span style=\"width:5px\"></span>";


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
	this.OnClick = tNodeOnClick;
	this.OnContextMenu = tNodeOnContextMenu;
}

//结点的右键菜单事件
function tNodeOnContextMenu()
{
   var popupoptions
   popupoptions = [
   						new ContextItem("刷新本页",function(){window.location.reload();}),
						new ContextItem("Disabled Item",null,true),
						new ContextSeperator(),
						new ContextItem("打印",function(){window.print()}),
						new ContextItem("查看结点代码",function(){tNodeOnClick();document.body.focus();}),
						new ContextSeperator(),
   						new ContextItem("哈哈",function(){alert('哈哈');document.body.focus();})
   				  ]
   ContextMenu.display(popupoptions)
}

//结点的OnClick事件
function tNodeOnClick()
{
	alert('你好，我的代码是 ' + this.nodeName + '。\n祝你好运 :)');
	self.status = "hello";
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
		l_tb.style.display = '';
		l_expand.innerHTML = this.ExpandImage(1);
		if (this.array.Find('ExpandImage') != null ) //如果有自定义的展开图片就用
			l_folder.innerHTML = this.array.Find('ExpandImage');
		else //否则用缺省的展开图片
			l_folder.innerHTML = StrExpandImage2;
	}else{
		l_tb.style.display = 'none';
		l_expand.innerHTML = this.ExpandImage(0);
		if (this.array.Find('CloseImage') != null ) //如果有自定义的收缩图片就用
			l_folder.innerHTML = this.array.Find('CloseImage');
		else //否则用缺省的收缩图片
			l_folder.innerHTML = StrCloseImage2;
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
		if ( this.BrotherCount() == 1 ) //一层中只有一个结点
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
	l_return += "<tr>\n";

	//虚线竖线
	l_node = this.paraNode;
	if ( l_node != null )
	{
		l_return += "<td style=\"width:18px;";
		if ( !( l_node.Layer() == 0 && l_node.Index() == (l_node.BrotherCount() - 1) ) )
		{
			l_return += "background-position-x:4px;background-repeat:repeat-y;background-image:url(images/line-bg.gif);";
		}
		l_return += "\"></td>\n";
	}

	l_return += "<td style=\"width:4px\"></td>\n";

	l_return += "<td>\n";

	l_return += "<table border=0 cellspacing=0 cellpadding=0>";
	l_return += "<tr>";

	l_return += "<td style=\"width:0%\">";
	l_return += "<span";

	//加减号图片
	if ( this.children.count > 0 ) //非叶子结点
	{
		l_return += " id='" + this.nodeName + "_expand' style=\"cursor:hand;width:18px\"";
		l_return += " onclick=\"javascript:oTree.Find('" + this.nodeName + "').HideTable();\">";
		l_return += this.ExpandImage(this.tree.ShowOnRender);
	}else{ //叶子结点
		l_return += " style=\"width:18px\">";
		l_return += this.ExpandImage(this.tree.ShowOnRender);
	}
	l_return += "</span>";
	l_return += "</td>";

	l_return += "<td>";

	//文件夹图片
	l_return += "<span id='" + this.nodeName + "_folder'>";
	l_return += StrCloseImage2;
	l_return += "</span>";

	//分类的描述
	l_return += "<a href=\"#\"";
	l_return += " onclick=\"javascript:oTree.Find('" + this.nodeName + "').OnClick();\"";
	l_return += " oncontextmenu=\"javascript:oTree.Find('" + this.nodeName + "').OnContextMenu()\"";
	l_return += ">";
	l_return += this.array.Find('cate_desc');
	l_return += "</a>";

	l_return += "</td>";

	l_return += "</tr>";

	//子分类的table
	l_return += "<tr>";

	l_return += "<td colspan=2>";
	l_return += "<table border=0 cellspacing=0 cellpadding=0 width=\"100%\"";
	if ( this.tree.ShowOnRender == 0 )
		l_return += " style=\"display:none\"";
	l_return += " id='" + this.nodeName + "_childrentable'>\n";

	for ( l_name in this.children.nodes )
	{
		l_node = this.children.nodes[l_name];
		l_return += l_node.RenderTable();
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

	l_return += "<table border=0 cellspacing=0 cellpadding=0>\n";
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

<SCRIPT type="text/javascript">
<!--
var oTree,ifExpandTree = 1;
oTree = new tTree();
<%
  String m_sMenuString = "oArray = new tArray(); oArray.Add('cate_desc','企业信息'); oArray.Add('cate_value','0012'); oTree.Add('0012','0',oArray); oArray = new tArray(); oArray.Add('cate_desc','移动新知'); oArray.Add('cate_value','00120001'); oTree.Add('00120001','0012',oArray); oArray = new tArray(); oArray.Add('cate_desc','行业动态'); oArray.Add('cate_value','00120002'); oTree.Add('00120002','0012',oArray); oArray = new tArray(); oArray.Add('cate_desc','公司信息'); oArray.Add('cate_value','00120003'); oTree.Add('00120003','0012',oArray);";
%>
<%=m_sMenuString%>

function window_onload()
{
	ContextMenu.intializeContextMenu(); //初始化右键菜单，只需要在页面onload的时候调用一次
	ShowTree();
}

function ShowTree()
{
//	document.all.item('divDebug').innerText = oTree.toString();
	ifExpandTree = ( ifExpandTree == 1 ) ? 0 : 1; //切换是否展开所有结点的标志
	document.all.item('divRender').innerHTML = oTree.RenderTable(ifExpandTree);
}

window.attachEvent("onload",window_onload);
//-->
</SCRIPT>
<body leftmargin="0" topmargin="0">
<a href="#" onclick="javascript:ShowTree();">看看我们的树罗</a>
<br>
<br>
<div id=divDebug></div>
<div id=divRender></div>
</body>
</html>
