Ext.onReady(function() {
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	var infoPanel = new Ext.form.FormPanel({
		autoScroll : true,
		region : 'east',
		split : true,
		width : 400,
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

					}, {
						xtype : 'numberfield',
						fieldLabel : '闸上水位',
						width : 250,
						name : 'ap_w_level',
						//regex: /^\d{1,4}(\.\d{1,2})?$/,
		                //regexText: '请输入正确的数据类型'
						validText: '请输入正确的数据类型'
					}, {
						xtype : 'numberfield',
						fieldLabel : '闸下水位',
						width : 250,
						name : 'bp_w_level',
						regex: /^\d{1,4}(\.\d{1,2})?$/,
		                regexText: '请输入正确的数据类型'
					}, {
						xtype : 'numberfield',
						fieldLabel : '28孔闸开闸数量',
						width : 250,
						name : 'gate_open_num_1'
					}, {
						xtype : 'numberfield',
						fieldLabel : '28空闸闸开度',
						width : 250,
						name : 'gate_open_degree_1'
					}, {
						xtype : 'numberfield',
						fieldLabel : '12孔闸开闸数量',
						width : 250,
						name : 'gate_open_num_2'
					}, {
						xtype : 'numberfield',
						fieldLabel : '12空闸闸开度',
						width : 250,
						name : 'gate_open_degree_2'
					}, {
						xtype : 'numberfield',
						fieldLabel : '电机',
						width : 250,
						name : 'elec_machine'
					}, {
						xtype : 'numberfield',
						fieldLabel : '流量',
						width : 250,
						name : 'flow_qty'
					}, {
						xtype : 'textfield',
						fieldLabel : '水位差',
						width : 250,
						name : 'w_level_chan',
						regex: /^\d{1,3}(\.\d{1,2})?$/,
		                regexText: '请输入正确的数据类型'
					}, {
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
								+ '/applyManageMaint.do?method=doSaveGczlInfo',
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
										+ '/applyManageMaint.do?method=doDeleteGczlInfoById',
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
	
	var gate_open_num_1 = infoPanel.getForm().findField("gate_open_num_1");
	var gate_open_degree_1 = infoPanel.getForm().findField("gate_open_degree_1");
	var gate_open_num_2 = infoPanel.getForm().findField("gate_open_num_2");
	var gate_open_degree_2 = infoPanel.getForm().findField("gate_open_degree_2");
	var elec_machine = infoPanel.getForm().findField("elec_machine");
	
	
	var flow_qty = infoPanel.getForm().findField("flow_qty");
	var w = infoPanel.getForm().findField("w_level_chan");
	function formAmt(value) {
		return Ext.util.Format.number(value, '000.00');
	}
	ap.on('blur', function() {
				if (ap.getValue() != '' && bp.getValue() != '') {
					if (bp.getValue() > ap.getValue()) {
						Ext.Msg.alert('', '闸下水位不能大于闸上水位！');
						bp.setValue('');
						return;
					}
					w.setValue(formAmt(ap.getValue() - bp.getValue()));
				}

				if ((ap.getValue() != '' && bp.getValue() != '')&&((gate_open_num_1.getValue() !='' && gate_open_degree_1.getValue() !='')||(gate_open_num_2.getValue() !='' && gate_open_degree_2.getValue() !=''))) {
					var elec_machineValue = elec_machine.getValue();
					if(elec_machineValue == ''){
						elec_machineValue = 0;
					}
					Ext.Ajax.request({
						url : __cxtPath
						+ '/gczlAction.do?method=getLiuLiang',
						success:function(response){
							var result = Ext.decode(response.responseText);
							if (result.success) {
								var liuliang = result.attributes.liuliang;
								flow_qty.setValue(liuliang);
							}
						},
						failure:function(){},
						params:{
							gate_open_num_1:gate_open_num_1.getValue(),
							gate_open_degree_1:gate_open_degree_1.getValue(),
							gate_open_num_2:gate_open_num_2.getValue(),
							gate_open_degree_2:gate_open_degree_2.getValue(),
							ap:ap.getValue(),
							bp:bp.getValue(),
							elec_machine:elec_machineValue
						}
					});
				}
			
			}); 
	bp.on('blur', function() {
				if (ap.getValue() != '' && bp.getValue() != '') {
					if (bp.getValue() > ap.getValue()) {
						Ext.Msg.alert('', '闸下水位不能大于闸上水位！');
						bp.setValue('');
						return;
					}
					w.setValue(formAmt(ap.getValue() - bp.getValue()));
				}

				if ((ap.getValue() != '' && bp.getValue() != '')&&((gate_open_num_1.getValue() !='' && gate_open_degree_1.getValue() !='')||(gate_open_num_2.getValue() !='' && gate_open_degree_2.getValue() !=''))) {
					var elec_machineValue = elec_machine.getValue();
					if(elec_machineValue == ''){
						elec_machineValue = 0;
					}
					Ext.Ajax.request({
						url : __cxtPath
						+ '/gczlAction.do?method=getLiuLiang',
						success:function(response){
							var result = Ext.decode(response.responseText);
							if (result.success) {
								var liuliang = result.attributes.liuliang;
								flow_qty.setValue(liuliang);
							}
						},
						failure:function(){},
						params:{
							gate_open_num_1:gate_open_num_1.getValue(),
							gate_open_degree_1:gate_open_degree_1.getValue(),
							gate_open_num_2:gate_open_num_2.getValue(),
							gate_open_degree_2:gate_open_degree_2.getValue(),
							ap:ap.getValue(),
							bp:bp.getValue(),
							elec_machine:elec_machineValue
						}
					});
				}
			
			});
	
	
	gate_open_num_1 .on('blur', function() {
		if ((ap.getValue() != '' && bp.getValue() != '')&&((gate_open_num_1.getValue() !='' && gate_open_degree_1.getValue() !='')||(gate_open_num_2.getValue() !='' && gate_open_degree_2.getValue() !=''))) {
			var elec_machineValue = elec_machine.getValue();
			if(elec_machineValue == ''){
				elec_machineValue = 0;
			}
				
			Ext.Ajax.request({
				url : __cxtPath
				+ '/gczlAction.do?method=getLiuLiang',
				success:function(response){
					var result = Ext.decode(response.responseText);
					if (result.success) {
						var liuliang = result.attributes.liuliang;
						flow_qty.setValue(liuliang);
					}
				},
				failure:function(){},
				params:{
					gate_open_num_1:gate_open_num_1.getValue(),
					gate_open_degree_1:gate_open_degree_1.getValue(),
					gate_open_num_2:gate_open_num_2.getValue(),
					gate_open_degree_2:gate_open_degree_2.getValue(),
					ap:ap.getValue(),
					bp:bp.getValue(),
					elec_machine:elec_machineValue
				}
			});
			
		}
	});
	gate_open_degree_1.on('blur', function() {
		if ((ap.getValue() != '' && bp.getValue() != '')&&((gate_open_num_1.getValue() !='' && gate_open_degree_1.getValue() !='')||(gate_open_num_2.getValue() !='' && gate_open_degree_2.getValue() !=''))) {
			var elec_machineValue = elec_machine.getValue();
			if(elec_machineValue == ''){
				elec_machineValue = 0;
			}
			Ext.Ajax.request({
				url : __cxtPath
				+ '/gczlAction.do?method=getLiuLiang',
				success:function(response){
					var result = Ext.decode(response.responseText);
					if (result.success) {
						var liuliang = result.attributes.liuliang;
						flow_qty.setValue(liuliang);
					}
				},
				failure:function(){},
				params:{
					gate_open_num_1:gate_open_num_1.getValue(),
					gate_open_degree_1:gate_open_degree_1.getValue(),
					gate_open_num_2:gate_open_num_2.getValue(),
					gate_open_degree_2:gate_open_degree_2.getValue(),
					ap:ap.getValue(),
					bp:bp.getValue(),
					elec_machine:elec_machineValue
				}
			});
		}
	});
	gate_open_num_2 .on('blur', function() {
		if ((ap.getValue() != '' && bp.getValue() != '')&&((gate_open_num_1.getValue() !='' && gate_open_degree_1.getValue() !='')||(gate_open_num_2.getValue() !='' && gate_open_degree_2.getValue() !=''))) {
			var elec_machineValue = elec_machine.getValue();
			if(elec_machineValue == ''){
				elec_machineValue = 0;
			}
			Ext.Ajax.request({
				url : __cxtPath
				+ '/gczlAction.do?method=getLiuLiang',
				success:function(response){
					var result = Ext.decode(response.responseText);
					if (result.success) {
						var liuliang = result.attributes.liuliang;
						flow_qty.setValue(liuliang);
					}
				},
				failure:function(){},
				params:{
					gate_open_num_1:gate_open_num_1.getValue(),
					gate_open_degree_1:gate_open_degree_1.getValue(),
					gate_open_num_2:gate_open_num_2.getValue(),
					gate_open_degree_2:gate_open_degree_2.getValue(),
					ap:ap.getValue(),
					bp:bp.getValue(),
					elec_machine:elec_machineValue
				}
			});
		}
	});
	gate_open_degree_2 .on('blur', function() {
		if ((ap.getValue() != '' && bp.getValue() != '')&&((gate_open_num_1.getValue() !='' && gate_open_degree_1.getValue() !='')||(gate_open_num_2.getValue() !='' && gate_open_degree_2.getValue() !=''))) {
			var elec_machineValue = elec_machine.getValue();
			if(elec_machineValue == ''){
				elec_machineValue = 0;
			}
			Ext.Ajax.request({
				url : __cxtPath
				+ '/gczlAction.do?method=getLiuLiang',
				success:function(response){
					var result = Ext.decode(response.responseText);
					if (result.success) {
						var liuliang = result.attributes.liuliang;
						flow_qty.setValue(liuliang);
					}
				},
				failure:function(){},
				params:{
					gate_open_num_1:gate_open_num_1.getValue(),
					gate_open_degree_1:gate_open_degree_1.getValue(),
					gate_open_num_2:gate_open_num_2.getValue(),
					gate_open_degree_2:gate_open_degree_2.getValue(),
					ap:ap.getValue(),
					bp:bp.getValue(),
					elec_machine:elec_machineValue
				}
			});
		}
	});
	
	elec_machine .on('blur', function() {
		if ((ap.getValue() != '' && bp.getValue() != '')&&((gate_open_num_1.getValue() !='' && gate_open_degree_1.getValue() !='')||(gate_open_num_2.getValue() !='' && gate_open_degree_2.getValue() !=''))) {
			var elec_machineValue = elec_machine.getValue();
			if(elec_machineValue == ''){
				elec_machineValue = 0;
			}
			Ext.Ajax.request({
				url : __cxtPath
				+ '/gczlAction.do?method=getLiuLiang',
				success:function(response){
					var result = Ext.decode(response.responseText);
					if (result.success) {
						var liuliang = result.attributes.liuliang;
						flow_qty.setValue(liuliang);
					}
				},
				failure:function(){},
				params:{
					gate_open_num_1:gate_open_num_1.getValue(),
					gate_open_degree_1:gate_open_degree_1.getValue(),
					gate_open_num_2:gate_open_num_2.getValue(),
					gate_open_degree_2:gate_open_degree_2.getValue(),
					ap:ap.getValue(),
					bp:bp.getValue(),
					elec_machine:elec_machineValue
				}
			});
		}
	});
	var maximumGYS = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
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
				header : '闸上水位',
				align:'right',
				dataIndex : 'ap_w_level',
				sortable : true
			}, {
				header : '闸下水位',
				align:'right',
				dataIndex : 'bp_w_level',
				sortable : true

			}, {
				header : '28空闸开闸数量',
				align:'right',
				dataIndex : 'gate_open_num_1',
				sortable : true
			}, {
				header : '28空闸闸开度',
				align:'right',
				dataIndex : 'gate_open_degree_1',
				sortable : true

			}, {
				header : '12空闸开闸数量',
				align:'right',
				dataIndex : 'gate_open_num_2',
				sortable : true
			}, {
				header : '12空闸闸开度',
				align:'right',
				dataIndex : 'gate_open_degree_2',
				sortable : true

			}, {
				header : '电机',
				align:'right',
				dataIndex : 'elec_machine',
				sortable : true
			}, {
				header : '流量',
				align:'right',
				dataIndex : 'flow_qty',
				sortable : true
			}, {
				header : '水位差',
				align:'right',
				dataIndex : 'w_level_chan',
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
	var reRecordGYS = Ext.data.Record.create([{
				name : 'tab_seed'
			}, {
				name : 'unit_code'
			}, {
				name : 'measure_date'
			}, {
				name : 'measure_time'
			}, {
				name : 'ap_w_level'
			}, {
				name : 'bp_w_level'
			}, {
				name : 'gate_open_num_1'
			}, {
				name : 'gate_open_degree_1'
			}, {
				name : 'gate_open_num_2'
			}, {
				name : 'gate_open_degree_2'
			}, {
				name : 'elec_machine'
			}, {
				name : 'flow_qty'
			}, {
				name : 'w_level_chan'
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
					+ '/applyManageMaint.do?method=getGczlInfoListWithGroupQuery',
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
		autoLoad : true,
		sortInfo : {
					field : 'measure_date',
					direction : 'DESC'
				}
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
				cm : maximumGYS,
				sm : new Ext.grid.RowSelectionModel({
							singleSelect : true,
							listeners : {
								rowselect : function(sm, row, rec) {
									infoPanel.getForm().loadRecord(rec);
								}
							}
						}),
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
				items : [grid, infoPanel]
			});
});
