Ext.ns('com.bitc.xmis.Records');
com.bitc.xmis.Records.deviceRecord = Ext.data.Record.create([{
			name : 'id',
			type : 'string'
		}, {
			name : 'projectID',
			type : 'string'
		}, {
			name : 'name',
			type : 'string'
		}, {
			name : 'number',
			type : 'int'
		}, {
			name : 'modelType',
			type : 'string'
		}, {
			name : 'standard'
		}, {
			name : 'weight'
		}, {
			name : 'referencePrice',
			type : 'float'
		}, {
			name : 'currencySymbol',
			mapping : 'currency.symbol'
		}, {
			name : 'entryEmpName',
			mapping : 'lastModifier.userName'
		}, {
			name : 'lastModified',
			type : 'date',
			dateFormat : 'Y-m-d'
		}, {
			name : 'purchaseStatus'
		}, 'statusCount']);
Ext.ns('com.bitc.xmis');
com.bitc.xmis.ProjectDeviceForm = Ext.extend(Ext.FormPanel, {
	initComponent : function() {
		if (this.project) {
			this.items = [{
						xtype : 'fieldset',
						title : '项目信息',
						items : [{
									xtype : 'hidden',
									fieldLabel : 'Label',
									anchor : '100%',
									name : 'projectID',
									value : this.project.projectID
								}, {
									xtype : 'displayfield',
									fieldLabel : '项目编码',
									name : 'projectCode',
									anchor : '100%',
									value : this.project.projectCode
								}, {
									xtype : 'displayfield',
									fieldLabel : '项目名称',
									name : 'projectName',
									anchor : '100%',
									value : this.project.projectName
								}]
					}, {
						xtype : 'fieldset',
						title : '设备详细信息',
						items : [{
									xtype : 'hidden',
									fieldLabel : 'Label',
									anchor : '100%',
									name : 'id'
								}, {
									xtype : 'textfield',
									fieldLabel : '设备名称',
									anchor : '100%',
									name : 'name',
									allowBlank : false
								}, {
									xtype : 'numberfield',
									fieldLabel : '数量',
									anchor : '100%',
									allowDecimals : false,
									name : 'number',
									allowBlank : false,
									minValue : 0
								}, {
									xtype : 'textfield',
									fieldLabel : '型号',
									anchor : '100%',
									name : 'modelType'
								}, {
									xtype : 'textfield',
									fieldLabel : '规格',
									anchor : '100%',
									name : 'standard'
								}, {
									xtype : 'textfield',
									fieldLabel : '重量',
									name : 'weight',
									anchor : '100%'
								}, {
									xtype : 'compositefield',
									fieldLabel : '参考价格',
									anchor : '100%',
									items : [
											new com.bitc.xmis.Combos.CurrencyCombo(
													{
														flex : 2,
														allowBlank : false,
														hiddenName : 'currencyId'														
													}), {
												xtype : 'numberfield',
												flex : 3,
												name : 'referencePrice'
											}]

								}]
					}];
		}
		com.bitc.xmis.ProjectDeviceForm.superclass.initComponent.call(this);
	}
});
com.bitc.xmis.ProjectDeviceWindow = Ext.extend(Ext.Window, {
			modal : true,
			closeAction : 'hide',
			showDeviceGrid : function() {
				this.deviceGrid.show();
			},
			hideDeviceGrid : function() {
				this.deviceGrid.hide();
			},
			isNew : false,
			initComponent : function() {
				if (this.project) {
					var form = new com.bitc.xmis.ProjectDeviceForm({
								project : this.project,
								region : 'west',
								width : 350,
								padding : '10 10 10 10'
							});
					this.form = form;
					var deviceGrid = new com.bitc.xmis.DeviceGridPanel({
								region : 'center',
								title : '设备库',
								listeners : {
									scope : this,
									'rowdblclick' : function(grid, rowIndex,
											event) {
										var record = grid.getStore()
												.getAt(rowIndex);
										var values = record.data;
										values.deviceName = values.name;
										values.number = 1;
										values.id = null;
										//values.currencyId=values.currency.id;
										this.form.getForm().setValues(values);
									}
								}
							});
					this.deviceGrid = deviceGrid;
					this.layout = 'border';
					this.items = [form, deviceGrid];
					this.tbar = [{
						text : '确定',
						iconCls : 'ok',
						handler : function() {
							var currencyValue = form.getForm().findField('currencyId').getValue();
							if(currencyValue==0){
								form.getForm().findField('currencyId').setValue(-1);
							}
							this.form.getForm().submit({
								url : String.format(
										'{0}/projectMain.do?method=saveDevice',
										__cxtPath),
								scope : this,
								success : function(f, action) {
									Ext.Msg.alert(action.result.messageTitle,
											action.result.message);
									this.fireEvent('updatecomplete', this);
								},
								failure : function(f, action) {
									com.bitc.xmis.util.AlertUtil
											.formSubmitFailedAlert(action);
								}
							});
						},
						scope : this
					}];
				}
				com.bitc.xmis.ProjectDeviceWindow.superclass.initComponent
						.call(this);
				this.addEvents('updatecomplete');
			}
		});
