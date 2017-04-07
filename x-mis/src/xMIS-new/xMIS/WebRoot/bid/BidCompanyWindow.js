Ext.ns('com.bitc.xmis.bid.BidCompanyWindow')

com.bitc.xmis.bid.BidCompanyWindow = Ext.extend(Ext.Window, {
	title : '投标方管理',
	width : 250,
	height : 500,
	layout : 'border',
	closeAction : 'hide',
	initComponent : function() {
		var sm = new Ext.grid.CheckboxSelectionModel({
			checkOnly : true,
			listeners : {
				scope : this,
				rowselect : function(selModel, rowIndex, record) {
					this.fireEvent('selectcompany', selModel, rowIndex, record);
				},
				rowdeselect : function(selModel, rowIndex, record) {
					this.fireEvent('deselectcompany', selModel, rowIndex,
						record);
				}
			}
		});
		var grid = new Ext.grid.EditorGridPanel({
			store : new Ext.data.JsonStore({
				autoLoad : true,
				url : __cxtPath + '/bidding.do?method=getBidCompanies',
				fields : ['id', 'name', 'order'],
				root : 'rows',
				totalProperty : 'totalCount',
				idProperty : 'id'
			}),
			sm : sm,
			columns : [new Ext.grid.RowNumberer(), sm, {
				header : '投标方',
				dataIndex : 'name',
				width : 180,
				editor : {
					type : 'textfield',
					allowBlank : false
				}
			}],
			region : 'center',
			clicksToEdit : 2,
			tbar : [{
				text : '添加',
				iconCls : 'add',
				handler : function() {
					Ext.Ajax.request({
						url : __cxtPath + '/bidding.do?method=addBidCompany',
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								var Record = grid.getStore().recordType;
								var r = new Record({
									id : result.attributes.id
								}, result.attributes.id);
								grid.stopEditing();
								grid.getStore().insert(0, r);
								grid.startEditing(0, 2);
							}
						}
					});

				}
			}],
			listeners : {
				beforeedit : function(e) {
				},
				'afteredit' : function(e) {
					Ext.Ajax.request({
						url : __cxtPath + '/bidding.do?method=updateObject',
						params : {
							clazz : 'com.bitc.jk.xmis.model.bid.BidCompany',
							id : e.record.get('id'),
							field : e.field,
							value : e.value,
							valueType : 'string'
						},
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								e.record.commit();
							}
						}
					});
				}

			}
		});
		this.items = [grid];
		this.addEvents('selectcompany', 'deselectcompany');
		com.bitc.xmis.bid.BidCompanyWindow.superclass.initComponent.call(this);
	}
});