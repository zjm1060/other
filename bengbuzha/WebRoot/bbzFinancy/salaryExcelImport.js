Ext.onReady(function() {

	Ext.QuickTips.init();
	var pageSize = 20;
	var record = new Ext.data.Record.create([{
				name : 'filename',
				mapping : '文件名'
			}, {
				name : 'name',
				mapping : 'name'
			}, {
				name : '上传时间',
				mapping : '上传时间'
			}, {
				name : 'attID',
				mapping : '附件ID'
			}]);

	var store = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : 
				__cxtPath+'/bbzFinancyMain.do?method=getAttachmentList'
			}),
		baseParams : {
			start : 0,
			limit : pageSize,
			attachmentTypeID : com.bitc.xmis.Attachment.bbzSalaryExcel
		},
		autoLoad : true,
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, record)

	});

	var column = new Ext.grid.ColumnModel([{
				header : '文件名',
				dataIndex : 'filename',
				sortable : true,
				width : 300
			}, {
				header : '上传人',
				dataIndex : 'name',
				sortable : 'true'
			}, {
				header : '上传时间',
				dataIndex : '上传时间',
				sortable : 'true',
				width : 200
			}]);

	var excelimportWin = new com.bitc.xmis.ExcelImportWin({
		width : 500,
		height : 120,
		modal : true,
		title : 'excel文件上传',
		closeAction : 'hide',
		REFID : '-99',// REFID 给一个固定值
		attachmentTypeID : com.bitc.xmis.Attachment.bbzSalaryExcel,
		url : String
				.format(
						'{0}/bbzFinancyMain.do?method=excelImportForSalary&attachmentTypeID={1}&REFID={2}&type={3}',
						__cxtPath, com.bitc.xmis.Attachment.bbzSalaryExcel,
						'-99', '2'),
		listeners : {
			'uploadsuccess' : function() {
				grid.getStore().reload();
			}
		}
	});

	var grid = new Ext.grid.GridPanel({
				region : 'center',
				autoScroll : true,
				loadMask : true,
				loadMask : {
					msg : '正在加载数据，请稍候...'
				},
				cm : column,
				store : store,
				tbar : [{
							xtype : 'button',
							text : '上传',
							iconCls : 'attatchment',
							disabled : false,
							handler : function() {
								excelimportWin.form.getForm().reset();
								excelimportWin.show();

							}
						}],
				bbar : new Ext.PagingToolbar({
							pageSize : pageSize,
							store : store,
							displayInfo : true,
							displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
							emptyMsg : "无显示数据",
							plugins : [new Ext.ux.PagelimitCombo()]
						}),
				listeners : {
					'rowclick' : function(grid, index) {

						var r = grid.store.getAt(index);
						var attID = r.get('attID');
						importSheet(attID);
					}
				}
			});

	this.importSheet = function(attID) {

		var sm = new Ext.grid.CheckboxSelectionModel({
					header : '',
					checkOnly : true
				});

		var infoPanel = new Ext.form.FormPanel({
			region : 'north',
			padding : 20,
			height : 110,
			border : false,
			lableWidth : 90,
			lableAlign : 'left',
			items : [{
						xtype : 'combo',
						fieldLabel : '年份',
						id : 'yearCom',
						hiddenName : 'year',
						displayField : 'iyear',
						valueField : 'iyear',
						mode : 'local',
						triggerAction : 'all',
						editable : false,
						value : new Date().getFullYear(),
						store : new Ext.data.ArrayStore({
									fields : [{
												name : 'iyear'
											}],
									data : []
								}),
						listeners : {
							'beforerender' : function() {
								var newyear = new Date().getFullYear();
								var yearlist = [];
								for (var i = newyear; i >= 2005; i--) {
									yearlist.push([i]);
								}
								this.store.loadData(yearlist);
							}
						}
					}, {
						xtype : 'combo',
						fieldLabel : '月份',
						id : 'monthCom',
						hiddenName : 'month',
						displayField : 'imonth',
						valueField : 'imonth',
						mode : 'local',
						triggerAction : 'all',
						editable : false,
						value : 1,
						store : new Ext.data.ArrayStore({
									fields : [{
												name : 'imonth'
											}],
									data : []
								}),
						listeners : {
							'beforerender' : function() {
								var monthlist = [];
								for (var i = 1; i <= 12; i++) {
									monthlist.push([i]);
								}
								this.store.loadData(monthlist);
							}
						}
					}, {
						xtype : 'combo',
						fieldLabel : '工资类别',
						id : 'salarytypeComb',
						hiddenName : 'salarytype',
						mode : 'remote',
						triggerAction : 'all',
						editable : false,
						allowBlank : false,
						displayField : 'type_name',
						valueField : 'type_id',
						store : new Ext.data.JsonStore({
							url : __cxtPath
									+ '/bbzFinancyMain.do?method=getSalaryType',
							autoLoad : true,
							fields : [{
										name : 'type_id'
									}, {
										name : 'type_name'
									}]
						})
					}]
		});

		var sheetRecord = new Ext.data.Record.create([{
					name : 'sheet_name',
					mapping : 'sheet_name'
				}, {
					name : 'isImport',
					mapping : 'isImport'
				}, {
					name : 'sheet_uuid',
					mapping : 'sheet_uuid'
				}, {
					name : 'sheet_index',
					mapping : 'sheet_index'
				}]);

		var sheetStore = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/bbzFinancyMain.do?method=getSalarySheetList'
					}),
			baseParams : {
				start : 0,
				limit : pageSize,
				attID : attID
			},
			autoLoad : true,
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, sheetRecord)
		});

		var sheetColumn = new Ext.grid.ColumnModel([sm, {
					header : '工作表名称',
					dataIndex : 'sheet_name',
					sortable : true,
					width : 300
				}, {
					header : '状态',
					dataIndex : 'isImport',
					sortable : 'true',
					renderer : function(value) {
						if (value == 0) {
							return "未导入";
						} else {
							return "已导入";
						}
					}
				}, {
					header : 'sheet_uuid',
					dataIndex : 'sheet_uuid',
					hidden : true
				}, {
					header : 'sheet_index',
					dataIndex : 'sheet_index',
					hidden : true
				}]);

		var sheetPanel = new Ext.grid.GridPanel({
					region : 'center',
					autoScroll : true,
					loadMask : true,
					loadMask : {
						msg : '正在加载数据，请稍候...'
					},
					stripeRows : true,
					cm : sheetColumn,
					sm : sm,
					store : sheetStore
				});

		var sheetWin = new Ext.Window({
			title : '请勾选要导入的工作表(SHEET)：',
			width : 650,
			height : 420,
			items : [infoPanel, sheetPanel],
			modal : true,
			layout : 'border',
			fbar : {
				xtype : 'toolbar',
				items : [{
					xtype : 'button',
					text : '导入',
					iconCls : 'ok',
					handler : function() {

						var array = sheetPanel.getSelectionModel()
								.getSelections();
						if (array.length < 1) {
							Ext.Msg.alert('提示', '选择要导入的sheet');
						} else {

							var error = "";
							for (var i = 0; i < array.length; i++) {

								var state = array[i].data.isImport;
								if (state == 1) {
									if (error.length == 0) {

										error = array[i].data.sheet_name;
									} else {
										error = error
												+ array[i].data.sheet_name;
									}
								}
							}

							if (error.length > 0) {
								Ext.Msg.alert('提示', error + '已经导入，无需再次导入！');
							} else {

								var year = infoPanel.getForm()
										.findField("year").getValue();
								var month = infoPanel.getForm()
										.findField("month").getValue();
								var salarytype = infoPanel.getForm()
										.findField("salarytype").getValue();

								if (Ext.isEmpty(year) || Ext.isEmpty(month)
										|| Ext.isEmpty(salarytype)) {
									Ext.Msg.alert("提示", "请先选择年份，月份，工资类别，再导入！");
								} else {
									var indexStr = "";
									for (var i = 0; i < array.length; i++) {
										indexStr += array[i].data.sheet_index
												+ "^"
												// + array[i].data.sheet_name +
												// "^"
												+ array[i].data.sheet_uuid
												+ ",";
									}
									if (indexStr.length > 1) {
										indexStr = indexStr.substring(0,
												indexStr.length - 1);
									}

									Ext.MessageBox.show({
												title : '请稍候',
												msg : '正在导入...',
												width : 300,
												wait : true,
												waitConfig : {
													interval : 200
												},
												closable : false
											});

									Ext.Ajax.request({
										url : __cxtPath
												+ '/bbzFinancyMain.do?method=importSalarySheetList',
										method : 'post',
										params : {
											attID : attID,
											sheetIndex : indexStr,
											iyear : year,
											imonth : month,
											salarytype : salarytype
										},
										success : function(response, action) {
											var result = Ext
													.decode(response.responseText);
											Ext.Msg.hide();
											Ext.Msg.alert(result.messageTitle,
													result.message);
											sheetStore.reload();
										},
										failure : function(response, action) {

										}
									});
								}
							}
						}
					}
				}]
			}
		}).show();
	};

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [grid]
			});
});