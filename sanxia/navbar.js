function Nav(config) {
	$.extend(this, config);
	this.currentPlan = 0;
	this.currentPage = 0;
}
Nav.prototype.go = function(index) {
	if (index < 0 || index >= this.plans[this.currentPlan].pages.length) {
		return;
	}
	var plan = this.plans[this.currentPlan];
	var page = this.pages[plan.pages[index]];
	var url = page.url;
	this.currentPage = index;
	// $("#outiframe").attr("src", url);
	Ext.util.Cookies.set('nav_current_plan', this.currentPlan);
	Ext.util.Cookies.set('nav_current_page', this.currentPage);
	location.href = url;
};
Nav.prototype.changePlan = function(index) {
	this.currentPlan = index;
	this.first();
};
Nav.prototype.first = function() {
	this.go(0);
};
Nav.prototype.next = function() {
	this.go(this.currentPage + 1);
};
Nav.prototype.prev = function() {
	this.go(this.currentPage - 1);
};
Nav.prototype.end = function() {
	this.go(this.plans[this.currentPlan].pages.length - 1);
};
Nav.prototype.jump = function(index) {
	this.go(index);
};
Nav.prototype.init = function(json) {
	var nav = this;
	var opts = '';
	for (i = 0; i < this.plans.length; i++) {
		opts += '<option value="' + i + '" '
				+ (this.currentPlan == i ? 'selected="selected" ' : '') + '>'
				+ this.plans[i].name + '</option>';
	}
	$("body")
			.append('<ul class="icons ui-widget ui-helper-clearfix">'
					+ '<li class="ui-state-default ui-corner-all"><span class="ui-icon ui-icon-carat-1-nw"></span></li>');
	$("body")
			.append('<div class="navbar ui-widget-content ui-corner-all">'
					+ '<div class="plan">选择方案：<select class="plans" value="'
					+ this.currentPlan
					+ '">'
					+ opts
					+ '</select></div>'
					+ '<ul id="icons" class="ui-widget ui-helper-clearfix">'
					+ '<li class="first ui-state-default ui-corner-all" title="第一页"><span class="ui-icon ui-icon-seek-first"></span></li>'
					+ '<li class="prev ui-state-default ui-corner-all" title="上一页"><span class="ui-icon ui-icon-seek-prev"></span></li>'
					+ '<li class="goto" title="跳转"><input class="navgoto"></input></li>'
					+ '<li class="next ui-state-default ui-corner-all" title="下一页"><span class="ui-icon ui-icon-seek-next"></span></li>'
					+ '<li class="end ui-state-default ui-corner-all" title="最后一页"><span class="ui-icon ui-icon-seek-end"></span></li>'
					+ '</ul></div>');
	$('div.navbar').css({
				position : 'fixed',
				bottom : 0,
				left : 0,
				'font-size' : '0.8em',
				'font-family' : '微软雅黑,Serif',
				padding : 10,
				'z-index' : 999999
			}).hide();
	$('input.navgoto').css({
				width : 20
			}).attr("value", nav.currentPage + 1);
	$('div.plan').css({
				'padding-bottom' : 5
			});
	$('ul.icons').css({
				position : 'fixed',
				bottom : 0,
				left : 0,
				'z-index' : 999998
			}).mouseover(function() {
				$(this).hide();
				$('div.navbar').show("normal");
			});
	$("div.navbar").bind("mouseleave", function(event) {
				var e = window.event || event;
				if (document.all) {
					if (!e.toElement)
						return;
				} else if (!e.relatedTarget)
					return;
				$(this).hide();
				$('ul.icons').show();
			});
	$('select.plans').change(function() {
				nav.changePlan($(this).val());
				$('input.navgoto').attr('value', nav.currentPage + 1);
			});
	$('ul li.first').click(function() {
				nav.first();
				$('input.navgoto').attr('value', nav.currentPage + 1);
			});
	$('ul li.next').click(function() {
				nav.next();
				$('input.navgoto').attr('value', nav.currentPage + 1);
			});
	$('ul li.prev').click(function() {
				nav.prev();
				$('input.navgoto').attr('value', nav.currentPage + 1);
			});
	$('ul li.end').click(function() {
				nav.end();
				$('input.navgoto').attr('value', nav.currentPage + 1);
			});
	$('input.navgoto').keydown(function(event) {
				if (event.keyCode == 13) {
					nav.jump($(this).val() - 1);
				}
			});
	$(document).keydown(function(event) {
				switch (event.keyCode) {
					case 37 :
						nav.prev();
						$('input.navgoto').attr('value', nav.currentPage + 1);
						break;
					case 39 :
						nav.next();
						$('input.navgoto').attr('value', nav.currentPage + 1);
						break;
				}
			});
	setInterval(function(){
		$('ul.icons').focus();
		if(console){
			console.info('focus success');
		}
	},3000)
};
$(function() {
			var contextPath = 'http://10.66.12.213:81/';
			Ext.Ajax.request({
						method : 'post',
						url : contextPath + 'webservice.asmx/GetPlanPageString',
						success : function(response) {
							var result = Ext.DomQuery.selectNode('string',
									response.responseXML).text;
							if (Ext.isEmpty(result)) {
								Ext.Msg.alert('出错', '加载数据出错，请刷新页面重试。');
								return;
							}
							var json = Ext.decode(result);
							var nav = new Nav(json);
							var plan = Ext.util.Cookies.get('nav_current_plan');
							var page = Ext.util.Cookies.get('nav_current_page');
							if (plan != null && page != null) {
								nav.currentPlan = Number(plan);
								nav.currentPage = Number(page);
							}
							nav.init();
							// nav.first();

						}
					});
		});