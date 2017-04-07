function menuControl(show)
{
  var objID = event.srcElement.id;
  var tempObj = document.all.item(objID);
  var index = objID.indexOf("_");
  var mainID = objID.substring(0,index);
  var numID = objID.substring(index+1,objID.length);
  if (mainID == "menubar")
  {
    if (show == 1)
    {
      tempObj.style.backgroundColor = "#34cafc";
      showMenu("menu_"+numID);
    }
    else
    {
      tempObj.style.backgroundColor = "#003399";
      hideMenu("menu_"+numID);
    }
  }
}

function showMenu(objName)
{
  objMenu = parent.document.all.item(objName);
  if (objMenu == null)
    return;
  var tempLeft = parseInt(objMenu.iLeftValue);
  var tempTop = parseInt(objMenu.iTopValue);
  objMenu.style.left = parseInt(SubInitOffsetLeft)+tempLeft;
  objMenu.style.top = parseInt(SubInitOffsetTop)+tempTop;
  objMenu.style.width = 200;
  objMenu.style.display = "block";
}

function hideMenu(objName)
{
  objMenu = parent.document.all.item(objName);
  if (objMenu == null)
    return;
  objMenu.style.display = "none";
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


//¹ØÓÚÒÆ¶¯
var DragLayerX,DragLayerY;
var ifMove=false;
document.onmousemove=Move;

function Move()
{
  if (ifMove && window.event.button==1)
  {
    DragLayer=parent.document.all.item("MoveiFrame").style;
    if (SubInitOffsetLeft > 0)
      DragLayer.posLeft += window.event.clientX-DragLayerX;
    if (SubInitOffsetTop > 0)
      DragLayer.posTop += window.event.clientY-DragLayerY;

    var oElement = parent.document.all.item("MoveiFrame");
    SubInitOffsetLeft = oElement.offsetLeft;
    SubInitOffsetTop = oElement.offsetTop;
  }
}

function Drag()
{
  ifMove=true;
  DragLayer = parent.document.all.item("MoveiFrame").style;
  if (SubInitOffsetLeft <= 0)
  {
    DragLayer.posLeft = 1;
    SubInitOffsetLeft = 1;
    ifMove=false;
  }
  if (SubInitOffsetTop <= 0)
  {
    DragLayer.posTop = 1;
    SubInitOffsetTop = 1;
    ifMove=false;
  }
  DragLayerX = window.event.clientX;
  DragLayerY = window.event.clientY;
}

function undoDrag()
{
  ifMove=false;
}

//zxc
function OnTreeViewButtonClickEx()
{
  xt = 0;
  leftcolInt = 160;
  parent.document.all.item("treeviewbutton").src = "images/hidetree.gif";
  //parent.document.all.item("treeviewbutton").title = "Òþ²ØÊ÷";
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