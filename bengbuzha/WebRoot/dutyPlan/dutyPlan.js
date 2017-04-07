Ext.onReady(function() {
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	var dutyPlanPanel = new Ext.form.FormPanel({
		id : 'dutyPlanPanel',
		autoScroll : true,
		region : 'east',
		split : true,
		width:400,
		collapsible : true,
		bodyStyle : 'padding:30px 5px 0px 5px',
		//region : 'center',
		defaultType : 'textfield',
		labelAlign : 'right',
		labelWidth : 100,
		collapseMode : 'mini',
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
								fieldLabel : 'ID',
								name : 'id',
								hidden : true
								},{
								xtype : 'textfield',
								fieldLabel : '岗位ID',
								name : 'job_type_id',
								hidden : true
							}, {
									xtype : 'panel',
									name:'on_watch_panel',
									layout : 'form',
									columnWidth : 0.33,
									labelWidth : 100,
									border:false,
									labelAlign : 'right',
									items : [
											{
												xtype : 'hidden',
												id : 'on_watch_id',
												name : 'on_watch_id'
											},
											new Ext.ux.form.ChooseOrgTreeField(
													{
														fieldLabel : '值班人员姓名',
														id : 'on_watch_name',
														name : 'on_watch_name',
														width : 250,
														winConfig : {
															title : '值班人员设置'
														},
														singleSelect : true,
														onlyPerson : true,
														expandNodePath : function() {
															var path = '/comp-old-1';
															var deptid = 0;
															if (Ext
																	.getCmp('bDept_id') != null
																	&& (Ext
																			.getCmp('bDept_id')
																			.getValue())
																			.trim() != '') {
																path = path
																		+ '/dept-old-'
																		+ Ext
																				.getCmp('bDept_id')
																				.getValue();
																deptid = Ext
																		.getCmp('bDept_id')
																		.getValue();
															}
															return {
																deptid : parseInt(deptid),
																path : path
															};
														},
														rootConfig : function() {
															return {
																id : 'comp-old-1'
															};
														},
														dataUrl : '/orgMaint.do?method=getOrganizationTree',
														setValueFun : function(
																result) {
															var empName = '';
															var empID = '';
															for (var i = 0; i < result.length; i++) {
																if (i == result.length
																		- 1) {
																	empID = empID
																			+ result[i].id;
																	empName = empName
																			+ result[i].text;
																} else {
																	empID = empID
																			+ result[i].id
																			+ ',';
																	empName = empName
																			+ result[i].text
																			+ ',';
																}
															}
															Ext
																	.getCmp('on_watch_id')
																	.setValue(empID
																			.split('-')[2]);
															Ext
																	.getCmp('on_watch_name')
																	.setValue(empName);
														},
														clearValueList : 'on_watch_id,on_watch_name',
														listeners : {
															'dblclick' : function(
																	select, num) {
																var empId = Ext
																		.getCmp('on_watch_id')
																		.getValue();
																com.bitc.xmis.util
																		.EmpInfoWin(
																				empId,
																				'值班人员信息');

															},
															'nodedblclick' : function(
																	node, e) {
																var path = String
																		.format(
																				'/root{0}',
																				node
																						.getPath());
																parent.xMis
																		.turn(
																				'company-info-'
																						+ Ext
																								.getCmp('bCompany_id')
																								.getValue(),
																				Ext
																						.getCmp('bCompany_name')
																						.getValue(),
																				__cxtPath
																						+ '/crmMaint.do?method=getCompanyInfo&companyID='
																						+ Ext
																								.getCmp('bCompany_id')
																								.getValue()
																						+ '&tab=1&path='
																						+ path,
																				'menu-company-info',
																				true);
															},
															'render' : function(
																	c) {
																Ext.QuickTips
																		.register(
																				{
																					target : c
																							.getEl(),
																					text : '双击可显示详细信息'
																				});
															}
														}
													})]
								
								
								
								
								
							}, {
								xtype : 'datefield',
								fieldLabel : '值班开始日期',
								width : 250,
								format:'Y-m-d',
								name : 'on_watch_date',
								allowBlank : false,
								editable : false
							},{
								xtype : 'timefield',
								name : 'begin_time',
								fieldLabel : '开始时间',
								format: 'H:i:s',
								allowBlank : false,
								increment : 30,
								width : 250,
								editable : false
							},{
								xtype : 'datefield',
								fieldLabel : '值班结束日期',
								width : 250,
								format:'Y-m-d',
								name : 'end_date',
								allowBlank : false,
								editable : false
							},{
								xtype : 'timefield',
								fieldLabel : '结束时间',
								width : 250,
								allowBlank : false,
								format: 'H:i:s',
								increment : 30,
								name : 'end_time',
								editable : false
							},{
								xtype : 'datefield',
								fieldLabel : '计划制定时间',
								width : 250,
								format:'Y-m-d',
								name : 'scheduled_datetime',
								increment : 1,
								value : new Date(),
								editable : false
							}, {
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									allowBlank : false,
									labelWidth : 100,
									border:false,
									labelAlign : 'right',
									items : [
											{
												xtype : 'hidden',
												id : 'scheduled_id',
												name : 'scheduled_id'
											},
											new Ext.ux.form.ChooseOrgTreeField(
													{
														fieldLabel : '计划制定人姓名',
														id : 'scheduled_name',
														name : 'scheduled_name',
														width : 250,
														winConfig : {
															title : '计划制定人设置'
														},
														singleSelect : true,
														onlyPerson : true,
														expandNodePath : function() {
															var path = '/comp-old-1';
															var deptid = 0;
															if (Ext
																	.getCmp('bDept_id') != null
																	&& (Ext
																			.getCmp('bDept_id')
																			.getValue())
																			.trim() != '') {
																path = path
																		+ '/dept-old-'
																		+ Ext
																				.getCmp('bDept_id')
																				.getValue();
																deptid = Ext
																		.getCmp('bDept_id')
																		.getValue();
															}
															return {
																deptid : parseInt(deptid),
																path : path
															};
														},
														rootConfig : function() {
															return {
																id : 'comp-old-1'
															};
														},
														dataUrl : '/orgMaint.do?method=getOrganizationTree',
														setValueFun : function(
																result) {
															var empName = '';
															var empID = '';
															for (var i = 0; i < result.length; i++) {
																if (i == result.length
																		- 1) {
																	empID = empID
																			+ result[i].id;
																	empName = empName
																			+ result[i].text;
																} else {
																	empID = empID
																			+ result[i].id
																			+ ',';
																	empName = empName
																			+ result[i].text
																			+ ',';
																}
															}
															Ext
																	.getCmp('scheduled_id')
																	.setValue(empID
																			.split('-')[2]);
															Ext
																	.getCmp('scheduled_name')
																	.setValue(empName);
														},
														clearValueList : 'scheduled_id,scheduled_name',
														listeners : {
															'dblclick' : function(
																	select, num) {
																var empId = Ext
																		.getCmp('scheduled_id')
																		.getValue();
																com.bitc.xmis.util
																		.EmpInfoWin(
																				empId,
																				'计划制定人信息');

															},
															'nodedblclick' : function(
																	node, e) {
																var path = String
																		.format(
																				'/root{0}',
																				node
																						.getPath());
																parent.xMis
																		.turn(
																				'company-info-'
																						+ Ext
																								.getCmp('bCompany_id')
																								.getValue(),
																				Ext
																						.getCmp('bCompany_name')
																						.getValue(),
																				__cxtPath
																						+ '/crmMaint.do?method=getCompanyInfo&companyID='
																						+ Ext
																								.getCmp('bCompany_id')
																								.getValue()
																						+ '&tab=1&path='
																						+ path,
																				'menu-company-info',
																				true);
															},
															'render' : function(){
																				Ext.getCmp('scheduled_id').setValue(__emp.userID);
																				Ext.getCmp('scheduled_name').setValue(__emp.userName);
															}
														}
													})]
								
								
								
								
								
							},{
								xtype : 'textarea',
								fieldLabel : '备注',
								width : 250,
								name : 'reason'
							},{
								xtype : 'panel',
								layout : 'column',
								name: '',
								border : false,
								width:400,
								items : [{
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.5,
									name : 'last_updated_id',
									border : false,
									labelAlign : 'right',
									items : [{
												xtype : 'hidden',
												id : 'last_updated_id',
												name : 'last_updated_id'
											},{
												fieldLabel : '最后修改人',
												xtype : 'displayfield',
												value : '',
												hidden:true,
												id : 'last_updated_name',
												name:'last_updated_name'
											}]

								},{
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.5,
									name : 'last_updated_datetime',
									border : false,
									labelAlign : 'right',
									items : [{
												fieldLabel : '最后修改时间',
												xtype : 'displayfield',
												hidden:true,
												value : '',
												id : 'last_updated_datetime',
												name: 'last_updated_datetime'
											}]

								}]

					
							
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
					
					
										var begin_date =	dutyPlanPanel.getForm().findField('on_watch_date').getValue();
										var end_date  =	dutyPlanPanel.getForm().findField('end_date').getValue();
										var begin_time =	dutyPlanPanel.getForm().findField('begin_time').getValue();
										var end_time  =	dutyPlanPanel.getForm().findField('end_time').getValue();
										if (!dateCompare(begin_date, end_date)) {
											Ext.Msg.alert('提示', '结束日期必须晚于开始日期！');
											dutyPlanPanel.getForm().findField('end_date').setValue('');
											return;
										}
										if(new Date(Date.parse(begin_date)).getTime() == new Date(Date.parse(end_date)).getTime()){
											if (begin_time >= end_time) {
												Ext.Msg.alert('提示', '同一天的结束时间必须晚于开始日期！');
												dutyPlanPanel.getForm().findField('end_time').setValue('');
												return;
											}
										}
									
					
					dutyPlanPanel.getForm().submit({
						url : __cxtPath
								+ '/orgMaint.do?method=saveDutyPlan',
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
											dutyPlanPanel.getForm().clear();
											Ext.getCmp('scheduled_id').setValue('');
											Ext.getCmp('on_watch_id').setValue('');
											Ext.getCmp('last_updated_name').setValue('');
											Ext.getCmp('last_updated_datetime').setValue('');
											Ext.getCmp('grid').getStore().reload();
						},
						failure : function(form, action) {
							com.bitc.xmis.util.AlertUtil
									.formSubmitFailedAlert(action);
						}
					});
				}
			},'-',{
				xtype : 'button',
				text : '删除',
				iconCls : 'delete',
				ref : '../refreshButton',
				handler : function() {
					Ext.MessageBox.confirm('提示',
								'是否删除', function(btn,
										text) {
									if (btn == 'yes') {
					dutyPlanPanel.getForm().submit({
						url : __cxtPath
								+ '/orgMaint.do?method=deleteDutyPlan',
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
											dutyPlanPanel.getForm().clear();
											Ext.getCmp('scheduled_id').setValue('');
											Ext.getCmp('on_watch_id').setValue('');
											Ext.getCmp('last_updated_id').setValue('');
											Ext.getCmp('last_updated_name').setValue('');
											Ext.getCmp('last_updated_datetime').setValue('');
											dutyPlanPanel.getForm().findField('last_updated_name').hide();
											dutyPlanPanel.getForm().findField('last_updated_datetime').hide();
											Ext.getCmp('grid').getStore().reload();
										
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
					dutyPlanPanel.getForm().clear();
					Ext.getCmp('scheduled_id').setValue('');
					Ext.getCmp('on_watch_id').setValue('');
					Ext.getCmp('last_updated_id').setValue('');
					Ext.getCmp('last_updated_name').setValue('');
					Ext.getCmp('last_updated_datetime').setValue('');
					dutyPlanPanel.getForm().findField('last_updated_name').hide();
					dutyPlanPanel.getForm().findField('last_updated_datetime').hide();
				}
			}]
		}
	});

		var maximumGYS = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						header : '序号',
						width:50
					}),{
						header : 'id',
						dataIndex : 'id',
						sortable : false,
						hidden:true
					},{
						header : 'on_watch_id',
						dataIndex : 'on_watch_id',
						sortable : false,
						hidden:true
					},{
						header : '值班人',
						dataIndex : 'on_watch_name',
						sortable : false
					}, {
						header : '岗位',
						dataIndex : 'job_type_id',
						sortable : false,
						hidden:true
					},{
						header : '部门',
						dataIndex : 'department',
						sortable : false
					},{
						header : '值班开始日期',
						dataIndex : 'on_watch_date',
						sortable : false
					}, {
						header : '开始时间',
						dataIndex : 'begin_time',
						sortable : false
					},{
						header : '值班结束日期',
						dataIndex : 'end_date',
						sortable : false
					}, {
						header : '结束时间',
						dataIndex : 'end_time',
						sortable : false
					},{
						header : '计划制定时间',
						dataIndex : 'scheduled_datetime',
						sortable : false,
						hidden:true
					},{
						header : 'scheduled_id',
						dataIndex : 'scheduled_id',
						sortable : false,
						hidden:true
					}, {
						header : '计划制定人姓名',
						dataIndex : 'scheduled_name',
						sortable : false,
						hidden:true
					},{
						header : 'last_updated_id',
						dataIndex : 'last_updated_id',
						sortable : false,
						hidden:true
					},{
						header : '最后修改人',
						dataIndex : 'last_updated_name',
						sortable : false
					}, {
						header : '最后修改时间',
						dataIndex : 'last_updated_datetime',
						sortable : false
					},{
						header : '备注',
						dataIndex : 'reason',
						sortable : false
					}]);
	var reRecordGYS = Ext.data.Record.create([{
				name : 'id'
			}, {
				name : 'job_type_id'
			}, {
				name : 'on_watch_id'
			},{
				name : 'department'
			},{
				name : 'on_watch_name'
			}, {
				name : 'on_watch_date'
			}, {
				name : 'begin_time'
			},{
				name : 'end_date'
			}, {
				name : 'end_time'
			}, {
				name : 'reason'
			},{
				name: 'scheduled_datetime'
			},{
				name : 'scheduled_id'
			},{
				name : 'scheduled_name'
			},{
				name : 'last_updated_id'
			},{
				name : 'last_updated_name'
			},{
				name : 'last_updated_datetime'
			}]);
	var storeGYS = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/orgMaint.do?method=getDutyPlanList',
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
	var grid = new Ext.grid.GridPanel({
				region : 'center',
				id:'grid',
				border : false,
				width : 980,
				loadMask : true,
				
				collapsible : true,
				autoScroll : true,
				collapseMode : 'mini',
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				stripeRows : true,
				store : storeGYS,
				cm : maximumGYS,
				tbar : ['&nbsp;部门：&nbsp;',new com.bitc.xmis.Combos.DivisionDeptCombo({
							width : 150,
							COMPANYID : 1,
							ref : '../division_combo',
							listeners : {
								'select' : function(combo, record, index) {
									var selValue = record.get('id');
									storeGYS.setBaseParam('department',
											selValue);
//									storeGYS.load({
//												params : {
//													start : 0,
//													limit : 0
//												}
//											});

								}
							}
						}), '&nbsp;值班日期：&nbsp;', new Ext.form.DateField({
							ref : '../startDate',
							format : 'Y-m-d',
							editable:false,
							showToday : true,
							// value : new Date(new Date().getFullYear(), 0, 1),
							anchor : '100%'
						}), {
					text : '条件重置',
					handler : function() {
						grid.division_combo.setValue('-1');
						grid.startDate.setValue('');
					}
				}, '-', {
					ref : '../searchBtn',
					iconCls : 'searchIcon',
					text : '查询',
					handler : function(b, e) {
						var divisionID = grid.division_combo.getValue();
						var begDate = grid.startDate.getValue();
						grid.store.setBaseParam('department', divisionID);
						grid.store.setBaseParam('on_watch_date', begDate);
						
						grid.store.load({
									params : {
										start : 0
									}
								});

					}
				}],
				bbar : new Ext.PagingToolbar({
							pageSize : 20,
							store : storeGYS,
							displayInfo : true,
							displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
							emptyMsg : "无显示数据",
							plugins : [new Ext.ux.PagelimitCombo()]
						}),
			   listeners:{ 
			   	'rowclick' : function(grid,rowIndex,e){
			   					var r = grid.getStore().getAt(rowIndex);
			   					dutyPlanPanel.getForm().findField('id').setValue(r.get('id'));
			   					Ext.getCmp('on_watch_name').setValue(r.get('on_watch_name'));
			   					Ext.getCmp('on_watch_id').setValue(r.get('on_watch_id'));
		   						dutyPlanPanel.getForm().findField('on_watch_date').setValue(r.get('on_watch_date'));
								dutyPlanPanel.getForm().findField('begin_time').setValue(r.get('begin_time'));
								dutyPlanPanel.getForm().findField('end_date').setValue(r.get('end_date'));
								dutyPlanPanel.getForm().findField('end_time').setValue(r.get('end_time'));
								dutyPlanPanel.getForm().findField('scheduled_datetime').setValue(r.get('scheduled_datetime'));
								Ext.getCmp('scheduled_id').setValue(r.get('scheduled_id'));
								Ext.getCmp('scheduled_name').setValue(r.get('scheduled_name'));
								dutyPlanPanel.getForm().findField('reason').setValue(r.get('reason'));
								Ext.getCmp('last_updated_id').setValue(r.get('last_updated_id'));
								Ext.getCmp('last_updated_name').setValue(r.get('last_updated_name'));
								Ext.getCmp('last_updated_datetime').setValue(r.get('last_updated_datetime'));
								dutyPlanPanel.getForm().findField('last_updated_name').show();
								dutyPlanPanel.getForm().findField('last_updated_datetime').show();
			   			     }
			   }
			});
	
	new Ext.Viewport({
				layout : 'border',
				items : [grid,dutyPlanPanel]
			});
});
com.bitc.xmis.Combos.DivisionDeptCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择部门',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
					url : String
							.format(
									'{0}/orgMaint.do?method=getAllDivisionDept&&companyID={1}',
									__cxtPath, this.COMPANYID),
					root : 'rows',
					autoLoad : true,
					fields : [{
								name : "id",
								type : "int",
								mapping : "部门ID"
							}, {
								name : "name",
								type : "string",
								mapping : "部门名称"
							}],
					listeners : {
						scope : this,
						'load' : function() {
							this.store.insert(0, new this.store.recordType({
												id : '-1',
												name : '-全部-'
											}));
							this.setValue(this.value);
						}
					}
				});
		com.bitc.xmis.Combos.DivisionDeptCombo.superclass.initComponent
				.call(this);
	}
});

