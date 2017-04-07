Ext.ns('com.bitc.xmis.gridPanel');
/**
 * 生成各个工资项的grid                 
 */
com.bitc.xmis.gridPanel.SalaryGridForType = Ext.extend(Ext.grid.GridPanel, {
	id : '',
	name : '',
	title : '',
	
	initComponent : function() {
		var grid = this;
		var pageSize = 0;
		var record = new Ext.data.Record.create([{
					name : 'id',
					mapping : 'id'
				}, {
					name : 'iyear',
					mapping : 'iyear'
				}, {
					name : 'sheet_uuid',
					mapping : 'sheet_uuid'
				}, {
					name : 'column_1',
					mapping : 'column_1'
				}, {
					name : 'column_2',
					mapping : 'column_2'
				}, {
					name : 'column_3',
					mapping : 'column_3'
				}, {
					name : 'column_4',
					mapping : 'column_4'
				}, {
					name : 'column_5',
					mapping : 'column_5'
				}, {
					name : 'column_6',
					mapping : 'column_6'
				}, {
					name : 'column_7',
					mapping : 'column_7'
				}, {
					name : 'column_8',
					mapping : 'column_8'
				}, {
					name : 'column_9',
					mapping : 'column_9'
				}, {
					name : 'column_10',
					mapping : 'column_10'
				}, {
					name : 'column_11',
					mapping : 'column_11'
				}, {
					name : 'column_12',
					mapping : 'column_12'
				}, {
					name : 'column_13',
					mapping : 'column_13'
				}, {
					name : 'column_14',
					mapping : 'column_14'
				}, {
					name : 'column_15',
					mapping : 'column_15'
				}, {
					name : 'column_16',
					mapping : 'column_16'
				}, {
					name : 'column_17',
					mapping : 'column_17'
				}, {
					name : 'column_18',
					mapping : 'column_18'
				}, {
					name : 'column_19',
					mapping : 'column_19'
				}, {
					name : 'column_20',
					mapping : 'column_20'
				}, {
					name : 'column_21',
					mapping : 'column_21'
				}, {
					name : 'column_22',
					mapping : 'column_22'
				}, {
					name : 'column_23',
					mapping : 'column_23'
				}, {
					name : 'column_24',
					mapping : 'column_24'
				}, {
					name : 'column_25',
					mapping : 'column_25'
				}, {
					name : 'column_26',
					mapping : 'column_26'
				}, {
					name : 'column_27',
					mapping : 'column_27'
				}, {
					name : 'column_28',
					mapping : 'column_28'
				}, {
					name : 'column_29',
					mapping : 'column_29'
				}, {
					name : 'column_30',
					mapping : 'column_30'
				}]);

		this.store = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/bbzFinancyMain.do?method=getQueryIncome'
					}),
			baseParams : {
				start : 0,
				limit : pageSize,
				year : this.year,
				type : 1
			},
			// autoLoad : true,
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, record),
			listeners : {
				'load' : function(store, records) {
					if (records.length > 0) {
						var data = records[0].data;
						for (var i = 1; i <= 30; i++) {
							var col = "column_" + i;
							var value = data[col];
							// if (value.length == 0 || value == null) {
							if (null == value || "" == value) {

								column.setHidden(column.findColumnIndex(col),
										true);
							}
						}
					}
				}
			}
		});
		var numberRenderer=function(value){
			if(isNaN(value)){
				return value;
			}else{
				return Ext.util.Format.number(value,"0,0.00");
			}
		}
		var column = new Ext.grid.ColumnModel([{
					header : 'A',
					dataIndex : 'column_1',
					width : 100,
					align:'right',
					sortable : true
				}, {
					header : 'B',
					dataIndex : 'column_2',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'C',
					dataIndex : 'column_3',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'D',
					dataIndex : 'column_4',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'E',
					dataIndex : 'column_5',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'F',
					dataIndex : 'column_6',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'G',
					dataIndex : 'column_7',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'H',
					dataIndex : 'column_8',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'I',
					dataIndex : 'column_9',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'J',
					dataIndex : 'column_10',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'K',
					dataIndex : 'column_11',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'L',
					dataIndex : 'column_12',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'M',
					dataIndex : 'column_13',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'N',
					dataIndex : 'column_14',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'O',
					dataIndex : 'column_15',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'P',
					dataIndex : 'column_16',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'Q',
					dataIndex : 'column_17',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'R',
					dataIndex : 'column_18',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'S',
					dataIndex : 'column_19',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'T',
					dataIndex : 'column_20',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'U',
					dataIndex : 'column_21',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'V',
					dataIndex : 'column_22',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'W',
					dataIndex : 'column_23',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'X',
					dataIndex : 'column_24',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'Y',
					dataIndex : 'column_25',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'Z',
					dataIndex : 'column_26',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'AB',
					dataIndex : 'column_27',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'AC',
					dataIndex : 'column_28',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'AD',
					dataIndex : 'column_29',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}, {
					header : 'AE',
					dataIndex : 'column_30',
					width : 100,
					align:'right',
					sortable : true,
					renderer:numberRenderer
				}]);

		this.id = this.id;
		this.name = this.name;
		this.height = 400;
		this.title = this.title;
		this.cm = column;
		this.hideHeaders = false;
		this.loadMask = true;
		this.loadMask = {
			msg : '正在加载数据，请稍候...'
		};
		com.bitc.xmis.gridPanel.SalaryGridForType.superclass.initComponent
				.call(this);
	},
	reload : function(options) {
		this.getStore().reload(options);
	}

});

