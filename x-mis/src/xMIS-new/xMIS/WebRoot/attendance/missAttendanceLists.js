Ext.onReady(function() {
	Ext.QuickTips.init();
	var paramArray = params.split(';');
	var todoids = paramArray[0];
	var isTodo = paramArray[1];
	if (isTodo == 'false') {
		isTodo = false;
	} else {
		isTodo = true;
	}
	var todoId = paramArray[2];
	var attendanceDatastore = new Ext.data.GroupingStore({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
							+ '/attdMaint.do?method=getMissAttendanceList&todoIds='
							+ todoids + '&isTodo=' + isTodo
							+ "&isOfficialTravel=1",
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
							"name" : "todoid"
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
		groupField : '部门名称',
		remoteSort : true
	});
	var businessDatastore = new Ext.data.GroupingStore({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
							+ '/attdMaint.do?method=getMissAttendanceList&todoIds='
							+ todoids + '&isTodo=' + isTodo
							+ "&isOfficialTravel=0",
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
							"name" : "todoid"
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
		groupField : '部门名称',
		remoteSort : true
	});

	var businessSm = new Ext.grid.CheckboxSelectionModel();
	var sm = new Ext.grid.CheckboxSelectionModel();
	var businessClm = new Ext.grid.ColumnModel([businessSm,
			new Ext.grid.RowNumberer(), {
				header : "ID",
				dataIndex : "ID",
				hidden : true,
				sortable : true
			}, {
				header : "申请人ID",
				dataIndex : "申请人ID",
				hidden : true,
				sortable : true
			}, {
				header : "workItemId",
				dataIndex : "workItemId",
				width : 200,
				hidden : true,
				sortable : true
			}, {
				header : "申请人",
				dataIndex : "applicantname",
				sortable : true
			}, {
				header : "部门",
				hidden : true,
				dataIndex : "部门名称"
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
				header : "审批意见",
				dataIndex : "审批人及审批时间",
				id : 'approveinfo',
				sortable : true
			}, {
				header : "申请日期",
				dataIndex : "申请日期",
				sortable : true,
				renderer : customer_formatDate
			}, {
				header : "操作人ID",
				dataIndex : "操作人ID",
				hidden : true,
				sortable : true
			}, {
				header : "代理人",
				dataIndex : "agentname",
				hidden : true,
				sortable : true
			}, {
				header : "流程实例ID",
				dataIndex : "流程实例ID",
				hidden : true,
				sortable : true
			}, {
				header : "考勤单ID",
				id : "考勤单ID",
				dataIndex : "考勤单ID",
				hidden : true,
				sortable : true
			}, {
				header : "todoid",
				dataIndex : "todoid",
				hidden : true,
				sortable : true
			}, {
				header : "任务人",
				dataIndex : "任务人",
				sortable : true
			}]);
	var attendanceClm = new Ext.grid.ColumnModel([sm,
			new Ext.grid.RowNumberer(), {
				header : "ID",
				dataIndex : "ID",
				hidden : true,
				sortable : true
			}, {
				header : "申请人ID",
				dataIndex : "申请人ID",
				hidden : true,
				sortable : true
			}, {
				header : "workItemId",
				dataIndex : "workItemId",
				width : 200,
				hidden : true,
				sortable : true
			}, {
				header : "申请人",
				dataIndex : "applicantname",
				sortable : true
			}, {
				header : "部门",
				hidden : true,
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
				header : "事由",
				dataIndex : "事由",
				sortable : true
			}, {
				header : "申请日期",
				dataIndex : "申请日期",
				sortable : true,
				renderer : customer_formatDate
			}, {
				header : "审批意见",
				dataIndex : "审批人及审批时间",
				id : 'approveinfo',
				sortable : true
			}, {
				header : "申请日期",
				dataIndex : "申请日期",
				sortable : true,
				renderer : customer_formatDate
			}, {
				header : "操作人ID",
				dataIndex : "操作人ID",
				hidden : true,
				sortable : true
			}, {
				header : "代理人",
				dataIndex : "agentname",
				hidden : true,
				sortable : true
			}, {
				header : "流程实例ID",
				dataIndex : "流程实例ID",
				hidden : true,
				sortable : true
			}, {
				header : "考勤单ID",
				id : "考勤单ID",
				dataIndex : "考勤单ID",
				hidden : true,
				sortable : true
			}, {
				header : "todoid",
				dataIndex : "todoid",
				hidden : true,
				sortable : true
			}, {
				header : "任务人",
				dataIndex : "任务人",
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
		view : new Ext.grid.GroupingView({
			forceFit : true,
			groupTextTpl : '{text} ({[values.rs.length]} {[values.rs.length >= 1? "条" : "条"]})'
		}),
		autoExpandColumn : 'approveinfo',
		tbar : [{
			ref : '../okBtn',
			iconCls : 'ok',
			text : '同意',
			handler : function() {
				var records = attendanceGrid.getSelectionModel()
						.getSelections();
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
				var attendancenum = records.length;
				var win = new Ext.Window({
					title : '请确定是否同意？',
					layout : 'border',
					width : 300,
					height : 180,
					modal : true,
					border : false,
					items : [new Ext.Panel({
						region : 'center',
						ref : 'p',
						frame : true,
						border : false,
						labelAlign : 'left',
						labelWidth : 80,
						bodyStyle : 'padding:10px',
						buttonAlign : 'center',
						fbar : [{
							text : '确定',
							handler : function() {
								var text = win.p.textID.getValue()
								Ext.Ajax.request({
									url : String
											.format(
													'{0}/attdMaint.do?method=doApprovalTask',
													__cxtPath),
									method : 'post',
									params : {
										isAgree : 0,
										comment : text,
										ids : ids
									},
									success : function(response) {
										var result = Ext
												.decode(response.responseText);
										if (result.success) {
											Ext.Msg.alert(result.messageTitle,
													result.message, function() {
														showMessageByNum(
																attendancenum,
																0);
														parent.xMis
																.refresh(
																		'mytodo',
																		'myworkitemnewtodo');
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
							ref : 'textID',
							width : 250,
							name : 'text'
						}]
					})]
				}).show();

			}
		}, {
			ref : '../noBtn',
			iconCls : 'ok',
			text : '不同意',
			// hidden : isTodo,
			handler : function() {
				var records = attendanceGrid.getSelectionModel()
						.getSelections();
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
				var attendancenum = records.length;
				var win = new Ext.Window({
					title : '请确定是否同意？',
					layout : 'border',
					width : 300,
					height : 180,
					modal : true,
					border : false,
					items : [new Ext.Panel({
						region : 'center',
						ref : 'p',
						frame : true,
						border : false,
						labelAlign : 'left',
						labelWidth : 80,
						bodyStyle : 'padding:10px',
						buttonAlign : 'center',
						fbar : [{
							text : '确定',
							handler : function() {
								var text = win.p.textID.getValue()
								if (text == '') {
									Ext.MessageBox.alert('提示',
											'不同意的原因不可为空，请填写原因！');
									return;
								}
								Ext.Ajax.request({
									url : String
											.format(
													'{0}/attdMaint.do?method=doApprovalTask',
													__cxtPath),
									method : 'post',
									params : {
										isAgree : 1,
										comment : text,
										ids : ids
									},
									success : function(response) {
										var result = Ext
												.decode(response.responseText);
										if (result.success) {
											Ext.Msg.alert(result.messageTitle,
													result.message, function() {
														showMessageByNum(
																attendancenum,
																0);
														parent.xMis
																.refresh(
																		'mytodo',
																		'myworkitemnewtodo');
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
							ref : 'textID',
							width : 250,
							name : 'text'
						}]
					})]
				}).show();
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
		sm : businessSm,
		// viewConfig : {
		// forceFit : true
		// },
		view : new Ext.grid.GroupingView({
			forceFit : true,
			groupTextTpl : '{text} ({[values.rs.length]} {[values.rs.length >= 1? "条" : "条"]})'
		}),
		autoExpandColumn : 'approveinfo',
		tbar : [{
			ref : '../okBtn',
			iconCls : 'ok',
			text : '同意',
			handler : function() {
				var records = businessGrid.getSelectionModel().getSelections();
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
				var busnnum = records.length;
				var win = new Ext.Window({
					title : '请确定是否同意？',
					layout : 'border',
					width : 300,
					height : 180,
					modal : true,
					border : false,
					items : [new Ext.Panel({
						region : 'center',
						ref : 'p',
						frame : true,
						border : false,
						labelAlign : 'left',
						labelWidth : 80,
						bodyStyle : 'padding:10px',
						buttonAlign : 'center',
						fbar : [{
							text : '确定',
							handler : function() {
								var text = win.p.textID.getValue()
								Ext.Ajax.request({
									url : String
											.format(
													'{0}/attdMaint.do?method=doApprovalTask',
													__cxtPath),
									method : 'post',
									params : {
										isAgree : 0,
										comment : text,
										ids : ids
									},
									success : function(response) {
										var result = Ext
												.decode(response.responseText);
										if (result.success) {
											Ext.Msg.alert(result.messageTitle,
													result.message, function() {
														showMessageByNum(0,
																busnnum);
														parent.xMis
																.refresh(
																		'mytodo',
																		'myworkitemnewtodo');
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
							ref : 'textID',
							width : 250,
							name : 'text'
						}]
					})]
				}).show();
			}
		}, {
			ref : '../noBtn',
			iconCls : 'ok',
			text : '不同意',
			handler : function() {
				var records = businessGrid.getSelectionModel().getSelections();
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
				var busnnum = records.length;

				var win = new Ext.Window({
					title : '请确定是否同意？',
					layout : 'border',
					width : 300,
					height : 180,
					modal : true,
					border : false,
					items : [new Ext.Panel({
						region : 'center',
						ref : 'p',
						frame : true,
						border : false,
						labelAlign : 'left',
						labelWidth : 80,
						bodyStyle : 'padding:10px',
						buttonAlign : 'center',
						fbar : [{
							text : '确定',
							handler : function() {
								var text = win.p.textID.getValue()
								if (text == '') {
									Ext.MessageBox.alert('提示',
											'不同意的原因不可为空，请填写原因！');
									return;
								}
								Ext.Ajax.request({
									url : String
											.format(
													'{0}/attdMaint.do?method=doApprovalTask',
													__cxtPath),
									method : 'post',
									params : {
										isAgree : 1,
										comment : text,
										ids : ids
									},
									success : function(response) {
										var result = Ext
												.decode(response.responseText);
										if (result.success) {
											Ext.Msg.alert(result.messageTitle,
													result.message, function() {
														showMessageByNum(0,
																busnnum);
														parent.xMis
																.refresh(
																		'mytodo',
																		'myworkitemnewtodo');
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
							ref : 'textID',
							width : 250,
							name : 'text'
						}]
					})]
				}).show();
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

	function showMessageByNum(attendancenum, busnnum) {

		attendanceGrid.store.reload();
		businessGrid.store.reload();

		var attendanceGridStore = attendanceGrid.store.data;

		var businessGridStore = businessGrid.store.data;
		var attdnum = attendanceGridStore.length - attendancenum;
		var busnum = businessGridStore.length - busnnum;
		var message = "";
		if (attdnum > 0) {
			message = '[考勤补报]有' + attdnum + '条待审批<br>';
		}
		if (busnum > 0) {
			message += '[出差补报]有' + busnum + '条待审批';
		}
		if (attdnum + busnum > 0) {
			Ext.MessageBox.alert('提示', message);
		}
		if (attdnum + busnum == 0) {
			Ext.Ajax.request({
						url : String
								.format(
										'{0}/attdMaint.do?method=deleteTodoCountByTodoId',
										__cxtPath),
						method : 'post',
						params : {
							todoId : todoId
						},
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								Ext.Msg.alert(result.messageTitle,
										result.message, function() {
											parent.xMis.refresh('mytodo',
													'myworkitemnewtodo');
											parent.xMis.closeTab(window);
										});
							} else {
								Ext.Msg.alert(result.messageTitle,
										result.message)
							}
						},
						failure : function(response) {
							Ext.Msg.alert('失败', '操作失败');
						}
					});

		}
	}
	sm.addListener('rowselect', function(sm, rowIndex, record) {
				var store = attendanceGrid.store;
				var workItemIdSelected = record.get('workItemId');// 选中的数据的workItemId
				var recordsArr = [];
				for (var i = 0; i < store.data.length; i++) {
					if (store.data.items[i].data.workItemId == workItemIdSelected) {
						var id = store.data.items[i].id;
						var newSelected = store.getById(id);// store中workItemId与被选中的workItemId相等的数据
						recordsArr.push(newSelected);
					}
				}
				sm.selectRecords(recordsArr, true);// 将recordsArr数据选中

			});
	sm.addListener('rowdeselect', function(sm, rowIndex, record) {
				var store = attendanceGrid.store;
				var selectedRecords = attendanceGrid.getSelectionModel()
						.getSelections();
				var workItemIdSelected = record.get('workItemId');// 选中的数据的workItemId
				var recordsArr = [];
				for (var i = 0; i < selectedRecords.length; i++) {
					var id = selectedRecords[i].id;
					var selected = store.getById(id);// selectedRecords中workItemId与被选中的workItemId相等的数据
					recordsArr.push(selected);
					if (selectedRecords[i].data.workItemId == workItemIdSelected) {
						var id = selectedRecords[i].id;
						var newDelSelected = store.getById(id);// selectedRecords中workItemId与被选中的workItemId相等的数据
						recordsArr.remove(newDelSelected);
					}
				}
				sm.selectRecords(recordsArr, false);// 将recordsArr数据选中
			});
	businessSm.addListener('rowselect', function(businessSm, rowIndex, record) {
				var store = businessGrid.store;
				var workItemIdSelected = record.get('workItemId');// 选中的数据的workItemId
				var recordsArr = [];
				for (var i = 0; i < store.data.length; i++) {
					if (store.data.items[i].data.workItemId == workItemIdSelected) {
						var id = store.data.items[i].id;
						var newSelected = store.getById(id);// store中workItemId与被选中的workItemId相等的数据
						recordsArr.push(newSelected);
					}
				}
				businessSm.selectRecords(recordsArr, true);// 将recordsArr数据选中

			});
	businessSm.addListener('rowdeselect',
			function(businessSm, rowIndex, record) {
				var store = businessGrid.store;
				var selectedRecords = businessGrid.getSelectionModel()
						.getSelections();
				var workItemIdSelected = record.get('workItemId');// 选中的数据的workItemId
				var recordsArr = [];
				for (var i = 0; i < selectedRecords.length; i++) {
					var id = selectedRecords[i].id;
					var selected = store.getById(id);// selectedRecords中workItemId与被选中的workItemId相等的数据
					recordsArr.push(selected);
					if (selectedRecords[i].data.workItemId == workItemIdSelected) {
						var id = selectedRecords[i].id;
						var newDelSelected = store.getById(id);// selectedRecords中workItemId与被选中的workItemId相等的数据
						recordsArr.remove(newDelSelected);
					}
				}
				businessSm.selectRecords(recordsArr, false);// 将recordsArr数据选中
			});

	var tabs = new Ext.TabPanel({
				height : 535,
				region : 'center',
				margins : '3 3 3 0',
				activeTab : 0,
				tabPosition : 'top',
				items : [{
							id : 'attendanceID',
							title : '考勤补报',
							layout : 'border',
							items : [attendanceGrid]
						}, {
							id : 'businessID',
							title : '出差补报',
							layout : 'border',
							items : [businessGrid]
						}]
			});
	var main = new Ext.Viewport({
				layout : 'border',
				items : tabs,
				border : false
			});
});
