Ext.ns('com.bitc.xmis');
com.bitc.xmis.ContractStateGroupGrid = Ext.extend(Ext.grid.GridPanel, {

	autoWidth : true,
	initComponent : function() {
		var autoExId = Ext.id();
		this.autoExpandColumn = autoExId;
		this.store = new Ext.data.GroupingStore({
			groupField : 'text',
			url : __cxtPath + '/contractMain.do?method=getContractStateList',
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
							name : 'projectcode',
							mapping : '项目编码'
						}, {
							name : 'text',
							mapping : 'text',
							convert : function(v, record) {
								if (v.trim() == '新建--收入合同') {
									return '<font color=#FF00FF>' + v
											+ '</font>';
								} else if (v.trim() == '新建--支出合同') {
									return '<font color=#FF3300>' + v
											+ '</font>';
								} else {
									return '<font color=green>' + v + '</font>';
								}
							}
						}, {
							name : 'managerId',
							mapping : 'manager_id'
						}, {
							name : 'bDeptId',
							mapping : '乙方部门ID'
						}, {
							name : 'bDeptName',
							mapping : '部门名称'
						}, {
							name : 'lastDate',
							mapping : '最后修改日期'
						}, {
							name : 'lastID',
							mapping : '最后修改人ID'
						}, {
							name : 'lastName',
							mapping : 'lastName'
						}]
			}),
			listeners : {
				scope : this,
				'load' : function(s, records, options) {
					if (this.getStore().data.length) {
						this.setHeight(250);
						// this.syncSize();
					}
				}
			}
		});
		this.columns = [{
					hidden : true,
					header : '',
					dataIndex : 'text'
				}, {
					header : '项目编码',
					sortable : true,
					dataIndex : 'projectcode',
					width : 180
				}, {
					id : autoExId,
					header : '合同名称',
					sortable : true,
					dataIndex : 'cname'
				}, {
					width : 140,
					header : '部门名称',
					sortable : true,
					dataIndex : 'bDeptName'
				}, {
					header : '最后修改人',
					dataIndex : 'lastName',
					hidden : true
				}, {
					header : '最后修改日期',
					dataIndex : 'lastDate',
					hidden : true
				}];
		this.view = new Ext.grid.GroupingView({
			enableGroupingMenu : false,
			startCollapsed : true,
			// forceFit : true,
			groupTextTpl : '{text} ({[values.rs.length]} {[values.rs.length >= 1 ? "条" : "条"]})'// ?
		});
		com.bitc.xmis.ContractStateGroupGrid.superclass.initComponent
				.call(this);
	},
	onRender : function(a, b) {
		this.on('rowclick', function(gridpanel, index, e) {
					var r = this.getStore().getAt(index);
					xMis.turn('contract-' + r.data.ID, '【合同】' + r.data.cname,
							'contractMain.do?method=contract&contractID='
									+ r.data.ID);
				})
		com.bitc.xmis.ContractStateGroupGrid.superclass.onRender.call(this, a,
				b);
	},
	reload : function() {
		this.getStore().reload();
	}

});
