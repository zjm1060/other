Ext.onReady(function() {
	Ext.QuickTips.init();
	var exportname = '审批列表';
	var PRIORITY_ARRAY = ['普通', '紧急'];
	var STATUS_ARRAY = ['已申请', '已处理'];

	var reRecord = Ext.data.Record.create([{
				"name" : "form_id",
				"type" : "string"
			}, {
				"name" : "app_user_id",
				"type" : "int"
			}, {
				"name" : "app_user_name",
				"type" : "string"
			}, {
				"name" : "app_datetime",
				"type" : "string"
			}, {
				"name" : "status_id",
				"type" : "int"
			}]);
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(), {
				header : "状态",
				width : 90,
				sortable:true,
				dataIndex : "status_id",
				renderer : function(value) {
					return STATUS_ARRAY[value];
				}
			}, {
				header : "申请人",
				width : 90,
				sortable:true,
				dataIndex : "app_user_name"
			}, {
				header : "申请时间",
				sortable:true,
				width : 180,
				dataIndex : "app_datetime"
//			},{
//				xtype : 'actioncolumn',
//				width : 40,
//				header : '详情',
//				items : [{
//					iconCls : 'edit',
//					text : '详情',
//					handler : function(grid, rowIndex, colIndex) {
//						var form_id_param = grid.store.getAt(rowIndex)
//								.get('form_id');
//						var url = __cxtPath
//								+ '/applymanage/applyofficesuppliesList.jsp?form_id='
//								+ form_id_param;
//						parent.xMis.turn('wf-openform-', '办公用品领用申请', url, 't',
//								true);
//					}
//				}]
			}
			]);
	var storeUrl = '';
	if (type == 'repair_type') {
		storeUrl = '/applyManageMaint.do?method=getApplyOfficialSuppliesRepairListSum';
	} else {
		storeUrl = '/applyManageMaint.do?method=getApplyOfficialSuppliesListSum';

	}
	var store = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath + storeUrl,
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
	store.load({
				params : {
					start : 0,
					limit : 20
				}
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
					}, '-', '查询：', new Ext.ux.form.SearchField({
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
									text : '输入查询的申请人、申请时间'
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
				}),
		listeners : {
			'rowclick' : function(grid, index) {
				var detailUrl = '';
				if (type == 'repair_type') {
					detailUrl = '/applyManageMaint.do?method=getApplyOfficialSuppliesRepairList';
				} else {
					detailUrl = '/applyManageMaint.do?method=getApplyOfficialSuppliesList';

				}
				var r = grid.store.getAt(index);
				var form_id_param = r.get('form_id');
				var gridPanelDetail = new com.bitc.xmis.ApplyOfficialSupplies.DetailGrid(
						{
							height : 100,
							dataUrl : detailUrl,
							autoScroll : true,
							region : 'center',
							autoLoadData : false
						});
				var win = new Ext.Window({
							title : '明细',
							layout : 'border',
							width : 600,
							height : 200,
							modal : true,
							items : gridPanelDetail
						}).show();
				gridPanelDetail.store.baseParams.form_id = form_id_param;
				gridPanelDetail.store.reload();

			}
		}

	});

	new Ext.Viewport({
				layout : 'border',
				items : gridPanel,
				border : false
			});

	var datetimeFormatFun = function(v) {
		return v != "" ? v.substring(0, 16) : "";
	};

});
