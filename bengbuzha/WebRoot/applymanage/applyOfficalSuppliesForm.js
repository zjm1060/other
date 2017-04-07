Ext.ns('com.bitc.xmis.ApplyOfficialSupplies');
com.bitc.xmis.ApplyOfficialSupplies.PRIORITY_ARRAY = ['普通', '紧急'];
com.bitc.xmis.ApplyOfficialSupplies.DetailRecord = Ext.data.Record.create([{
			"name" : "form_id",
			"type" : "string"
		}, {
			"name" : "supplies_id",
			"type" : "string"
		}, {
			"name" : "supplies_name",
			"type" : "string"
		}, {
			"name" : "supplies_type_id",
			"type" : "string"
		}, {
			"name" : "supplies_type_Name",
			"type" : "string"
		}, {
			"name" : "priority",
			"type" : "int"
		}, {
			"name" : "qty",
			"type" : "int"
		}, {
			"name" : "reason",
			"type" : "string"
		}, {
			"name" : "app_user_id",
			"type" : "int"
		}, {
			"name" : "app_user_name",
			"type" : "string"
		}, {
			"name" : "app_datetime",
			"type" : "string"
		}, {
			"name" : "remark",
			"type" : "string"
		}]);

com.bitc.xmis.ApplyOfficialSupplies.DetailGrid = Ext.extend(Ext.grid.GridPanel,
		{
			viewConfig : {
				emptyText : '无申请数据'
			},
			autoExpandColumn : 'remark',
			stripeRows : true,
			initComponent : function() {
				this.store = new Ext.data.Store({
					url : __cxtPath
							+ this.dataUrl,
					baseParams : {
						limit : 20
					},
					autoLoad : this.autoLoadData,
					remoteSort : true,
					reader : new Ext.data.JsonReader({
								totalProperty : 'totalCount',
								root : 'rows'
							}, com.bitc.xmis.ApplyOfficialSupplies.DetailRecord)
				});
				this.columns = [new Ext.grid.RowNumberer()];
				this.columns = this.columns.concat([{
							header : "办公用品名称",
							width : 120,
							sortable : true,
							dataIndex : "supplies_name"
						}, {
							header : "优先等级",
							sortable : true,
							width : 90,
							dataIndex : "priority",
							renderer : function(value) {
								return com.bitc.xmis.ApplyOfficialSupplies.PRIORITY_ARRAY[value];
							}
						}, {
							header : "事由",
							sortable : true,
							width : 180,
							dataIndex : "reason"
						}, {
							header : "数量",
							sortable : true,
							width : 60,
							dataIndex : "qty"
						}, {
							id : 'remark',
							header : "备注",
							sortable : true,
							width : 180,
							dataIndex : "remark"
						}]);
				if (this.actionColumn) {
					this.columns = this.columns.concat(this.actionColumn);
				}
				com.bitc.xmis.ApplyOfficialSupplies.DetailGrid.superclass.initComponent
						.call(this);
			}
		});
com.bitc.xmis.ApplyOfficialSupplies.OfficialSuppliesTypeCombo = Ext.extend(
		Ext.form.ComboBox, {
			fieldLabel : '',
			hiddenName : '',
			width : 0,
			emptyText : '选择办公用品类别',
			dataUrl : '/applyManageMaint.do?method=getOfficialSuppliesTypeCombo',
			baseParams : '',
			displayField : 'supplies_type_Name',
			valueField : 'supplies_type_id',
			mode : 'remote',
			triggerAction : 'all',
			lazyInit : false,
			initComponent : function() {
				var url = this.dataUrl;
				this.store = new Ext.data.JsonStore({
							url : __cxtPath + url,
							baseParams : this.baseParams,
							root : 'rows',
							fields : [{
										name : 'supplies_type_id',
										mapping : 'supplies_type_id'
									}, {
										name : 'supplies_type_Name',
										type : 'string',
										mapping : 'supplies_type_Name'
									}],
							listeners : {
								scope : this,
								'load' : function(s, records, options) {
									this.setValue(this.value);
								}
							}
						});
				com.bitc.xmis.ApplyOfficialSupplies.OfficialSuppliesTypeCombo.superclass.initComponent
						.call(this);
			}
		});
