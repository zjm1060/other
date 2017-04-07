Ext.ns('com.xmis.gczl');

com.xmis.gczl.comparisonGrid = Ext.extend(Ext.grid.PivotGrid,{
				border : false,
				id:'comparisonGrid',
				autoWidth:true,
				height:295,
				loadMask : true,
				autoScroll : true,
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				initComponent : function() {
					 var comparisonRecord = Ext.data.Record.create([{
				name:'year',
				type:'string'
			},{
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
				convert: function(v){ 
								if(v == 0.00){
								 return '0.00';
								}else{
									return v;
								}
							}
			}]);

var comparisonStore = new Ext.data.Store({
	
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
							+ '/applyManageMaint.do?method=getGczlInfoListComparison',
					method : "POST"
				}),
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, comparisonRecord),
		remoteSort : true,
		baseParams : {
			b_year : new Date().getFullYear()-1,
			e_year : new Date().getFullYear(),
			month: '01'
		},
		autoLoad : true
	
});
					
					this.tbar = ['&nbsp;开始年份：', {
				width:80,	
				xtype : 'combo',
				id : 'b_year',
				hiddenName : 'b_year',
				displayField : 'b_year',
				valueField : 'b_year',
				mode : 'local',
				triggerAction : 'all',
				editable : false,
				value : new Date().getFullYear()-1,
				store : new Ext.data.ArrayStore({
					fields : [ {
						name : 'b_year'
					} ],
					data : []
				}),
				listeners : {
					'beforerender' : function() {
						var newyear = new Date().getFullYear();
						var yearlist = [];
						for (var i = newyear; i >= 1960; i--) {
							yearlist.push([ i ]);
						}
						this.store.loadData(yearlist);
					}
				}
			
				}, '&nbsp;结束年份：', {
					
				xtype : 'combo',
				id:'e_year',
				width:80,
				hiddenName : 'e_year',
				displayField : 'e_year',
				valueField : 'e_year',
				mode : 'local',
				triggerAction : 'all',
				editable : false,
				value : new Date().getFullYear(),
				store : new Ext.data.ArrayStore({
					fields : [ {
						name : 'e_year'
					} ],
					data : []
				}),
				listeners : {
					'beforerender' : function() {
						var newyear = new Date().getFullYear();
						var yearlist = [];
						for (var i = newyear; i >= 1960; i--) {
							yearlist.push([ i ]);
						}
						this.store.loadData(yearlist);
					}
				}
			
				}, '&nbsp;对比月份', {
					
				xtype : 'combo',
				id:'month',
				width:80,
				hiddenName : 'month',
				displayField : 'month',
				valueField : 'month',
				mode : 'local',
				triggerAction : 'all',
				editable : false,
				value : '01',
				store : new Ext.data.ArrayStore({
					fields : [ {
						name : 'month'
					} ],
					data : []
				}),
				listeners : {
					'beforerender' : function() {
						var monthlist = [];
						for (var i = 1; i <=12; i++) {
							if(i<10)
							{i= '0'+i}
							monthlist.push([ i ]);
						}
						this.store.loadData(monthlist);
					}
				}
			
				},'-', {
							ref : '../searchBtn',
							iconCls : 'searchIcon',
							text : '查询',
							handler : function(){
								var b_year = Ext.getCmp('b_year').getValue();
								var e_year = Ext.getCmp('e_year').getValue();
								var month = Ext.getCmp('month').getValue();
								if(b_year >= e_year){
									Ext.Msg.alert("提示","结束时间必须大于开始时间");
									return;
								}
								Ext.getCmp('comparisonGrid').store.setBaseParam('b_year',b_year);
								Ext.getCmp('comparisonGrid').store.setBaseParam('e_year',e_year);
								Ext.getCmp('comparisonGrid').store.setBaseParam('month',month);
								Ext.getCmp('comparisonGrid').store.reload({
												params : {
													start : 0,
													limit : 0
												}
											});
								
							}

						}];
				this.store = comparisonStore;
				com.xmis.gczl.comparisonGrid.superclass.initComponent.call(this);
				},
				measure : 'space',
				aggregator : 'max',
				leftAxis : [{
					width : 80,
					dataIndex : 'year'
				},{
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
