Ext.ns('App.Calendar');
App.Calendar.Lunar = function() {
	/*
version:1.0.0(2011-9-29)
author:dansion#163.com,www.dansion.com/blog/
*/
var lunarDate=function(r){function l(a){for(var c=348,b=32768;b>8;b>>=1)c+=h[a-1900]&b?1:0;return c+(i(a)?(h[a-1899]&15)==15?30:29:0)}function i(a){a=h[a-1900]&15;return a==15?0:a}function o(a){if(!a||!a.getFullYear)return!1;var c=a.getFullYear(),b=a.getMonth(),a=a.getDate();return Date.UTC(c,b,a)>Date.UTC(2101,0,28)||Date.UTC(c,b,a)<Date.UTC(1900,0,31)?!0:!1}var h=[19416,19168,42352,21717,53856,55632,21844,22191,39632,21970,19168,42422,42192,53840,53845,46415,54944,44450,38320,18807,18815,42160,
46261,27216,27968,43860,11119,38256,21234,18800,25958,54432,59984,27285,23263,11104,34531,37615,51415,51551,54432,55462,46431,22176,42420,9695,37584,53938,43344,46423,27808,46416,21333,19887,42416,17779,21183,43432,59728,27296,44710,43856,19296,43748,42352,21088,62051,55632,23383,22176,38608,19925,19152,42192,54484,53840,54616,46400,46752,38310,38335,18864,43380,42160,45690,27216,27968,44870,43872,38256,19189,18800,25776,29859,59984,27480,23232,43872,38613,37600,51552,55636,54432,55888,30034,22176,
43959,9680,37584,51893,43344,46240,47780,44368,21977,19360,42416,20854,21183,43312,31060,27296,44368,23378,19296,42726,42208,53856,60005,54576,23200,30371,38608,19195,19152,42192,53430,53855,54560,56645,46496,22224,21938,18864,42359,42160,43600,45653,27951,44448,19299,37759,18936,18800,25776,26790,59999,27424,42692,43759,37600,53987,51552,54615,54432,55888,23893,22176,42704,21972,21200,43448,43344,46240,46758,44368,21920,43940,42416,21168,45683,26928,29495,27296,44368,19285,19311,42352,21732,53856,
59752,54560,55968,27302,22239,19168,43476,42192,53584,62034,54560],g="\u96f6,\u4e00,\u4e8c,\u4e09,\u56db,\u4e94,\u516d,\u4e03,\u516b,\u4e5d,\u5341".split(","),p=["\u521d","\u5341","\u5eff","\u5345","\u25a1"],s="\u7532,\u4e59,\u4e19,\u4e01,\u620a,\u5df1,\u5e9a,\u8f9b,\u58ec,\u7678".split(","),t="\u5b50,\u4e11,\u5bc5,\u536f,\u8fb0,\u5df3,\u5348,\u672a,\u7533,\u9149,\u620c,\u4ea5".split(","),u="\u9f20,\u725b,\u864e,\u5154,\u9f99,\u86c7,\u9a6c,\u7f8a,\u7334,\u9e21,\u72d7,\u732a".split(","),q="\u5c0f\u5bd2,\u5927\u5bd2,\u7acb\u6625,\u96e8\u6c34,\u60ca\u86f0,\u6625\u5206,\u6e05\u660e,\u8c37\u96e8,\u7acb\u590f,\u5c0f\u6ee1,\u8292\u79cd,\u590f\u81f3,\u5c0f\u6691,\u5927\u6691,\u7acb\u79cb,\u5904\u6691,\u767d\u9732,\u79cb\u5206,\u5bd2\u9732,\u971c\u964d,\u7acb\u51ac,\u5c0f\u96ea,\u5927\u96ea,\u51ac\u81f3".split(","),
v=[0,21208,42467,63836,85337,107014,128867,150921,173149,195551,218072,240693,263343,285989,308563,331033,353350,375494,397447,419210,440795,462224,483532,504758],m=r||new Date;this.date=m;this.toLunarDate=function(a){a=a||m;if(o(a)){return"the function[toLunarDate()]range[1900/0/31-2101/0/28]";throw"dateRangeError";}for(var c=a.getFullYear(),b=a.getMonth(),a=a.getDate(),c=(Date.UTC(c,b,a)-Date.UTC(1900,0,31))/864E5,d,b=1900;b<2100&&c>0;b++)d=l(b),c-=d;c<0&&(c+=d,b--);lunarYear=b;_isLeap=!1;leap=
i(lunarYear);for(b=1;b<13&&c>0;b++)leap>0&&b==leap+1&&_isLeap==!1?(b--,_isLeap=!0,d=i(lunarYear)?(h[lunarYear-1899]&15)==15?30:29:0):d=h[lunarYear-1900]&65536>>b?30:29,_isLeap==!0&&b==leap+1&&(_isLeap=!1),c-=d;c==0&&leap>0&&b==leap+1&&(isLeap?_isLeap=!1:(_isLeap=!0,b--));c<0&&(c+=d,b--);lunarMonth=b-1;lunarDay=c+1;return{y:lunarYear,m:lunarMonth,d:lunarDay,leap:leap,isleep:_isLeap,toString:function(){var a=_isLeap?"(\u95f0)":"",b=g[parseInt(lunarYear/1E3)]+g[parseInt(lunarYear%1E3/100)]+g[parseInt(lunarYear%
100/10)]+g[parseInt(lunarYear%10)],c=parseInt((lunarMonth+1)/10)==0?"":p[1];c+=g[parseInt((lunarMonth+1)%10)];var d=p[parseInt(lunarDay/10)];d+=parseInt(lunarDay%10)==0?"":g[parseInt(lunarDay%10)];return""+b+"\u5e74"+c+"\u6708"+a+d+"\u65e5"}}};this.toSolar=function(){if(arguments.length==0)return m;else{var a,c,b;arguments[0]&&(a=arguments[0]);c=arguments[1]?arguments[1]:0;b=arguments[2]?arguments[2]:1;for(var d=0,e=1900;e<a;e++){var f=l(e);d+=f}for(e=0;e<c;e++)f=h[a-1900]&65536>>e?30:29,d+=f;d+=
b-1;return new Date(Date.UTC(1900,0,31)+d*864E5)}};this.ganzhi=function(a){function c(a,b){return(new Date(3.15569259747E10*(a-1900)+v[b]*6E4+Date.UTC(1900,0,6,2,5))).getUTCDate()}function b(a){return s[a%10]+t[a%12]}var d=a||m;if(o(d)){return"the function[ganzhi()] date'range[1900/0/31-2101/0/28]";throw"dateRangeError";}var e=d.getFullYear(),f=d.getMonth(),a=d.getDate(),d=d.getHours(),h,g,k,j,n;g=f<2?e-1900+36-1:e-1900+36;k=(e-1900)*12+f+12;h=c(e,f*2);var i=c(e,f*2+1);h=a==h?q[f*2]:a==i?q[f*2+1]:
"";var i=c(e,2),l=c(e,f*2);f==1&&a>=i&&(g=e-1900+36);a+1>=l&&(k=(e-1900)*12+f+13);j=Date.UTC(e,f,1,0,0,0,0)/864E5+25577+a-1;n=j%10%5*12+parseInt(d/2)%12;d==23&&j++;g%=60;k%=60;j%=60;n%=60;return{y:g,m:k,d:j,h:n,jie:h,animal:u[g%12],toString:function(a){var c=b(g)+b(k)+b(j)+b(n);return a?c.substring(0,a):c}}}};

	// ==== 算出农历, 传入日期物件, 传回农历日期物件
	// 该物件属性有 .year .month .day .isleap .yearcyl .daycyl .moncyl
	function lunar(objdate) {
		var d=new lunarDate(objdate).toLunarDate();
		return {year:d.y,month:d.m,day:d.d,isleap:d.isleep};
	}
	function cday(m, d) {
		var nstr1 = new Array('日', '一', '二', '三', '四', '五', '六', '七', '八', '九',
				'十');
		var nstr2 = new Array('初', '十', '廿', '卅', '　');
		var s;
		if (m > 10) {
			s = '十' + nstr1[m - 10];
		} else {
			s = nstr1[m];
		}
		s += '月';
		if (s == "十二月")
			s = "腊月";
		if (s == "一月")
			s = "正月";
		switch (d) {
			case 10 :
				s += '初十';
				break;
			case 20 :
				s += '二十';
				break;
			case 30 :
				s += '三十';
				break;
			default :
				s += nstr2[Math.floor(d / 10)];
				s += nstr1[d % 10];
		}
		return (s);
	}
	return function(y, m, d) {
		var sdobj = new Date(y, m, d);
		var ldobj = new lunar(sdobj);
		var cl = '';
		return cday(ldobj.month, ldobj.day);
	};
}();
App.Calendar.DayRenderer = function() {
	var tpl = '<span style="font-size:16px;font-weight: bold;">{0}</span><p>{1}</p><input type="checkbox" name="holiday" value="{2}" {3} onclick="App.Calendar.changeColor(\'{4}\',{5},{6},this)" />';
	return function(value, metaData, record, rowIndex, colIndex, store) {
		if (value == null) {
			return '';
		}
		if (value.type != 0) {
			metaData.css = "holiday";
		}
		var year = Ext.num(value.date.substr(0, 4));
		var month = Ext.num(value.date.substr(4, 2));
		var day = Ext.num(value.date.substr(6, 2));
		var lunar = App.Calendar.Lunar(year, month, day);
		return String.format(tpl, day, lunar, value.date, (value.type != 0)
						? 'checked="checked"'
						: '', this.id, rowIndex, colIndex);
	};
}();
App.Calendar.Change = function(combo, nav) {
	var store = combo.getStore();
	var value = Ext.num(combo.getValue()) + nav;
	var index = store.find('field1', value);
	combo.setValue(value);
	var record = store.getAt(index);
	combo.fireEvent('select', combo, record);
};
App.Calendar.changeColor = function(panelid, row, col, ck) {
	var grid = Ext.getCmp(panelid).grid;
	var store = grid.getStore();
	var record = store.getAt(row);
	var pname = store.fields.keys[col + 1];
	var day = record.get(pname);
	day.type = ck.checked ? 1 : 0;
	record.set(pname, day);
	record.commit();
};
App.Calendar.Panel = Ext.extend(Ext.form.FormPanel, {
			initComponent : function() {
				this.layout = 'border';
				this.year = this.year || Ext.util.Format.date(new Date(), 'Y');
				this.month = this.month
						|| Ext.util.Format.date(new Date(), 'm');
				this.grid = new Ext.grid.GridPanel({
							region : 'center',
							enableColumnMove : false,
							enableColumnResize : false,
							trackMouseOver : false,
							store : new Ext.data.JsonStore({
										url : String
												.format(
														'{0}/workflow.do?method=getWfCalendar',
														__cxtPath),
										baseParams : {
											year : this.year,
											month : this.month
										},
										root : 'rows',
										idProperty : 'id',
										fields : ['id', 'sunday', 'monday',
												'tuesday', 'wednesday',
												'thursday', 'friday',
												'saturday']
									}),
							cm : new Ext.grid.ColumnModel({
										defaults : {
											width : 100,
											align : 'center',
											menuDisabled : true,
											renderer : {
												fn : App.Calendar.DayRenderer,
												scope : this
											}
										},
										columns : [{
													header : '日',
													dataIndex : 'sunday'
												}, {
													header : '一',
													dataIndex : 'monday'
												}, {
													header : '二',
													dataIndex : 'tuesday'
												}, {
													header : '三',
													dataIndex : 'wednesday'

												}, {
													header : '四',
													dataIndex : 'thursday'

												}, {
													header : '五',
													dataIndex : 'friday'
												}, {
													header : '六',
													dataIndex : 'saturday'
												}]
									}),
							selModel : new Ext.grid.CellSelectionModel()
						});
				this.items = [this.grid];
				var years = [];
				for (i = 1900; i < 2150; i++) {
					years[i - 1900] = i;
				}
				var months = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12];
				this.yearCombo = new Ext.form.ComboBox({
							width : 70,
							store : years,
							triggerAction : 'all',
							editable : false,
							value : this.year,
							listeners : {
								scope : this,
								'select' : function(combo, record) {
									this.year = record.get('field1');
									this.grid.getStore().setBaseParam('year',
											this.year);
									this.grid.getStore().reload();
								}
							}
						});
				this.monthCombo = new Ext.form.ComboBox({
							width : 70,
							store : months,
							triggerAction : 'all',
							editable : false,
							value : this.month,
							listeners : {
								scope : this,
								'select' : function(combo, record) {
									this.month = record.get('field1');
									this.grid.getStore().setBaseParam('month',
											this.month);
									this.grid.getStore().reload();
								}
							}
						});
				this.tbar = [{
					text : '－',
					handler : App.Calendar.Change.createCallback(
							this.yearCombo, -1)
				}, this.yearCombo, '年', {
					text : '＋',
					handler : App.Calendar.Change.createCallback(
							this.yearCombo, 1)
				}, '-', {
					text : '－',
					handler : App.Calendar.Change.createCallback(
							this.monthCombo, -1)
				}, this.monthCombo, '月', {
					text : '＋',
					handler : App.Calendar.Change.createCallback(
							this.monthCombo, 1)
				}, '-', {
					text : '系统管理员确认',
					iconCls : 'save',
					disabled:__emp.userID==-1?false:true,
					scope : this,
					handler : function() {
						this.getForm().submit({
									params : {
										year : this.year,
										month : this.month
									},
									success : function(form, action) {
										Ext.Msg.alert("成功", "保存成功");
									},
									failure : function() {
										Ext.Msg.alert("失败", "保存失败");
									}
								});
					}
				}];
				App.Calendar.Panel.superclass.initComponent.call(this);
			},
			getStore : function() {
				return this.grid.getStore();
			}
		});
Ext.onReady(function() {
			var calpanel = new App.Calendar.Panel({
						region : 'center',
						url : String
								.format('{0}/workflow.do?method=saveHoliday',
										__cxtPath)
					});
			calpanel.getStore().load();
			new Ext.Viewport({
						layout : 'border',
						items : [calpanel]
					});
		});