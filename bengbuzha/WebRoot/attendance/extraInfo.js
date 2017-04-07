/**
 * 根据部门ID 列出部门下的所有员工的下拉列表
 * 
 * 用于补报考勤申请页面、出差补报页面
 */
Ext.ns('com.bitc.xmis.Combos');
com.bitc.xmis.Combos.ApplicantCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择申请人',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
					url : String
							.format(
									'{0}/orgMaint.do?method=getPersonByDeptID&&deptID={1}',
									__cxtPath, this.DEPTID),
					root : 'rows',
					autoLoad : true,
					fields : [{
								name : "id",
								type : "int",
								mapping : "userID"
							}, {
								name : "name",
								type : "string",
								mapping : "userName"
							}],
					listeners : {
						scope : this,
						'load' : function() {
							this.setValue(this.value);
						}
					}
				});
		com.bitc.xmis.Combos.ApplicantCombo.superclass.initComponent.call(this);
	}
});
/**
 * 事业部的下拉列表 即一级部门
 * 
 */
com.bitc.xmis.Combos.DivisionDeptCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择事业部',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
					url : String
							.format(
									'{0}/orgMaint.do?method=getAllDivisionDept&&companyID={1}',
									__cxtPath, this.COMPANYID),
					root : 'rows',
					autoLoad : true,
					fields : [{
								name : "id",
								type : "int",
								mapping : "部门ID"
							}, {
								name : "name",
								type : "string",
								mapping : "部门名称"
							}],
					listeners : {
						scope : this,
						'load' : function() {
							this.store.insert(0, new this.store.recordType({
												id : '-1',
												name : '-全部-'
											}));
							this.setValue(this.value);
						}
					}
				});
		com.bitc.xmis.Combos.DivisionDeptCombo.superclass.initComponent
				.call(this);
	}
});

Ext.ns('com.bitc.xmis.attd');
// 最小的可输入日期
com.bitc.xmis.attd.getMin = function(lastday) {
	// 参数 截止日期号；
	var currentDate = new Date();
	var day = Ext.util.Format.date(new Date(), "d");
	if (day <= lastday) {
		currentDate = currentDate.add(Date.MONTH, -1);
	}
	var minDate = Ext.util.Format.date(currentDate, 'Y-m');
	return minDate + "-01";
}
// 当天可录入的有效的考勤年月
com.bitc.xmis.attd.getKqMonth = function(lastday) {
	var currentDate = new Date();
	var day = Ext.util.Format.date(new Date(), "d");
	if (day <= lastday) {
		var result1 = Ext.util.Format.date(currentDate, 'Y-m');
		currentDate = currentDate.add(Date.MONTH, -1);

		return Ext.util.Format.date(currentDate, 'Y-m') + '，' + result1;
	}
	return Ext.util.Format.date(currentDate, 'Y-m');
}

// 本日的年月yyyy-mm
com.bitc.xmis.attd.getMaxMonth = function(lastday) {
	var currentDate = new Date();
	var day = Ext.util.Format.date(new Date(), "d");
	if (day > lastday) {
		currentDate = currentDate.add(Date.MONTH, +1);
	}
	var lastDate = Ext.util.Format.date(currentDate, 'Y-m');
	return lastDate;

}

com.bitc.xmis.attd.Tip = Ext.extend(Ext.Panel, {
			height : 30,
			border : false,
			initComponent : function() {
				this.html = '<div style=\"margin-top:8px;\">' + '温馨提示：截至'
						+ com.bitc.xmis.attd.getMaxMonth(this.lastday) + '-'
						+ this.lastday + '，您可以提交'
						+ com.bitc.xmis.attd.getKqMonth(this.lastday)
						+ '考勤月的补报申请。</div>'
				com.bitc.xmis.attd.Tip.superclass.initComponent.call(this);
			},
			onRender : function(a, b) {
				com.bitc.xmis.attd.Tip.superclass.onRender.call(this, a, b);
			}
		});
com.bitc.xmis.attd.TipWithCondition = Ext.extend(Ext.Panel, {
	height : 50,
	border : false,
	initComponent : function() {
		this.html = '<div style=\"margin-top:0px;\"><font color=\"#CC0000\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
				+ com.bitc.xmis.attd.getMin(this.lastday).substring(0, 7)
				+ '考勤月补报申请截至日期为本日'
				+ com.bitc.xmis.attd.getMaxMonth(this.lastday)
				+ '-'
				+ this.lastday
				+ '，请您提醒主管领导，'
				+ '</font>'
				+ '</div>'
				+ '<div style=\"margin-top:8px;\"><font  color=\"#CC0000\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;务必于今日完成'
				+ com.bitc.xmis.attd.getMin(this.lastday).substring(0, 7)
				+ '月考勤审批，逾期考勤补报无效。</font></div>'
		com.bitc.xmis.attd.TipWithCondition.superclass.initComponent.call(this);
	},
	onRender : function(a, b) {
		com.bitc.xmis.attd.TipWithCondition.superclass.onRender
				.call(this, a, b);
	}
});
com.bitc.xmis.attd.Btip = new Ext.Panel({
	height : 150,
	border : false,
	html : '<div style=\"margin-top:10;\">备注：</div><div style=\"margin-top:8px;\">提交成功后，你可以通过点击首页的“查看所有任务”按钮，在“已处理任务”中查找跟踪此任务。如下图：</div>'
			+ '<div style=\"margin-top:8px;\"><img src="../resources/images/seetask3.jpg"/></div>'
});
com.bitc.xmis.attd.Btip2 = new Ext.Panel({
	height : 150,
	border : false,
	html : '<div style=\"margin-top:10;\">备注：</div><div style=\"margin-top:8px;\">提交成功后，你可以通过点击首页的“查看所有任务”按钮，在“已处理任务”中查找跟踪此任务。如下图：</div>'
			+ '<div style=\"margin-top:8px;\"><img src="resources/images/seetask3.jpg"/></div>'
});

