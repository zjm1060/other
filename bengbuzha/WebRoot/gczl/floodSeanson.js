Ext.ns('com.xmis.gczl');

	com.xmis.gczl.floodSeasonGrid = Ext.extend(Ext.grid.PivotGrid,{
				border : false,
				id:'floodSeasonGrid',
				autoWidth:true,
				height:295,
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
								convert: function(v){ 
												if(v == 0.00){
												 return '0.00';
												}else{
													return v;
												}
											}
							}]);
					var storeGYS = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
									url : __cxtPath
											+ '/applyManageMaint.do?method=getGczlInfoListForFlood',
									method : "POST"
								}),
						reader : new Ext.data.JsonReader({
									root : 'rows',
									totalProperty : 'totalCount'
								}, reRecordGYS),
						remoteSort : true,
						baseParams : {
							year :  new Date().getFullYear(),
							flood : '1'
						},
						autoLoad : true
					});
						this.tbar = ['&nbsp;年份：',{
				xtype : 'combo',
				id:'year',
				width:80,
				hiddenName : 'year',
				displayField : 'year',
				valueField : 'year',
				mode : 'local',
				triggerAction : 'all',
				editable : false,
				value : new Date().getFullYear(),
				store : new Ext.data.ArrayStore({
					fields : [ {
						name : 'year'
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
			
				
				},'&nbsp;&nbsp;&nbsp;&nbsp;',{ 
					xtype: 'radiogroup',
         		   fieldLabel: 'Auto Layout',
         		   id:'floodSeason',
          		  items: [
		                {boxLabel: '汛期', name: 'flood', inputValue: 1, checked: true},
		                {boxLabel: '非汛期', name: 'flood', inputValue: 0,width:80}
           	 ]},'&nbsp;&nbsp;&nbsp;&nbsp;',{
							ref : '../searchBtn',
							iconCls : 'searchIcon',
							text : '查询',
							handler : function(b, e) {
								Ext.getCmp('floodSeasonGrid').store.setBaseParam('year',
									Ext.getCmp('year').getValue()
											);
									Ext.getCmp('floodSeasonGrid').store.setBaseParam('flood',
											Ext.getCmp('floodSeason').getValue()
													);
									Ext.getCmp('floodSeasonGrid').store.reload({
												params : {
													start : 0,
													limit : 0
												}
											});
									
								
							}

						}];
						this.store = storeGYS;
					com.xmis.gczl.floodSeasonGrid.superclass.initComponent.call(this);
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