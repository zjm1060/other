<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>

<%
	String path = request.getContextPath();
	String basePath = request.getScheme() + "://"
			+ request.getServerName() + ":" + request.getServerPort()
			+ path + "/";

	String holidays = "01.01,01.02,01.03,"
			+ "02.13,02.14,02.15,02.16,02.17,02.18,02.19,"
			+ "04.03,04.04,04.05," + "05.01,05.02,05.03,"
			+ "06.14,06.15,06.16," + "09.22,09.23,09.24,"
			+ "10.01,10.02,10.03,10.04,10.05,10.06,10.07";
%>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<html>
	<head>
		<base href="<%=basePath%>" />
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />


		<meta http-equiv="pragma" content="no-cache" />
		<meta http-equiv="cache-control" content="no-cache" />
		<meta http-equiv="expires" content="0" />
		<meta http-equiv="keywords" content="keyword1,keyword2,keyword3" />
		<meta http-equiv="description" content="This is my page" />

		<script type="text/javascript" src="/xMIS/resources/js/drag.js"></script>
		<link href="/xMIS/resources/css/dragStyle.css" type="text/css"
			rel="Stylesheet" />

		<link rel="STYLESHEET" type="text/css"
			href="/xMIS/resources/css/dhtmlxcalendar.css" />
		<script type="text/javascript"
			src="/xMIS/resources/js/dhtmlxcommon_calendar.js"></script>
		<script type="text/javascript" src="/xMIS/resources/js/dhtmlxcalendar.js"></script>

	</head>

	<BODY onresize=setHeightOfFooter() leftMargin=0 topMargin=0
		marginheight="0" marginwidth="0">
		<br>
		<div id="container" style="width:100%">
			<ul id="tabs">
				<li>
					Page1
				</li>
			</ul>

			<table width=100%><tr><td nowrap>
			<div id="pages" style="width:100%">
				<div id="page1" class="columns" style="width:100%">
					<div class="column" style="width:30%;">
					</div>
					<div class="column" style="width:30%">
					</div>
					<div class="column" style="width:30%">
					</div>
				</div>
			</div>
			</td></tr></table>
			
			<div id="portalContaniner" style="display:none;">

				<div id="newTask" class="item">
					<h3 class="page1">
						新的任务
					</h3>
					<jsp:include page="newTaskListOnPortal.jsp"/>
				</div>
				<div id="activedTask" class="item">
					<h3 class="page1">
						我的任务
					</h3>
					<jsp:include page="activedTaskListOnPortal.jsp"/>
				</div>
				<div id="historyTask" class="item">
					<h3 class="page1">
						历史任务
					</h3>
					<jsp:include page="historyTaskListOnPortal.jsp"/>
					
				</div>
				<div id="newMessage" class="item">
					<h3 class="page1">
						新产品公告
					</h3>
				</div>
				<div id="projectPlan" class="item">
					<h3 class="page1">
						项目进度
					</h3>
					<jsp:include page="projectListOnPortal.jsp"/>
				</div>

				<div id="contractPlan" class="item">
					<h3 class="page1">
						合同状态
					</h3>
					<jsp:include page="contractListOnPortal.jsp"/>
				</div>
				<div id="paymentAlert" class="item">
					<h3 class="page1">
						收付款提示
					</h3>
					<jsp:include page="paymentListOnPortal.jsp"/>
				</div>
				<div id="calendar" class="item">
					<h3 class="page1">
						日历
					</h3>
					<jsp:include flush="true" page="calendar.jsp"></jsp:include>
				</div>
				<div id="schedule" class="item">
					<h3 class="page1">
						日程安排
					</h3>
					<jsp:include page="scheduleListOnPortal.jsp"/>
				</div>

				<div id="outSide" class="item">
					<h3 class="page1">
						外出提示
					</h3>
					<jsp:include page="outSideListOnPortal.jsp"/>
				</div>
			</div>
		</div>
	</body>
</html>

<script type="text/javascript">

var cookieName = "xMisPortal";
//------------------------Utility------------------------
function findPosX(obj) {//辅助函数 得到元素左边与浏览器左边的边距
	var curleft = 0;
	if (obj && obj.offsetParent) {
		while (obj.offsetParent) {
			curleft += obj.offsetLeft;
			obj = obj.offsetParent;
		}
	} else if (obj && obj.x) curleft += obj.x;
	return curleft;// + document.body.scrollLeft - document.body.clientLeft;
}

