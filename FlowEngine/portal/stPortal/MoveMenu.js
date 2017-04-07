function menuControl(show)
{
  var objID = event.srcElement.id;
  var index = objID.indexOf("_");
  var mainID = objID.substring(0,index);
  var numID = objID.substring(index+1,objID.length);
  if (mainID == "menubar")
  {
    if (show == 1)
      eval("showMenu("+"menu_"+numID+")");
    else
      eval("hideMenu("+"menu_"+numID+")");
  }
}

function showMenu(objMenu)
{
  if (objMenu == null)
    return;
  var objMenuallItem = objMenu.all;
  var iNum=0;
  for (i=0;i<objMenuallItem.length;i++)
  {
    if (objMenuallItem[i].tagName.toLowerCase() == "span")
      iNum = iNum + 1;
  }
  var iH = 70+iNum*20;
  var objiFrame = parent.document.all.item("MoveiFrame");
  objiFrame.style.height = iH;
  objMenu.style.display = "block";
}

function hideMenu(objMenu)
{
  objMenu.style.display = "none";
  var objiFrame = parent.document.all.item("MoveiFrame");
  objiFrame.style.height = 48;
}

function keepMenu(objMenu)
{
  showMenu(objMenu);
}

function Navigation(asPage)
{
	parent.ifrmDashBoardContent.window.location = asPage;
}

function closeIt()
{
	OnTreeViewButtonClickEx();
  parent.document.all.item("MoveiFrame").style.display = "none";
}
function OnTreeViewButtonClickEx()
{
		xt = 0;
		leftcolInt = 160;
		parent.document.all.item("treeviewbutton").src = "images/hidetree.gif";
		parent.document.all.item("treeviewbutton").title = parent.L_TreeViewHide_Text;		
		TreeView_GoRightEx();
}
function TreeView_GoRightEx()
{
   	xt = xt + 20 ;
    if (xt < leftcolInt ){
		 	parent.document.all.item("divLeftMenu").style.pixelWidth = xt;
			setTimeout('TreeView_GoRightEx()',10);
		}
		else
		{
			parent.document.all.item("divLeftMenu").style.pixelWidth = leftcolInt;
		}
	}
//¹ØÓÚÒÆ¶¯
var DragLayerX,DragLayerY;
var ifMove=false;
document.onmousemove=Move;

function Move()
{
  if (ifMove && window.event.button==1)
  {
    DragLayer=parent.document.all.item("MoveiFrame").style;
    DragLayer.posLeft += window.event.clientX-DragLayerX;
    DragLayer.posTop += window.event.clientY-DragLayerY;
  }
}

function Drag()
{
  ifMove=true;
  DragLayer = parent.document.all.item("MoveiFrame").style;
  DragLayerX = window.event.clientX;
  DragLayerY = window.event.clientY;
}

function undoDrag()
{
  ifMove=false;
}

