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
				height : 150,
				title : '审批记录',
				autoLoadData : true,
				autoScroll : true,
				processInstanceId : processInstanceId
			});
	var absentForm = com.bitc.xmis.AskForLeave.AbsentFormPanel;
	var attFileGrid = new com.bitc.xmis.AttachmentList({
		title : '相关文件列表',
		collapsible : true,
		width : 300,
		heigth : 400,
		region : 'east',
		stripeRows : true,
		REFID : requestId == null ? 0 : requestId,
		viewConfig : {
			emptyText : '病假超过３天及以上时间的，填写<b>销假申请</b>时候必须<b>上传</b>医院及医疗机构证明(病历、化验单、住院缴费单等)'
		},
		attachmentTypeID : com.bitc.xmis.Attachment.absentForm
	});

	var panel = new Ext.Panel({
		region : 'center',
		title : '',
		layout : 'border',
		frame : false,
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

					if (absentForm.getForm().isValid()) {

						// 请假类型；
						var absent_type_value = Ext.getCmp('absent_type_group').getValue();
						// 请假时间

						Ext.MessageBox.confirm("提示", "是否提交此条信息？",
								function(btn) {
									if (btn == 'yes') {
										absentForm.getForm().submit({
											url : __cxtPath
													+ '/applyManageMaint.do?method=doStartAskForLeave',
											params : {
												form_id : requestId,
												app_user_id : __emp.userID,
												plan_begin_date : Ext.util.Format
														.date(
																absentForm
																		.getForm()
																		.findField('plan_begin_date')
																		.getValue(),
																'Y-m-d')
														+ ' '
														+ absentForm
																.getForm()
																.findField('plan_begin_hour')
																.getValue()
														+ ':00:00',
												plan_end_date : Ext.util.Format
														.date(
																absentForm
																		.getForm()
																		.findField('plan_end_date')
																		.getValue(),
																'Y-m-d')
														+ ' '
														+ absentForm
																.getForm()
																.findField('plan_end_hour')
																.getValue()
														+ ':00:00',
												plan_sum: absentForm
																.getForm()
																.findField('plan_sum')
																.getValue(),
												absent_type_id : absent_type_value
											},
											scope : this,
											waitTitle : "请稍候",
											waitMsg : '提交数据，请稍候...',
											success : function(response, action) {
												Ext.MessageBox
														.alert(
																action.result.messageTitle,
																action.result.message);
												panel.submit_btn
														.setDisabled(true);
											},
											failure : function(response, action) {
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
				text : '同意',
				hidden : true,
				ref : '../agree_btn',
				iconCls : 'add',
				handler : function() {
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
					if (absentForm.getForm().isValid()) {
						var absent_type_value = Ext.getCmp('absent_type_id').getValue();
						Ext.MessageBox.confirm("提示", "是否提交此条信息？",
								function(btn) {
									if (btn == 'yes') {
										absentForm.getForm().submit({
											url : __cxtPath
													+ '/applyManageMaint.do?method=doStartAskForLeave',
											params : {
												form_id : requestId,
												absent_type_id : absent_type_value,
												plan_begin_date : Ext.util.Format
														.date(
																absentForm
																		.getForm()
																		.findField('plan_begin_date')
																		.getValue(),
																'Y-m-d')
														+ ' '
														+ absentForm
																.getForm()
																.findField('plan_begin_hour')
																.getValue()
														+ ':00:00',
												plan_end_date : Ext.util.Format
														.date(
																absentForm
																		.getForm()
																		.findField('plan_end_date')
																		.getValue(),
																'Y-m-d')
														+ ' '
														+ absentForm
																.getForm()
																.findField('plan_end_hour')
																.getValue()
														+ ':00:00',
												actual_begin_date : Ext.util.Format
														.date(
																absentForm
																		.getForm()
																		.findField('actual_begin_date')
																		.getValue(),
																'Y-m-d')
														+ ' '
														+ absentForm
																.getForm()
																.findField('actual_begin_hour')
																.getValue()
														+ ':00:00',
												actual_end_date : Ext.util.Format
														.date(
																absentForm
																		.getForm()
																		.findField('actual_end_date')
																		.getValue(),
																'Y-m-d')
														+ ' '
														+ absentForm
																.getForm()
																.findField('actual_end_hour')
																.getValue()
														+ ':00:00',
												plan_sum: absentForm
																.getForm()
																.findField('plan_sum')
																.getValue(),
												processInstanceId : processInstanceId,
												workItemId : workItemId
											},
											scope : this,
											waitTitle : "请稍候",
											waitMsg : '提交数据，请稍候...',
											success : function(response, action) {
												Ext.MessageBox
														.alert(
																action.result.messageTitle,
																action.result.message);
												panel.tbar.hide();
											},
											failure : function(response, action) {
												com.bitc.xmis.util.AlertUtil
														.formSubmitFailedAlert(action);
											}
										});
									}
								});
					}
				}
			}, {
				xtype : 'uploadButton',
				text : '附件上传',
				REFID : requestId,
				hidden : true,
				ref : '../back_upload_btn',
				attachmentTypeID : com.bitc.xmis.Attachment.absentForm,
				uploadPanelConfig : {
					listeners : {
						'uploadcomplete' : function() {
							attFileGrid.reload();
						}
					}
				}
			}, {
				xtype : 'button',
				text : '提交',
				hidden : true,
				ref : '../back_submit_btn',
				iconCls : 'add',
				handler : function() {
					// 病假超过３天 ，attFileGrid列表不可为空
					var absent_type_value = Ext.getCmp('absent_type_id').getValue();
					var actual_begin_date = absentForm.getForm()
							.findField('actual_begin_date').getValue();
					var actual_end_date = absentForm.getForm()
							.findField('actual_end_date').getValue();

					var actual_sum = absentForm.getForm()
							.findField('actual_sum').getValue();

					if (actual_begin_date == '' || actual_end_date == '') {
						Ext.MessageBox.alert('提示', '请填写实际请假时间');
						return;
					} else if (absent_type_value == 3
							&& actual_sum >= 3
							&& attFileGrid.store.data.length < 1) {
						Ext.Msg.alert('提示', '您未上传相关附件');
						return;
					} else {

						if (absentForm.getForm().isValid()) {

							Ext.MessageBox.confirm("提示", "是否提交此条信息？", function(
									btn) {
								if (btn == 'yes') {
									absentForm.getForm().submit({
										url : __cxtPath
												+ '/applyManageMaint.do?method=doUpdateAbsentForm',
										params : {
											form_id : requestId,
											actual_begin_date : Ext.util.Format
													.date(
															absentForm
																	.getForm()
																	.findField('actual_begin_date')
																	.getValue(),
															'Y-m-d')
													+ ' '
													+ absentForm
															.getForm()
															.findField('actual_begin_hour')
															.getValue()
													+ ':00:00',
											actual_end_date : Ext.util.Format
													.date(
															absentForm
																	.getForm()
																	.findField('actual_end_date')
																	.getValue(),
															'Y-m-d')
													+ ' '
													+ absentForm
															.getForm()
															.findField('actual_end_hour')
															.getValue()
													+ ':00:00',
											actual_sum: absentForm
																.getForm()
																.findField('actual_sum')
																.getValue(),
											processInstanceId : processInstanceId,
											workItemId : workItemId
										},
										scope : this,
										waitTitle : "请稍候",
										waitMsg : '提交数据，请稍候...',
										success : function(response, action) {
											Ext.MessageBox.alert(
													action.result.messageTitle,
													action.result.message);
											panel.tbar.hide();
										},
										failure : function(response, action) {
											com.bitc.xmis.util.AlertUtil
													.formSubmitFailedAlert(action);
										}
									});
								}
							});
						}
					}
				}

			}, {
				xtype : 'button',
				text : '任务确定',
				hidden : true,
				ref : '../end_btn',
				iconCls : 'add',
				handler : function() {
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
												parent.xMis.refresh('mytodo',
														'myworkitemnewtodo');

											} else {
												Ext.Msg.alert(
														result.messageTitle,
														result.message);
											}
											panel.tbar.hide();

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
					var url = '/applyManageMaint.do?method=doEndApply';
					Ext.MessageBox.confirm("提示", "是否确定结束此流程？", function(btn) {
						if (btn == 'yes') {
							Ext.Ajax.request({
										url : __cxtPath + url,
										method : 'post',
										params : {
											workItemId : workItemId,
											forceStop : true,
											processInstanceId : processInstanceId
										},
										success : function(response, action) {

											var result = Ext
													.decode(response.responseText);
											if (result.success) {
												Ext.Msg.alert(
														result.messageTitle,
														result.message);
												parent.xMis.refresh('mytodo',
														'myworkitemnewtodo');

											} else {
												Ext.Msg.alert(
														result.messageTitle,
														result.message);
											}
											panel.tbar.hide();
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
		items : [absentForm, attFileGrid, approvalInfoGridPanel]
	});

	if (request.getParameter("todoId") == null) {
		approvalInfoGridPanel.hide();
	}
	new Ext.Viewport({
				layout : 'border',
				items : [panel],
				border : false
			});
	if (requestId == null || requestId == '') {
		// 新建
		absentForm.getForm().findField('app_user_id').setValue(__emp.userID);
		absentForm.getForm().findField('app_user_name')
				.setValue(__emp.userName);
		absentForm.getForm().findField('app_datetime').setValue(Ext.util.Format
				.date(new Date(), 'Y-m-d'));

	} else {
		absentForm.getForm().load({
			url : __cxtPath
					+ '/applyManageMaint.do?method=getAbsentFormByFormId',
			params : {
				form_id : requestId
			},
			waitMsg : '正在载入数据...',
			waitTitle : '请等待...',
			success : function(form, action) {

				var plan_begin_time = new Date(Date.parse(absentForm.getForm()
						.findField('plan_begin_time').getValue().replace(/-/g,
								"/")));
				var plan_end_time = new Date(Date.parse(absentForm.getForm()
						.findField('plan_end_time').getValue().replace(/-/g,
								"/")));

				absentForm.getForm().findField('plan_begin_hour')
						.setValue(plan_begin_time.dateFormat('H'));
				absentForm.getForm().findField('plan_end_hour')
						.setValue(plan_end_time.dateFormat('H'));
				if (step == 'AskForLeave.ApplicantReportBack.Task') {
					 absentForm.getForm().findField('leavetime').setValue(Ext.util.Format
				.date(new Date(), 'Y-m-d H:i:s'));
				}
				if (absentForm.getForm().findField('actual_begin_time')
						.getValue() != ''
						&& absentForm.getForm().findField('actual_end_time')
								.getValue() != '') {
					var actual_begin_time = new Date(Date.parse(absentForm
							.getForm().findField('actual_begin_time')
							.getValue().replace(/-/g, "/")));
					var actual_end_time = new Date(Date.parse(absentForm
							.getForm().findField('actual_end_time').getValue()
							.replace(/-/g, "/")));

					absentForm.getForm().findField('actual_begin_hour')
							.setValue(actual_begin_time.dateFormat('H'));
					absentForm.getForm().findField('actual_end_hour')
							.setValue(actual_end_time.dateFormat('H'));
				}

				com.bitc.xmis.AskForLeave.DateValidAbsentDaysSum(absentForm);
				com.bitc.xmis.AskForLeave
						.DateValidAbsentDaysSumActual(absentForm);
			},
			failure : function(form, action) {
				Ext.MessageBox.alert('提示', '载入失败');
			}

		});

	}
	if (step == null || step == '') {
		// 新建
//		Ext.getCmp('back_fieldset').setDisabled(true);
		 if(request.getParameter("todoId") == 'query'){
		 	panel.submit_btn.hide();
		  absentForm.getForm().findField('plan_begin_date').setReadOnly(true) ;
		 absentForm.getForm().findField('plan_end_date').setReadOnly(true) ;
		 absentForm.getForm().findField('plan_begin_hour').setReadOnly(true) ;
		 absentForm.getForm().findField('plan_end_hour').setReadOnly(true) ;
		 absentForm.getForm().findField('absent_type_group').setReadOnly(true) ;
		 absentForm.getForm().findField('reason').setReadOnly(true) ;
		 absentForm.getForm().findField('actual_begin_date').setReadOnly(true) ;
		 absentForm.getForm().findField('actual_end_date').setReadOnly(true) ;
		 absentForm.getForm().findField('actual_begin_hour').setReadOnly(true) ;
		 absentForm.getForm().findField('actual_end_hour').setReadOnly(true) ;
		absentForm.getForm().findField('actual_sum').setReadOnly(true) ;
		absentForm.getForm().findField('plan_sum').setReadOnly(true) ;
		}else{
			panel.submit_btn.show();
			 absentForm.getForm().findField('actual_begin_date').setReadOnly(true) ;
		 absentForm.getForm().findField('actual_end_date').setReadOnly(true) ;
		 absentForm.getForm().findField('actual_begin_hour').setReadOnly(true) ;
		 absentForm.getForm().findField('actual_end_hour').setReadOnly(true) ;
		 absentForm.getForm().findField('actual_sum').setReadOnly(true) ;
		}
							
		
	}else if (step == 'AskForLeave.DeptManagerApproval.Task'
			|| step == 'AskForLeave.ChargeLeaderApproval.Task'
			|| step == 'AskForLeave.MagenagerLeaderApproval.Task' ) {
		// 审批
		panel.agree_btn.show();
		panel.notagree_btn.show();
	} else if ( step == 'AskForLeave.DeptManagerConfirmMRole.Task'
			|| step == 'AskForLeave.DeptManagerConfirmDeptRole.Task'
			|| step == 'AskForLeave.DeptManagerConfirm.Task') {
		// 确认审批
		
		panel.agree_btn.show();
		panel.notagree_btn.show();
	} else if (step == 'AskForLeave.OfficeManagerConfirm.Task') {
		// 确定
		panel.end_btn.show();
	} else if (step == 'AskForLeave.Activity1.Task1') {
		// 退回
		panel.goonsubmit_btn.show();
		panel.forcestop_btn.show();
		panel.back_upload_btn.show();
	} else if (step == 'AskForLeave.ApplicantReportBack.Task') {
		// 销假
		panel.back_submit_btn.show();
		panel.back_upload_btn.show();
		panel.forcestop_btn.show();
//		Ext.getCmp('app_fieldset').setDisabled(true);
		
		 absentForm.getForm().findField('plan_begin_date').setReadOnly(true) ;
		 absentForm.getForm().findField('plan_end_date').setReadOnly(true) ;
		 absentForm.getForm().findField('plan_begin_hour').setReadOnly(true) ;
		 absentForm.getForm().findField('plan_end_hour').setReadOnly(true) ;
		 absentForm.getForm().findField('absent_type_group').setReadOnly(true) ;
		 absentForm.getForm().findField('reason').setReadOnly(true) ;
		 absentForm.getForm().findField('plan_sum').setReadOnly(true) ;
		
	}

	// 日期校验
	com.bitc.xmis.AskForLeave.DateValid(absentForm);
});