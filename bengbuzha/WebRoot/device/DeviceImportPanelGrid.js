Ext.ns('com.bitc.xmis.Records');
com.bitc.xmis.Records.DeviceImportPanelRecord = Ext.data.Record.create([{
			name : 'id'
		}, {
			name : 'sheet_uuid'
		}, {
			name : 'level_id'
		}, {
			name : '序号'
		}, {
			name : 'groupname'
		}, {
			name : 'groupname2'
		}, {
			name : '附件名字'
		}, {
			name : 'sheet_name'
		}, {
			name : '名称'
		}, {
			name : '规格'
		}, {
			name : '数量'
		}, {
			name : '单位'
		}, {
			name : '合同单价',
			type : 'float'
		}, {
			name : '合同合价',
			type : 'float'
		}, {
			name : '成本单价',
			type : 'float'
		}, {
			name : '产地'
		}, {
			name : '制造商'
		}, {
			name : '附件ID'
		}, {
			name : 'projectId'
		}]);
Ext.ns('com.bitc.xmis');

com.bitc.xmis.DeviceImportPanelGrid = Ext.extend(Ext.grid.GridPanel, {
	initComponent : function() {
		this.store = new Ext.data.GroupingStore({
			url : String
					.format(
							'{0}/device.do?method=getImportDeviceInfoList&projectId={1}',
							__cxtPath, this.projectId),
			autoLoad : true,
			reader : new Ext.data.JsonReader({
						idProperty : 'id',
						root : 'rows',
						totalProperty : 'totalCount'
					}, com.bitc.xmis.Records.DeviceImportPanelRecord),
			groupField : 'groupname2',
			sortInfo : {
				field : 'level_id',
				direction : 'ASC'
			},
			baseParams : {
				limit : 0
			}
		});
		this.view = new Ext.grid.GroupingView({
			forceFit : true,
			startCollapsed : true,
			groupOnSort : true,
			groupTextTpl : '{text} ({[values.rs.length]}{[values.rs.length >= 1 ? "条" : "条"]})'
		});
		var store = this.store;
		this.columns = [new Ext.grid.RowNumberer(), {
					header : '',
					sortable : true,
					hidden : true,
					dataIndex : 'groupname'
				}, {
					header : '',
					sortable : true,
					hidden : true,
					dataIndex : 'groupname2'
				}, {
					header : '序号',
					sortable : true,
					dataIndex : '序号'
				}, {
					header : 'level_id',
					hidden : true,
					sortable : true,
					dataIndex : 'level_id'
				}, {
					header : '设备名称',
					width : 150,
					sortable : true,
					dataIndex : '名称'
				}, {
					header : '规格',
					sortable : true,
					width : 250,
					dataIndex : '规格'
				}, {
					header : '数量',
					sortable : true,
					dataIndex : '数量'
				}, {
					header : '单位',
					sortable : true,
					dataIndex : '单位'
				}, {
					header : '合同单价',
					// hidden:true,
					width : 150,
					dataIndex : '合同单价'
				}, {
					header : '合同合价',
					width : 150,
					// hidden:true,
					dataIndex : '合同合价'
				}, {
					header : '成本单价',
					width : 150,
					// hidden:true,
					dataIndex : '成本单价'
				}, {
					header : '产地',
					width : 150,
					dataIndex : '产地'
				}, {
					header : '制造商',
					dataIndex : '制造商'
				}];
		this.tbar = new Ext.Toolbar({
					enableOverflow : true
				});

		// this.tbar = new Ext.Toolbar({
		// enableOverflow : true,
		// items : [{
		// xtype : 'button',
		// text : '刷新',
		// iconCls : 'reload',
		// ref : '../refreshButton',
		// handler : function() {
		// store.reload();
		// }
		// }]
		//		
		// });
		this.bbar = new Ext.PagingToolbar({
					pageSize : 100000,
					store : store,
					displayInfo : true,
					displayMsg : '显示第 {0} 条到 {1} 条记录，一共 {2} 条',
					emptyMsg : "无显示数据"
				});
		com.bitc.xmis.DeviceImportPanelGrid.superclass.initComponent.call(this);
	}
});
com.bitc.xmis.ImpOrtStatus = ['未导入', '<font color="green">已导入</font>', '失败',
		'不导入'];