Ext.ns("com.bitc.xmis.workflow");
/**
 * @param gridpanel
 *            用于grid的刷新
 * @param isAgreeInt
 *            int 是否同意的标识
 * @param ids
 *            多个workItemId拼成的字符串
 * @param closewin
 *            操作成功后是否需要关闭此窗口
 */
com.bitc.xmis.workflow.DoApprovalTaskIsAgree = function(gridpanel, isAgreeInt,
		ids, closewin) {
	var win = new Ext.Window({
		title : '请确定是否同意？',
		layout : 'border',
		width : 300,
		height : 180,
		modal : true,
		border : false,
		items : [new Ext.Panel({
			region : 'center',
			ref:'p',
			frame : true,
			border : false,
			labelAlign : 'left',
			labelWidth : 80,
			bodyStyle : 'padding:10px',
			buttonAlign:'center',
			fbar : [{
				text : '确定',
				handler : function() {
					var text = win.p.textID.getValue()
					if (isAgreeInt == '1' && text == '') {
						Ext.MessageBox.alert('提示', '不同意的原因不可为空，请填写原因！');
						return;
					}
					Ext.Ajax.request({
								url : String
										.format(
												'{0}/attdMaint.do?method=doApprovalTask',
												__cxtPath),
								method : 'post',
								params : {
									isAgree : isAgreeInt,
									comment : text,
									ids : ids
								},
								success : function(response) {
									var result = Ext
											.decode(response.responseText);
									if (result.success) {
										Ext.Msg.alert(result.messageTitle,
												result.message, function() {
													gridpanel.store.reload();
															parent.xMis.refresh('mytodo','myworkitemnewtodo');
													if (closewin) {
														parent.xMis
																.closeTab(window);
													}
												});
									} else {
										Ext.Msg.alert(result.messageTitle,
												result.message)
									}
									win.hide();
								},
								failure : function(response) {
									Ext.Msg.alert('失败', '操作失败');
								}
							});

				}
			}, {
				text : '取消',
				handler : function() {
					win.hide();
				}
			}],
			items : [new Ext.form.Label({
								width : 100,
								hideMode : "visibility",// 默认display,可以取值：display，offsets，visibility
								html : "<font size='2px'>请填写备注:</font><br/>"
							}), {
						xtype : 'textarea',
						ref:'textID',
						width : 250,
						name : 'text'
					}]
		})]
	}).show();

}

/**
 * 批量审批 可选
 * 
 * @param gridpanel
 * @param int
 *            isAgreeInt
 */
com.bitc.xmis.workflow.DoApprovalTaskBySelectFunction = function(gridpanel,
		isAgreeInt) {
	var records = gridpanel.getSelectionModel().getSelections();
	var ids = "";
	if (records.length == 0) {
		Ext.MessageBox.alert("提示", "请选择任务");
		return;
	} else {
		for (var i = 0; i < records.length; i++) {
			ids += records[i].get('workItemId') + ",";
		}
		if (ids.length > 1) {
			ids = ids.substr(0, ids.length - 1);
		}
	}
	com.bitc.xmis.workflow.DoApprovalTaskIsAgree(gridpanel, isAgreeInt, ids,
			false);
}

/**
 * 审批 只有一条workItemId 操作完成关闭窗口
 * 
 * @param gridpanel
 * @param int
 *            isAgreeInt
 * @param workItemId
 */
com.bitc.xmis.workflow.DoApprovalTaskByAllFunction = function(gridpanel,
		isAgreeInt, workItemId) {
	var records = gridpanel.store.data;
	if (records.length == 0) {
		Ext.MessageBox.alert("提示", "请选择任务");
		return;
	}
	com.bitc.xmis.workflow.DoApprovalTaskIsAgree(gridpanel, isAgreeInt,
			workItemId, true);
}

/**
 * @class com.bitc.xmis.workflow.ApprovalTaskPanel
 * @extends Ext.grid.GridPanel 任务列表 -用于批量处理任务
 */
