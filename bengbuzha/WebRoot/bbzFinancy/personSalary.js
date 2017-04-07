Ext.ns('com.bitc.xmis.gridPanel');
/**
 * 生成各个工资项的grid
 */


com.bitc.xmis.gridPanel.SalaryGridForType = Ext.extend(Ext.grid.GridPanel, {
	month : new Date().getMonth() + 1,
	type : 1,
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
					name : 'imonth',
					mapping : 'imonth'
				}, {
					name : 'type_id',
					mapping : 'type_id'
				}, {
					name : 'sheet_uuid',
					mapping : 'sheet_uuid'
				}, {
					name : 'name_column',
					mapping : 'name_column'
				}, {
					name : 'salary_column',
					mapping : 'salary_column'
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
				}, {
					name : 'column_31',
					mapping : 'column_31'
				}, {
					name : 'column_32',
					mapping : 'column_32'
				}, {
					name : 'column_33',
					mapping : 'column_33'
				}, {
					name : 'column_34',
					mapping : 'column_34'
				}, {
					name : 'column_35',
					mapping : 'column_35'
				}, {
					name : 'column_36',
					mapping : 'column_36'
				}, {
					name : 'column_37',
					mapping : 'column_37'
				}, {
					name : 'column_38',
					mapping : 'column_38'
				}, {
					name : 'column_39',
					mapping : 'column_39'
				}, {
					name : 'column_40',
					mapping : 'column_40'
				}, {
					name : 'column_41',
					mapping : 'column_41'
				}, {
					name : 'column_42',
					mapping : 'column_42'
				}, {
					name : 'column_43',
					mapping : 'column_43'
				}, {
					name : 'column_44',
					mapping : 'column_44'
				}, {
					name : 'column_45',
					mapping : 'column_45'
				}, {
					name : 'column_46',
					mapping : 'column_46'
				}, {
					name : 'column_47',
					mapping : 'column_47'
				}, {
					name : 'column_48',
					mapping : 'column_48'
				}, {
					name : 'column_49',
					mapping : 'column_49'
				}, {
					name : 'column_50',
					mapping : 'column_50'
				}, {
					name : 'column_51',
					mapping : 'column_51'
				}, {
					name : 'column_52',
					mapping : 'column_52'
				}, {
					name : 'column_53',
					mapping : 'column_53'
				}, {
					name : 'column_54',
					mapping : 'column_54'
				}, {
					name : 'column_55',
					mapping : 'column_55'
				}, {
					name : 'column_56',
					mapping : 'column_56'
				}, {
					name : 'column_57',
					mapping : 'column_57'
				}, {
					name : 'column_58',
					mapping : 'column_58'
				}, {
					name : 'column_59',
					mapping : 'column_59'
				}, {
					name : 'column_60',
					mapping : 'column_60'
				}]);

		this.store = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/bbzFinancyMain.do?method=getPersonForOneSalaryType'
					}),
			baseParams : {
				start : 0,
				limit : pageSize,
				year : this.year,
				month : this.month,
				type : this.type
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
						for (var i = 1; i <= 60; i++) {
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
		
		var column = new Ext.grid.ColumnModel([ {
					header : 'column_1',
					dataIndex : 'column_1',
					width : 80,
					sortable : true
				}, {
					header : 'column_2',
					dataIndex : 'column_2',
					width : 80,
					sortable : true
				}, {
					header : 'column_3',
					dataIndex : 'column_3',
					width : 80,
					sortable : true
				}, {
					header : 'column_4',
					dataIndex : 'column_4',
					width : 80,
					sortable : true
				}, {
					header : 'column_5',
					dataIndex : 'column_5',
					width : 80,
					sortable : true
				}, {
					header : 'column_6',
					dataIndex : 'column_6',
					width : 80,
					sortable : true
				}, {
					header : 'column_7',
					dataIndex : 'column_7',
					width : 80,
					sortable : true
				}, {
					header : 'column_8',
					dataIndex : 'column_8',
					width : 80,
					sortable : true
				}, {
					header : 'column_9',
					dataIndex : 'column_9',
					width : 80,
					sortable : true
				}, {
					header : 'column_10',
					dataIndex : 'column_10',
					width : 80,
					sortable : true
				}, {
					header : 'column_11',
					dataIndex : 'column_11',
					width : 80,
					sortable : true
				}, {
					header : 'column_12',
					dataIndex : 'column_12',
					width : 80,
					sortable : true
				}, {
					header : 'column_13',
					dataIndex : 'column_13',
					width : 80,
					sortable : true
				}, {
					header : 'column_14',
					dataIndex : 'column_14',
					width : 80,
					sortable : true
				}, {
					header : 'column_15',
					dataIndex : 'column_15',
					width : 80,
					sortable : true
				}, {
					header : 'column_16',
					dataIndex : 'column_16',
					width : 80,
					sortable : true
				}, {
					header : 'column_17',
					dataIndex : 'column_17',
					width : 80,
					sortable : true
				}, {
					header : 'column_18',
					dataIndex : 'column_18',
					width : 80,
					sortable : true
				}, {
					header : 'column_19',
					dataIndex : 'column_19',
					width : 80,
					sortable : true
				}, {
					header : 'column_20',
					dataIndex : 'column_20',
					width : 80,
					sortable : true
				}, {
					header : 'column_21',
					dataIndex : 'column_21',
					width : 80,
					sortable : true
				}, {
					header : 'column_22',
					dataIndex : 'column_22',
					width : 80,
					sortable : true
				}, {
					header : 'column_23',
					dataIndex : 'column_23',
					width : 80,
					sortable : true
				}, {
					header : 'column_24',
					dataIndex : 'column_24',
					width : 80,
					sortable : true
				}, {
					header : 'column_25',
					dataIndex : 'column_25',
					width : 80,
					sortable : true
				}, {
					header : 'column_26',
					dataIndex : 'column_26',
					width : 80,
					sortable : true
				}, {
					header : 'column_27',
					dataIndex : 'column_27',
					width : 80,
					sortable : true
				}, {
					header : 'column_28',
					dataIndex : 'column_28',
					width : 80,
					sortable : true
				}, {
					header : 'column_29',
					dataIndex : 'column_29',
					width : 80,
					sortable : true
				}, {
					header : 'column_30',
					dataIndex : 'column_30',
					width : 80,
					sortable : true
				}, {
					header : 'column_31',
					dataIndex : 'column_31',
					width : 80,
					sortable : true
				}, {
					header : 'column_32',
					dataIndex : 'column_32',
					width : 80,
					sortable : true
				}, {
					header : 'column_33',
					dataIndex : 'column_33',
					width : 80,
					sortable : true
				}, {
					header : 'column_34',
					dataIndex : 'column_34',
					width : 80,
					sortable : true
				}, {
					header : 'column_35',
					dataIndex : 'column_35',
					width : 80,
					sortable : true
				}, {
					header : 'column_36',
					dataIndex : 'column_36',
					width : 80,
					sortable : true
				}, {
					header : 'column_37',
					dataIndex : 'column_37',
					width : 80,
					sortable : true
				}, {
					header : 'column_38',
					dataIndex : 'column_38',
					width : 80,
					sortable : true
				}, {
					header : 'column_39',
					dataIndex : 'column_39',
					width : 80,
					sortable : true
				}, {
					header : 'column_40',
					dataIndex : 'column_40',
					width : 80,
					sortable : true
				}, {
					header : 'column_41',
					dataIndex : 'column_41',
					width : 80,
					sortable : true
				}, {
					header : 'column_42',
					dataIndex : 'column_42',
					width : 80,
					sortable : true
				}, {
					header : 'column_43',
					dataIndex : 'column_43',
					width : 80,
					sortable : true
				}, {
					header : 'column_44',
					dataIndex : 'column_44',
					width : 80,
					sortable : true
				}, {
					header : 'column_45',
					dataIndex : 'column_45',
					width : 80,
					sortable : true
				}, {
					header : 'column_46',
					dataIndex : 'column_46',
					width : 80,
					sortable : true
				}, {
					header : 'column_47',
					dataIndex : 'column_47',
					width : 80,
					sortable : true
				}, {
					header : 'column_48',
					dataIndex : 'column_48',
					width : 80,
					sortable : true
				}, {
					header : 'column_49',
					dataIndex : 'column_49',
					width : 80,
					sortable : true
				}, {
					header : 'column_50',
					dataIndex : 'column_50',
					width : 80,
					sortable : true
				}, {
					header : 'column_51',
					dataIndex : 'column_51',
					width : 80,
					sortable : true
				}, {
					header : 'column_52',
					dataIndex : 'column_52',
					width : 80,
					sortable : true
				}, {
					header : 'column_53',
					dataIndex : 'column_53',
					width : 80,
					sortable : true
				}, {
					header : 'column_54',
					dataIndex : 'column_54',
					width : 80,
					sortable : true
				}, {
					header : 'column_55',
					dataIndex : 'column_55',
					width : 80,
					sortable : true
				}, {
					header : 'column_56',
					dataIndex : 'column_56',
					width : 80,
					sortable : true
				}, {
					header : 'column_57',
					dataIndex : 'column_57',
					width : 80,
					sortable : true
				}, {
					header : 'column_58',
					dataIndex : 'column_58',
					width : 80,
					sortable : true
				}, {
					header : 'column_59',
					dataIndex : 'column_59',
					width : 80,
					sortable : true
				}, {
					header : 'column_60',
					dataIndex : 'column_60',
					width : 80,
					sortable : true
				}]);

		this.id = this.id;
		this.name = this.name;
		this.height = 100;
		this.title = this.title;
		this.cm = column;
		this.hideHeaders = true;
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
	 * 年份，月份选择panel
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
						columnWidth : 0.3,
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
															+ '/bbzFinancyMain.do?method=getSalaryYearList'
												}),
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
						labelWidth : 60,
						columnWidth : 0.3,
						border : false,
						labelAlign : 'right',
						items : [{
									xtype : 'combo',
									fieldLabel : '月份',
									id : 'monthCom',
									hiddenName : 'month',
									displayField : 'imonth',
									valueField : 'imonth',
									mode : 'local',
									triggerAction : 'all',
									editable : false,
									value : new Date().getMonth() + 1,// 当月
									store : new Ext.data.ArrayStore({
												fields : [{
															name : 'imonth'
														}],
												data : []
											}),
									listeners : {
										'beforerender' : function() {
											var monthlist = [];
											for (var i = 1; i <= 12; i++) {
												monthlist.push([i]);
											}
											this.store.loadData(monthlist);
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
									fieldLabel : '实发合计',
									xtype : 'displayfield',
									value : 0.00,
									id : 'totalSalary'
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
								var imonth = infoPanel.getForm()
										.findField("month").getValue()

								var total = 0.00;
								Ext.getCmp('baseGridID').reload({
									params : {
										year : iyear,
										month : imonth,
										type : 1
									},
									callback : function() {
										total = getTotalSalary(total,
												'baseGridID');
										Ext.getCmp('totalSalary')
												.setValue(formAmt(total));
									}
								});

								Ext.getCmp('indefiniteGridID').reload({
									params : {
										year : iyear,
										month : imonth,
										type : 2
									},
									callback : function() {
										total = getTotalSalary(total,
												'indefiniteGridID');
										Ext.getCmp('totalSalary')
												.setValue(formAmt(total));
									}
								});

								Ext.getCmp('teaGridID').reload({
									params : {
										year : iyear,
										month : imonth,
										type : 3
									},
									callback : function() {
										total = getTotalSalary(total,
												'teaGridID');
										Ext.getCmp('totalSalary')
												.setValue(formAmt(total));
									}
								});

								Ext.getCmp('workOvertimeGridID').reload({
									params : {
										year : iyear,
										month : imonth,
										type : 4
									},
									callback : function() {
										total = getTotalSalary(total,
												'workOvertimeGridID');
										Ext.getCmp('totalSalary')
												.setValue(formAmt(total));
									}
								});
							}
						}]
					}]
		}]
	});

	/**
	 * 工资信息panel
	 */
	var gridForm = new Ext.form.FormPanel({
				region : 'center',
				padding : 20,
				border : false,
				layout : 'form',
				items : [new com.bitc.xmis.gridPanel.SalaryGridForType({
							year : infoPanel.getForm().findField("year")
									.getValue(),
							month : infoPanel.getForm().findField("month")
									.getValue(),
							type : 1,
							id : 'baseGridID',
							name : 'baseGrid',
							title : '基本工资'
						}), new com.bitc.xmis.gridPanel.SalaryGridForType({
							year : infoPanel.getForm().findField("year")
									.getValue(),
							month : infoPanel.getForm().findField("month")
									.getValue(),
							type : 2,
							id : 'indefiniteGridID',
							name : 'indefiniteGrid',
							title : '活工资'
						}), new com.bitc.xmis.gridPanel.SalaryGridForType({
							year : infoPanel.getForm().findField("year")
									.getValue(),
							month : infoPanel.getForm().findField("month")
									.getValue(),
							type : 3,
							id : 'teaGridID',
							name : 'teaGrid',
							title : '茶叶费'
						}), new com.bitc.xmis.gridPanel.SalaryGridForType({
							year : infoPanel.getForm().findField("year")
									.getValue(),
							month : infoPanel.getForm().findField("month")
									.getValue(),
							type : 4,
							id : 'workOvertimeGridID',
							name : 'workOvertimeGrid',
							title : '加班费'
						})]
			});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [infoPanel, gridForm]
			});

	/**
	 * 获取指定类型工资的实发工资
	 */

	function formAmt(value) {
		return Ext.util.Format.number(value, '000.00');
	}
	/**
	 * 获取实发合计金额
	 */
	function getTotalSalary(total, id) {
		if (Ext.getCmp(id).store.data.length > 0) {
			var data = Ext.getCmp(id).store.data.items[0].data;
			var columnIndex = '';
			for (var i = 1; i <= 60; i++) {
				var col = "column_" + i;
				var value = data[col];
				if (value == '实发' || value == '实发工资' || value == '实发金额') {
					columnIndex = col;
					break;
				}
			}

			var Value = Ext.getCmp(id).store.data.items[1].data[columnIndex];
			return total = parseFloat(total) + parseFloat(Value);
		}
		return total;
	}
});