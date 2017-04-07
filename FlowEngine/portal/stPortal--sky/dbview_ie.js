var fMenuUp;

fMenuUp = false;


//--功能块类----------------------------------------------------
function tPart(as_DashboardID, as_DashboardType, as_PartID, as_PartType ) //功能块类
{
	//属性
	this.DashboardID = as_DashboardID;
	this.DashboardType = as_DashboardType;
	this.PartID = as_PartID;
	this.PartType = as_PartType;
	this.DashboardID = as_DashboardID;
	this.IsIncluded = 1;
	this.FrameState = 1;

	//方法
	this.OnMinRestoreClick = tPartOnMinRestoreClick;
	this.OnCloseClick = tPartOnCloseClick;
	this.OnRefreshClick = tPartRefresh;
	this.OnNewWindow = tPartNewWindow;
    this.OnLayoutClick=tOnLayoutClick;
	this.Save = tPartSave;
}

//Open the part in new window
function tPartNewWindow()
{
	var elemSrc;
	var strPartID;
	var strNewURL;
	var ich;
	var elem;

	elemSrc = window.event.srcElement;
	if (elemSrc.className == "DashPartNewWindow")
	{
		// The refresh icon is contained in a table.
		elem = elemSrc.parentElement;
		while(String(elem.tagName).toLowerCase() != "table")
			{
			elem = elem.parentElement;
			}

		// And that table is contained in another table.
		elem = elem.parentElement;
		while(String(elem.tagName).toLowerCase() != "table")
			{
			elem = elem.parentElement;
			}

		// Find the first IFrame within this outermost table.
		// That is the IFrame that we want to refresh.
		elem = elem.all.tags("IFrame").item(0);

		window.open(elem.src, 'Newwindow');
	}
}

// OnDashboardClick - Handles click events for the dashboard.
function tPartRefresh()
{
	var elemSrc;
	var strPartID;
	var strNewURL;
	var ich;
	var elem;

	elemSrc = window.event.srcElement;
	if (elemSrc.className == "DashPartRefresh")
	{
		// The refresh icon is contained in a table.
		elem = elemSrc.parentElement;
		while(String(elem.tagName).toLowerCase() != "table")
			{
			elem = elem.parentElement;
			}

		// And that table is contained in another table.
		elem = elem.parentElement;
		while(String(elem.tagName).toLowerCase() != "table")
			{
			elem = elem.parentElement;
			}

		// Find the first IFrame within this outermost table.
		// That is the IFrame that we want to refresh.
		elem = elem.all.tags("IFrame").item(0);
		elem.src = elem.src;
	}
}

// OnMinRestoreClick - Handles click events for the minimize/restore buttons
function tPartOnMinRestoreClick(partElem)
{
	var strNewValue;
	var varProp;
	var elem;

	elem = partElem;
	// Determine the new value for the FrameState property.
	while(String(elem.tagName).toLowerCase() != "table")
		elem = elem.parentElement;

	if (partElem.style.display == "none")
	{
		partElem.style.display = "";
		window.event.srcElement.src = "./images/minus.gif";
		window.event.srcElement.title = "最小化页面";
		this.FrameState = 0;

		if( partElem.style.height == "100%")
			elem.height = "100%";
	}
	else
	{
		if( partElem.style.height == "100%")
		{
			elem.height = "";
		}
		partElem.style.display = "none";
		window.event.srcElement.src = "./images/plus.gif";
		window.event.srcElement.title ="恢复页面";
		this.FrameState = 1;
	}
	this.Save();

	return true;
}
//OnLayoutClick - 弹出一个窗口，转到功能设计界面
function tOnLayoutClick()
{

	//弹出一个窗口，转到功能设计界面

	var url;
	var sFeatures;

	url = "PersonalSetting/structure/modufunc_setup.jsp?func_id=" + this.PartID +
		  "&func_type=" + this.PartType +
		  "&modu_id=" + this.DashboardID +
		  "&modu_type=" + this.DashboardType +
		  "&Source=stportal";

	sFeatures = "dialogHeight:400px;dialogLeft:30px;dialogTop:30px;dialogWidth:496px;";
	sFeatures = sFeatures + "center:yes;edge:raised;scroll:no;status:no;unadorned:yes;"

	var sReturn = showModalDialog(url,"",sFeatures);

	//如果按了取消键则不刷新
	if (sReturn == "cancel")
		{
          //do nothing,hehe
        }
		else
		{
			url = "dashboardEx.jsp";
			url += "?DashboardID=" + this.DashboardID + "&DashboardType=" + this.DashboardType;
			window.location = url;
		}

}
// OnCloseClick - Handles click events for the close button
function tPartOnCloseClick(partElem)
{
	var varProp;

	this.IsIncluded = 0;

	elem = window.event.srcElement;
	while(elem.className != "DashPartFrame")
	{
		elem = elem.parentElement;
	}
	elem.removeNode(true);

	this.Save();

	return true;
}

// Gets real Top value with respect to client area
function GetAbsoluteTop(elem)
{
	var n;

	n = 0;

	// There must be an easier way to do this!
	while (elem != null)
	{
		if (elem.tagName == "BODY")
		{
			break;
		}

		n = n + elem.offsetTop;
		elem = elem.offsetParent;
	}

	return n;
}

// Get real Left value with respect to client area
function GetAbsoluteLeft(elem)
{
	var n;

	n = 0;

	// There must be an easier way to do this!
	while (elem != null)
	{
		if (elem.tagName == "BODY")
		{
			break;
		}

		n = n + elem.offsetLeft;
		elem = elem.offsetParent;
	}

	return n;
}

// Save Web Part Status.
function tPartSave()
{
	var strURL;
	var strCmd;
	var xhreq;

	strURL = varStoreASP;
	strURL += "?DashboardID=" + this.DashboardID;
	strURL += "&DashboardType=" + this.DashboardType;
	strURL += "&PartID=" + this.PartID;
	strURL += "&PartType=" + this.PartType;


	strCmd = "restore";
	if ( this.FrameState )
		strCmd = "minimize"
	else if ( !this.IsIncluded )
		strCmd = "close";

	strURL += "&WebPartCmd=" + strCmd;

	// Pass the XML to STORE.JSP.
	xhreq = new ActiveXObject("Microsoft.XMLHTTP");
	xhreq.open("POST", strURL, false);

	xhreq.send();

	// If we got "OK" back, then consider the post successful.
	return ("OK" == xhreq.statusText);
}

function ChangeSetting()
{
	var url;
    var varDetailBoardID="";
    var varDetailBoardType="";
    try{
      varDetailBoardID=m_DetailBoardID;
      varDetailBoardType=m_DetailBoardType;
    }
    catch(e){
      varDetailBoardID=m_CurrentBoardID;
      varDetailBoardType=m_CurrentBoardType;
    }
	url = "SysManager/default.jsp?DashboardID=" + m_CurrentBoardID + "&DashboardType=" + m_CurrentBoardType;
	url += "&DetailBoardID=" + varDetailBoardID + "&DetailBoardType=" + varDetailBoardType;
	window.location = url;
}

