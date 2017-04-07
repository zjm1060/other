Ext.ns('com.bitc.xmis.workflow');
/**
 * @class com.bitc.xmis.TodoScript 待办脚本
 * @singleton
 */
com.bitc.xmis.workflow.TodoScript = function() {
	var infoTpl = new Ext.XTemplate(
			'<table style="width:100%" class="info">',
			'<tr class="tr1"><td>任务名称：</td><td>{text}</td></tr>',
			'<tr class="tr2"><td>任务发送人：</td><td>{senderName}</td></tr>',
			'<tr class="tr2"><td>任务到达时间：</td><td>{[values.createTime.substr(0,16)]}</td></tr>',
			'</table>');
	/**
	 * var infoTpl = new Ext.XTemplate( '<table style="width:100%"
	 * class="info">', '
	 * <tr class="tr1">
	 * <td>任务名称：</td>
	 * <td>{[values.text.substring(1,values.text.indexOf("]")).replace("】[","-")]}</td>
	 * </tr>', '
	 * <tr class="tr1">
	 * <td>事由：</td>
	 * <td>{[values.text.substring(values.text.indexOf("]")+1,values.text.length)]}</td>
	 * </tr>', // '
	 * <tr class="tr1">
	 * <td>任务名称：</td>
	 * <td>{text}</td>
	 * </tr>', '
	 * <tr class="tr2">
	 * <td>任务发送人：</td>
	 * <td>{[values.sender.userName]}</td>
	 * </tr>', '
	 * <tr class="tr2">
	 * <td>任务到达时间：</td>
	 * <td>{createTime}</td>
	 * </tr>', '</table>');
	 */
	return {
		/**
		 * @param {Ext.data.Record}
		 *            todoRecord 待办记录
		 */
		showTaskInfo : function(todoRecord) {
			var todoId = todoRecord.get('id');
			var workItemId = todoRecord.get('appId');
			var url = String
					.format(
							'{0}/workflow.do?method=openForm&workItemId={1}&todoId={2}',
							__cxtPath, workItemId, todoId);
			parent.xMis.turn('wf-openform-' + workItemId, todoRecord
							.get('text'), url, true);
//		
//			Ext.Msg.show({
//				icon : Ext.Msg.INFO,
//				msg : infoTpl.apply(todoRecord.data),
//				buttons : {
//					ok : '签收任务',
////					no : '拒收任务',
//					cancel : '取消'
//				},
//				fn : function(buttonId) {
//					if (buttonId == 'ok') {
//						com.bitc.xmis.workflow.TodoActions['wf-task'][0].handler
//								.call(window, null, todoRecord, null);
//					} else if (buttonId == 'no') {
//						com.bitc.xmis.workflow.TodoActions['wf-task'][1].handler
//								.call(window, null, todoRecord, null);
//					}
//				},
//				title : "任务信息"
//			});
		},
		showTaskList : function(record) {
			var workItemId = record.get('appId');
			var url = __cxtPath + '/attendance/missAttendanceList.jsp?params='
					+ workItemId + ',true';
			parent.xMis.turn('wf-openform-', record.get('text'), url, 'task',
					true);

		},
		showTaskLists : function(record) {
			var url = __cxtPath + record.get('url') + ";true" + ";"
					+ record.get('id');
			parent.xMis.turn('wf-openform-', record.get('text'), url, 'task',
					true);
		},

		/**
		 * @param {Ext.data.Record}
		 *            todoRecord 待办记录
		 */
		doTask : function(todoRecord) {
			var todoId = todoRecord.get('id');
			var workItemId = todoRecord.get('appId');
			var url = String
					.format(
							'{0}/workflow.do?method=openForm&workItemId={1}&todoId={2}',
							__cxtPath, workItemId, todoId);
			parent.xMis.turn('wf-openform-' + workItemId, todoRecord
							.get('text'), url, true);
		},
		doTaskList : function(record) {
			var workItemId = record.get('appId');
			var url = __cxtPath + '/attendance/missAttendanceList.jsp?params='
					+ workItemId + ',false';
			parent.xMis.turn('wf-openform-', record.get('text'), url, 'task',
					true);
		},
		doTaskLists : function(record) {
			var url = __cxtPath + record.get('url') + ";false;"
					+ record.get('id');
			parent.xMis.turn('wf-openform-', record.get('text'), url, 'task',
					true);
		}
	};
}();
/**
 * @class com.bitc.xmis.workflow.TodoActions
 * @singleton
 */
