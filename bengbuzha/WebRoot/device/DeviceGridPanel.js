Ext.ns('com.bitc.xmis.Records');
com.bitc.xmis.Records.DeviceInfoRecord = Ext.data.Record.create([{
			name : 'id'
		}, {
			name : 'name'
		}, {
			name : 'modelType'
		}, {
			name : 'standard'
		}, {
			name : 'weight'
		}, {
			name : 'power'
		}, {
			name : 'referencePrice',
			type : 'float'
		}, 'currency', 'lastModifier', {
			name : 'entryDate',
			type : 'date',
			dateFormat : 'Y-m-d H:i:s'
		}, {
			name : 'guarantees'
		}]);
Ext.ns('com.bitc.xmis');
com.bitc.xmis.DeviceGridPanel = Ext.extend(Ext.grid.GridPanel, {
	initComponent : function() {
		this.store = new Ext.data.Store({
					url : String.format('{0}/device.do?method=loadDeviceInfo',
							__cxtPath),
					autoLoad : true,
					reader : new Ext.data.JsonReader({
								idProperty : 'id',
								root : 'rows',
								totalProperty : 'totalCount'
							}, com.bitc.xmis.Records.DeviceInfoRecord),
					baseParams : {
						limit : 20
					}
				});
		this.columns = [{
					header : '名称',
					dataIndex : 'name'
				}, {
					header : '型号',
					dataIndex : 'modelType'
				}, {
					header : '规格',
					dataIndex : 'standard'
				}, {
					header : '重量',
					dataIndex : 'weight'
				}, {
					header : '功率',
					dataIndex : 'power'
				}, {
					header : '参考价格',
					dataIndex : 'referencePrice',
					renderer : function(value, metaData, record) {
						var symbol = record.get('currency.symbol');
						return Ext.util.Format.number(value, symbol + '0,0.00');
					}
				}, {
					header : '三包信息',
					dataIndex : 'guarantees'
				}, {
					header : '最后更新人',
					dataIndex : 'lastModifier.userName'
				}, {
					header : '最后更新时间',
					xtype : 'datecolumn',
					dataIndex : 'entryDate',
					format : 'Y-m-d'
				}];
		this.tbar = ['模糊查询：', new Ext.ux.InklingSearchField({
							width : 120,
							store : this.store,
							tooltip : {
								title : '模糊查询',
								text : '输入设备名称进行查询。'
							},
							inklings : [{
										name : 'name',
										op : 'like',
										type : 'string'
									}]
						})];
		this.bbar = new Ext.PagingToolbar({
					pageSize : 20,
					store : this.store,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据"
				});
		com.bitc.xmis.DeviceGridPanel.superclass.initComponent.call(this);
	}
});