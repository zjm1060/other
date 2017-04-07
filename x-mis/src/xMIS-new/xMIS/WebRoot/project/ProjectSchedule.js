Ext.ns('com.bitc.xmis.project');
com.bitc.xmis.project.ProjectSchedulePlan = {};
/**
 * 项目进度计划Record
 */
com.bitc.xmis.project.ProjectSchedulePlan.Record = Ext.data.Record.create([
		'id', 'project', 'status', {
			name : 'startDate',
			type : 'date',
			dateFormat : 'Y-m-d H:i:s'
		}, {
			name : 'statusId',
			mapping : 'status',
			convert : function(v) {
				return v.id;
			}
		}, {
			name : 'statusName',
			mapping : 'status',
			convert : function(v) {
				return v.name;
			}
		}, {
			name : 'actualStartDate',
			type : 'date',
			dateFormat : 'Y-m-d H:i:s'
		}]);
/**
 * @class com.bitc.xmis.project.ProjectSchedulePlanGrid
 * @extends Ext.grid.EditorGridPanel 项目进度计划EditorGrid
 */
com.bitc.xmis.project.ProjectSchedulePlanGrid = Ext.extend(
		Ext.grid.EditorGridPanel, {
			/**
			 * @cfg {String} projectId 项目ID
			 * @requires
			 */
			/**
			 * @cfg {Boolean} showActualStartDate 是否显示实际开始日期列。(defaults to true)
			 */
			showActualStartDate : true,
			/**
			 * @cfg {Boolean} checkable 是否有选择框。defaults to false
			 */
			checkable : false,
			/**
			 * @cfg {Boolean} editable 是否允许编辑
			 */
			editable : true,
			/**
			 * @cfg {Array} editableColumns 允许编辑的列。以dataIndex为标识
			 */
			editableColumns : [],
			clicksToEdit : 1,
			viewConfig : {
				getRowClass : function(record, index, rowParam, store) {
					if (record.get('actualStartDate') != null
							&& new Date() >= record.get('actualStartDate')) {
						return 'font-lightgreen';
					}
				}
			},
			initComponent : function() {
				this.store = new Ext.data.Store({
					url : String.format(
							'{0}/projectMain.do?method=getProjectSchedulePlan',
							__cxtPath),
					sortInfo : {
						field : 'startDate',
						direction : 'ASC'
					},
					pruneModifiedRecords : true,
					reader : new Ext.data.JsonReader({
								idProperty : 'id',
								root : 'rows',
								totalProperty : 'totalCount'
							}, com.bitc.xmis.project.ProjectSchedulePlan.Record),
					baseParams : {
						projectId : this.projectId
					}
				});
				var columns = this.checkable
						? [new Ext.grid.CheckboxSelectionModel({
									singleSelect : true
								})]
						: [];
				columns = columns.concat([new Ext.grid.RowNumberer(),{
							header : '节点名称',
							dataIndex : 'status.name',
							width : 120,
							editor : {
								xtype : 'textfield'
							}
						}, {
							id : 'stauts_description',
							header : '节点描述',
							dataIndex : 'status.description',
							width:300,
							editor : {
								xtype : 'textarea',
								width : 200
							}
						}, {
							header : '预计进入日期',
							xtype : 'datecolumn',
							dataIndex : 'startDate',
							format : 'Y-m-d',
							sortable : true,
							editor : new Ext.form.DateField({
										format : 'Y-m-d'
									})
						}]);
				if (this.showActualStartDate) {
					columns.push({
								header : '实际进入日期',
								xtype : 'datecolumn',
								dataIndex : 'actualStartDate',
								format : 'Y-m-d',
								sortable : true,
								editor : new Ext.form.DateField({
											format : 'Y-m-d'
										})
							});
				}
				columns.push({
							header : '本项目专用',
							dataIndex : 'status.custom',
							hidden : false,
							sortable : true,
							width : 80,
							renderer : function(value) {
								if (value) {
									return "是";
								} else {
									return "否";
								}
							}
						});
				this.colModel = new Ext.grid.ColumnModel({
							columns : columns
						});
				//this.autoExpandColumn = 'stauts_description';
				com.bitc.xmis.project.ProjectSchedulePlanGrid.superclass.initComponent
						.call(this);
				for (var i = 0; i < columns.length; i++) {
					var col = columns[i];
					var e = this.editable
							&& col.editor
							&& (this.editableColumns.length == 0 || this.editableColumns
									.indexOf(col.dataIndex) > -1);
					this.colModel.setEditable(i, e);
				}
				if (this.editable) {
					this.on('beforeedit', function(e) {
								if (e.record.get('status.custom')) {
									return true;
								}
								if (e.field == 'actualStartDate'
										|| e.field == 'startDate') {
									return true;
								}
								return false;
							}, this);
					this.on('afteredit', function(e) {
								if (e.field == 'startDate') {
									this.store.sort('startDate', 'ASC');
								};
							}, this);
				}
				this.on('click', function(e) {
							if (e.getTarget().className == 'x-grid3-scroller') {
								this.getSelectionModel().clearSelections();
							}
						}, this);
			}
		});