com.bitc.xmis.workflow.TodoActions = function() {
	return {
		getActions : function(todo) {
			if (todo.model == 'wf-task' && todo.state != 0) {
				return [];
			} else {
				return com.bitc.xmis.workflow.TodoActions[todo.model];
			}
		}
//		,
//		'wf-task' : [{
//			cls : 'ok',
//			handler : function(listview, record, value) {
//				var url = String.format('{0}{1}', __cxtPath, record.get('url'));
//				var todoId = record.get('id');
//				var workItemId = record.get('appId');
//				Ext.Ajax.request({
//					url : url,
//					success : function(response, opts) {
//						var obj = Ext.decode(response.responseText);
//						Ext.Msg.alert(obj.messageTitle, obj.message);// 签收成功提示
//						if (obj.success) {
//							parent.xMis.refresh('newtodo', 'mytodo');
//							var url = String
//									.format(
//											'{0}/workflow.do?method=openForm&workItemId={1}&todoId={2}',
//											__cxtPath, workItemId, todoId);
//							parent.xMis.turn('wf-openform-' + workItemId,
//									record.get('text'), url);
//						} else {
//
//						}
//					},
//					failure : function(response, opts) {
//						Ext.Msg.alert('签收失败', '服务器错误，状态码：' + response.status);
//					}
//				});
//			},
//			qtip : '签收任务'
//		}, {
//			cls : 'delete',
//			qtip : '拒收任务',
//			handler : function(listview, record, value) {
//				Ext.Msg.prompt('拒收原因', '请填写拒收任务的原因：', function(btn, text) {
//							if (btn == 'ok') {
//								var url = __cxtPath
//										+ '/workflow.do?method=reject';
//								Ext.Ajax.request({
//											url : url,
//											method : 'post',
//											params : {
//												workItemId : record
//														.get('appId'),
//												comments : text
//											},
//											success : function(response, opts) {
//												var obj = Ext
//														.decode(response.responseText);
//												Ext.Msg.alert(obj.messageTitle,
//														obj.message);
//												if (obj.success) {
//													xMis.refresh('newtodo');
//												} else {
//
//												}
//											},
//											failure : function(response, opts) {
//												Ext.Msg
//														.alert(
//																'拒收收失败',
//																'服务器错误，状态码：'
//																		+ response.status);
//											}
//										});
//							}
//						}, this, true);
//
//			}
//		}]
	};
}();
com.bitc.xmis.workflow.TodoStateTemplate = new Ext.XTemplate(
		'{[this.getState(values.state)]}', {
			getState : function(v) {
				switch (v) {
					case 0 :
						return '待签收';
					default :
						return '';
				}
			}
		});
/**
 * @class com.bitc.xmis.workflow.TodoList
 * @extends Ext.list.ListView 用户待办列表
 */
