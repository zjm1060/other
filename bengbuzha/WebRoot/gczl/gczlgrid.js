Ext.ns('com.xmis.gczl');
com.xmis.gczl.grid = Ext.extend(Ext.grid.PivotGrid,{
				border : false,
				id:'grid',
				autoWidth : true,
				height : 295,
				loadMask : true,
				autoScroll : true,
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				initComponent : function() {
						var reRecordGYS = Ext.data.Record.create([{
				name : 'title',
				type : 'string'
			}, {
				name : 'boundary',
				type : 'string'
			}, {
				name : 'measure_date',
				type : 'string'
			}, {
				name : 'space',
				type : 'string'
			}, {
				name : 'VALUE',
				type : 'float',
				convert : function(v) {
					if (v == 0.00) {
						return '0.00';
					} else {
						return v;
					}
				}
			}]);
	var storeGYS = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
							+ '/applyManageMaint.do?method=getGczlInfoListWithQuery',
					method : "POST"
				}),
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, reRecordGYS),
		remoteSort : true,
		baseParams : {
			b_date : new Date(new Date().getFullYear(), 0, 1),
			e_date : Ext.util.Format.date(new Date(), 'Y-m-d')
		},
		autoLoad : true
	});
				this.tbar = ['&nbsp;开始日期：', new Ext.form.DateField({
									ref : '../startDate',
									id:'startDate',
									format : 'Y-m-d',
									showToday : true,
									value : new Date(new Date().getFullYear(),
											0, 1),
									anchor : '100%'
								}), '&nbsp;结束日期：', new Ext.form.DateField({
									ref : '../endDate',
									id:'endDate',
									format : 'Y-m-d',
									showToday : true,
									value : new Date(),
									anchor : '100%'
								}), '-', {
							ref : '../searchBtn',
							iconCls : 'searchIcon',
							text : '查询',
							handler : function(b, e) {
								var startDate = Ext.getCmp('startDate').getValue();
								var endDate = Ext.getCmp('endDate').getValue();
								if (startDate>=endDate) {
									Ext.Msg.alert('提示', '结束日期必须晚于开始日期！');
									return;
								} else {
									Ext.getCmp('grid').store.setBaseParam('b_date',

											startDate);
									Ext.getCmp('grid').store.setBaseParam('e_date',
											endDate);
									Ext.getCmp('grid').store.reload({
												params : {
													start : 0,
													limit : 0
												}
											});
								}
							}

						}],
				this.store = storeGYS;
				com.xmis.gczl.grid.superclass.initComponent.call(this);
				},
				
				measure : 'space',
				aggregator : 'max',
				leftAxis : [{
							width : 80,
							dataIndex : 'title'
						}, {
							dataIndex : 'boundary'
						}, {
							dataIndex : 'VALUE'
						}, {
							dataIndex : 'measure_date',
							width : 200
						}]

			});