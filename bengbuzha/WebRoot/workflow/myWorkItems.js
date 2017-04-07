Ext.onReady(function() {
	Ext.QuickTips.init();

	var myTodoList = new com.bitc.xmis.workflow.TodoGridPanel({
				region : 'center',
				viewConfig : {
					emptyText : '您暂时没有新的任务'
				},
				state : 1
			});
	top.xMis.addRefresh('myworkitemnewtodo', function() {
				myTodoList.reload();
			});

	var myHistWorkItem = new com.bitc.xmis.workflow.WorkItemHistoryGridPanel({
		region : 'center',
		autoLoadData : false,
		cellTip : true,
		viewConfig : {
			emptyText : '没有已处理的任务'
		},
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		showPagingToolbar : true,
		itemState : 7,
		actionColumn : {
			xtype : 'actioncolumn',
			width : 30,
			items : [ {
				iconCls : 'diagram',
				tooltip : '流程跟踪',
				handler : function(grid, rowIndex, colIndex) {
					var tracePanel = new com.bitc.xmis.workflow.TracePanel({
								border : false,
								region : 'center',
								listHeight : Ext.getBody().getHeight() / 2 - 50,
								processInstanceId : grid.store.getAt(rowIndex)
										.get('processInstanceId')
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
		}
	});
	var tabs = new Ext.TabPanel({
				region : 'center',
				ref : 'tabs',
				activeTab : 0,
				autoScroll : true,
				deferredRender : false,
				items : [{
							title : '我的任务',
							layout : 'border',
							items : [myTodoList]
						}, {
							id : 'haveDoneTask',
							title : '已处理的任务',
							layout : 'border',
							items : [myHistWorkItem]
						}],
				listeners : {
					'tabchange' : function(a, activeTab) {
						if (activeTab.getItemId() == 'haveDoneTask') {
							myHistWorkItem.getStore().load({
										params : {
											start : 0,
											limit : 20
										},
										callback : function(record, options,
												success) {
											if (success == true) {
											} else {
												Ext.Msg.alert("出错了", "加载数据错误");
											}
										}
									});
							tabs.removeListener('tabchange');
						}
					}
				}

			});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [tabs]
			});
});