com.bitc.xmis.workflow.TodoList = Ext.extend(Ext.list.ListView, {
	/**
	 * @cfg {Number} 列表显示的条数，如果要显示所有，设置为-1
	 */
	listSize : 10,
	hideAction : false,
	hideHeaders : true,
	emptyText : '您暂时没有待办任务',
	initComponent : function() {
		var listview = this;
		this.store = new Ext.data.JsonStore({
			url : String
					.format('{0}/workflow.do?method=getTodoListSQL', __cxtPath),
			root : 'rows',
			baseParams : {
				state : Ext.isNumber(this.state) ? this.state : '',
				start : 0,
				limit : this.listSize
			},
			fields : ['id', 'model', 'appId', 'text', 'url', 'createTime',
					'userID', 'script', 'state', 'senderID','senderName', 'senderTime','actorName'],
			sortInfo : {
				field : 'createTime',
				direction : 'DESC'
			},
			autoLoad : true
				/**
				 * , listeners : { scope : this, 'load' : function(s, records,
				 * options) { if (this.getStore().data.length) { if
				 * (this.showHeight) { this.setHeight(100); } else {
				 * this.setHeight(ht - 10); } } } }
				 */
			});
		this.columns = [{
					header : '标题',
					dataIndex : 'text'
				}, {
					width : .1,
					header : '状态',
					dataIndex : 'state',
					tpl : com.bitc.xmis.workflow.TodoStateTemplate
				}];
		if (this.hideAction) {
			this.columns.push({
						width : .2,
						header : '日期',
						dataIndex : 'createTime',
						align : 'center',
						tpl : '{[values.createTime.substr(5,5)]}'
					});
		} else {
			var actioncol = new Ext.ux.list.ActionColumn({
				width : .2,
				header : '日期',
				dataIndex : 'createTime',
				tpl : '{[values.createTime.substr(5,5)]}',
				getActionsFun : function(value) {
					return com.bitc.xmis.workflow.TodoActions.getActions(value);
				}
			});
			this.columns.push(actioncol);
			this.plugins = actioncol;
		}
		com.bitc.xmis.workflow.TodoList.superclass.initComponent.call(this);
	},
	onRender : function(a, b) {
		this.on('click', function(listview, index) {
					var r = this.getStore().getAt(index);
					var s = r.get('script');
					if (com.bitc.xmis.workflow.TodoScript[s]) {
						com.bitc.xmis.workflow.TodoScript[s]
								.call(this, r, this);
					}
				}, this);
		com.bitc.xmis.workflow.TodoList.superclass.onRender.call(this, a, b);
	},
	reload : function(options) {
		this.getStore().reload(options);
	}
});

/**
 * @class com.bitc.xmis.workflow.TodoGridPanel
 * @extends Ext.list.ListView 用户待办列表
 */
