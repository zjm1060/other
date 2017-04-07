Ext.ns('com.bitc.xmis');
com.bitc.xmis.ContractStateGroupGrid = Ext.extend(Ext.grid.GridPanel, {

	width:800,
	compareYear: '',
	initComponent : function() {
		this.store = new Ext.data.GroupingStore({
			groupField : '部门',
			url : __cxtPath + '/bbzFinancyMain.do?method=getPayList',
			baseParams : {
				year : this.year,
				compareYear : this.compareYear
			},
			remoteSort: true ,
			reader : new Ext.data.JsonReader({
				root : 'rows',
				//totalProperty : 'totalCount',
				fields : [{
							name : '部门',
							mapping : '部门',
							convert : function(v, record) {
								if (v.trim() == '年度合计' || v.trim() == '差额') {
									return '<font color=#FF00FF>' + v
											+ '</font>';
								}else{
									return   v;
								}
							}
						}, {
							name : '支出内容',
							mapping : '支出内容'
						}, {
							name : '1月',
							mapping : '1月'
						}, {
							name : '2月',
							mapping : '2月'
						}, {
							name : '3月',
							mapping : '3月'
						}, {
							name : '4月',
							mapping : '4月'
						}, {
							name : '5月',
							mapping : '5月'
						}, {
							name : '6月',
							mapping : '6月'
						}, {
							name : '7月',
							mapping : '7月'
						}, {
							name : '8月',
							mapping : '8月'
						}, {
							name : '9月',
							mapping : '9月'
						}, {
							name : '10月',
							mapping : '10月'
						}, {
							name : '11月',
							mapping : '11月'
						}, {
							name : '12月',
							mapping : '12月'
						}, {
							name : '合计',
							mapping : '合计'
						}]
			})
		});
		
		this.columns = [{
					hidden : true,
					header : '',
					dataIndex : '部门'
				}, {
					header : '支出内容',
					sortable : true,
					align:'right',
					dataIndex : '支出内容',
					width : 80
				}, {
					header : '1月',
					sortable : true,
					align:'right',
					dataIndex : '1月',
					width : 80
				}, {
					width : 80,
					header : '2月',
					align:'right',
					sortable : true,
					dataIndex : '2月'
				}, {
					header : '3月',
					align:'right',
					dataIndex : '3月',
					width : 80,
					sortable : true
				}, {
					header : '4月',
					align:'right',
					dataIndex : '4月',
					sortable : true,
					width : 80
				}, {
					header : '5月',
					align:'right',
					dataIndex : '5月',
					sortable : true,
					width : 80
				}, {
					header : '6月',
					align:'right',
					dataIndex : '6月',
					sortable : true,
					width : 80
				}, {
					header : '7月',
					align:'right',
					dataIndex : '7月',
					sortable : true,
					width : 80
				}, {
					header : '8月',
					align:'right',
					dataIndex : '8月',
					sortable : true,
					width : 80
				}, {
					header : '9月',
					align:'right',
					dataIndex : '9月',
					sortable : true,
					width : 80
				}, {
					header : '10月',
					align:'right',
					dataIndex : '10月',
					sortable : true,
					width : 80
				}, {
					header : '11月',
					align:'right',
					dataIndex : '11月',
					sortable : true,
					width : 80
				}, {
					header : '12月',
					align:'right',
					dataIndex : '12月',
					sortable : true,
					width : 80
				}, {
					header : '合计',
					align:'right',
					dataIndex : '合计',
					sortable : true,
					width : 80
				}];
		this.view = new Ext.grid.GroupingView({
			enableGroupingMenu : false
		});
		com.bitc.xmis.ContractStateGroupGrid .superclass.initComponent
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
				columnWidth : 0.3,
				labelWidth : 60,
				border : false,
				labelAlign : 'right',
				items : [{
							xtype : 'combo',
							fieldLabel : '年份',
							id : 'yearCom',
							hiddenName : 'year',
							displayField : 'year',
							valueField : 'year',
							mode : 'local',
							triggerAction : 'all',
							editable : false,
								store : new Ext.data.Store({
										proxy : new Ext.data.HttpProxy({
													url : __cxtPath
															+ '/bbzFinancyMain.do?method=getYearList'
												}),
										 autoLoad : true,
										reader : new Ext.data.JsonReader({
													root : 'rows',
												 fields : [{
													name: 'year'
												}]
										}),
										listeners:{
											'load' : function(store,records){
												var  maxYear = records[0].data['year'];
												Ext.getCmp('yearCom')
												.setValue(maxYear);
											}
										}
									})
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
						var icompareYear = '';
						Ext.getCmp('baseGridID').reload({
							params : {
								year : iyear
							}
						});
						
					}
				}]
			
			
			},{
				xtype : 'panel',
				layout : 'form',
				labelWidth : 60,
				columnWidth : 0.3,
				border : false,
				labelAlign : 'right',
				items : [{
							xtype : 'combo',
							fieldLabel : '对比年份',
							id : 'compareYearCom',
							hiddenName : 'compareYear',
							displayField : 'year',
							valueField : 'year',
							mode : 'local',
							triggerAction : 'all',
							editable : false,
							store : new Ext.data.Store({
								proxy : new Ext.data.HttpProxy({
											url : __cxtPath
													+ '/bbzFinancyMain.do?method=getYearList'
										}),
								 autoLoad : true,
								reader : new Ext.data.JsonReader({
											root : 'rows',
										 fields : [{
											name: 'year'
										}]
								}),
								listeners:{
									'load' : function(store,records){
										var  maxYear = records[0].data['year']-1;
										Ext.getCmp('compareYearCom')
										.setValue(maxYear);
									}
								}
							})
						}]
			},  {
				xtype : 'panel',
				layout : 'form',
				columnWidth : 0.2,
				labelWidth : 80,
				border : false,
				labelAlign : 'right',
				items : [{
					xtype : 'button',
					text : '对比',
					handler : function() {
						var iyear = infoPanel.getForm()
								.findField("year").getValue();
						var icompareYear = infoPanel.getForm()
								.findField("compareYear").getValue();
						if(iyear == icompareYear){
							Ext.Msg.alert('提示',"对比年份不能和查询年份相同！");
							return false;
						}
						Ext.getCmp('baseGridID').reload({
							params : {
								year : iyear,
								compareYear: icompareYear
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
	var grid = new com.bitc.xmis.ContractStateGroupGrid({
		region:'center',
		year : infoPanel.getForm().findField("year").getValue(),
		id : 'baseGridID',
		name : 'baseGrid',
		title : '管理处业务招待费统计表'
});
	
	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [infoPanel, grid]
			});
});