com.bitc.xmis.project.ProjectStatusGrid = Ext.extend(Ext.grid.GridPanel, {
	autoExpandColumn : 'status-description',
	stripeRows : true,
	clicksToEdit : 1,
	/**
	 * @cfg {Boolean} editable 是否允许编辑
	 */
	editable : true,
	/**
	 * @cfg {Array} editableColumns 允许编辑的列。以dataIndex为标识
	 */
	editableColumns : [],
	initComponent : function() {
		this.store = new Ext.data.Store({
					url : String
							.format(
									'{0}/projectMain.do?method=getDefaultProjectStatus',
									__cxtPath),
					reader : new Ext.data.JsonReader({
								root : 'rows',
								totalProperty : 'totalCount',
								fields : ['id', 'name', 'description', 'custom']
							})
				});
		var columns = [{
					header : '节点名称',
					dataIndex : 'name',
					sortable : true,
					editor : {
						xtype : 'textfield'
					}
				}, {
					id : 'status-description',
					header : '节点描述',
					dataIndex : 'description',
					sortable : true,
					width:300,
					editor : {
						xtype : 'textarea',
						width : 200
					}
				}];
		this.colModel = new Ext.grid.ColumnModel({
					columns : columns
				});
		com.bitc.xmis.project.ProjectStatusGrid.superclass.initComponent
				.call(this);
		// this.on('beforeedit', function(e) {
		// if (!e.record.get('custom')) {
		// return false
		// }
		// return true;
		// }, this);
		// for (var i = 0; i < columns.length; i++) {
		// var col = columns[i];
		// var e = this.editable
		// && col.editor
		// && (this.editableColumns.length == 0 || this.editableColumns
		// .indexOf(col.dataIndex) > -1);
		// this.colModel.setEditable(i, e);
		// }
	}

});

