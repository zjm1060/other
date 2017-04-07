Ext.onReady(function() {
	Ext.QuickTips.init();
	var exportname = '审批列表';
	var PRIORITY_ARRAY = ['普通', '紧急'];

	var reRecord = Ext.data.Record.create([{
				"mapping" : "form_id",
				"name" : "form_id",
				"type" : "string"
			}, {
				"mapping" : "document_type_id",
				"name" : "document_type_id",
				"type" : "int"
			}, {
				"mapping" : "document_type_name",
				"name" : "document_type_name",
				"type" : "string"
			}, {
				"mapping" : "priority",
				"name" : "priority",
				"type" : "int"
			}, {
				"mapping" : "title",
				"name" : "title",
				"type" : "string"
			}, {
				"mapping" : "external_name",
				"name" : "external_name",
				"type" : "string"
			}, {
				"mapping" : "document_code",
				"name" : "document_code",
				"type" : "string"
			}, {
				"mapping" : "summary",
				"name" : "summary",
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
				"mapping" : "app_dept_leader_id",
				"name" : "app_dept_leader_id",
				"type" : "int"
			}, {
				"mapping" : "app_dept_leader_name",
				"name" : "app_dept_leader_name",
				"type" : "string"
			}, {
				"mapping" : "app_dept_approved_datetime",
				"name" : "app_dept_approved_datetime",
				"type" : "string"
			}, {
				"mapping" : "app_dept_approved_reason",
				"name" : "app_dept_approved_reason",
				"type" : "string"
			}, {
				"mapping" : "app_spec_leader_id",
				"name" : "app_spec_leader_id",
				"type" : "int"
			}, {
				"mapping" : "app_spec_leader_name",
				"name" : "app_spec_leader_name",
				"type" : "string"
			}, {
				"mapping" : "app_spec_approved_datetime",
				"name" : "app_spec_approved_datetime",
				"type" : "string"
			}, {
				"mapping" : "app_spec_approved_reason",
				"name" : "app_spec_approved_reason",
				"type" : "string"
			}, {
				"mapping" : "app_direc_leader_id",
				"name" : "app_direc_leader_id",
				"type" : "int"
			}, {
				"mapping" : "app_direc_leader_name",
				"name" : "app_direc_leader_name",
				"type" : "string"
			}, {
				"mapping" : "app_direc_approved_datetime",
				"name" : "app_direc_approved_datetime",
				"type" : "string"
			}, {
				"mapping" : "app_direc_approved_reason",
				"name" : "app_direc_approved_reason",
				"type" : "string"
			}]);
	var store = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/applyManageMaint.do?method=getDocumentAppFormDetailList',
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
				header : "标题",
				sortable : true,
				width : 90,
				dataIndex : "title"
			}, {
				header : "外部单位名称",
				width : 90,
				dataIndex : "external_name"
			}, {
				header : "公文类别",
				width : 90,
				sortable : true,
				dataIndex : "document_type_name"
			}, {
				header : "优先等级",
				width : 90,
				dataIndex : "priority",
				renderer : function(value) {
					return PRIORITY_ARRAY[value];
				}
			}, {
				header : "公文编号",
				width : 90,
				dataIndex : "document_code"
			}, {
				header : "摘要说明",
				width : 180,
				dataIndex : "summary"
			}, {
				header : "申请人",
				width : 90,
				sortable : true,
				dataIndex : "app_user_name"
			}, {
				header : "申请时间",
				width : 180,
				dataIndex : "app_datetime"
			}, {
				header : "部门领导",
				width : 90,
				hidden : true,
				dataIndex : "app_dept_leader_name"
			}, {
				header : "部门领导审批时间",
				width : 180,
				hidden : true,
				dataIndex : "app_dept_approved_datetime"
			}, {
				header : "部门领导意见",
				width : 90,
				hidden : true,
				dataIndex : "app_dept_approved_reason"
			}, {
				header : "分管领导",
				width : 90,
				hidden : true,
				dataIndex : "app_spec_leader_name"
			}, {
				header : "分管领导审批时间",
				width : 180,
				hidden : true,
				dataIndex : "app_spec_approved_datetime"
			}, {
				header : "分管领导意见",
				width : 90,
				hidden : true,
				dataIndex : "app_spec_approved_reason"
			}, {
				header : "主管领导",
				width : 90,
				hidden : true,
				dataIndex : "app_direc_leader_name"
			}, {
				header : "主管领导审批时间",
				width : 180,
				hidden : true,
				dataIndex : "app_direc_approved_datetime"
			}, {
				header : "主管领导审批意见",
				width : 90,
				hidden : true,
				dataIndex : "app_direc_approved_reason"

			}, {
				xtype : 'actioncolumn',
				width : 80,
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
			},{

				xtype : 'actioncolumn',
				width : 80,
				header : '下载行文',
				align : 'center',
				items : [{
					getClass : function(v, meta, rec) {
						return 'edit';
					},
					handler : function(grid, rowIndex, colIndex) {
						var rec = grid.getStore().getAt(rowIndex);
						var form_id = rec.get('form_id');

						var url = __cxtPath
						+ '/fileUpload.do?type=11'
						+ '&form_id='
						+ form_id
						+'&fileName=行文';
						location.href=url;
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
									text : '输入查询的公文标题、申请人姓名、申请时间、摘要说明、公文类型'
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
	var datetimeFormatFun = function(v) {
		return v != "" ? v.substring(0, 16) : "";
	};
	// main
	new Ext.Viewport({
				layout : 'border',
				items : [gridPanel]
			});

});
