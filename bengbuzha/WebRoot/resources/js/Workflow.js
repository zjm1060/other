Ext.ns("com.bitc.xmis.workflow");
/**
 * 环节双击事件，响应流程跟踪器的环节双击事件， 函数名称须为onActivityDoubleClick，由flex调用，传入选中环节对象，
 * 该对象包含4个属性 id,sn,name,displayName 使用方法如下
 */
function onActivityDoubleClick(activity) {
	alert("环节： " + "\n" + "id[ " + activity.id + " ]\n" + "sn[ " + activity.sn
			+ " ]\n" + "name[ " + activity.name + " ]\n" + "displayName[ "
			+ activity.displayName + " ]\n");
}
/**
 * 任务双击事件，响应流程跟踪器的任务双击事件， 函数名称须为onTaskDoubleClick，由flex调用，传入选中任务实例， 该对象包含8个属性
 * id,name,taskId,activityId,state,stepNumber,isSuspended,displayName 使用方法如下
 */
function onTaskDoubleClick(task) {
	alert("任务实例： " + "\n" + "id[ " + task.id + " ]\n" + "name[ " + task.name
			+ " ]\n" + "taskId[ " + task.taskId + " ]\n" + "activityId[ "
			+ task.activityId + " ]\n" + "state[ " + task.state + "]\n"
			+ "stepNumber[ " + task.stepNumber + " ]\n" + "isSuspended[ "
			+ task.isSuspended + " ]\n" + "displayName[ " + task.displayName
			+ " ]\n");
}
com.bitc.xmis.workflow.WorkItemHistoryRecord = Ext.data.Record.create(['id', {
			name : 'state',
			type : 'int'
		}, {
			name : 'createdTime',
			type : 'date',
			dateFormat : 'Y-m-d H:i:s'
		}, {
			name : 'claimedTime',
			type : 'date',
			dateFormat : 'Y-m-d H:i:s'
		}, {
			name : 'endTime',
			type : 'date',
			dateFormat : 'Y-m-d H:i:s'
		}, 'actor', 'sender', 'comments', 'processId', 'processInstanceId',
		'processName', 'activityId', 'activityName', 'taskInstanceId',
		'taskId', 'taskName','agent','authorizeeName']);
com.bitc.xmis.workflow.WorkItemStatus = ['未签收', '正在进行', '', '', '', '', '',
		'已处理', '', '已拒绝'];
com.bitc.xmis.workflow.ProcessInstanceTrace = Ext.data.Record.create(['id',
		'processInstanceId', 'stepNumber', 'minorNumber', 'type', 'edgeId',
		'fromNodeId', 'toNodeId']);
Ext.apply(com.bitc.xmis.workflow.ProcessInstanceTrace, {
			TRANSITION_TYPE : "Transition",
			LOOP_TYPE : "Loop",
			JUMPTO_TYPE : "JumpTo",
			WITHDRAW_TYPE : "Withdraw",
			REJECT_TYPE : "Reject"
		});

