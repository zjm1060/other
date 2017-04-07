Ext
		.onReady(function() {
			Ext.QuickTips.init();
			var type = request.getParameter('type');
			var continueEdit = request.getParameter('continueEdit');
			var requestId = request.getParameter('form_id');
			var step = request.getParameter('step');
			var workItemId = request.getParameter('workItemId');
			var processInstanceId = request.getParameter('processInstanceId');
			var HQ_isAgree = request.getParameter('isAgree');
			var approvalInfoGridPanel = new com.bitc.xmis.workflow.approvalInfoList(
					{

						collapsible : true,
						// collapsed : true,
						region : 'south',
						width : 700,
						height : 150,
						title : '审批记录',
						autoLoadData : true,
						autoScroll : true,
						processInstanceId : processInstanceId
					});
			var attFileGrid = new com.bitc.xmis.AttachmentList({
				title : '相关文件列表',
				width : 300,
				height : 100,
				stripeRows : true,
				REFID : requestId == null ? 0 : requestId,
				attachmentTypeID : com.bitc.xmis.Attachment.document
			});
			var draft = new com.bitc.xmis.sys.Draft({
				model : 'doc',
				appId : 'step1-',
				userID : __emp.userID,
				text : '【公文编辑】' + (type == 1 ? '下行文办理' : '上行文办理') + ' 创建时间'
						+ new Date().format('m-d H:i'),
				url : String
						.format('/officialdocument/applyDocument.jsp?&type='
								+ type)
			});
			// var documentInfoCombo = new
			// com.bitc.xmis.Combos.DocumentTypeInfoCombo({
			// fieldLabel : '公文类别',
			// width : 150,
			// readOnly : true,
			// allowBlank : false,// 不允许为空
			// hiddenName : 'document_type_id'
			// });

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
				name : 'file_flag',
				type : 'string'
			}, {
				name : 'app_datetime',
				type : 'string'
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
							id : 'document_type_id',
							name : 'document_type_id'
						}, {
							xtype : 'hidden',
							id : 'HQ_name'
						}, {
							xtype : 'displayfield',
							fieldLabel : '公文类别',
							width : 150,
							id : 'document_type_name',
							name : 'document_type_name'
						}, {
							xtype : 'textfield',
							fieldLabel : '发送机关',
							width : 250,
							name : 'external_name',
							allowBlank : true,// 不允许为空
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
						}, /*{
							xtype : 'textfield',
							fieldLabel : '文件标题',
							allowBlank : false,// 不允许为空
							width : 250,
							name : 'title'
						},
							 * { xtype : 'textfield', fieldLabel : '公文编号', width :
							 * 250, name : 'document_code' },
							 */{

							xtype : 'combo',
							name : 'file_flag',
							fieldLabel : '发文标志（字）',
							id : 'file_flag',
							displayField : 'file_flag',
							valueField : 'file_flag',
							mode : 'local',
							value : '蚌埠闸处',
							triggerAction : 'all',
							allowBlank : false,// 不允许为空
							width : 250,
							store : new Ext.data.ArrayStore({
								fields : [ 'file_flag' ],
								data : [ [ '蚌埠闸处' ], [ '蚌埠闸党' ] ]
							})

						}, /*{
							xtype : 'numberfield',
							fieldLabel : '公文编号',
							allowDecimals:false,               //不允许输入小数   
				            nanText:'请输入有效整数',           //无效数字提示   
				            allowNegative:false,                //不允许输入负数   
							width : 250,
							name : 'file_no'
						}, */{
							xtype : 'textarea',
							fieldLabel : '事由或文件',
							allowBlank : false,// 不允许为空
							width : 250,
							name : 'summary'
						}, {
							xtype : 'displayfield',
							name : 'app_datetime',
							fieldLabel : '申请时间',
							allowBlank : false,// 不允许为空
							blankText : '该输入项为必输项'
						}, {
							xtype : 'hidden',
							name : 'app_user_id'
						}, {
							xtype : 'displayfield',
							name : 'app_user_name',
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
								name : 'app_dept_id'
							}, {
								xtype : 'displayfield',
								fieldLabel : '申请部门',
								name : 'app_dept_name',
								id : 'app_dept_name'
							} ]

						}, attFileGrid ]
					}, {
						xytpe : 'panel',
						layout : 'form',
						columnWidth : 0.5,
						border : false,
						items : [ {
							xtype : 'panel',
							border : false,
							layout : 'hbox',
							fieldLabel : '主管领导意见',
							items : [ {
								xtype : 'hidden',
								name : 'app_direc_leader_id'
							}, {
								xtype : 'displayfield',
								name : 'app_direc_approved_reason',
								width : 30,
								ref : 'app_direc_approved_reason',
								height : 22
							}, {
								xtype : 'displayfield',
								width : 10,
								value : ''
							}, {
								xtype : 'displayfield',
								width : 40,
								name : 'app_direc_leader_name'
							}, {
								xtype : 'displayfield',
								name : 'app_direc_approved_datetime'
							}

							]
						}, {
							xtype : 'panel',
							border : false,
							layout : 'hbox',
							fieldLabel : '分管领导意见',
							items : [ {
								xtype : 'hidden',
								name : 'app_spec_leader_id'
							}, {
								xtype : 'displayfield',
								name : 'app_spec_approved_reason',
								width : 30,
								ref : 'app_spec_approved_reason',
								height : 22
							}, {
								xtype : 'displayfield',
								width : 10,
								value : ''
							}, {
								xtype : 'displayfield',
								width : 40,
								name : 'app_spec_leader_name'
							}, {
								xtype : 'displayfield',
								name : 'app_spec_approved_datetime'
							}

							]
						}, {
							xtype : 'panel',
							border : false,
							hidden : true,
							layout : 'hbox',
							fieldLabel : '部门领导意见',
							items : [ {
								xtype : 'hidden',
								name : 'app_dept_leader_id'
							}, {
								xtype : 'displayfield',
								name : 'app_dept_approved_reason',
								ref : 'app_dept_approved_reason',
								width : 30,
								height : 22
							}, {
								xtype : 'displayfield',
								width : 10
							}, {
								xtype : 'displayfield',
								width : 40,
								name : 'app_dept_leader_name'
							}, {
								xtype : 'displayfield',
								name : 'app_dept_approved_datetime'
							}

							]
						} ]
					} ]
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
										text : '保存草稿',
										ref : '../saveDraft',
										hidden : true,
										iconCls : 'save',
										handler : function() {
											var content = {
												form : applyForm.getForm()
														.getValues(),
												grid : com.bitc.xmis.util.ExtUtil
														.getStoreData(attFileGrid.store)

											};

											draft
													.save(
															function(d, result) {
																if (parent
																		&& parent.xMis) {
																	parent.xMis
																			.refresh('mydraft');
																}
																Ext.Msg
																		.alert(
																				'提示',
																				'保存草稿成功！<br/>你可以在首页的草稿箱中找到。');

															}, applyForm,
															content);
										}
									},
									{
										xtype : 'button',
										text : '保存',
										hidden : true,
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
																	app_datetime : Ext.util.Format
																			.date(
																					new Date(),
																					'Y-m-d H:i:s'),
																	// form_id :
																	// requestId,
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
																	draft
																			.remove(function() {
																				if (parent
																						&& parent.xMis) {
																					parent.xMis
																							.refresh('mydraft');
																				}
																				parent.xMis
																						.closeTab(window);
																			});
																	parent.xMis
																			.turn(
																					'doc-info-'
																							+ action.result.attributes.form_id,
																					'公文编辑',
																					__cxtPath
																							+ '/officialdocument/applyDocument.jsp?&form_id='
																							+ action.result.attributes.form_id
																							+ '&type='
																							+ type,
																					'menu-doc-info',
																					true);
																	if (requestId == null
																			&& draftId == null) {
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
										xtype : 'button',
										text : '正文',
										ref : '../finish_btn',
										hidden : true,
										iconCls : 'ok',
										handler : function() {
											panel.finish_btn.hide();
											panel.look_btn.show();
											panel.show_btn.show();
											var url = __cxtPath
													+ '/officialdocument/CreateWord1.jsp?form_id='
													+ requestId
													+ "&attachmentTypeID=18";
											parent.xMis.turn('zw-doc-info',
													'公文正文', url, 't', true);
										}
									},
									{
										xtype : 'button',
										text : '查看正文',
										ref : '../look_btn',
										hidden : true,
										iconCls : 'ok',
										handler : function() {
											var url = __cxtPath
													+ '/officialdocument/OpenWord.jsp?form_id='
													+ requestId;
											parent.xMis.turn('zw-doc-info2',
													'公文正文', url, 't', true);
										}
									},
									{
										xtype : 'uploadButton',
										text : '附件上传',
										ref : '../attach_btn',
										hidden : true,
										reuses : false,
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
									},
									{
										xtype : 'button',
										text : '同意',
										hidden : true,
										ref : '../agree_btn',
										id : 'agree',
										iconCls : 'add',
										handler : function() {
											com.bitc.xmis.workflow
													.DoApprovalTaskIsAgreeWithFormInfo(
															0, workItemId,
															true, step,
															requestId);

										}
									},
									{
										xtype : 'button',
										text : '不同意',
										hidden : true,
										ref : '../notagree_btn',
										iconCls : 'add',
										handler : function() {
											com.bitc.xmis.workflow
													.DoApprovalTaskIsAgreeWithFormInfo(
															1, workItemId,
															true, step,
															requestId);

										}

									},
									{
										xtype : 'button',
										text : '成文',
										hidden : true,
										ref : '../XWcreat_btn',
										iconCls : 'add',
										handler : function() {

											panel.XWcreat_btn.hide();
											panel.XWshow_btn.show();
											panel.XWdown_btn.show();
											var attFile = "";
											for ( var i = 0; i < attFileGrid.store.data.length; i++) {
												attFile = attFile
														+ ' ('
														+ (i + 1)
														+ ')'
														+ attFileGrid.store
																.getAt(i)
																.get(
																		DBSchema.Attachment.AttachmentName);
											}
											var url = __cxtPath
													+ '/officialdocument/creatXW.jsp?'
													+ 'form_id='
													+ requestId
													+ '&title='
													+ applyForm.getForm()
															.findField('summary')
															.getValue()
													+ '&app_direc_leader_name='
													+ applyForm
															.getForm()
															.findField(
																	'app_direc_leader_name')
															.getValue()
													+ '&approved_datetime='
													+ applyForm
															.getForm()
															.findField(
																	'app_direc_approved_datetime')
															.getValue()
													+ '&attFile='
													+ attFile
													+ '&external_name='
													+ applyForm.getForm()
															.findField(
																	'external_name')
															.getValue()
													+ '&file_flag='
													+ Ext.getCmp('file_flag')
															.getValue()
													+ '&document_type_id='
													+ Ext.getCmp('document_type_id').getValue();
											window
													.open(
															url,
															'合成正文',
															'height=300, width=500, top=200, left=400, toolbar=no, menubar=no, scrollbars=no, resizable=no, location=no, status=no');
										}

									},
									{
										xtype : 'button',
										text : '行文预览',
										hidden : true,
										ref : '../XWshow_btn',
										iconCls : 'add',
										handler : function() {
											var url = __cxtPath
													+ '/officialdocument/OpenWord.jsp?form_id='
													+ requestId + '&zw=1';
											parent.xMis.turn('XWshow-doc',
													'行文预览', url, 't', true);
										}
									},
									{
										xtype : 'button',
										text : '行文下载',
										hidden : true,
										ref : '../XWdown_btn',
										iconCls : 'add',
										handler : function() {
											var url = __cxtPath
													+ '/fileUpload.do?type=11'
													+ '&form_id=' + requestId
													+ '&fileName=行文';
											location.href = url;
										}

									},
									{
										xtype : 'button',
										text : '拟稿预览',
										ref : '../show_btn',
										iconCls : 'add',
										handler : function() {
											var attFile = "";
											for ( var i = 0; i < attFileGrid.store.data.length; i++) {
												attFile = attFile
														+ ' ('
														+ (i + 1)
														+ ')'
														+ attFileGrid.store
																.getAt(i)
																.get(
																		DBSchema.Attachment.AttachmentName);
											}
											var url = __cxtPath
													+ '/officialdocument/showDocument.jsp?'
													+ 'form_id='
													+ requestId
													+ '&app_user_name='
													+ applyForm
															.getForm()
															.findField(
																	'app_user_name')
															.getValue()
													+ '&app_datetime='
													+ applyForm
															.getForm()
															.findField(
																	'app_datetime')
															.getValue()
													+ '&app_dept_approved_datetime='
													+ applyForm
															.getForm()
															.findField(
																	'app_dept_approved_datetime')
															.getValue()
													+ '&app_spec_approved_datetime='
													+ applyForm
															.getForm()
															.findField(
																	'app_spec_approved_datetime')
															.getValue()
													+ '&app_direc_approved_datetime='
													+ applyForm
															.getForm()
															.findField(
																	'app_direc_approved_datetime')
															.getValue()
													+ '&app_dept_leader_name='
													+ applyForm
															.getForm()
															.findField(
																	'app_dept_leader_name')
															.getValue()
													+ '&app_spec_leader_name='
													+ applyForm
															.getForm()
															.findField(
																	'app_spec_leader_name')
															.getValue()
													+ '&app_direc_leader_name='
													+ applyForm
															.getForm()
															.findField(
																	'app_direc_leader_name')
															.getValue()
													+ '&summary='
													+ applyForm.getForm()
															.findField(
																	'summary')
															.getValue()
													+ '&external_name='
													+ applyForm
															.getForm()
															.findField(
																	'external_name')
															.getValue()
													+ '&attFile=' + attFile
													+'&HQ_name='
													+Ext.getCmp('HQ_name').getValue();

											parent.xMis.turn(
													'showDocument-doc', '拟稿预览',
													url, 't', true);
										}

									},
									{
										xtype : 'button',
										text : '同意会签',
										hidden : true,
										ref : '../huiqian_agree_btn',
										id : 'huiq_agree',
										iconCls : 'add',
										handler : function() {
											var url = '/applyManageMaint.do?method=doupdateHuiQianMessage';
											Ext.Ajax
													.request({
														url : __cxtPath + url,
														method : 'post',
														params : {
															type : type,
															isAgree : 'true',
															formId : requestId
														},
														success : function(
																form, action) {
															panel.huiqian_agree_btn
																	.hide();
															panel.huiqian_notagree_btn
																	.hide();
															Ext.Msg.alert('提示',
																	'会签成功！',callBack);
															function callBack(){
																parent.xMis.closeTab(window);
															}
															// parent.xMis.closeTab(window);
														},
														failure : function() {
														}
													});
										}

									},
									{
										xtype : 'button',
										text : '不同意会签',
										ref : '../huiqian_notagree_btn',
										id : 'huiqian_notagree',
										hidden : true,
										iconCls : 'add',
										handler : function() {
											panel.huiqian_agree_btn.hide();
											panel.huiqian_notagree_btn.hide();
											Ext.Msg.alert('提示',
													'会签成功！',callBack);
											function callBack(){
												parent.xMis.closeTab(window);
											}
										}
									},
									{
										xtype : 'button',
										text : '是否需要会签',
										ref : '../is_huiqian',
										iconCls : 'add',
										hidden : true,
										handler : function() {
											win.show();
										}

									},
									{
										xtype : 'button',
										text : '提交',
										hidden : true,
										ref : '../submit_btn',
										iconCls : 'save',
										handler : function() {
											if (applyForm.getForm().isValid()) {
												Ext.MessageBox
														.confirm(
																"提示",
																"是否提交此条信息？",
																function(btn) {
																	if (btn == 'yes') {

																		Ext.Ajax
																				.request({
																					url : __cxtPath
																							+ '/applyManageMaint.do?method=doStartApplyDocumentAppFormDetail',
																					method : 'post',
																					params : {
																						form_id : requestId,
																						type : type
																					},
																					success : function(
																							response,
																							action) {

																						var result = Ext
																								.decode(response.responseText);
																						if (result.success) {
																							Ext.Msg
																									.alert(
																											result.messageTitle,
																											result.message);
																							draft
																									.remove(function() {
																										if (parent
																												&& parent.xMis) {
																											parent.xMis
																													.refresh('mydraft');
																										}
																									});
																							parent.xMis
																									.refresh(
																											'mytodo',
																											'myworkitemnewtodo');

																						} else {
																							Ext.Msg
																									.alert(
																											result.messageTitle,
																											result.message);
																						}
																						panel.tbar
																								.hide();

																					},
																					failure : function(
																							response,
																							action) {
																						var result = Ext
																								.decode(response.responseText);
																						Ext.MessageBox
																								.hide();
																						Ext.Msg
																								.alert(
																										result.messageTitle,
																										result.message);
																					}
																				});

																	}
																});
											}
										}
									},
									{
										xtype : 'button',
										text : '修改后提交',
										hidden : true,
										ref : '../goonsubmit_btn',
										iconCls : 'add',
										handler : function() {
											if (attFileGrid.store.data.length < 1) {
												Ext.Msg.alert('提示',
														'附件列表不允许为空！');
												return;
											} else {
												if (applyForm.getForm()
														.isValid()) {
													Ext.MessageBox
															.confirm(
																	"提示",
																	"是否提交此条信息？",
																	function(
																			btn) {
																		if (btn == 'yes') {

																			applyForm
																					.getForm()
																					.submit(
																							{
																								url : __cxtPath
																										+ '/applyManageMaint.do?method=doSubmitAfterUpdateApplyDocumentAppFormDetail',
																								params : {
																									form_id : requestId,
																									priority : applyForm
																											.getForm()
																											.findField(
																													'priority')
																											.getValue(),
																									app_user_id : applyForm
																											.getForm()
																											.findField(
																													'app_user_id')
																											.getValue(),
																									app_dept_id : applyForm
																											.getForm()
																											.findField(
																													'app_dept_id')
																											.getValue(),
																									app_datetime : applyForm
																											.getForm()
																											.findField(
																													'app_datetime')
																											.getValue(),
																									app_dept_leader_id : applyForm
																											.getForm()
																											.findField(
																													'app_dept_leader_id')
																											.getValue(),
																									app_dept_approved_reason : applyForm
																											.getForm()
																											.findField(
																													'app_dept_approved_reason')
																											.getValue(),
																									app_dept_approved_datetime : applyForm
																											.getForm()
																											.findField(
																													'app_dept_approved_datetime')
																											.getValue(),

																									app_spec_leader_id : applyForm
																											.getForm()
																											.findField(
																													'app_spec_leader_id')
																											.getValue(),
																									app_spec_approved_reason : applyForm
																											.getForm()
																											.findField(
																													'app_spec_approved_reason')
																											.getValue(),
																									app_spec_approved_datetime : applyForm
																											.getForm()
																											.findField(
																													'app_spec_approved_datetime')
																											.getValue(),

																									app_direc_leader_id : applyForm
																											.getForm()
																											.findField(
																													'app_direc_leader_id')
																											.getValue(),
																									app_direc_approved_reason : applyForm
																											.getForm()
																											.findField(
																													'app_direc_approved_reason')
																											.getValue(),
																									app_direc_approved_datetime : applyForm
																											.getForm()
																											.findField(
																													'app_direc_approved_datetime')
																											.getValue(),

																									workItemId : workItemId
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
																											.refresh(
																													'mytodo',
																													'myworkitemnewtodo');
																								},
																								failure : function(
																										response,
																										action) {
																									com.bitc.xmis.util.AlertUtil
																											.formSubmitFailedAlert(action);
																								}
																							});

																		}
																	});
												}
											}
										}
									},
									{
										xtype : 'button',
										text : '强制结束',
										hidden : true,
										ref : '../forcestop_btn',
										iconCls : 'add',
										handler : function() {
											var url = '/applyManageMaint.do?method=doEndApply';
											Ext.MessageBox
													.confirm(
															"提示",
															"是否确定结束此流程？",
															function(btn) {
																if (btn == 'yes') {
																	Ext.Ajax
																			.request({
																				url : __cxtPath
																						+ url,
																				method : 'post',
																				params : {
																					workItemId : workItemId,
																					forceStop : true,
																					processInstanceId : processInstanceId
																				},
																				success : function(
																						response,
																						action) {

																					var result = Ext
																							.decode(response.responseText);
																					if (result.success) {
																						Ext.Msg
																								.alert(
																										result.messageTitle,
																										result.message);
																						parent.xMis
																								.refresh(
																										'mytodo',
																										'myworkitemnewtodo');

																					} else {
																						Ext.Msg
																								.alert(
																										result.messageTitle,
																										result.message);
																					}
																					panel.tbar
																							.hide();
																				},
																				failure : function(
																						response,
																						action) {
																					var result = Ext
																							.decode(response.responseText);
																					Ext.MessageBox
																							.hide();
																					Ext.Msg
																							.alert(
																									result.messageTitle,
																									result.message);
																				}
																			});

																}
															});
										}
									},
									{
										xtype : 'button',
										text : '任务确定',
										hidden : true,
										ref : '../end_btn',
										iconCls : 'add',
										handler : function() {
											var url = '/applyManageMaint.do?method=doEndApply';
											Ext.MessageBox
													.confirm(
															"提示",
															"是否确定结束此流程？",
															function(btn) {
																if (btn == 'yes') {
																	Ext.Ajax
																			.request({
																				url : __cxtPath
																						+ url,
																				method : 'post',
																				params : {
																					workItemId : workItemId,
																					processInstanceId : processInstanceId,
																					form_id : requestId
																				},
																				success : function(
																						response,
																						action) {

																					var result = Ext
																							.decode(response.responseText);
																					if (result.success) {
																						Ext.Msg
																								.alert(
																										result.messageTitle,
																										result.message);
																						parent.xMis
																								.refresh(
																										'mytodo',
																										'myworkitemnewtodo');

																					} else {
																						Ext.Msg
																								.alert(
																										result.messageTitle,
																										result.message);
																					}
																					panel.tbar
																							.hide();

																				},
																				failure : function(
																						response,
																						action) {
																					var result = Ext
																							.decode(response.responseText);
																					Ext.MessageBox
																							.hide();
																					Ext.Msg
																							.alert(
																									result.messageTitle,
																									result.message);
																				}
																			});

																}
															});
										}
									} ]
						},
						items : [ applyForm, approvalInfoGridPanel ]
					});

			var reRecord = new Ext.data.Record.create([ {
				name : 'getDeptManager_id',
				mapping : 'getDeptManager_id'
			}, {
				name : 'getDeptManagerDept_id',
				mapping : 'getDeptManagerDept_id'
			}, {
				name : 'getDeptManagerDept_name',
				mapping : 'getDeptManagerDept_name'
			}, {
				name : 'getDeptManager_name',
				mapping : 'getDeptManager_name'
			} ]);
			var sm = new Ext.grid.CheckboxSelectionModel({
				header : '',
				checkOnly : true
			});
			var cm = new Ext.grid.ColumnModel([ sm, {
				header : '会签人ID',
				dataIndex : 'getDeptManager_id',
				sortable : false,
				hidden : true
			}, {
				header : '部门ID',
				dataIndex : 'getDeptManagerDept_id',
				sortable : false,
				hidden : true
			}, {
				header : '部门',
				width : 150,
				align : 'center',
				dataIndex : 'getDeptManagerDept_name',
				sortable : false
			}, {
				header : '会签人',
				width : 150,
				align : 'center',
				dataIndex : 'getDeptManager_name',
				sortable : false
			} ]);
			var store = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
					url : __cxtPath + '/orgMaint.do?method=getDeptManagerList',
					method : "POST"
				}),
				reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, reRecord),
				baseParams : {},
				remoteSort : true,
				autoLoad : true
			});
			var win = new Ext.Window(
					{
						id : 'win',
						title : '选择会签人',
						layout : 'border',
						width : 400,
						height : 400,
						// modal : true,
						border : false,
						closable : true,
						closeAction : 'hide',

						bbar : [
								{
									text : '确定',
									align : 'center',
									handler : function() {
										var array = Ext.getCmp('huiqianGrid')
												.getSelectionModel()
												.getSelections();
										var indexStr = "";
										if (array.length < 1) {
											Ext.Msg.alert('提示', '请选择会签人');
										} else {
											for ( var i = 0; i < array.length; i++) {
												indexStr += array[i].data.getDeptManager_id
														+ ",";
											}
											var url = '/applyManageMaint.do?method=dosendHuiQianMessage';
											Ext.Ajax
													.request({
														url : __cxtPath + url,
														method : 'post',
														params : {
															type : type,
															indexStr : indexStr,
															formId : requestId
														},
														success : function(
																form, action) {
															panel.submit_btn
																	.hide();
															win.hide();
															Ext.Msg.alert('提示',
																	'会签信息发送成功！！',callBack);
															function callBack(){
																parent.xMis.closeTab(window);
															}
															// parent.xMis.closeTab(window);
														},
														failure : function() {
														}
													});
										}
									}
								}, {
									text : '取消',
									align : 'center',
									handler : function() {
										win.hide();
									}
								} ],

						items : [ new Ext.Panel({
							region : 'center',
							layout : 'border',
							items : [ {
								xtype : 'grid',
								id : 'huiqianGrid',
								border : false,
								// labelAlign : 'left',
								// labelWidth : 80,
								// frame : true,
								width : 300,
								border : false,
								autoScroll : true,
								loadMask : {
									msg : '正在加载数据，请稍侯……'
								},
								stripeRows : true,
								// bodyStyle : 'padding:10px',
								// buttonAlign : 'center',
								region : 'center',
								store : store,
								cm : cm,
								sm : sm

							} ]
						})
						/*  */]
					});

			if (request.getParameter("draftId") != null) {
				var draftId = request.getParameter("draftId");
				draft.id = draftId;
				draft.load(function(d) {

					var c = d.content;

					applyForm.getForm().setValues(c.form);
					requestId = applyForm.form.findField('form_id').getValue();
					if (c.form.priority == '普通') {
						applyForm.form.findField('priority').setValue(0);
					} else {
						applyForm.form.findField('priority').setValue(1);
					}
					var records = [];
					if (c.grid.length > 0) {
						panel.attach_btn.show();
						panel.submit_btn.show();
						panel.save_btn.hide();
						panel.finish_btn.show();
					} else {
						panel.submit_btn.hide();
						panel.save_btn.show();
					}

					Ext.iterate(c.grid, function(item) {
						records.push(new attFileGrid.store.recordType(item));
					}, this);
					attFileGrid.store.add(records);
				}, this);

			}
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
										if(Ext.getCmp('document_type_id').getValue() == 1){
											Ext.getCmp('external_name').hide();
											if (step == 'ApplyDocumentCirculation.SuppliesManagerConfirm.Task') {
												panel.XWcreat_btn.show();
											}
										}
