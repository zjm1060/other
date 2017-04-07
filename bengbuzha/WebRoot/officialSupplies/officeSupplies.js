Ext.onReady(function() {
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	var DTSJ = new Date().format('Y-m-d');
	var SJ = "";

	var officeSuppliesPanel = new Ext.FormPanel({
		id : 'officeSuppliesPanel',
		autoScroll : true,
		bodyStyle : 'padding:30px 5px 0px 5px',
		region : 'center',
		defaultType : 'textfield',
		labelAlign : 'right',
		labelWidth : 100,
		items : [{
			xtype : 'panel',
			border : false,
			// columnWidth : .5,
			items : [{
				layout : 'column', // 从左往右布局
				xtype : 'panel',
				autoScroll : true,
				border : false,
				width : 400,
				items : [{// 第一列
					// columnWidth : .34,
					layout : 'form',
					xtype : 'panel',
					border : false,
					items : [ {
								xtype : 'textfield',
								fieldLabel : '办公用品ID',
								name : 'supplies_id',
								hidden : true
							}, {
								xtype : 'panel',
								layout : 'form',
								border : false,
								items : [{
									xtype : 'panel',
									border : false,
									layout : 'column',
									fieldLabel : '办公用品类别',
									labelAlign : 'right',
									items : [new com.bitc.xmis.Combos.OfficeSuppliesCombo(
											{
												margins : '0 0 0 5',
												width : 250,
												hiddenName : 'supplies_type_id'
											})]
								}]
							}, {
								xtype : 'textfield',
								fieldLabel : '办公用品名称',
								width : 250,
								allowBlank : false,
								name : 'supplies_name'
							}, {
								xtype : 'textfield',
								fieldLabel : '办公用品品牌',
								width : 250,
								name : 'supplies_brand'
							},{
								xtype : 'textfield',
								fieldLabel : '办公用品型号',
								width : 250,
								name : 'supplies_model'
							}, {
								xtype : 'datefield',
								fieldLabel : '购买日期',
								width : 250,
								format:'Y-m-d',
								name : 'purchase_date'
							},{
								xtype : 'hidden',
								fieldLabel : '数量',
								width : 250,
								name : 'qty'
							}, {
								xtype : 'textarea',
								fieldLabel : '备注',
								width : 250,
								name : 'remark'
							}]
				 }]
			}]
		}],
		tbar : {
			xtype : 'toolbar',
			items : [{
				xtype : 'button',
				text : '保存',
				iconCls : 'ok',
				handler : function() {
					officeSuppliesPanel.getForm().submit({
						url : __cxtPath
								+ '/crmMaint.do?method=doOfficeSupplies',
						params : {
						},
						waitMsg : '正在保存，请稍候……',
						waitTitle : '正在处理',
						success : function(form, action) {
							Ext.MessageBox.hide();
							Ext.MessageBox.hide();
									Ext.MessageBox.alert(
											action.result.messageTitle,
											action.result.message);
											officeSuppliesPanel.getForm().clear();
											Ext.getCmp('supplierGird').getStore().reload();
					officeSuppliesPanel.getForm().findField('supplies_id').setValue('');
						},
						failure : function(form, action) {
							com.bitc.xmis.util.AlertUtil
									.formSubmitFailedAlert(action);
						}
					});
				}
			},{
				xtype : 'button',
				text : '删除',
				iconCls : 'delete',
				ref : '../refreshButton',
				handler : function() {
					Ext.MessageBox.confirm('提示',
								'是否删除', function(btn,
										text) {
									if (btn == 'yes') {
					officeSuppliesPanel.getForm().submit({//保存修改会议室信息方法
						url : __cxtPath
								+ '/crmMaint.do?method=dodelOfficeSuppliesById',
						params : {
							
						},
						waitMsg : '正在删除，请稍候……',
						waitTitle : '正在处理',
						success : function(form, action) {
							Ext.MessageBox.hide();
							Ext.MessageBox.hide();
									Ext.MessageBox.alert(
											action.result.messageTitle,
											action.result.message);
										officeSuppliesPanel.getForm().clear();
											Ext.getCmp('supplierGird').getStore().reload();
					officeSuppliesPanel.getForm().findField('supplies_id').setValue('');
										
						},
						failure : function(form, action) {
							com.bitc.xmis.util.AlertUtil
									.formSubmitFailedAlert(action);
						}
					
					});
									}
										});
				}
			}, '-', {
				xtype : 'button',
				text : '刷新',
				iconCls : 'reload',
				ref : '../refreshButton',
				handler : function() {
					location.reload();
				}
			},'-',{
				xtype : 'button',
				text : '新建',
				iconCls : 'add',
				ref : '../AddBit',
				handler : function() {
					officeSuppliesPanel.getForm().clear();
					officeSuppliesPanel.getForm().findField('supplies_id').setValue('');
				}
			}]
		}
	});

		var maximumGYS = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						width : 25
					}),{
						header : '办公用品ID',
						dataIndex : 'supplies_id',
						hidden : true,
						width : 230
					},{
					header : '办公用品类别ID',
						dataIndex : 'supplies_type_id',
						hidden : true,
						width : 230
					}, {
						header : '办公用品类别',
						dataIndex : 'supplies_type_name',
						sortable : false,
						width : 150
					},  {
						header : '办公用品名称',
						dataIndex : 'supplies_name',
						sortable : false,
						width : 150,
						listeners:{
					'click' : function(t, grid, rowIndex, e) {
						// qualification.attButton1.enable();

						var r = grid.getStore().getAt(rowIndex);
//						var contactPersonID = r.data.room_id;
						var meetingRoomPanelAssignment = new Ext.data.JsonStore(
								{
									url : __cxtPath
											+ '/crmMaint.do?method=getOfficeSuppliesid',
									root : 'rows',
									fields : [{
												name : 'supplies_id'
											}, {
												name : 'supplies_type_id'
											}, {
												name : 'supplies_type_name'
											}, {
												name : 'supplies_name'
											}, {
												name : 'supplies_brand'
											}, {
												name : 'supplies_model'
											}, {
												name : 'purchase_date'
											},{
												name: 'qty'
											},{
												name : 'remark'
											}],
									baseParams : {
										supplies_id : r.data.supplies_id
									},
									autoLoad : true
								});
						meetingRoomPanelAssignment.on('load', function(
								store, record, opts) {
									officeSuppliesPanel.getForm().clear();

							officeSuppliesPanel.getForm().findField('supplies_id')
									.setValue(record[0].data.supplies_id);
							officeSuppliesPanel.getForm().findField('supplies_type_id')
									.setValue(record[0].data.supplies_type_id);
//							officeSuppliesPanel.getForm().findField('supplies_type_name')
//									.setValue(record[0].data.supplies_type_name);
							officeSuppliesPanel.getForm().findField('supplies_name')
									.setValue(record[0].data.supplies_name);
							officeSuppliesPanel.getForm().findField('supplies_brand')
									.setValue(record[0].data.supplies_brand);
							officeSuppliesPanel.getForm().findField('supplies_model')
									.setValue(record[0].data.supplies_model);
									if(!Ext.isEmpty(record[0].data.purchase_date)){
							officeSuppliesPanel.getForm().findField('purchase_date')
									.setValue((record[0].data.purchase_date).substring(0,10));
									}else{
									}
							officeSuppliesPanel.getForm().findField('qty')
									.setValue(record[0].data.qty);
							officeSuppliesPanel.getForm().findField('remark')
									.setValue(record[0].data.remark);		
								});
							
					}
				
						}
					},{
					   header : '办公用品品牌',
					   dataIndex : 'supplies_brand',
					   sortable : false
					   
					},{
						header : '办公用品型号',
						dataIndex : 'supplies_model',
						sortable : false,
						width : 100
					},{
						header : '购买日期',
						dataIndex : 'purchase_date',
						sortable : false,
						width : 100,
						renderer : function(value, cellmeta, record, rowIndex, columnIndex,store) {
										return value.substring(0,10);
									}
					},{
						header : '数量',
						hidden:true,
						dataIndex : 'qty',
						sortable : false,
						width : 100
					},{
						header : '备注',
						dataIndex : 'remark',
						sortable : false,
						width : 200
					}]);
	var reRecordGYS = Ext.data.Record.create([{
				name : 'supplies_id'
			}, {
				name : 'supplies_type_id'
			}, {
				name : 'supplies_type_name'
			}, {
				name : 'supplies_name'
			}, {
				name : 'supplies_brand'
			}, {
				name : 'supplies_model'
			}, {
				name : 'purchase_date'
			
			},{
				name: 'numberfield'
			},{
				name : 'remark'
			}]);
	var storeGYS = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/crmMaint.do?method=getOfficeSupplies',//查询会议室
							method : "POST"
						}),
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, reRecordGYS),
				remoteSort : true,
				baseParams : {
					start : 0,
					limit : 20
				},
				autoLoad : true
			});
	var supplierGird = new Ext.grid.GridPanel({
				region : 'east',
				id:'supplierGird',
//				title : '供应商',
				border : false,
				width : 680,
				loadMask : true,
				split : true,
				collapsible : true,
				autoScroll : true,
				collapseMode : 'mini',
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				stripeRows : true,
				store : storeGYS,
				cm : maximumGYS,
				tbar : {
					xtype : 'toolbar',
					items : [
					'查询：',new Ext.ux.form.SearchField({
								width : 140,
								emptyText : '请输入条件',
								onRender : function(ct, position) {
									Ext.ux.InklingSearchField.superclass.onRender
											.call(this, ct, position);
									if (this.tooltip && this.tooltip.text)
										new Ext.ToolTip({
													target : this.id,
													trackMouse : false,
													draggable : false,
													maxWidth : 200,
													minWidth : 100,
													title : this.tooltip.title,
													html : this.tooltip.text
												});
								},
								tooltip : {
									title : '办公用品查询',
									text : '输入办公用品类别、办公用品名称、购买日期查询'
								},
								onTrigger1Click : function() {
								},
								onTrigger2Click : function() {
									var inputName = this.el.dom.value;
									if (inputName != null
											&& '' != inputName.trim()) {
										storeGYS.setBaseParam('queryStr',
												inputName);
										storeGYS.reload({
													params : {
														start : 0
													}
												});
									} else {
										Ext.MessageBox.alert("提示",
												"请输入要查询的内容后进行查询");
									}
								}
							})
					]
				},
				bbar : new Ext.PagingToolbar({
							pageSize : 20,
							store : storeGYS,
							displayInfo : true,
							displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
							emptyMsg : "无显示数据",
							plugins : [new Ext.ux.PagelimitCombo()]
						})
			});
	
	new Ext.Viewport({
				layout : 'border',
				items : [officeSuppliesPanel,supplierGird]
			});
});