com.bitc.xmis.workflow.showTraceWin = function(processInstanceId) {
	var tracePanel = new com.bitc.xmis.workflow.TracePanel({
				border : false,
				region : 'center',
				listHeight : Ext.getBody().getHeight() / 2 - 50,
				processInstanceId : processInstanceId
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
/**
 * @class com.bitc.xmis.workflow.WorkItemHistoryList
 * @extends Ext.grid.GridPanel 工作项Grid
 * @author bomwu
 * @version 1.0
 */
com.bitc.xmis.workflow.WorkItemHistoryList = Ext.extend(Ext.grid.GridPanel, {
	/**
	 * @cfg {Object} queryParam 用于查询的参数，以Object的形式传进来。
	 *      <p>
	 *      例，显示用户ID为-1的历史任务:
	 * 
	 * <pre><code>
	 * new com.bitc.xmis.workflow.WorkItemHistoryList({
	 * 			queryParam : {
	 * 				actor : {
	 * 					userID : -1
	 * 				}
	 * 			}
	 * 		});
	 * </code></pre>
	 * 
	 * </p>
	 */
	/**
	 * @cfg {Boolean} autoLoadData 是否自动加载 默认true
	 */
	autoLoadData : true,
	/**
	 * @cfg {Object} sortInfo Store的排序信息，默认为
	 *      <code>{ field : 'createdTime',direction : 'DESC' }</code>
	 */
	/**
	 * @cfg {Boolean} showPagingToolbar 是否显示分布工具栏
	 */
	showPagingToolbar : false,
	autoExpandColumn : 'c_comments',
	initComponent : function() {
		this.store = new Ext.data.Store({
					url : __cxtPath
							+ '/workflow.do?method=getWorkItemHistoryList',
					baseParams : {
						queryJson : Ext.encode(this.queryParam),
						agentId:__emp.userID,//拒收的代理人能查看2014-01-06
						agentState:this.queryParam.state,//拒收的代理人能查看2014-01-06
						limit : 20
					},
					autoLoad : this.autoLoadData,
					remoteSort : true,
					sortInfo : this.sortInfo || {
						field : 'endTime',
						direction : 'DESC'
					},
					reader : new Ext.data.JsonReader({
								totalProperty : 'totalCount',
								root : 'rows'
							}, com.bitc.xmis.workflow.WorkItemHistoryRecord)
				});
		this.columns = [new Ext.grid.RowNumberer({
					width : 30
				})];
		if (this.actionColumn) {
			this.columns = this.columns.concat(this.actionColumn);
		}
		this.columns = this.columns.concat([{
					header : '流程名称',
					hidden : true,
					dataIndex : 'processName'
				}, {
					header : '步骤名称',
					hidden : true,
					dataIndex : 'activityName'
				}, {
					header : '任务名称',
					width : 200,
					dataIndex : 'taskName',
					renderer : function(value, metaData, record) {
						var processInstanceId = "'"
								+ record.data.processInstanceId + "'";
						return '<a  href="javascript:;"  onclick="com.bitc.xmis.workflow.showTraceWin('
								+ processInstanceId + ');">' + value + '</a>';
					}

				}, {
					header : '任务人',
					dataIndex : 'actor.userName',
					width:120,
					renderer : function(value, metaData, record) {
						if(record.data.state==0||record.data.state==1){
							if ( record.data.authorizeeName != null && record.data.authorizeeName !='') {
								return value+"("+record.data.authorizeeName+"代理)" ;
							} else {
								return value;
							}
						}else{
							if (record.data.agent != null) {
								return value+"("+record.data.agent.userName+"代理)" ;
							} else {
								return value;
							}
						}
					}
				}, {
					header : '代理人',
					hidden : true,
					dataIndex : 'agent.userName'
				}, {
					header : '发送人',
					hidden : true,
					dataIndex : 'sender.userName'
				}, {
					header : '创建时间',
					dataIndex : 'createdTime',
					sortable : true,
					xtype : 'datecolumn',
					format : 'Y-m-d H:i'
				}, {
					header : '签收时间',
					dataIndex : 'claimedTime',
					sortable : true,
					hidden : true,
					xtype : 'datecolumn',
					format : 'Y-m-d H:i'
				}, {
					header : '处理时间',
					dataIndex : 'endTime',
					xtype : 'datecolumn',
					sortable : true,
					format : 'Y-m-d H:i'
				}, {
					header : '状态',
					dataIndex : 'state',
					type : 'int',
					renderer : function(value) {
						return com.bitc.xmis.workflow.WorkItemStatus[value];
					}
				}, {
					id : 'c_comments',
					header : '备注',
					dataIndex : 'comments'
					// },{
				// xtype : 'actioncolumn',
				// width : 120,
				// align:'center',
				// items : [{
				// iconCls : 'edit',
				// tooltip : '取回',
				// handler : function(grid, rowIndex, colIndex) {
				// var id= grid.store.getAt(rowIndex).get('id');
				// Ext.MessageBox.confirm("提示",
				// "是否取回此任务？",function(btn){
				// if(btn == 'yes'){
				// Ext.Ajax.request({
				// url : __cxtPath
				// + '/workflow.do?method=withdraw',
				// method : 'post',
				// params : {
				// workItemId : id,
				// comments : '取回'
				// },
				// success : function(response, action) {
				//										
				// var obj = Ext.decode(response.responseText);
				// Ext.Msg.alert(obj.messageTitle, obj.message);
				// if (obj.success) {
				// Ext.MessageBox.alert("提示", "操作成功！");
				// } else {
				//
				// }
				// },
				// failure : function(response, action) {
				// Ext.MessageBox.hide();
				// Ext.MessageBox.alert("提示", "操作失败！");
				// }
				// });
				//						 	
				// }
				// });
				// }
				// }]
			}]);

		this.tbar = new Ext.Toolbar({
					enableOverflow : true
				});

		this.tbar.add('模糊查询：', new Ext.ux.InklingSearchField({
							width : 120,
							store : this.store,
							tooltip : {
								title : '模糊查询',
								text : '您可以输入“任务名称”,“发送人” 进行查询'
							},
							inklings : [{
										name : 'taskName',
										op : 'like',
										type : 'string'
									}, {
										name : 'sender.userName',
										op : 'like',
										type : 'string'
									}]
						}));

		if (this.showPagingToolbar) {
			this.bbar = new Ext.PagingToolbar({
				pageSize : 20,
				store : this.store,
				displayInfo : true,
				displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
				emptyMsg : "无显示数据",
				plugins : [new Ext.ux.Export({
					exportname : '历史任务',
					formater : [{
								columnHeader : '',
								formatClass : 'com.bitc.jk.xmis.export.RowNumber'
							}]
				}), new Ext.ux.PagelimitCombo()],
				items : this.bbar || []
			});
		}
		com.bitc.xmis.workflow.WorkItemHistoryList.superclass.initComponent
				.call(this);
	}
});

// 20131116
com.bitc.xmis.workflow.WorkItemHistoryGridPanel = Ext.extend(
		Ext.grid.GridPanel, {
			/**
			 * @cfg {Object} queryParam 用于查询的参数，以Object的形式传进来。
			 *      <p>
			 *      例，显示用户ID为-1的历史任务:
			 * 
			 * <pre><code>
			 * new com.bitc.xmis.workflow.WorkItemHistoryGridPanel({
			 * 			queryParam : {
			 * 				actor : {
			 * 					userID : -1
			 * 				}
			 * 			}
			 * 		});
			 * </code></pre>
			 * 
			 * </p>
			 */
			/**
			 * @cfg {Boolean} autoLoadData 是否自动加载 默认true
			 */
			autoLoadData : true,
			/**
			 * @cfg {Object} sortInfo Store的排序信息，默认为
			 *      <code>{ field : 'createdTime',direction : 'DESC' }</code>
			 */
			/**
			 * @cfg {Boolean} showPagingToolbar 是否显示分布工具栏
			 */
			showPagingToolbar : false,
			initComponent : function() {
				this.store = new Ext.data.Store({
					url : __cxtPath
							+ '/workflow.do?method=getHaveDoneHistoryListStore',
					baseParams : {
						itemState : this.itemState,
						limit : 20
					},
					autoLoad : this.autoLoadData,
					remoteSort : true,
					sortInfo : this.sortInfo || {
						field : '完成时间',
						direction : 'DESC'
					},
					reader : new Ext.data.JsonReader({
								totalProperty : 'totalCount',
								root : 'rows'
							}, Ext.data.Record.create(['ID', {
										name : '状态',
										type : 'int'
									}, {
										name : '创建时间'
									}, {
										name : '签收时间'
									}, {
										name : '完成时间'
									}, '备注', '任务名称', '任务人','发送人', '任务人ID',  
									'任务状态', '流程名称', 'VERSION', '当前执行人',
									'processInstanceId', '代理人','PROCESS_ID']))
				});
				this.columns = [new Ext.grid.RowNumberer({
							width : 30
						})];
				if (this.actionColumn) {
					this.columns = this.columns.concat(this.actionColumn);
				}
				this.columns = this.columns.concat([{
							header : '流程名称',
							width : 200,
							hidden:true,
							sortable : true,
							dataIndex : '流程名称'
						}, {
							header : '任务名称',
							width : 200,
							sortable : true,
							dataIndex : '任务名称',
							renderer : function(value, metaData, record) {
								var processInstanceId = "'"
										+ record.data.processInstanceId + "'";
								return '<a  href="javascript:;"  onclick="com.bitc.xmis.workflow.showTraceWin('
										+ processInstanceId
										+ ');">'
										+ value
										+ '</a>';
							}

						}, {
				xtype : 'actioncolumn',
				width : 40,
				header : '详情',
				align : 'center',
				items : [{
                    getClass: function(v, meta, rec) {          // Or return a class from a function
                    	if (rec.data.PROCESS_ID =='MissAttence') {
                            this.items[0].tooltip = '查看考勤单内容';
                            return 'edit';
                        }
                    },
                    handler: function(grid, rowIndex, colIndex) {
					Ext.Ajax.request({
						url : String
								.format(
										'{0}/attdMaint.do?method=getMissAttendanceObjByProcessInstanceId',
										__cxtPath),
						method : 'post',
						params : {
							processInstanceId : grid.store.getAt(rowIndex)
									.get('processInstanceId')
						},
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								var applicantID = result.attributes.applicantID;
								var isOfficialTravel = result.attributes.isOfficialTravel;
								var isAgent = result.attributes.isAgent;
								var missAttendanceId = result.attributes.missAttendanceId;
								if (missAttendanceId == null
										|| missAttendanceId == '') {
									Ext.Msg.alert('提示',
											'目前只提供考勤单的查看，有疑问请联系管理员~')
									return;
								}
								var applicantdate = result.attributes.applicantdate;
								var url = __cxtPath
										+ '/attdMaint.do?method=showPage&applicantID='
										+ applicantID + '&isOfficialTravel='
										+ isOfficialTravel + '&isAgent='
										+ isAgent + '&missAttendanceId='
										+ missAttendanceId + ''
										+ '&workItemId='
										+ grid.store.getAt(rowIndex).get('ID')
										+ '&applicantdate=' + applicantdate
										+ '&isSelect=true';
								parent.xMis.turn('wf-openform-', grid.store
												.getAt(rowIndex).get('任务名称'),
										url, 'task', true);

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
                }]
			},{
							header : '任务人',
							dataIndex : '任务人',
							renderer : function(value, metaData, record) {
								if (record.data.代理人 != null && record.data.代理人 != '') {
									return value+"("+record.data.代理人+"代理)";
								} else {
									return value;
								}
							}
						}, {
							header : '代理人',
							hidden:true,
							dataIndex : '代理人'
						}, {
							header : '发送人',
							hidden:true,
							sortable : true,
							dataIndex : '发送人'
						}, {
							header : '创建时间',
							dataIndex : '创建时间',
							sortable : true,
							renderer : function(value, metaData, record) {
								if (value != null && value != ''
										&& value.length > 10) {
									return value.substr(0, 16);
								} else {
									return value;
								}
							}
						}, {
							header : '签收时间',
							dataIndex : '签收时间',
							hidden : true,
							sortable : true,
							renderer : function(value, metaData, record) {
								if (value != null && value != ''
										&& value.length > 10) {
									return value.substr(0, 16);
								} else {
									return value;
								}
							}
						}, {
							header : '处理时间',
							dataIndex : '完成时间',
							sortable : true,
							renderer : function(value, metaData, record) {
								if (value != null && value != ''
										&& value.length > 10) {
									return value.substr(0, 16);
								} else {
									return value;
								}
							}
						}, {
							header : '任务状态',
							dataIndex : '状态',
							type : 'int',
							exportFormater : function(value, row, index,
									pageinfo) {
									return '已处理';
							},
							renderer : function(value) {
								return com.bitc.xmis.workflow.WorkItemStatus[value];
							}
						}, {
							header : '流程状态',
							dataIndex : '任务状态',
							sortable : true,
							type : 'int',
							exportFormater : function(value, row, index,
									pageinfo) {
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
							header : '当前执行人',
							sortable : true,
							dataIndex : '当前执行人'
						}, {
							header : '备注',
							dataIndex : '备注'
						}]);

				this.tbar = new Ext.Toolbar({
							enableOverflow : true
						});

				this.tbar.add('模糊查询：', new Ext.ux.InklingSearchField({
									width : 120,
									store : this.store,
									tooltip : {
										title : '模糊查询',
										text : '您可以输入“任务名称”,“发送人” 进行查询'
									},
									inklings : [{
												name : '任务名称',
												op : 'like',
												type : 'string'
											}, {
												name : '发送人',
												op : 'like',
												type : 'string'
											}]
								}));

				if (this.showPagingToolbar) {
					this.bbar = new Ext.PagingToolbar({
						pageSize : 20,
						store : this.store,
						displayInfo : true,
						displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
						emptyMsg : "无显示数据",
						plugins : [new Ext.ux.Export({
							exportname : '历史任务',
							formater : [{
								columnHeader : '',
								formatClass : 'com.bitc.jk.xmis.export.RowNumber'
							}]
						}), new Ext.ux.PagelimitCombo()],
						items : this.bbar || []
					});
				}
				com.bitc.xmis.workflow.WorkItemHistoryGridPanel.superclass.initComponent
						.call(this);
			}
		});

/**
 * @class com.bitc.xmis.workflow.TraceChartPanel
 * @extends Ext.Panel 流程跟踪图 Flex实现
 * @cfg {String} processInstanceId 流程实例ID
 */
com.bitc.xmis.workflow.FlexTracePanel = Ext.extend(Ext.Panel, {
	initComponent : function() {
		var varStr = 'endpoint=messagebroker/amf&processInstanceId='
				+ this.processInstanceId
				+ '&taskDoubleClick=true&activityDoubleClick=true';
		this.objectId = Ext.id();
		this.html = '<object id="'
				+ this.objectId
				+ '" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" '
				+ 'codebase="http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab" > '
				+ '<param name="src" value="'
				+ __cxtPath
				+ '/workflow/FireflowInstanceTrace.swf" /> '
				+ '<param name="wmode" value="transparent" /> '
				+ '<param name="flashVars" '
				+ 'value="'
				+ varStr
				+ '" /> '
				+ '<embed name="fireflowInstanceTrace" src="FireflowInstanceTrace.swf" '
				+ 'pluginspage="http://www.macromedia.com/shockwave/download/index.cgi?P1_Prod_Version=ShockwaveFlash" '
				+ 'flashVars="' + varStr + '" /> </object>';
		com.bitc.xmis.workflow.FlexTracePanel.superclass.initComponent
				.call(this);
		this.on('afterlayout', function() {
					var obj = Ext.get(this.objectId);
					obj.dom.height = this.body.getHeight();
					obj.dom.width = this.body.getWidth();
				}, this);
	}
});
com.bitc.xmis.workflow.TracePanel = Ext.extend(Ext.Panel, {
	/**
	 * @cfg {String} processInstanceId
	 * @requires
	 */
	initComponent : function() {
		if (!this.processInstanceId) {
			this.html = '未定义processInstanceId，请检查配置';
			com.bitc.xmis.workflow.TracePanel.superclass.initComponent
					.call(this);
			return;
		}
		this.layout = 'border';
		var paneltype = Ext.isCanvas
				? com.bitc.xmis.workflow.Html5TracePanel
				: com.bitc.xmis.workflow.FlexTracePanel
		this.chartPanel = new paneltype({
					region : 'south',
					title : '流程跟踪图',
					collapsible : true,
					split: true,
					height : this.listHeight || 300,
					collapsed : true,
					processInstanceId : this.processInstanceId
				});
		var historyList = this.historyList = new com.bitc.xmis.workflow.WorkItemHistoryList(
				{
					region : 'center',
					height : this.listHeight || 300,
					title : '任务列表',
					autoLoadData : true,
					queryParam : {
						processInstanceId : this.processInstanceId
					},
					sortInfo : {
						field : 'createdTime',
						direction : 'ASC'
					}
				});
		if (Ext.isCanvas) {
			this.chartPanel.on('canvasload', this.loadTraceData, this);
		} else {
			historyList.getStore().load();
		}
		this.items = [this.chartPanel, this.historyList];
		com.bitc.xmis.workflow.TracePanel.superclass.initComponent.call(this);
	},
	reload : function() {
		this.loadTraceData(this.chartPanel, this.chartPanel.canvas);
	},
	loadTraceData : function(tracepanel, canvas) {
		var traceStore = new Ext.data.Store({
					url : __cxtPath + '/workflow.do?method=getHistoryTrace',
					baseParams : {
						processInstanceId : this.processInstanceId
					},
					reader : new Ext.data.JsonReader({
								totalProperty : 'totalCount',
								root : 'rows'
							}, com.bitc.xmis.workflow.ProcessInstanceTrace)
				});
		traceStore.load({
			scope : this,
			callback : function(records) {
				var layer = canvas.layer;
				var tLayer = canvas.tLayer;
				for (var i = 0; i < records.length; i++) {
					var trace = records[i].data;
					if (trace.type == com.bitc.xmis.workflow.ProcessInstanceTrace.TRANSITION_TYPE) {
						layer.get('#' + trace.fromNodeId)[0].active();
						layer.get('#' + trace.toNodeId)[0].active();
						tLayer.get('#' + trace.edgeId)[0].active();
					}
				}
				this.loadWorkItemData(canvas);
			}
		});
	},
	loadWorkItemData : function(canvas) {
		this.historyList.getStore().load({
			scope : this,
			callback : function(records) {
				var stage = canvas.stage;
				var layer = canvas.layer;
				var tooltipLayer = canvas.tooltipLayer;
				var tooltip = new Kinetic.Text({
							text : "",
							fontSize : 10,
							lineHeight : 2,
							cornerRadius : 2,
							padding : 5,
							visible : false,
							fill : "#E9F2FF",
							verticalAlign : 'middle',
							textFill : "black",
							stroke : "#99BBE8",
							strokeWidth : 1
						});
				tooltipLayer.add(tooltip);
				var template = '任务人：{0}\n状态：{1}\n到达时间：{2}\n签收时间：{3}\n完成时间：{4}';
				function formatDate(date) {
					return date == null ? '' : date.format('m-d H:i');
				}
				for (var i = 0; i < records.length; i++) {
					(function() {
						var hist = records[i];
						var act = layer.get('#' + hist.get('activityId'))[0];
						act.on("mousemove", function() {
							// update tooltip
							var mousePos = stage.getMousePosition();
							tooltip.setPosition(mousePos.x + 5, mousePos.y + 5);
							tooltip.setText(String.format(template, hist
											.get('actor.userName'),
									com.bitc.xmis.workflow.WorkItemStatus[hist
											.get('state')], formatDate(hist
											.get('createdTime')),
									formatDate(hist.get('claimedTime')),
									formatDate(hist.get('endTime'))));
							tooltip.show();
							tooltipLayer.draw();
						});
						act.on("mouseout", function() {
									tooltip.hide();
									tooltipLayer.draw();
								});
					}());
				}
			}
		});
		canvas.redraw();
	}
});
/**
 * @class com.bitc.xmis.workflow.Html5TracePanel
 * @extends Ext.Panel 流程跟踪图 使用HTML5 Canvas实现
 */
com.bitc.xmis.workflow.Html5TracePanel = Ext.extend(Ext.Panel, {
			autoScroll : true,
			constructor : function(config) {
				this.request(config.processId ? {
					processId : config.processId
				} : {
					processInstanceId : config.processInstanceId
				});
				com.bitc.xmis.workflow.Html5TracePanel.superclass.constructor
						.call(this, config);
				this.addEvents('canvasload');
			},
			// private
			request : function(params) {
				Ext.Ajax.request({
							url : __cxtPath
									+ '/workflow.do?method=getProcessDefinitionXML',
							params : params,
							scope : this,
							success : function(response) {
								var xmlDoc = response.responseXML;
								this.initCanvas(xmlDoc);
							}
						});
			},
			// private
			initCanvas : function(xmlDoc) {
				this.canvas = new com.bitc.xmis.workflow.flowchart.Canvas({
							xmlDoc : xmlDoc,
							height : 1000,
							width : 1000
						});
				this.canvas.on('loaded', function(canvas) {
							this.fireEvent('canvasload', this, canvas);
						}, this);
				this.add(this.canvas);
				this.doLayout();
			},
			initComponent : function() {
				com.bitc.xmis.workflow.Html5TracePanel.superclass.initComponent
						.call(this);
			},
			beforeDestroy : function() {
				com.bitc.xmis.workflow.Html5TracePanel.superclass.beforeDestroy
						.call(this);
				if (this.canvas) {
					Ext.destroy(this.canvas);
				}
			}
		});
com.bitc.xmis.workflow.TraceButton = Ext.extend(Ext.Button, {
			iconCls : 'diagram',
			text : '流程跟踪',
			initComponent : function() {
				var tracePanel = new com.bitc.xmis.workflow.TracePanel({
							region : 'center',
							processInstanceId : this.processInstanceId
						});
				var traceWin = new Ext.Window({
							title : '流程跟踪',
							x : 25,
							y : 25,
							width : 900,
							height : 550,
							items : tracePanel,
							layout : 'border',
							closeAction : 'hide',
							tools : [{
										qtip : '刷新',
										id : 'refresh',
										handler : function() {
											tracePanel.reload();
										}
									}]
						});
				/** 每次点击流程跟踪，数据库做一次查询操作 */
				// this.handler = function() {
				// traceWin.show();
				// };
				this.handler = function() {
					var tracePanel = new com.bitc.xmis.workflow.TracePanel({
								region : 'center',
								processInstanceId : this.processInstanceId
							});
					var traceWin = new Ext.Window({
								title : '流程跟踪',
								x : 25,
								y : 25,
								width : 900,
								height : 550,
								items : tracePanel,
								layout : 'border',
								closeAction : 'hide',
								tools : [{
											qtip : '刷新',
											id : 'refresh',
											handler : function() {
												tracePanel.reload();
											}
										}]
							});
					traceWin.show();
				};

				com.bitc.xmis.workflow.TraceButton.superclass.initComponent
						.call(this);
			}
		});
/**
 * @class com.bitc.xmis.workflow.SubmitButton 提交按钮
 * @extends Ext.Button
 */
com.bitc.xmis.workflow.SubmitButton = Ext.extend(Ext.Button, {
	text : '提交',
	iconCls : 'ok',
	/**
	 * @cfg {String}
	 */
	initComponent : function() {
		if (this.handler) {
			this.menuhandler = this.handler;
			this.handler = undefined;
		}
		this.menu = {
			items : []
		};
		com.bitc.xmis.workflow.SubmitButton.superclass.initComponent.call(this);
		if (this.expression) {
			Ext.Ajax.request({
						url : __cxtPath + '/workflow.do?method=getCandidate',
						params : {
							expression : this.expression
						},
						scope : this,
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result && result.success) {
								this.setText('发送给...');
								var emps = result.attributes.employees;
								for (var i = 0; i < emps.length; i++) {
									this.menu.addMenuItem({
												text : emps[i].userName,
												handler : this.menuhandler
														.createDelegate(
																this.scope
																		|| this,
																[emps[i].userID])
											})
								}
							} else {
								this.handler = this.errorHandler
										.createDelegate(this, [result
														? result.message
														: '获取候选任务人失败。<br/>']);
							}
						}
					});
		} else {
			this.handler = this.errorHandler.createDelegate(this, ['未定义候选人范围']);
		}
	},
	errorHandler : function(msg) {
		Ext.Msg.alert('错误', msg);
	}
});
/**
 * @class com.bitc.xmis.workflow.DomHelper
 * @static
 * @singlton
 */
com.bitc.xmis.workflow.DomHelper = {
	getX : function(node) {
		return new Number(node
				.selectSingleNode('fpdl:ExtendedAttributes/fpdl:ExtendedAttribute[@Name="FIRE_FLOW.bounds.x"]')
				.getAttribute('Value'));
	},
	getY : function(node) {
		return new Number(node
				.selectSingleNode('fpdl:ExtendedAttributes/fpdl:ExtendedAttribute[@Name="FIRE_FLOW.bounds.y"]')
				.getAttribute('Value'));
	},
	getWidth : function(node) {
		return new Number(node
				.selectSingleNode('fpdl:ExtendedAttributes/fpdl:ExtendedAttribute[@Name="FIRE_FLOW.bounds.width"]')
				.getAttribute('Value'));
	},
	getHeight : function(node) {
		return new Number(node
				.selectSingleNode('fpdl:ExtendedAttributes/fpdl:ExtendedAttribute[@Name="FIRE_FLOW.bounds.height"]')
				.getAttribute('Value'));
	},
	getBounds : function(node) {
		return {
			x : com.bitc.xmis.workflow.DomHelper.getX(node),
			y : com.bitc.xmis.workflow.DomHelper.getY(node),
			width : com.bitc.xmis.workflow.DomHelper.getWidth(node),
			height : com.bitc.xmis.workflow.DomHelper.getHeight(node)
		};
	},
	getConfig : function(node) {
		return {
			id : node.getAttribute('Id'),
			text : node.getAttribute('DisplayName'),
			x : com.bitc.xmis.workflow.DomHelper.getX(node),
			y : com.bitc.xmis.workflow.DomHelper.getY(node),
			width : com.bitc.xmis.workflow.DomHelper.getWidth(node),
			height : com.bitc.xmis.workflow.DomHelper.getHeight(node)
		}
	}
};
com.bitc.xmis.workflow.Factory = {
	createActivity : function(node) {
		var fc = com.bitc.xmis.workflow.flowchart;// shortcut
		var activity = new fc.Activity(com.bitc.xmis.workflow.DomHelper
				.getConfig(node));
		var taskNodes = node.selectNodes('fpdl:Tasks/fpdl:Task');
		for (var j = 0; j < taskNodes.length; j++) {
			var taskNode = taskNodes[j];
			activity.addTask({
						id : taskNode.getAttribute('Id'),
						text : taskNode.getAttribute('DisplayName')
					});
		}
		return activity;
	},
	createEnd : function(node, container) {
		var fc = com.bitc.xmis.workflow.flowchart;// shortcut
		return new fc.End(com.bitc.xmis.workflow.DomHelper.getConfig(node),
				container);
	},
	createSynchronizer : function(node, container) {
		var fc = com.bitc.xmis.workflow.flowchart;// shortcut
		return new fc.Synchronizer(com.bitc.xmis.workflow.DomHelper
						.getConfig(node), container);
	},
	createTrasition : function(node, nodes) {
		var fc = com.bitc.xmis.workflow.flowchart;// shortcut
		var h = com.bitc.xmis.workflow.DomHelper;
		var from = nodes[node.getAttribute('From')];
		var to = nodes[node.getAttribute('To')];
		var fx = h.getX(from), fy = h.getY(from), fw = h.getWidth(from), fh = h
				.getHeight(from);
		// from 节点的中心坐标
		var fcenter = {
			x : fx + fw / 2,
			y : fy + fh / 2
		};
		var tx = h.getX(to), ty = h.getY(to), tw = h.getWidth(to), th = h
				.getHeight(to);
		// to 节点的中心坐标
		var tcenter = {
			x : tx + tw / 2,
			y : ty + th / 2
		};
		return new fc.Transition({
					id : node.getAttribute('Id'),
					text : node.getAttribute('DisplayName'),
					cut1 : (Math.sqrt(Math.pow(fw, 2) + Math.pow(fh, 2))) / 2,
					cut2 : (Math.sqrt(Math.pow(tw, 2) + Math.pow(th, 2))) / 2,
					x1 : fcenter.x,
					y1 : fcenter.y,
					x2 : tcenter.x,
					y2 : tcenter.y
				});
	}
};
Ext.ns('com.bitc.xmis.workflow.flowchart');
/**
 * @class com.bitc.xmis.workflow.flowchart.Canvas
 * @extends Ext.BoxComponent 用于画流程图的画布所处的容器
 */
com.bitc.xmis.workflow.flowchart.Canvas = Ext.extend(Ext.BoxComponent, {
			/**
			 * @cfg {XMLDocument} xmlDoc 流程定义XML
			 */
			constructor : function(config) {
				var id = Ext.id(null, 'canvas-');
				Ext.apply(config, {
							id : id,
							autoEl : {
								id : id,
								tag : 'div'
							}
						});
				com.bitc.xmis.workflow.flowchart.Canvas.superclass.constructor
						.call(this, config);
				this.addEvents('loaded');
			},
			// private
			initDatas : function() {
				var helper = com.bitc.xmis.workflow.DomHelper;// shortcut
				var factory = com.bitc.xmis.workflow.Factory;
				this.nodes = {};
				this.activities = new Array();
				this.ends = new Array();
				this.trasitions = new Array();
				this.synchronizers = new Array();
				var process = this.xmlDoc.selectNodes('fpdl:WorkflowProcess')[0];
				var startNode = process.selectNodes('fpdl:StartNode')[0];
				var fc = com.bitc.xmis.workflow.flowchart;// shortcut
				this.start = new fc.Start(com.bitc.xmis.workflow.DomHelper
								.getConfig(startNode), this.layer);
				this.nodes[startNode.getAttribute('Id')] = startNode;
				var actNodes = process
						.selectNodes('fpdl:Activities/fpdl:Activity');
				for (var i = 0; i < actNodes.length; i++) {
					var actNode = actNodes[i];
					this.nodes[actNode.getAttribute('Id')] = actNode;
					var activity = factory.createActivity(actNode);
					this.activities.push(activity);
					this.layer.add(activity);
				}
				var endNodes = process
						.selectNodes('fpdl:EndNodes/fpdl:EndNode');
				for (var i = 0; i < endNodes.length; i++) {
					var end = endNodes[i];
					this.nodes[end.getAttribute('Id')] = end;
					this.ends.push(factory.createEnd(end, this.layer));
				}
				var synchronizerNodes = process
						.selectNodes('fpdl:Synchronizers/fpdl:Synchronizer');
				for (var i = 0; i < synchronizerNodes.length; i++) {
					var syncNode = synchronizerNodes[i];
					this.nodes[syncNode.getAttribute('Id')] = syncNode;
					this.synchronizers.push(factory.createSynchronizer(
							syncNode, this.layer));
				}
				var transitionNodes = process
						.selectNodes('fpdl:Transitions/fpdl:Transition');
				for (var i = 0; i < transitionNodes.length; i++) {
					var transitionNode = transitionNodes[i];
					var transition = factory.createTrasition(transitionNode,
							this.nodes)
					this.trasitions.push(transition);
					this.tLayer.add(transition);
				}
				// --start 虚线?
				/**
				 * var loopNodes = process.selectNodes('fpdl:Loops/fpdl:Loop');
				 * for(var i=0;i<loopNodes.length;i++){ var loopNode =
				 * loopNodes[i]; var loop = factory.createTrasition(loopNode,
				 * this.nodes) this.trasitions.push(loop);
				 * this.tLayer.add(loop); }
				 */
				// --stop
				this.resize();
			},
			resize : function() {
				var maxX, maxY;
				for (var nodeId in this.nodes) {
					var helper = com.bitc.xmis.workflow.DomHelper;// shortcut
					var bounds = helper.getBounds(this.nodes[nodeId]);
					maxX = maxX > bounds.x + bounds.width ? maxX : bounds.x
							+ bounds.width;
					maxY = maxY > bounds.y + bounds.height ? maxY : bounds.y
							+ bounds.height;
				}
				this.setSize(maxX + 30, maxY + 30);
				this.stage.setSize(maxX + 30, maxY + 30);
			},
			redraw : function() {
				this.tLayer.draw();
				this.layer.draw();
				this.tooltipLayer.draw();
			},
			initComponent : function() {
				com.bitc.xmis.workflow.flowchart.Canvas.superclass.initComponent
						.call(this);
				this.on('afterrender', function() {
							this.stage = new Kinetic.Stage({
										container : this.id,
										width : 1000,
										height : 1000
									});
							this.layer = new Kinetic.Layer();
							this.tLayer = new Kinetic.Layer();
							this.tooltipLayer = new Kinetic.Layer();
							this.stage.add(this.layer);
							this.stage.add(this.tLayer);
							this.stage.add(this.tooltipLayer);
							this.initDatas();
							this.redraw();
							this.fireEvent('loaded', this);
						}, this);
			},
			// private
			beforeDestroy : function() {
				com.bitc.xmis.workflow.flowchart.Canvas.superclass.beforeDestroy
						.call(this);
				if (this.layer) {
					Ext.destroy(this.layer);
				}
				if (this.tLayer) {
					Ext.destroy(this.tLayer);
				}
				if (this.tooltipLayer) {
					Ext.destroy(this.tooltipLayer);
				}
				if (this.stage) {
					Ext.destroy(this.stage);
				}
			}
		});

/**
 * @class com.bitc.xmis.workflow.flowchart.ImageNode
 * @extends Kinetic.Image 图片节点
 * @cfg {String} src 图片SRC
 * @constructor
 * @param {Object}
 *            config 配置对象
 * @param {Kinetic.Layer}
 *            layer the Kinetic Layer
 */
com.bitc.xmis.workflow.flowchart.ImageNode = function(config, container) {
	var imageObj = this.imageObj = new Image();
	Ext.apply(config, {
				image : imageObj
			});
	var node = this;
	imageObj.onload = function() {
		com.bitc.xmis.workflow.flowchart.ImageNode.superclass.constructor.call(
				node, config);
		if (container) {
			container.add(node);
			container.draw();
		}
	};
	imageObj.src = this.src;
};
Ext.extend(com.bitc.xmis.workflow.flowchart.ImageNode, Kinetic.Image, {});
/**
 * @class com.bitc.xmis.workflow.flowchart.Start
 * @extends com.bitc.xmis.workflow.flowchart.ImageNode 开始节点
 */
com.bitc.xmis.workflow.flowchart.Start = Ext.extend(
		com.bitc.xmis.workflow.flowchart.ImageNode, {
			src : __cxtPath + '/resources/images/workflow/startNodeIcon.png',
			active : function() {
				// console.info('Active:' + this.attrs.id);
				this.imageObj.src = __cxtPath
						+ '/resources/images/workflow/startNodeRunIcon.png'
			}
		});
/**
 * @class com.bitc.xmis.workflow.flowchart.End
 * @extends com.bitc.xmis.workflow.flowchart.ImageNode 结束节点
 */
com.bitc.xmis.workflow.flowchart.End = Ext.extend(
		com.bitc.xmis.workflow.flowchart.ImageNode, {
			src : __cxtPath + '/resources/images/workflow/endNodeIcon.png',
			active : Ext.emptyFn
		});
/**
 * @class com.bitc.xmis.workflow.flowchart.Synchronizer
 * @extends com.bitc.xmis.workflow.flowchart.ImageNode 流程图中的同步节点
 */
com.bitc.xmis.workflow.flowchart.Synchronizer = Ext.extend(
		com.bitc.xmis.workflow.flowchart.ImageNode, {
			src : __cxtPath + '/resources/images/workflow/synchronizerIcon.png',
			active : function() {
				// console.info('Active:' + this.attrs.id);
				this.imageObj.src = __cxtPath
						+ '/resources/images/workflow/synchronizerRunIcon.png'
			}
		});
/**
 * @class com.bitc.xmis.workflow.flowchart.Activity
 * @extends Kinetic.Group 流程图中的Activity节点
 */
com.bitc.xmis.workflow.flowchart.Activity = Kinetic.Group.extend({
			init : function(config) {
				this._super(config);
				/**
				 * Activity节点中的节点本身
				 * 
				 * @type Kinetic.Rect
				 * @property activity
				 */
				this.activity = new Kinetic.Rect({
							x : 0,
							y : 0,
							width : config.width,
							height : config.height,
							fill : "white",
							stroke : "black",
							strokeWidth : 2
						});
				this.add(this.activity);
				this.label = new Kinetic.Text({
							x : 0,
							y : 0,
							width : config.width,
							height : 30,
							fill : '#CCCCCC',
							padding : 5,
							textFill : 'black',
							stroke : "black",
							strokeWidth : 2,
							text : config.text,
							align : 'center',
							verticalAlign : 'middle',
							fontSize : 10
						});
				this.add(this.label);
				/**
				 * 包含的Task数组
				 * 
				 * @type Array
				 * @property tasks
				 */
				this.tasks = {};
			},
			active : function() {
				// console.info('Active:' + this.attrs.id);
				this.activity.setStroke('green');
				this.label.setStroke('green');
				this.label.setTextFill('green');
				for (var task in this.tasks) {
					this.tasks[task].setTextFill('green');
				}
			},
			addTask : function(config) {
				var task = new Kinetic.Text({
							x : 0,
							y : 30,
							width : config.width,
							height : 25,
							// fill : '#CCCCCC',
							padding : 5,
							textFill : 'black',
							// stroke : "black",
							// strokeWidth : 2,
							text : config.text,
							align : 'center',
							fontSize : 10
						});
				this.tasks[config.id] = task;
				this.add(task);
			}
		});
/**
 * @class com.bitc.xmis.workflow.flowchart.Transition
 * @extends Kinetic.Shape 流程图中的箭头
 * @cfg {Number} x1 起始点X坐标
 * @cfg {Number} y1 起始点Y坐标
 * @cfg {Number} x2 终止点X坐标
 * @cfg {Number} y2 终止点Y坐标
 * @cfg {String} text 箭头上显示的文字
 * @cfg {String} color 箭头的颜色 默认为#000000
 */
com.bitc.xmis.workflow.flowchart.Transition = Kinetic.Shape.extend({
			init : function(config) {
				config.len = Math.sqrt(Math.pow((config.y2 - config.y1), 2)
						+ Math.pow((config.x2 - config.x1), 2));
				this.shapeType = "Arrow";
				config.drawFunc = this.drawFunc;
				Ext.apply(config, {
							stroke : 'black',
							fill : 'black',
							strokeWidth : 1
						})
				this._super(config);
			},
			active : function() {
				// console.info('Active:' + this.attrs.id);
				this.attrs.color = '#00FF00';
			},
			drawFunc : function(context) {
				context.save();
				context.translate(this.attrs.x1, this.attrs.y1);
				context.rotate(Math.atan2(this.attrs.y2 - this.attrs.y1,
						this.attrs.x2 - this.attrs.x1));
				var start = this.attrs.cut1;
				var end = this.attrs.len - this.attrs.cut2;
				context.strokeStyle = this.attrs.color || '#000000';
				context.fillStyle = this.attrs.color || '#000000';
				context.lineWidth = this.attrs.lineWidth || 1;
				// this.stroke(context);
				context.beginPath();
				context.moveTo(start, 0);
				context.lineTo(end, 0);
				context.moveTo(end, 0);
				context.lineTo(end - 4, -3);
				context.lineTo(end - 4, 3);
				context.lineTo(end, 0);
				context.closePath();
				context.fill();
				context.stroke();
				// this.fill(context);
				// this.stroke(context);
				if (this.attrs.text) {
					context.font = '12px serif';
					context.fillStyle = this.attrs.color || '#000000';
					context.textBaseline = 'ideographic';
					context.fillText(this.attrs.text, start, -8);
				}
				context.restore();
			}
		});
/**
 * 画loop线 com.bitc.xmis.workflow.flowchart.Loop = Kinetic.Shape.extend({ init :
 * function(config) { config.len = Math.sqrt(Math.pow((config.y2 - config.y1),
 * 2) + Math.pow((config.x2 - config.x1), 2)); this.shapeType = "Arrow";
 * config.drawFunc = this.drawFunc; Ext.apply(config, { stroke : 'black', fill :
 * 'black', strokeWidth : 1 }) this._super(config); }, active : function() { //
 * console.info('Active:' + this.attrs.id); this.attrs.color = '#00FF00'; },
 * drawFunc : function(context) { context.save();
 * context.translate(this.attrs.x1, this.attrs.y1);
 * context.rotate(Math.atan2(this.attrs.y2 - this.attrs.y1, this.attrs.x2 -
 * this.attrs.x1)); var start = this.attrs.cut1; var end = this.attrs.len -
 * this.attrs.cut2; context.strokeStyle = this.attrs.color || '#000000';
 * context.fillStyle = this.attrs.color || '#000000'; context.lineWidth =
 * this.attrs.lineWidth || 1; // this.stroke(context); context.beginPath();
 * context.moveTo(start, 0); context.lineTo(end, 0); context.moveTo(end, 0);
 * context.lineTo(end - 4, -3); context.lineTo(end - 4, 3); context.lineTo(end,
 * 0); context.closePath(); context.fill(); context.stroke(); //
 * this.fill(context); // this.stroke(context); if (this.attrs.text) {
 * context.font = '12px serif'; context.fillStyle = this.attrs.color ||
 * '#000000'; context.textBaseline = 'ideographic';
 * context.fillText(this.attrs.text, start, -8); } context.restore(); } });
 */
com.bitc.xmis.workflow.ApprovalInfoRecord = Ext.data.Record.create(['ID', {
			name : '是否通过',
			type : 'int'
		}, {
			name : '审批人ID',
			type : 'int'
		}, {
			name : '审批时间',
			type : 'date',
			dateFormat : 'Y-m-d H:i:s'
		}, '任务名称', '审批人', '审批意见', 'workItemID','actorID','actorName']);
com.bitc.xmis.workflow.approvalInfoList = Ext.extend(Ext.grid.GridPanel, {
			autoLoadData : true,
			autoExpandColumn : 'c_comments',
			initComponent : function() {
				this.store = new Ext.data.Store({
							url : __cxtPath
									+ '/workflow.do?method=getApprovalInfos',
							baseParams : {
								processInstanceId : this.processInstanceId,
								limit : 20
							},
							autoLoad : this.autoLoadData,
							remoteSort : true,
							sortInfo : this.sortInfo || {
								field : '审批时间',
								direction : 'ASC'
							},
							reader : new Ext.data.JsonReader({
										totalProperty : 'totalCount',
										root : 'rows'
									},
									com.bitc.xmis.workflow.ApprovalInfoRecord)
						});
				this.columns = [new Ext.grid.RowNumberer()];
				this.columns = this.columns.concat([{
							header : 'ID',
							hidden : true,
							dataIndex : 'ID'
						}, {
							header : 'workItemID',
							hidden : true,
							dataIndex : 'workItemID'
						}, {
							header : '任务名称',
							width : 200,
							dataIndex : '任务名称'
						}, {
							header : '审批人ID',
							hidden : true,
							dataIndex : '审批人ID'
						}, {
							header : '审批人',
							dataIndex : '审批人',
							renderer : function(value, metaData, record) {
								if (record.data.actorID == record.data.审批人ID ) {
									return value;
								} else {
									return record.data.actorName+"("+value+"代理)" ;
								}
							}
						}, {
							header : '审批时间',
							dataIndex : '审批时间',
							xtype : 'datecolumn',
							format : 'm-d H:i',
							sortable : true
						}, {
							header : '是否通过',
							dataIndex : '是否通过',
							type : 'int',
							renderer : function(value) {
								if (value == 0) {
									return '<font  color="green">通过</font>';
								} else {
									return '<font  color="#CC0000">未通过</font>';
								}
							}
						}, {
							id : 'c_comments',
							header : '审批意见',
							dataIndex : '审批意见'
						}]);
				com.bitc.xmis.workflow.approvalInfoList.superclass.initComponent
						.call(this);
			}
		});
/**
 * 审批信息窗口
 */
com.bitc.xmis.workflow.ApprovalInfosButton = Ext.extend(Ext.Button, {
	iconCls : 'edit',
	text : '审批记录',
	initComponent : function() {
		/** 每次点击流程跟踪，数据库做一次查询操作 */
		this.handler = function() {
			var approvalInfoPanel = new com.bitc.xmis.workflow.approvalInfoList(
					{
						collapsible : true,
						region : 'center',
						title : '审批记录',
						autoLoadData : true,
						processInstanceId : this.processInstanceId,
						sortInfo : {
							field : '审批时间',
							direction : 'ASC'
						}
					});
			var approvalInfoWin = new Ext.Window({
						x : 25,
						y : 25,
						width : 700,
						height : 200,
						items : approvalInfoPanel,
						layout : 'border',
						closeAction : 'hide',
						tools : [{
									qtip : '刷新',
									id : 'refresh',
									handler : function() {
										approvalInfoPanel.reload();
									}
								}]
					});
			approvalInfoWin.show();
		};
		com.bitc.xmis.workflow.ApprovalInfosButton.superclass.initComponent
				.call(this);
	}
});