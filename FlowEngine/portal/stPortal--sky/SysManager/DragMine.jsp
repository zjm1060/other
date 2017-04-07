<%@ page contentType="text/html; charset=gb2312" errorPage="../include/error.jsp"%>
<%@ page language="java" import="jbportal.PortalUtil,suntek.portal.publicLib.RecordSetObject" %>
<jsp:useBean id="oManager" scope="session" class="jbportal.sysManagerBean" />
<%
       String m_StrpNode = "";
       String ZoneTop = "4";
	  String m_modu_id = request.getParameter("modu_id");
	  if (m_modu_id.equals(""))
          {
		m_modu_id = "0001";
          }

  	  String [] m_Str1 = PortalUtil.GetPositionTopForDragMine(oManager,"4",m_StrpNode,m_modu_id);
          String m_StrTop   = m_Str1 [0];
          m_StrpNode = m_Str1 [1];

	String [] m_Str2 = PortalUtil.GetPositionTopForDragMine(oManager,"3",m_StrpNode,m_modu_id);
          String m_StrMiddle= m_Str2 [0];
          m_StrpNode = m_StrpNode + m_Str2 [1];

	String [] m_Str3 = PortalUtil.GetPositionTopForDragMine(oManager,"1",m_StrpNode,m_modu_id);
          String m_StrLeft  = m_Str3 [0];
          m_StrpNode = m_StrpNode + m_Str3 [1];

	String [] m_Str4 = PortalUtil.GetPositionTopForDragMine(oManager,"2",m_StrpNode,m_modu_id);
          String m_StrRight = m_Str4 [0];
          m_StrpNode = m_StrpNode + m_Str4 [1];

	String [] m_Str5 = PortalUtil.GetPositionTopForDragMine(oManager,"5",m_StrpNode,m_modu_id);
          String m_StrBottom = m_Str5 [0];
          m_StrpNode  = m_StrpNode + m_Str5 [1];

	String m_StrLeftBar =PortalUtil.GetLeftBarForDragMine(oManager,m_modu_id);


%>
<link REL="stylesheet" HREF="../include/drag/cust.css"></link>
<link REL="stylesheet" HREF="../include/Public.css"></link>
<script language="JavaScript" src="../include/drag/layout.js"></script>
<script language="JavaScript" src="../include/drag/Drag.js"></script>