Ext.onReady(function() {

	Ext.QuickTips.init();
	var pageSize = 0;

	/**
	 * 年份选择panel
	 */
	var infoPanel = new Ext.form.FormPanel({
		region : 'north',
		padding : 20,
		height : 45,
		border : false,
		lableWidth : 100,
		lableAlign : 'right',
		layout : 'form',
		items : [{
			xtype : 'panel',
			layout : 'column',
			border : false,
			width : 800,
			items : [{
						xtype : 'panel',
						layout : 'form',
						columnWidth : 0.4,
						labelWidth : 60,
						border : false,
						labelAlign : 'right',
						items : [{
									xtype : 'combo',
									fieldLabel : '年份',
									id : 'yearCom',
									hiddenName : 'year',
									displayField : 'iyear',
									valueField : 'iyear',
									mode : 'local',
									triggerAction : 'all',
									editable : false,
									store : new Ext.data.Store({
										proxy : new Ext.data.HttpProxy({
													url : __cxtPath
															+ '/bbzFinancyMain.do?method=getBankDYearList'
												}),
										baseParams : {
											type: 1
										},
										 autoLoad : true,
										reader : new Ext.data.JsonReader({
													root : 'rows',
												 fields : [{
													name: 'iyear'
												}]
										}),
										listeners:{
											'load' : function(store,records){
												var  maxYear = records[0].data['iyear'];
												Ext.getCmp('yearCom')
												.setValue(maxYear);
											}
										}
									}),
									listeners : {
										'expand' : function(){
											this.getStore().reload();
										}
									}
								}]
					}, {
						xtype : 'panel',
						layout : 'form',
						columnWidth : 0.2,
						labelWidth : 80,
						border : false,
						labelAlign : 'right',
						items : [{
							xtype : 'button',
							text : '查询',
							handler : function() {
								var iyear = infoPanel.getForm()
										.findField("year").getValue();
								Ext.getCmp('baseGridID').reload({
									params : {
										year : iyear
									}
								});
							}
						}]
					}]
		}]
	});

	/**
	 * 列表信息panel
	 */
	var gridForm = new Ext.form.FormPanel({
				region : 'center',
				padding : 20,
				border : false,
				layout : 'form',
				items : [new com.bitc.xmis.gridPanel.SalaryGridForType({
							year : infoPanel.getForm().findField("year")
									.getValue(),
							id : 'baseGridID',
							name : 'baseGrid',
							title : '收入列表'
						})]
			});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [infoPanel, gridForm]
			});
	
});