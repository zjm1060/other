Ext.onReady(function() {
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	var DTSJ = new Date().format('Y-m-d');
	var SJ = "";

	var officialVehiclesPanel = new Ext.FormPanel({
		id : 'officialVehiclesPanel',
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
								fieldLabel : '车辆ID',
								name : 'car_id',
								hidden : true
							}, {
								xtype : 'textfield',
								fieldLabel : '车牌号',
								name : 'car_No',
								allowBlank : false,
								//hidden : true,
								width : 250,
								blankText : '该输入项为必输项'
							}, {
								xtype : 'textfield',
								fieldLabel : '品牌名称',
								width : 250,
								allowBlank : false,
								name : 'car_brand'
							}, {
								xtype : 'textfield',
								fieldLabel : '车型',
								width : 250,
								name : 'car_type'
							}, {
								xtype : 'datefield',
								fieldLabel : '购买日期',
								width : 250,
								allowBlank : false,
								format : 'Y-m-d',
								name : 'purchase_date'
							}, {
								xtype : 'numberfield',
								fieldLabel : '准乘人数',
								width : 250,
								name : 'capacity'
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
					officialVehiclesPanel.getForm().submit({//保存修改物资库方法
						url : __cxtPath
								+ '/crmMaint.do?method=doVehicleManagement',
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
											Ext.getCmp('supplierGird').getStore().reload();
											officialVehiclesPanel.getForm().clear();
					officialVehiclesPanel.getForm().findField('car_id').setValue('');
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
					officialVehiclesPanel.getForm().submit({//保存修改会议室信息方法
						url : __cxtPath
								+ '/crmMaint.do?method=dodelVehicleManagemenById',
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
											officialVehiclesPanel.getForm().clear();
											officialVehiclesPanel.getForm().findField('car_id').setValue('');
											Ext.getCmp('supplierGird').getStore().reload();
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
					officialVehiclesPanel.getForm().clear();
					officialVehiclesPanel.getForm().findField('car_id').setValue('');
				}
			}]
		}
	});

	/**
	 * 最大供应商
	 */
	var maximumGYS = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						width : 25
					}),{
						header : '车辆id',
						dataIndex : 'car_id',
						hidden : true,
						width : 230
					}, {
						header : '车牌号',
						dataIndex : 'car_No',
						sortable : false,
						//hidden : true,
						width : 150
					},  {
						header : '品牌名称',
						dataIndex : 'car_brand',
						sortable : false,
						width : 150,
						listeners:{
							'click' : function(t, grid, rowIndex, e) {
								// qualification.attButton1.enable();

								var r = grid.getStore().getAt(rowIndex);
//								var contactPersonID = r.data.room_id;
								var meetingRoomPanelAssignment = new Ext.data.JsonStore(
										{
											url : __cxtPath
													+ '/crmMaint.do?method=getVehicleManagemenid',
											root : 'rows',
											fields : [{
														name : 'car_id'
													}, {
														name : 'car_No'
													}, {
														name : 'car_brand'
													}, {
														name : 'car_type'
													}, {
														name : 'purchase_date'
													}, {
														name : 'remark'
													}, {
														name : 'capacity'
													}],
											baseParams : {
												car_id : r.data.car_id
											},
											autoLoad : true
										});
								meetingRoomPanelAssignment.on('load', function(
										store, record, opts) {
											officialVehiclesPanel.getForm().clear();

									officialVehiclesPanel.getForm().findField('car_id')
											.setValue(record[0].data.car_id);
									officialVehiclesPanel.getForm().findField('car_No')
											.setValue(record[0].data.car_No);
									officialVehiclesPanel.getForm().findField('car_brand')
											.setValue(record[0].data.car_brand);
									officialVehiclesPanel.getForm().findField('car_type')
											.setValue(record[0].data.car_type);
											if(!Ext.isEmpty(record[0].data.purchase_date)){
									officialVehiclesPanel.getForm().findField('purchase_date')
											.setValue((record[0].data.purchase_date).substring(0,10));
											}else{
											}
									officialVehiclesPanel.getForm().findField('capacity')
											.setValue(record[0].data.capacity);
									officialVehiclesPanel.getForm().findField('remark')
											.setValue(record[0].data.remark);
										});
									
							}
						
								}
					},{
					   header : '车型',
					   dataIndex : 'car_type',
					   sortable : false
					   
					},{
						header : '购买日期',
						dataIndex : 'purchase_date',
						sortable : false,
						width : 100,
						renderer : function(value, cellmeta, record, rowIndex, columnIndex,store) {
										return value.substring(0,10);
									}
					},{
						header : '准乘人数',
						dataIndex : 'capacity',
						sortable : false,
						width : 100
					},{
						header : '备注',
						dataIndex : 'remark',
						sortable : false,
						width : 200
					}]);
	var reRecordGYS = Ext.data.Record.create([{
				name : 'car_id'
			}, {
				name : 'car_No'
			}, {
				name : 'car_brand'
			}, {
				name : 'car_type'
			}, {
				name : 'purchase_date'
			}, {
				name : 'capacity'
			}, {
				name : 'remark'
			}]);
	var storeGYS = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/crmMaint.do?method=getVehicleManagement',//查询会议室
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
				cm : maximumGYS
			});

	new Ext.Viewport({
				layout : 'border',
				items : [officialVehiclesPanel, supplierGird]
			});
});