com.bitc.xmis.workflow.TodoGridPanel = Ext.extend(Ext.grid.GridPanel, {
	/**
	 * @cfg {Number} 列表显示的条数，如果要显示所有，设置为-1
	 */
	autoExpandColumn : 'text_id',
	autoLoadData:true,
	initComponent : function() {
		var p = this;
		var reRecord = Ext.data.Record.create([{
					"name" : "id",
					"type" : "string"
				}, {
					"name" : "model",
					"type" : "string"
				}, {
					"name" : "appId",
					"type" : "string"
				}, {
					"name" : "text",
					"type" : "string"
				}, {
					"name" : "url",
					"type" : "string"
				}, {
					"name" : "createTime",
					"type" : "string"
				}, {
					"name" : "userID",
					"type" : "string"
				}, {
					"name" : "actorName",
					"type" : "string"
				}, {
					"name" : "script",
					"type" : "string"
				}, {
					"name" : "state",
					"type" : "string"
				}, {
					"name" : "senderID",
					"type" : "string"
				}, {
					"name" : "senderName",
					"type" : "string"
				}, {
					"name" : "senderTime",
					"type" : "string"
				}]);
		this.store = new Ext.data.Store({
					url : String.format('{0}/workflow.do?method=getTodoListSQL',
							__cxtPath),
					root : 'rows',
					baseParams : {
						state : Ext.isNumber(this.state) ? this.state : '',
						limit : 20

					},
					reader : new Ext.data.JsonReader({
								root : 'rows',
								totalProperty : 'totalCount'
							}, reRecord),
					sortInfo : {
						field : 'createTime',
						direction : 'DESC'
					},
					autoLoad : this.autoLoadData
				});
		var csm = new Ext.grid.CheckboxSelectionModel();
		this.sm = csm;
		this.columns = [new Ext.grid.RowNumberer()];

//		if (this.state == 0) {
//			this.columns = this.columns.concat(csm);
//		}
		this.columns = this.columns.concat({
					header : '标题',
					width : 500,
					id : 'text_id',
					sortable : true,
					dataIndex : 'text'
				}, {
					header : '任务人',
					sortable : true,
					width : 200,
					dataIndex : 'actorName'
				}, {
					header : '发送人',
					sortable : true,
					width : 200,
					dataIndex : 'senderName'
				}, {
					header : '日期',
					width : 200,
					dataIndex : 'createTime',
					sortable : true,
					renderer : function(value, metaData, record) {
						if (value != null && value != '' && value.length > 10) {
							return value.substr(0, 16);
						} else {
							return value;
						}
					}
				});

		if (this.actionColumn) {
			this.columns = this.columns.concat(this.actionColumn);
		}
		this.tbar = new Ext.Toolbar({
					enableOverflow : true
				});
		this.tbar.add('模糊查询：', new Ext.ux.InklingSearchField({
							width : 120,
							store : this.store,
							tooltip : {
								title : '模糊查询',
								text : '您可以输入“标题”,“发送人”进行查询' 
							},
							inklings : [{
										name : 'text',
										op : 'like',
										type : 'string'
									}, {
										name : 'senderName',
										op : 'like',
										type : 'string'
									}]
						}));
/*		if (this.state == 0) {
			this.tbar.add({
						text : '批量签收',
						iconCls : 'ok',
						tooltip : '签收',
						hidden : true,
						id : 'btn_claim',
						ref : 'btn_claim',
						handler : function() {
							var records = p.getSelectionModel().getSelections();
							var ids = "";
							if (records.length == 0) {
								Ext.MessageBox.alert("提示", "请选择任务");
							} else {
								for (var i = 0; i < records.length; i++) {
									ids += records[i].get('appId') + ",";
								}
								if (ids.length > 1) {
									ids = ids.substr(0, ids.length - 1);
								}
								Ext.Ajax.request({
											url : __cxtPath
													+ '/workflow.do?method=claim',
											method : 'post',
											params : {
												workItemId : ids
											},
											success : function(response) {
												var result = Ext
														.decode(response.responseText);
												if (result.success) {
													Ext.MessageBox
															.alert(
																	result.messageTitle,
																	result.message)
												} else {
													Ext.MessageBox
															.alert(
																	result.messageTitle,
																	result.message)
												}
												p.reload();
												parent.xMis.refresh('newtodo');
												parent.xMis.refresh('mytodo');
											},
											failure : function(response, action) {
												Ext.MessageBox.hide();
												Ext.MessageBox.alert("提示",
														"操作失败！");
											}

										});
							}

						}
					});
		}
		if (this.helpBar) {
			this.tbar.add(this.helpBar);
		}*/
		this.bbar = new Ext.PagingToolbar({
					pageSize : 20,
					store : this.store,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					items : this.bbar || []
				});
		com.bitc.xmis.workflow.TodoGridPanel.superclass.initComponent
				.call(this);
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
	},
	reload : function(options) {
		this.getStore().reload(options);
	}

});

/**
 * @class com.bitc.xmis.workflow.TaskPanel
 * @extends Ext.grid.GridPanel 任务列表 -用于批量处理任务-- 未使用
 */
