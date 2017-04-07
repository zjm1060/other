Ext.ns("com.bitc.xmis");

com.bitc.xmis.OffcialVehiclesGridRecord = Ext.data.Record.create([{
			name : 'car_id'
		}, {
			name : 'car_No'
		}, {
			name : 'car_brand'
		}, {
			name : 'car_type'
		}]);

com.bitc.xmis.OffcialVehiclesGrid = Ext.extend(Ext.grid.GridPanel, {
	autoLoadData : true,
	initComponent : function() {
		this.store = new Ext.data.Store({
			url : __cxtPath
					+ '/applyManageMaint.do?method=getVehicleManagementListToSelectGrid',
			baseParams : {
				limit : 20
			},
			autoLoad : this.autoLoadData,
			remoteSort : true,
			reader : new Ext.data.JsonReader({
						totalProperty : 'totalCount',
						root : 'rows'
					}, com.bitc.xmis.OffcialVehiclesGridRecord)
		});
		this.columns = [new Ext.grid.RowNumberer({
							width : 25
						}), this.sm, {
					header : '车辆id',
					dataIndex : 'car_id',
					hidden : true,
					width : 230
				}, {
					header : '车型',
					dataIndex : 'car_type',
					sortable : false

				}, {
					header : '车牌号',
					dataIndex : 'car_No',
					sortable : false,
					hidden:true,
					width : 120
				}, {
					header : '品牌名称',
					dataIndex : 'car_brand',
					sortable : false,
					width : 120
				}];
		com.bitc.xmis.OffcialVehiclesGrid.superclass.initComponent.call(this);
	}
});
