Ext.onReady(function() {
	Ext.QuickTips.init();
	var exportname = '工作流查询';

	var reRecord = Ext.data.Record.create([{
				"mapping" : "流程实例ID",
				"name" : "流程实例ID",
				"type" : "string"
			}, {
				"name" : "流程名称",
				"type" : "string"
			}, {
				"name" : "PROCESS_ID",
				"type" : "string"
			}, {
				"name" : "CREATOR_ID",
				"type" : "int"
			}, {
				"name" : "创建人",
				"type" : "string"
			}, {
				"name" : "状态",
				"type" : "int"
			}, {
				"name" : "创建时间",
				"type" : "string"
			}, {
				"name" : "完成时间",
				"type" : "string"
			}, {
				"name" : "VERSION",
				"type" : "string"
			}, {
				"name" : "接收人",
				"type" : "string"
			}]);
	var store = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/workflow.do?method=getProcessInstanceList',
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
					field : '完成时间',
					direction : 'DESC'
				},
				remoteSort : true
			});

	var csm = new Ext.grid.CheckboxSelectionModel();
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						width : 40
					}), {
				header : "流程实例ID",
				hidden : true,
				width : 220,
				dataIndex : '流程实例ID'
			}, {
				header : "流程名称",
				sortable : true,
				width : 400,
				dataIndex : '流程名称',
					renderer : function(value, metaData, record) {
						var processInstanceId = "'"+record.data.流程实例ID + "'";
						return '<a  href="javascript:;" onclick="com.bitc.xmis.workflow.showTraceWin('+processInstanceId+');">'
								+ value + '</a>';
					}
			}, {
				header : "创建人ID",
				hidden : true,
				dataIndex : 'CREATOR_ID'
			}, {
				header : "状态",
				sortable : true,
				width : 80,
				dataIndex : '状态',
				exportFormater : function(value, row, index, pageinfo) {
					if (value == 1) {
						return '正在进行';
					} else if (value == 7) {
						return '已完成';
					} else {
						return value;
					}
				},
				renderer : function(value) {
					if (value == 1) {
						return '<font  color="green">正在进行</font>'
					} else if (value == 7) {
						return '<font  color="red">已完成</font>'
					} else {
						return value;
					}
				}
			}, {
				header : "创建人",
				sortable : true,
				dataIndex : '创建人'
			}, {
				header : "创建时间",
				sortable : true,
				width : 180,
				dataIndex : '创建时间',
				renderer : function(value) {
					return value != null ? value.substring(0, 16) : '';
				}
			}, {
				header : "当前执行人",
				width : 180,
				dataIndex : '接收人'
			}, {
				header : "处理时间",
				sortable : true,
				width : 180,
				dataIndex : '完成时间',
				renderer : function(value) {
					return value != null ? value.substring(0, 16) : '';
				}
			}, {
				header : "版本",
				hidden : true,
				width : 80,
				dataIndex : 'VERSION'

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
				tbar : ['模糊查询：', new Ext.ux.InklingSearchField({
									width : 120,
									store : store,
									tooltip : {
										title : '模糊查询',
										text : '您可以输入“流程名称”,“创建人”,“时间”进行查询'
									},
									inklings : [{
												name : '创建人',
												op : 'like',
												type : 'string'
											}, {
												name : '流程名称',
												op : 'like',
												type : 'string'
											}, {
												name : '流程实例ID',
												op : 'like',
												type : 'string'
											}, {
												name : '创建时间',
												op : 'like',
												type : 'string'
											}, {
												name : '完成时间',
												op : 'like',
												type : 'string'
											}]
								}),'->','<font color="blue">*单击流程名字可以对其进行流程跟踪</font>'],
				bbar : new Ext.PagingToolbar({
							pageSize : 20,
							store : store,
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
													exp.onExport(gridPanel,
															exportname, true);
												}
											}, {
												text : '导出所有数据',
												handler : function() {
													exp.onExport(gridPanel,
															exportname, false);
												}
											}]
										}
									})]
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
