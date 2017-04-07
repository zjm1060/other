Ext.onReady(function(){
	Ext.QuickTips.init();
	var Record = new Ext.data.Record.create([{
		name:'formid',
		mapping:'formid'
	},{
		name:'proid',
		mapping:'proid'
	},{
		name:'work_month',
		mapping:'work_month'
	},{
		name:'app_dept_name',
		mapping:'app_dept_name'
	},{
		name:'status',
		mapping:'status'
		
	},{
		name:'plan_assigned_name',
		mapping:'plan_assigned_name'
	},{
		name:'plan_assigned_date',
		mapping:'plan_assigned_date'
	},{
		name:'app_user_name',
		mapping:'app_user_name'
	},{
		name:'app_date',
		mapping:'app_date'
	}]);
	var gridstore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/applyManageMaint.do?method=monthPlanQuery',
			method : "POST"
		}),
		baseParams : {
			limit : 20
		},
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, Record),
	    autoLoad:true
	});

	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(),{
		header:'formid',
		hidden:true,
		width:90,
		dataIndex:'formid'
	},{
		header:'proid',
		width:90,
		hidden:true,
		dataIndex:'proid'
	},{
		header:'月份',
		width:90,
		dataIndex:'work_month'
	},{
		header:'部门名称',
		width:90,
		dataIndex:'app_dept_name'
	},{
		header:'状态',
		width:200,
		dataIndex:'status',
		align:'center'
	},{
		header:'处理人',
		width:90,
		dataIndex:'plan_assigned_name'
	},{
		header:'处理时间',
		width:90,
		dataIndex:'plan_assigned_date'
	},{
		header:'申请人',
		width:90,
		dataIndex:'app_user_name'
	},{
		header:'申请时间',
		width:90,
		dataIndex:'app_date'
	},{
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
						var form_id = rec.get('formid');
						var proid = rec.get('proid');
						var url = __cxtPath
								+ '/MonthlyManagementWork/MonthlyManagementWork.jsp?form_id='
								+ form_id+'&processInstanceId='+proid+'&todoId=query';
						parent.xMis.turn('wf-openform-', '月报查询', url, 't',
								true);
					}
				}]
			}]);
	var grid = new Ext.grid.GridPanel({
		region:'center',
		border:false,
		loadMask:true,
		loadMask:{
			msg:'正在加载数据，请稍等……'
		},
		store:gridstore,
		cm:cm,
		tbar:['部门选择：', new com.bitc.xmis.Combos.DivisionDeptCombo({
							width : 150,
							COMPANYID : 1,
							ref : '../division_combo',
							listeners : {
								'select' : function(combo, record, index) {
									var selValue = record.get('id');
									gridstore.setBaseParam('divisionID',
											selValue);
								}
							}
						}),'计划时间：&nbsp;',new Ext.form.DateField({
							ref:'../workMonth',
							format:'Y-m',
							showToday:true,
							editable:false,
							anchor:'100%',
							plugins:'monthPickerPlugin'
						}),{
							text:'条件重置',
							handler: function(){
								grid.division_combo.setValue('-1');
								grid.workMonth.setValue('');
							}
						},'-',{
							ref:'../searchBtn',
							iconCls:'searchIcon',
							text:'查询',
							handler:function(b,e){
								var divisionID = grid.division_combo.getValue();
								var workMonth = grid.workMonth.getValue();
								grid.store.setBaseParam('divisionID', divisionID);
								grid.store.setBaseParam('workMonth', workMonth);
								
								grid.store.load({
									params:{
										start: 0
									}
								});
							}
						}
				],
				bbar : new Ext.PagingToolbar({
					pageSize : 20,
					store : gridstore,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					plugins : [new Ext.ux.PagelimitCombo()]
				})

	});
//	grid.on('rowclick', function(gridpanel, index, e) {
//		var r = gridpanel.getStore().getAt(index);
//		var processinstanceid = r.get('processinstanceid');
//		com.bitc.xmis.workflow.showTraceWin(processinstanceid);
//	});
	var main = new Ext.Viewport({
				layout : 'border',
				items : grid,
				border : false
			});
});