com.bitc.xmis.workflow.TaskPanel = Ext.extend(Ext.grid.GridPanel, {
	ref : 'p',
	initComponent : function() {
		var tPanel = this;
		this.store = new Ext.data.JsonStore({
					url : String.format('{0}/workflow.do?method=getTaskList',
							__cxtPath),
					root : 'rows',
					baseParams : {
						task_ID : this.task_ID,
						limit : 20
					},
					fields : ['ID', 'task_ID', 'todoId', 'workItemId',
							'DISPLAY_NAME', '标题', '发送人', '发送人部门ID', '发送人部门名称',
							'接收人', '发送人ID', '用户ID', '状态', 'PROCESSINSTANCE_ID',
							'COMMENTS', '创建时间'],
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
			width : 500,
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
			width : 100,
			sortable : true,
			dataIndex : '接收人'
		}, {
			header : '日期',
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

/*		this.tbar.add({
			text : '做任务',
			iconCls : 'ok',
			tooltip : '接收',
			ref : 'btn_t',
			handler : function() {
				var records = tPanel.getSelectionModel().getSelections();
				var ids = "";
				if (records.length == 0) {
					Ext.MessageBox.alert("提示", "请选择任务");
				} else {
					for (var i = 0; i < records.length; i++) {
						ids += records[i].get('workItemId') + ",";
					}
					if (ids.length > 1) {
						ids = ids.substr(0, ids.length - 1);
					}
					alert(records[0].get('task_ID') + "----" + ids);
					var task_ID = records[0].get('task_ID');
					// 如果是“接收任务”

					if (task_ID == "ProjectTaskAssign.Accept.Accept"
							|| task_ID == "StartProject.memberConfirm.confirm"
							|| task_ID == "StartProject.Manager.confirm") {
						alert("开始批量做任务");
						var myUrl = "/projectMain.do?method=batchAcceptProjectTask";
						Ext.Ajax.request({
									url : __cxtPath + myUrl,
									method : 'post',
									params : {
										workItemId : ids
									},
									success : function(response) {
										var result = Ext
												.decode(response.responseText);
										if (result.success) {
											Ext.MessageBox.alert(
													result.messageTitle,
													result.message)
										} else {
											Ext.MessageBox.alert(
													result.messageTitle,
													result.message)
										}
										tPanel.reload();
										parent.xMis.refresh('mytodo');
									},
									failure : function(response, action) {
										Ext.MessageBox.hide();
										Ext.MessageBox.alert("提示", "操作失败！");
									}

								});

					}
					// 如果是“接收项目经理变更通知”
					if (task_ID == "ProjectExecutiveDeptChange.NoticeManager.NoticeManagerTask") {
						alert("开始批量做任务");
						var myUrl = "/projectMain.do?method=batchAcceptProjectManager";
						Ext.Ajax.request({
									url : __cxtPath + myUrl,
									method : 'post',
									params : {
										workItemId : ids
									},
									success : function(response) {
										var result = Ext
												.decode(response.responseText);
										if (result.success) {
											Ext.MessageBox.alert(
													result.messageTitle,
													result.message)
										} else {
											Ext.MessageBox.alert(
													result.messageTitle,
													result.message)
										}
										tPanel.reload();
										parent.xMis.refresh('mytodo');
									},
									failure : function(response, action) {
										Ext.MessageBox.hide();
										Ext.MessageBox.alert("提示", "操作失败！");
									}

								});

					}
					// 如果是“ 修改执行部门 部门主任接收通知”
					if (task_ID == "ProjectExecutiveDeptChange.NoticeDeptDirector.NoticeDeptDirectorTask") {
						alert("开始批量做任务");
						var myUrl = "/projectMain.do?method=getBatchExecutiveDeptChangeNotice";
						Ext.Ajax.request({
									url : __cxtPath + myUrl,
									method : 'post',
									params : {
										workItemId : ids
									},
									success : function(response) {
										var result = Ext
												.decode(response.responseText);
										if (result.success) {
											Ext.MessageBox.alert(
													result.messageTitle,
													result.message)
										} else {
											Ext.MessageBox.alert(
													result.messageTitle,
													result.message)
										}
										tPanel.reload();
										parent.xMis
												.refresh('newtodo', 'mytodo');
									},
									failure : function(response, action) {
										Ext.MessageBox.hide();
										Ext.MessageBox.alert("提示", "操作失败！");
									}

								});

					}
				}

			}

		});*/
		this.bbar = new Ext.PagingToolbar({
					pageSize : 20,
					store : this.store,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					items : this.bbar || []
				});

		com.bitc.xmis.workflow.TaskPanel.superclass.initComponent.call(this);
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