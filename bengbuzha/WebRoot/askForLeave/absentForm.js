Ext.ns('com.bitc.xmis.AskForLeave');
com.bitc.xmis.AskForLeave.AbsentFormPanelRecord = Ext.data.Record.create([{
			"mapping" : "form_id",
			"name" : "form_id",
			"type" : "string"
		},  {
			"mapping" : "absent_type_id",
			"name" : "absent_type_id",
			"type" : "int"
		},{
			"mapping" : "absent_type_name",
			"name" : "absent_type_group",
			"type" : "string"
		},{
			"mapping" : "plan_sum",
			"name" : "plan_sum_hidden",
			"type" : "string"
		}, {
			"mapping" : "actual_sum",
			"name" : "actual_sum_hidden",
			"type" : "string"
		},{
			"dateFormat" : "Y-m-d H:i:s",
			"mapping" : "plan_begin_date",
			"name" : "plan_begin_date",
			"type" : "date"
		}, {
			"mapping" : "plan_begin_date",
			"name" : "plan_begin_time",
			"type" : "string"
		}, {
			"mapping" : "plan_end_date",
			"name" : "plan_end_time",
			"type" : "string"
		}, {
			"mapping" : "actual_begin_date",
			"name" : "actual_begin_time",
			"type" : "string"
		}, {
			"mapping" : "actual_end_date",
			"name" : "actual_end_time",
			"type" : "string"
		}, {
			"dateFormat" : "Y-m-d H:i:s",
			"mapping" : "plan_end_date",
			"name" : "plan_end_date",
			"type" : "date"
		}, {
			"dateFormat" : "Y-m-d H:i:s",
			"mapping" : "actual_begin_date",
			"name" : "actual_begin_date",
			"type" : "date"
		}, {
			"dateFormat" : "Y-m-d H:i:s",
			"mapping" : "actual_end_date",
			"name" : "actual_end_date",
			"type" : "date"
		}, {
			"mapping" : "leavetime",
			"name" : "leavetime"
		}, {
			"mapping" : "reason",
			"name" : "reason",
			"type" : "string"
		}, {
			"mapping" : "app_datetime",
			"name" : "app_datetime"
		}, {
			"mapping" : "app_user_id",
			"name" : "app_user_id",
			"type" : "int"
		}, {
			"mapping" : "app_user_name",
			"name" : "app_user_name",
			"type" : "string"
		}]);

