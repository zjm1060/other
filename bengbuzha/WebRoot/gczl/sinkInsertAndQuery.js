Ext.onReady(function() {

	Ext.QuickTips.init();
	var isInsert = request.getParameter('isInsert');
	Ext.form.Field.prototype.msgTarget = 'under';
	var infoPanel = new Ext.form.FormPanel({
		autoScroll : true,
		split : true,
		region : 'east',
		width : 450,
		collapsible : true,
		bodyStyle : 'padding:30px 5px 0px 5px',
		labelAlign : 'right',
		labelWidth : 100,
		items : [{
			layout : 'form',
			xtype : 'panel',
			autoScroll : true,
			border : false,
			width : 400,
			collapseMode : 'mini',
			defaultType : 'textfield',
			items : [{
						xtype : 'hidden',
						fieldLabel : 'tab_seed',
						name : 'tab_seed'
					}, {
						xtype : 'hidden',
						fieldLabel : 'unit_code',
						name : 'unit_code'
					}, {

						xtype : 'datefield',
						fieldLabel : '测量日期',
						width : 250,
						format : 'Y-m-d',
						name : 'measure_date',
						allowBlank : false,
						editable : false
					}, {
						xtype : 'timefield',
						name : 'measure_time',
						fieldLabel : '时间',
						format : 'H:i:s',
						allowBlank : false,
						increment : 30,
						width : 250,
						editable : false

					},{
						xtype : 'combo',
						allowBlank : false,
						fieldLabel : '部位名称',
						id : 'sp_code',
						hiddenName : 'sp_code',
						value : '01',
						valueField : 'sp_code',
						displayField : 'sp_name',
						mode : 'remote',
						triggerAction : 'all',
						editable : false,
						store : new Ext.data.Store({
							proxy : new Ext.data.HttpProxy({
								url : __cxtPath
										+ '/gczlAction.do?method=getSpCodeList'
							}),
							autoLoad : true,
							reader : new Ext.data.JsonReader({
										root : 'rows',
										fields : [{
													name : 'sp_code',
													mapping : 'sp_code'
												}, {
													name : 'sp_name',
													mapping : 'sp_name'
												}]
									}),
							listeners : {								
								// 解决在Form中setValue时显示的是valueField而不是displayField的问题
								'load' : function() {
									Ext.getCmp('sp_code').setValue(Ext.getCmp('sp_code').value);
								}
							}
						})
					}, {
						xtype : 'combo',
						allowBlank : false,
						fieldLabel : '沉陷点',
						value : '001',
						id:'s_point_code',
						hiddenName : 's_point_code',
						valueField : 's_point_code',
						displayField : 's_point_name',
						mode : 'remote',
						triggerAction : 'all',
						editable : false,
						store : new Ext.data.Store({
							proxy : new Ext.data.HttpProxy({
								url : __cxtPath
										+ '/gczlAction.do?method=getS_point_codeList'
							}),
							baseParams : {
								sp_code : '01'
							},
							autoLoad : true,
							reader : new Ext.data.JsonReader({
										root : 'rows',
										fields : [{
											name : 's_point_code',
											mapping : 's_point_code'
										},{
													name : 's_point_name',
													mapping : 's_point_name'
												}]
									}),
									listeners : {								
										// 解决在Form中setValue时显示的是valueField而不是displayField的问题
										'load' : function() {
											Ext.getCmp('s_point_code').setValue(Ext.getCmp('s_point_code').value);
										}
									}

						})
					}, {
						xtype : 'textfield',
						fieldLabel : '高程',
						width : 250,
						name : 'high_degree',
						regex: /^\d{1,4}(\.\d{1,5})?$/,
		                regexText: '请输入正确的数据类型'
					},{
						xtype : 'textfield',
						fieldLabel : '闸上水位',
						width : 250,
						name : 'ap_w_level',
						regex: /^\d{1,4}(\.\d{1,2})?$/,
		                regexText: '请输入正确的数据类型'
					}, {
						xtype : 'textfield',
						fieldLabel : '闸下水位',
						width : 250,
						name : 'bp_w_level',
						regex: /^\d{1,4}(\.\d{1,2})?$/,
		                regexText: '请输入正确的数据类型'
					},{
						xtype : 'textarea',
						fieldLabel : '备注',
						width : 250,
						name : 'memo'
					}, {

						xtype : 'panel',
						layout : 'form',
						allowBlank : false,
						labelWidth : 100,
						border : false,
						labelAlign : 'right',
						items : [{
									xtype : 'hidden',
									id : 'measure_man_id',
									name : 'measure_man_id'
								}, new Ext.ux.form.ChooseOrgTreeField({
									fieldLabel : '测量人',
									name : 'measure_man',
									id : 'measure_man',
									width : 250,
									winConfig : {
										title : '测量人设置'
									},
									singleSelect : true,
									onlyPerson : true,
									expandNodePath : function() {
										var path = '/comp-old-1';
										var deptid = 0;
										if (Ext.getCmp('bDept_id') != null
												&& (Ext.getCmp('bDept_id')
														.getValue()).trim() != '') {
											path = path
													+ '/dept-old-'
													+ Ext.getCmp('bDept_id')
															.getValue();
											deptid = Ext.getCmp('bDept_id')
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
									setValueFun : function(result) {
										var empName = '';
										var empID = '';
										for (var i = 0; i < result.length; i++) {
											if (i == result.length - 1) {
												empID = empID + result[i].id;
												empName = empName
														+ result[i].text;
											} else {
												empID = empID + result[i].id
														+ ',';
												empName = empName
														+ result[i].text + ',';
											}
										}
										Ext.getCmp('measure_man_id')
												.setValue(empID.split('-')[2]);
										Ext.getCmp('measure_man')
												.setValue(empName);
									},
									clearValueList : 'measure_man_id,measure_man',
									listeners : {
										'dblclick' : function(select, num) {
											var empId = Ext
													.getCmp('measure_man_id')
													.getValue();
											com.bitc.xmis.util.EmpInfoWin(
													empId, '测量人信息');

										},
										'nodedblclick' : function(node, e) {
											var path = String.format(
													'/root{0}', node.getPath());
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
										'render' : function() {
											Ext.getCmp('measure_man_id')
													.setValue(__emp.userID);
											Ext.getCmp('measure_man')
													.setValue(__emp.userName);
										}
									}
								})]

					}]
		}],
		tbar : {
			xtype : 'toolbar',
			items : [{
				xtype : 'button',
				text : '保存',
				iconCls : 'ok',
				handler : function() {

					var measure_date = Ext.util.Format.date(infoPanel.getForm()
									.findField('measure_date').getValue(),
							'Y-m-d');
					var measure_time = infoPanel.getForm()
							.findField('measure_time').getValue();
					infoPanel.getForm().submit({
						url : __cxtPath
								+ '/gczlAction.do?method=doSaveSinkInfo',
						params : {
							measure_date : measure_date + ' ' + measure_time,
							measure_man : infoPanel.getForm()
									.findField('measure_man_id').getValue()
						},
						waitMsg : '正在保存，请稍候……',
						waitTitle : '正在处理',
						success : function(form, action) {
							Ext.MessageBox.alert(action.result.messageTitle,
									action.result.message);
							infoPanel.getForm().clear();

							grid.getStore().reload();
						},
						failure : function(form, action) {
							com.bitc.xmis.util.AlertUtil
									.formSubmitFailedAlert(action);
						}
					});

					// Ext.getCmp('mptp_code').setRawValue('aa');
					// alert(Ext.getCmp('mptp_code').getValue());
				}
			}, '-', {
				xtype : 'button',
				text : '删除',
				iconCls : 'delete',
				ref : '../refreshButton',
				handler : function() {
					Ext.MessageBox.confirm('提示', '是否删除', function(btn, text) {
						if (btn == 'yes') {
							infoPanel.getForm().submit({
								url : __cxtPath
										+ '/gczlAction.do?method=doDeleteSinkInfoById',
								params : {
									tab_seed : infoPanel.getForm()
											.findField('tab_seed').getValue()

								},
								waitMsg : '正在删除，请稍候……',
								waitTitle : '正在处理',
								success : function(form, action) {
									Ext.MessageBox.alert(
											action.result.messageTitle,
											action.result.message);
									infoPanel.getForm().clear();

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
			}, '-', {
				xtype : 'button',
				text : '新建',
				iconCls : 'add',
				ref : '../AddBit',
				handler : function() {
					infoPanel.getForm().reset();

				}
			}]
		}
	});
	var ap = infoPanel.getForm().findField("ap_w_level");
	var bp = infoPanel.getForm().findField("bp_w_level");
	bp.on('blur', function() {
				if (ap.getValue() != null) {
					if (bp.getValue() > ap.getValue()) {
						Ext.Msg.alert('', '闸下水位不能大于闸上水位！');
						bp.setValue('');
						return;
					}
				}
			});
	ap.on('blur', function() {
				if (ap.getValue() != null) {
					if (bp.getValue() > ap.getValue()) {
						Ext.Msg.alert('', '闸下水位不能大于闸上水位！');
						bp.setValue('');
						return;
					}
				}
			});
	var sp_code = infoPanel.getForm().findField("sp_code");
	var s_point_code = infoPanel.getForm().findField("s_point_code");
	sp_code.on('select', function() {
		s_point_code.setValue('');
		s_point_code.store.setBaseParam('sp_code',sp_code.getValue());
		s_point_code.store.load();
	
	});
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						width : 50
					}), {
				header : '序号',
				dataIndex : 'tab_seed',
				sortable : true,
				hidden : true
			}, {
				header : '单位编号',
				dataIndex : 'unit_code',
				sortable : true,
				hidden : true
			}, {
				header : '测量日期',
				dataIndex : 'measure_date',
				sortable : true
			}, {
				header : '测量时间',
				dataIndex : 'measure_time',
				sortable : true
			}, {
				hidden : true,
				header : '部位ID',
				dataIndex : 'sp_code'
			}, {
				header : '部位名称',
				dataIndex : 'sp_name'
			}, {
				header : '沉陷点ID',
				dataIndex : 's_point_code',
				hidden : true
			},{
				header : '沉陷点',
				dataIndex : 's_point_name'
			}, {
				header : '高程',
				dataIndex : 'high_degree',
				sortable : true
			}, {
				header : '闸上水位',
				dataIndex : 'ap_w_level',
				sortable : true
			}, {
				header : '闸下水位',
				dataIndex : 'bp_w_level',
				sortable : true

			}, {
				header : '测量人ID',
				width : 80,
				dataIndex : 'measure_man_id',
				sortable : true,
				hidden : true
			}, {
				header : '测量人',
				width : 80,
				dataIndex : 'measure_man',
				sortable : true
			}, {
				header : '备注',
				width : 120,
				dataIndex : 'memo',
				sortable : true
			}]);
	var record = Ext.data.Record.create([{
				name : 'tab_seed'
			}, {
				name : 'unit_code'
			}, {
				name : 'measure_date'
			}, {
				name : 'sp_code'
			}, {
				name : 'sp_name'
			}, {
				name : 's_point_code'
			},{
				name : 's_point_name'
			}, {
				name : 'measure_time'
			}, {
				name : 'high_degree'
			}, {
				name : 'ap_w_level'
			}, {
				name : 'bp_w_level'
			}, {
				name : 'memo'
			}, {
				name : 'measure_man_id'
			}, {
				name : 'measure_man'
			}]);
	var storeGYS = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/gczlAction.do?method=getSinkInfoListWithGroupQuery',
			method : "POST"
		}),
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, record),
		remoteSort : true,
		baseParams : {
			start : 0,
			limit : 20
		},
		autoLoad : true,
		sortInfo : {
			field : 'measure_date',
			direction : 'DESC'
		}
	});
	var exp = new Ext.ux.Export({
				formater : []
			});
	var grid = new Ext.grid.GridPanel({
		region : 'center',
		id : 'grid',
		border : false,
		width : 980,
		loadMask : true,
		autoScroll : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		tbar : {
			xtype : 'toolbar',
			items : ['模糊查询：', new Ext.ux.InklingSearchField({
								width : 120,
								store : storeGYS,
								emptyText : '请输入测量日期',
								tooltip : {
									title : '模糊查询',
									text : '您可以输入"测量日期"进行查询'
								},
								inklings : [{
											name : 'measure_date',
											op : 'like',
											type : 'string'
										}]
							}), '-', {
						xtype : 'button',
						text : '组合查询',
						iconCls : 'ok',
						handler : function() {
							com.bitc.xmis.SearchWinShow(storeGYS);

						}
					}]
		},
		stripeRows : true,
		store : storeGYS,
		cm : cm,
		sm : new Ext.grid.RowSelectionModel({
			singleSelect : true,
			listeners : {
				rowselect : function(sm, row, rec) {
					infoPanel.getForm().loadRecord(rec);
					s_point_code.store.reload({
						params : {
							sp_code : rec.get('sp_code')
						}
					});
				}
			}
		}),
		bbar : new Ext.PagingToolbar({
					pageSize : 20,
					store : storeGYS,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					plugins : [new Ext.ux.PagelimitCombo()],
					items : [new Ext.Button({
								text : this.text || '导出Excel',
								iconCls : 'xls',
								menu : {
									items : [{
												text : '导出本页数据',
												handler : function() {
													exp.onExport(grid, '沉陷资料',
															true);
												}
											}, {
												text : '导出所有数据',
												handler : function() {
											Ext.Msg
													.confirm(
															'确认',
															'导出所有数据可能时间较长，请谨慎选择，是否继续?',
															function(buttonId) {
																if (buttonId == 'yes') {
																exp.onExport(
																		grid,
																		'沉陷资料',
																		false);
																}
															},this);
										}
											}]
								}
							})]
				})
	});
	if (isInsert == 1) {
		infoPanel.hide();
	}
	new Ext.Viewport({
				layout : 'border',
				items : [grid, infoPanel]
			});
});