Ext.onReady(function() {
	Ext.QuickTips.init();
	var attendanceDatastore = new Ext.data.GroupingStore({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/attdMaint.do?method=getMissAttendanceListByQuery&isOfficialTravel=1',
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
							"name" : "事业部"
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
							"name" : "是否被处理"
						}, {
							"name" : "审批人及审批时间"
						}, {
							"name" : "操作人ID",
							"type" : "int"
						}, {
							"name" : "agentname"
						}, {
							"name" : "流程实例ID"
						}, {
							"name" : "考勤单ID"
						}])),
		groupField : '事业部',
		sortInfo : {
			field : "申请日期",
			direction : "DESC"
		},
		autoLoad : true,
		remoteSort : true,
		listeners : {
			load : function(store) {
				var index = 0;
				var array = new Array();
				store.each(function(record) {
							if (record.data.是否被处理 == '1') {
								array.push(index);
							}
							index++;
						})
				sm.selectRows(array);
			}
		}
	});
	var businessDatastore = new Ext.data.GroupingStore({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/attdMaint.do?method=getMissAttendanceListByQuery&isOfficialTravel=0',
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
							"name" : "事业部"
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
							"name" : "是否被处理"
						}, {
							"name" : "审批人及审批时间"
						}, {
							"name" : "操作人ID",
							"type" : "int"
						}, {
							"name" : "agentname"
						}, {
							"name" : "流程实例ID"
						}, {
							"name" : "考勤单ID"
						}])),
		sortInfo : {
			field : "申请日期",
			direction : "DESC"
		},
		groupField : '事业部',
		remoteSort : true,
		listeners : {
			load : function(store) {
				var index = 0;
				var array = new Array();
				store.each(function(record) {
							if (record.data.是否被处理 == '1') {
								array.push(index);
							}
							index++;
						})
				bsm.selectRows(array);
			}
		}
	});

	var bsm = new Ext.grid.CheckboxSelectionModel({
				checkOnly : true,
				header : ''
			});

	var businessClm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({width:40}),
			bsm, {
				header : "ID",
				dataIndex : "ID",
				hidden : true,
				sortable : true
			}, {
				header : "是否被处理",
				dataIndex : "是否被处理",
				hidden : true,
				sortable : true
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
				header : "事业部名称",
				dataIndex : "事业部"

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
				header : "审批人及审批时间",
				dataIndex : "审批人及审批时间",
				id : 'approveinfo2',
				width : 180,
				sortable : true,
				renderer : function(value, metaData, record) {
					var processInstanceId = "'" + record.data.流程实例ID + "'";
					return '<a  href="javascript:;"  onclick="com.bitc.xmis.workflow.showTraceWin('
							+ processInstanceId + ');">' + value + '</a>';
				}
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
			}]);
	var sm = new Ext.grid.CheckboxSelectionModel({
				checkOnly : true,
				header : ''
			});

	var attendanceClm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({width:40}),
			sm, {
				header : "ID",
				dataIndex : "ID",
				hidden : true,
				sortable : true
			}, {
				header : "是否被处理",
				dataIndex : "是否被处理",
				hidden : true,
				sortable : true
			}, {
				header : "申请人ID",
				dataIndex : "申请人ID",
				hidden : true,
				sortable : true
			}, {
				header : "申请人",
				dataIndex : "applicantname",
				sortable : true
			}, {
				header : "部门",
				dataIndex : "部门名称"
			}, {
				header : "事业部名称",
				dataIndex : "事业部"

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
				header : "审批人及审批时间",
				dataIndex : "审批人及审批时间",
				id : 'approveinfo',
				width : 180,
				sortable : true,
				renderer : function(value, metaData, record) {
					var processInstanceId = "'" + record.data.流程实例ID + "'";
					return '<a  href="javascript:;"  onclick="com.bitc.xmis.workflow.showTraceWin('
							+ processInstanceId + ');">' + value + '</a>';
				}
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
				width:300,
				hidden : true,
				sortable : true
			}, {
				header : "考勤单ID",
				id : "考勤单ID",
				dataIndex : "考勤单ID",
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
		// stripeRows : true,
		store : attendanceDatastore,
		cm : attendanceClm,
		sm : sm,
		// viewConfig : {
		// forceFit : true
		// },
		view : new Ext.grid.GroupingView({
			forceFit : true,
			startCollapsed : true,
			groupOnSort : true,
			groupTextTpl : '{text} ({[values.rs.length]}{[values.rs.length >= 1 ? "条" : "条"]})'
		}),
		autoExpandColumn : 'approveinfo',
		tbar : [
				// '模糊查询：', new Ext.ux.InklingSearchField({
				// width : 120,
				// store : attendanceDatastore,
				// tooltip : {
				// title : '模糊查询',
				// text : '您可以输入 员工、或部门名称中的任意一个关键词进行查询'
				// },
				// inklings : [{
				// name : 'applicantname',
				// op : 'like',
				// type : 'string'
				// }, {
				// name : '部门名称',
				// op : 'like',
				// type : 'string'
				//
				// }]
				// }),
				new com.bitc.xmis.Combos.DivisionDeptCombo({
							width : 150,
							COMPANYID : 1,
							ref : '../division_combo',
							listeners : {
								'select' : function(combo, record, index) {
									var selValue = record.get('id');
									attendanceDatastore.setBaseParam(
											'divisionID', selValue);
									attendanceDatastore.load({
												params : {
													start : 0,
													limit : 0
												}
											});

								}
							}
						}), {
					xtype : 'combo',
					displayField : 'text',
					valueField : 'value',
					mode : 'local',
					triggerAction : 'all',
					editable : false,
					value : 2014,
					ref : '../yearID',
					width : 100,
					store : new Ext.data.ArrayStore({
								fields : ['value', 'text'],
								data : [[2013, '2013年'], [2014, '2014年']]
							}),
					listeners : {
						'select' : function(combo, record, index) {
							var selValue = record.data.value;
							var store = attendanceDatastore;
							store.setBaseParam('yearID', selValue);
							// store.load({
							// params : {
							// start : 0
							// }
							// });

						}
					}

				}, '考勤月：', {
					xtype : 'combo',
					displayField : 'text',
					valueField : 'value',
					mode : 'local',
					triggerAction : 'all',
					editable : false,
					ref : '../monthID',
					value : Ext.util.Format.date(new Date(), "m"),
					width : 100,
					store : new Ext.data.ArrayStore({
								fields : ['value', 'text'],
								data : [[1, '1'], [2, '2'], [3, '3'], [4, '4'],
										[5, '5'], [6, '6'], [7, '7'], [8, '8'],
										[9, '9'], [10, '10'], [11, '11'],
										[12, '12']]
							}),
					listeners : {
						'select' : function(combo, record, index) {
							var selValue = record.data.value;
							var store = attendanceDatastore;
							store.setBaseParam('monthID', selValue);
							// store.load({
							// params : {
							// start : 0
							// }
							// });

						}
					}

				}
				// , {
				// xtype : 'combo',
				// id : 'isAgent',
				// displayField : 'text',
				// valueField : 'value',
				// mode : 'local',
				// triggerAction : 'all',
				// editable : false,
				// value : 0,
				// width : 100,
				// store : new Ext.data.ArrayStore({
				// fields : ['value', 'text'],
				// data : [[0, '缺勤月份'], [1, '1'],
				// [2, '2'], [3, '3'], [4, '4'],
				// [5, '5'], [6, '6'], [7, '7'],
				// [8, '8'], [9, '9'], [10, '10'],
				// [11, '11'], [12, '12']]
				// }),
				// listeners : {
				// 'select' : function(combo, record, index) {
				// var selValue = record.data.value;
				// if (selValue != 0) {
				// attendanceDatastore.setBaseParam(
				// 'monthID', selValue);
				// }
				// attendanceDatastore.load({
				// params : {
				// start : 0
				// }
				// });
				//
				// }
				// }
				//
				// }
				// ,
				// '人名或部门查询：', {
				// xtype : 'hidden',
				// name : 'searchColumn',
				// ref : '../searchColumn',
				// value : 'applicantname,部门名称'
				// }, new Ext.form.TextField({
				// width : 100,
				// name : 'searchParam',
				// ref : '../searchParam',
				// emptyText : '请输入查询人名',
				// listeners : {
				// 'render' : function(c) {
				// Ext.QuickTips.register({
				// target : c.getEl(),
				// text : '您可以输入人名进行查询'
				// });
				// }
				// }
				// })
				// , '&nbsp;开始日期：', new Ext.form.DateField({
				// ref : '../startDate',
				// format : 'Y-m-d',
				// minValue : '2013-01-01',
				// showToday : true,
				// value : new Date(new Date().getFullYear(), 0, 1),
				// anchor : '100%'
				// }), '&nbsp;结束日期：', new Ext.form.DateField({
				// ref : '../endDate',
				// format : 'Y-m-d',
				// minValue : '2013-01-01',
				// showToday : true,
				// value : new Date(),
				// anchor : '100%'
				// }), '-'
				, {
					ref : '../searchBtn',
					iconCls : 'searchIcon',
					text : '查询',
					handler : function(b, e) {
						var grid = attendanceGrid;
						var yearId = grid.yearID.getValue();
						var selValue = grid.monthID.getValue();
						grid.store.setBaseParam('yearID', yearId);
						grid.store.setBaseParam('monthID', selValue);
						grid.store.reload({
									params : {
										start : 0,
										limit : 0
									}
								});
						// if (!dateCompare(grid.startDate.getValue(),
						// grid.endDate.getValue())) {
						// Ext.Msg.alert('提示', '结束日期必须晚于开始日期！');
						// } else {
						// var start = Ext.util.Format.date(grid.startDate
						// .getValue(), 'Y-m-d');
						// var end = Ext.util.Format.date(grid.endDate
						// .getValue(), 'Y-m-d');
						// grid.store.setBaseParam('begDate', start);
						// grid.store.setBaseParam('endDate', end);
						// grid.store.load({
						// params : {
						// start : 0
						// }
						// });
						// }

					}
				}],
		bbar : new Ext.PagingToolbar({
			pageSize : 1000,
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
			})//, new Ext.ux.PagelimitCombo()
			]
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
		sm : bsm,
		// viewConfig : {
		// forceFit : true
		// },
		view : new Ext.grid.GroupingView({
			forceFit : true,
			startCollapsed : true,
			groupTextTpl : '{text} ({[values.rs.length]}{[values.rs.length >= 1 ? "条" : "条"]})'
		}),
		autoExpandColumn : 'approveinfo2',
		// tbar : ['模糊查询：', new Ext.ux.InklingSearchField({
		// width : 120,
		// store : businessDatastore,
		// tooltip : {
		// title : '模糊查询',
		// text : '您可以输入 中的任意一个关键词进行查询'
		// },
		// inklings : [{
		// name : 'applicantname',
		// op : 'like',
		// type : 'string'
		// }, {
		// name : '部门名称',
		// op : 'like',
		// type : 'string'
		//
		// }]
		// })],
		tbar : [
				// '模糊查询：', new Ext.ux.InklingSearchField({
				// width : 120,
				// store : businessDatastore,
				// tooltip : {
				// title : '模糊查询',
				// text : '您可以输入 员工、或部门名称中的任意一个关键词进行查询'
				// },
				// inklings : [{
				// name : 'applicantname',
				// op : 'like',
				// type : 'string'
				// }, {
				// name : '部门名称',
				// op : 'like',
				// type : 'string'
				//
				// }]
				// }),
				new com.bitc.xmis.Combos.DivisionDeptCombo({
							width : 150,
							COMPANYID : 1,
							ref : '../division_combo',
							listeners : {
								'select' : function(combo, record, index) {
									var selValue = record.get('id');
									var store = businessDatastore;
									businessDatastore.setBaseParam(
											'divisionID', selValue);
									businessDatastore.load({
												params : {
													start : 0,
													limit : 0
												}
											});

								}
							}
						}), {
					xtype : 'combo',
					displayField : 'text',
					valueField : 'value',
					mode : 'local',
					triggerAction : 'all',
					editable : false,
					value : 2013,
					ref : '../yearID',
					width : 100,
					store : new Ext.data.ArrayStore({
								fields : ['value', 'text'],
								data : [[2013, '2013年'], [2014, '2014年']]
							}),
					listeners : {
						'select' : function(combo, record, index) {
							var selValue = record.data.value;
							var store = businessDatastore;
							store.setBaseParam('yearID', selValue);
							// store.load({
							// params : {
							// start : 0
							// }
							// });

						}
					}

				}, '考勤月：', {
					xtype : 'combo',
					displayField : 'text',
					valueField : 'value',
					mode : 'local',
					triggerAction : 'all',
					editable : false,
					ref : '../monthID',
					value : Ext.util.Format.date(new Date(), "m"),
					width : 100,
					store : new Ext.data.ArrayStore({
								fields : ['value', 'text'],
								data : [[1, '1'], [2, '2'], [3, '3'], [4, '4'],
										[5, '5'], [6, '6'], [7, '7'], [8, '8'],
										[9, '9'], [10, '10'], [11, '11'],
										[12, '12']]
							}),
					listeners : {
						'select' : function(combo, record, index) {
							var selValue = record.data.value;
							if (selValue < 10) {
								selValue = "0" + selValue;
							}
							var store = businessDatastore;
							store.setBaseParam('monthID', selValue);
							// store.load({
							// params : {
							// start : 0
							// }
							// });

						}
					}

				}
				// ,
				// '人名或部门查询：', {
				// xtype : 'hidden',
				// name : 'searchColumn',
				// ref : '../searchColumn',
				// value : 'applicantname,部门名称'
				// }, new Ext.form.TextField({
				// width : 100,
				// name : 'searchParam',
				// ref : '../searchParam',
				// emptyText : '请输入查询人名',
				// listeners : {
				// 'render' : function(c) {
				// Ext.QuickTips.register({
				// target : c.getEl(),
				// text : '您可以输入人名进行查询'
				// });
				// }
				// }
				// })
				// , '&nbsp;开始日期：', new Ext.form.DateField({
				// ref : '../startDate',
				// format : 'Y-m-d',
				// minValue : '2013-01-01',
				// showToday : true,
				// value : new Date(new Date().getFullYear(),
				// 0, 1),
				// anchor : '100%'
				// }), '&nbsp;结束日期：', new Ext.form.DateField({
				// ref : '../endDate',
				// format : 'Y-m-d',
				// minValue : '2013-01-01',
				// showToday : true,
				// value : new Date(),
				// anchor : '100%'
				// }), '-'
				, {
					ref : '../searchBtn',
					iconCls : 'searchIcon',
					text : '查询',
					handler : function(b, e) {
						var grid = businessGrid;
						var yearId = grid.yearID.getValue();
						var selValue = grid.monthID.getValue();
						grid.store.setBaseParam('yearID', yearId);
						grid.store.setBaseParam('monthID', selValue);
						grid.store.reload({
									params : {
										start : 0,
										limit : 0
									}
								});
						// if (!dateCompare(grid.startDate.getValue(),
						// grid.endDate.getValue())) {
						// Ext.Msg.alert('提示', '结束日期必须晚于开始日期！');
						// } else {

					}
				}],
		bbar : new Ext.PagingToolbar({
			pageSize : 1000,
			store : businessDatastore,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : '无显示数据',
			plugins : [new Ext.ux.Export({
						exportname : '出差补报',
						formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.RowNumber'
								// }, {
								// columnHeader : '',
								// formatClass :
								// 'com.bitc.jk.xmis.export.CheckBoxSelecttionFormatNull'
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
					})//, new Ext.ux.PagelimitCombo()
					]
		})

	});

	sm.addListener('rowselect', function(sm, rowIndex, record) {
				var grid = attendanceGrid;
				var year = grid.yearID.getValue();
				var month = grid.monthID.getValue();
				var id = record.data.ID;
				var url = __cxtPath
						+ '/attdMaint.do?method=doUpdateMissAttendanceRecordState';
				Ext.Ajax.request({
							url : url,
							method : 'post',
							params : {
								infoId : id,
								state : 1,
								year : year,
								month : month
							},
							success : function(response, opts) {
								var obj = Ext.decode(response.responseText);
								// Ext.Msg.alert(obj.messageTitle, obj.message);
							},
							failure : function(response, opts) {
								Ext.Msg.alert("提示", "操作失败，请联系管理员");
							}
						});
			});
	sm.addListener('rowdeselect', function(sm, rowIndex, record) {
		var recordsArr = [record];
		sm.selectRecords(recordsArr, true);
			// var grid = attendanceGrid;
			// var year = grid.yearID.getValue();
			// var month = grid.monthID.getValue();
			// var id = record.data.ID;
			// var url = __cxtPath
			// + '/attdMaint.do?method=doUpdateMissAttendanceRecordState';
			// Ext.Ajax.request({
			// url : url,
			// method : 'post',
			// params : {
			// infoId : id,
			// state : 0,
			// year : year,
			// month : month
			// },
			// success : function(response, opts) {
			// var obj = Ext.decode(response.responseText);
			// // Ext.Msg.alert(obj.messageTitle, obj.message);
			// },
			// failure : function(response, opts) {
			// Ext.Msg.alert("提示", "操作失败，请联系管理员");
			// }
			// });
		});
	bsm.addListener('rowselect', function(sm, rowIndex, record) {
				var grid = businessGrid;
				var year = grid.yearID.getValue();
				var month = grid.monthID.getValue();
				var id = record.data.ID;
				var url = __cxtPath
						+ '/attdMaint.do?method=doUpdateMissAttendanceRecordState';
				Ext.Ajax.request({
							url : url,
							method : 'post',
							params : {
								infoId : id,
								state : 1,
								year : year,
								month : month
							},
							success : function(response, opts) {
								var obj = Ext.decode(response.responseText);
								// Ext.Msg.alert(obj.messageTitle, obj.message);
							},
							failure : function(response, opts) {
								Ext.Msg.alert("提示", "操作失败，请联系管理员");
							}
						});
			});
	bsm.addListener('rowdeselect', function(sm, rowIndex, record) {
		var recordsArr = [record];
		sm.selectRecords(recordsArr, true);
			// var grid = businessGrid;
			// var year = grid.yearID.getValue();
			// var month = grid.monthID.getValue();
			// var id = record.data.ID;
			// var url = __cxtPath
			// + '/attdMaint.do?method=doUpdateMissAttendanceRecordState';
			// Ext.Ajax.request({
			// url : url,
			// method : 'post',
			// params : {
			// infoId : id,
			// state : 0,
			// year : year,
			// month : month
			// },
			// success : function(response, opts) {
			// var obj = Ext.decode(response.responseText);
			// // Ext.Msg.alert(obj.messageTitle, obj.message);
			// },
			// failure : function(response, opts) {
			// Ext.Msg.alert("提示", "操作失败，请联系管理员");
			// }
			// });
		});
	var tabFlag = true;
	var tabs = new Ext.TabPanel({
				height : 535,
				region : 'center',
				margins : '3 3 3 0',
				activeTab : 0,
				tabPosition : 'top',
				listeners : {
					'tabchange' : function(a, activeTab) {
						if (activeTab.getItemId() == 'businessID' && tabFlag) {
							businessDatastore.load({
										params : {
											start : 0,
											// limit : outPageSize
											limit : 0
										},
										callback : function(record, options,
												success) {
											if (success == true) {
												tabFlag = false;
											} else {
												Ext.Msg.alert("出错了", "加载数据错误");
											}
										}
									});
							tabs.removeListener('tabchange');
						}
					}
				},
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
