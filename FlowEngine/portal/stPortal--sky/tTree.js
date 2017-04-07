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

var StrExpandImage2 = "<img src=\"images/open.gif\" border=\"0\">";
var StrCloseImage2 = "<img src=\"images/close.gif\" border=\"0\">";


var StrLastNode="";



function tNode(a_NodeName,a_ParaName,a_ParaNode,a_tAry,a_Tree)
{
	this.nodeName = a_NodeName;
	this.paraName = a_ParaName;
	this.paraNode = a_ParaNode;
	this.array = a_tAry;
	this.tree = a_Tree;
	this.children = new tArray();

	this.Update = tNodeUpdate;
	this.toString = tNodetoString;
	this.isLeaf = tNodeisLeaf;
	this.Layer = tNodeLayer;
	this.Index = tNodeIndex;
	this.BrotherCount = tNodeBrotherCount;


	this.RenderTable = tNodeRenderTable;
	this.HideTable = tNodeHideTable;
	this.ExpandImage = tNodeExpandImage;
	this.OnClick = tNodeOnClick;
	this.OnContextMenu = null;
}


function tNodeHideTable()
{
	var l_tb,l_dis,l_expand,l_folder;
	l_tb = document.all.item(this.nodeName + '_childrentable');
	l_dis = l_tb.style.display;
	l_expand = document.all.item(this.nodeName + '_expand');
	l_folder = document.all.item(this.nodeName + '_folder');
	if (l_expand==null)
		return
	if ( l_dis == 'none' )
	{
		l_tb.style.display = '';
		l_expand.innerHTML = this.ExpandImage(1);
		if (this.array.Find('ExpandImage') != null )
			l_folder.innerHTML = '<img src="' + this.array.Find('ExpandImage') + '" border=0>';
		else
			l_folder.innerHTML = StrExpandImage2;
	}else{
		l_tb.style.display = 'none';
		l_expand.innerHTML = this.ExpandImage(0);
		if (this.array.Find('CloseImage') != null )
			l_folder.innerHTML = '<img src="' + this.array.Find('CloseImage') + '" border=0>';
		else
			l_folder.innerHTML = StrCloseImage2;
	}
}


function tNodeExpandImage(a_expand)
{
	if ( this.children.count > 0 )
	{
		if ( this.Layer() == 0 && this.BrotherCount() == 1 )
			return (a_expand == 1) ? StrExpandImage : StrCloseImage;
		else if ( this.Index() == 0 && this.Layer() == 0 )
			return (a_expand == 1) ? StrExpandImageTop : StrCloseImageTop;
		else if ( this.Index() == this.BrotherCount() - 1 )
			return (a_expand == 1) ? StrExpandImageBottom : StrCloseImageBottom;
		else
			return (a_expand == 1) ? StrExpandImageLeft : StrCloseImageLeft;
	}else{
		if ( this.Index() == this.BrotherCount() - 1 )
			return StrLeafImage1;
		else
			return StrLeafImage;
	}
}


