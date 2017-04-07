Ext.ns('com.bitc.xmis.Records');
com.bitc.xmis.Records.ProjectDevicePanelGridRecord = Ext.data.Record.create([{
			name : '物资ID'
		}, {
			name : '项目ID'
		}, {
			name : '设备名称'
		}, {
			name : '型号'
		}, {
			name : '规格'
		}, {
			name : '重量'
		}, {
			name : '重量单位'
		}, {
			name : 'state'
		}, {
			name : '参考价格',
			type : 'float'
		}, {
			name : '货币符号'
		}, {
			name : '最后修改人'
		}, {
			name : '最后修改日期'
		}]);
Ext.ns('com.bitc.xmis');
com.bitc.xmis.ProjectDevicePanelGrid = Ext.extend(Ext.grid.GridPanel, {
	initComponent : function() {
		this.store = new Ext.data.Store({
			url : String
					.format(
							'{0}/device.do?method=getPorjectDeviceInfoList&projectId={1}',
							__cxtPath, this.projectId),
			autoLoad : true,
			reader : new Ext.data.JsonReader({
						idProperty : 'id',
						root : 'rows',
						totalProperty : 'totalCount'
					}, com.bitc.xmis.Records.ProjectDevicePanelGridRecord),
			baseParams : {
				limit : 20
			}
		});
		var store = this.store;
		this.columns = [new Ext.grid.RowNumberer(), {
					header : '名称',
					sortable : true,
					dataIndex : '设备名称'
				}, {
					header : '型号',
					sortable : true,
					dataIndex : '型号'
				}, {
					header : '规格',
					sortable : true,
					dataIndex : '规格'
				}, {
					header : '重量',
					sortable : true,
					dataIndex : '重量'
				}, {
					header : '状态',
					width : 150,
					dataIndex : 'state'
				}];
		this.actionColumn = [{
			xtype : 'actioncolumn',
			header : '存放地址',
			width : 65,
			items : [{
				iconCls : 'edit',
				tooltip : '存放地址',
				handler : function(grid, rowIndex, colIndex) {
					var rec = grid.getStore().getAt(rowIndex);
					var projectId = rec.get('项目ID');
					var deviceInfoId = rec.get('物资ID');
					var deviceInfoName = rec.get('设备名称');
					var tracePanel = new Ext.grid.GridPanel({
						region : 'center',
						border : false,
						autoScorll : true,
						loadMask : true,
						loadMask : {
							msg : '正在加载数据，请稍侯……'
						},
						stripeRows : true,
						cm : new Ext.grid.ColumnModel([// bsm,
						new Ext.grid.RowNumberer(), {
									dataIndex : 'addressname',
									sortable : true,
									width : 200,
									header : '存储地址'
								}, {
									dataIndex : '存放数量',
									sortable : true,
									width : 100,
									header : '存放数量'
								}]),
						store : new Ext.data.Store({
							proxy : new Ext.data.HttpProxy({
								url : __cxtPath
										+ '/device.do?method=getDeviceAddressList&projectId='
										+ projectId + '&deviceInfoId='
										+ deviceInfoId,
								method : "POST"
							}),
							reader : new Ext.data.JsonReader({
										root : 'rows',
										totalProperty : 'totalCount'
									}, new Ext.data.Record.create([{
												"name" : "ID",
												"type" : "string"
											}, {
												"name" : "存放数量",
												"type" : "int"
											}, {
												"name" : "物资名称",
												"type" : "string"
											}, {
												"name" : "addressname",
												"type" : "string"
											}])),
							autoLoad : true,
							remoteSort : true,
							baseParams : {
								limit : 20
							}
						})
					});
					var traceWin = new Ext.Window({
								title : deviceInfoName + '存储地址信息',
								width : 350,
								height : 150,
								items : tracePanel,
								modal:true,
								layout : 'border'
							});
					traceWin.show();

				}

			}]
		}];
		this.columns = this.columns.concat(this.actionColumn);
		this.tbar = new Ext.Toolbar({
					enableOverflow : true
				});

		this.tbar = new Ext.Toolbar({
					enableOverflow : true,
					items : [{
								xtype : 'button',
								text : '刷新',
								iconCls : 'reload',
								ref : '../refreshButton',
								handler : function() {
									store.reload();
								}
							}]

				});

		com.bitc.xmis.ProjectDevicePanelGrid.superclass.initComponent
				.call(this);
	}
});
