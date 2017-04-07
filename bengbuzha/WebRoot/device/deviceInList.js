Ext.onReady(function() {
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	var reRecord = Ext.data.Record.create([{
				"name" : "id",
				"type" : "string"
			}, {
				"name" : "收入合同编号",
				"type" : "string"
			}, {
				"name" : "execu_id",
				"type" : "int"
			}, {
				"name" : "支出合同编号",
				"type" : "string"
			}, {
				"name" : "物资ID",
				"type" : "string"
			}, {
				"name" : "项目ID",
				"type" : "string"
			}, {
				"name" : "采购清单ID",
				"type" : "string"
			}, {
				"name" : "收入合同ID",
				"type" : "string"
			}, {
				"name" : "支出合同ID",
				"type" : "int"
			}, {
				"name" : "物资名称",
				"type" : "string"
			}, {
				name : '到货数量'
			}, {
				name : '到货日期'
			}, {
				name : '登记日期'
			}, {
				name : 'registerName'
			}, {
				name : '出厂日期'
			}, {
				name : '保修期限'
			}, {
				name : '原厂保修天数'
			}, {
				name : '原厂可退天数'
			}, {
				name : '原厂可换天数'
			}, {
				name : '保修期限'
			}, {
				name : '可退期限'
			}, {
				name : '可换期限'
			}]);
	// store org_公司
	var store = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/device.do?method=getDeviceInListByContractId',
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
					field : '登记日期',
					direction : 'DESC'
				},
				autoLoad : true,
				remoteSort : true
			});
	// var bsm = new Ext.grid.CheckboxSelectionModel({
	// checkOnly : true,
	// singleSelect : true,
	// header : ''
	// });
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(), {
				dataIndex : '收入合同编号',
				sortable : true,
				width : 200,
				header : '收入合同编号'
			}, {
				dataIndex : '支出合同编号',
				sortable : true,
				width : 200,
				header : '支出合同编号'
			}, {
				dataIndex : '物资名称',
				sortable : true,
				header : '物资名称',
				listeners : {
					'click' : function(column, grid, rowIndex, e) {
						var record = grid.getStore().getAt(rowIndex);
						var id = record.data.id;
						var contractID = record.get('支出合同ID');
						var flag = "edit";
						if (record.data.支出合同ID != 0 && record.data.支出合同编号 != '') {
							flag = 'edit';
						} else {
							flag = 'editOutcontract';
						}
						com.bitc.xmis.device.windowShow(flag, id, contractID,
								0, '', '', '', '', 'id', 'code');
					}
				}
			}, {
				dataIndex : '到货数量',
				sortable : true,
				width : 150,
				header : '到货数量'
			}, {
				dataIndex : '到货日期',
				width : 150,
				sortable : true,
				header : '到货日期'
			}, {
				dataIndex : '登记日期',
				width : 150,
				sortable : true,
				header : '登记日期'
			}, {
				dataIndex : 'registerName',
				width : 150,
				sortable : true,
				header : '登记人'
			}]);
	// gridpanel org_公司
	var gridPanel = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		loadMask : true,
		title : '到货登记',
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : store,
		cm : cm,
		// sm : bsm,
		tbar : ['模糊查询：', new Ext.ux.InklingSearchField({
							width : 120,
							store : store,
							tooltip : {
								title : '模糊查询',
								text : '请输入“收入合同编号、或支出合同编号、到货日期、登记日期、物资名称”的关键词进行查询'
							},
							inklings : [{
										name : '支出合同编号',
										op : 'like',
										type : 'string'
									}, {
										name : '收入合同编号',
										op : 'like',
										type : 'string'
									}, {
										name : '到货日期',
										op : 'like',
										type : 'string'
									}, {
										name : '登记日期',
										op : 'like',
										type : 'string'
									}, {
										name : '物资名称',
										op : 'like',
										type : 'string'
									}]
						}), '-', {
					ref : '../addRelated',
					text : '到货',
					iconCls : 'add',
					handler : function() {
						com.bitc.xmis.device.windowShow('newcreate', '', 0, 0,
								'', '', '');
						// var records = gridPanel.getSelectionModel()
						// .getSelections();
						// var rec = records[0];
						// if (records.length == 0) {
						// Ext.MessageBox.alert("提示", "请选择数据");
						// } else if (rec.get('支出合同ID') != null
						// && rec.get('支出合同ID') != 0) {
						//
						// var contractID = rec.get('支出合同ID');
						// var contractCode = rec.get('支出合同编号');
						// var deviceInfoID = rec.get('物资ID');
						// var deviceInfoName = rec.get('物资名称');
						// var purchaseListID = rec.get('采购清单ID');
						// var projectIDParam = rec.get('收入合同ID');
						// var projectCodeParam = rec.get('收入合同编号');
						//
						// com.bitc.xmis.device.windowShow('register_all', '',
						// 0, contractID, contractCode, deviceInfoID,
						// deviceInfoName, purchaseListID,
						// projectIDParam, projectCodeParam);
						// } else {
						//
						// var deviceInfoID = rec.get('物资ID');
						// var deviceInfoName = rec.get('物资名称');
						// var purchaseListID = rec.get('采购清单ID');
						// var projectIDParam = rec.get('收入合同ID');
						// var projectCodeParam = rec.get('收入合同编号');
						//
						// com.bitc.xmis.device.windowShow('register_null',
						// '', 0, 0, contractCode, deviceInfoID,
						// deviceInfoName, purchaseListID,
						// projectIDParam, projectCodeParam);
						// }

					}
				}, '->', '<font color=blue>单击“物资名称”可对此条到货记录进行编辑、删除</font>'],
		bbar : new Ext.PagingToolbar({
			pageSize : 20,
			store : store,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : "无显示数据",
			plugins : [new Ext.ux.Export({
						exportname : '到货信息',
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
	top.xMis.addRefresh('deviceInRegisterLOAD', function() {
				store.reload();
			});
	// main
	new Ext.Viewport({
				layout : 'border',
				items : [gridPanel]
			});

});