<script LANGUAGE="javascript">
<!--
	var CShow	=	"show";	//显示状态
	var CNew	=	"new";		//新增状态
	var CDel	=	"delete";  //删除状态
	var apk = 0;			   //Left UP DOWN by Max
	var Position=18;
	//用于保留各状态的类
	function dBuffer()
	{
		this.nodes=new Array();
		this.Add= dAddNode;
		this.Delete = dDelNode;
	}

	//增加一个节点
	function dAddNode(a_func_id,a_zone,a_status,a_Title,a_Position,a_modu_id,a_modu_Title)
	{
		//alert('operator=new');
		var oNode = new pNode(a_func_id,a_zone,a_status,a_Title,a_Position,a_modu_id,a_modu_Title);
		var oNode1;
		//新增一个接点的时候，判断该接点是否在类中已经存在
		//如果存在且状态为CDel就改变其状态为CShow，如果存在且状态为CShow或CNew则不
		//做任何处理,如果不存在则添加。
		oNode1 = this.nodes[a_func_id];

		if (oNode1==null)
			{
				//不存在的处理

				this.nodes[a_func_id]=oNode;
				return 1;
			}
		else
			{
				if (this.nodes[a_func_id].Status==CDel)
				{
					this.nodes[a_func_id].Status==CShow;
					return 2;
				}
				else
				{
					return 3;
				}
			}
		return 1;
	}

	//删除一个节点
	function dDelNode(a_func_id)
	{
		//alert('operator=del');
		var newnodes = new Array();
		var l_s;

		if (this.nodes[a_func_id]==null)
			{
				alert("节点为空");
				return -1 ;
				}
		if (this.nodes[a_func_id].Status==CNew)
			{
			for (l_s in this.nodes)
				{
					if (l_s!=a_func_id)
						{
							newnodes[l_s]=this.nodes[l_s];
						}
				}
			this.nodes = newnodes;
			return true;
			}
		else
			{
				//如果该节点的状态不是"new"则改变该节点的状态为"delete"
				this.nodes[a_func_id].UpdateStatus(CDel);
				return true;
			}
	}

	//页面功能节点类
	function pNode(a_func_id,a_zone,a_status,a_Title,a_Position,a_modu_id,a_modu_Title)
	{
		this.Func_id	= a_func_id;
		this.modu_id	= a_modu_id;
		this.modu_Title = a_modu_Title;
		this.Zone		= a_zone;
		//状态为三种:1--show 2--new 3--deleted
		this.Status		= a_status;
		this.Title		= a_Title;
		this.Position   = a_Position;
		this.UpdateStatus		= pNodeUpdateStatus;
		this.UpdateZone = pNodeUpdateZone;
	}

	//对节点进行修改,改变状态
	//返回: true 成功 false 失败
	function pNodeUpdateStatus(a_Status)
	{
                    //alert('state=' + a_Status);
		this.Status = a_Status;
		return true;
	}

	//对节点进行修改，改变位置
	function pNodeUpdateZone(a_Zone)
	{
		this.Zone=a_Zone;
		return true;
	}




	//类用于存放所有的模块功能
	function cAllFunc()
	{
		this.nodes = new Array();
		this.Add = AllFuncAdd;  //增加一个模块元素
		this.AddCell = FuncAddCell;
		this.Del = DelAllFunc;
		this.DelCell=DelCellAllFunc;
		this.Find=FindFunc;
	}

	//找到指定的元素
	function FindFunc(a_func_id)
	{
		var l_s,l_func;
		for (l_s in this.nodes)
		{
			for (l_func in this.nodes[l_s].nodes)
			{
				if (l_func==a_func_id)
				{
					return this.nodes[l_s].nodes[l_func];
				}
			}
		}
	}
	//从对象cAllFunc中直接删除一个function节点
	function DelCellAllFunc(a_func_id)
	{
		var l_s,l_func;

		for (l_s in this.nodes)
		{
			for (l_func in this.nodes[l_s].nodes)
			{
				if (l_func==a_func_id)
				{
					this.nodes[l_s].Delete(a_func_id);
					return true;
				}
			}
		}
		return false;
	}

	function DelAllFunc(a_modu_id)
	{
		var newnodes = new Array();
		var l_s;

		if (this.nodes[a_func_id]==null)
			{
				alert("节点为空");
				return -1 ;
			}

			for (l_s in this.nodes)
				{
					if (l_s!=a_func_id)
						{
							newnodes[l_s]=this.nodes[l_s];
						}
				}
			this.nodes = newnodes;
			return true;
	}

	function AllFuncAdd(a_modu_id,a_modu_Title)
	{
		var oNode;
		oNode = new cModuFunc(a_modu_id,a_modu_Title);
		this.nodes[a_modu_id] = oNode;
	}

	//增加一个节点
	function FuncAddCell(a_modu_id,a_modu_Title,a_func_id,a_Title,a_ary)
	{
		var oNode;

		//先判断节点中是否已经存在相应的节点
		if (this.nodes[a_modu_id]==null)
		{
			//增加一个节点
			oNode = new cModuFunc(a_modu_id,a_modu_Title);
			this.nodes[a_modu_id] = oNode;
		}

		//判断模块节点中是否存在，如果不存在则添加
		if (this.nodes[a_modu_id].nodes[a_func_id]==null)
		{
			this.nodes[a_modu_id].Add(a_func_id,a_modu_id,a_Title,a_modu_Title,a_ary)
		}

	}

	//类用于存放每个模块所有的功能
	function cModuFunc(a_Modu_id,a_modu_Title)
	{
		this.nodes = new Array();
		this.moduid = a_Modu_id;
		this.moduTitle=a_modu_Title;
		this.Add=ModuFuncAdd;
		this.Delete=ModuFuncDel;
	}

	//类function用于存放各元素的属性
	function cfunc(a_func_id,a_modu_id,a_Title,a_modu_Title,a_ary)
	{
		this.func_id=a_func_id;
		this.modu_id=a_modu_id;
		this.modu_Title=a_modu_Title
		this.Title=a_Title;
		this.other=a_ary;
	}

	//增加一个元素
	function ModuFuncAdd(a_func_id,a_modu_id,a_Title,a_modu_Title,a_ary)
	{
		var ofunc;

		ofunc = new cfunc(a_func_id,a_modu_id,a_Title,a_modu_Title,a_ary);
		this.nodes[a_func_id] = ofunc;
	}

	//删除一个元素
	function ModuFuncDel(a_func_id)
	{
		var newnodes = new Array();
		var l_s;

		if (this.nodes[a_func_id]==null)
			{
				alert("节点为空");
				return -1 ;
			}

			for (l_s in this.nodes)
				{
					if (l_s!=a_func_id)
						{
							newnodes[l_s]=this.nodes[l_s];
						}
				}
			this.nodes = newnodes;
			return true;
	}

