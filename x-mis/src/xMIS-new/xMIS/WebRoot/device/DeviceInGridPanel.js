Ext.ns('com.bitc.xmis.Records');
Ext.ns('com.bitc.xmis');

com.bitc.xmis.DeviceInGridPanel = Ext.extend(Ext.grid.GridPanel, {
	initComponent : function() {
		var gridPanel = this;
		// var csm = new Ext.grid.CheckboxSelectionModel({
		// checkOnly : true,
		// singleSelect:true,
		// header : ''
		// });
		this.store = new Ext.data.GroupingStore({
			url : String
					.format(
							'{0}/device.do?method=getDeviceInListByContractId&contractId={1}',
							__cxtPath, this.contractId),
			autoLoad : true,
			remoteSort : true,
			reader : new Ext.data.JsonReader({
						idProperty : 'id',
						root : 'rows',
						totalProperty : 'totalCount'
					}, Ext.data.Record.create([{
								name : 'id'
							}, {
								name : '物资ID'
							}, {
								name : '物资名称'
							}, {
								name : '支出合同ID'
							}, {
								name : '收入合同ID'
							}, {
								name : '支出合同编号'
							}, {
								name : '收入合同编号'
							}, {
								name : '到货数量'
							}, {
								name : '到货日期'
							}, {
								name : '出厂日期'
							}, {
								name : '保修期限'
							}, {
								name : '原厂保修天数'
							}, {
								name : '原厂可退天数'
							}, {
								name : '原厂可换天数'
							}, {
								name : '保修期限'
							}, {
								name : '可退期限'
							}, {
								name : '可换期限'
							}])),
			groupField : '支出合同编号',
			baseParams : {
				limit : 20
			}
		});
		this.view = new Ext.grid.GroupingView({
			forceFit : true,
			// startCollapsed : true,
			// groupOnSort : true,
			groupTextTpl : '{text} ({[values.rs.length]}{[values.rs.length >= 1 ? "条" : "条"]})'
		});
		// this.sm = csm;
		this.columns = [// csm,
		{
			header : '物资名称',
			dataIndex : '物资名称',
			listeners : {
				'click' : function(column, grid, rowIndex, e) {
					var record = grid.getStore().getAt(rowIndex);
					var id = record.data.id;
					
					com.bitc.xmis.device.windowShow('edit',id, contract.contractID, 0,
							'', '', '', '','id','code');
				}
			}
		}, {
			header : '合同编号',
			hidden : true,
			dataIndex : '支出合同编号'
		}, {
			header : '到货数量',
			dataIndex : '到货数量'
		}, {
			header : '到货日期',
			dataIndex : '到货日期'
		}, {
			header : '出厂日期',
			hidden : true,
			dataIndex : '出厂日期'
		}, {
			header : '保修期限',
			dataIndex : '保修期限',
			renderer : function(value, cellMeta, record) {
				cellMeta.attr = 'ext:qtitle="提示" ext:qtip="' + '保修天数'
						+ record.get('原厂保修天数') + '"';
				return value;
			}
		}, {
			header : '可退期限',
			dataIndex : '可退期限',
			renderer : function(value, cellMeta, record) {
				cellMeta.attr = 'ext:qtitle="提示" ext:qtip="' + '可退天数'
						+ record.get('原厂可退天数') + '"';
				return value;
			}
		}, {
			header : '可换期限',
			dataIndex : '可换期限',
			renderer : function(value, cellMeta, record) {
				cellMeta.attr = 'ext:qtitle="提示" ext:qtip="' + '可换天数'
						+ record.get('原厂可换天数') + '"';
				return value;
			}
		}];

		// if (false) {
		// this.tbar = new Ext.Toolbar({
		// enableOverflow : true,
		// items : [{
		// xtype : 'button',
		// text : '删除',
		// hidden : true,
		// icon : './resources/images/split-false.gif',
		// handler : function() {
		// var records = gridPanel.getSelectionModel()
		// .getSelections();
		// var ids = "";
		// if (records.length == 0) {
		// Ext.MessageBox.alert("提示", "请选择数据");
		// } else {
		// for (var i = 0; i < records.length; i++) {
		// ids += records[i].get('companyID') + ",";
		// }
		// ids = records[0].get('id');
		// Ext.Ajax.request({
		// url : __cxtPath
		// + '/device.do?method=delDeviceInObjByID',
		// method : 'post',
		// params : {
		// id : ids
		// },
		// success : function(response, action) {
		// Ext.MessageBox.alert("提示", "删除成功！");
		// gridPanel.store.reload();
		// },
		// failure : function(response, action) {
		// Ext.MessageBox.hide();
		// Ext.MessageBox.alert("提示", "操作失败！");
		// }
		// });
		// }
		// }
		// }]
		// });
		// this.tbar.add('模糊查询：', new Ext.ux.InklingSearchField({
		// width : 120,
		// store : this.store,
		// tooltip : {
		// title : '模糊查询',
		// text : '您可以输入“物资名称”进行查询'
		// },
		// inklings : [{
		// name : '物资名称',
		// op : 'like',
		// type : 'string'
		// }]
		// }));
		// this.bbar = new Ext.PagingToolbar({
		// pageSize : 20,
		// store : this.store,
		// displayInfo : true,
		// displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
		// emptyMsg : "无显示数据",
		// plugins : [
		// new Ext.ux.Export({
		// exportname : '到库清单',
		// formater : [{
		// columnHeader : '',
		// formatClass : 'com.bitc.jk.xmis.export.RowNumber'
		// }]
		// }),
		// new Ext.ux.PagelimitCombo()],
		// items : this.bbar || []
		// });
		// }
		com.bitc.xmis.DeviceInGridPanel.superclass.initComponent.call(this);
	}
});