com.bitc.xmis.DeviceAttachmentList = Ext.extend(Ext.grid.GridPanel, {
	autoExpandColumn : 'filename',
	hideHeaders : true,
	REFID : '',
	actionColumnHidden : false,
	viewConfig : {
		emptyText : '还未添加附件'
	},
	/**
	 * 设置隐藏的列
	 */
	setActionColumnHidden : function(flag) {
		this.getColumnModel().setHidden(2, flag);
	},
	initComponent : function() {
		/**
		 * @property store Grid Store
		 * @type Ext.data.Store
		 */
		this.store = new Ext.data.JsonStore({
					url : String.format(
							'{0}/fileUpload.do?method=getAttachmentList',
							__cxtPath),
					root : 'rows',
					totalProperty : 'totalCount',
					baseParams : {
						REFID : this.REFID,
						attachmentTypeID : this.attachmentTypeID
					},
					fields : [DBSchema.Attachment.AttachmentID,
							DBSchema.Attachment.AttachmentName]
				});

		if (!Ext.isEmpty(this.REFID)) {
			this.store.load();
		}
		this.action = new Ext.ux.grid.RowActions({
					header : '删除',
					dataIndex : DBSchema.Attachment.AttachmentID,
					width : 25,
					hidden : this.actionColumnHidden,
					actions : [{
								iconCls : 'download',
								tooltip : '下载'
							}]

				});
		this.columns = [new Ext.grid.RowNumberer({
							width : 30
						}), {
					id : 'filename',
					header : '文件名称',
					sortable : true,
					dataIndex : DBSchema.Attachment.AttachmentName
				}, this.action];
		this.plugins = [this.action];
		this.action.on('action', function(grid, record, action, row, col) {
		if (action == 'download') {
				location.href = __cxtPath + '/fileUpload.do?type=3&attID='
						+ record.get(DBSchema.Attachment.AttachmentID);
			}
		}, this);
		com.bitc.xmis.DeviceAttachmentList.superclass.initComponent.call(this);
	},
	/**
	 * 重新加载附件列表
	 * 
	 * @param {Object}
	 *            options 调用Store的reload时使用的参数
	 * @return {String}
	 */
	reload : function(options) {
		this.getStore().reload(options);
		return '';
	},
	onRender : function(a, b) {
		if (this.attachmentTypeID == com.bitc.xmis.Attachment.importDevice) {
			this.on('rowclick', function(grid, index) {
				var r = this.getStore().getAt(index);
				var attID = r.get('附件ID');
				var sm = new Ext.grid.CheckboxSelectionModel({
							checkOnly : true,
							header : ''
						});
				var tracePanel = new Ext.grid.GridPanel({
					region : 'center',
					// border : false,
					autoScorll : true,
					loadMask : true,
					loadMask : {
						msg : '正在加载数据，请稍侯……'
					},
					stripeRows : true,
					cm : new Ext.grid.ColumnModel([sm, {
								dataIndex : 'name',
								sortable : true,
								width : 300,
								header : 'SHEET名字'
							}, {
								dataIndex : 'flag',
								sortable : true,
								width : 300,
								header : '状态',
								renderer : function(value) {
									return com.bitc.xmis.ImpOrtStatus[value];
								}
							}]),
					sm : sm,
					store : new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
							url : String
									.format(
											'{0}/importExcel.do?method=getDevicesSheetList&type=2&&attID={1}',
											__cxtPath, attID),
							method : "POST"
						}),
						reader : new Ext.data.JsonReader({
									root : 'rows'
								}, new Ext.data.Record.create([{
											"name" : "id",
											"type" : "string"
										}, {
											"name" : "name",
											"type" : "string"
										}, {
											"name" : "flag",
											"type" : "int"
										}])),
						autoLoad : true,
						remoteSort : true,
						baseParams : {
							limit : 20
						}
					})
				});
				var formpanel = new Ext.form.FormPanel({
							region : 'north',
							padding : 20,
							width : 450,
							height : 80,
							autoScroll : true,
							border : false,
							labelWidth : 101,
							labelAlign : 'left',
							items : [new com.bitc.xmis.Combos.CurrencyCombo({
												fieldLabel : '货币类型',
												margins : '0 0 0 0',
												hiddenName : 'currencyTypeID',
												height : 22,
												width : 90
											}), {
										xtype : 'radiogroup',
										fieldLabel : '金额单位',
										name : 'unitgroup',
										items : [{
													boxLabel : '元',
													name : 'unit',
													inputValue : 1,
													checked : true
												}, {
													boxLabel : '万元',
													name : 'unit',
													inputValue : 2
												}, {
													boxLabel : '百万元',
													name : 'unit',
													inputValue : 3
												}]
									}]
						});
				var traceWin = new Ext.Window({
					title : '请勾选要导入的工作表(SHEET)：',
					width : 450,
					height : 350,
					items : [tracePanel, formpanel],
					modal : true,
					layout : 'border',
					fbar : {
						xtype : 'toolbar',
						items : [{
							xtype : 'button',
							text : '导入',
							iconCls : 'ok',
							handler : function() {
								var currencyTypeID = formpanel.getForm()
										.findField('currencyTypeID').getValue();
								var unitGroup = formpanel.getForm()
										.findField('unitgroup').items;
								var unitValue = formpanel.getForm()
										.findField('unit').getGroupValue();
								var unit = '';
								for (var i = 0; i < unitGroup.length; i++) {
									if (unitGroup.get(i).inputValue == unitValue) {
										unit = unitGroup.get(i).boxLabel;
										break
									}
								}
								var array = tracePanel.getSelectionModel()
										.getSelections();
								if (array.length < 1) {
									Ext.Msg.alert('提示', '选择要导入的sheet');
								}
								var indexStr = "";
								for (var i = 0; i < array.length; i++) {
									indexStr += array[i].data.id + "^"
											+ array[i].data.name + ",";
								}
								if (indexStr.length > 1) {
									indexStr = indexStr.substring(0,
											indexStr.length - 1);
								}
								Ext.Ajax.request({
									url : String
											.format(
													'{0}/importExcel.do?method=importDevicesList',
													__cxtPath),
									method : 'post',
									params : {
										attID : attID,
										sheetIndex : indexStr,
										currencyTypeID : currencyTypeID,
										unit : unit
									},
									success : function(response, action) {
										var result = Ext
												.decode(response.responseText);
										if (result.success) {
											Ext.Msg.alert(result.messageTitle,
													result.message, function() {
													});
										} else {
											Ext.Msg.alert(result.messageTitle,
													result.message)
										}
										tracePanel.store.reload();
										parent.xMis.refresh('deviceImportPanelGrid');
									},
									failure : function(response, action) {
										var result = Ext
												.decode(response.responseText);
										Ext.Msg.alert(result.messageTitle,
												result.message)
									}
								});
							}
						}]
					}
				});
				traceWin.show();

			}, this);
		}
		com.bitc.xmis.DeviceAttachmentList.superclass.onRender.call(this, a, b);
	}
});
