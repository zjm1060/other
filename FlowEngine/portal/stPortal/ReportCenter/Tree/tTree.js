//==这里开始 为了实现树形显示，对树模型对象进行适当的扩充的代码

//图片路径常量
var StrLeafImage = "<img src=\"./Tree/images/line.gif\" border=0>";
var StrLeafImage1 = "<img src=\"./Tree/images/line1.gif\" border=0>";
var StrEmptyImage = "<img src=\"./Tree/images/dot.gif\" border=0 style=\"width:15px\">";

var StrCloseImage = "<img src=\"./Tree/images/add.gif\" border=\"0\">";
var StrExpandImage = "<img src=\"./Tree/images/sub.gif\" border=\"0\">";

var StrCloseImageTop = "<img src=\"./Tree/images/addtop.gif\" border=\"0\">";
var StrExpandImageTop = "<img src=\"./Tree/images/plustop.gif\" border=\"0\">";
var StrCloseImageBottom = "<img src=\"./Tree/images/addbottom.gif\" border=\"0\">";
var StrExpandImageBottom = "<img src=\"./Tree/images/plusbottom.gif\" border=\"0\">";
var StrCloseImageLeft = "<img src=\"./Tree/images/addleft.gif\" border=\"0\">";
var StrExpandImageLeft = "<img src=\"./Tree/images/plusleft.gif\" border=\"0\">";

var StrExpandImage2 = "<img src=\"./Tree/images/open.gif\" border=\"0\">";
var StrCloseImage2 = "<img src=\"./Tree/images/close.gif\" border=\"0\">";
var strReportImage="<img src=\"./Tree/images/report.gif\" border=\"0\">";

//当前节点的值
var StrLastNode="";

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
			l_folder.innerHTML = '<img src="' + this.array.Find('ExpandImage') + '" border=0>';
		else //否则用缺省的展开图片
			l_folder.innerHTML = StrExpandImage2;
	}else{
		l_tb.style.display = 'none';
		l_expand.innerHTML = this.ExpandImage(0);
		if (this.array.Find('CloseImage') != null ) //如果有自定义的收缩图片就用
			l_folder.innerHTML = '<img src="' + this.array.Find('CloseImage') + '" border=0>';
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
	l_return += "<tr>\n";

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
		l_return += "background-position-x:4px;background-repeat:repeat-y;background-image:url(../common/tree/images/line-bg.GIF);";
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

	l_return += "<td style=\"width:0%\">";

	//文件夹图片
	l_return += "<span id='" + this.nodeName + "_folder'>";
	if(this.array.Find("cate_type")==0){
		if ( this.tree.ShowOnRender == 1 && this.children.count > 0)
			if (this.array.Find('ExpandImage') != null ) //如果有自定义的展开图片就用
				l_return += '<img src="' + this.array.Find('ExpandImage') + '" border=0>';
			else //否则用缺省的展开图片
				l_return += StrExpandImage2;
		else
			if (this.array.Find('CloseImage') != null ) //如果有自定义的收缩图片就用
				l_return +=  '<img src="' + this.array.Find('CloseImage') + '" border=0>';
			else //否则用缺省的收缩图片
				l_return += StrCloseImage2;
	}
	else{
		l_return +=strReportImage;
	}
	l_return += "</span>";

	l_return += "</td>";
	l_return += "<td align=left>";

	//分类的描述
	l_return += "<input type=text id=" + this.nodeName + " class=\"TreeNode\" readonly "
	l_return += "style=\"background-color:transparent;border:0;height:17px;cursor:hand\"";
	l_return += " onclick=\"javascript:oTree.Find('" + this.nodeName + "').OnClick();\"";
	l_return += " oncontextmenu=\"javascript:oTree.Find('" + this.nodeName + "').OnContextMenu()\"";
	l_return += " value=\"";
	l_return += this.array.Find('cate_desc');
	l_return += "\"></input>";

	l_return += "</td>";

	l_return += "</tr>";

	//子分类的table
	l_return += "<tr>";

	l_return += "<td colspan=3>";
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

////////////////////////////////////////////////////////////////////////////////
// 下面的代码是为了显示或隐藏 Treeview
var j;
var xt;
var leftcolInt;

/*
	OnTreeViewButtonClick：显示或隐藏 Treeview 菜单
*/

function OnTreeViewButtonClick()
{

	j = 0;
	if (document.all.item("divLeftMenu").offsetWidth == 0)
	{	//to right
		document.all.item("treeviewbutton").src = "../common/tree/images/hidetree.gif";
		document.all.item("treeviewbutton").title = L_TreeViewHide_Text;
		TreeView_GoRight();
	}
	else
	{	//to left

		leftcolInt = document.all.item("divLeftMenu").offsetWidth
		xt = leftcolInt;
		document.all.item("treeviewbutton").src = "../common/tree/images/showtree.gif";
		document.all.item("treeviewbutton").title = L_TreeViewShow_Text;
		TreeView_GoLeft();
	}
}

function TreeView_GoRight()
{
   		xt = xt + 20 ;
    	if (xt < leftcolInt ){
		   	document.all.item("divLeftMenu").style.pixelWidth = xt;
			setTimeout('TreeView_GoRight()',10);
		}
		else
		{
			document.all.item("divLeftMenu").style.pixelWidth = leftcolInt;
		}
	}

function TreeView_GoLeft()
{
	xt = xt - 20 ;
    if (xt > -20)
    {
		if (xt < 0) { xt =0; }

		document.all.item("divLeftMenu").style.pixelWidth = xt;
		setTimeout('TreeView_GoLeft()',10);
	}
}

// Click
function tNodeOnClick()
{
	var deptseq
	if( StrLastNode != "" )
		document.all.item(StrLastNode).className = "TreeNode";
	StrLastNode=this.nodeName;
	document.all.item(StrLastNode).className = "TreeNodeSelected";
	//m_DetailBoardID = this.nodeName;
	//m_DetailBoardType= this.array.Find("cate_type");
	deptseq = this.array.Find("cate_value");

	//document.all.ifrmDashBoardContent.src = "DashboardContent.asp?DashboardID=" + m_DetailBoardID + "&DashboardType=" + m_DetailBoardType;
	//alert(document.all.userlist.src)
	//document.all.userlist.src="userlist.asp?deptseq="+deptseq
	OnTreeNodeClick(deptseq)

}
//OnContextMenu
function tNodeOnContextMenu()
{


}
