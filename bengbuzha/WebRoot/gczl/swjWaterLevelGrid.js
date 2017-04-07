Ext.onReady(function() {
	Ext.QuickTips.init();
	var record = new Ext.data.Record.create([ {
		name : '日期',
		mapping : 'measure_date'
	}, {
		name : '上游水位',
		mapping : 'ap_w_level'
	}, {
		name : '下游水位',
		mapping : 'bp_w_level'
	} ]);

	var cm = new Ext.grid.ColumnModel([ new Ext.grid.RowNumberer({
		width : 50
	}), {
		header : '日期',
		width:200,
		dataIndex : '日期',
		align : 'center'
	}, {
		header : '上游水位',
		dataIndex : '上游水位',
		align : 'right'
	}, {
		header : '下游水位',
		dataIndex : '下游水位',
		align : 'right'
	} ]);
	var store = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
							+ '/gczlAction.do?method=getSWJWaterLevel'
				}),
		remoteSort:false,
		sortInfo: {
		    field: '日期',
		    direction: 'DESC' // or 'DESC' (case sensitive for local sorting)
		},
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, record),
				baseParams : {
					//start_date : Ext.getCmp('startDate').value,
					//end_date : Ext.getCmp('endDate').value
				}
	});
	var grid = new Ext.grid.GridPanel({
		title : '水文局上下游水位原始数据表',
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
							value : new Date(),
							anchor : '100%'
						}),{

			xtype : 'combo',
			ref : '../startTime',
			displayField : 'text',
			valueField : 'value',
			mode : 'local',
			triggerAction : 'all',
			editable : false,
			name : 'begin_hour',
			value : '00',
			width : 50,
			store : new Ext.data.ArrayStore({
						fields : ['value', 'text'],
						data : [[00, '00'],
								[01, '01'],
								[02, '02'],
								[03, '03'],
								[04, '04'],
								[05, '05'],
								[06, '06'],
								[07, '07'],
								[08, '08'],
								[09, '09'],
								[10, '10'],
								[11, '11'],
								[12, '12'],
								[13, '13'],
								[14, '14'],
								[15, '15'],
								[16, '16'],
								[17, '17'],
								[18, '18'],
								[19, '19'],
								[20, '20'],
								[21, '21'],
								[22, '22'],
								[23, '23']]
					})

		
		}, '点&nbsp;&nbsp;&nbsp;结束日期&nbsp;', new Ext.form.DateField({
							ref : '../endDate',
							format : 'Y-m-d',
							id:'endDate',
							showToday : true,
							 value : new Date(),
							anchor : '100%'
						}),{

			xtype : 'combo',
			ref : '../endTime',
			displayField : 'text',
			valueField : 'value',
			mode : 'local',
			triggerAction : 'all',
			editable : false,
			name : 'end_hour',
			value : '23',
			width : 50,
			store : new Ext.data.ArrayStore({
						fields : ['value', 'text'],
						data : [[00, '00'],
								[01, '01'],
								[02, '02'],
								[03, '03'],
								[04, '04'],
								[05, '05'],
								[06, '06'],
								[07, '07'],
								[08, '08'],
								[09, '09'],
								[10, '10'],
								[11, '11'],
								[12, '12'],
								[13, '13'],
								[14, '14'],
								[15, '15'],
								[16, '16'],
								[17, '17'],
								[18, '18'],
								[19, '19'],
								[20, '20'],
								[21, '21'],
								[22, '22'],
								[23, '23']]
					})

		
		},'点&nbsp;&nbsp;&nbsp;', '-', {
					ref : '../searchBtn',
					iconCls : 'searchIcon',
					text : '查询',
					handler : function(b, e) {
						var begDate = Ext.util.Format.date(grid.startDate.getValue(), 'Y-m-d');
						var begTime = grid.startTime.getValue();
						var endDate =Ext.util.Format.date(grid.endDate.getValue(), 'Y-m-d');
						var endTime = grid.endTime.getValue();
						//grid.store.setBaseParam('divisionID', divisionID);
					    if (begDate == '' || endDate == '') {
							Ext.Msg.alert('提示', '开始日期和结束日期不可为空！');
							return;
						} else {
							if (begDate > endDate) {
								Ext.Msg.alert('提示', '结束日期必须晚于开始日期！');
								return;
							} else {
								grid.store.setBaseParam('start_date', begDate+' '+begTime+':00');
								grid.store.setBaseParam('end_date', endDate+' '+endTime+':00');
							}
						}
						grid.store.load();

					}
				}]

	});
	new Ext.Viewport({
		border : false,
		layout : 'border',
		items : [grid]
	});
});
