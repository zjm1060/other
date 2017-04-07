Ext.onReady(function() {
	Ext.QuickTips.init();
	var exportname = '审批列表';
	var PRIORITY_ARRAY = ['普通', '紧急'];

	var reRecord = Ext.data.Record.create([{
				"name" : "form_id",
				"type" : "string"
			},{
				"name" : "car_type",
				"type" : "string"
			},{
				"name" : "car_brand",
				"type" : "string"
			}, {
				"name" : "car_No",
				"type" : "string"
			}, {
				"name" : "status_id",
				"type" : "int"
			}, {
				"name" : "status_name",
				"type" : "string"
			}, {
				"name" : "priority",
				"type" : "int"
			}, {
				"name" : "reason",
				"type" : "string"
			}, {
				"name" : "app_user_id",
				"type" : "int"
			}, {
				"name" : "app_datetime",
				"type" : "string"
			}, {
				"name" : "assigned_id",
				"type" : "int"
			}, {
				"name" : "assigned_datetime",
				"type" : "string"
			}, {
				"name" : "assigned_surname",
				"type" : "string"
			}, {
				"name" : "assigned_name",
				"type" : "string"
			}, {
				"name" : "begin_datetime",
				"type" : "string"
			}, {
				"name" : "end_datetime",
				"type" : "string"
			}]);
	var store = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/applyManageMaint.do?method=getAppCarList',
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
					field : 'begin_datetime',
					direction : 'DESC'
				},
				remoteSort : true
			});
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(),
	                                   {
		header : "车品牌",
		width : 90,
		dataIndex : "car_brand"
	},{
		header : "车型",
		width : 90,
		dataIndex : "car_type"
	},{
				header : "车牌号",
				width : 90,
				hidden:true,
				dataIndex : "car_No"
			}, {
				header : "状态",
				width : 90,
				dataIndex : "status_name",
				renderer:function(value){
					 if(value == '预约申请'){
					    return '<font color="red">'+value+'</font>';
					 }else if(value == '已撤销'){
					 	 return '<font color="orange">'+value+'</font>';
					 }else if(value == '预约中'){
					 	 return '<font color="blue">'+value+'</font>';
					 }else if(value == '已过期'){
					 	 return '<font color="black">'+value+'</font>';
					 }else if(value == '使用中'){
					 	 return '<font color="green">'+value+'</font>';
					 }else{
						return value;					 	
					 }
				}
			}, {
				header : "优先等级",
				width : 90,
				dataIndex : "priority",
				renderer : function(value) {
					return PRIORITY_ARRAY[value];
				}
			}, {
				header : "事由",
				sortable : true,
				width : 90,
				dataIndex : "reason"
			}, {
				header : "预计开始时间",
				sortable : true,
				width : 180,
				dataIndex : "begin_datetime",
				renderer : function(value) {
					return datetimeFormatFun(value);
				}
			}, {
				header : "预计结束时间",
				sortable : true,
				width : 180,
				dataIndex : "end_datetime",
				renderer : function(value) {
					return datetimeFormatFun(value);
				}
			}, {
				header : "app_user_id",
				hidden : true,
				width : 180,
				dataIndex : "app_user_id"
			}, {
				header : "app_datetime",
				hidden : true,
				width : 180,
				dataIndex : "app_datetime"
			}, {
				header : "assigned_id",
				hidden : true,
				width : 180,
				dataIndex : "assigned_id"
			}, {
				header : "assigned_datetime",
				hidden : true,
				width : 180,
				dataIndex : "assigned_datetime",
				renderer : function(value) {
					return datetimeFormatFun(value);
				}
			}, {
				xtype : 'actioncolumn',
				width : 40,
				header : '同意',
				align : 'center',
				items : [{
					getClass : function(v, meta, rec) {

						if (((isApplyPerson || rec.data.assigned_id == __emp.userID)
								&& (rec.data.status_id == 1 || rec.data.status_id == 2) && (rec.data.status_name != '已过期'))) {
							this.items[0].tooltip = '同意';
							return 'edit';
						}
					},
					handler : function(grid, rowIndex, colIndex) {

						var form_id = grid.store.getAt(rowIndex).data.form_id;
						com.bitc.xmis.ApplyCarWinShow(grid, 'agree', form_id,
								com.bitc.xmis.Combos.STATUS.agree);

					}
				}]
			}, {
				xtype : 'actioncolumn',
				width : 40,
				header : '修改',
				align : 'center',
				items : [{
					getClass : function(v, meta, rec) {
						if ((rec.data.app_user_id == __emp.userID && (rec.data.status_id == 1 || rec.data.status_id == 3))
								|| ((isApplyPerson || rec.data.assigned_id == __emp.userID) && rec.data.status_id == 3)) {
							this.items[0].tooltip = '修改';
							return 'edit';
						}
					},
					handler : function(grid, rowIndex, colIndex) {
						if (grid.store.getAt(rowIndex).data.status_id == 1) {
							com.bitc.xmis.Combos.STATUS.update = 1;
						}
						if ((isApplyPerson || grid.store.getAt(rowIndex).data.assigned_id == __emp.userID)
								&& grid.store.getAt(rowIndex).data.status_id == 3) {
							com.bitc.xmis.Combos.STATUS.update = 3;
						}
						var form_id = grid.store.getAt(rowIndex).data.form_id;
						com.bitc.xmis.ApplyCarWinShow(grid, 'update', form_id,
								com.bitc.xmis.Combos.STATUS.update);

					}
				}]
			}, {
				xtype : 'actioncolumn',
				width : 40,
				header : '撤销',
				align : 'center',
				items : [{
					getClass : function(v, meta, rec) {
						if ((rec.data.app_user_id == __emp.userID && (rec.data.status_id == 1
								|| rec.data.status_id == 2 || rec.data.status_id == 3))
								|| ((isApplyPerson || rec.data.assigned_id == __emp.userID) && rec.data.status_id == 3)) {
							this.items[0].tooltip = '撤销';
							return 'edit';
						}
					},
					handler : function(grid, rowIndex, colIndex) {
						var form_id = grid.store.getAt(rowIndex).data.form_id;
						com.bitc.xmis.ApplyCarWinShow(grid, 'cancel', form_id,
								com.bitc.xmis.Combos.STATUS.cancel);

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
						text : '新建',
						iconCls : 'save',
						handler : function() {
							com.bitc.xmis
									.ApplyCarWinShow(gridPanel, 'create', null,
											com.bitc.xmis.Combos.STATUS.create,
											0);
						}
					}, '-', {
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
									text : '输入查询的车牌、开始时间（日期）'
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
