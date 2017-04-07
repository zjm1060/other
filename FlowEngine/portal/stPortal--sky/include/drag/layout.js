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

