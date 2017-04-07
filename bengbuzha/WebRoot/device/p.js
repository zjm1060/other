// 采购清单
Ext.ns('com.bitc.xmis.device.PurchasingList');
/**
 * 采购清单查看或编辑Form
 * 
 * @class com.bitc.xmis.device.PurchasingList.Form
 * @extends Ext.form.FormPanel
 */
com.bitc.xmis.device.PurchasingList.Form = Ext.extend(Ext.form.FormPanel, {
	layout : 'auto',
	initComponent : function() {
		var tobeList = new com.bitc.xmis.device.PurchasingList.TobePurchaseList(
				{
					region : 'center',
					checkable : true,
					tbar : [{
						xtype : 'button',
						text : '添加',
						iconCls : 'add',
						handler : function() {
							var records = tobeList.getSelectionModel()
									.getSelections();
							if (records.length <= 0) {
								Ext.Msg.alert('提示', '请先勾选要添加的物资');
								return;
							}
							grid.getStore().add(records);
							tobeList.getStore().remove(records);
						}
					}, {
						iconCls : 'ok',
						text : '完成',
						handler : function() {
							var records = tobeList.getSelectionModel()
									.getSelections();
							grid.getStore().add(records);
							tobeList.getStore().remove(records);
							win.hide();
						}
					}, '-']
				});

		var win = new Ext.Window({
					title : '选择待采购物资',
					width : 900,
					height : 400,
					layout : 'border',
					items : tobeList,
					closeAction : 'hide',
					x : 200,
					y : 100
				});
		var grid = this.grid = new com.bitc.xmis.device.PurchasingList.DetailList(
				{
					autoHeight : true,
					viewConfig : {
						emptyText : '请添加物资',
						markDirty : false
					},
					plugins : [new Ext.ux.GridRowEditForm()],
					tbar : [{
								text : '添加物资',
								iconCls : 'add',
								scope : this,
								handler : function() {
									// tobeList.getStore().load();
									win.show();
								}
							}, {
								text : '删除',
								iconCls : 'delete',
								scope : this,
								handler : function() {
									var record = grid.getSelectionModel()
											.getSelected();
									if (record == null) {
										Ext.Msg.alert('提示', '请先选择要删除的物资');
										return;
									}
									grid.getStore().remove(record);
									tobeList.getStore().add(record);
								}
							}]
				});
		this.items = [{
			xtype : 'fieldset',
			title : '采购信息',
			defaults : {
				msgTarget : 'under'
			},
			items : [{
						xtype : 'hidden',
						name : 'id'
					}, {
						xtype : 'textfield',
						fieldLabel : '采购清单编码',
						width : 200,
						name : 'code',
						allowBlank : false
					}, {
						xtype : 'displayfield',
						name : 'entryName',
						fieldLabel : '创建人'
					}, {
						xtype : 'displayfield',
						name : 'entryDate',
						fieldLabel : '创建时间'
					}, {
						xtype : 'hidden',
						name : 'compId'
					}, {
						xtype : 'choosefield',
						name : 'compName',
						fieldLabel : '供货商',
						width : 200,
						winConfig : {
							title : '选择供货商'
						},
						allowBlank : false,
						baseParamFun : function() {
							return {
								businessRelation : 2,
								start : 0,
								limit : 20
							};
						},
						chooseUrl : __cxtPath
								+ '/resources/choose/company.json.jsp',
						setValueFun : function(o) {
							this.getForm().findField('compId').setValue(o
									.get(DBSchema.Company.CompanyID));
							this.getForm().findField('compName').setValue(o
									.get(DBSchema.Company.CompanyName));
						},
						clearValueFun : function() {
							this.getForm().findField('compId').setValue('');
							this.getForm().findField('compName').setValue('');
						},
						scope : this
					}, {
						xtype : 'textarea',
						anchor : '100%',
						fieldLabel : '备注',
						name : 'remark'
					}]
		}, {
			xtype : 'fieldset',
			title : '物资清单',
			items : grid
		}];
		com.bitc.xmis.device.PurchasingList.Form.superclass.initComponent
				.call(this);
	}
});
com.bitc.xmis.device.PurchasingList.Action = function() {
	return {
		submit : function(form, params) {
			form.getForm().submit({
				url : String.format('{0}/device.do?method=savePurchasingList',
						__cxtPath),
				method : 'post',
				waitMsg : '正在提交……',
				waitTitle : '请稍候',
				submitEmptyText : false,
				params : params,
				success : function(f, action) {
					Ext.Msg.hide();
					form.getForm().findField('id')
							.setValue(action.result.attributes.id);
					Ext.Msg.alert(action.result.messageTitle,
							action.result.message);
				},
				failure : function(f, action) {
					com.bitc.xmis.util.AlertUtil.formSubmitFailedAlert(action);
				}
			});
		}
	};
}();
/**
 * 创建、查看、编辑采购清单的页面
 */
Ext.onReady(function() {
	var request = com.bitc.xmis.device.PurchasingList.request;
	var purchasingId = request.getParameter('purchasingId');
	var form = new com.bitc.xmis.device.PurchasingList.Form({
		region : 'center',
		border : false,
		autoScroll : true,
		padding : 10,
		tbar : [{
			text : '保存',
			iconCls : 'ok',
			handler : function() {
				var details = com.bitc.xmis.util.ExtUtil
						.encodeStoreData(form.grid.getStore());
				var params = {
					detailstore : details
				};
				com.bitc.xmis.device.PurchasingList.Action.submit(form, params);
			}
		}]
	});

	new Ext.Viewport({
				layout : 'border',
				items : [form],
				border : false
			});

	var loadParam = {};
	if (!Ext.isEmpty(purchasingId, false)) {// 查看编辑已有申请
		loadParam = {
			purchasingId : purchasingId
		};
	};
	form.load({
				url : String.format('{0}/device.do?method=loadPurchasingList',
						__cxtPath),
				params : loadParam,
				success : function(form2, action) {
					form2.findField('entryName')
							.setValue(action.result.data.entryEmp == null
									? ''
									: action.result.data.entryEmp.userName);
					if (action.result.data.supplier) {
						form2
								.findField('compId')
								.setValue(action.result.data.supplier.companyID);
						form2
								.findField('compName')
								.setValue(action.result.data.supplier.companyName);
					}
					if (action.result.data.details.length > 0) {
						form.grid.getStore().loadData(action.result.data);
					}
				}
			});
	var details = request.getParameter('details');
	if (details) {
		form.grid.getStore().loadData({
					details : details
				});
	}
});