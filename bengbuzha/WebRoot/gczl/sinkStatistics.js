Ext.onReady(function() {
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	var cm = new Ext.grid.ColumnModel([ new Ext.grid.RowNumberer({
		width : 50
	}), {
		header : '沉陷部位',
		dataIndex : 'sp_name'
	}, {
		header : '沉陷点',
		dataIndex : 's_point_name'
	}, {
		header : '上次高程',
		align:'right',
		dataIndex : 'last_high_degree',
		sortable : true
	}, {
		header : '本次高程',
		align:'right',
		dataIndex : 'the_high_degree',
		sortable : true
	},{
		header : '沉陷量',
		align:'right',
		dataIndex : 'qty',
		sortable : true,
		renderer: function(v){ 
			if(v == 0.00000){
			 return '0.00';
			}else{
				return Ext.util.Format.number(v,"0,0.00");
			}
		}
	}, {
		header : '备注',
		dataIndex : 'memo',
		sortable : true
	} ]);
	var record = Ext.data.Record.create([ {
		name : 'sp_name'
	}, {
		name : 's_point_name'
	},{
		name : 'last_high_degree'
	},{
		name : 'the_high_degree'
	}, {
		name:'qty'
	},{
		name : 'memo'
	}]);
	var storeGYS = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/gczlAction.do?method=getHightDegree',
			method : "POST"
		}),
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, record),
		remoteSort : true,
		baseParams : {
			start : 0,
			limit : 20
		}
	});
	var exp = new Ext.ux.Export({
				formater : []
	});
	var grid = new Ext.grid.GridPanel({
		region : 'center',
		id : 'grid',
		width : 980,
		border : false,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : storeGYS,
		cm : cm,
		tbar : {
			xtype : 'toolbar',
			items : ['&nbsp;部位名称&nbsp;' ,
					{
						xtype : 'combo',
						allowBlank : false,
						fieldLabel : '部位名称',
						id : 'sp_code',
						hiddenName : 'sp_code',
						value : '01',
						valueField : 'sp_code',
						displayField : 'sp_name',
						mode : 'remote',
						triggerAction : 'all',
						editable : false,
						store : new Ext.data.Store({
							proxy : new Ext.data.HttpProxy({
								url : __cxtPath
										+ '/gczlAction.do?method=getSpCodeList'
							}),
							autoLoad : true,
							reader : new Ext.data.JsonReader({
								root : 'rows',
								fields : [ {
									name : 'sp_code',
									mapping : 'sp_code'
								}, {
									name : 'sp_name',
									mapping : 'sp_name'
								} ]
							}),
							listeners : {
								// 解决在Form中setValue时显示的是valueField而不是displayField的问题
								'load' : function() {
									Ext.getCmp('sp_code').setValue(
											Ext.getCmp('sp_code').value);
								}
							}
						})
					}, '-','&nbsp;上次日期&nbsp;' ,{

						xtype : 'datefield',
						fieldLabel : '上次日期',
						format : 'Y-m-d',
						id : 'last_date',
						name : 'last_date',
						allowBlank : false,
						editable : false
					}, '-', '&nbsp;本次日期&nbsp;' ,{

						xtype : 'datefield',
						fieldLabel : '本次日期',
						format : 'Y-m-d',
						id : 'the_date',
						name : 'the_date',
						allowBlank : false,
						editable : false
					}, '-',{
						id:'count',
						text:''
					}, {
						ref : '../searchBtn',
						iconCls : 'searchIcon',
						text : '查询',
						handler : function(b, e) {
							var last_date = Ext.getCmp('last_date').getValue();
							var the_date = Ext.getCmp('the_date').getValue();
							if(last_date == ''||the_date ==''){
								Ext.Msg.alert('提示','查询日期不能为空！');
								return;
							}
							var sp_code = Ext.getCmp('sp_code').getValue();
							Ext.getCmp('count').setText('间隔天数：'+(the_date-last_date)/1000/3600/24);
							grid.store.setBaseParam('last_date', last_date);
							grid.store.setBaseParam('the_date', the_date);
							grid.store.setBaseParam('sp_code', sp_code);
							grid.store.load({
								params : {
									start : 0
								}
							});

						}
					} ]
		},
		bbar : new Ext.PagingToolbar({
			pageSize : 20,
			store : storeGYS,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : "无显示数据",
			plugins : [ new Ext.ux.PagelimitCombo() ],
			items : [ new Ext.Button({
				text : this.text || '导出Excel',
				iconCls : 'xls',
				menu : {
					items : [
							{
								text : '导出本页数据',
								handler : function() {
									exp.onExport(grid, '沉陷资料统计表', true);
								}
							},
							{
								text : '导出所有数据',
								handler : function() {
									Ext.Msg.confirm('确认',
											'导出所有数据可能时间较长，请谨慎选择，是否继续?',
											function(buttonId) {
												if (buttonId == 'yes') {
													exp.onExport(grid, '沉陷资料统计表',
															false);
												}
											}, this);
								}
							} ]
				}
			}) ]
		})
	});
	new Ext.Viewport({
		layout : 'border',
		items : [ grid ]
	});
});