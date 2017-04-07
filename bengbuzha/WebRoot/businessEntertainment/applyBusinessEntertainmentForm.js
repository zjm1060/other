Ext.onReady(function() {
	Ext.QuickTips.init();
	var requestId = request.getParameter('form_id');
	var step = request.getParameter('step');
	var workItemId = request.getParameter('workItemId');
	var processInstanceId = request.getParameter('processInstanceId');
	var approvalInfoGridPanel = new com.bitc.xmis.workflow.approvalInfoList({
				collapsible : true,
				region : 'south',
				width : 700,
				height : 130,
				title : '审批记录',
				autoLoadData : true,
				autoScroll : true,
				processInstanceId : processInstanceId
			});

	var applyBusinessEntertainmentForm = com.bitc.xmis.BusinessEntertainment.BusinessEntertainmentFormPanel;
	var panel = new Ext.Panel({
		region : 'center',
		title : '',
		layout : 'border',
		border : false,
		autoScroll : true,
		tbar : {
			xtype : 'toolbar',
			items : [{
				xtype : 'button',
				text : '提交',
				hidden : true,
				ref : '../submit_btn',
				iconCls : 'save',
				handler : function() {
					if (applyBusinessEntertainmentForm.getForm().isValid()) {
						Ext.MessageBox.confirm("提示", "是否提交此条信息？",
								function(btn) {
									if (btn == 'yes') {
										applyBusinessEntertainmentForm
												.getForm().submit({
													url : __cxtPath
															+ '/applyManageMaint.do?method=doStartApplyBusinessEntertainment',
													params : {
														form_id : requestId,
														meals_date : applyBusinessEntertainmentForm
																.getForm()
																.findField('meals_date')
																.getValue()
													},
													scope : this,
													waitTitle : "请稍候",
													waitMsg : '提交数据，请稍候...',
													success : function(
															response, action) {
														Ext.MessageBox
																.alert(
																		action.result.messageTitle,
																		action.result.message);
														panel.submit_btn
																.setDisabled(true);
													},
													failure : function(
															response, action) {
														com.bitc.xmis.util.AlertUtil
																.formSubmitFailedAlert(action);
													}
												});
									}
								});

					}
				}
			}, /*{
				xtype : 'button',
				text : '修改签办号',
				hidden : true,
				ref : '../agree_signNo_btn',
				iconCls : 'add',
				handler : function() {
					var win = new Ext.Window({
						title : '签办号的修改',
						layout : 'border',
						width : 300,	
						height : 120,
						modal : true,
						border : false,
						items : [new Ext.form.FormPanel({
							region : 'center',
							ref : 'p',
							frame : true,
							border : false,
							labelAlign : 'left',
							labelWidth : 80,
							bodyStyle : 'padding:10px',
							buttonAlign : 'center',
							fbar : [{
								text : '确定',
								handler : function() {
									var sign_No_old = applyBusinessEntertainmentForm
											.getForm().findField('sign_No')
											.getValue();
									var sign_No_new = win.p.form
											.findField('sign_No_new')
											.getValue();
									// if (sign_No_old == sign_No_new) {
									// Ext.MessageBox.alert('提示',
									// '您没有修改，点击取消关闭此窗口');
									// return;
									// }
									Ext.Ajax.request({
										url : __cxtPath
												+ '/applyManageMaint.do?method=doUpdateSignNoByFormId',
										method : 'post',
										params : {
											sign_No : sign_No_new,
											form_id : requestId
										},
										success : function(response, action) {

											var result = Ext
													.decode(response.responseText);
											if (result.success) {
												Ext.Msg.alert(
														result.messageTitle,
														result.message);
												win.hide();
												applyBusinessEntertainmentForm
														.getForm()
														.findField('sign_No')
														.setValue(sign_No_new);
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
							}, {
								text : '取消',
								handler : function() {
									win.hide();
								}
							}],
							items : [{
										xtype : 'textfield',
										name : 'sign_No_new',
										width : 150,
										fieldLabel : '签办号'

									}]
						})]
					}).show();
					var sign_No_old = applyBusinessEntertainmentForm.getForm()
							.findField('sign_No').getValue();
					win.p.form.findField('sign_No_new').setValue(sign_No_old);
				}
			},*/ {
				xtype : 'button',
				text : '同意',
				hidden : true,
				id:'agree_btn',
				ref : '../agree_btn',
				iconCls : 'add',
				handler : function() {
//					if(step == 'ApplyBusinessEntertainment.OfficeManagerConfirm.Task'){
//						if(Ext.getCmp('sign_No').getValue() == '' ){
//							Ext.Msg.alert('提示','签办号不能为空！');
//							return;
//						} 
//					}
					com.bitc.xmis.workflow.DoApprovalTaskIsAgree(0, workItemId,
							true);
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

				}

			}, {
				xtype : 'button',
				text : '修改后提交',
				hidden : true,
				ref : '../goonsubmit_btn',
				iconCls : 'add',
				handler : function() {
					if (applyBusinessEntertainmentForm.getForm().isValid()) {

						Ext.MessageBox.confirm("提示", "是否提交此条信息？",
								function(btn) {
									if (btn == 'yes') {
										applyBusinessEntertainmentForm
												.getForm().submit({
													url : __cxtPath
															+ '/applyManageMaint.do?method=doStartApplyBusinessEntertainment',
													params : {
														form_id : requestId,
														meals_date : applyBusinessEntertainmentForm
																.getForm()
																.findField('meals_date')
																.getValue(),
														processInstanceId : processInstanceId,
														workItemId : workItemId
													},
													scope : this,
													waitTitle : "请稍候",
													waitMsg : '提交数据，请稍候...',
													success : function(
															response, action) {
														Ext.MessageBox
																.alert(
																		action.result.messageTitle,
																		action.result.message);
													},
													failure : function(
															response, action) {
														com.bitc.xmis.util.AlertUtil
																.formSubmitFailedAlert(action);
													}
												});
									}
								});
					}
				}
			}, {
				xtype : 'button',
				text : '提交',
				hidden : true,
				ref : '../canteen_submit_btn',
				iconCls : 'add',
				handler : function() {
				if(	Ext.getCmp('total_amt').getValue() == '0' ||
					Ext.getCmp('cost').getValue()== '0'){
					Ext.Msg.alert('提示','除烟酒金额外，其他为必填项！');
					return;
					}
					
					if (applyBusinessEntertainmentForm.getForm().isValid()) {
						Ext.MessageBox.confirm("提示", "是否提交此条信息？",
								function(btn) {
									if (btn == 'yes') {
										applyBusinessEntertainmentForm
												.getForm().submit({
													url : __cxtPath
															+ '/applyManageMaint.do?method=doUpdateApplyBusinessEntertainmentByCanteen',
													params : {
														form_id : requestId,
														meals_date : applyBusinessEntertainmentForm
																.getForm()
																.findField('meals_date')
																.getValue(),
														processInstanceId : processInstanceId,
														workItemId : workItemId
													},
													scope : this,
													waitTitle : "请稍候",
													waitMsg : '提交数据，请稍候...',
													success : function(
															response, action) {
														Ext.MessageBox
																.alert(
																		action.result.messageTitle,
																		action.result.message);
														panel.canteen_submit_btn
																.setDisabled(true);
													},
													failure : function(
															response, action) {
														com.bitc.xmis.util.AlertUtil
																.formSubmitFailedAlert(action);
													}
												});
									}
								});
					}
				}
			}, {
				xtype : 'button',
				text : '任务确定',
				hidden : true,
				ref : '../end_btn',
				iconCls : 'add',
				handler : function() {
//					var sign_No = Ext.getCmp('sign_No').getValue();
//					if(sign_No == null || sign_No ==''){
//						Ext.Msg.alert("提示","签办号不能为空！");
//						return;
//					}
					var url = '/applyManageMaint.do?method=doEndApply';
					Ext.MessageBox.confirm("提示", "是否确定结束此流程？", function(btn) {
						if (btn == 'yes') {
							Ext.Ajax.request({
										url : __cxtPath + url,
										method : 'post',
										params : {
											workItemId : workItemId,
											processInstanceId : processInstanceId
										},
										success : function(response, action) {

											var result = Ext
													.decode(response.responseText);
											if (result.success) {
												Ext.Msg.alert(
														result.messageTitle,
														result.message);
												panel.tbar
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
										+ '/applyManageMaint.do?method=doForceStopApplyBusinessEntertainment',
								method : 'post',
								params : {
									workItemId : workItemId,
									processInstanceId : processInstanceId,
									forceStop : true,
									businessEntertentWorkedId : 1,
									form_id : requestId
								},
								success : function(response, action) {

									var result = Ext
											.decode(response.responseText);
									if (result.success) {
										Ext.Msg.alert(result.messageTitle,
												result.message);
										gridPanel.tbar.setVisible(false);
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
		items : [applyBusinessEntertainmentForm, approvalInfoGridPanel]
	});

	applyBusinessEntertainmentForm.getForm().findField('total_amt').on('blur',
			function(c) {
				var value = applyBusinessEntertainmentForm.getForm()
						.findField('total_amt').getValue();
				var qian = getUpper(parseInt(value / 1000));
				var bai = getUpper(parseInt((value % 1000) / 100));
				var shi = getUpper(parseInt((value % 100) / 10));
				var yuan = getUpper(parseInt(value % 10));

				applyBusinessEntertainmentForm.getForm()
						.findField('total_amt_name')
						.setValue('(大写)&nbsp;&nbsp;' + qian
								+ '&nbsp;&nbsp;仟&nbsp;&nbsp; ' + bai
								+ '&nbsp;&nbsp;佰&nbsp;&nbsp; ' + shi
								+ '&nbsp;&nbsp;拾&nbsp;&nbsp; ' + yuan
								+ '&nbsp;&nbsp;元整 ');
			});

	function getUpper(val) {
		if (val == '1') {
			return '壹';
		} else if (val == '2') {
			return '贰';
		} else if (val == '3') {
			return '叁';
		} else if (val == '4') {
			return '肆';
		} else if (val == '5') {
			return '伍';
		} else if (val == '6') {
			return '陆';
		} else if (val == '7') {
			return '柒';
		} else if (val == '8') {
			return '捌';
		} else if (val == '9') {
			return '玖';
		} else {
			return '';
		}

	}
	new Ext.Viewport({
				layout : 'border',
				items : [panel],
				border : false
			});

	if (requestId == null || requestId == '') {
		// 新建
		applyBusinessEntertainmentForm.getForm().findField('app_user_id')
				.setValue(__emp.userID);
		applyBusinessEntertainmentForm.getForm().findField('app_user_name')
				.setValue(__emp.userName);
		applyBusinessEntertainmentForm.getForm().findField('app_deptment')
				.setValue(__emp.departmentID);
		applyBusinessEntertainmentForm.getForm().findField('app_deptment_name')
				.setValue(__emp.departmentName);
		applyBusinessEntertainmentForm.getForm().findField('app_datetime')
				.setValue(Ext.util.Format.date(new Date(), 'Y-m-d'));

	} else {
		
		if(step == 'ApplyBusinessEntertainment.CanteenExecutorHandle.Task'){

			Ext.Ajax.request({
				url : __cxtPath
						+ '/applyManageMaint.do?method=doUpdateSignNoByFormId',
				method : 'post',
				params : {
					sign_No : 'new_sign_no',
					form_id : requestId
				},
				success : function(response, action) {
					applyBusinessEntertainmentForm.getForm().load({
						url : __cxtPath
								+ '/applyManageMaint.do?method=getBusinessEntertainmentFormByFormId',
						params : {
							form_id : requestId
						},
						waitMsg : '正在载入数据...',
						waitTitle : '请等待...',
						success : function(form, action) {
							var value = applyBusinessEntertainmentForm.getForm()
									.findField('total_amt').getValue();
							var qian = getUpper(parseInt(value / 1000));
							var bai = getUpper(parseInt((value % 1000) / 100));
							var shi = getUpper(parseInt((value % 100) / 10));
							var yuan = getUpper(parseInt(value % 10));

							applyBusinessEntertainmentForm.getForm()
									.findField('total_amt_name')
									.setValue('(大写)&nbsp;&nbsp;' + qian
											+ '&nbsp;&nbsp;仟&nbsp;&nbsp; ' + bai
											+ '&nbsp;&nbsp;佰&nbsp;&nbsp; ' + shi
											+ '&nbsp;&nbsp;拾&nbsp;&nbsp; ' + yuan
											+ '&nbsp;&nbsp;元整 ');
						},
						failure : function(form, action) {
							Ext.MessageBox.alert('提示', '载入失败');
						}

					});
				},
				failure : function(response, action) {}
			});
			
		
		}else{
			applyBusinessEntertainmentForm.getForm().load({
				url : __cxtPath
						+ '/applyManageMaint.do?method=getBusinessEntertainmentFormByFormId',
				params : {
					form_id : requestId
				},
				waitMsg : '正在载入数据...',
				waitTitle : '请等待...',
				success : function(form, action) {
					var value = applyBusinessEntertainmentForm.getForm()
							.findField('total_amt').getValue();
					var qian = getUpper(parseInt(value / 1000));
					var bai = getUpper(parseInt((value % 1000) / 100));
					var shi = getUpper(parseInt((value % 100) / 10));
					var yuan = getUpper(parseInt(value % 10));

					applyBusinessEntertainmentForm.getForm()
							.findField('total_amt_name')
							.setValue('(大写)&nbsp;&nbsp;' + qian
									+ '&nbsp;&nbsp;仟&nbsp;&nbsp; ' + bai
									+ '&nbsp;&nbsp;佰&nbsp;&nbsp; ' + shi
									+ '&nbsp;&nbsp;拾&nbsp;&nbsp; ' + yuan
									+ '&nbsp;&nbsp;元整 ');
				},
				failure : function(form, action) {
					Ext.MessageBox.alert('提示', '载入失败');
				}

			});
		}
		
		

	}

	/*if (step == 'ApplyBusinessEntertainment.OfficeManagerConfirm.Task') {
		applyBusinessEntertainmentForm.getForm().findField('sign_No').on(
				'focus', function(field) {
					var win = new Ext.Window({
						title : '签办号的修改',
						layout : 'border',
						width : 300,
						height : 120,
						modal : true,
						border : false,
						items : [new Ext.form.FormPanel({
							region : 'center',
							ref : 'p',
							frame : true,
							border : false,
							labelAlign : 'left',
							labelWidth : 80,
							bodyStyle : 'padding:10px',
							buttonAlign : 'center',
							fbar : [{
								text : '确定',
								id : 'sign_no_btn',
								handler : function() {
									var sign_No_old = applyBusinessEntertainmentForm
											.getForm().findField('sign_No')
											.getValue();
									var sign_No_new = win.p.form
											.findField('sign_No_new')
											.getValue();
									// if (sign_No_old ==
									// sign_No_new) {
									// Ext.MessageBox.alert('提示',
									// '您没有修改，点击取消关闭此窗口');
									// return;
									// }
									Ext.Ajax.request({
										url : __cxtPath
												+ '/applyManageMaint.do?method=doUpdateSignNoByFormId',
										method : 'post',
										params : {
											sign_No : sign_No_new,
											form_id : applyBusinessEntertainmentForm
													.getForm()
													.findField('form_id')
													.getValue()
										},
										success : function(response, action) {

											var result = Ext
													.decode(response.responseText);
											if (result.success) {
												Ext.Msg.alert(
														result.messageTitle,
														result.message);
												win.hide();
												applyBusinessEntertainmentForm
														.getForm()
														.findField('sign_No')
														.setValue(sign_No_new);
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
							}, {
								text : '取消',
								handler : function() {
									win.hide();
								}
							}],
							items : [{
										xtype : 'textfield',
										name : 'sign_No_new',
										width : 150,
										fieldLabel : '签办号'

									}]
						})]
					}).show();
					var sign_No_old = applyBusinessEntertainmentForm.getForm()
							.findField('sign_No').getValue();
					win.p.form.findField('sign_No_new').setValue(sign_No_old);

				});

	}*/

	if (step == null || step == '') {
		// 新建
		 if(request.getParameter("todoId") == 'query'){
		 	panel.submit_btn.hide();
		 	app_user_fieldset_readonly();
		 	canteen_executor_fieldset_readonly();
		}else{
			 Ext.getCmp('canteen_executor_fieldset').hide();
			panel.submit_btn.show();
		}
		// Ext.getCmp('assign_user_fieldset').setDisabled(true);
		// Ext.getCmp('canteen_executor_fieldset').setDisabled(true);
		canteen_executor_fieldset_readonly();
	} else if (step == 'ApplyBusinessEntertainment.DeptManagerApproval.Task'
			|| step == 'ApplyBusinessEntertainment.ChargeLeaderApproval.Task'
			|| step == 'ApplyBusinessEntertainment.LogisticsLeaderApproval.Task'
			|| step == 'ApplyBusinessEntertainment.MagenagerLeaderApprova.Task') {
		// 审批
		panel.agree_btn.show();
		// panel.agree_signNo_btn.show();
		panel.notagree_btn.show();

		// Ext.getCmp('app_user_fieldset').setDisabled(true);
		app_user_fieldset_readonly();
		// Ext.getCmp('canteen_executor_fieldset').setDisabled(true);

	} else if (step == 'ApplyBusinessEntertainment.OfficeManagerConfirm.Task') {
		// 确定
		panel.end_btn.show();
	} else if (step == 'ApplyBusinessEntertainment.ApplicantApply.Task') {
		// 退回
		panel.goonsubmit_btn.show();
		panel.forcestop_btn.show();

		// Ext.getCmp('assign_user_fieldset').setDisabled(true);
		// Ext.getCmp('canteen_executor_fieldset').setDisabled(true);
		canteen_executor_fieldset_readonly();
	} else if (step == 'ApplyBusinessEntertainment.CanteenExecutorHandle.Task') {
		panel.canteen_submit_btn.show();

		// Ext.getCmp('app_user_fieldset').setDisabled(true);
		app_user_fieldset_readonly();
	}

	function app_user_fieldset_readonly() {
		applyBusinessEntertainmentForm.getForm()
				.findField('visitor_company_name').setReadOnly(true);
		applyBusinessEntertainmentForm.getForm().findField('visitor_qty')
				.setReadOnly(true);
		applyBusinessEntertainmentForm.getForm().findField('meals_date')
				.setReadOnly(true);
		applyBusinessEntertainmentForm.getForm().findField('meals_type')
				.setDisabled(true);
		applyBusinessEntertainmentForm.getForm().findField('meals_fee_budget')
				.setReadOnly(true);
		applyBusinessEntertainmentForm.getForm().findField('accompany_qty')
				.setReadOnly(true);
		applyBusinessEntertainmentForm.getForm().findField('remark')
				.setReadOnly(true);
	}
	function canteen_executor_fieldset_readonly() {
		applyBusinessEntertainmentForm.getForm()
				.findField('cigarettes_wine_amt').setReadOnly(true);
//		applyBusinessEntertainmentForm.getForm().findField('profit')
//				.setReadOnly(true);
		applyBusinessEntertainmentForm.getForm().findField('total_amt')
				.setReadOnly(true);
		applyBusinessEntertainmentForm.getForm().findField('cost')
				.setReadOnly(true);
	}
	if (request.getParameter("todoId") == null) {
		approvalInfoGridPanel.hide();
	}
});