//--------------------------------其他函数-------------------------------------------
	//得到当前系统包括那些模块
	function GetModu()
	{
		var l_s;
		var l_option;
		var l_i,l_CurrModu;

		if (oAllFunc==null) {return false;}

		l_i = 0
		l_option="<SELECT class='Select' ID='moduSelect' name='moduSelect' onclick='javascript:moduSelect_Changed();'>";
		for (l_s in oAllFunc.nodes)
		{
			l_option = l_option + "<OPTION value=" + l_s
			if (l_i == 0)
			{l_option = l_option + " Selected ";
			 l_CurrModu = l_s;
			 }

			l_option = l_option + "> " + oAllFunc.nodes[l_s].moduTitle + "</OPTION> "
			l_i = l_i + 1;
		}
		if (l_i==0)
		{
		return "";
		}

		l_option = l_option + "</SELECT>"
		return l_option;

	}

	//得到指定模块的未使用功能的现实
	function GetFunction(a_modu_id)
	{
		var l_s;
		var l_StrTop;

		l_StrTop="";
		for (l_s in oAllFunc.nodes[a_modu_id].nodes)
		{
			l_StrTop = l_StrTop + "<Table border = '0' CELLSPACING='1' CELLPADDING='0' CLASS='LayoutWebPartFrame' ID='" + l_s + "' >"
			l_StrTop = l_StrTop + "<tr><td CLASS='LayoutWebPart' >" + oAllFunc.nodes[a_modu_id].nodes[l_s].Title + "</td></tr> "
			l_StrTop = l_StrTop + "</TABLE>"

		}

		return l_StrTop;
	}



//-------------------------处理函数--------------------------------------
function ChangeBuffer(a_prtSrc,a_prtTarget)
	{
		var l_zone;
		var l_Srcid;
		var oNode;

		if (a_prtSrc==null) {return;}

		if (a_prtSrc.id==null){return;}

		l_Srcid = a_prtSrc.id
		l_zone=-1;
		//判断移动的位置
		if (a_prtTarget!=null)
			//判断Target来决定要做的处理
			switch (a_prtTarget.className)
				{
					case 'LayoutDelZone':
						//删除的处理
						l_zone = 0;

						oNode = oBuffer.nodes[l_Srcid];

						if (oNode==null){
							a_prtTarget.innerText=' ';
							moduSelect_Changed();
							return;
								}

						//在oAllFunc对象中添加
						oAllFunc.AddCell(oNode.modu_id	,oNode.modu_Title,oNode.Func_id,oNode.Title);

						//oBuffer从对象中删除掉
						oBuffer.Delete(l_Srcid);
						a_prtTarget.innerText=" ";
						moduSelect_Changed();
						break;
						return;
					case 'LayoutZoneTop':
						//移到顶部的处理
						l_zone = 4;
						break;
					case 'LayoutZoneLeft':
						//移到左边的处理
						l_zone = 1;
						break;
					case 'LayoutZoneCenter':
						//移到中间的处理
						l_zone = 3;
						break;
					case 'LayoutZoneRight':
						//移到右边的处理
						l_zone = 2;
						break;
					case 'LayoutZoneBottom':
						//移到底部的处理
						l_zone = 5;
						break;
					default:
						l_zone = -1;
						break;
				}

			//如果不是删除则在对象oAllFunc中清除掉当前的节点
			//并在oBuffer对象中添加一个新的节点
			/*
			if ((l_zone!=-1) && (l_zone!=0))
			{
				oNode = oAllFunc.Find(a_prtSrc.id);
				if (oNode==null) {return;}

				oBuffer.Add(a_prtSrc.id,l_zone,CNew,oNode.Title,0,oNode.modu_id,oNode.modu_Title);
				oAllFunc.DelCell(a_prtSrc.id);
			}
			*/

		}

	//当切换模块时作的处理
	function moduSelect_Changed()
	{
		var l_modu_id,l_return;

		if (moduSelect==null) {return;}

		l_modu_id = moduSelect.options(moduSelect.selectedIndex).value;
		if (l_modu_id==null){return;}

		l_return = GetFunction(l_modu_id);
		document.all.item("LeftBar").innerHTML=l_return;

	}

