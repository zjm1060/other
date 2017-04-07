Ext.onReady(function() {
	Ext.QuickTips.init();

	var reRecord = Ext.data.Record.create([{
				"name" : "项目ID",
				"type" : "string"
			}, {
				"name" : "物资ID",
				"type" : "string"
			}, {
				"name" : "项目编码",
				"type" : "string"
			}, {
				"name" : "execu_id",
				"type" : "int"
			}, {
				"name" : "物资名称",
				"type" : "string"
			}, {
				"name" : "状态",
				"type" : "string"
			}, {
				"name" : "存放数量",
				"type" : "int"
			}, {
				"name" : "addressname",
				"type" : "string"

			}]);
	// store org_公司
	var store = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/device.do?method=getDeviceAddressList',
							method : "POST"
						}),
				baseParams : {
					limit : 20
				},
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, reRecord),
				sortInfo : {
					field : '项目编码',
					direction : 'ASC'
				},
				autoLoad : false,
				remoteSort : true
			});
	// var bsm = new Ext.grid.CheckboxSelectionModel({
	// checkOnly :true,
	// singleSelect:true,
	// header:''
	// });
	var cm = new Ext.grid.ColumnModel([// bsm,
	new Ext.grid.RowNumberer(), {
				dataIndex : '项目编码',
				sortable : true,
				width : 200,
				header : '项目编码'
			}, {
				dataIndex : '物资名称',
				sortable : true,
				header : '物资名称'
			}, {
				dataIndex : 'addressname',
				sortable : true,
				id : 'addressname',
				header : '存放地址'
			}, {
				dataIndex : '存放数量',
				sortable : true,
				header : '存放数量'
			}, {
				dataIndex : '状态',
				sortable : true,
				header : '状态'

			}]);
	// gridpanel org_公司
	var gridPanel = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		loadMask : true,
		title : '物资存放信息',
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : store,
		cm : cm,
		autoExpandColumn : 'addressname',
		// sm : bsm,
		tbar : ['模糊查询：', new Ext.ux.InklingSearchField({
							width : 120,
							store : store,
							tooltip : {
								title : '模糊查询',
								text : '请输入“项目编码”“物资名称”“状态”“存放地址”的关键词进行查询'
							},
							inklings : [{
										name : '项目编码',
										op : 'like',
										type : 'string'
									}, {
										name : '物资名称',
										op : 'like',
										type : 'string'
									}, {
										name : '状态',
										op : 'like',
										type : 'string'
									}, {
										name : 'addressname',
										op : 'like',
										type : 'string'
									}]
						})],
		bbar : new Ext.PagingToolbar({
			pageSize : 20,
			store : store,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : "无显示数据",
			plugins : [new Ext.ux.Export({
						exportname : '物资存放信息',
						formater : [{
									columnHeader : '',
									formatClass : 'com.bitc.jk.xmis.export.RowNumber'
								}]
					}), new Ext.ux.PagelimitCombo()],
			items : this.bbar || []
		})

	});

	store.load({
				params : {
					start : 0,
					limit : 20
				}
			});
	// main
	new Ext.Viewport({
				layout : 'border',
				items : [gridPanel]
			});

});
