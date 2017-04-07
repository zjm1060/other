Ext.onReady(function(){
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						header : '序号',
						width : 50
					}),
		{
			header:'部门',
			dataIndex:'deptName',
			width: 130,
			align:'center'
		},{
			header:'姓名',
			dataIndex:'hrName',
			width: 100,
			align:'center'
		},{
			header:'带薪年假',
			dataIndex :'带薪年假',
			width:100,
			align:'center',
			renderer : function(v) {
				if(v != '' && v != null)
				return v+'天';
			}
			
		},{
			header:'探亲假',
			dataIndex :'探亲假',
			width:100,
			align:'center',
			renderer : function(v) {
				if(v != '' && v != null)
				return v+'天';
			}
		},{
			header:'病假',
			dataIndex :'病假',
			width:100,
			align:'center',
			renderer : function(v) {
				if(v != '' && v != null)
				return v+'天';
			}
		},{
			header:'事假',
			dataIndex :'事假',
			width:100,
			align:'center',
			renderer : function(v) {
				if(v != '' && v != null)
				return v+'天';
			}
		},{
			header:'婚假',
			dataIndex :'婚假',
			width:100,
			align:'center',
			renderer : function(v) {
				if(v != '' && v != null)
				return v+'天';
			}
		},{
			header:'丧假',
			dataIndex :'丧假',
			width:100,
			align:'center',
			renderer : function(v) {
				if(v != '' && v != null)
				return v+'天';
			}
		},{
			header:'计划生育、优生优育假',
			dataIndex :'计划生育、优生优育假',
			width:150,
			align:'center',
			renderer : function(v) {
				if(v != '' && v != null)
				return v+'天';
			}
		},{
			header:'公派',
			dataIndex :'公派',
			width:100,
			align:'center',
			renderer : function(v) {
				if(v != '' && v != null)
				return v+'天';
			}
		}
	]);
	var record = new Ext.data.Record.create([
	{
		name:'deptName'
	},{
		name:'hrName'
	},{
		name:'带薪年假'
	},{
		name:'探亲假'
	},{
		name:'病假'
	},{
		name:'事假'
	},{
		name:'婚假'
	},{
		name:'丧假'
	},{
		name:'优生优育假'
	},{
		name:'公派'
	}]);
	var store = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/applyManageMaint.do?method=getAskForLeaveList',
							method : "POST"
						}),
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, record),
				remoteSort : true,
				baseParams : {
					start : 0,
					limit : 20,
					year : new Date().getFullYear(),
				    month : new Date().getMonth()+1
				},
				autoLoad : true
	});
	var grid = new Ext.grid.GridPanel({
				id:'grid',
				border : false,
				region : 'center',
				width : 980,
				stripeRows : true,
				loadMask : true,
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				store : store,
				cm : cm,
				tbar : ['&nbsp;年份&nbsp;',
						new Ext.form.ComboBox({	
									ref : '../year_combo',
									id : 'year',
									hiddenName : 'year',
									displayField : 'iyear',
									valueField : 'iyear',
									mode : 'local',
									triggerAction : 'all',
									editable : false,
									value : new Date().getFullYear(),// 当年
									store : new Ext.data.ArrayStore({
												fields : [{
															name : 'iyear'
														}],
												data : []
											}),
									listeners : {
										'beforerender' : function() {
											var yearlist = [];
											for (var i = new Date().getFullYear(); i >= 2005; i--) {
												yearlist.push([i]);
											}
											this.store.loadData(yearlist);
										}
									}}),'-',
						'&nbsp;月份&nbsp;', new Ext.form.ComboBox({	
									ref : '../month_combo',
									id : 'month',
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
											monthlist.push(['全年']);
											for (var i = 1; i <= 12; i++) {
												if(i<10)
												monthlist.push(['0'+i]);
												else
												monthlist.push([i]);
											}
											this.store.loadData(monthlist);
										}
									}}), '-',
					'&nbsp;职工姓名&nbsp;', new Ext.form.TextField({	
									ref : '../hrName',
									fieldLabel : '职工姓名：',
									editable : false,
									emptyText:'全部',
									name :'hrName',
									id : 'hrName'
									}),				
					{
					text : '条件重置',
					handler : function() {
						grid.year_combo.setValue(new Date().getFullYear());
						grid.month_combo.setValue(new Date().getMonth()+1);
						grid.hrName.setValue('');
					}
				}, '-', {
					ref : '../searchBtn',
					iconCls : 'searchIcon',
					text : '查询',
					handler : function(b, e) {
						var year = grid.year_combo.getValue();
						var month = grid.month_combo.getValue();
						var hrName = grid.hrName.getValue();
						grid.store.setBaseParam('year', year);
						grid.store.setBaseParam('month', month);
						grid.store.setBaseParam('hrName', hrName);
						grid.store.load({
									params : {
										start : 0
									}
								});

					}
				}],
				bbar : new Ext.PagingToolbar({
							pageSize : 20,
							store : store,
							displayInfo : true,
							displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
							emptyMsg : "无显示数据",
							plugins : [new Ext.ux.PagelimitCombo()]
						})
				
	});
	new Ext.Viewport({
				layout : 'border',
				items : [grid]
			});
});