//------------------div的滚动-------------
setTimeout('ini()',300);
function ini(){
    //maxy =425
    maxy=500
    scrollicon = 40*(Math.round(Math.round(document.body.offsetHeight/2.5)/25));
          if ((apk+document.body.offsetHeight) < maxy){
              //down2.style.display = '';
          }
          else {
              //down2.style.display = 'none';
          }
    //up2.style.pixelLeft = document.body.offsetWidth - 700
    //down2.style.pixelLeft = document.body.offsetWidth - 700
    //up2.style.pixelTop = 200
    //down2.style.pixelTop = document.body.offsetHeight - 25
}

function addy(){
   ik = apk;
    addygo();
}

	function addygo()
	{
	 if(inFloder.clientHeight <= 260){
    maxHeight=-10;
    }
    if(inFloder.clientHeight > 260 && inFloder.clientHeight <= 900){
    maxHeight=inFloder.clientHeight/1.8;
    }
    if(inFloder.clientHeight > 900 && inFloder.clientHeight <= 1150){
    maxHeight=inFloder.clientHeight/1.6;
    }
    if(inFloder.clientHeight > 1150){
    maxHeight=inFloder.clientHeight/1.4;
    }

    k = apk;

    inFloder.style.clip = 'rect(' + (k) + ' 100% ' + (k+270) + ' 0)';
    inFloder.style.top = (0 - k) + Position;

    //inFloder.style.top = (0 - 200)
    if ((k < ik + scrollicon) && k<(maxHeight)){
        apk = (k + 10);
        setTimeout('addygo()',20);
    }
    if ((k+document.body.offsetHeight)-25>=500){
    }
    }


function miny(){
    ih = apk;
    minygo();
}

function minygo(){
    h = apk
    inFloder.style.clip = 'rect(' + (h) + ' 100% ' + (h+270) + ' 0)';

    inFloder.style.top = (0 - h) + Position;

    if ((h > ih - scrollicon) && ( h >= 0)){
        apk = (h - 10);
        setTimeout('minygo()',20);
    }
    if (apk <= 0){
    }
}
//////////////////
var m_fMouseDown = false;	// mouse down flag
var m_fMouseMoved = false;	// mouse moved flag
var m_cxOffset = 0;			// x offset of mouse from left of drag part
var m_cyOffset = 0;			// y offset of mouse from top of drag part
var m_prtSrc = null;		// ref to source of the part drag
var m_prtDrag = null;		// ref to drag part
var m_zoneLast = null;		// last active zone
var m_idxInsert = 0;		// position at which to insert dropped part

// This function resets any table attributes
// which may have been lost during browser reload.
function CleanUpTables()
{
	document.all.OuttermostContentBodyTable.width = "40px";	 // 40px is an arbitrary number
	document.all.OuttermostContentBodyTable.width = "100%";
}

function Document_OnMouseDown()
{
	if (window.event.srcElement.className == 'LayoutWebPart')
	{
		m_fMouseDown = true;

		m_cxOffset = window.event.clientX - GetAbsoluteLeft(window.event.srcElement);
		m_cyOffset = window.event.clientY - GetAbsoluteTop(window.event.srcElement);

		m_prtSrc = GetPartElement(window.event.srcElement);
		m_prtDrag = m_prtSrc.cloneNode(true);

		m_prtDrag.style.position = 'absolute';
		m_prtDrag.style.filter = 'alpha(opacity=50)';
		m_prtDrag.style.left = (window.event.x - m_cxOffset) + 'px';
		m_prtDrag.style.top = (window.event.y - m_cyOffset) + 'px';
		m_prtDrag.style.width = m_prtSrc.offsetWidth;

		document.body.insertAdjacentElement('BeforeEnd', m_prtDrag);

		divMouse.setCapture();
	}
}

