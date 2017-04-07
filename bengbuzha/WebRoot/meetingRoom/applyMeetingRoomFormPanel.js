Ext.ns('com.bitc.xmis.Combos');
com.bitc.xmis.Combos.STATUS_ARRAY = ['', '预约申请', '修改申请', '预约中', '已撤销', '已过期',
		'使用中'];
com.bitc.xmis.Combos.STATUS = {
	create : 1,
	update : 2,
	agree : 3,
	cancel : 4
};

com.bitc.xmis.Combos.MeetingRoomCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择会议室',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
					url : String
							.format(
									'{0}/applyManageMaint.do?method=getMeetingRoomCombo',
									__cxtPath),
					root : 'rows',
					autoLoad : true,
					fields : [{
								name : "id",
								type : "string",
								mapping : "room_id"
							}, {
								name : "name",
								type : "string",
								mapping : "room_No"
							}],
					listeners : {
						scope : this,
						'load' : function() {
							this.setValue(this.value);
						}
					}
				});
		com.bitc.xmis.Combos.MeetingRoomCombo.superclass.initComponent
				.call(this);
	}
});

Ext.ns('com.bitc.xmis');
com.bitc.xmis.ApplyFormPanelRecord = Ext.data.Record.create([{
			mapping : "id",
			name : "id",
			type : "string"
		}, {
			mapping : "room_id",
			name : "room_id",
			type : "string"
		}, {
			mapping : "priority",
			name : "priority",
			type : "int"
		}, {
			mapping : "sponsor_id",
			name : "sponsor_id",
			type : "int"
		}, {
			mapping : "sponsor_surname",
			name : "sponsor_surname",
			type : "string"
		}, {
			mapping : "sponsor_name",
			name : "sponsor_name",
			type : "string"
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
			mapping : "number_people",
			name : "number_people",
			type : "int"
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
			"dateFormat" : "Y-m-d H:i:s",
			mapping : "assigned_datetime",
			name : "assigned_datetime",
			type : "date"
		}, {
			mapping : "status_id",
			name : "status_id",
			type : "int"
		}]);
