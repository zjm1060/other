Ext.ns('com.bitc.xmis.Combos');
com.bitc.xmis.Combos.STATUS_ARRAY = [ '', '预约申请', '修改申请', '预约中', '已撤销', '已过期',
		'使用中' ];
com.bitc.xmis.Combos.STATUS = {
	create : 1,
	update : 2,
	agree : 3,
	cancel : 4
};

Ext.ns('com.bitc.xmis');
com.bitc.xmis.ApplyFormPanelRecord = Ext.data.Record.create([ {
	mapping : "form_id",
	name : "form_id",
	type : "string"
}, {
	mapping : "priority",
	name : "priority",
	type : "int"
}, {
	mapping : "reason",
	name : "reason",
	type : "string"
}, {
	"dateFormat" : "Y-m-d H:i:s",
	mapping : "begin_datetime",
	name : "begin_datetime",
	type : "date"
}, {
	"dateFormat" : "Y-m-d H:i:s",
	mapping : "end_datetime",
	name : "end_datetime",
	type : "date"
}, {
	mapping : "app_datetime",
	name : "app_datetime",
	type : "string"
}, {
	mapping : "app_user_id",
	name : "app_user_id",
	type : "int"
}, {
	mapping : "app_user_surname",
	name : "app_user_surname",
	type : "string"
}, {
	mapping : "app_user_name",
	name : "app_user_name",
	type : "string"
}, {
	mapping : "app_surname",
	name : "app_surname",
	type : "string"
}, {
	mapping : "app_name",
	name : "app_name",
	type : "string"
}, {
	mapping : "remark",
	name : "remark",
	type : "string"
}, {
	mapping : "assigned_id",
	name : "assigned_id",
	type : "int"
}, {
	mapping : "assigned_Name",
	name : "assigned_Name",
	type : "string"
}, {
	"name" : "assigned_surname",
	"type" : "string"
}, {
	"name" : "assigned_name",
	"type" : "string"
}, {
	dateFormat : "Y-m-d H:i:s",
	mapping : "assigned_datetime",
	name : "assigned_datetime",
	type : "date"
}, {
	mapping : "status_id",
	name : "status_id",
	type : "int"
} ]);
com.bitc.xmis.ApplyCarWinShow = function(grid, button_num, form_id, status_id) {
	var bsm = new Ext.grid.CheckboxSelectionModel({
		checkOnly : false,
		header : ''
	});
	var vehiclesGrid = new com.bitc.xmis.OffcialVehiclesGrid({
		region : 'north',
		border : false,
		height : 100,
		autoScorll : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		sm : bsm,
		stripeRows : true
	});

	if (button_num != 'create') {
		vehiclesGrid.store
				.on(
						'load',
						function(store, records, options) {
							Ext.Ajax
									.request({
										url : String
												.format(
														'{0}/applyManageMaint.do?method=getApplyCarIdsDetalsByFormId',
														__cxtPath),
										method : 'post',
										params : {
											form_id : form_id
										},
										success : function(response) {
											var result = Ext
													.decode(response.responseText);
											if (result.success) {
												var car_idstr = result.attributes.car_idstr;

												var carIdStrArray = car_idstr
														.split(',');
												var array = new Array();
												for ( var j = 0; j < records.length; j++) {
													for ( var i = 0; i < carIdStrArray.length; i++) {
														if (records[j].data.car_id == carIdStrArray[i]) {
															array.push(j);
														}
													}
												}
												bsm.selectRows(array);
											} else {
												Ext.Msg.alert(
														result.messageTitle,
														result.message);
											}
										},
										failure : function(response) {
											Ext.Msg.alert('提示', '操作失败');
										}
									});
						});

	}
	var vehiclesPanel = new Ext.Panel(
			{
				region : 'north',
				height : 120,
				layout : 'column',
				border : false,
				xtype : 'panel',
				items : [
						{
							columnWidth : .24,
							layout : 'form',
							xtype : 'panel',
							border : false,
							bodyStyle : 'margin-top:10px',
							items : [ new Ext.form.Label(
									{
										width : 101,
										hideMode : "visibility",// 默认display,可以取值：display，offsets，visibility
										html : '<div style=\"margin-left:70px;\"><font>车辆选择:</font></div>'

									}) ]
						}, {
							columnWidth : .74,
							layout : 'form',
							bodyStyle : 'margin-top:10px',
							xtype : 'panel',
							border : true,
							items : vehiclesGrid
						} ]
			});
	var applyCarForm = new Ext.form.FormPanel({
		ref : 'form',
		region : 'center',
		padding : '0px 20px 20px 20px',
		height : 350,
		autoScroll : true,
		border : false,
		labelWidth : 101,
		labelAlign : 'right',
		reader : new Ext.data.JsonReader({
			root : 'rows'
		}, com.bitc.xmis.ApplyFormPanelRecord),
		// tbar : {},
		items : [
				{
					xtype : 'hidden',
					name : 'form_id',
					fieldLabel : 'form_id',
					style : 'color:blue'
				},
				{
					xtype : 'hidden',
					fieldLabel : '开始时间字符串',
					name : 'begin_datetime',
					width : 300
				},
				{
					xtype : 'hidden',
					fieldLabel : '结束时间字符串',
					name : 'end_datetime',
					width : 300
				},
				{
					xtype : 'hidden',
					fieldLabel : '审批人姓氏',
					name : 'assigned_surname',
					width : 300
				},
				{
					xtype : 'hidden',
					fieldLabel : '审批人名字',
					name : 'assigned_name',
					width : 300
				},
				{
					xtype : 'hidden',
					fieldLabel : '申请人姓氏',
					name : 'app_surname',
					width : 300
				},
				{
					xtype : 'hidden',
					fieldLabel : '申请人名字',
					name : 'app_name',
					width : 300
				},
				{
					xtype : 'combo',
					name : 'priority',
					fieldLabel : '优先等级',
					displayField : 'text',
					valueField : 'value',
					mode : 'local',
					triggerAction : 'all',
					value : 0,
					width : 100,
					store : new Ext.data.ArrayStore({
						fields : [ 'value', 'text' ],
						data : [ [ 1, '紧急' ], [ 0, '普通' ] ]
					}),
					listeners : {
						'select' : function(combo, record, index) {
							var selValue = record.data.value;
							if (selValue == 0) {
							} else if (selValue == 1) {
							}
						}
					}
				},
				{
					xtype : 'textfield',
					fieldLabel : '事由',
					name : 'reason',
					allowBlank : false,
					width : 300,
					blankText : '该输入项为必输项'
				},
				{
					xtype : 'panel',
					border : false,
					layout : 'hbox',
					fieldLabel : '开始时间',
					items : [
							{
								xtype : 'datefield',
								name : 'begin_date',
								format : 'Y-m-d',
								value : '',
								editable : false,
								listeners : {
									'change' : validBeginEndTime
								}
							},
							{
								xtype : 'combo',
								displayField : 'text',
								valueField : 'value',
								mode : 'local',
								triggerAction : 'all',
								editable : false,
								name : 'begin_hour',
								value : '',
								width : 50,
								store : new Ext.data.ArrayStore({
									fields : [ 'value', 'text' ],
									data : [ [ 00, '00' ], [ 01, '01' ],
											[ 02, '02' ], [ 03, '03' ],
											[ 04, '04' ], [ 05, '05' ],
											[ 06, '06' ], [ 07, '07' ],
											[ 08, '08' ], [ 09, '09' ],
											[ 10, '10' ], [ 11, '11' ],
											[ 12, '12' ], [ 13, '13' ],
											[ 14, '14' ], [ 15, '15' ],
											[ 16, '16' ], [ 17, '17' ],
											[ 18, '18' ], [ 19, '19' ],
											[ 20, '20' ], [ 21, '21' ],
											[ 22, '22' ], [ 23, '23' ] ]
								}),
								listeners : {
									'change' : validBeginEndTime
								}

							},
							{
								xtype : 'displayfield',
								value : ':'
							},
							{
								xtype : 'combo',
								displayField : 'text',
								valueField : 'value',
								mode : 'local',
								triggerAction : 'all',
								name : 'begin_minute',
								value : '',
								regex : /^([0-5][0-9])$/,
								regexText : "只能输入00-59的数字",
								width : 50,
								store : new Ext.data.ArrayStore({
									fields : [ 'value', 'text' ],
									data : [ [ 00, '00' ], [ 10, '10' ],
											[ 20, '20' ], [ 30, '30' ],
											[ 40, '40' ], [ 50, '50' ] ]
								}),
								listeners : {
									'change' : validBeginEndTime
								}

							} ]
				},
				{
					xtype : 'panel',
					border : false,
					layout : 'hbox',
					fieldLabel : '结束时间',
					items : [
							{
								xtype : 'datefield',
								name : 'end_date',
								format : 'Y-m-d',
								editable : false,
								value : '',
								listeners : {
									'change' : validBeginEndTime
								}
							},
							{
								xtype : 'combo',
								displayField : 'text',
								valueField : 'value',
								mode : 'local',
								triggerAction : 'all',
								editable : false,
								name : 'end_hour',
								value : Ext.util.Format.date(new Date(), "H"),
								width : 50,
								store : new Ext.data.ArrayStore({
									fields : [ 'value', 'text' ],
									data : [ [ 00, '00' ], [ 01, '01' ],
											[ 02, '02' ], [ 03, '03' ],
											[ 04, '04' ], [ 05, '05' ],
											[ 06, '06' ], [ 07, '07' ],
											[ 08, '08' ], [ 09, '09' ],
											[ 10, '10' ], [ 11, '11' ],
											[ 12, '12' ], [ 13, '13' ],
											[ 14, '14' ], [ 15, '15' ],
											[ 16, '16' ], [ 17, '17' ],
											[ 18, '18' ], [ 19, '19' ],
											[ 20, '20' ], [ 21, '21' ],
											[ 22, '22' ], [ 23, '23' ] ]
								}),
								listeners : {
									'change' : validBeginEndTime
								}

							},
							{
								xtype : 'displayfield',
								value : ':'
							},
							{
								xtype : 'combo',
								displayField : 'text',
								valueField : 'value',
								mode : 'local',
								triggerAction : 'all',
								name : 'end_minute',
								regex : /^([0-5][0-9])$/,
								regexText : "只能输入00-59的数字",
								value : 0,
								width : 50,
								store : new Ext.data.ArrayStore({
									fields : [ 'value', 'text' ],
									data : [ [ 00, '00' ], [ 10, '10' ],
											[ 20, '20' ], [ 30, '30' ],
											[ 40, '40' ], [ 50, '50' ] ]
								}),
								listeners : {
									'change' : validBeginEndTime
								}

							} ]
				}, {
					xtype : 'displayfield',
					name : 'app_datetime',
					fieldLabel : '申请时间',
					value : ''
				}, {
					xtype : 'hidden',
					fieldLabel : '申请人ID',
					name : 'app_user_id',
					allowBlank : false,
					width : 300
				}, {
					xtype : 'displayfield',
					fieldLabel : '申请人',
					name : 'app_user_name',
					allowBlank : false,
					width : 300

				}, {
					xtype : 'textarea',
					fieldLabel : '备注',
					name : 'remark',
					width : 300,
					allowBlank : true
				}, {
					xtype : 'hidden',
					fieldLabel : '审批人ID',
					name : 'assigned_id',
					width : 300
				}, {
					xtype : 'displayfield',
					fieldLabel : '审批人',
					name : 'assigned_Name',
					width : 300

				}, {
					xtype : 'displayfield',
					name : 'assigned_datetime',
					fieldLabel : '审批时间'
				}, {
					xtype : 'hidden',
					name : 'status_id',
					fieldLabel : '状态ID'
				}, {
					xtype : 'hidden',
					name : 'status_name',
					fieldLabel : '状态'
				} ]
	});
	var str = "";
	if (button_num == 'create') {
		str = "-创建"
	} else if (button_num == 'agree') {
		str = "-审批";
	} else if (button_num == 'update') {
		str = "-修改";
	} else if (button_num == 'cancel') {
		str = "-撤销";
	} else {
		str = "";
	}

	var applyCarFormWin = new Ext.Window(
			{
				title : '预约信息' + str,
				width : 550,
				height : 480,
				tbar : {
					xtype : 'toolbar',
					items : [ {
						xtype : 'button',
						text : '提交',
						ref : '../save_btn',
						id : 'save_btn',
						iconCls : 'save',
						handler : function() {
							var array = vehiclesGrid.getSelectionModel()
									.getSelections();
							var caridStrs = "";
							for ( var i = 0; i < array.length; i++) {
								caridStrs += array[i].data.car_id + ",";
							}
							if (caridStrs.length > 1) {
								caridStrs = caridStrs.substring(0,
										caridStrs.length - 1);
							}
							var form_id = applyCarForm.getForm().findField(
									'form_id').getValue();
							var begin_datetime = Ext.util.Format.date(
									applyCarForm.getForm().findField(
											'begin_date').getValue(), "Y-m-d ")
									+ applyCarForm.getForm().findField(
											"begin_hour").getValue()
									+ ':'
									+ applyCarForm.getForm().findField(
											"begin_minute").getValue() + ":00";
							var end_datetime = Ext.util.Format.date(
									applyCarForm.getForm()
											.findField('end_date').getValue(),
									"Y-m-d ")
									+ applyCarForm.getForm().findField(
											"end_hour").getValue()
									+ ':'
									+ applyCarForm.getForm().findField(
											"end_minute").getValue() + ":00";
							var priority = applyCarForm.getForm().findField(
									'priority').getValue();
							var app_datetime = applyCarForm.getForm()
									.findField('app_datetime').getValue();
							var assigned_id = applyCarForm.getForm().findField(
									"assigned_id").getValue();
							var assigned_datetime = applyCarForm.getForm()
									.findField("assigned_datetime").getValue();
							// 同意的保存， 且审批人人不为空的修改
							if (button_num != 'cancel') {
								if (caridStrs == null || "" == caridStrs) {
									Ext.MessageBox.alert("提示", "车辆不允许为空！");
									return;
								}
								Ext.Ajax
										.request({
											url : __cxtPath
													+ '/applyManageMaint.do?method=getMeetingRoomUsingTimeList',
											method : 'post',
											params : {
												param_id : caridStrs,
												form_id : form_id,
												begin_datetime : begin_datetime,
												end_datetime : end_datetime,

												type : 'car'
											},
											success : function(response, action) {
												var action = Ext
														.decode(response.responseText);
												var isInuse = action.attributes.isInuse;
												if (isInuse) {
													Ext.MessageBox.alert("提示",
															"此公务车的预约时段的冲突！");
													return;
												} else {
													applyCarForm
															.getForm()
															.submit(
																	{
																		url : __cxtPath
																				+ '/applyManageMaint.do?method=doSaveAppCars',
																		params : {
																			form_id : form_id,
																			priority : priority,
																			begin_datetime : begin_datetime,
																			end_datetime : end_datetime,
																			app_datetime : app_datetime,
																			status_id : status_id,
																			assigned_id : assigned_id,
																			assigned_datetime : assigned_datetime,
																			caridStrs : caridStrs
																		},
																		scope : this,
																		waitTitle : "请稍候",
																		waitMsg : '提交数据，请稍候...',
																		success : function(
																				response,
																				action) {
																			if (button_num == 'agree') {
																				Ext.MessageBox
																						.alert(
																								action.result.messageTitle,
																								'审批成功！');

																			} else if (button_num == 'cancel') {
																				Ext.MessageBox
																						.alert(
																								action.result.messageTitle,
																								'撤销成功！');
																			} else {
																				Ext.MessageBox
																						.alert(
																								action.result.messageTitle,
																								action.result.message);
																			}
																			applyCarFormWin
																					.hide();
																			grid.store
																					.reload();
																		},
																		failure : function(
																				response,
																				action) {
																			com.bitc.xmis.util.AlertUtil
																					.formSubmitFailedAlert(action);
																		}
																	});
												}
											},
											failure : function(response, action) {
												Ext.MessageBox.alert("提示",
														"判断预约时段的是否冲突出错！");
												return;
											}
										});

							} else {

								applyCarForm
										.getForm()
										.submit(
												{
													url : __cxtPath
															+ '/applyManageMaint.do?method=doSaveAppCars',
													params : {
														form_id : form_id,
														priority : priority,
														begin_datetime : begin_datetime,
														end_datetime : end_datetime,
														app_datetime : app_datetime,
														status_id : status_id,
														caridStrs : caridStrs
													},
													scope : this,
													waitTitle : "请稍候",
													waitMsg : '提交数据，请稍候...',
													success : function(
															response, action) {
														if (button_num == 'agree') {
															Ext.MessageBox
																	.alert(
																			action.result.messageTitle,
																			'审批成功！');

														} else if (button_num == 'cancel') {
															Ext.MessageBox
																	.alert(
																			action.result.messageTitle,
																			'撤销成功！');
														} else {
															Ext.MessageBox
																	.alert(
																			action.result.messageTitle,
																			action.result.message);
														}
														applyCarFormWin.hide();
														grid.store.reload();
													},
													failure : function(
															response, action) {
														com.bitc.xmis.util.AlertUtil
																.formSubmitFailedAlert(action);
													}
												});
							}

						}
					} ]
				},
				items : [ vehiclesPanel, applyCarForm ],
				closable : true,
				closeAction : 'hide',
				modal : true,
				layout : 'border'
			});
	applyCarFormWin.show();

	if (button_num == 'create') {
		applyCarForm.getForm().findField("app_datetime").setValue(
				Ext.util.Format.date(new Date(), 'Y-m-d H:i:s'));
		applyCarForm.getForm().findField("status_id").setValue(0);
		applyCarForm.getForm().findField("status_name").setValue(
				com.bitc.xmis.Combos.STATUS_ARRAY[0]);

		applyCarForm.getForm().findField("app_user_id").setValue(__emp.userID);
		applyCarForm.getForm().findField("app_user_name").setValue(
				__emp.userName);

		applyCarForm.getForm().findField("begin_date").setValue(
				Ext.util.Format.date(new Date().add('d', 1), 'Y-m-d'));
		applyCarForm.getForm().findField("begin_hour").setValue(
				Ext.util.Format.date(new Date(), "H"));
		applyCarForm.getForm().findField("begin_minute").setValue(
				Ext.util.Format.date(new Date(), "i"));
		applyCarForm.getForm().findField("end_date").setValue(
				Ext.util.Format.date(new Date().add('d', 1), 'Y-m-d'));
		applyCarForm.getForm().findField("end_hour").setValue(
				Ext.util.Format.date(new Date(), "H"));
		applyCarForm.getForm().findField("end_minute").setValue(
				Ext.util.Format.date(new Date(), "i"));
	} else {

		applyCarForm
				.getForm()
				.load(
						{
							url : __cxtPath
									+ '/applyManageMaint.do?method=getApplyCarByFormId',
							params : {
								form_id : form_id
							},
							waitMsg : '正在载入数据...',
							waitTitle : '请等待...',
							success : function(form, action) {

								var form_id = applyCarForm.getForm().findField(
										"form_id").getValue();
								var a = vehiclesGrid.store.data.length;

								var begin_datetime_value = applyCarForm
										.getForm().findField("begin_datetime")
										.getValue();
								applyCarForm.getForm().findField("begin_date")
										.setValue(
												Ext.util.Format.date(
														begin_datetime_value,
														'Y-m-d'));
								applyCarForm.getForm().findField("begin_hour")
										.setValue(
												Ext.util.Format.date(
														begin_datetime_value,
														'H'));
								applyCarForm.getForm()
										.findField("begin_minute").setValue(
												Ext.util.Format.date(
														begin_datetime_value,
														'i'));
								var end_datetime_value = applyCarForm.getForm()
										.findField("end_datetime").getValue();
								applyCarForm.getForm().findField("end_date")
										.setValue(
												Ext.util.Format.date(
														end_datetime_value,
														'Y-m-d'));
								applyCarForm
										.getForm()
										.findField("end_hour")
										.setValue(
												Ext.util.Format
														.date(
																end_datetime_value,
																'H'));
								applyCarForm
										.getForm()
										.findField("end_minute")
										.setValue(
												Ext.util.Format
														.date(
																end_datetime_value,
																'i'));
								applyCarForm.getForm().findField(
										"app_user_name").setValue(
										applyCarForm.getForm().findField(
												"app_surname").getValue()
												+ applyCarForm.getForm()
														.findField("app_name")
														.getValue());

								if (button_num == 'agree') {
									applyCarForm.getForm().findField(
											"assigned_id").setValue(
											__emp.userID);
									applyCarForm.getForm().findField(
											"assigned_Name").setValue(
											__emp.userName);
									applyCarForm.getForm().findField(
											"assigned_datetime").setValue(
											Ext.util.Format.date(new Date(),
													'Y-m-d H:i:s'));
								} else if (button_num == 'update'
										|| button_num == 'cancel') {
									applyCarForm
											.getForm()
											.findField("assigned_Name")
											.setValue(
													applyCarForm
															.getForm()
															.findField(
																	"assigned_surname")
															.getValue()
															+ applyCarForm
																	.getForm()
																	.findField(
																			"assigned_name")
																	.getValue());
									applyCarForm
											.getForm()
											.findField("assigned_datetime")
											.setValue(
													Ext.util.Format
															.date(
																	applyCarForm
																			.getForm()
																			.findField(
																					"assigned_datetime")
																			.getValue(),
																	'Y-m-d H:i:s'));
								}

							},
							failure : function(form, action) {
								Ext.MessageBox.alert('提示', '载入失败');
							}

						});

	}

	function validBeginEndTime() {
		var begin_datetime = Ext.util.Format.date(applyCarForm.getForm()
				.findField('begin_date').getValue(), "Y-m-d ")
				+ applyCarForm.getForm().findField("begin_hour").getValue()
				+ ':'
				+ applyCarForm.getForm().findField("begin_minute").getValue()
				+ ":00";
		var end_datetime = Ext.util.Format.date(applyCarForm.getForm()
				.findField('end_date').getValue(), "Y-m-d ")
				+ applyCarForm.getForm().findField("end_hour").getValue()
				+ ':'
				+ applyCarForm.getForm().findField("end_minute").getValue()
				+ ":00";
		Ext.Ajax.request({
			url : __cxtPath + '/applyManageMaint.do?method=validBeginEndTime',
			method : 'post',
			params : {
				begin_datetime : begin_datetime,
				end_datetime : end_datetime
			},
			success : function(response, action) {
				var action = Ext.decode(response.responseText);
				var isInuse = action.attributes.isInuse;
				if (!isInuse) {
					Ext.MessageBox.alert("提示", "结束时间必须大于开始时间！");
					return;
				}
			},
			failure : function(response, action) {
				Ext.MessageBox.alert("提示", "判断结束时间必须大于开始时间出错！");
				return;
			}
		});
	}
};
