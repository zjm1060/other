Ext.onReady(function() {
	Ext.QuickTips.init();
	var workItemId = params.substring(0, params.indexOf(','));
	var isTodo = params.substring(params.indexOf(',') + 1, params.length);
	if (isTodo == 'false') {
		isTodo = false;
	} else {
		isTodo = true;
	}

	var workItemId = request.getParameter("workItemId");// 
	var attendanceDatastore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/attdMaint.do?method=getMissAttendanceListByEmpId&workItemId='
					+ workItemId + '&isTodo=' + isTodo + "&isOfficialTravel=1",
			method : "POST"
		}),
		baseParams : {
			limit : 20
		},
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, new Ext.data.Record.create([{
							"name" : "ID"
						}, {
							"name" : "申请人ID",
							"type" : "int"
						}, {
							"name" : "applicantname"
						}, {
							"name" : "部门名称"
						}, {
							"name" : "申请日期"
						}, {
							"name" : "缺勤日期"
						}, {
							"name" : "上下午"
						}, {
							"name" : "开始日期"
						}, {
							"name" : "结束日期"
						}, {
							"name" : "事由"
						}, {
							"name" : "审批人及审批时间"
						}, {
							"name" : "操作人ID",
							"type" : "int"
						}, {
							"name" : "agentname"
						}, {
							"name" : "workItemId"
						}, {
							"name" : "流程实例ID"
						}, {
							"name" : "考勤单ID"
						}, {
							"name" : "用户ID"
						}, {
							"name" : "任务人"
						}])),
		sortInfo : {
			field : "考勤单ID",
			direction : "ASC"
		},
		autoLoad : true,
		remoteSort : true
	});
	var businessDatastore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/attdMaint.do?method=getMissAttendanceListByEmpId&workItemId='
					+ workItemId + '&isTodo=' + isTodo + "&isOfficialTravel=0",
			method : "POST"
		}),
		baseParams : {
			limit : 20
		},
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, new Ext.data.Record.create([{
							"name" : "ID"
						}, {
							"name" : "申请人ID",
							"type" : "int"
						}, {
							"name" : "applicantname"
						}, {
							"name" : "部门名称"
						}, {
							"name" : "申请日期"
						}, {
							"name" : "缺勤日期"
						}, {
							"name" : "上下午"
						}, {
							"name" : "开始日期"
						}, {
							"name" : "结束日期"
						}, {
							"name" : "事由"
						}, {
							"name" : "审批人及审批时间"
						}, {
							"name" : "操作人ID",
							"type" : "int"
						}, {
							"name" : "agentname"
						}, {
							"name" : "workItemId"
						}, {
							"name" : "流程实例ID"
						}, {
							"name" : "考勤单ID"
						}, {
							"name" : "用户ID"
						}, {
							"name" : "任务人"
						}])),
		sortInfo : {
			field : "考勤单ID",
			direction : "ASC"
		},
		autoLoad : true,
		remoteSort : true
	});

	var sm = new Ext.grid.CheckboxSelectionModel();
	var businessClm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(), {
				header : "申请人",
				dataIndex : "applicantname",
				sortable : true
			}, {
				header : "部门",
				dataIndex : "部门名称",
				sortable : true
			}, {
				header : "开始日期",
				dataIndex : "开始日期",
				sortable : true,
				renderer : customer_formatDate
			}, {
				header : "结束日期",
				dataIndex : "结束日期",
				sortable : true,
				renderer : customer_formatDate
			}, {
				header : "事由",
				dataIndex : "事由",
				sortable : true
			}, {
				header : "审批意见",
				dataIndex : "审批人及审批时间",
				sortable : true
			}, {
				header : "申请日期",
				dataIndex : "申请日期",
				sortable : true,
				renderer : customer_formatDate
			}, {
				header : "代理人",
				dataIndex : "agentname",
				hidden : true,
				sortable : true
			}, {
				header : "考勤单ID",
				dataIndex : "考勤单ID",
				hidden : true,
				sortable : true
			}, {
				header : "任务人",
				dataIndex : "任务人",
				sortable : true
			}, {
				header : "被授权人",
				dataIndex : "被授权人",
				hidden : true,
				sortable : true
			}]);
	var attendanceClm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(), {
				header : "申请人",
				dataIndex : "applicantname",
				sortable : true
			}, {
				header : "部门",
				dataIndex : "部门名称",
				sortable : true
			}, {
				header : "缺勤日期",
				dataIndex : "缺勤日期",
				sortable : true,
				renderer : customer_formatDate
			}, {
				header : "全日/上班/下班",
				dataIndex : "上下午",
				sortable : true,
				exportFormater : function(val, row, index, pageinfo) {
					if (val == 1) {
						return '上班';
					} else if (val == 2) {
						return '下班';
					} else {
						return '全日';
					}
				},
				renderer : function(val) {
					if (val == 1) {
						return '上班';
					} else if (val == 2) {
						return '下班';
					} else {
						return '全日';
					}
				}
			}, {
				header : "事由",
				dataIndex : "事由",
				sortable : true
			}, {
				header : "审批意见",
				dataIndex : "审批人及审批时间",
				sortable : true
			}, {
				header : "申请日期",
				dataIndex : "申请日期",
				sortable : true,
				renderer : customer_formatDate
			}, {
				header : "代理人",
				dataIndex : "agentname",
				hidden : true,
				sortable : true
			}, {
				header : "考勤单ID",
				dataIndex : "考勤单ID",
				hidden : true,
				sortable : true
			}, {
				header : "任务人",
				dataIndex : "任务人",
				sortable : true
			}, {
				header : "被授权人",
				dataIndex : "被授权人",
				hidden : true,
				sortable : true
			}]);
	var attendanceGrid = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : attendanceDatastore,
		cm : attendanceClm,
		sm : sm,
		viewConfig : {
			forceFit : true
		},
		tbar : [{
			ref : '../okBtn',
			iconCls : 'ok',
			text : '同意',
			handler : function() {
				var records = attendanceGrid.store.data;
				com.bitc.xmis.workflow.DoApprovalTaskByAllFunction(attendanceGrid,0, records.items[0].data.workItemId);
			}
		}, {
			ref : '../noBtn',
			iconCls : 'ok',
			text : '不同意',
			handler : function() {
				var records = attendanceGrid.store.data;
				com.bitc.xmis.workflow.DoApprovalTaskByAllFunction(attendanceGrid,1, records.items[0].data.workItemId);
			}
		}],
		bbar : new Ext.PagingToolbar({
			pageSize : 20,
			store : attendanceDatastore,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : '无显示数据',
			plugins : [new Ext.ux.Export({
				exportname : '考勤补报',
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.RowNumber'
						}, {
							columnHeader : '申请日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '缺勤日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}]
			}), new Ext.ux.PagelimitCombo()]
		})

	});
	var businessGrid = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : businessDatastore,
		cm : businessClm,
		sm : sm,
		viewConfig : {
			forceFit : true
		},
		tbar : [{
			ref : '../okBtn',
			iconCls : 'ok',
			text : '同意',
			handler : function() {
				var records = businessGrid.store.data;
				com.bitc.xmis.workflow.DoApprovalTaskByAllFunction(businessGrid,0, records.items[0].data.workItemId);
			}
		}, {
			ref : '../noBtn',
			iconCls : 'ok',
			text : '不同意',
			handler : function() {
				var records = businessGrid.store.data;
				com.bitc.xmis.workflow.DoApprovalTaskByAllFunction(businessGrid,1, records.items[0].data.workItemId);
			}
		}],
		bbar : new Ext.PagingToolbar({
			pageSize : 20,
			store : businessDatastore,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : '无显示数据',
			plugins : [new Ext.ux.Export({
				exportname : '出差补报',
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.RowNumber'
						}, {
							columnHeader : '申请日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '开始日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '结束日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}]
			}), new Ext.ux.PagelimitCombo()]
		})

	});
	var isOfficialTravelValue = request.getParameter('isOfficialTravel');//0 是出差
	var tabs = new Ext.TabPanel({
				height : 535,
				region : 'center',
				margins : '3 3 3 0',
				activeTab : isOfficialTravelValue==0?1:0,
				tabPosition : 'top',
				items : [{
							id : 'attendanceID',
							title : '考勤补报',
							layout : 'border',
							disabled:isOfficialTravelValue==0?true:false,
							items : [attendanceGrid]
						}, {
							id : 'businessID',
							title : '出差补报',
							layout : 'border',
							disabled:isOfficialTravelValue==1?true:false,
							items : [businessGrid]
						}]
			});
	var main = new Ext.Viewport({
				layout : 'border',
				items : tabs,
				border : false
			});
});
