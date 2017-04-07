Ext.ns('com.xmis.gczl');

com.xmis.gczl.excretiongrid = Ext.extend(Ext.grid.GridPanel, {
	border : false,
	id:'excretiongrid',
	autoWidth : true,
	height : 295,
	loadMask : true,
	autoScroll : true,
	loadMask : {
		msg : '正在加载数据，请稍侯……'
	},
	initComponent : function() {
		var reRecord1 = Ext.data.Record.create([{
					name : 'time',
					type : 'string'
				}, {
					name : 'value',
					type : 'float'
				}]);
		var store = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/applyManageMaint.do?method=getGczlInfoExcretion',
						method : "POST"
					}),
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, reRecord1),
			remoteSort : true,
			baseParams : {
				b_date : new Date(new Date().getFullYear(), 0, 1),
				e_date : Ext.util.Format.date(new Date(), 'Y-m-d'),
				start : 0,
				limit : 10
			},
			autoLoad : true,
			listeners : {
				load : function() {
					Ext.Ajax.request({
						url : __cxtPath
								+ '/applyManageMaint.do?method=getExcretionSum',
						success : function(response, action) {
							var action = Ext.decode(response.responseText);
							var sum = action.attributes.sum;
							if (sum != '' && sum != null)
								Ext.getCmp('sum')
										.setText('<font color=red><b>排泄量：'
												+ Ext.util.Format.number(sum,
														"0,0")
												+ '/m³</b><font>');
						},
						failure : '',
						params : {
							b_year : Ext.getCmp('start').getValue(),
							e_year : Ext.getCmp('end').getValue()

						}
					});
				}
			}
		});
		var cm2 = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
					width : 60,
					header : '序号'
						// width : 50
					}), {
					header : '测量时间',
					dataIndex : 'time',
					width : 130,
					align : 'center'
				}, {
					header : '测量值(立方米)',
					dataIndex : 'value',
					width : 100,
					align : 'center'
				}]);
		this.tbar = ['&nbsp;开始日期：', new Ext.form.DateField({
							ref : '../startDate',
							id:'start',
							width : 90,
							format : 'Y-m-d',
							showToday : true,
							value : new Date(new Date().getFullYear(), 0, 1),
							anchor : '100%'
						}), '&nbsp;结束日期：', new Ext.form.DateField({
							width : 90,
							id:'end',
							ref : '../endDate',
							format : 'Y-m-d',
							showToday : true,
							value : new Date(),
							anchor : '100%'
						}), '-', {
					ref : '../searchBtn',
					iconCls : 'searchIcon',
					text : '查询',
					handler : function(b, e) {
						var start = Ext.getCmp('start').getValue();
						var end = Ext.getCmp('end').getValue();
						if (start >= end) {
							Ext.Msg.alert('提示', '结束日期必须晚于开始日期！');
							return;
						} else {
							Ext.getCmp('excretiongrid').store
									.setBaseParam(
											'b_date',

											start);
							Ext.getCmp('excretiongrid').store.setBaseParam(
									'e_date',end);
							Ext.getCmp('excretiongrid').store.setBaseParam(
									'limit', 10), 
							Ext.getCmp('excretiongrid').store
									.reload({
												params : {
													start : 0
												}
											});
						}
					}
				}, '->', {
					id : 'sum',
					text : ''
				}];
		
		this.cm = cm2;
		this.bbar = new Ext.PagingToolbar({
					pageSize : 10,
					store : store,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					plugins : [new Ext.ux.PagelimitCombo()]
				});
				this.store = store;
		com.xmis.gczl.excretiongrid.superclass.initComponent.call(this);
	}

});