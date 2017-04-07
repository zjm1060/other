Ext.onReady(function() {
	Ext.QuickTips.init();
	var exportname = '审批列表';
	var PRIORITY_ARRAY = ['普通', '紧急'];
	var requestId = request.getParameter('form_id');
	var step = request.getParameter('step');
	var workItemId = request.getParameter('workItemId');
	var processInstanceId = request.getParameter('processInstanceId');
	var storeUrl = '/applyManageMaint.do?method=getApplyOfficialSuppliesList';
	if (type == 'repair_type') {
		storeUrl = '/applyManageMaint.do?method=getApplyOfficialSuppliesRepairList';
	}
	var gridPanel = new com.bitc.xmis.ApplyOfficialSupplies.DetailGrid({
		loadMask : true,
		dataUrl : storeUrl,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		tbar : {
			xtype : 'toolbar',
			items : [{
				xtype : 'button',
				text : '添加',
				ref : '../add_btn',
				hidden : true,
				iconCls : 'add',
				handler : function() {
					var dataUrl = '/applyManageMaint.do?method=doSaveApplyOfficialSuppliesForm';
					if (type == 'repair_type') {
						dataUrl = '/applyManageMaint.do?method=doSaveApplyOfficialSuppliesRepairForm';
					}
					com.bitc.xmis.ApplyOfficialSupplies.WindowShow(form,
							gridPanel, 'create', dataUrl, '', '');
				}
			}]
		},
		actionColumn : {
			xtype : 'actioncolumn',
			width : 60,
			hidden : true,
			items : [{
				iconCls : 'edit',
				text : '编辑',
				tooltip : '编辑',
				handler : function(grid, rowIndex, colIndex) {
					var dataUrl = '/applyManageMaint.do?method=doUpdateApplyOfficialSuppliesForm';
					if (type == 'repair_type') {
						dataUrl = '/applyManageMaint.do?method=doUpdateApplyOfficialSuppliesRepairForm';
					}
					var form_id_param = grid.store.getAt(rowIndex)
							.get('form_id');
					var supplies_id_param = grid.store.getAt(rowIndex)
							.get('supplies_id');

					com.bitc.xmis.ApplyOfficialSupplies.WindowShow(form,
							gridPanel, 'edit', dataUrl, form_id_param,
							supplies_id_param);
				}
			}, {
				ref : '../removeBtn',
				iconCls : 'remove2',
				text : '删除',
				tooltip : '删除',
				handler : function(grid, rowIndex, colIndex) {
					var form_id_param = grid.store.getAt(rowIndex)
							.get('form_id');
					var supplies_id_param = grid.store.getAt(rowIndex)
							.get('supplies_id');
					Ext.MessageBox.confirm("提示", "是否删除此条信息？", function(btn) {
						if (btn == 'yes') {
							var dataUrl = '/applyManageMaint.do?method=doDeleteApplyOfficialSuppliesForm';
							if (type == 'repair_type') {
								dataUrl = '/applyManageMaint.do?method=doDeleteApplyOfficialSuppliesRepairForm';
							}
							Ext.Ajax.request({
								url : __cxtPath + dataUrl,
								method : 'post',
								params : {
									form_id : form_id_param,
									supplies_id : supplies_id_param
								},
								success : function(response, action) {
									Ext.MessageBox.alert("提示", "删除成功！");
									gridPanel.store.baseParams.form_id = form_id_param;
									gridPanel.store.reload();
								},
								failure : function(response, action) {
									Ext.MessageBox.hide();
									Ext.MessageBox.alert("提示", "操作失败！");
								}
							});

						}
					});
				}
			}]
		},
		height : 150,
		autoScroll : true,
		autoLoadData : false
	});
	gridPanel.store.on('load', function(store, records, options) {

				if (step == null || step == '' || store.data.length == 1) {
					form.getForm().findField('app_user_name')
							.setValue(__emp.userName);
					form.getForm().findField('app_datetime')
							.setValue(Ext.util.Format.date(new Date(),
									'Y-m-d H:i:s'));
				} else {
					form.getForm().findField('app_user_name')
							.setValue(records[0].data.app_user_name);
					form.getForm().findField('app_datetime')
							.setValue(records[0].data.app_datetime);
				}

			});
	var form = new Ext.form.FormPanel({
		region : 'center',
		border : false,
		autoScroll : true,
		padding : 10,
		tbar : {
			xtype : 'toolbar',
			items : [{
				xtype : 'button',
				text : '提交',
				hidden : true,
				ref : '../submit_btn',
				iconCls : 'save',
				handler : function() {
					if (gridPanel.store.data.length == 0) {
						Ext.MessageBox.alert("提示", "申请单不可为空！");
						return;
					} else {
						var dataUrl = '/applyManageMaint.do?method=doStartApplyOfficialSupplies';
						if (type == 'repair_type') {
							dataUrl = '/applyManageMaint.do?method=doStartApplyOfficialSuppliesRepair';
						}
						Ext.MessageBox.confirm("提示", "是否提交此条信息？",
								function(btn) {
									if (btn == 'yes') {
										Ext.Ajax.request({
											url : __cxtPath + dataUrl,
											method : 'post',
											params : {
												form_id : form.getForm()
														.findField('form_id')
														.getValue()
											},
											success : function(response) {

												var result = Ext
														.decode(response.responseText);
												if (result.success) {
													Ext.Msg
															.alert(
																	result.messageTitle,
																	result.message);
													gridPanel.store.baseParams.form_id = form
															.getForm()
															.findField('form_id')
															.getValue();
													gridPanel.store.reload();
													form.tbar.setVisible(false);
													gridPanel.tbar
															.setVisible(false);
													gridPanel.getColumnModel()
															.setHidden(6, true);
													parent.xMis
															.refresh('mytodo',
																	'myworkitemnewtodo');

												} else {
													Ext.Msg
															.alert(
																	result.messageTitle,
																	result.message);
												}

											},
											failure : function(response) {
												Ext.MessageBox.hide();
												Ext.Msg.alert(
														result.messageTitle,
														result.message);
											}
										});

									}
								});
					}

				}
			}, {
				xtype : 'button',
				text : '同意',
				hidden : true,
				ref : '../agree_btn',
				iconCls : 'add',
				handler : function() {
					com.bitc.xmis.workflow.DoApprovalTaskIsAgree(0, workItemId,
							true);
					// tbar grid上的新建、编辑、删除
				}
			}, {
				xtype : 'button',
				text : '不同意',
				hidden : true,
				ref : '../notagree_btn',
				iconCls : 'add',
				handler : function() {
					com.bitc.xmis.workflow.DoApprovalTaskIsAgree(1, workItemId,
							true);
					// tbar grid上的新建、编辑、删除
				}
			}, {
				xtype : 'button',
				text : '修改后提交',
				hidden : true,
				ref : '../goonsubmit_btn',
				iconCls : 'add',
				handler : function() {
					Ext.MessageBox.confirm("提示", "是否确定提交？", function(btn) {
						if (btn == 'yes') {
							Ext.Ajax.request({
								url : __cxtPath
										+ '/applyManageMaint.do?method=doGoOnSubmitApplyOfficialSupplies',
								method : 'post',
								params : {
									workItemId : workItemId
								},
								success : function(response, action) {

									var result = Ext
											.decode(response.responseText);
									if (result.success) {
										Ext.Msg.alert(result.messageTitle,
												result.message);
										form.tbar.setVisible(false);
										gridPanel.tbar.setVisible(false);
										gridPanel.getColumnModel().setHidden(6, true);
										parent.xMis.refresh('mytodo',
												'myworkitemnewtodo');

									} else {
										Ext.Msg.alert(result.messageTitle,
												result.message);
									}

								},
								failure : function(response, action) {
									var result = Ext
											.decode(response.responseText);
									Ext.MessageBox.hide();
									Ext.Msg.alert(result.messageTitle,
											result.message);
								}
							});

						}
					});
				}
			}, {
				xtype : 'button',
				text : '任务确定',
				hidden : true,
				ref : '../end_btn',
				iconCls : 'add',
				handler : function() {
					var url = '/applyManageMaint.do?method=doEndApplyOfficialSuppliesUpdateNum';
					if (type == 'repair_type') {
						url = '/applyManageMaint.do?method=doEndApply';
					}
					Ext.MessageBox.confirm("提示", "是否确定结束此流程？", function(btn) {
						if (btn == 'yes') {
							Ext.Ajax.request({
										url : __cxtPath + url,
										method : 'post',
										params : {
											workItemId : workItemId,
											form_id : requestId,
											processInstanceId : processInstanceId
										},
										success : function(response, action) {

											var result = Ext
													.decode(response.responseText);
											if (result.success) {
												Ext.Msg.alert(
														result.messageTitle,
														result.message);
												form.tbar.setVisible(false);
												gridPanel.tbar
														.setVisible(false);
												parent.xMis.refresh('mytodo',
														'myworkitemnewtodo');

											} else {
												Ext.Msg.alert(
														result.messageTitle,
														result.message);
											}

										},
										failure : function(response, action) {
											var result = Ext
													.decode(response.responseText);
											Ext.MessageBox.hide();
											Ext.Msg.alert(result.messageTitle,
													result.message);
										}
									});

						}
					});
				}
			}, {
				xtype : 'button',
				text : '强制结束',
				hidden : true,
				ref : '../forcestop_btn',
				iconCls : 'add',
				handler : function() {
					Ext.MessageBox.confirm("提示", "是否确定结束此流程？", function(btn) {
						if (btn == 'yes') {
							Ext.Ajax.request({
								url : __cxtPath
										+ '/applyManageMaint.do?method=doEndApply',
								method : 'post',
								params : {
									workItemId : workItemId,
									processInstanceId : processInstanceId,
									forceStop : true
								},
								success : function(response, action) {

									var result = Ext
											.decode(response.responseText);
									if (result.success) {
										Ext.Msg.alert(result.messageTitle,
												result.message);
										form.tbar.setVisible(false);
										gridPanel.tbar.setVisible(false);
										gridPanel.getColumnModel().setHidden(6, true);
										parent.xMis.refresh('mytodo',
												'myworkitemnewtodo');

									} else {

										Ext.Msg.alert(result.messageTitle,
												result.message);
									}

								},
								failure : function(response, action) {
									var result = Ext
											.decode(response.responseText);
									Ext.MessageBox.hide();
									Ext.Msg.alert(result.messageTitle,
											result.message);
								}
							});

						}
					});
				}
			}]
		},
		items : [{
					xtype : 'fieldset',
					title : (type == 'repair_type')
							? '办公用品维修申请明细'
							: '办公用品领用申请明细',
					items : [{
								xtype : 'hidden',
								name : 'form_id',
								fieldLabel : '表单ID'
							}, {
								xtype : 'displayfield',
								name : 'app_user_name',
								fieldLabel : '申请人'
							}, {
								xtype : 'displayfield',
								name : 'app_datetime',
								fieldLabel : '申请时间'
							}, gridPanel]
				}]
	});
	if (type == 'repair_type') {
		if (step == null || step == '') {
			// 新建
			form.submit_btn.show();
			gridPanel.getColumnModel().setHidden(6, false);
			gridPanel.add_btn.show();
		} else if (step == 'ApplyOfficialSuppliesRepair.DeptManagerApproval.Task'
				|| step == 'ApplyOfficialSuppliesRepair.ChargeLeaderApproval.Task'
				|| step == 'ApplyOfficialSuppliesRepair.OfficeDirectorApproval.Task') {
			// 审批
			form.agree_btn.show();
			form.notagree_btn.show();
		} else if (step == 'ApplyOfficialSuppliesRepair.SuppliesManagerConfirm.Task') {
			// 确定
			form.end_btn.show();

		} else if (step == 'ApplyOfficialSuppliesRepair.ApplicantApply.Task') {
			// 退回
			form.goonsubmit_btn.show();
			form.forcestop_btn.show();
			gridPanel.add_btn.show();
			gridPanel.getColumnModel().setHidden(6, false);
		}
	} else {

		if (step == null || step == '') {
			// 新建
			form.submit_btn.show();
			gridPanel.getColumnModel().setHidden(6, false);
			gridPanel.add_btn.show();
		} else if (step == 'ApplyOfficialSupplies.DeptManagerApproval.Task'
				|| step == 'ApplyOfficialSupplies.ChargeLeaderApproval.Task'
				|| step == 'ApplyOfficialSupplies.OfficeDirectorApproval.Task') {
			// 审批
			form.agree_btn.show();
			form.notagree_btn.show();
		} else if (step == 'ApplyOfficialSupplies.SuppliesManagerConfirm.Task') {
			// 确定
			form.end_btn.show();

		} else if (step == 'ApplyOfficialSupplies.ApplicantApply.Task') {
			// 退回
			form.goonsubmit_btn.show();
			form.forcestop_btn.show();
			gridPanel.add_btn.show();
			gridPanel.getColumnModel().setHidden(6, false);
		}
	}

	if (requestId == null || requestId == '') {
		form.getForm().findField('app_user_name').setValue(__emp.userName);
		form.getForm().findField('app_datetime').setValue(Ext.util.Format.date(
				new Date(), 'Y-m-d H:i:s'));
	} else {
		gridPanel.store.baseParams.form_id = requestId;
		gridPanel.store.reload();

	}
		var approvalInfoGridPanel = new com.bitc.xmis.workflow.approvalInfoList(
				{
					collapsible : true,
					width : 700,
					height : 150,
					region: 'south',
					title : '审批记录',
					autoLoadData : true,
					autoScroll : true,
					processInstanceId : processInstanceId,
					sortInfo : {
						field : '审批时间',
						direction : 'ASC'
					}
				});
		if (request.getParameter("todoId") == null) {
			approvalInfoGridPanel.hide();
		}
	new Ext.Viewport({
				layout : 'border',
				items : [form,approvalInfoGridPanel],
				border : false
			});
	if (request.getParameter("todoId") == null) {
		approvalInfoGridPanel.hide();
	}
	var datetimeFormatFun = function(v) {
		return v != "" ? v.substring(0, 16) : "";
	};

});