com.bitc.xmis.workflow.ApprovalTaskPanel = Ext.extend(Ext.grid.GridPanel, {
	ref : 'p',
	initComponent : function() {
		var tPanel = this;
		this.store = new Ext.data.JsonStore({
					url : String.format('{0}/attdMaint.do?method=getTaskList',
							// url :
							// String.format('{0}/workflow.do?method=getApprovalTaskList',
							__cxtPath),
					root : 'rows',
					baseParams : {
						task_ID : this.task_ID,
						limit : 20
					},
					fields : ['ID', 'task_ID', 'todoId', 'workItemId',
							'DISPLAY_NAME', '标题', '发送人', '发送人部门ID', '发送人部门名称',
							'接收人', '发送人ID', '用户ID', '状态', 'PROCESSINSTANCE_ID',
							'COMMENTS', '创建时间', '申请人ID', 'applicantname',
							'部门名称', '申请日期', '缺勤日期', '上下午', '开始日期', '结束日期',
							'事由', '考勤单ID'],
					sortInfo : {
						field : '创建时间',
						direction : 'DESC'
					},
					autoLoad : true
				});
		this.store.load({
					params : {
						start : 0,
						limit : 20
					}
				})
		var csm = new Ext.grid.CheckboxSelectionModel();
		this.columns = [csm, new Ext.grid.RowNumberer(), {
			xtype : 'actioncolumn',
			width : 27,
			items : [{
				iconCls : 'diagram',
				tooltip : '任务跟踪',
				handler : function(grid, rowIndex, colIndex) {
					var tracePanel = new com.bitc.xmis.workflow.TracePanel({
								border : false,
								region : 'center',
								listHeight : Ext.getBody().getHeight() / 2 - 50,
								processInstanceId : grid.store.getAt(rowIndex)
										.get('PROCESSINSTANCE_ID')
							});
					var traceWin = new Ext.Window({
								title : '流程跟踪',
								x : 25,
								y : 25,
								width : Ext.getBody().getWidth() - 100,
								height : Ext.getBody().getHeight() - 100,
								items : tracePanel,
								layout : 'border',
								closeAction : 'close',
								tools : [{
											qtip : '刷新',
											id : 'refresh',
											handler : function() {
												tracePanel.reload();
											}
										}]
							});
					traceWin.show();
				}
			}]
		}, {
			header : '标题',
			width : 400,
			id : 'text_id',
			dataIndex : '标题',
			sortable : true,
			renderer : function(value, metaData, record) {
				return '<a href="javascript:;" onclick="parent.xMis.turn(\'wf-openform-'
						+ record.get('workItemId')
						+ '\',\''
						+ value
						+ '\',\''
						+ __cxtPath
						+ '/workflow.do?method=openForm&workItemId='
						+ record.get('workItemId')
						+ '&todoId='
						+ record.get('todoId')
						+ '\',\'menu-openform-info\')">'
						+ value + '</a>';
			}
		}, {
			header : '发送人',
			width : 100,
			hidden : true,
			sortable : true,
			dataIndex : '发送人'
		}, {
			header : '发送人所在部门',
			width : 150,
			hidden : true,
			sortable : true,
			dataIndex : '发送人部门名称'
		}, {
			header : '接收人',
			hidden : true,
			width : 100,
			sortable : true,
			dataIndex : '接收人'
		}, {
			header : "申请人ID",
			dataIndex : "申请人ID",
			hidden : true,
			sortable : true
		}, {
			header : "申请人",
			align : "center",
			dataIndex : "applicantname",
			sortable : true
		}, {
			header : "部门",
			dataIndex : "部门名称"
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
			header : "申请日期",
			dataIndex : "申请日期",
			sortable : true,
			renderer : customer_formatDate
		}, {
			header : '创建时间',
			width : 200,
			dataIndex : '创建时间',
			sortable : true,
			renderer : function(value, metaData, record) {
				if (value != null && value != '' && value.length > 10) {
					return value.substr(5, 11);
				} else {
					return value;
				}
			}
		}];

		this.sm = csm;
		this.tbar = new Ext.Toolbar({
					enableOverflow : true
				});
		this.tbar.add({
					text : '同意',
					iconCls : 'ok',
					tooltip : '同意',
					ref : 'btn_ok',
					handler : function() {
						com.bitc.xmis.workflow.DoApprovalTaskBySelectFunction(
								tPanel, 0);
					}
				}, {
					text : '不同意',
					iconCls : 'ok',
					tooltip : '不同意',
					ref : 'btn_no',
					handler : function() {
						com.bitc.xmis.workflow.DoApprovalTaskBySelectFunction(
								tPanel, 1);
					}
				});
		this.bbar = new Ext.PagingToolbar({
					pageSize : 20,
					store : this.store,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					items : this.bbar || []
				});

		com.bitc.xmis.workflow.ApprovalTaskPanel.superclass.initComponent
				.call(this);
	},
	reload : function(options) {
		this.getStore().reload(options);
	},
	onRender : function(a, b) {
		this.on('rowclick', function(grid, index) {
					var r = this.getStore().getAt(index);
					var s = r.get('script');
					if (com.bitc.xmis.workflow.TodoScript[s]) {
						com.bitc.xmis.workflow.TodoScript[s]
								.call(this, r, this);
					}

				}, this);
		com.bitc.xmis.workflow.TodoGridPanel.superclass.onRender.call(this, a,
				b);
	}

});