com.bitc.xmis.project.ProjectScheduleEditPanel = Ext.extend(Ext.Panel, {
	layout : 'hbox',
	layoutConfig : {
		align : 'stretch',
    	pack  : 'start'
	},
	plain : true,
	statusTitle : '',
	scheduleTitle : '',
	initComponent : function() {
		var defaultStatusGrid = this.defaultStatusGrid = new com.bitc.xmis.project.ProjectStatusGrid(
				{
					projectId : this.projectId,
					enableDragDrop : true,
					title : this.statusTitle || '标准项目节点',
					selModel : new Ext.grid.RowSelectionModel(),
					flex:1,
					ddGroup : 'statusGroup'
				});
		var schedulePlanGrid = this.schedulePlanGrid = new com.bitc.xmis.project.ProjectSchedulePlanGrid(
				{
					title : this.scheduleTitle || '项目进度计划',
					projectId : this.projectId,
					editable : true,
					flex : 3,
					checkable : this.checkable,
					selModel : new Ext.grid.RowSelectionModel({
								listeners : {
									'rowselect' : function() {
										schedulePlanGrid.delButton
												.setDisabled(false);
									}
								}
							}),
					tbar : [{
								text : '保存',
								iconCls : 'save',
								scope : this,
								handler : this.savePlan
							}, {
								text : '添加自定义节点',
								iconCls : 'add',
								scope : this,
								handler : function() {
									var R = schedulePlanGrid.store.recordType;
									var r = new R({
												id : null,
												project : {
													projectID : this.projectId
												},
												status : {
													id : null,
													name : '',
													description : '',
													custom : true
												},
												startDate : null
											});
									schedulePlanGrid.store.add([r]);
									schedulePlanGrid.startEditing(
											schedulePlanGrid.store.getCount()
													- 1, 0);
								}
							}, {
								text : '删除',
								ref : '../delButton',
								iconCls : 'delete',
								// tooltip : '只可删除还未保存的计划',
								disabled : true,
								handler : function() {
									var records = schedulePlanGrid
											.getSelectionModel()
											.getSelections();
									Ext.Msg.confirm('确认', String.format(
													'确定删除这{0}条记录吗',
													records.length), function(
													buttonId) {
												if (buttonId == 'yes') {
													schedulePlanGrid.store
															.remove(records);
												}
											});
								}
							}],
					bbar : ['可从右边拖曳标准项目节点，也可添加自定义节点。点击单元格可编辑内容']
				});
		defaultStatusGrid.store.on('update', function(store, record, opt) {
					if (opt != Ext.data.Record.EDIT) {
						return;
					}
					var url = String.format(
							'{0}/projectMain.do?method=saveProjectStatus',
							__cxtPath);
					var params = Ext.apply({
								projectId : this.projectId
							}, record.data);
					Ext.Ajax.request({
								method : 'post',
								url : url,
								params : params,
								success : function(response) {
									var result = Ext
											.decode(response.responseText);
									if (result.success) {
										record.data.id = result.attributes.id
									}
								},
								failure : function(response) {
								}
							});
				}, this);
		schedulePlanGrid.store.on('remove', function(store, record, index) {
			if (record.data.id != null) {
				Ext.Ajax.request({
					method : 'post',
					url : String
							.format(
									'{0}/projectMain.do?method=deleteProjectSchedulePlan',
									__cxtPath),
					params : {
						projectSchedulePlanId : record.data.id
					},
					success : function(response) {
						var result = Ext.decode(response.responseText);
						if (!result.success) {
							Ext.Msg.alert(result.messageTitle, result.message,
									function() {
										schedulePlanGrid.store.reload();
									});
						}
					}
				});
			}
		});
		// schedulePlanGrid.store.on('load', function(store, records) {
		// Ext.iterate(records, function(item) {
		// var r = defaultStatusGrid.store
		// .getById(item.data.status.id);
		// defaultStatusGrid.store.remove(r);
		// });
		// });
		defaultStatusGrid.store.load({});
		schedulePlanGrid.store.load({});
		var navButtonPanel = new Ext.ux.NavButtonPanel({
					imagePath : __cxtPath + '/resources/images/',
					width : 20,
					drawTopIcon : false,
					drawBotIcon : false,
					drawUpIcon : false,
					drawDownIcon : false,
					scope : this,
					// 右键头
					fromTo : function() {
						var records = schedulePlanGrid.getSelectionModel()
								.getSelections();
						this.moveToStatus(records);
					},
					toFrom : function() {
						var records = defaultStatusGrid.getSelectionModel()
								.getSelections();
						this.moveToSchedulePlan(records);
					}
				});
		this.items = [schedulePlanGrid, navButtonPanel, defaultStatusGrid];
		com.bitc.xmis.project.ProjectScheduleEditPanel.superclass.initComponent
				.call(this);
	},
	moveToSchedulePlan : function(records) {
		var results = [];
		for (var i = 0; i < records.length; i++) {
			var item = records[i];
			for (var j = 0; j < this.schedulePlanGrid.store.getCount(); j++) {
				var r = this.schedulePlanGrid.store.getAt(j);
				if (item.data.id === r.data.status.id) {
					Ext.Msg.alert('提示', '进度计划中已有该节点，不能重复添加');
					return false;
				}
			}
			var record = new com.bitc.xmis.project.ProjectSchedulePlan.Record({
						id : null,
						project : {
							projectID : this.projectId
						},
						status : item.data,
						startDate : null
					});
			results.push(record);
		}
		this.schedulePlanGrid.store.add(results);
		if (results.length > 0) {
			this.schedulePlanGrid.startEditing(this.schedulePlanGrid.store
							.getCount()
							- 1, 2);
			if(this.schedulePlanGrid.activeEditor&&this.schedulePlanGrid.activeEditor!=null){
				this.schedulePlanGrid.activeEditor.field.onTriggerClick();
			}
		}
	},
	moveToStatus : function(records) {
		Ext.iterate(records, function(record) {
			this.schedulePlanGrid.store.remove(record);
			var status = new this.defaultStatusGrid.store.recordType(record.data.status);
			this.defaultStatusGrid.store.add(status);
		}, this);
	},
	initDragDrop : function(a, b) {
		var notify = function(ddSource, e, data) {
			var records = ddSource.dragData.selections;
			this.moveToSchedulePlan(records);
			return true
		};
		new Ext.dd.DropTarget(this.schedulePlanGrid.getView().scroller.dom, {
					ddGroup : 'statusGroup',
					notifyDrop : notify.createDelegate(this)
				});
	},
	savePlan : function() {
		var schedulePlanGrid = this.schedulePlanGrid;
		for (var i = 0; i < schedulePlanGrid.store.getCount(); i++) {
			var r = schedulePlanGrid.store.getAt(i);
			if (r.data.startDate == null || Ext.isEmpty(r.data.startDate)) {
				Ext.Msg.alert('提示', '请先设置所有节点的预计进入日期再保存！');
				return;
			}
		}
		var plans = com.bitc.xmis.util.ExtUtil
				.encodeStoreModifiedData(schedulePlanGrid.store);
		Ext.Ajax.request({
					url : String
							.format(
									'{0}/projectMain.do?method=saveProjectSchedulePlan',
									__cxtPath),
					params : {
						projectId : this.projectId,
						plans : plans
					},
					success : function(response) {
						var act = Ext.decode(response.responseText);
						if (act.success) {
							schedulePlanGrid.store.reload();
						}
						Ext.Msg.alert(act.messageTitle, act.message);
					},
					failure : function(form, act) {
						com.bitc.xmis.util.AlertUtil.formSubmitFailedAlert(act);
					},
					waitMsg : '正在保存，请稍候……',
					waitTitle : '保存'
				});
	}
});
