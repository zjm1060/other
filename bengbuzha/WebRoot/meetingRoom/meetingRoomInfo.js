Ext.onReady(function() {
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	var DTSJ = new Date().format('Y-m-d');
	var SJ = "";

	var meetingRoomPanel = new Ext.FormPanel({
		id : 'meetingRoomPanel',
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
								fieldLabel : '会议室ID',
								name : 'room_id',
								hidden : true
							}, {
								xtype : 'textfield',
								fieldLabel : '会议室编号',
								name : 'room_No',
								allowBlank : false,
								width : 250,
								blankText : '该输入项为必输项'
							}, {
								xtype : 'textfield',
								fieldLabel : '会议室名称',
								width : 250,
								allowBlank : false,
								name : 'room_name'
							}, {
								xtype : 'textfield',
								fieldLabel : '楼层',
								width : 250,
								name : 'floor'
							}, {
								xtype : 'textfield',
								fieldLabel : '地址',
								width : 250,
								name : 'address'
							}, {
								xtype : 'numberfield',
								fieldLabel : '最多可容纳人数',
								width : 250,
								name : 'number_people'
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
					meetingRoomPanel.getForm().submit({//
						url : __cxtPath
								+ '/crmMaint.do?method=doMeetingRoomInfo',
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
											meetingRoomPanel.getForm().clear();
					meetingRoomPanel.getForm().findField('room_id').setValue('');
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
					meetingRoomPanel.getForm().submit({//保存修改会议室信息方法
						url : __cxtPath
								+ '/crmMaint.do?method=dodelMeetingRoomInfoById',
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
											meetingRoomPanel.getForm().clear();
					meetingRoomPanel.getForm().findField('room_id').setValue('');
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
					meetingRoomPanel.getForm().clear();
					meetingRoomPanel.getForm().findField('room_id').setValue('');
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
						header : '会议室ID',
						dataIndex : 'room_id',
						hidden : true,
						width : 230
					}, {
						header : '会议室编号',
						dataIndex : 'room_No',
						sortable : false,
						width : 200,
						listeners:{
					'click' : function(t, grid, rowIndex, e) {
						// qualification.attButton1.enable();

						var r = grid.getStore().getAt(rowIndex);
//						var contactPersonID = r.data.room_id;
						var meetingRoomPanelAssignment = new Ext.data.JsonStore(
								{
									url : __cxtPath
											+ '/crmMaint.do?method=getMeetingRoomInfo',
									root : 'rows',
									fields : [{
												name : 'room_id'
											}, {
												name : 'room_No'
											}, {
												name : 'room_name'
											}, {
												name : 'number_people'
											}, {
												name : 'floor'
											}, {
												name : 'address'
											}, {
												name : 'remark'
											}],
									baseParams : {
										room_id : r.data.room_id
									},
									autoLoad : true
								});
						meetingRoomPanelAssignment.on('load', function(
								store, record, opts) {
									meetingRoomPanel.getForm().clear();

							meetingRoomPanel.getForm().findField('room_id')
									.setValue(record[0].data.room_id);
							meetingRoomPanel.getForm().findField('room_No')
									.setValue(record[0].data.room_No);
							meetingRoomPanel.getForm().findField('room_name')
									.setValue(record[0].data.room_name);
							meetingRoomPanel.getForm().findField('number_people')
									.setValue(record[0].data.number_people);
							meetingRoomPanel.getForm().findField('floor')
									.setValue(record[0].data.floor);
							meetingRoomPanel.getForm().findField('address')
									.setValue(record[0].data.address);
							meetingRoomPanel.getForm().findField('remark')
									.setValue(record[0].data.remark);
								});
							
					}
				
						}
					},  {
						header : '会议室名称',
						dataIndex : 'room_name',
						sortable : false,
						width : 200
					},{
					   header : '最多容纳人数',
					   dataIndex : 'number_people',
					   sortable : false
					   
					},{
						header : '楼层',
						dataIndex : 'floor',
						sortable : false,
						width : 100
					},{
						header : '地址',
						dataIndex : 'address',
						sortable : false,
						width : 200
					},{
						header : '备注',
						dataIndex : 'remark',
						sortable : false,
						width : 200
					}]);
	var reRecordGYS = Ext.data.Record.create([{
				name : 'room_id'
			}, {
				name : 'room_No'
			}, {
				name : 'room_name'
			}, {
				name : 'floor'
			}, {
				name : 'address'
			}, {
				name : 'remark'
			}, {
				name : 'number_people'
			}]);
	var storeGYS = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/crmMaint.do?method=getMeetingRoomInfor',//查询会议室
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
//	var eastPanelComp = new Ext.TabPanel({
//		id : 'eastPanelComp',
//		region : 'east',
//		width : 680,
//		border : false,
//		split : true,
//		autoScroll : true,
//		collapsible : true, // 是否有右上方展开收缩条
//		collapseMode : 'mini', // 将滚动收缩条模式‘迷你’
//		stateful : true,
//		stateId : 'editContract-eastpanel',
//		stateEvent : ['resize'],
//		activeTab : 0,
//		items : [supplierGird]
//		});

	new Ext.Viewport({
				layout : 'border',
				items : [meetingRoomPanel, supplierGird]
			});


	

	
	
});
