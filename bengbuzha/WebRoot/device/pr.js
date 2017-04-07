// 采购申请
Ext.ns('com.bitc.xmis.device.PurchaseRequisition');
com.bitc.xmis.device.PurchaseRequisition.initStep = function(request, form) {
	var toolbar = form.getTopToolbar();
	if (toolbar == null) {
		toolbar = new Ext.Toolbar();
		form.topToolbar = toolbar;
	}
	var readonly = request.getParameter('readonly');
	if (readonly) {
		return;
	}
	var step = request.getParameter('step');
	function submitForm(appendParams) {

		var details = com.bitc.xmis.util.ExtUtil.encodeStoreData(form.grid
				.getStore());
		var params = Ext.apply(appendParams || {}, {
					detailstore : details
				});
		form.getForm().submit({
			url : String.format('{0}/device.do?method=savePurchaseRequisition',
					__cxtPath),
			method : 'post',
			waitMsg : '正在提交……',
			waitTitle : '请稍候',
			submitEmptyText : false,
			params : params,
			success : function(f, action) {
				Ext.Msg.hide();
				if (action.result.attributes.processInstanceId) {
					toolbar.add(new com.bitc.xmis.workflow.TraceButton({
						processInstanceId : action.result.attributes.processInstanceId
					}));
				}
				Ext.Msg.alert(action.result.messageTitle,
						action.result.message, function() {
							form.submitButton.setDisabled(true);
						});
			},
			failure : function(f, action) {
				com.bitc.xmis.util.AlertUtil.formSubmitFailedAlert(action);
			}
		});
	}
	toolbar.add({
				text : '提交',
				ref : '../submitButton',
				iconCls : 'ok',
				handler : function() {
					Ext.Msg.prompt('任务意见', '请填写任务意见：', function(btn, text) {
								if (btn == 'ok') {
									submitForm({
												comments : text
											})
								}
							}, this, true);
				}
			});
	if (step != null) {
		form.add([{
					xtype : 'hidden',
					name : 'step',
					value : step
				}, {
					xtype : 'hidden',
					name : 'workItemId',
					value : request.getParameter('workItemId')
				}]);
		var processInstanceId = request.getParameter("processInstanceId");
		toolbar.add(new com.bitc.xmis.workflow.TraceButton({
					ref : 'traceButton',
					processInstanceId : processInstanceId
				}));
		if (step == 'PurchaseRequisition.CompPurchaseAssign.Task') {
			toolbar.remove(toolbar.ownerCt.submitButton);
			toolbar.insert(0, new com.bitc.xmis.workflow.SubmitButton({
								ref : '../submitButton',
								expression : '$role(16)',
								handler : function(userID) {
									submitForm({
												purchaser : userID
											});
								}
							}));
		} else if (step == 'PurchaseRequisition.DeptPurchaseAssign.Task') {
// 测试采购申请流程时候加的
//			toolbar.remove(toolbar.ownerCt.submitButton);
//			toolbar.insert(0, new com.bitc.xmis.workflow.SubmitButton({
//								ref : '../submitButton',
//								expression : '$role(106)',
//								handler : function(userID) {
//									submitForm({
//												purchaser : userID
//											});
//								}
//							}));
		}
	}
	if (request.getParameter("todoId") != null) {
		form.insert(0, new com.bitc.xmis.TodoInfoPanel({
							todoId : request.getParameter("todoId"),
							frame : true
						}));
	}
}
Ext.onReady(function() {
	Ext.QuickTips.init();
	var pr = com.bitc.xmis.device.PurchaseRequisition;
	var request = com.bitc.xmis.device.PurchaseRequisition.request;
	var requestId = request.getParameter('requestId');
	var projectId = request.getParameter('projectId');
	var seldeviceids = request.getParameter('seldeviceids');
	var form = new Ext.form.FormPanel({
		region : 'center',
		border : false,
		autoScroll : true,
		padding : 10,
		tbar : [],
		items : [{
					xtype : 'fieldset',
					title : '采购申请信息',
					items : [{
								xtype : 'hidden',
								name : 'id'
							}, {
								xtype : 'hidden',
								name : 'projectId'
							}, {
								xtype : 'displayfield',
								name : 'projectCode',
								fieldLabel : '项目编码'
							}, {
								xtype : 'displayfield',
								name : 'projectName',
								fieldLabel : '项目名称'
							}, {
								xtype : 'hidden',
								name : 'status',
								fieldLabel : '状态'
							}, {
								xtype : 'textarea',
								name : 'remark',
								fieldLabel : '备注',
								anchor : '80%'
							}, {
								xtype : 'displayfield',
								name : 'entryName',
								fieldLabel : '申请人'
							}, {
								xtype : 'displayfield',
								name : 'entryDate',
								fieldLabel : '申请时间'
							}]
				}, {
					xtype : 'fieldset',
					title : '申请采购物资明细（双击条目可编辑）',
					items : [
							{
								xtype : 'displayfield',
								name : 'sum',
								fieldLabel : '预计采购总额'
							},
							new com.bitc.xmis.device.PurchaseRequisition.DetailGrid(
									{
										ref : '../grid',
										cellTip : true,
										autoHeight : true
									})]
				}]
	});
	new Ext.Viewport({
				layout : 'border',
				items : [form],
				border : false
			});

	var loadParam = {};
	if (!Ext.isEmpty(requestId, false)) {// 查看编辑已有申请
		loadParam = {
			requestId : requestId
		};
	} else if (!Ext.isEmpty(projectId, false)) {// 从项目新建申请
		loadParam = {
			projectId : projectId,
			seldeviceids : seldeviceids
		};
	}
	function updateSum() {
		var sum = 0.0;
		var symbol;
		form.grid.getStore().each(function(record) {
					symbol = record.get('currency.symbol');
					sum += record.get('referencePrice') * record.get('number');
				});
		form.getForm().findField('sum').setValue(Ext.util.Format.number(sum,
				symbol + '0,0.00'));
	}
	form.grid.getStore().on('update', function(store, record, opt) {
				if (opt == Ext.data.Record.EDIT) {
					record.commit();
					updateSum();
				}
			});
	form.load({
		url : String.format('{0}/device.do?method=loadPurchaseRequisition',
				__cxtPath),
		params : loadParam,
		success : function(form2, action) {
			form2.findField('entryName')
					.setValue(action.result.data.entryEmp.userName);
			form.grid.getStore().loadData(action.result.data);
			updateSum();
			if (!form.getTopToolbar().traceButton
					&& !Ext.isEmpty(action.result.data.processInstanceId)) {
				form.getTopToolbar()
						.add(new com.bitc.xmis.workflow.TraceButton({
							processInstanceId : action.result.data.processInstanceId
						}));
				form.getTopToolbar().doLayout();

			}
		}
	});
	pr.initStep(request, form);
});