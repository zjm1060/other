Ext.onReady(function() {
	Ext.QuickTips.init();
	var record = new Ext.data.Record.create([{
		name:'日期',
		mapping:'日期'
	},{
		name:'上游水位',
		mapping:'上游水位'
	},{
		name:'下游水位',
		mapping:'下游水位'
	},{
		name:'12孔闸开度',
		mapping:'12孔闸开度'
	},{
		name:'12孔闸开启孔数',
		mapping:'12孔闸开启孔数'
	},{
		name:'28孔闸开度',
		mapping:'28孔闸开度'
	},{
		name:'28孔闸开启孔数',
		mapping:'28孔闸开启孔数'
	},{
		name:'电机数',
		mapping:'电机数'
	},{
		name:'闸上水头',
		mapping:'闸上水头'
	},{
		name:'闸下水头',
		mapping:'闸下水头'
	},{
		name:'12孔闸流量类型',
		mapping:'12孔闸流量类型'
	},{
		name:'12孔闸流量',
		mapping:'12孔闸流量'
	},{
		name:'28孔闸流量',
		mapping:'28孔闸流量'
	},{
		name:'电站流量',
		mapping:'电站流量'
	},{
		name:'总流量',
		mapping:'总流量'
	}]);
	var recordDetail = new Ext.data.Record.create([{
		name : 'A',
		mapping : 'A'
	}, {
		name : 'B',
		mapping : 'B'
	}, {
		name : 'C',
		mapping : 'C'
	}, {
		name : 'D',
		mapping : 'D'
	}, {
		name : 'E',
		mapping : 'E'
	}, {
		name : 'F',
		mapping : 'F'
	}, {
		name : 'G',
		mapping : 'G'
	}, {
		name : 'H',
		mapping : 'H'
	}, {
		name : 'I',
		mapping : 'I'
	}, {
		name : 'J',
		mapping : 'J'
	}, {
		name : 'K',
		mapping : 'K'
	}, {
		name : 'L',
		mapping : 'L'
	}]);

	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
		width : 50
	}),{
		header : '日期',
		dataIndex : '日期',
		align:'center'
	},{
		header : '上游水位',
		dataIndex : '上游水位',
		align:'right'
	},{
		header : '下游水位',
		dataIndex : '下游水位',
		align:'right'
	},{
		header : '12孔闸开度',
		dataIndex : '12孔闸开度',
		align:'center'
	},{
		header : '12孔闸开启孔数',
		dataIndex : '12孔闸开启孔数',
		align:'center'
	},{
		header : '28孔闸开度',
		dataIndex : '28孔闸开度',
		align:'center'
	},{
		header : '28孔闸开启孔数',
		dataIndex : '28孔闸开启孔数',
		align:'center'
	},{
		header : '电机数',
		dataIndex : '电机数',
		align:'right'
	},{
		header : '闸上水头',
		dataIndex : '闸上水头',
		align:'right'
	},{
		header : '闸下水头',
		dataIndex : '闸下水头',
		align:'right'
	},{
		header : '12孔闸流量类型',
		dataIndex : '12孔闸流量类型',
		align:'center'
	},{
		header : '12孔闸流量',
		dataIndex : '12孔闸流量',
		align:'right'
	},{
		header : '28孔闸流量',
		dataIndex : '28孔闸流量',
		align:'right'
	},{
		header : '电站流量',
		dataIndex : '电站流量',
		align:'right'
	},{
		header : '总流量',
		dataIndex : '总流量',
		align:'right'
	}]);
	var column = new Ext.grid.ColumnModel([{
		header : 'A',
		dataIndex : 'A',
		align:'center'
	}, {
		header : 'B',
		dataIndex : 'B',
		align:'center'
	}, {
		header : 'C',
		dataIndex : 'C',
		align:'center'
	}, {
		header : 'D',
		dataIndex : 'D',
		align:'center'
	}, {
		header : 'E',
		dataIndex : 'E',
		align:'center'
	}, {
		header : 'F',
		dataIndex : 'F',
		align:'center'
	}, {
		header : 'G',
		dataIndex : 'G',
		align:'center'
	}, {
		header : 'H',
		dataIndex : 'H',
		align:'center'
	}, {
		header : 'I',
		dataIndex : 'I',
		align:'center'
	}, {
		header : 'J',
		dataIndex : 'J',
		align:'center'
	}, {
		header : 'K',
		dataIndex : 'K',
		align:'center'
	}, {
		header : 'L',
		dataIndex : 'L',
		align:'center'
	}]);
	var store = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/gczlAction.do?method=getLLValue'
					}),
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, record),
					baseParams : {
						start : 0,
						limit : 20
					}
		});
	var storeDetail = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
							+ '/gczlAction.do?method=getLLValue'
				}),
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, recordDetail),
				baseParams : {
					start : 0,
					limit : 20
				}
	});
	var exp = new Ext.ux.Export({
		formater : []
	});
		var grid = new Ext.grid.GridPanel({
			title : '查询表',
			id:'grid',
			border : false,
			loadMask : {
				msg : '正在加载数据，请稍侯……'
			},
			region : 'center',
			loadMask : true,
			stripeRows : true,
			store : store,
			cm : cm,
			tbar : ['开始日期：&nbsp;', new Ext.form.DateField({
								ref : '../startDate',
								format : 'Y-m-d',
								id:'startDate',
								showToday : true,
								value : new Date(new Date().getFullYear(), 0, 1),
								anchor : '100%'
							}), '&nbsp;结束日期&nbsp;', new Ext.form.DateField({
								ref : '../endDate',
								format : 'Y-m-d',
								id:'endDate',
								showToday : true,
								 value : new Date(),
								anchor : '100%'
							}), {
						text : '条件重置',
						handler : function() {
							grid.startDate.setValue('');
							grid.endDate.setValue('');
						}
					}, '-', {
						ref : '../searchBtn',
						iconCls : 'searchIcon',
						text : '查询',
						handler : function(b, e) {
							var begDate = grid.startDate.getValue();
							var endDate = grid.endDate.getValue();
							//grid.store.setBaseParam('divisionID', divisionID);

						    if (begDate == '' || endDate == '') {
								Ext.Msg.alert('提示', '开始日期和结束日期不可为空！');
								return;
							} else {
								if (begDate >= endDate) {
									Ext.Msg.alert('提示', '结束日期必须晚于开始日期！');
									return;
								} else {
									grid.store.setBaseParam('begDate', begDate);
									grid.store.setBaseParam('endDate', endDate);
									grid.store.setBaseParam('type', 12);
								}
							}
							grid.store.load({params : {
								start : 0,
								type:12
							}});

						}
					}],
			bbar : new Ext.PagingToolbar({
				pageSize : 20,
				store : store,
				displayInfo : true,
				displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
				emptyMsg : "无显示数据",
				plugins : [new Ext.ux.PagelimitCombo()],
				items : [ new Ext.Button({
					text : this.text || '导出Excel',
					iconCls : 'xls',
					menu : {
						items : [
						         {
							text : '导出本页数据',
							handler : function() {
								exp.onExport(grid, '流量', true);
							}
						},
								{
									text : '导出所有数据',
									scope:this,
									handler : function() {
										Ext.Msg.confirm('确认',
												'导出所有数据可能时间较长，请谨慎选择，是否继续?',
												function(buttonId) {
													if (buttonId == 'yes') {
														exp.onExport(grid, '流量',
																false);
													}
												}, this);
									}
								} ]
					}
				}) ]
			}),
			 
			 listeners:{ 
				   	'rowclick' : function(grid,rowIndex,e){
				   					var r = grid.getStore().getAt(rowIndex);
				   					var time =	r.get('日期');
				   	                gridDetail.getStore().load({
										params : {
											begDate:time,
											endDate:time,
											type:12
										}
									});		     
				   	}
				   }

		});
		
		var gridDetail = new Ext.grid.GridPanel({
			title : '详细表',
			id:'gridDetail',
			border : false,
			region:'south',
			height:300,
			split : true,
			loadMask : true,
			collapsible : true,
			autoScroll : true,
			collapseMode : 'mini',
			loadMask : {
				msg : '正在加载数据，请稍侯……'
			},
			stripeRows : true,
			
			store : storeDetail,
			cm : column
		});


	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [grid,gridDetail]
			});

});