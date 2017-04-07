Ext.ns('com.bitc.xmis');
com.bitc.xmis.PaymentPromptList = Ext.extend(Ext.list.ListView, {
	reserveScrollOffset : true,
	autoScroll : true,
	listSize : 10,
	hideHeaders : true,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
					url : __cxtPath
							+ '/contractMain.do?method=getPaymentPromptList',
					root : 'rows',
					baseParams : {
						start : 0,
						limit : this.listSize
					},
					// totalProperty : 'totalCount',
					autoLoad : true,
					fields : [{
								name : 'ID',
								mapping : '合同ID'
							}, {
								name : 'cname',
								mapping : '合同名称'
							}, {
								name : 'text',
								mapping : 'text'
							}, {
								name : 'managerId',
								mapping : '合同负责人'
							}, {
								name : 'bDeptId',
								mapping : '乙方部门ID'
							}, {
								name : 'hasReceivedAmt',
								mapping : '已收金额'
							}, {
								name : 'financialAmt',
								mapping : '财务来款'
							}, {
								name : 'unregisterAmt',
								mapping : '未登记金额'
							}]
				});
		this.columns = [{
					width : .25,
					header : 'text',
					dataIndex : 'text'
				}, {
					id : '合同名称',
					header : '合同名称',
					sortable : true,
					dataIndex : 'cname'
				}];
		com.bitc.xmis.PaymentPromptList.superclass.initComponent.call(this);
	},
	onRender : function(a, b) {
		this.on('click', function(listview, index) {
			var r = this.getStore().getAt(index);
			xMis.turn('contract-' + r.data.ID, r.data.cname,
					'/contractMain.do?method=contract&contractID=' + r.data.ID);
		}, this);
		com.bitc.xmis.PaymentPromptList.superclass.onRender.call(this, a, b);
	},
	reload : function() {
		this.getStore().reload();
	}
});

Ext.ns('com.bitc.xmis');
com.bitc.xmis.PaymentPromptGroupGrid = Ext.extend(Ext.grid.GridPanel, {
	autoExpandColumn : 'autoExId',
	autoWidth : true,
//	hideHeaders : true,
	initComponent : function() {
		this.store = new Ext.data.GroupingStore({
			groupField : 'text',
			url : __cxtPath + '/contractMain.do?method=getPaymentPromptList',
			baseParams : {
				start : 0,
				limit : 0
			},
			autoLoad : true,
			reader : new Ext.data.JsonReader({
				root : 'rows',
				totalProperty : 'totalCount',
				fields : [{
							name : 'ID',
							mapping : '合同ID'
						}, {
							name : 'cname',
							mapping : '合同名称'
						}, {
							name : 'text',
							mapping : 'text',
							convert : function(v, record) {
								if (v.trim() == '有金额未登记') {
									return '<font color=#FF00FF>' + v
											+ '</font>';
								} else {
									return '<font color=green>' + v + '</font>';
								}
							}
						}, {
							name : 'managerId',
							mapping : '合同负责人'
						}, {
							name : 'bDeptId',
							mapping : '乙方部门ID'
						}, {
							name : 'hasReceivedAmt',
							mapping : '已收金额'
						}, {
							name : 'financialAmt',
							mapping : '财务来款'
						}, {
							name : 'unregisterAmt',
							mapping : '未登记金额'
						}]
			}),
			listeners : {
				scope : this,
				'load' : function(s, records, options) {
					if (this.getStore().data.length) {
						this.setHeight(250);
					}
				}
			}
		});
		this.columns = [{
					hidden : true,
					header : '',
					dataIndex : 'text'
				}, {
					id : 'autoExId',
					header : '合同名称',
					sortable : true,
					dataIndex : 'cname'
				}];
		this.view = new Ext.grid.GroupingView({
			enableGroupingMenu : false,
			startCollapsed : true,
			groupTextTpl : '{text} ({[values.rs.length]} {[values.rs.length >= 1 ? "条" : "条"]})'
		});
		com.bitc.xmis.PaymentPromptGroupGrid.superclass.initComponent
				.call(this);
	},
	onRender : function(a, b) {
		this.on('rowclick', function(gridpanel, index, e) {
			var r = this.getStore().getAt(index);
			xMis.turn('contract-' + r.data.ID, '【合同】'+r.data.cname,
					'/contractMain.do?method=contract&contractID=' + r.data.ID);
		})
		com.bitc.xmis.PaymentPromptGroupGrid.superclass.onRender.call(this, a,
				b);
	},
	reload : function() {
		this.getStore().reload();
	}

});