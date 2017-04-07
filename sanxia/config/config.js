Ext.onReady(function() {
	Ext.BLANK_IMAGE_URL = '../images/default/s.gif';
	Ext.QuickTips.init();
	var contextPath = 'http://192.169.11.69:4001/3gweb/'; //开发环境
	// var contextPath = '../../';  //生产环境
	var pageRecord = Ext.data.Record.create(['id', 'code', 'name', 'url']);
	var planRecord = Ext.data.Record.create(['id', 'name', 'pages']);
	var dirty = false;
	var checkSm = new Ext.grid.CheckboxSelectionModel({
				listeners : {
					'rowselect' : function() {
						pageGrid.delButton.setDisabled(false);
					}
				}
			});
	var pageGrid = new Ext.grid.EditorGridPanel({
		title : '所有页面',
		flex : 1,
		autoExpandColumn : 'url',
		enableDragDrop : true,
		ddGroup : 'pageGroup',
		sm : checkSm,
		store : new Ext.data.Store({
			reader : new Ext.data.JsonReader({
						idProperty : 'id'
					}, pageRecord),
			listeners : {
				'update' : function(store, record, opt) {
					if (opt != Ext.data.Record.EDIT) {
						return;
					}
					Ext.Ajax.request({
								url : contextPath
										+ 'webservice.asmx/AddOrUpdatePage',
								method : 'post',
								params : record.data,
								success : function(response) {
									var result = Ext.DomQuery.selectNode(
											'string', response.responseXML).text;
									if (Ext.isEmpty(result)
											|| !Ext.decode(result).success) {
										Ext.Msg.alert('出错',
												'保存数据出错，暂时无法配置方案，请检查网络或联系管理员。');
										return;
									} else {
										record.commit();
									}
								}
							})
				}
			}
		}),
		columns : [checkSm, {
					header : 'ID',
					dataIndex : 'id',
					width : 30
				}, {
					header : '页面编码',
					dataIndex : 'code',
					width : 150,
					editor : {
						xtype : 'textfield'
					}
				}, {
					header : '名称',
					dataIndex : 'name',
					editor : {
						xtype : 'textfield'
					}
				}, {
					id : 'url',
					header : '链接地址',
					dataIndex : 'url',
					editor : {
						xtype : 'textfield'
					}
				}],
		tbar : [{
					text : '添加',
					iconCls : 'add',
					handler : function() {
						var maxId = 0;
						pageGrid.store.each(function(record) {
									if (record.get('id') > maxId) {
										maxId = record.get('id');
									}
								});
						var r = new pageGrid.store.recordType({
									id : maxId + 1
								});
						pageGrid.store.add([r]);
					}
				}, {
					text : '删除',
					ref : '../delButton',
					iconCls : 'delete',
					disabled : true,
					handler : function() {
						var records = pageGrid.getSelectionModel()
								.getSelections();
						Ext.iterate(records, deletePage);
					}
				}]
	});
	var planGrid = new Ext.grid.EditorGridPanel({
		title : '方案列表',
		flex : 1,
		autoExpandColumn : 'pages',
		selModel : new Ext.grid.RowSelectionModel({
					singleSelect : true,
					listeners : {
						'rowselect' : function(sm, rowIndex, record) {
							planGrid.delButton.setDisabled(false);
							planPageGrid.plan = record;
							planPageGrid.setTitle('【' + record.get('name')
									+ '】页面配置');
							var ids = record.get('pages')
							if (!Ext.isArray(ids)) {
								ids = new Array();
							}
							loadPlanPage(ids);
						}
					}
				}),
		store : new Ext.data.Store({
			reader : new Ext.data.JsonReader({}, planRecord),
			listeners : {
				'update' : function(store, record, opt) {
					if (opt != Ext.data.Record.EDIT) {
						return;
					}
					Ext.Ajax.request({
								url : contextPath
										+ 'webservice.asmx/AddOrUpdatePlan',
								method : 'post',
								params : {
									id : record.data.id,
									name : record.data.name,
									pageid : record.data.pages.join(",")
								},
								success : function(response) {
									var result = Ext.DomQuery.selectNode(
											'string', response.responseXML).text;
									if (Ext.isEmpty(result)
											|| !Ext.decode(result).success) {
										Ext.Msg.alert('出错',
												'保存数据出错，暂时无法配置方案，请检查网络或联系管理员。');
										return;
									}else{
										record.commit();
									}
								}
							})
				}
			}
		}),
		columns : [{
					header : 'ID',
					dataIndex : 'id',
					width : 30
				}, {
					header : '名称',
					dataIndex : 'name',
					editor : {
						xtype : 'textfield'
					}
				}, {
					id : 'pages',
					header : '页面配置',
					dataIndex : 'pages',
					renderer : function(value) {
						return Ext.isArray(value) ? value.join(',') : '';
					}
				}],
		tbar : [{
					text : '添加',
					iconCls : 'add',
					handler : function() {
						var maxId = 0;
						planGrid.store.each(function(record) {
									if (record.get('id') > maxId) {
										maxId = record.get('id');
									}
								});
						var r = new planGrid.store.recordType({
									id : maxId + 1
								});
						planGrid.store.add([r]);
					}
				}, {
					text : '删除',
					iconCls : 'delete',
					ref : '../delButton',
					disabled : true,
					handler : function() {
						var sm = planGrid.getSelectionModel();
						var record = sm.getSelected();
						Ext.Msg.confirm('提示', '确定要删除这个页面吗？',
								function(buttonId) {
									if (buttonId != 'yes') {
										return;
									}
									Ext.Ajax.request({
										url : contextPath
												+ 'webservice.asmx/DeletePlan',
										method : 'post',
										params : {
											id : record.data.id
										},
										success : function(response) {
											var result = Ext.DomQuery
													.selectNode(
															'string',
															response.responseXML).text;
											if (Ext.isEmpty(result)
													|| !Ext.decode(result).success) {
												Ext.Msg
														.alert('出错',
																'删除数据出错，暂时无法配置方案，请检查网络或联系管理员。');
												return;
											} else {
												Ext.Msg.alert('成功', '删除成功。');
												planGrid.store.remove(record);
												sm.selectRow(0);
											}
										}
									})
								});
					}
				}]
	});
	var planPageGrid = new Ext.grid.GridPanel({
				title : '方案配置',
				flex : 3,
				autoExpandColumn : 'url',
				ddGroup : 'pageGroup',
				enableDragDrop : true,
				selModel : new Ext.grid.RowSelectionModel({
							listeners : {
								'rowselect' : function() {
									planPageGrid.delButton.setDisabled(false);
								}
							}
						}),
				store : new Ext.data.Store({
							reader : new Ext.data.JsonReader({}, pageRecord)
						}),
				columns : [new Ext.grid.RowNumberer(), {
							header : 'ID',
							dataIndex : 'id',
							width : 30
						}, {
							header : '页面编码',
							dataIndex : 'code',
							width : 150
						}, {
							header : '名称',
							dataIndex : 'name'
						}, {
							id : 'url',
							header : '链接地址',
							dataIndex : 'url'
						}],
				tbar : [{
					text : '删除',
					ref : '../delButton',
					iconCls : 'delete',
					disabled : true,
					handler : function() {
						var records = planPageGrid.getSelectionModel()
								.getSelections();
						planPageGrid.store.remove(records);
						planPageStoreChange();
					}
				}]
			});
	var navPanel = new Ext.ux.NavButtonPanel({
				imagePath : '../images/',
				width : 20,
				scope : this,
				// 右键头
				fromTo : function() {
					var records = pageGrid.getSelectionModel().getSelections();
					if (records.length == 0) {
						return;
					}
					addPlanPage(records);
				},
				toFrom : planPageGrid.delButton.handler.createCallback(),
				toTop : function() {
					var records = planPageGrid.getSelectionModel()
							.getSelections();
					if (records.length == 0) {
						return;
					}
					moveRecords(planPageGrid, records, 0);
				},
				up : function() {
					var records = planPageGrid.getSelectionModel()
							.getSelections();
					if (records.length == 0) {
						return;
					}
					var index = planPageGrid.store.indexOf(records[0]) - 1;
					moveRecords(planPageGrid, records, index);
				},
				down : function() {
					var records = planPageGrid.getSelectionModel()
							.getSelections();
					if (records.length == 0) {
						return;
					}
					var index = planPageGrid.store.indexOf(records[0]) + 1;
					moveRecords(planPageGrid, records, index);
				},
				toBottom : function() {
					var records = planPageGrid.getSelectionModel()
							.getSelections();
					if (records.length == 0) {
						return;
					}
					moveRecords(planPageGrid, records, pageGrid.store
									.getCount());
				}
			});
	new Ext.Viewport({
				layout : 'border',
				items : {
					xtype : 'panel',
					region : 'center',
					layout : {
						align : 'stretch',
						type : 'hbox',
						padding : 5
					},
					items : [pageGrid, navPanel, {
								xtype : 'panel',
								border : false,
								layout : {
									align : 'stretch',
									type : 'vbox'
								},
								flex : 1,
								items : [planGrid, planPageGrid]
							}]
				}
			});

	var dt = new Ext.dd.DropTarget(planPageGrid.getView().scroller.dom, {
				ddGroup : 'pageGroup',
				notifyDrop : function(ddSource, e, data) {
					if (ddSource.dragData.grid === planPageGrid) {
						var records = ddSource.dragData.selections;
						var index = ddSource.getDragData(e) ? ddSource
								.getDragData(e).rowIndex : planPageGrid.store
								.getCount()
								- records.length;
						moveRecords(planPageGrid, records, index)
						return true;
					} else if (ddSource.dragData.grid === pageGrid) {
						var records = ddSource.dragData.selections;
						addPlanPage(records);
						return true;
					}
				}
			});

	// 加载数据
	loadDatas();
	function loadDatas() {
		Ext.Msg.wait('正在加载数据，请稍候...', '提示');
		Ext.Ajax.request({
					method : 'post',
					url : contextPath + 'webservice.asmx/GetPlanPageString',
					success : function(response) {
						var result = Ext.DomQuery.selectNode('string',
								response.responseXML).text;
						if (Ext.isEmpty(result)) {
							Ext.Msg.alert('出错', '加载数据出错，请刷新页面重试。');
							return;
						}
						var json = Ext.decode(result);
						json.pages.shift();
						pageGrid.store.loadData(json.pages);
						planGrid.store.loadData(json.plans);
						Ext.Msg.hide();
					}
				});
	}
	function addPlanPage(records, notUpdatePlan) {
		if (Ext.isEmpty(records)) {
			return;
		}
		if (Ext.isEmpty(planPageGrid.plan)) {
			Ext.Msg.alert('提示', '请先选择方案');
			return;
		}
		Ext.iterate(records, function(record) {
					var rec = record.copy(Ext.id());
					planPageGrid.store.add([rec]);
				});
		if (!notUpdatePlan) {
			planPageStoreChange();
		}
	}
	function planPageStoreChange() {
		var ids = new Array();
		planPageGrid.store.each(function(record) {
					ids.push(record.get('id'));
				});
		if (planPageGrid.plan) {
			planPageGrid.plan.set('pages', ids);
		}
	}
	function loadPlanPage(pageIds) {
		var records = new Array();
		Ext.iterate(pageIds, function(pageId) {
					records.push(pageGrid.store.getById(pageId));
				});
		planPageGrid.store.removeAll();
		addPlanPage(records, true);
	}
	function moveRecords(grid, records, targetIndex) {
		var ds = grid.store;
		ds.remove(records);
		ds.insert(targetIndex, records);
		grid.view.refresh();
		grid.getSelectionModel().selectRecords(records);
		planPageStoreChange();
	}
	function deletePage(record) {
		for (var i = 0; i < planGrid.store.getCount(); i++) {
			var r = planGrid.store.getAt(i);
			if (r.get('pages').indexOf(record.get('id')) > -1) {
				Ext.Msg.alert('提示', '该页面有与其关联的方案，无法删除。请先在方案中删除该页面的配置。');
				return;
			}
		}
		Ext.Msg.confirm('提示', '确定要删除这个页面吗？', function(buttonId) {
					if (buttonId != 'yes') {
						return;
					}
					Ext.Ajax.request({
								url : contextPath
										+ 'webservice.asmx/DeletePage',
								method : 'post',
								params : {
									id : record.data.id
								},
								success : function(response) {
									var result = Ext.DomQuery.selectNode(
											'string', response.responseXML).text;
									if (Ext.isEmpty(result)
											|| !Ext.decode(result).success) {
										Ext.Msg.alert('出错',
												'删除数据出错，暂时无法配置方案，请检查网络或联系管理员。');
										return;
									} else {
										Ext.Msg.alert('成功', '删除成功。');
										pageGrid.store.remove(record);
									}
								}
							})
				});
	}
});