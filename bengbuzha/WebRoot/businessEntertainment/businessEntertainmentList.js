Ext.onReady(function() {
	Ext.QuickTips.init();
	var reRecord = Ext.data.Record.create([{
				"mapping" : "form_id",
				"name" : "form_id",
				"type" : "string"
			}, {
				"mapping" : "visitor_company_name",
				"name" : "visitor_company_name",
				"type" : "string"
			}, {
				"mapping" : "visitor_qty",
				"name" : "visitor_qty",
				"type" : "int"
			}, {
				"dateFormat" : "Y-m-d",
				"mapping" : "meals_date",
				"name" : "meals_date",
				"type" : "date"
			}, {
				"mapping" : "meals_type",
				"name" : "meals_type",
				"type" : "int"
			}, {
				"mapping" : "meals_fee_budget",
				"name" : "meals_fee_budget"
			}, {
				"mapping" : "accompany_qty",
				"name" : "accompany_qty",
				"type" : "int"
			}, {
				"mapping" : "cost",
				"name" : "cost"
			}, {
				"mapping" : "profit",
				"name" : "profit"
			}, {
				"mapping" : "cigarettes_wine_amt",
				"name" : "cigarettes_wine_amt"
			}, {
				"mapping" : "total_amt",
				"name" : "total_amt"
			}, {
				"mapping" : "app_datetime",
				"name" : "app_datetime",
				"type" : "string"
			}, {
				"mapping" : "app_user_id",
				"name" : "app_user_id",
				"type" : "int"
			}, {
				"mapping" : "app_user_name",
				"name" : "app_user_name",
				"type" : "string"
			}, {
				"mapping" : "app_deptment",
				"name" : "app_deptment",
				"type" : "int"
			}, {
				"mapping" : "app_deptment_name",
				"name" : "app_deptment_name",
				"type" : "string"
			}, {
				"mapping" : "sign_No",
				"name" : "sign_No",
				"type" : "string"
			}, {
				"mapping" : "remark",
				"name" : "remark",
				"type" : "string"
			}, {
				"mapping" : "is_worked",
				"name" : "is_worked",
				"type" : "int"
			}, {
				"mapping" : "proid",
				"name" : "proid",
				"type" : "string"
			}]);
	var gridstore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/applyManageMaint.do?method=getBusinessEntertainmentFormList',
			method : "POST"
		}),
		baseParams : {
			limit : 20
		},
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, reRecord),
		sortInfo : {
			field : 'app_datetime',
			direction : 'DESC'
		},
		autoLoad : true,
		remoteSort : true
	});

	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(), {
				header : "签办号",
				sortable : true,
				width : 90,
				dataIndex : "sign_No"
			}, {
				header : "申请部门",
				sortable : true,
				width : 90,
				dataIndex : "app_deptment_name"
			}, {
				header : "申请日期",
				sortable : true,
				width : 90,
				dataIndex : "app_datetime"
			}, {
				header : "来客单位",
				sortable : true,
				width : 180,
				dataIndex : "visitor_company_name"
			}, {
				header : "来客人数",
				sortable : true,
				width : 90,
				dataIndex : "visitor_qty"
			}, {
				header : "标准",
				align : 'right',
				sortable : true,
				width : 90,
				dataIndex : "meals_fee_budget"
			}, {
				header : "实际金额",
				align : 'right',
				sortable : true,
				width : 90,
				dataIndex : "total_amt"
			}, {
				xtype : 'actioncolumn',
				width : 40,
				header : '详情',
				items : [{
					iconCls : 'edit',
					text : '详情',
					handler : function(grid, rowIndex, colIndex) {
						var form_id_param = grid.store.getAt(rowIndex)
								.get('form_id');
					    var proid = grid.store.getAt(rowIndex)
								.get('proid');
						var url = __cxtPath
								+ '/businessEntertainment/applyBusinessEntertainmentForm.jsp?form_id='
								+ form_id_param+'&processInstanceId='+proid+'&todoId=query';
						parent.xMis.turn('wf-openform-', '业务招待详情', url, 't',
								true);
					}
				}]
			}]);
	var grid = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : gridstore,
		cm : cm,
		tbar : ['部门选择：',new com.bitc.xmis.Combos.DivisionDeptCombo({
							width : 150,
							COMPANYID : 1,
							ref : '../division_combo',
							listeners : {
								'select' : function(combo, record, index) {
									var selValue = record.get('id');
									gridstore.setBaseParam('divisionID',
											selValue);
//									gridstore.load({
//												params : {
//													start : 0,
//													limit : 0
//												}
//											});

								}
							}
						}), '申请日期：&nbsp;', new Ext.form.DateField({
							ref : '../startDate',
							format : 'Y-m-d',
							minValue : '2013-01-01',
							showToday : true,
							// value : new Date(new Date().getFullYear(), 0, 1),
							anchor : '100%'
						}), '&nbsp;—&nbsp;', new Ext.form.DateField({
							ref : '../endDate',
							format : 'Y-m-d',
							minValue : '2013-01-01',
							showToday : true,
							// value : new Date(),
							anchor : '100%'
						}), {
					text : '条件重置',
					handler : function() {
						grid.division_combo.setValue('-1');
						grid.startDate.setValue('');
						grid.endDate.setValue('');

					}
				}, '-', {
					ref : '../searchBtn',
					iconCls : 'searchIcon',
					text : '查询',
					handler : function(b, e) {
						var divisionID = grid.division_combo.getValue();
						var begDate = grid.startDate.getValue();
						var endDate = grid.endDate.getValue();
						grid.store.setBaseParam('divisionID', divisionID);

						if (begDate == '' && endDate == '') {
							grid.store.setBaseParam('begDate', begDate);
							grid.store.setBaseParam('endDate', endDate);
						} else if (begDate != '' && endDate == '') {
							Ext.Msg.alert('提示', '开始日期已填写，结束日期不可为空！');
							return;
						} else if (begDate == '' && endDate != '') {
							Ext.Msg.alert('提示', '结束日期已填写，开始日期不可为空！');
							return;
						} else {
							if (!dateCompare(begDate, endDate)) {
								Ext.Msg.alert('提示', '结束日期必须晚于开始日期！');
								return;
							} else {

								grid.store.setBaseParam('begDate', begDate);
								grid.store.setBaseParam('endDate', endDate);
							}
						}
						grid.store.load({
									params : {
										start : 0
									}
								});

					}
				}],
		bbar : new Ext.PagingToolbar({
			pageSize : 20,
			store : gridstore,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : '无显示数据',
			plugins : [new Ext.ux.Export({
				exportname : '业务招待费查询',
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.RowNumber'
						}, {
							columnHeader : '申请日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}]
			})// , new Ext.ux.PagelimitCombo()
			]
		})

	});
	// grid.on('rowclick', function(gridpanel, index, e) {
	// var r = gridpanel.getStore().getAt(index);
	// var processinstanceid = r.get('processinstanceid');
	// var app_user_name = r.get('app_user_name');
	// var app_datetime = r.get('app_datetime');
	// com.bitc.xmis.workflow.showTraceWin(processinstanceid);
	// });
	var main = new Ext.Viewport({
				layout : 'border',
				items : grid,
				border : false
			});
});