function findPosY(obj) {//辅助函数 得到元素上边与浏览器上边的边距
	var curtop = 0;
	if (obj && obj.offsetParent) {
		while (obj.offsetParent) {
			curtop += obj.offsetTop;
			obj = obj.offsetParent;
		}
	} else if (obj && obj.y) curtop += obj.y;
	return curtop;// + document.body.scrollTop - document.body.clientTop;
}

var StringBuilder = function(){
	var self = this;
	var array = [];
	this.append = function(str){
		array.push(str);
	}
	this.toString = function(){
		return array.join("");
	}
}

function setCookie(sName,sValue,oExpires,sPath,sDomain,bSecure){
	var sCookie = sName + "=" + encodeURIComponent(sValue);
	
	if(oExpires)
		sCookie += "; expires=" + oExpires.toGMTString();
	if(sPath)
		sCookie += "; path=" + sPath;
	if(sDomain)
		sCookie += "; domain=" + sDomain;
	if(bSecure)
		sCookie += "; secure";

    var Days = 30; //此 cookie 将被保存 30 天
    var exp  = new Date();    //new Date("December 31, 9998");
    exp.setTime(exp.getTime() + Days*24*60*60*1000);
//    document.cookie = sCookie + ";expires=" + exp.toGMTString();
		
	document.cookie = sCookie;

}

function getCookie(sName){
	var sRE = "(?:; )?" + sName + "=([^;]*);?";
	var oRE = new RegExp(sRE);
	if(oRE.test(document.cookie))
		return decodeURIComponent(RegExp["$1"]);
	else
		return null;
}

var dragGhost = document.createElement("div");
dragGhost.style.border = "dashed 1px #CCCCCC";
dragGhost.style.background = "white";
dragGhost.style.display = "none";
dragGhost.style.margin = "10px";

var isIE = document.all;

//------------------------Start Here------------------------
var XDrag = new Object();
XDrag.tabs = [];//所有的tab集合 每个元素是tab对象 tab.elm才是html对象
XDrag.pages = [];//所有的页的集合 每个元素是html对象
XDrag.selectedTab = null;//当前选中的tab
XDrag.changeTabTimeout = 1000;

XDrag.save = function(){//将结果保存到cookie
	var jsonString = new StringBuilder();
	jsonString.append('{"pages":[')
	for(var i=0;i<XDrag.pages.length;i++){//遍历每一页
		jsonString.append("[");
		var page = XDrag.pages[i];
		for(var j=0;j<page.childNodes.length;j++){//遍历每一列
			var column = page.childNodes[j];
			if(column.nodeName.toLowerCase() != "div")
				continue;
			jsonString.append("[");
			for(var k=0;k<column.childNodes.length;k++){//遍历每一列的module
				var module = column.childNodes[k];
				if(module.nodeName.toLowerCase() != "div" || module.style.display == "none")
					continue;
				jsonString.append('"' + module.id + '",');
			}
			jsonString.append("],");
		}
		jsonString.append("],");
	}
	jsonString.append(']}');
	var jsonStr = jsonString.toString();
	var cookieValue = jsonStr.replace(/,]/g,"]");

	setCookie(cookieName,cookieValue,null,null,null,false);
}

XDrag.defaultLayout = '{"pages":[[["newTask","activedTask","historyTask","newMessage"],["projectPlan","contractPlan","paymentAlert"],["calendar","schedule","outSide"]]]}';

XDrag.readFromCookie = function(){//读取cookie
	var value = getCookie(cookieName) || XDrag.defaultLayout;
	var obj = eval('(' + value + ')');
	return obj;
}