function DivMouse_OnMouseMove()
{
	if (m_fMouseDown)
	{
		var zoneTarget;

		m_fMouseMoved = true;

		m_prtDrag.style.left = window.event.x - m_cxOffset + 'px';
		m_prtDrag.style.top = window.event.y - m_cyOffset + 'px';

		zoneTarget = GetZoneFromPoint(window.event.x, window.event.y, m_prtDrag, tblIBeam);

		if (FIsZone(zoneTarget))
		{
			// We are over a zone
			// If the class name is just "Zone" this is a new zone
			// and we need to switch the last drop zone
			if (m_zoneLast)
			{
				SetAsDropTarget(m_zoneLast, false);
			}

			SetAsDropTarget(zoneTarget, true);

			PositionIBeam(window.event.x, window.event.y, tblIBeam, zoneTarget);
		}
		else
		{
			// Not over a zone at all. Reset last zone and clear it
			if (m_zoneLast)
			{
				SetAsDropTarget(m_zoneLast, false);
			}
			tblIBeam.style.display = 'none';
		}
	}
}

function DivMouse_OnMouseUp()
{
	if (m_fMouseDown)
	{
		if (m_fMouseMoved)
		{
			var zoneTarget;
			var prtTemp;

			zoneTarget = GetZoneFromPoint(window.event.x, window.event.y, m_prtDrag, tblIBeam);

			if (zoneTarget)
			{
				switch (m_idxInsert)
				{
				case -1:
					{
					zoneTarget.insertAdjacentElement('afterBegin', m_prtSrc.removeNode(true));
					break;
					}
				case zoneTarget.children.length:
					{
						// inserting part at the end of the zone
						prtTemp = zoneTarget.children(zoneTarget.children.length - 1);

						// check to make sure the source and target are not the same part
						if (prtTemp != m_prtSrc)
						{
							prtTemp.insertAdjacentElement('afterEnd', m_prtSrc.removeNode(true));
						}
						break;
					}
				default:
					{
						// inserting part before indexed child
						prtTemp = zoneTarget.children(m_idxInsert);
						// check to make sure the source and target are not the same
						if (prtTemp != m_prtSrc)
						{
							prtTemp.insertAdjacentElement('beforeBegin', m_prtSrc.removeNode(true));
						}
					}
				}

				saveToServer.partLayout.value = '<Dashboard>';
				AddPartsInfoForSave(tdZoneTop, 4);
				AddPartsInfoForSave(tdZoneLeft, 1);
				AddPartsInfoForSave(tdZoneCenter, 3);
				AddPartsInfoForSave(tdZoneRight, 2);
				AddPartsInfoForSave(tdZoneBottom, 5);
				saveToServer.partLayout.value += '</Dashboard>';
			}

			tblIBeam.style.display = 'none';
		}

		m_prtDrag.style.position = '';
		m_prtDrag.removeNode(true);
		m_prtDrag = null;

		if (m_zoneLast)
		{
			SetAsDropTarget(m_zoneLast, false);
		}

		divMouse.releaseCapture();
		m_fMouseDown = false;
		m_fMouseMoved = false;
		ChangeBuffer(m_prtSrc,zoneTarget);
	}
}

function AddPartsInfoForSave(node, zone)
{
	var nIndex,oNode,oBufferNode;
	var partCount = node.children.length;
	for (nIndex = 0; nIndex < partCount; nIndex++)
	{
		saveToServer.partLayout.value +=
			'<WebPart>' +
			'  <WebPartID>' + node.children[nIndex].id.substr(7) + '</WebPartID>' +
			'  <Zone>' + zone + '</Zone>' +
			'  <PartOrder>' + nIndex + '</PartOrder>' +
			'</WebPart>';

		oNode = oAllFunc.Find(node.children[nIndex].id);
		if (oNode!=null)
		{
			oBuffer.Add(node.children[nIndex].id,zone,CNew,oNode.Title,0,oNode.modu_id,oNode.modu_Title);
			oAllFunc.DelCell(node.children[nIndex].id);
		}

		//在这里进行次序的调整
		oBufferNode=oBuffer.nodes[node.children[nIndex].id];

		oBufferNode.Position=nIndex;
		oBufferNode.Zone=zone
	}
}

