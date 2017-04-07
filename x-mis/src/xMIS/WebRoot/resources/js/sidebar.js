function initSideBar()
{
	var sb = getCookie('sidebar');
    //alert(sb);
	if(sb == 'false'){
		clickSideBar();
    }
}
function clickSideBar()
{
    switchImage('img_sidebar',url_sidebar_img_left,url_sidebar_img_right);
    expendCollapseLayers('sidebar','','');
    tmd_SwitchDiv('sideimg', 1, 161);
    tmd_SwitchDiv('content',20,185);
    setCookie("sidebar",isLayerExpended('sidebar'),null,contextPath);
}

function hideSideBar()
{
    if(isLayerExpended('sidebar')){
    	clickSideBar();
    }
}
var perf_sale_status = 0 ;
function perf_saleMenu()
{
  if(perf_sale_status==0) {
    document.all.img_perf_sale.src = url_collapse_img;
    document.all.perf_sale.style.display = "none";
    perf_sale_status = 1;
  }else{
    document.all.img_perf_sale.src = url_expand_img;
    document.all.perf_sale.style.display = "";
    perf_sale_status = 0;
  }
}
var perf_vendor_status = 0 ;
function perf_vendorMenu()
{
  if(perf_vendor_status==0) {
    document.all.img_perf_vendor.src = url_collapse_img;
    document.all.perf_vendor.style.display = "none";
    perf_vendor_status = 1;
  }else{
    document.all.img_perf_vendor.src = url_expand_img;
    document.all.perf_vendor.style.display = "";
    perf_vendor_status = 0;
  }
}
var perf_program_status = 0;
function perf_programMenu()
{
  if(perf_program_status==0) {
    document.all.img_perf_program.src = url_collapse_img;
    document.all.perf_program.style.display = "none";
    perf_program_status = 1;
  }else{
    document.all.img_perf_program.src = url_expand_img;
    document.all.perf_program.style.display = "";
    perf_program_status = 0;
  }
}
var task_status = 0 ;
function taskMenu()
{
  if(task_status==0) {
    document.all.img_tasks.src = url_collapse_img;
    document.all.tasks.style.display = "none";
    task_status = 1;
  }else{
    document.all.img_tasks.src = url_expand_img;
    document.all.tasks.style.display = "";
    task_status = 0;
  }
}
var alertstatus = 0 ;
function alertMenu()
{
  if(alert_status==0) {
    document.all.img_alerts.src = url_collapse_img;
    document.all.alerts.style.display = "none";
    task_status = 1;
  }else{
    document.all.img_alerts.src = url_expand_img;
    document.all.alerts.style.display = "";
    task_status = 0;
  }
}
var team_perf_status = 0 ;
function team_perfMenu(){
  if(team_perf_status==0) {
    document.all.img_team_perf.src = url_collapse_img;
    document.all.team_perf.style.display = "none";
    team_perf_status = 1;
  }else{
    document.all.img_team_perf.src = url_expand_img;
    document.all.team_perf.style.display = "";
    team_perf_status = 0;
  }
}

var tools_status = 0 ;
function toolMenu()
{
  if(tools_status==0) {
    document.all.img_tools.src =url_collapse_img;
    document.all.tools.style.display = "none";
    tools_status = 1;
  }else{
    document.all.img_tools.src = url_expand_img;
    document.all.tools.style.display = "";
    tools_status = 0;
  }
}
var team_status = 0 ;
function teamMenu()
{
  if(team_status==0) {
    document.all.img_team.src =url_collapse_img;
    document.all.team.style.display = "none";
    team_status = 1;
  }else{
    document.all.img_team.src = url_expand_img;
    document.all.team.style.display = "";
    team_status = 0;
  }
}





/**
  * Show order type drop down for order search at side bar, please refer to myCIS\src\web\common\sideBarSearchOrder.jsp for usage
**/
function OrderTypeDropDown(middelcell_1, middelcell_2, orderTypeDropDownDiv, orderTypeElement, orderNoElement){
	this.selectIconVisible = false;
	this.orderTypeDropdownVisible = false;
	this.currentTarget = null;
	
	this.initialize(middelcell_1, middelcell_2, orderTypeDropDownDiv, orderTypeElement, orderNoElement);
}