XDrag.init = function(){
	var tabs = document.getElementById("tabs");
	for(var i=0;i<tabs.childNodes.length;i++){
		if(tabs.childNodes[i].nodeName.toLowerCase() == "li")
			XDrag.tabs.push(new tab(tabs.childNodes[i]));
	}//初始化所有的tab页标签
	
	var pages = document.getElementById("pages");
	for(var i=0;i<pages.childNodes.length;i++){
		if(pages.childNodes[i].nodeName.toLowerCase() == "div"){
			XDrag.pages.push(pages.childNodes[i]);
		}
	}//找到所有的页
	data = XDrag.readFromCookie();
	for(var i=0;i<XDrag.tabs.length;i++){
		if(XDrag.pages[i]){
			var page =  XDrag.pages[i];//page
			XDrag.tabs[i].page = page;
			page.tab = XDrag.tabs[i];//page和tab可以互相引用
			
			var pageData = data.pages[i];
			var columnIndex = 0;
			for(var j=0;j<page.childNodes.length;j++){
				var column = page.childNodes[j];//column
				if(column.nodeName.toLowerCase() == "div"){
					var columnData = pageData[columnIndex];
					if(!page.columns)
						page.columns = [];
					page.columns.push(column);
					column.page = page;
					
					for(var k=0;k<columnData.length;k++){
						
						var moduleElm = document.getElementById(columnData[k]);
						moduleElm.column = column;
						column.appendChild(moduleElm);
						new module(moduleElm);
						//if(column.childNodes[k].nodeName.toLowerCase() == "div"){
						//	var moduleElm = column.childNodes[k];//module
						//	moduleElm.column = column;
						//	new module(moduleElm);
						//	
						//	moduleIndex ++;
						//}
					}
					columnIndex ++;
				}
			}
		}
	}//把tab页标签和页内容对应起来 初始化每个module
	
	XDrag.tabs[0].select();//默认选中第一个页签
}
var tab = function(tabElm){
	var self = this;
	this.page = null;
	this.elm = tabElm;
	this.select = function(){
		if(!this.page)
			return;
		if(XDrag.selectedTab){
			XDrag.selectedTab.elm.className = "";
			XDrag.selectedTab.page.style.display = "none";
		}
		this.elm.className = "tabSelected";
		this.page.style.display = "block";
		XDrag.selectedTab = this;
	}
	this.elm.onclick = function(){
		//this.select(); 这个时候this指的是this.elm
		self.select();
	}
	this.elm.onmouseover = function(){
		if(XDrag.selectedTab != self)
			this.className = "tabHover";
	}
	this.elm.onmouseout = function(){
		if(XDrag.selectedTab != self)
			this.className = "";
	}
}
var module = function(moduleElm){
	var self = this;
	this.elm = moduleElm;
	this.elm.module = this;
	this.column = moduleElm.column;
	this.page = this.column.page;
	this.handle = this.elm.getElementsByTagName("h3")[0];
	
	if(this.handle && this.elm){
		Drag.init(this.handle,this.elm);
	}else{
		return;
	}
	this.elm.onDragStart = function(left,top,mouseX,mouseY){
		//开始拖动的时候设置透明度
		
		this.style.opacity = "0.5";
		this.style.filter = "alpha(opacity=50)";
		dragGhost.style.height = (isIE?this.offsetHeight:this.offsetHeight - 2) + "px";
		
		//this指的是item
		
		this.style.width = this.offsetWidth + "px";//因为初始的width为auto
		this.style.left = (findPosX(this) - 5) + "px";
		this.style.top = (findPosY(this) - 5) + "px";
		this.style.position = "absolute";
		
		//将ghost插入到当前位置
		
		dragGhost.style.display = "block";
		self.column.insertBefore(dragGhost,this);
		
		//记录每一列的左边距 在拖动过程中判断拖动对象所在的列会用到
		
		this.columnsX = [];
		for(var i=0;i<self.column.page.columns.length;i++){
			this.columnsX.push(findPosX(self.column.page.columns[i]));
		}
			
	}
	this.elm.onDrag = function(left,top,mouseX,mouseY){
		this.currentTab = null;
		//判断是否在tab上
		
		for(var i=0;i<XDrag.tabs.length;i++){
			var tabElm = XDrag.tabs[i].elm;
			if((findPosX(tabElm) < mouseX) &&
				(findPosX(tabElm) + tabElm.offsetWidth > mouseX) &&
				(findPosY(tabElm) < mouseY) &&
				(findPosY(tabElm) + tabElm.offsetHeight > mouseY)){
				this.currentTab = XDrag.tabs[i];
				break;
			}
		}
		if(this.currentTab != null){
			if(dragGhost.parentNode)
				dragGhost.parentNode.removeChild(dragGhost);

			function changeTab(){
				//先得把module放到当前的这一页
				//否则会随着tab的改变而消失
				
				var currentColumn = self.elm.currentTab.page.columns[0];
				
				var flag = false;
				for(var i=0;i<currentColumn.childNodes.length;i++){
					if(currentColumn.childNodes[i].nodeName.toLowerCase() == "div"){
						currentColumn.insertBefore(self.elm,currentColumn.childNodes[i]);
						flag = true;
						break;
					}
				}
				if(!flag)
					currentColumn.appendChild(this);
				self.column = currentColumn;//将拖动的module添加到这一页的第一列 因为display还为absolute 所以module还跟着鼠标在走
				
				self.elm.currentTab.select();
				XDrag.changeTabTimeoutId == null;
			}
			
			//如果Timeout不为空（防止重复设置Timeout） 而且移动到的tab不是当前的tab（如果是当前tab则不需要改变tab页了）
			
			if(XDrag.changeTabTimeoutId == null && this.currentTab != XDrag.selectedTab)
				XDrag.changeTabTimeoutId = setTimeout(changeTab,XDrag.changeTabTimeout);
			return;//如果鼠标在tab上 则不必理会页面内的移动了
			
		}
		
		//以下是计算在页面内拖拽的代码
		
		clearTimeout(XDrag.changeTabTimeoutId);
		XDrag.changeTabTimeoutId = null;//既然鼠标都没有在tab上了 当然就应该清空timeout了
		//先要判断在哪一列移动
		
		var columnIndex = 0; 
		
		for(var i=0;i<this.columnsX.length;i++){
			if((left + this.offsetWidth/2) > this.columnsX[i]){
				columnIndex = i;
			}
		}
		//如果columnIndex在循环中没有被赋值 则表示当前拖动对象在第一列的左边
		//此时也把它放到第一列
		
		var column = self.column.page.columns[columnIndex];
		
		if(self.column != column){
			//之前拖动对象不在这个列
			//将ghost放置到这一列的最下方
			
			//如果已经跨页拖放了 也会执行这里的
			
			column.appendChild(dragGhost);
			self.column = column;
		}
		
		//然后在判断放在这一列的什么位置
		
		var currentNode = null;
		for(var i=0;i<self.column.childNodes.length;i++){
			if(self.column.childNodes[i].className == "item"
			&& self.column.childNodes[i] != this//不能跟拖动元素自己比较 否则不能在本列向下移动
			
			&& top < findPosY(self.column.childNodes[i])){//从上到下找到第一个比拖动元素的上边距大的元素
			
				currentNode = self.column.childNodes[i];
				break;
			}
		}
		if(currentNode)
			self.column.insertBefore(dragGhost,currentNode);
		else//拖到最下边 没有任何一个元素的上边距比拖动元素的top大 则添加到列的最后
		
			self.column.appendChild(dragGhost);
	}
	this.elm.onDragEnd = function(left,top,mouseX,mouseY){
		if(this.currentTab != null){
			//this.currentTab != null表示鼠标拖拽的module在tab上释放 无论这个时候tab是否因为鼠标的停留而转换了页签
			
			clearTimeout(XDrag.changeTabTimeoutId);
			XDrag.changeTabTimeoutId = null;
			var firstColumn = this.currentTab.page.columns[0];
			var flag = false;
			for(var i=0;i<firstColumn.childNodes.length;i++){
				if(firstColumn.childNodes[i].nodeName.toLowerCase() == "div"){
					firstColumn.insertBefore(this,firstColumn.childNodes[i]);
					flag = true;
					break;
				}
			}
			if(!flag)
				firstColumn.appendChild(this);
			self.column = firstColumn;
		}else{
			self.column.insertBefore(this,dragGhost);
		}
		this.style.opacity = "1";
		this.style.filter = "alpha(opacity=100)";
		
		this.style.position = "static";
		this.style.display = "block";
		this.style.width = "auto";
		dragGhost.style.display = "none";
		self.page = self.column.page;//需要手动更新(奇怪 self.page难道是个值类型)
		//也可以不要最后这一句 仅仅是为了数据的完整性
		
		if(this.id == "calendar"){
			location=location;
		}
	}
}


window.onload=function(){
	XDrag.init();
	window.onbeforeunload = RunOnBeforeUnload;

//	var mCal;
//	mCal = new dhtmlxCalendarObject('calendar', true, {isWinHeader: true, isWinDrag: false});
//	mCal.setSkin("dhx_skyblue");
//	mCal.setHolidays('<%=holidays%>');
}

function RunOnBeforeUnload(){
	XDrag.save();
}


//alert(cookieName +"=" + getCookie(cookieName));


</script>