// Positions the IBeam element at the right insertion point
function PositionIBeam(x, y, elemIBeam, zoneTarget)
{
	var nParts;
	var prtFirst;
	var prtTemp;
	var idxPart;

	nParts = zoneTarget.children.length;
	if (nParts > 0)
	{
		// get the first part in the zone
		prtFirst = zoneTarget.children(0);

		// set the ibeam width to the part width
		elemIBeam.style.width = prtFirst.offsetWidth + 1

		// get the part before which we should display the ibeam
		for(m_idxInsert = 0; m_idxInsert < nParts; m_idxInsert++)
			{
				prtTemp = zoneTarget.children(m_idxInsert);
				if (y <= (GetAbsoluteTop(prtTemp) + ((prtTemp.offsetHeight + (parseInt(prtTemp.cellSpacing) * 2)) / 2)))
				{
					break;
				}
			}

		// if the part index is beyond the last part,
		// move the ibeam to after the last part
		if (m_idxInsert >= nParts)
		{
			m_idxInsert = nParts;
			prtTemp = zoneTarget.children(m_idxInsert - 1);
			elemIBeam.style.top = String(((GetAbsoluteTop(prtTemp) +
				(prtTemp.offsetHeight + (parseInt(prtTemp.cellSpacing) * 2))) -
				(elemIBeam.offsetHeight / 2))) + 'px';
			elemIBeam.style.left = String(GetAbsoluteLeft(prtTemp)) + 'px';
		}
		else
		{
			prtTemp = zoneTarget.children(m_idxInsert);
			elemIBeam.style.top = String((GetAbsoluteTop(prtTemp) -
				(elemIBeam.offsetHeight / 2) + 1)) + 'px';
			elemIBeam.style.left = String(GetAbsoluteLeft(prtTemp)) + 'px'
		}

		// show it
		elemIBeam.style.display = 'inline';
	}
	else
	{
		elemIBeam.style.display = 'none';
		m_idxInsert = -1;
	}
}

// Navigates up from an element until it finds the element
// of type LayoutWebPartFrame (the whole table representing the WebPart)
function GetPartElement(subElem)
{
	var partFrame = subElem
	while (partFrame.className != 'LayoutWebPartFrame')
	{
		partFrame = partFrame.parentElement;
	}
	return partFrame;
}

// Gets real Top value with respect to client area
function GetAbsoluteTop(elem)
{
	var topPosition = 0;

	while (elem)
	{
		if (elem.tagName == 'BODY')
		{
			break;
		}
		topPosition += elem.offsetTop;
		elem = elem.offsetParent;
	}
	return topPosition;
}

// Get real Left value with respect to client area
function GetAbsoluteLeft(elem)
{
	var leftPosition = 0;

	while (elem)
	{
		if (elem.tagName == 'BODY')
		{
			break;
		}
		leftPosition += elem.offsetLeft;
		elem = elem.offsetParent;
	}
	return leftPosition;
}

// Returns true if the given element is one of our zones.
function FIsZone(elem)
{
	var fRet = false;
	var strClass;
	var ich;

	if (elem)
	{
		strClass = elem.className;
		ich = strClass.indexOf(' ');
		if (ich != -1)
		{
			strClass = strClass.substr(0, ich);
		}

		fRet = strClass == 'LayoutZoneTop' ||
				strClass == 'LayoutZoneLeft' ||
				strClass == 'LayoutZoneCenter' ||
				strClass == 'LayoutZoneRight' ||
				strClass == 'LayoutZoneBottom'||
				strClass == 'LayoutDelZone';
	}

	return fRet;
}

// Sets or clears elem as the drop target.
function SetAsDropTarget(elem, fSet)
{
	var strClass;
	var ich;

	if (fSet)
	{
		strClass = elem.className;
		if (strClass.indexOf(' LayoutDropZone') == -1)
		{
			elem.className = strClass + ' LayoutDropZone';
			m_zoneLast = elem;
		}
	}
	else
	{
		strClass = elem.className;
		ich = strClass.indexOf(' LayoutDropZone');
		if (ich != -1)
		{
			elem.className = strClass.substr(0, ich);
			m_zoneLast = null;
		}
	}
}


