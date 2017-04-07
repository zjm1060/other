Ext.onReady(function() {
	var pageSize = 0;
	var date = new Date();
	var year = date.getFullYear();
	var record = new Ext.data.Record.create([{
				name : '项目',
				mapping : '项目'
			}, {
				name : '金额',
				mapping : '金额'
			}]);

	var columnModel = new Ext.grid.ColumnModel([{
		header : '项目',
		dataIndex : '项目',
		align : 'left',
		sortable : false,
		width : 300,
		hidden : false,
		renderer : function(value) {
			var count = 0;
			var index = 0;
			var str = value;
			while ((index = str.indexOf('\t')) != -1) {
				str = str.substring(index + '\t'.length);
				count++;
			}
			if (count == 0) {
				return value;
			} else if (count == 1) {
				return '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;' + value;
			} else if (count == 2) {
				return '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
						+ value;
			}
		}
	}, {
		header : '金额',
		dataIndex : '金额',
		align : 'right',
		sortable : false,
		hidden : false,
		width : 250,
		renderer : function(value) {
			return formatAmt(value);
		}
	}]);

	var store = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/financyMain.do?method=getManagementFeeStore'
						}),
				baseParams : {
					start : 0,
					limit : pageSize,
					executeString : '{call xmis.sp_managementFee}'
				},
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, record)
			});

	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '金额',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}]
			});

	var grid = new Ext.grid.GridPanel({
				title : year + '年管理费收支统计表',
				region : 'center',
				store : store,
				cm : columnModel,
				bodyStyle : 'width ： 33%',
				loadMask : true,
				loadMask : {
					msg : '正在加载数据，请稍候...'
				},
				bbar : new Ext.ux.TotalPagingToolbar({
							pageSize : 50,
							store : store,
							displayInfo : true,
							displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
							emptyMsg : "无显示数据",
							plugins : [exp, new Ext.ux.PagelimitCombo()]
						})
			});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [grid]
			});

	grid.getStore().on('load', function(s, records) {
		var gridcount = 0;
		s.each(function(r) {
			if (r.get('项目').indexOf('\t') == -1) {
				grid.getView().getRow(gridcount).style.backgroundColor = '#FFFF00';// #DCDCDC
			}
			gridcount = gridcount + 1;
		});
	});

	store.load();

	this.formatAmt = function(value) {
		return Ext.util.Format.number(value, '0,000.00');
	}

});