function tNodeRenderTable()
{
	var l_name,l_node,l_return = "";
	l_return += "<tr>\n";


	l_node = this.paraNode;
	if ( l_node != null )
	{
		l_return += "<td id='" + this.nodeName + "_vline' style=\"width:18px;";
	if ( ! ( this.paraNode != null
			&& this.paraNode.Index() == ( this.paraNode.BrotherCount() -1 )
			)
		)
	{
		l_return += "background-position-x:4px;background-repeat:repeat-y;background-image:url(images/line-bg.gif);";
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


	if ( this.children.count > 0 )
	{
		l_return += " id='" + this.nodeName + "_expand' style=\"cursor:hand;width:18px\"";
		l_return += " onclick=\"javascript:oTree.Find('" + this.nodeName + "').HideTable();\">";
		l_return += this.ExpandImage(this.tree.ShowOnRender);
	}else{
		l_return += " style=\"width:18px\">";
		l_return += this.ExpandImage(this.tree.ShowOnRender);
	}
	l_return += "</span>";
	l_return += "</td>";

	l_return += "<td style=\"width:0%\">";


	l_return += "<span id='" + this.nodeName + "_folder' style=\"cursor:hand;\" onclick=\"javascript:oTree.Find('" + this.nodeName + "').OnClick();\">";
	if ( this.tree.ShowOnRender == 1 && this.children.count > 0 )
		if (this.array.Find('ExpandImage') != null )
			l_return += '<img src="' + this.array.Find('ExpandImage') + '" border=0>';
		else
			l_return += StrExpandImage2;
	else
		if (this.array.Find('CloseImage') != null )
			l_return +=  '<img src="' + this.array.Find('CloseImage') + '" border=0>';
		else
			l_return += StrCloseImage2;
	l_return += "</span>";

	l_return += "</td>";
	l_return += "<td align=left>";


	l_return += "<input type=text id=" + this.nodeName + " class=\"TreeNode\" readonly "
	l_return += "style=\"background-color:transparent;border:0;height:17px;cursor:hand;width:100px;\"";
	l_return += " onclick=\"javascript:oTree.Find('" + this.nodeName + "').OnClick();\"";
	l_return += " onmouseover=\"javascript:this.style.backgroundColor='#ffffe1';this.style.border='1px solid black';\" onmouseout=\"javascript:this.style.backgroundColor='white';this.style.border='0px solid black';\"";
	l_return += " value=\"";
	l_return += this.array.Find('cate_desc');
	l_return += "\"></input>";

	l_return += "</td>";

	l_return += "</tr>";


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

	//½áÊø
	l_return += "</td>\n";
	l_return += "</tr>\n";
	return l_return;
}



function tTree()
{
	this.nodes = new tArray();
	this.count = tTreeCount;
	this.Add = tAddNode;
	this.Find = tFindNode;
	this.Remove = tRemoveNode;
	this.Update = tUpdateNode;
	this.toString = tTreetoString;


	this.ShowOnRender = 0;
	this.RenderTable = tRenderTable;
}


function tRenderTable(a_ShowOnRender)
{
	var l_name,l_node,l_return = "";

	this.ShowOnRender = a_ShowOnRender;

	l_return += "<table border=0 cellspacing=0 cellpadding=0>\n";
	l_return += "<tr height=10px ><td ></td></tr>\n";
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


var j;
var xt;
var leftcolInt;



function OnTreeViewButtonClick()
{
	j = 0;
	if (document.all.item("divLeftMenu").offsetWidth == 0)
	{	//to right
		xt = 0;
		leftcolInt = 160;
		document.all.item("treeviewbutton").src = "images/hidetree.gif";
		document.all.item("treeviewbutton").title = L_TreeViewHide_Text;
		TreeView_GoRight();
		document.all.item("MoveiFrame").style.display="none";
	}
	else
	{	//to left
		leftcolInt = document.all.item("divLeftMenu").offsetWidth
		xt = leftcolInt;
		document.all.item("treeviewbutton").src = "images/showtree.gif";
		document.all.item("treeviewbutton").title = L_TreeViewShow_Text;
		TreeView_GoLeft();
		document.all.item("MoveiFrame").style.display="";
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


function tNodeOnClick()
{
	if( StrLastNode != "" )
		document.all.item(StrLastNode).className = "TreeNode";
	StrLastNode=this.nodeName;
	document.all.item(StrLastNode).className = "TreeNodeSelected";
	m_DetailBoardID = this.nodeName;
	m_DetailBoardType= this.array.Find("cate_type");
    document.all.ifrmDashBoardContent.src = "dashboardEx.jsp?DashboardID=" + m_DetailBoardID + "&DashboardType=" + m_DetailBoardType;
	this.HideTable();
}


function navigationClick(m_DetailBoardID,m_DetailBoardType)
{
  document.all.ifrmDashBoardContent.src = "dashboardEx.jsp?DashboardID=" + m_DetailBoardID + "&DashboardType=" + m_DetailBoardType;
}