com.bitc.xmis.ProjectDevicePanel = Ext.extend(Ext.grid.GridPanel, {
	editable : false,
//	autoExpandColumn : 'devName',
	stripeRows : true,
	viewConfig : {
		markDirty : false
	},
	initComponent : function() {
		if (this.projectId) {
			this.project = {
				projectID : this.projectId
			};
		}
		var deviceReader = new Ext.data.JsonReader({
					idProperty : 'id',
					root : 'rows',
					totalProperty : 'totalCount'
				}, com.bitc.xmis.Records.deviceRecord);
		var deviceStore = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
				url : String
						.format(
								'{0}/projectMain.do?method=getDeviceByProjectID&projectID={1}',
								__cxtPath, this.project.projectID)
			}),
			reader : deviceReader,
			autoLoad : true,
			autoSave : false
		});
		this.sm = new Ext.grid.RowSelectionModel({
					singleSelect : true
				});
		this.store = deviceStore;
		this.columns = [new Ext.grid.RowNumberer(), {
					header : '设备ID',
					dataIndex : 'id',
					hidden : true
				}, {
					id : 'devName',
					header : '设备名称',
					dataIndex : 'name'
				}, {
					header : '数量',
					align : 'right',
					dataIndex : 'number',
					width : 80
				}, {
					header : '型号',
					dataIndex : 'modelType',
					width : 100
				}, {
					dataIndex : 'standard',
					header : '规格',
					width : 90
				}, {
					dataIndex : 'weight',
					header : '重量',
					align : 'right',
					width : 100
				}, {
					dataIndex : 'referencePrice',
					header : '参考价格',
					align : 'right',
					width : 90,
					renderer : function(value, metaData, record) {
						return Ext.util.Format.number(value, record
										.get('currencySymbol')
										+ '0,0.00');
					}
				}, {
					dataIndex : 'statusCount',
					header : '状态',
					width : 200,
					renderer : function(value, metaData, record) {
						if (value == null) {
							return '';
						}
						return String.format('已申请：{0}；采购中：{1}；已采购：{2}',
								value[0], value[1], value[2])
					}
				}, {
					dataIndex : 'entryEmpName',
					header : '最后更新人',
					width : 90
				}, {
					dataIndex : 'lastModified',
					header : '更新时间',
					align : 'right',
					width : 90,
					renderer : Ext.util.Format.dateRenderer('Y-m-d')
				}];
		if (this.editable) {
			deviceStore.on('remove', function(store, record, index) {
						Ext.Ajax.request({
									url : __cxtPath
											+ '/projectMain.do?method=deleteDevice',
									method : 'post',
									params : {
										'id' : record.get('id')
									},
									success : function(response) {
										var action = Ext
												.decode(response.responseText);
										Ext.Msg.alert(action.messageTitle,
												action.message);
										if (action.success) {
										} else {
											store.reload();
										}
									},
									failure : function(response) {
									}
								});
					}, this);
			this.editWindow = new com.bitc.xmis.ProjectDeviceWindow({
						project : this.project,
						width : 700,
						height : 400,
						listeners : {
							scope : this,
							'updatecomplete' : function(win) {
								this.store.reload();
								win.hide();
							}
						}
					});

			this.on('rowdblclick', function(grid, rowIndex, event) {
						var record = grid.getStore().getAt(rowIndex);
						this.editWindow.setTitle('修改设备——' + record.get('name'));
						// this.editWindow.hideDeviceGrid();
						this.editWindow.form.getForm().setValues(record.data);
						this.editWindow.isNew = false;
						this.editWindow.show();
					}, this);
		}
		com.bitc.xmis.ProjectDevicePanel.superclass.initComponent.call(this);
	},
	reload : function(params) {
		this.store.reload(params);
	},
	newDevice : function(b, e) {

		this.editWindow.setTitle('添加设备');
		// this.editWindow.showDeviceGrid();
		this.editWindow.form.getForm().reset();
		this.editWindow.form.getForm().setValues({
			projectID : this.project.projectID,
			projectName : this.project.projectName + '（'
					+ this.project.projectCode + '）'
		});
		this.editWindow.isNew = true;
		this.editWindow.show();
	},
	delSelDevice : function(b, e) {
		Ext.Msg.confirm("请确认", "确定要删除该条设备记录吗？", function(btn) {
					if (btn == 'yes') {
						var r = this.getSelectionModel().getSelected();
						deviceStore.remove(r);
					}
				}, this);
	}
});
