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
	var MonthPlanForm = com.bitc.xmis.MonthlyManagementWork.AbsentFormPanel;
//	var attFileGrid = new com.bitc.xmis.AttachmentList({
//		title : '相关文件列表',
//		collapsible : true,
//		width : 300,
//		heigth : 400,
//		region : 'east',
//		stripeRows : true,
//		REFID : requestId == null ? 0 : requestId,
//		viewConfig : {
//			emptyText : '计划中需要的文件'
//		},
//		attachmentTypeID : com.bitc.xmis.Attachment.MonthPlanForm
//	});

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

					if (MonthPlanForm.getForm().isValid()) {

						Ext.MessageBox.confirm("提示", "是否提交此条信息？",
								function(btn) {
									if (btn == 'yes') {
										MonthPlanForm.getForm().submit({
											url : __cxtPath
													+ '/applyManageMaint.do?method=doStartMonthPlan',
											params : {
												form_id : requestId,
												type_id: 2
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
				ref : '../earlyagree_btn',
				iconCls : 'add',
				handler : function() {//参数说明  第一个参数 表示 是否同意0表示同意，1表示不同意，第二个参数表示 那个步骤审批，0表示月初审批，1表示月末审批
								com.bitc.xmis.workflow.DoTaskIsOrNotAgree(0,0,workItemId,requestId,true);
						   }
			}, {
				xtype : 'button',
				text : '不同意',
				hidden : true,
				ref : '../earlynotagree_btn',
				iconCls : 'add',
				handler : function() {
								com.bitc.xmis.workflow.DoTaskIsOrNotAgree(1,0, workItemId,requestId,true);
				}

			},  {
				xtype : 'button',
				text : '同意',
				hidden : true,
				ref : '../endagree_btn',
				iconCls : 'add',
				handler : function() {
								com.bitc.xmis.workflow.DoTaskIsOrNotAgree(0,1,workItemId,requestId,true);
						   }
			}, {
				xtype : 'button',
				text : '不同意',
				hidden : true,
				ref : '../endnotagree_btn',
				iconCls : 'add',
				handler : function() {
								com.bitc.xmis.workflow.DoTaskIsOrNotAgree(1,1, workItemId,requestId,true);
				}

			},{
				xtype : 'button',
				text : '修改后提交',
				hidden : true,
				ref : '../goonsubmit_btn',
				iconCls : 'add',
				handler : function() {
					if (MonthPlanForm.getForm().isValid()) {

						Ext.MessageBox.confirm("提示", "是否提交此条信息？",
								function(btn) {
									if (btn == 'yes') {
										MonthPlanForm.getForm().submit({
											url : __cxtPath
													+ '/applyManageMaint.do?method=doStartMonthPlan',
											params : {
												workItemId : workItemId,
												processInstanceId : processInstanceId,
												form_id : requestId,
												type_id: 2
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
				text : '提交',
				hidden : true,
				ref : '../back_submit_btn',
				iconCls : 'add',
				handler : function() {

					if (MonthPlanForm.getForm().isValid()) {

						Ext.MessageBox.confirm("提示", "是否提交此条信息？",
								function(btn) {
									if (btn == 'yes') {
										MonthPlanForm.getForm().submit({
											url : __cxtPath
													+ '/applyManageMaint.do?method=doStartEndOfMonthPlan',
											params : {
												workItemId : workItemId,
												processInstanceId : processInstanceId,
												form_id : requestId,
												type_id: 2
											},
											scope : this,
											waitTitle : "请稍候",
											waitMsg : '提交数据，请稍候...',
											success : function(response, action) {
												Ext.MessageBox
														.alert(
																action.result.messageTitle,
																action.result.message);
												panel.back_submit_btn
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

			}]
		},
		items : [MonthPlanForm/*, attFileGrid*/, approvalInfoGridPanel]
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
		MonthPlanForm.getForm().findField('app_dept_id').setValue(__emp.departmentID);
		MonthPlanForm.getForm().findField('app_dept_name').setValue(__emp.departmentName);
		MonthPlanForm.getForm().findField('app_user_id').setValue(__emp.userID);
		MonthPlanForm.getForm().findField('app_user_name').setValue(__emp.userName);
		MonthPlanForm.getForm().findField('app_date').setValue(new Date().format('Y-m-d'));

	} else {
		MonthPlanForm.getForm().load({
			url : __cxtPath
					+ '/applyManageMaint.do?method=getMonthPlanByFormId',
			params : {
				form_id : requestId
			},
			waitMsg : '正在载入数据...',
			waitTitle : '请等待...',
			success : function(form, action) {
			
			},
			failure : function(form, action) {
				Ext.MessageBox.alert('提示', '载入失败');
			}

		});

	}
	if (step == null || step == '') {
		
		// 新建
		 if(request.getParameter("todoId") == 'query'){
		 panel.submit_btn.hide();
		MonthPlanForm.getForm().findField('excuted_assigned_date').hide();
		MonthPlanForm.getForm().findField('excuted_assigned_name').hide();
		MonthPlanForm.getForm().findField('plan_assigned_name').hide();
		MonthPlanForm.getForm().findField('plan_assigned_date').hide();
		
		MonthPlanForm.getForm().findField('work_month').setReadOnly(true);
		MonthPlanForm.getForm().findField('plan_comments').setReadOnly(true);
		
		MonthPlanForm.getForm().findField('unfinished_comments').setReadOnly(true);
		MonthPlanForm.getForm().findField('excuted_comments').setReadOnly(true);
		}else{
			panel.submit_btn.show();
			MonthPlanForm.getForm().findField('excuted_assigned_date').hide();
			MonthPlanForm.getForm().findField('excuted_assigned_name').hide();
			MonthPlanForm.getForm().findField('plan_assigned_name').hide();
			MonthPlanForm.getForm().findField('plan_assigned_date').hide();
			MonthPlanForm.getForm().findField('unfinished_comments').setReadOnly(true);
			MonthPlanForm.getForm().findField('excuted_comments').setReadOnly(true);
		}
	} else if (step == 'MonthlyManagementWork.Activity2.Task2') {
		// 月初审批
		panel.earlyagree_btn.show();
		panel.earlynotagree_btn.show();
		MonthPlanForm.getForm().findField('excuted_assigned_date').hide();
		MonthPlanForm.getForm().findField('excuted_assigned_name').hide();
		MonthPlanForm.getForm().findField('plan_assigned_name').hide();
		MonthPlanForm.getForm().findField('plan_assigned_date').hide();
		
		MonthPlanForm.getForm().findField('work_month').setReadOnly(true);
		MonthPlanForm.getForm().findField('plan_comments').setReadOnly(true);
		
		MonthPlanForm.getForm().findField('unfinished_comments').setReadOnly(true);
		MonthPlanForm.getForm().findField('excuted_comments').setReadOnly(true);
	}else if(step == 'MonthlyManagementWork.Activity4.Task4'){
		//月末审批
		panel.endagree_btn.show();
		panel.endnotagree_btn.show();
		MonthPlanForm.getForm().findField('excuted_assigned_date').hide();
		MonthPlanForm.getForm().findField('excuted_assigned_name').hide();
		
		MonthPlanForm.getForm().findField('work_month').setReadOnly(true);
		MonthPlanForm.getForm().findField('plan_comments').setReadOnly(true);
		
		MonthPlanForm.getForm().findField('excuted_comments').setReadOnly(true);
		MonthPlanForm.getForm().findField('unfinished_comments').setReadOnly(true);
	}else if (step == 'MonthlyManagementWork.Activity1.Task1') {
		// 月初退回
		panel.goonsubmit_btn.show();
		MonthPlanForm.getForm().findField('excuted_assigned_date').hide();
		MonthPlanForm.getForm().findField('excuted_assigned_name').hide();
		
		MonthPlanForm.getForm().findField('unfinished_comments').setReadOnly(true);
		MonthPlanForm.getForm().findField('excuted_comments').setReadOnly(true);
	} else if (step == 'MonthlyManagementWork.Activity3.Task3') {
		// 月末
		panel.back_submit_btn.show();
		MonthPlanForm.getForm().findField('work_month').setReadOnly(true);
		MonthPlanForm.getForm().findField('plan_comments').setReadOnly(true);
	}
});