//										if(Ext.getCmp('document_type_id').getValue() == 3){
//											Ext.getCmp('external_name').hide();
//											if (step == 'ApplyDocumentCirculation.SuppliesManagerConfirm.Task') {
//												panel.XWshow_btn.show();
//											}
//										}
										Ext.Ajax.request({
											url : __cxtPath  + '/applyManageMaint.do?method=getHQ_nameByFormId',
											method : 'post',
											params : {
												type : type,
												//indexStr : indexStr,
												formId : requestId
											},
											success : function(response, action) {
												var result = Ext
												.decode(response.responseText);
												Ext.getCmp('HQ_name').setValue(result.attributes.HQ_name);
												//Ext.Msg.alert(result.attributes.HQ_name);
											},
											failure : function() {
											}
										});
									},
									failure : function(form, action) {
										Ext.MessageBox.alert('提示', '载入失败');
									}

								});
			} else {
				applyForm.getForm().findField('app_user_id').setValue(
						__emp.userID);
				applyForm.getForm().findField('app_user_name').setValue(
						__emp.userName);
				applyForm.getForm().findField('app_dept_id').setValue(
						__emp.departmentID);
				applyForm.getForm().findField('app_dept_name').setValue(
						__emp.departmentName);
				applyForm.getForm().findField('app_datetime').setValue(
						Ext.util.Format.date(new Date(), 'Y-m-d H:i:s'));
			}

			if (step == null || step == '') {
				// 新建
				panel.saveDraft.show();
				if (requestId == null || requestId == '') {
					panel.show_btn.hide();
					panel.save_btn.show();
				} else {
					panel.show_btn.hide();
					panel.finish_btn.show();
					panel.attach_btn.show();
					panel.submit_btn.show();
					panel.is_huiqian.show();
					if (HQ_isAgree != null && HQ_isAgree != '') {
						panel.look_btn.show();
						panel.show_btn.show();
						panel.finish_btn.hide();
						panel.submit_btn.hide();
						panel.attach_btn.hide();
						panel.huiqian_agree_btn.show();
						panel.huiqian_notagree_btn.show();
						panel.saveDraft.hide();
						panel.is_huiqian.hide();
					}
					if (continueEdit != null && continueEdit != '') {
						panel.look_btn.show();
						panel.show_btn.show();
						panel.finish_btn.hide();
						panel.saveDraft.hide();
					}
				}
				if (type == 1) {
					applyForm.getForm().findField('document_type_id').setValue(
							1);
					applyForm.getForm().findField('document_type_name')
							.setValue("下行文办理");
					Ext.getCmp('external_name').hide();
				} else if (type == 3) {
					applyForm.getForm().findField('document_type_id').setValue(
							3);
					applyForm.getForm().findField('document_type_name')
							.setValue("上行文办理");
				}
				
			} else if (step == 'ApplyDocumentCirculation.DeptManagerApproval.Task'
					|| step == 'ApplyDocumentCirculation.ChargeLeaderApproval.Task'
					|| step == 'ApplyDocumentCirculation.OfficeDirectorApproval.Task') {
				// 审批
				panel.agree_btn.show();
				panel.notagree_btn.show();
				panel.show_btn.show();
			} else if (step == 'ApplyDocumentCirculation.Activity7.Task') {
				// 上下文处理人 确定
				//Ext.getCmp("agree").setText("任务确认");
				//Ext.getCmp("agree").show();
				panel.end_btn.show();
				panel.show_btn.show();
				panel.XWcreat_btn.show();
				// panel.XWcreat_btn.show();
			} else if (step == 'ApplyDocumentCirculation.SuppliesManagerConfirm.Task') {
				// 确定
				// panel.end_btn.show();
				panel.show_btn.show();
				panel.end_btn.show();
			} else if (step == 'ApplyDocumentCirculation.ApplicantApply.Task') {
				// 退回
				panel.goonsubmit_btn.show();
				panel.forcestop_btn.show();
				panel.attach_btn.show();
				panel.finish_btn.show();
				panel.show_btn.show();
			}

			new Ext.Viewport({
				layout : 'border',
				items : [ panel ],
				border : false
			});

		});