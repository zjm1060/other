Ext.ns('com.xmis.gczl');

com.xmis.gczl.everyMonthLevel = Ext.extend(Ext.grid.GridPanel, {
	// border : false,
	id : 'everyMonthLevel',
	autoWidth : true,
	height : 295,
	loadMask : true,
	autoScroll : true,
	loadMask : {
		msg : '正在加载数据，请稍侯……'
	},
	initComponent : function() {
		var reRecord = Ext.data.Record.create([{
					name : '年份',
					mapping : '年份'
				}, {
					name : '项目',
					mapping : '项目'
				}, {
					name : '1月',
					mapping : '1月'
				}, {
					name : '2月',
					mapping : '2月'
				}, {
					name : '3月',
					mapping : '3月'
				}, {
					name : '4月',
					mapping : '4月'
				}, {
					name : '5月',
					mapping : '5月'
				}, {
					name : '6月',
					mapping : '6月'
				}, {
					name : '7月',
					mapping : '7月'
				}, {
					name : '8月',
					mapping : '8月'
				}, {
					name : '9月',
					mapping : '9月'
				}, {
					name : '10月',
					mapping : '10月'
				}, {
					name : '11月',
					mapping : '11月'
				}, {
					name : '12月',
					mapping : '12月'
				}]);
		var store1 = new Ext.data.GroupingStore({
			groupField : '年份',
			proxy : new Ext.data.HttpProxy({
				url : __cxtPath
						+ '/applyManageMaint.do?method=getGczlInfoEveryMonthLevel',
				method : "POST"
			}),
			reader : new Ext.data.JsonReader({
						root : 'rows'
					}, reRecord),
			remoteSort : true,
			baseParams : {
				syear : new Date().getFullYear() - 1,
				eyear : new Date().getFullYear(),
				type : '1' // 1 为最大，2为最小值，3为平均值
			},
			autoLoad : true
		});
		var cm1 = new Ext.grid.ColumnModel([{
					hidden : true,
					header : '',
					dataIndex : '年份'
				}, {
					header : '项目',
					sortable : true,
					align:'center',
					dataIndex : '项目',
					width:60,
					sortable : false
				}, {
					header : '1月',
					sortable : true,
					dataIndex : '1月',
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}, {
					header : '2月',
					sortable : true,
					dataIndex : '2月',
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}, {
					header : '3月',
					dataIndex : '3月',
					sortable : true,
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}, {
					header : '4月',
					dataIndex : '4月',
					sortable : true,
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}, {
					header : '5月',
					dataIndex : '5月',
					sortable : true,
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}, {
					header : '6月',
					dataIndex : '6月',
					sortable : true,
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}, {
					header : '7月',
					dataIndex : '7月',
					sortable : true,
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}, {
					header : '8月',
					dataIndex : '8月',
					sortable : true,
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}, {
					header : '9月',
					dataIndex : '9月',
					sortable : true,
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}, {
					header : '10月',
					dataIndex : '10月',
					sortable : true,
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}, {
					header : '11月',
					dataIndex : '11月',
					sortable : true,
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}, {
					header : '12月',
					dataIndex : '12月',
					sortable : true,
					renderer: formarValue,
					align:'center',
					width:50,
					sortable : false
				}]);
		this.tbar = ['&nbsp;开始：', {
					xtype : 'combo',
					id : 'syear',
					width : 80,
					displayField : 'syear',
					valueField : 'syear',
					mode : 'local',
					triggerAction : 'all',
					editable : false,
					value : new Date().getFullYear() - 1,
					store : new Ext.data.ArrayStore({
								fields : [{
											name : 'syear'
										}],
								data : []
							}),
					listeners : {
						'beforerender' : function() {
							var newyear = new Date().getFullYear();
							var yearlist = [];
							for (var i = newyear; i >= 1960; i--) {
								yearlist.push([i]);
							}
							this.store.loadData(yearlist);
						}
					}

				}, '&nbsp;&nbsp;结束：', {
					xtype : 'combo',
					id : 'eyear',
					width : 80,
					displayField : 'eyear',
					valueField : 'eyear',
					mode : 'local',
					triggerAction : 'all',
					editable : false,
					value : new Date().getFullYear(),
					store : new Ext.data.ArrayStore({
								fields : [{
											name : 'eyear'
										}],
								data : []
							}),
					listeners : {
						'beforerender' : function() {
							var newyear = new Date().getFullYear();
							var yearlist = [];
							for (var i = newyear; i >= 1960; i--) {
								yearlist.push([i]);
							}
							this.store.loadData(yearlist);
						}
					}

				}, '&nbsp;&nbsp;&nbsp;&nbsp;', {
					xtype : 'radiogroup',
					fieldLabel : 'Auto Layout',
					id : 'type',
					items : [{
								boxLabel : '最大值',
								name : 'type',
								inputValue : 1,
								checked : true,
								width : 80
							}, {
								boxLabel : '最小值',
								name : 'type',
								inputValue : 2,
								width : 80
							}, {
								boxLabel : '平均值',
								name : 'type',
								inputValue : 3,
								width : 80
							}]
				}, '&nbsp;&nbsp;', {
					ref : '../searchBtn',
					iconCls : 'searchIcon',
					text : '查询',
					handler : function(b, e) {
						Ext.getCmp('everyMonthLevel').store.setBaseParam(
								'syear', Ext.getCmp('syear').getValue());
						Ext.getCmp('everyMonthLevel').store.setBaseParam(
								'eyear', Ext.getCmp('eyear').getValue());
						Ext.getCmp('everyMonthLevel').store.setBaseParam(
								'type', Ext.getCmp('type').getValue());
						Ext.getCmp('everyMonthLevel').store.reload({
									params : {
										start : 0,
										limit : 0
									}
								});

					}

				}];
		this.cm = cm1;
		this.store = store1;
		this.view = new Ext.grid.GroupingView({
					enableGroupingMenu : false
				});
		function formarValue(value){
			 return Ext.util.Format.number(value,"0,0.00");
		}
		com.xmis.gczl.everyMonthLevel.superclass.initComponent.call(this);
	}
});