OrderTypeDropDown.prototype = {
	initialize: function(middelcell_1, middelcell_2, orderTypeDropDownDiv, orderTypeElement, orderNoElement){
		this.middelcell_1 = middelcell_1;
		this.middelcell_2 = middelcell_2;
		this.orderTypeElement = orderTypeElement;
		this.orderNoElement = orderNoElement;
		this.orderTypeDropDownDiv = orderTypeDropDownDiv;
		
		
		this._bindEvent(this._showSelectIcon, [this], this.orderTypeElement, 'mousemove');
		this._bindEvent(this._hideSelectIcon, [this], this.orderTypeElement, 'mouseout');
		this._bindEvent(this._showSelectIcon, [this], this.middelcell_1, 'mousemove');
		this._bindEvent(this._hideSelectIcon, [this], this.middelcell_1, 'mouseout');
		this._bindEvent(this._showSelectIcon, [this], this.middelcell_2, 'mousemove');
		this._bindEvent(this._hideSelectIcon, [this], this.middelcell_2, 'mouseout');
		
		this._bindEvent(this._switchDropdownDiv, [this], this.middelcell_2.firstChild, 'click');
		
		this._bindEvent(this._showOrderTypeDropdown, [this], document, 'click');

		var table = this.orderTypeDropDownDiv.firstChild;
		for(var i=0; i<table.rows.length; i++){
			var row = table.rows[i];
			this._bindEvent(this._itemMouseove, [this, row], row, 'mousemove');
			this._bindEvent(this._itemClick, [this, row], row, 'click');
			row.style.cursor='default';
		}
		
		//document.attachEvent ('onclick', function(){});

	},
	
	_bindEvent: function(method, argu, target, eventname){
		var _method = method;
		var _target = target;
		var _argument = argu;
		var _t = function(){
			return _method.apply(_target, _argument);
		};

		this._addEvent(target, eventname, _t );
	},
	
	_showSelectIcon: function(orderTypeDropDown){
		if(orderTypeDropDown.selectIconVisible == false){
			orderTypeDropDown.showSelectIcon(true);
		}
	},
	
	_hideSelectIcon: function(orderTypeDropDown){
		if(orderTypeDropDown.selectIconVisible == true && orderTypeDropDown.orderTypeDropdownVisible==false){
			orderTypeDropDown.showSelectIcon(false);
		}
	},
	
	_itemMouseove: function(orderTypeDropDown, row){
		if(orderTypeDropDown.selectIconVisible == false){
			orderTypeDropDown.showSelectIcon(true);
		}
		orderTypeDropDown._orderTypeDropdownMouseOut(orderTypeDropDown.currentTarget);
		row.style.backgroundColor = "#316AC5";
		row.style.color = "#FFFFFF";
		orderTypeDropDown.currentTarget = row;
	},
	
	_itemClick: function(orderTypeDropDown, row){
		orderTypeDropDown.orderTypeElement.value = row.getAttribute('orderType');
		orderTypeDropDown.orderNoElement.focus();
	},
	
	_switchDropdownDiv: function(orderTypeDropDown){
		orderTypeDropDown.showOrderTypeDropdown(!orderTypeDropDown.orderTypeDropdownVisible);	
		window.event.returnValue  = false;
		return false;
	},
	
	_showOrderTypeDropdown: function(orderTypeDropDown){
		if(orderTypeDropDown.orderTypeDropdownVisible && (window.event.returnValue==null || window.event.returnValue==true)){
			orderTypeDropDown.showOrderTypeDropdown(false);
		}
	},

	
	_showElement: function(element, visible){
		if(visible){
			element.style.position = '';
			element.style.top = '0px';
			element.style.visibility = 'visible';
		}else{
			element.style.visibility = 'hidden';
			element.style.position = 'absolute';
			element.style.top = '-1000px';
		}
	},
	
	_addEvent: function(element, event, method){
		element.attachEvent('on' + event, method);
	},
	
	showSelectIcon: function(visible){
		if(visible){
			this._showElement(middelcell_1, false);
			this._showElement(middelcell_2, true);
		}else{
			this._showElement(middelcell_2, false);
			this._showElement(middelcell_1, true);
		}
		
		this.selectIconVisible = visible;
	},
	
	showOrderTypeDropdown: function(visible){
		if(visible){
			this.orderTypeDropDownDiv.style.top = (this._getOffsetTopforOrderTypeDropdown(this.orderTypeElement)+this.orderTypeElement.clientHeight+2)+'px';
			this.orderTypeDropDownDiv.style.visibility = 'visible';
		}else{
			this.orderTypeDropDownDiv.style.top = '-1000px';
			this.orderTypeDropDownDiv.style.visibility = 'hidden';
			
			if(this.selectIconVisible == true){
				this.showSelectIcon(false);
			}
		}
		
		this._orderTypeDropdownMouseOut(this.currentTarget);
		this.orderTypeDropdownVisible = visible;
	},
	_getOffsetTopforOrderTypeDropdown: function(el) {
		var ot=el.offsetTop;
		while((el=el.offsetParent) != null && el.style.position!='absolute') { ot += el.offsetTop; }
		return ot;
	},
	
	_orderTypeDropdownMouseOut: function(target){
		if (target != null) {
			target.style.backgroundColor = "";
			target.style.color = "";
		}
		target = null;
	}
}

