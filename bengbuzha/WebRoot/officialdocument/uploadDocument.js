Ext.onReady(function(){
	Ext.QuickTips.init();
	var requestId = request.getParameter('form_id');
	// var documentInfoCombo = new
	// com.bitc.xmis.Combos.DocumentTypeInfoCombo({
	// fieldLabel : '公文类别',
	// width : 150,
	// readOnly : true,
	// allowBlank : false,// 不允许为空
	// hiddenName : 'document_type_id'
	// });
	var attFileGrid = new com.bitc.xmis.AttachmentList({
		title : '相关文件列表',
		width : 300,
		height : 100,
		stripeRows : true,
		REFID : requestId == null ? 0 : requestId,
		attachmentTypeID : com.bitc.xmis.Attachment.document
	});
	var applyRecord = Ext.data.Record.create([ {
		name : 'form_id',
		type : 'string'
	}, {
		name : 'document_type_id',
		type : 'int'
	}, {
		name : 'document_type_name',
		type : 'string'
	}, {
		name : 'priority',
		type : 'int'
	}, {
		name : 'file_no',
		type : 'int'
	}, {
		name : 'summary',
		type : 'string'
	}, {
		name : 'title',
		type : 'string'
	}, {
		name : 'external_name',
		type : 'string'
	}, {
		name : 'document_code',
		type : 'string'
	}, {
		name:'file_flag',
		type:'string'
	},{
		name : 'app_datetime',
		type : 'date',
		dateFormat : 'Y-m-d H:i:s'
	}, {
		name : 'app_user_id',
		type : 'int'
	}, {
		name : 'app_dept_id',
		type : 'int'
	}, {
		name : 'app_user_name',
		type : 'string'
	}, {
		name : 'app_dept_name',
		type : 'string'
	}, {
		name : 'app_dept_leader_id',
		type : 'int'
	}, {
		name : 'app_dept_leader_name',
		type : 'string'
	}, {
		name : 'app_dept_approved_datetime',
		type : 'string'
	}, {
		name : 'app_dept_approved_reason',
		type : 'string'
	}, {
		name : 'app_spec_leader_id',
		type : 'int'
	}, {
		name : 'app_spec_leader_name',
		type : 'string'
	}, {
		name : 'app_spec_approved_datetime',
		type : 'string'
	}, {
		name : 'app_spec_approved_reason',
		type : 'string'
	}, {
		name : 'app_direc_leader_id',
		type : 'int'
	}, {
		name : 'app_direc_leader_name',
		type : 'string'
	}, {
		name : 'app_direc_approved_datetime',
		type : 'string'
	}, {
		name : 'app_direc_approved_reason',
		type : 'string'
	} ]);

	var applyForm = new Ext.form.FormPanel({
		ref : 'form',
		region : 'center',
		padding : 20,
		autoScroll : true,
		border : false,
		width : 800,
		labelWidth : 90,
		labelAlign : 'right',
		reader : new Ext.data.JsonReader({
			root : 'rows'
		}, applyRecord),
		items : [ {
			xtype : 'panel',
			layout : 'column',
			border : false,
			items : [ {
				xytpe : 'panel',
				layout : 'form',
				columnWidth : 0.5,
				border : false,
				items : [ {
					xtype : 'hidden',
					name : 'document_type_id'
				}, {
					xtype : 'hidden',
					name : 'file_no'
				}, {
					xtype : 'textfield',
					fieldLabel : '公文类别',
					width : 150,
					name : 'document_type_name'
				}, {
					xtype : 'textfield',
					fieldLabel : '发送至',
					width : 250,
					name : 'external_name',
					id : 'external_name'
				}, {
					xtype : 'hidden',
					name : 'form_id'
				}, {
					xtype : 'combo',
					name : 'priority',
					fieldLabel : '优先等级',
					displayField : 'text',
					valueField : 'value',
					mode : 'local',
					triggerAction : 'all',
					value : 0,
					allowBlank : false,// 不允许为空
					width : 250,
					store : new Ext.data.ArrayStore({
						fields : [ 'value', 'text' ],
						data : [ [ 1, '紧急' ], [ 0, '普通' ] ]
					})
				}, {
					xtype : 'textfield',
					fieldLabel : '标题',
					allowBlank : false,// 不允许为空
					width : 250,
					name : 'title'
				},/* {
					xtype : 'textfield',
					fieldLabel : '公文编号',
					width : 250,
					name : 'document_code'
				},*/ {
					xtype : 'combo',
					fieldLabel : '公文标志(字)',
					width : 250,
					allowBlank : false,// 不允许为空
						id : 'file_flag',
						name : 'file_flag',
						displayField : 'file_flag',
						valueField : 'file_flag',
						mode : 'remote',
						triggerAction : 'all',
						editable : true,
						store : new Ext.data.Store({
							proxy : new Ext.data.HttpProxy({
										url : __cxtPath
												+ '/applyManageMaint.do?method=getFileFlagList'
									}),
							 autoLoad : true,
							reader : new Ext.data.JsonReader({
										root : 'rows',
									 fields : [{
										name: 'file_flag'
									}]
							})
						}),
						listeners : {
							'expand' : function(){
								this.getStore().reload();
							}
						}
					
				}, {
					xtype : 'textarea',
					fieldLabel : '摘要说明',
					width : 250,
					name : 'summary'
				}, {
					xtype : 'datefield',
					id:'app_datetime',
					name : 'app_datetime',
					fieldLabel : '申请时间',
					allowBlank : false,// 不允许为空
					format:'Y-m-d'
				}, {
					xtype : 'hidden',
					name : 'app_user_id',
					value:__emp.userID
				}, {
					xtype : 'displayfield',
					name : 'app_user_name',
					value:__emp.userName,
					fieldLabel : '申请人'
				}, {
					xtype : 'panel',
					layout : 'form',
					border : false,
					labelAlign : 'rigth',
					width : 300,
					items : [ {
						xtype : 'hidden',
						fieldLabel : '申请部门ID',
						id : 'app_dept_id',
						name : 'app_dept_id',
						value:__emp.departmentID
					}, {
						xtype : 'displayfield',
						fieldLabel : '申请部门',
						name : 'app_dept_name',
						id : 'app_dept_name',
						value:__emp.departmentName
					} ,attFileGrid]

				}]
			}]
		} ]
	});

	var panel = new Ext.Panel(
			{
				region : 'center',
				title : '',
				layout : 'border',
				frame : false,
				border : false,
				autoScroll : true,
				tbar : {
					xtype : 'toolbar',
					items : [
							{
								xtype : 'button',
								text : '保存',
								ref : '../save_btn',
								iconCls : 'ok',
								handler : function() {
									applyForm
											.getForm()
											.submit(
													{
														url : __cxtPath
																+ '/applyManageMaint.do?method=saveDocumentAppFormDetail',
														params : {
															app_user_id : __emp.userID,
															app_datetime : Ext.getCmp('app_datetime').getValue(),
															priority : applyForm
																	.getForm()
																	.findField(
																			'priority')
																	.getValue()
														},
														waitTitle : "请稍候",
														waitMsg : '提交数据，请稍候...',
														success : function(
																response,
																action) {
															Ext.MessageBox
																	.alert(
																			action.result.messageTitle,
																			action.result.message);
															parent.xMis
																	.turn(
																			'doc-upload',
																			'公文上传',
																			__cxtPath
																					+ '/officialdocument/applyDocument.jsp?&form_id='
																					+ action.result.attributes.form_id,
																			'menu-doc-upload',
																			true);
															if (requestId == null) {
																parent.xMis
																		.closeTab(window);
															}

														},
														failure : function(
																response,
																action) {
															com.bitc.xmis.util.AlertUtil
																	.formSubmitFailedAlert(action);
														}
													});
								}
							},
							{
								xtype : 'uploadButton',
								text : '附件上传',
								ref : '../attach_btn',
								reuses : false,
								hidden:true,
								REFIDFun : function() {
									return requestId;
								},
								attachmentTypeID : com.bitc.xmis.Attachment.document,
								uploadPanelConfig : {
									listeners : {
										'uploadcomplete' : function() {
											attFileGrid.reload();
										}
									}
								}
							}]
				},
				items : [ applyForm]
			});
	
	if (requestId != null && requestId != '') {

		applyForm
				.getForm()
				.load(
						{
							url : __cxtPath
									+ '/applyManageMaint.do?method=getDocumentAppFormDetailByFormId',
							params : {
								form_id : requestId
							},
							waitMsg : '正在载入数据...',
							waitTitle : '请等待...',
							success : function(form, action) {
								panel.save_btn.hide();
								panel.attach_btn.show();
							},
							failure : function(form, action) {
								Ext.MessageBox.alert('提示', '载入失败');
							}

						});
	}
	new Ext.Viewport({
		layout : 'border',
		items : [ panel ],
		border : false
	});
});