// Gets the zone underneath the x,y or returns Nothing if not over a zone
function GetZoneFromPoint(x, y, prtDrag, elemIBeam)
{
	// This is totally hoaky. Hopefully someone on DHTML talk can
	// give me a better idea. The problem is that elementFromPoint will
	// return the top-most element under the point, which is the part
	// being dragged, not the zone underneath it. Setting zIndex to -1
	// temporarily seems to work, but also seems like a lot of overhead.

	var prtDragZIndexOld = prtDrag.style.zIndex;
	var elemIBeamZIndexOld = elemIBeam.style.zIndex;

	prtDrag.style.zIndex = -1;
	elemIBeam.style.zIndex = -1;

	//当焦点移动到本窗体之外后则退出!
	//if (window.event.x <= 0 || window.event.y <= 0 ||
	//	window.event.x >= window.frameElement.Width ||
	//	window.event.y >= window.frameElement.Height){
	//	zone=null;
	//	 return zone;}

	if (window.event.x <= 0 || window.event.y <= 0 ||
		window.event.x >= document.all.item("OuttermostContentBodyTable").Width ||
		window.event.y >= document.all.item("OuttermostContentBodyTable").Height){
		zone=null;
		return zone;}

	var zone = document.elementFromPoint(x, y);

	prtDrag.style.zIndex = prtDragZIndexOld;
	elemIBeam.style.zIndex = elemIBeamZIndexOld;

	// Now the element we just got might be another part sitting in
	// the zone, so check to see if the class name is "LayoutWebPart". If it
	// is then go up the parent chain until we get its zone

	if ((zone.className == 'LayoutWebPart') || (zone.className == 'LayoutWebPartFrame'))
	{
		while ((!FIsZone(zone)) &&
				(zone.className != 'LayoutDropZone') &&
				(zone.tagName != 'BODY'))
		{
			zone = zone.parentElement;
		}
	}

	if(zone==null){
	}
	else
	{
	// Last check is that we might be over the body or something
	// else that is not a zone at all. If the current return pointer
	// is not of className = "Zone" or "LayoutDropZone" then return Nothing
	if ((!FIsZone(zone)) && (zone.className != 'LayoutDropZone') && (zone.className!='LayoutDelZone'))
		{
		zone = null;
		}
	}
	return zone;
}


//////////////////
	var oBuffer,oAllFunc;
	oBuffer = new dBuffer;
	oAllFunc = new cAllFunc;
	<%=m_StrLeftBar%>
	<%=m_StrpNode%>

	//当按取消的时候返回上一页。
	function Exit(){
      window.location.reload();
	}
	//进行存盘处理
	function Save()
	{
		//将oBuffer中的节点切换成相应的字串
		var l_s,oNode;
		var l_value,l_sql,l_where;
		var l_modu_id;
		l_value ="";
		l_sql ="";
		l_where="";

		for (l_s in oBuffer.nodes)
		{
			//alert(l_s);

			oNode = oBuffer.nodes[l_s];
			//alert(oNode.Status);
			l_where = " where modu_id = '" + '<%=m_modu_id%>'
			l_where+= "' and func_id = '" + oNode.Func_id + "'" ;

			switch (oNode.Status)
			{

				case 'new':
					l_sql = "Insert into modu_func " +
								"(modu_id, func_id, IsIncluded, IsVisible, HasFrame, AllowRemove, AllowMinimize, " +
								"Zone, FrameState, HeightEnabled, Height, HeightUnitType, WidthEnabled, Width, " +
								"WidthUnitType, RequiresIsolation,RefreshInterval,DashBoardOrder) " +
							" (Select '" +
								'<%=m_modu_id%>' + "' , func_id, IsIncluded, IsVisible, HasFrame, AllowRemove, AllowMinimize, " +
								oNode.Zone + ", FrameState, HeightEnabled, Height, HeightUnitType, WidthEnabled, Width, " +
								"WidthUnitType, RequiresIsolation,RefreshInterval," + oNode.Position +
							" from  func_def " +
							" Where func_id = '" + oNode.Func_id + "');";
					break;
				case 'delete':
					l_sql = "Delete from modu_func " + l_where + ";";
					break;
				case 'show':
					l_sql = "Update modu_func set zone=" + oNode.Zone +
							",DashBoardOrder=" + oNode.Position + " " + l_where + ";";

					break;
				default:
					break;
			}
			l_value += l_sql;
		}

		saveToServer.partLayout.value = l_value;
                   // alert(l_value);
		saveToServer.modu_id.value  = '<%=m_modu_id%>'
		saveToServer.action = "_DragOperator.jsp";
		saveToServer.submit();
	}

	function window_onload()
	{
		var l_return;
		var l_selvalue;
		//建立下拉框显示模块
		l_return = GetModu();

		if (l_return!=""){
			document.all.item("LeftBarSelect").innerHTML=l_return;
			document.all.item("moduSelect").style.posWidth=160;
			l_return = moduSelect.options(moduSelect.selectedIndex).value;
			}
		else
			{
			return;
			}

		//显示对应的各模块功能
		l_return = GetFunction(l_return);

		if (l_return!=""){
			document.all.item("LeftBar").innerHTML=l_return;
			//document.all.item("inFloder").innerHTML=l_return;

			}
	}