com.bitc.xmis.ApplyOfficialSupplies.OfficialSuppliesCombo = Ext.extend(
		Ext.form.ComboBox, {
			valueField : 'supplies_id',
			displayField : 'supplies_name',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择办公用品名称',
			editable : false,
			initComponent : function() {
				this.store = new Ext.data.JsonStore({
					url : String
							.format(
									'{0}/applyManageMaint.do?method=getOfficialSuppliesCombo',
									__cxtPath),
					root : 'rows',
					// autoLoad : true,
					fields : [{
								name : "supplies_id",
								type : "string",
								mapping : "supplies_id"
							}, {
								name : "supplies_name",
								type : "string",
								mapping : "supplies_name"
							}],
					listeners : {
						scope : this,
						'load' : function() {
							this.setValue(this.value);
						}
					}
				});
				com.bitc.xmis.ApplyOfficialSupplies.OfficialSuppliesCombo.superclass.initComponent
						.call(this);
			}
		});
com.bitc.xmis.ApplyOfficialSupplies.WindowShow = function(form, gridPanel,
		button_num, dataUrl, form_id_param, supplies_id_param) {
	
	var officialSuppliesCombo = new com.bitc.xmis.ApplyOfficialSupplies.OfficialSuppliesCombo(
			{
				fieldLabel : '选择办公用品名称',
				width : 140,
				emptyText : '选择办公用品名称',
				allowBlank : false,
				hiddenName : 'officialSuppliesCombo',
				listeners : {
					'select' : function(combo, record, index) {
						applyOfficialSuppliesForm.getForm()
								.findField('supplies_id')
								.setValue(record.data.supplies_id);
					}
				}
			});
	var officialSuppliesTypeCombo = new com.bitc.xmis.ApplyOfficialSupplies.OfficialSuppliesTypeCombo(
			{
				fieldLabel : '选择办公用品类别',
				width : 140,
				emptyText : '选择办公用品类别',
				allowBlank : false,
				hiddenName : 'suppliesTypeId',
				listeners : {
					'select' : function(combo, record, index) {
						officialSuppliesCombo.clearValue();
						officialSuppliesCombo.store.baseParams.type = record.data.supplies_type_id;
						applyOfficialSuppliesForm.getForm()
								.findField('supplies_type_id')
								.setValue(record.data.supplies_type_id);
						officialSuppliesCombo.store.load();
					}
				}
			});
	var applyOfficialSuppliesForm = new Ext.form.FormPanel({
		ref : 'form',
		region : 'center',
		padding : 20,
		autoScroll : true,
		border : false,
		labelWidth : 101,
		labelAlign : 'right',
		reader : new Ext.data.JsonReader({
					root : 'rows'
				}, com.bitc.xmis.ApplyOfficialSupplies.DetailRecord),
		tbar : {
			xtype : 'toolbar',
			items : [{
				xtype : 'button',
				text : '确定',
				ref : '../save_btn',
				iconCls : 'save',
				handler : function() {
					var form_id = form.getForm().findField('form_id')
							.getValue();
					if (form_id_param != '') {
						form_id = applyOfficialSuppliesForm.getForm()
								.findField('form_id').getValue();
					}
					var supplies_id = applyOfficialSuppliesForm.getForm()
							.findField('supplies_id').getValue();

					var priority = applyOfficialSuppliesForm.getForm()
							.findField('priority').getValue();
					var reason = applyOfficialSuppliesForm.getForm()
							.findField('reason').getValue();
					var qty = applyOfficialSuppliesForm.getForm()
							.findField('qty').getValue();
						if(qty == '' || qty== null){
							return false;
						}
					var remark = applyOfficialSuppliesForm.getForm()
							.findField('remark').getValue();

					Ext.Ajax.request({
								url : __cxtPath + dataUrl,
								params : {
									form_id_param : form_id_param,
									supplies_id_param : supplies_id_param,
									form_id : form_id,
									supplies_id : supplies_id,
									priority : priority,
									reason : reason,
									qty : qty,
									remark : remark
								},
								method : "post",
								success : function(response) {
									var result = Ext
											.decode(response.responseText);
									if (result.success) {
										var return_id = result.attributes.return_id;
										form.getForm().findField('form_id')
												.setValue(return_id);
										gridPanel.store.baseParams.form_id = return_id;
										applyOfficialSuppliesFormWin.hide();
										gridPanel.store.reload();
									}
								},
								failure : function(response) {
									Ext.MessageBox.alert('提示', '操作失败');
								}
							});

				}
			}]
		},
		items : [{
					xtype : 'panel',
					border : false,
					layout : 'hbox',
					fieldLabel : '办公用品选择',
					items : [officialSuppliesTypeCombo, {
								xtype : 'displayfield',
								value : '-'
							}, officialSuppliesCombo]
				}, {
					xtype : 'combo',
					name : 'priority',
					fieldLabel : '优先等级',
					displayField : 'text',
					valueField : 'value',
					mode : 'local',
					triggerAction : 'all',
					value : 0,
					width : 100,
					store : new Ext.data.ArrayStore({
								fields : ['value', 'text'],
								data : [[1, '紧急'], [0, '普通']]
							})
				}, {
					xtype : 'textarea',
					fieldLabel : '事由',
					name : 'reason',
					allowBlank : false,
					width : 300,
					blankText : '该输入项为必输项'
				}, {
					xtype : 'numberfield',
					name : 'qty',
					fieldLabel : '数量',
					allowBlank : false,
					width : 300,
					blankText : '该输入项为必输项'
				}, {
					xtype : 'textarea',
					name : 'remark',
					fieldLabel : '备注',
					width : 300,
					blankText : '该输入项为必输项'
				}, {
					xtype : 'hidden',
					name : 'form_id',
					width : 300
				}, {
					xtype : 'hidden',
					name : 'supplies_id',
					width : 300
				}, {
					xtype : 'hidden',
					name : 'supplies_type_id',
					width : 300
				}, {
					xtype : 'hidden',
					name : 'supplies_name',
					width : 300
				}, {
					xtype : 'hidden',
					name : 'supplies_type_Name',
					width : 300
				}]
	});
	var applyOfficialSuppliesFormWin = new Ext.Window({
				title : '清单信息',
				width : 550,
				height : 320,
				items : applyOfficialSuppliesForm,
				closable : true,
				closeAction : 'hide',
				modal : true,
				layout : 'border'
			});
	applyOfficialSuppliesFormWin.show();

	if (button_num == 'edit') {

		applyOfficialSuppliesForm.getForm().load({
			url : __cxtPath
					+ '/applyManageMaint.do?method=getOfficalSuppliesByCompositeIds',
			params : {
				form_id : form_id_param,
				supplies_id : supplies_id_param
			},
			waitMsg : '正在载入数据...',
			waitTitle : '请等待...',
			success : function(form, action) {

				var supplies_id = applyOfficialSuppliesForm.getForm()
						.findField('supplies_id').getValue();
				var supplies_type_id = applyOfficialSuppliesForm.getForm()
						.findField('supplies_type_id').getValue();
				applyOfficialSuppliesForm.getForm()
						.findField('supplies_type_id')
						.setValue(supplies_type_id);
				applyOfficialSuppliesForm.getForm().findField('supplies_id')
						.setValue(supplies_id);
				var supplies_name = applyOfficialSuppliesForm.getForm()
						.findField('supplies_name').getValue();
				var supplies_type_Name = applyOfficialSuppliesForm.getForm()
						.findField('supplies_type_Name').getValue();
				officialSuppliesCombo.setValue(supplies_name);
				officialSuppliesTypeCombo.setValue(supplies_type_Name);
				officialSuppliesCombo.store.baseParams.type = supplies_type_id;
				officialSuppliesCombo.store.load();
			},
			failure : function(form, action) {
				Ext.MessageBox.alert('提示', '载入失败');
			}

		});

	}

};
