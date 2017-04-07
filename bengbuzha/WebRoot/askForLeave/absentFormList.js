Ext.onReady(function() {
	Ext.QuickTips.init();
	var exportname = '缺勤列表';

	var reRecord = Ext.data.Record.create([{
				"mapping" : "form_id",
				"name" : "form_id",
				"type" : "string"
			},{
				"mapping" : "proId",
				"name" : "proId",
				"type" : "string"
			}, {
				"mapping" : "absent_type_id",
				"name" : "absent_type_id",
				"type" : "int"
			}, {
				"mapping" : "actual_begin_date",
				"name" : "actual_begin_date",
				"type" : "string"
			}, {
				"mapping" : "actual_end_date",
				"name" : "actual_end_date",
				"type" : "string"
			}, {
				"mapping" : "leavetime",
				"name" : "leavetime",
				"type" : "string"
			}, {
				"mapping" : "reason",
				"name" : "reason",
				"type" : "string"
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
				"mapping" : "diffstr",
				"name" : "diffstr",
				"type" : "string"
			}]);
	var store = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/applyManageMaint.do?method=getAbsentFormList',
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
				remoteSort : true
			});
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(), {
				header : "form_id",
				hidden : true,
				width : 90,
				dataIndex : "form_id"
			},{
				header : "proId",
				hidden : true,
				width : 90,
				dataIndex : "porId"
			}, {
				header : "app_user_id",
				hidden : true,
				width : 180,
				dataIndex : "app_user_id"
			}, {
				header : "申请人",
				width : 90,
				dataIndex : "app_user_name"
			}, {
				header : "请假类别",
				width : 90,
				dataIndex : "absent_type_id",
				renderer : function(v) {
					if (v == 1) {
						return '带薪年休假';
					} else if (v == 2) {
						return '探亲假'
					}else if (v == 3) {
						return '病假'
					}else if (v == 4) {
						return '事假'
					}else if (v == 5) {
						return '婚假'
					}else if (v == 6) {
						return '丧假'
					}else if (v == 7) {
						return '计划生育、优生优育假'
					}else if (v == 8) {
						return '公派'
					}
				}

			}, {
				header : "请假时间",
				width : 180,
				dataIndex : "app_datetime"
			}, {
				header : "销假时间",
				width : 180,
				dataIndex : "leavetime"
			}, {
				header : "请假天数",
				width : 120,
				dataIndex : "diffstr"
			}, {
				header : "事由",
				sortable : true,
				width : 90,
				dataIndex : "reason"
			}, {
				header : "app_datetime",
				hidden : true,
				width : 180,
				dataIndex : "app_datetime"
			}, {
				header : "leavetime",
				hidden : true,
				width : 180,
				dataIndex : "leavetime"
			}, {
				xtype : 'actioncolumn',
				width : 40,
				header : '详情',
				align : 'center',
				items : [{
					getClass : function(v, meta, rec) {
						return 'edit';
					},
					handler : function(grid, rowIndex, colIndex) {
						var rec = grid.getStore().getAt(rowIndex);
						var form_id = rec.get('form_id');
						var proid = rec.get('proId');
						var url = __cxtPath
								+ '/askForLeave/askForLeave.jsp?form_id='
								+ form_id+'&processInstanceId='+proid+'&todoId=query';
						parent.xMis.turn('wf-openform-', '请假申请', url, 't',
								true);
					}
				}]
			},{
				xtype : 'actioncolumn',
				hidden:true,
				width : 40,
				header : '附件列表',
				align : 'center',
				items : [{
					getClass : function(v, meta, rec) {
						return 'edit';
					},
					handler : function(grid, rowIndex, colIndex) {
						var rec = grid.getStore().getAt(rowIndex);
						var form_id = rec.get('form_id');
						com.bbz.AttachListWindow(form_id);
					}
				}]
			}]);
	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.PagingRowNumber'
						}]
			});
	var gridPanel = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : store,
		cm : cm,
		tbar : {
			xtype : 'toolbar',
			items : [{
						xtype : 'button',
						text : '查询所有',
						ref : '../refreshButton',
						handler : function() {
							location.reload();
						}
					}, '-', '条件查询：', new Ext.ux.form.SearchField({
								width : 180,
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
									title : '条件查询',
									text : '输入查询申请人姓名'
								},
								onTrigger1Click : function() {

								},
								onTrigger2Click : function() {
									var inputName = this.el.dom.value;
									if (inputName != null
											&& '' != inputName.trim()) {
										store.setBaseParam('queryStr',
												inputName);
										store.reload({
													params : {
														start : 0
													}
												});
									} else {
										Ext.MessageBox.alert("提示",
												"请输入要查询的内容后进行查询");
									}
								}
							})]
		},
		bbar : new Ext.PagingToolbar({
					pageSize : 20,
					store : store,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					plugins : [new Ext.ux.PagelimitCombo()]
				})

	});

	store.load({
				params : {
					start : 0,
					limit : 20
				}
			});
	// main
	new Ext.Viewport({
				layout : 'border',
				items : [gridPanel]
			});

});