com.bitc.xmis.ApplyMeetRoomWinShow = function(grid, button_num, form_id,
		status_id) {
	var str = "";
	if (button_num == 'create') {
		str = "创建"
	} else if (button_num == 'agree') {
		str = "审批";
	} else if (button_num == 'update') {
		str = "修改";
	} else if (button_num == 'cancel') {
		str = "撤销";
	} else {
		str = "";
	}

	var meetingRoomCombo = new com.bitc.xmis.Combos.MeetingRoomCombo({
				fieldLabel : '会议室编号',
				ref : 'room_id',
				width : 100,
				hiddenName : 'room_id'
			});

	var applyRoomForm = new Ext.form.FormPanel({
		ref : 'form',
		region : 'center',
		padding : 20,
		autoScroll : true,
		border : false,
		labelWidth : 101,
		labelAlign : 'right',
		reader : new Ext.data.JsonReader({
					root : 'rows'
				}, com.bitc.xmis.ApplyFormPanelRecord),
		tbar : {
			xtype : 'toolbar',
			items : [{
				xtype : 'button',
				text : '提交',
				ref : '../save_btn',
				iconCls : 'save',
				handler : function() {

					var id = applyRoomForm.getForm().findField('id').getValue();
					var begin_datetime = Ext.util.Format.date(applyRoomForm
									.getForm().findField('begin_date')
									.getValue(), "Y-m-d ")
							+ applyRoomForm.getForm().findField("begin_hour")
									.getValue()
							+ ':'
							+ applyRoomForm.getForm().findField("begin_minute")
									.getValue() + ":00";
					var end_datetime = Ext.util.Format.date(
							applyRoomForm.getForm().findField('end_date')
									.getValue(), "Y-m-d ")
							+ applyRoomForm.getForm().findField("end_hour")
									.getValue()
							+ ':'
							+ applyRoomForm.getForm().findField("end_minute")
									.getValue() + ":00";

					var priority = applyRoomForm.getForm()
							.findField('priority').getValue();
					var app_datetime = applyRoomForm.getForm()
							.findField('app_datetime').getValue();

					var assigned_id = applyRoomForm.getForm()
							.findField("assigned_id").getValue();
					var assigned_datetime = applyRoomForm.getForm()
							.findField("assigned_datetime").getValue();

					if (end_datetime <= begin_datetime) {
						Ext.MessageBox.alert('提示', '结束时间应该大于开始时间');
						return;
					}

					if (button_num != 'cancel') {
						var param_id = applyRoomForm.getForm()
								.findField('room_id').getValue();
						if (param_id == null || "" == param_id) {
							Ext.MessageBox.alert("提示", "会议室不允许为空！");
							return;
						}
						Ext.Ajax.request({
							url : __cxtPath
									+ '/applyManageMaint.do?method=getMeetingRoomUsingTimeList',
							method : 'post',
							params : {
								param_id : param_id,
								form_id : id,
								begin_datetime : begin_datetime,
								end_datetime : end_datetime,
								type : 'room'
							},
							success : function(response, action) {
								var action = Ext.decode(response.responseText);
								var isInuse = action.attributes.isInuse;
								if (isInuse) {
									Ext.MessageBox.alert("提示", "此会议室的预约时段的冲突！");
									return;
								} else {
									Ext.MessageBox.confirm('提示', '是否确定' + str
													+ '？', function(btn, text) {
												if (btn == 'yes') {
													applyRoomForm.getForm()
															.submit({
																url : __cxtPath
																		+ '/applyManageMaint.do?method=doSaveAppMeetingRoom',
																params : {
																	id : id,
																	priority : priority,
																	begin_datetime : begin_datetime,
																	end_datetime : end_datetime,
																	app_datetime : app_datetime,
																	status_id : status_id,
																	assigned_id : assigned_id,
																	assigned_datetime : assigned_datetime
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
																	applyRoomFormWin
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
											});
								}
							},
							failure : function(response, action) {
								Ext.MessageBox.alert("提示", "判断预约时段的是否冲突出错！");
								return;
							}
						});
					} else {
						Ext.MessageBox.confirm('提示', '是否确定' + str + '？',
								function(btn, text) {
									if (btn == 'yes') {
										applyRoomForm.getForm().submit({
											url : __cxtPath
													+ '/applyManageMaint.do?method=doSaveAppMeetingRoom',
											params : {
												id : id,
												priority : priority,
												begin_datetime : begin_datetime,
												end_datetime : end_datetime,
												app_datetime : app_datetime,
												status_id : status_id,
												assigned_id : assigned_id,
												assigned_datetime : assigned_datetime
											},
											scope : this,
											waitTitle : "请稍候",
											waitMsg : '提交数据，请稍候...',
											success : function(response, action) {
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
												applyRoomFormWin.hide();
												grid.store.reload();
											},
											failure : function(response, action) {
												com.bitc.xmis.util.AlertUtil
														.formSubmitFailedAlert(action);
											}
										});
									}
								});
					}

				}
			}]
		},
		items : [{
					xtype : 'hidden',
					name : 'id',
					fieldLabel : 'id',
					style : 'color:blue'
				}, {
					xtype : 'hidden',
					fieldLabel : '开始时间字符串',
					name : 'begin_datetime',
					width : 300
				}, {
					xtype : 'hidden',
					fieldLabel : '结束时间字符串',
					name : 'end_datetime',
					width : 300
				}, {
					xtype : 'hidden',
					fieldLabel : '召唤人姓氏',
					name : 'sponsor_surname',
					width : 300
				}, {
					xtype : 'hidden',
					fieldLabel : '召唤人名字',
					name : 'sponsor_name',
					width : 300
				}, {
					xtype : 'hidden',
					fieldLabel : '审批人姓氏',
					name : 'assigned_surname',
					width : 300
				}, {
					xtype : 'hidden',
					fieldLabel : '审批人名字',
					name : 'assigned_name',
					width : 300
				}, {
					xtype : 'hidden',
					fieldLabel : '申请人姓氏',
					name : 'app_surname',
					width : 300
				}, {
					xtype : 'hidden',
					fieldLabel : '申请人名字',
					name : 'app_name',
					width : 300
				}, {
					xtype : 'panel',
					border : false,
					fieldLabel : '会议室选择',
					layout : 'hbox',
					items : [meetingRoomCombo, {
								xtype : 'displayfield',
								value : '<font color=red> 审批人可以重新分配</font>',
								width : 150
							}]
				}, {
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
								fields : ['value', 'text'],
								data : [[1, '紧急'], [0, '普通']]
							}),
					listeners : {
						'select' : function(combo, record, index) {
							var selValue = record.data.value;
							if (selValue == 0) {
							} else if (selValue == 1) {
							}
						}
					}
				}, {
					xtype : 'hidden',
					name : 'sponsor_id',
					fieldLabel : '召集人ID'// 召集人
				}, new Ext.ux.form.ChooseOrgTreeField({
							fieldLabel : '召集人',
							name : 'sponsor_Name',
							modal : true,
							allowBlank : false,
							width : 100,
							winConfig : {
								title : '召集人设置'
							},
							singleSelect : true,
							onlyPerson : true,
							getExpandPath : function() {
								return {
									empId : applyRoomForm.getForm()
											.findField('sponsor_id').getValue()
								};
							},
							rootConfig : function() {
								return {
									id : 'comp-old-1'
								};
							},
							dataUrl : '/orgMaint.do?method=getOrganizationTree',
							clearValueFun : function() {
								applyRoomForm.getForm().findField('sponsor_id')
										.setValue(null);
								applyRoomForm.getForm()
										.findField('sponsor_Name')
										.setValue(null);
							},
							setValueFun : function(result) {

								var empName = '';
								var empID = '';
								for (var i = 0; i < result.length; i++) {
									if (i == result.length - 1) {
										empID = empID + result[i].id;
										empName = empName + result[i].text;
									} else {
										empID = empID + result[i].id + ',';
										empName = empName + result[i].text
												+ ',';
									}
								}
								applyRoomForm.getForm()
										.findField('sponsor_Name')
										.setValue(empName);
								applyRoomForm.getForm().findField('sponsor_id')
										.setValue(empID.split('-')[2]);
							}
						}), {
					xtype : 'textfield',
					fieldLabel : '事由',
					name : 'reason',
					allowBlank : false,
					width : 300,
					blankText : '该输入项为必输项'
				}, {
					xtype : 'panel',
					border : false,
					layout : 'hbox',
					fieldLabel : '开始时间',
					items : [{
								xtype : 'datefield',
								name : 'begin_date',
								format : 'Y-m-d',
								editable : false,
								value : '',
								listeners : {
									'change' : validBeginEndTime
								}
							}, {
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
											fields : ['value', 'text'],
											data : [[00, '00'], [01, '01'],
													[02, '02'], [03, '03'],
													[04, '04'], [05, '05'],
													[06, '06'], [07, '07'],
													[08, '08'], [09, '09'],
													[10, '10'], [11, '11'],
													[12, '12'], [13, '13'],
													[14, '14'], [15, '15'],
													[16, '16'], [17, '17'],
													[18, '18'], [19, '19'],
													[20, '20'], [21, '21'],
													[22, '22'], [23, '23']]
										}),
								listeners : {
									'change' : validBeginEndTime
								}

							}, {
								xtype : 'displayfield',
								value : ':'
							}, {
								xtype : 'combo',
								displayField : 'text',
								valueField : 'value',
								mode : 'local',
								triggerAction : 'all',
								name : 'begin_minute',
								regex : /^([0-5][0-9])$/,
								regexText : "只能输入00-59的数字",
								value : '',
								width : 50,
								store : new Ext.data.ArrayStore({
											fields : ['value', 'text'],
											data : [[00, '00'], [10, '10'],
													[20, '20'], [30, '30'],
													[40, '40'], [50, '50']]
										}),
								listeners : {
									'change' : validBeginEndTime
								}

							}]
				}, {
					xtype : 'panel',
					border : false,
					layout : 'hbox',
					fieldLabel : '结束时间',
					items : [{
								xtype : 'datefield',
								name : 'end_date',
								format : 'Y-m-d',
								editable : false,
								value : '',
								listeners : {
									'change' : validBeginEndTime
								}
							}, {
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
											fields : ['value', 'text'],
											data : [[00, '00'], [01, '01'],
													[02, '02'], [03, '03'],
													[04, '04'], [05, '05'],
													[06, '06'], [07, '07'],
													[08, '08'], [09, '09'],
													[10, '10'], [11, '11'],
													[12, '12'], [13, '13'],
													[14, '14'], [15, '15'],
													[16, '16'], [17, '17'],
													[18, '18'], [19, '19'],
													[20, '20'], [21, '21'],
													[22, '22'], [23, '23']]
										}),
								listeners : {
									'change' : validBeginEndTime
								}

							}, {
								xtype : 'displayfield',
								value : ':'
							}, {
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
											fields : ['value', 'text'],
											data : [[1, '00'], [10, '10'],
													[20, '20'], [30, '30'],
													[40, '40'], [50, '50']]
										}),
								listeners : {
									'change' : validBeginEndTime
								}

							}]
				}, {
					xtype : 'displayfield',
					name : 'app_datetime',
					fieldLabel : '申请时间',
					value : ''
				}, {
					xtype : 'numberfield',
					name : 'number_people',
					fieldLabel : '开会人数',
					allowBlank : false,
					width : 300,
					blankText : '该输入项为必输项'
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
				}]
	});

	var applyRoomFormWin = new Ext.Window({
				title : '预约信息' + str,
				width : 550,
				height : 450,
				items : applyRoomForm,
				closable : true,
				closeAction : 'hide',
				modal : true,
				layout : 'border'
			});
	applyRoomFormWin.show();

	if (button_num == 'create') {
		applyRoomForm.getForm().findField("app_datetime")
				.setValue(Ext.util.Format.date(new Date(), 'Y-m-d H:i:s'));
		applyRoomForm.getForm().findField("status_id").setValue(0);
		applyRoomForm.getForm().findField("status_name")
				.setValue(com.bitc.xmis.Combos.STATUS_ARRAY[0]);

		applyRoomForm.getForm().findField("app_user_id").setValue(__emp.userID);
		applyRoomForm.getForm().findField("app_user_name")
				.setValue(__emp.userName);

		applyRoomForm.getForm().findField("sponsor_id").setValue(__emp.userID);
		applyRoomForm.getForm().findField("sponsor_Name")
				.setValue(__emp.userName);

		applyRoomForm
				.getForm()
				.findField("begin_date")
				.setValue(Ext.util.Format.date(new Date().add('d', 1), 'Y-m-d'));
		applyRoomForm.getForm().findField("begin_hour")
				.setValue(Ext.util.Format.date(new Date(), "H"));
		applyRoomForm.getForm().findField("begin_minute")
				.setValue(Ext.util.Format.date(new Date(), "i"));
		applyRoomForm.getForm().findField("end_date").setValue(Ext.util.Format
				.date(new Date().add('d', 1), 'Y-m-d'));
		applyRoomForm.getForm().findField("end_hour").setValue(Ext.util.Format
				.date(new Date(), "H"));
		applyRoomForm.getForm().findField("end_minute")
				.setValue(Ext.util.Format.date(new Date(), "i"));
	} else {
		applyRoomForm.getForm().load({
			url : __cxtPath + '/applyManageMaint.do?method=getApplyMeetingRoom',
			params : {
				id : form_id
			},
			waitMsg : '正在载入数据...',
			waitTitle : '请等待...',
			success : function(form, action) {
				var begin_datetime_value = applyRoomForm.getForm()
						.findField("begin_datetime").getValue();
				applyRoomForm.getForm().findField("begin_date")
						.setValue(Ext.util.Format.date(begin_datetime_value,
								'Y-m-d'));
				applyRoomForm.getForm().findField("begin_hour")
						.setValue(Ext.util.Format.date(begin_datetime_value,
								'H'));
				applyRoomForm.getForm().findField("begin_minute")
						.setValue(Ext.util.Format.date(begin_datetime_value,
								'i'));
				var end_datetime_value = applyRoomForm.getForm()
						.findField("end_datetime").getValue();
				applyRoomForm.getForm().findField("end_date")
						.setValue(Ext.util.Format.date(end_datetime_value,
								'Y-m-d'));
				applyRoomForm
						.getForm()
						.findField("end_hour")
						.setValue(Ext.util.Format.date(end_datetime_value, 'H'));
				applyRoomForm
						.getForm()
						.findField("end_minute")
						.setValue(Ext.util.Format.date(end_datetime_value, 'i'));
				applyRoomForm.getForm().findField("app_user_name")
						.setValue(applyRoomForm.getForm()
								.findField("app_surname").getValue()
								+ applyRoomForm.getForm().findField("app_name")
										.getValue());
				applyRoomForm.getForm().findField("sponsor_Name")
						.setValue(applyRoomForm.getForm()
								.findField("sponsor_surname").getValue()
								+ applyRoomForm.getForm()
										.findField("sponsor_name").getValue());

				if (button_num == 'agree') {
					applyRoomForm.getForm().findField("assigned_id")
							.setValue(__emp.userID);
					applyRoomForm.getForm().findField("assigned_Name")
							.setValue(__emp.userName);
					applyRoomForm.getForm().findField("assigned_datetime")
							.setValue(Ext.util.Format.date(new Date(),
									'Y-m-d H:i:s'));

				} else if (button_num == 'update' || button_num == 'cancel') {
					applyRoomForm.getForm().findField("assigned_Name")
							.setValue(applyRoomForm.getForm()
									.findField("assigned_surname").getValue()
									+ applyRoomForm.getForm()
											.findField("assigned_name")
											.getValue());
					applyRoomForm.getForm().findField("assigned_datetime")
							.setValue(Ext.util.Format.date(applyRoomForm.getForm()
									.findField("assigned_datetime").getValue(),
									'Y-m-d H:i:s'));
				}
			},
			failure : function(form, action) {
				Ext.MessageBox.alert('提示', '载入失败');
			}

		});

	}
	function validBeginEndTime() {

		var begin_datetime = Ext.util.Format.date(applyRoomForm.getForm()
						.findField('begin_date').getValue(), "Y-m-d ")
				+ applyRoomForm.getForm().findField("begin_hour").getValue()
				+ ':'
				+ applyRoomForm.getForm().findField("begin_minute").getValue()
				+ ":00";
		var end_datetime = Ext.util.Format.date(applyRoomForm.getForm()
						.findField('end_date').getValue(), "Y-m-d ")
				+ applyRoomForm.getForm().findField("end_hour").getValue()
				+ ':'
				+ applyRoomForm.getForm().findField("end_minute").getValue()
				+ ":00";
		Ext.Ajax.request({
					url : __cxtPath
							+ '/applyManageMaint.do?method=validBeginEndTime',
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