window.attachEvent("onload",window_onload);
//id="LeftBar" class="LayoutLeftBar"
//-->
</script>

<body onmousedown="Document_OnMouseDown();" onload="CleanUpTables();" leftmargin="0" rightmargin="0" topmargin="0">
	<div ID="divMouse" STYLE="display:none" onmousemove="DivMouse_OnMouseMove();" onmouseup="DivMouse_OnMouseUp();"></div>
	<table width="100%" height="100%" border="0" cellspacing="0" cellpadding="0" id="OuttermostContentBodyTable">
		<tr>
			<td valign="Top" height="90%" width="150" class="3DTD">
				<table border="0" width="100%" class="LeftBarTable" height="100%">
					<tr><td id="LeftBarSelect" align="center"></td></tr>
					<!--<tr><td align="right"><img src="../images/up100.gif" style="cursor:hand;" onclick="javaScript:miny();" WIDTH="16" HEIGHT="16"></td></tr>
					<tr><td align="right"><img src="../images/down.gif" style="cursor:hand;" onclick="javaScript:addy();" WIDTH="16" HEIGHT="16"></td></tr>
                    -->
					<tr><td><div id="inFloder" style="overflow:auto;position:absolute;left:7;top:28;width:160px;height:280px;z-index:0;clip:rect(0 160 280 0);">
								<table BORDER="0" VALIGN="TOP" CLASS="LayoutFuncZones">
									<tr>
										<td VALIGN="TOP" id="LeftBar" class="LayoutLeftBar">
										</td>
									</tr>
								</table>
							</div>
						</td></tr>
					<tr><td height="100%"></td></tr>
					<tr><td bgcolor="#f0f0f0">删除区</td></tr>
					<tr><td class="LayoutDelZone" height="30" width="100%" id="Del">&nbsp;</td></tr>
				</table>

			</td>
		  <td>
		 	<table CLASS="LayoutZonesContainer" BORDER="0" CELLSPACING="0" CELLPADDING="0">
		 		<tr>
				<td>
					<table CLASS="LayoutZones" BORDER="0" CELLSPACING="4" CELLPADDING="2">
						<tr>
							<td CLASS="LayoutZoneTop" ID="tdZoneTop" COLSPAN="3">
								 <%=m_StrTop%>
								&nbsp;
							</td>
						</tr>
						<tr>
							<td CLASS="LayoutZoneLeft" ID="tdZoneLeft">
								<%=m_StrLeft%>
								&nbsp;
							</td>
							<td CLASS="LayoutZoneCenter" ID="tdZoneCenter">
								<%=m_StrMiddle%>
								&nbsp;
							</td>
							<td CLASS="LayoutZoneRight" ID="tdZoneRight">
								<%=m_StrRight%>
								&nbsp;
							</td>
						</tr>
						<tr>
							<td CLASS="LayoutZoneBottom" ID="tdZoneBottom" COLSPAN="3">
								<%=m_StrBottom%>
								&nbsp;
							</td>
						</tr>
					</table>
				</td>
				</tr>
			</table>
		 </td>
		</tr>
		<tr>
			 <TD></TD>
			 <td class="CustButtonSectionRight" NOWRAP>
				<table>
					<tr>
					<td>
						<form METHOD="POST" ID="saveToServer" ACTION="layout.jsp">
							<input class="Input_Button" TYPE="button" id="submit1" name="submit1" value="确    定" onclick="javascript:Save();">
							<input class="Input_Button" TYPE="button" id="button1" name="button1" value="取    消" onclick="javascript:Exit();">
							<input TYPE="hidden" NAME="partLayout" ID="partLayout" >
							<input TYPE="hidden" NAME="DashboardID" >
							<input TYPE="hidden" NAME="modu_id" >
						</form>
					</td>
					</tr>
				</table>
			</td>
		</tr>
	</table>

		<table CLASS="LayoutIBeam" ID="tblIBeam" CELLPADDING="0" CELLSPACING="0" BORDER="0" height=0>
			<tr CLASS="LayoutIBeamTR"><td></td></tr>
			<tr CLASS="LayoutIBeamTR">
				<td CLASS="LayoutIBeamTDTop"></td>
			</tr>
			<tr CLASS="LayoutIBeamTR">
				<td CLASS="LayoutIBeamTDBottom"></td>
			</tr>
			<tr CLASS="LayoutIBeamTR"><td></td></tr>
		</table>

</body>

