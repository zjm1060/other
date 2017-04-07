Ext.onReady(function() {
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	var cm = new Ext.grid.ColumnModel([ new Ext.grid.RowNumberer({
		width : 50
	}), {
		header : '测压管',
		dataIndex : 'mpt_name'
	}, {
		header : '测量时间',
		dataIndex : 'measure_date'
	}, {
		header : '管水位',
		align:'right',
		dataIndex : 'tubo_w_level'
	}, {
		header : '闸上水位',
		align:'right',
		dataIndex : 'ap_w_level',
		sortable : true
	},{
		header : '闸下水位',
		align:'right',
		dataIndex : 'bp_w_level',
		sortable : true
	}, {
		header : '灵敏度',
		align:'right',
		dataIndex : 'sensitivity',
		sortable : true
	},{
		header : '备注',
		dataIndex : 'memo',
		sortable : true
	} ]);
	var record = Ext.data.Record.create([ {
		name : 'mpt_name'
	}, {
		name : 'measure_date'
	},{
		name : 'tubo_w_level'
	},{
		name : 'ap_w_level'
	}, {
		name:'bp_w_level'
	},{
		name:'sensitivity'
	},{
		name : 'memo'
	}]);
	var storeGYS = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/gczlAction.do?method=getCeYaStatistics',
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
		},
		listeners:{
			'load' : function() {
				var start_date = Ext.getCmp('start_date').getValue();
				var end_date = Ext.getCmp('end_date').getValue();
			Ext.getCmp('maxAndMin').store.setBaseParam('start_date',start_date);
			Ext.getCmp('maxAndMin').store.setBaseParam('end_date',end_date);
			Ext.getCmp('maxAndMin').store.setBaseParam('mptp_code',Ext.getCmp('mptp_code').getValue());	
			Ext.getCmp('maxAndMin').store.setBaseParam('mpt_name', Ext.getCmp('mpt_name').getValue());	
			Ext.getCmp('maxAndMin').store.load({
								params : {
									start : 0,
									limit : 0
								}
							});}
		}
	});
	var exp = new Ext.ux.Export({
				formater : []
	});
	var grid = new Ext.grid.GridPanel({
		id : 'grid',
		autoWidth : true,
		height:document.documentElement.clientHeight/2,
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
			items : ['&nbsp;测压部位&nbsp;' ,
					{
				xtype : 'combo',
				allowBlank : false,
				fieldLabel : '测压部位',
				id : 'mptp_code',
				// name : 'mptp_name',
				hiddenName : 'mptp_code',
				value : '01',
				valueField : 'mptp_code',
				displayField : 'mptp_name',
				mode : 'remote',
				triggerAction : 'all',
				editable : false,
				store : new Ext.data.Store({
					proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/applyManageMaint.do?method=getMptpCodeList'
					}),
					autoLoad : true,
					reader : new Ext.data.JsonReader({
								root : 'rows',
								fields : [{
											name : 'mptp_code',
											mapping : 'mptp_code'
										}, {
											name : 'mptp_name',
											mapping : 'mptp_name'
										}]
							}),
					listeners : {								
						// 解决在Form中setValue时显示的是valueField而不是displayField的问题
						'load' : function() {
							Ext.getCmp('mptp_code').setValue(Ext.getCmp('mptp_code').value);
						}
					}
				})
			}, '-','&nbsp;测压管名&nbsp;' ,{
				xtype : 'combo',
				allowBlank : false,
				fieldLabel : '测压管名',
				value : '2-1',
				id:'mpt_name',
				name : 'mpt_name',
				valueField : 'mpt_name',
				displayField : 'mpt_name',
				mode : 'local',
				triggerAction : 'all',
				editable : false,
				store : new Ext.data.Store({
					proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/applyManageMaint.do?method=getMptCodeList'
					}),
					baseParams : {
						mptp_code : '01'
					},
					autoLoad : true,
					reader : new Ext.data.JsonReader({
								root : 'rows',
								fields : [{
											name : 'mpt_code',
											mapping : 'mpt_code'
										}, {
											name : 'mpt_name',
											mapping : 'mpt_name'
										}]
							})

				})
			}, '-','&nbsp;开始日期&nbsp;' ,{

				xtype : 'datefield',
				fieldLabel : '开始日期',
				format : 'Y-m-d',
				id : 'start_date',
				name : 'start_date',
				allowBlank : false,
				editable : false
			}, '-', '&nbsp;结束日期&nbsp;' ,{

				xtype : 'datefield',
				fieldLabel : '结束日期',
				format : 'Y-m-d',
				id : 'end_date',
				name : 'end_date',
				allowBlank : false,
				editable : false
			}, '-', {
				ref : '../searchBtn',
				iconCls : 'searchIcon',
				text : '查询',
				handler : function(b, e) {
					var start_date = Ext.getCmp('start_date').getValue();
					var end_date = Ext.getCmp('end_date').getValue();
					if(start_date == ''||end_date ==''){
						Ext.Msg.alert('提示','查询日期不能为空！');
						return;
					}
					var mptp_code = Ext.getCmp('mptp_code').getValue();
					var mpt_name = Ext.getCmp('mpt_name').getValue();
					grid.store.setBaseParam('start_date', start_date);
					grid.store.setBaseParam('end_date', end_date);
					grid.store.setBaseParam('mptp_code', mptp_code);
					grid.store.setBaseParam('mpt_name', mpt_name);
					grid.store.load();
				}
			}]
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
									exp.onExport(grid, '测压水位统计表', true);
								}
							},
							{
								text : '导出所有数据',
								handler : function() {
									Ext.Msg.confirm('确认',
											'导出所有数据可能时间较长，请谨慎选择，是否继续?',
											function(buttonId) {
												if (buttonId == 'yes') {
													exp.onExport(grid, '测压水位统计表',
															false);
												}
											}, this);
								}
							} ]
				}
			}) ]
		})
	});
	var mptp_code = Ext.getCmp("mptp_code");
	var mpt_name = Ext.getCmp("mpt_name");
	mptp_code.on('select', function() {

		mpt_name.setValue('');
		mpt_name.store.setBaseParam('mptp_code',mptp_code.getValue());
		mpt_name.store.load();
	});
	var reRecordGYS = Ext.data.Record.create([{
		name : 'title',
		type : 'string'
	}, {
		name : 'boundary',
		type : 'string'
	}, {
		name : 'measure_date',
		type : 'string'
	}, {
		name : 'space',
		type : 'string'
	}, {
		name : 'VALUE',
		type : 'float',
		convert: function(v){ 
						if(v == 0.00){
						 return '0.00';
						}else{
							return v;
						}
					}
	}]);
	var storeMinAndMax = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
					+ '/gczlAction.do?method=getMaxAndMinCeYa',
					method : "POST"
				}),
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, reRecordGYS),
		baseParams : {}
		
	});
	var maxAndMin = new Ext.grid.PivotGrid({
		border : false,
		//autoWidth : true,
		height : document.documentElement.clientHeight/2,
		loadMask : true,
		autoScroll : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		id:'maxAndMin',
		store:storeMinAndMax,
		measure : 'space',
		aggregator : 'max',
		leftAxis : [{
					width : 80,
					dataIndex : 'title'
				}, {
					dataIndex : 'boundary'
				}, {
					dataIndex : 'VALUE'
				}, {
					dataIndex : 'measure_date',
					width : 200
				}]
		//region:'south'
	});
	new Ext.Viewport({
		//layout : 'border',
		items : [grid,maxAndMin]
	});
});