com.bitc.xmis.AskForLeave.AbsentFormPanel = new Ext.form.FormPanel({
			ref : 'form',
			region : 'center',
			padding : 20,
			autoScroll : true,
			border : false,
			labelWidth : 90,
			labelAlign : 'right',
			reader : new Ext.data.JsonReader({
						root : 'rows'
					}, com.bitc.xmis.AskForLeave.AbsentFormPanelRecord),
			items : [{
						xtype : 'hidden',
						fieldLabel : '申请人',
						name : 'app_user_id',
						width : 300
					}, {
						xtype : 'hidden',
						name : 'plan_begin_time',
						width : 120,
						allowBlank : false
					}, {
						xtype : 'hidden',
						name : 'plan_end_time',
						width : 120,
						allowBlank : false
					}, {
						xtype : 'hidden',
						name : 'actual_begin_time',
						width : 120,
						allowBlank : false
					}, {
						xtype : 'hidden',
						name : 'actual_end_time',
						width : 120,
						allowBlank : false
					}, {
						xtype : 'panel',
						layout : 'column',
						border : false,
						items : [{
									xtype : 'panel',
									layout : 'form',
									columnWith : 0.5,
									border : false,
									labelAlign : 'rigth',
									items : [{
												xtype : 'displayfield',
												fieldLabel : '申请人',
												name : 'app_user_name',
												width : 300
											}]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWith : 0.5,
									border : false,
									labelAlign : 'rigth',
									items : [{
												xtype : 'displayfield',
												fieldLabel : '申请时间',
												name : 'app_datetime',
												width : 300
											}]
								}]
					}, {
						xtype : 'fieldset',
						id : 'app_fieldset',
						width : 800,
						title : '请假申请',
						defaults : {
							border : false
						},
						items : [{
							xtype : 'panel',
							border : false,
							layout : 'hbox',
							fieldLabel : '请假时间',
							items : [{
										xtype : 'datefield',
										name : 'plan_begin_date',
										format : 'Y-m-d',
										//disabledDates:['08/16/2014'],
										width : 120,
										allowBlank : false
									}, {
										xtype : 'combo',
										displayField : 'text',
										valueField : 'value',
										mode : 'local',
										triggerAction : 'all',
										editable : false,
										name : 'plan_begin_hour',
										value : '00',
										width : 50,
										store : new Ext.data.ArrayStore({
													fields : ['value', 'text'],
													data : [[00, '00'],
															[01, '01'],
															[02, '02'],
															[03, '03'],
															[04, '04'],
															[05, '05'],
															[06, '06'],
															[07, '07'],
															[08, '08'],
															[09, '09'],
															[10, '10'],
															[11, '11'],
															[12, '12'],
															[13, '13'],
															[14, '14'],
															[15, '15'],
															[16, '16'],
															[17, '17'],
															[18, '18'],
															[19, '19'],
															[20, '20'],
															[21, '21'],
															[22, '22'],
															[23, '23']]
												})

									}, {
										xtype : 'displayfield',
										value : '时 至'
									}, {
										xtype : 'datefield',
										name : 'plan_end_date',
										allowBlank : false,
										width : 120,
										format : 'Y-m-d'
									}, {
										xtype : 'combo',
										displayField : 'text',
										valueField : 'value',
										mode : 'local',
										triggerAction : 'all',
										editable : false,
										name : 'plan_end_hour',
										value : '23',
										width : 50,
										store : new Ext.data.ArrayStore({
													fields : ['value', 'text'],
													data : [[00, '00'],
															[01, '01'],
															[02, '02'],
															[03, '03'],
															[04, '04'],
															[05, '05'],
															[06, '06'],
															[07, '07'],
															[08, '08'],
															[09, '09'],
															[10, '10'],
															[11, '11'],
															[12, '12'],
															[13, '13'],
															[14, '14'],
															[15, '15'],
															[16, '16'],
															[17, '17'],
															[18, '18'],
															[19, '19'],
															[20, '20'],
															[21, '21'],
															[22, '22'],
															[23, '23']]
												})

									}, {
										xtype : 'displayfield',
										value : '时'
									}]
						},  {
							xtype : 'hidden',
							name : 'plan_sum_hidden'
						},{
								xtype : 'panel',
								border : false,
								layout : 'hbox',
								fieldLabel : '合计',
								items : [{
									xtype : 'textfield',
									fieldLabel : '合计',
									name : 'plan_sum',
									width : 100
								}, {
									xtype:'displayfield',
									value:'天'
							}]
						}, {
							xtype : 'hidden',
							id : 'absent_type_id',
							name : 'absent_type_id'
						},{
							xtype : 'combo',
							allowBlank : false,
							fieldLabel : '请假类型',
							id : 'absent_type_group',
							name : 'absent_type_group',
							valueField : 'absent_type_id',
							displayField : 'absent_type_name',
									mode : 'local',
									triggerAction : 'all',
									editable : false,
									store : new Ext.data.Store({
										proxy : new Ext.data.HttpProxy({
													url : __cxtPath
															+ '/applyManageMaint.do?method=getAbsentList'
												}),
									
										 autoLoad : true,
										reader : new Ext.data.JsonReader({
													root : 'rows',
												 fields : [{
													name: 'absent_type_id',
													mapping: 'absent_type_id'
												},{
													name: 'absent_type_name',
													mapping: 'absent_type_name'
												}]
										})
										
									})

						}, {
							xtype : 'textarea',
							fieldLabel : '原因',
							name : 'reason',
							width : 410
						}]
					}, {
						xtype : 'fieldset',
						width : 800,
						id : 'back_fieldset',
						title : '销假申请',
						defaults : {
							border : false
						},
						items : [{
												xtype : 'displayfield',
												fieldLabel : '销假时间',
												name : 'leavetime',
												width : 300
											},{
							xtype : 'panel',
							border : false,
							layout : 'hbox',
							fieldLabel : '实际请假时间',
							items : [{
										xtype : 'datefield',
										name : 'actual_begin_date',
										format : 'Y-m-d',
										width : 120,
										value : ''
									}, {
										xtype : 'combo',
										displayField : 'text',
										valueField : 'value',
										mode : 'local',
										triggerAction : 'all',
										editable : false,
										name : 'actual_begin_hour',
										value : '00',
										width : 50,
										store : new Ext.data.ArrayStore({
													fields : ['value', 'text'],
													data : [[00, '00'],
															[01, '01'],
															[02, '02'],
															[03, '03'],
															[04, '04'],
															[05, '05'],
															[06, '06'],
															[07, '07'],
															[08, '08'],
															[09, '09'],
															[10, '10'],
															[11, '11'],
															[12, '12'],
															[13, '13'],
															[14, '14'],
															[15, '15'],
															[16, '16'],
															[17, '17'],
															[18, '18'],
															[19, '19'],
															[20, '20'],
															[21, '21'],
															[22, '22'],
															[23, '23']]
												})

									}, {
										xtype : 'displayfield',
										value : '时 至'
									}, {
										xtype : 'datefield',
										name : 'actual_end_date',
										format : 'Y-m-d',
										width : 120,
										value : ''
									}, {
										xtype : 'combo',
										displayField : 'text',
										valueField : 'value',
										mode : 'local',
										triggerAction : 'all',
										editable : false,
										name : 'actual_end_hour',
										value : '23',
										width : 50,
										store : new Ext.data.ArrayStore({
													fields : ['value', 'text'],
													data : [[00, '00'],
															[01, '01'],
															[02, '02'],
															[03, '03'],
															[04, '04'],
															[05, '05'],
															[06, '06'],
															[07, '07'],
															[08, '08'],
															[09, '09'],
															[10, '10'],
															[11, '11'],
															[12, '12'],
															[13, '13'],
															[14, '14'],
															[15, '15'],
															[16, '16'],
															[17, '17'],
															[18, '18'],
															[19, '19'],
															[20, '20'],
															[21, '21'],
															[22, '22'],
															[23, '23']]
												})

									}, {
										xtype : 'displayfield',
										value : '时'
									}]
						}, {
							xtype : 'hidden',
							name : 'actual_sum_hidden'
						}, {
								xtype : 'panel',
								border : false,
								layout : 'hbox',
								fieldLabel : '合计',
								items : [{
									xtype : 'textfield',
									fieldLabel : '合计',
									name : 'actual_sum',
									width : 100
								}, {
									xtype:'displayfield',
									value:'天'
							}]
						}]
					}]

		});
