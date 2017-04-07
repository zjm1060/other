Ext
		.onReady(function() {
			Ext.QuickTips.init();
			var type = request.getParameter('type');
			var requestId = request.getParameter('form_id');
			var step = request.getParameter('step');
			var workItemId = request.getParameter('workItemId');
			var processInstanceId = request.getParameter('processInstanceId');
			var approvalInfoGridPanel = new com.bitc.xmis.workflow.approvalInfoList(
					{
						collapsible : true,
						region : 'south',
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
				text : '【公文编辑】' + (type == 1 ? '内部流转' : '上行文流转') + ' 创建时间'
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
			},{
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
				padding : 20,
				columnWidth : 0.4,
				autoScroll : true,
				border : false,
				labelWidth : 90,
				labelAlign : 'right',
				reader : new Ext.data.JsonReader({
					root : 'rows'
				}, applyRecord),
				items : [ {
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
				}, {
					xytpe : 'panel',
					layout : 'form',
					columnWidth : 1.0,
					border : false,
					items : [ {
						xtype : 'hidden',
						name : 'document_type_id'
					},{
						xtype : 'hidden',
						name : 'file_no'
					}, {
						xtype : 'displayfield',
						fieldLabel : '公文类别',
						width : 150,
						name : 'document_type_name'
					}, {
						xtype : 'textfield',
						fieldLabel : '发送至',
						width : 250,
						name : 'external_name',
						allowBlank : false,// 不允许为空
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
					}, /*{
						xtype : 'textfield',
						fieldLabel : '公文编号',
						width : 250,
						name : 'document_code'
					}*/,{
						xtype : 'combo',
						fieldLabel : '公文标志(字)',
						width : 250,
						allowBlank : false,// 不允许为空
							id : 'file_flag',
							name : 'file_flag',
							displayField : 'file_flag',
							valueField : 'file_flag',
							mode : 'local',
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
						}, {
							xtype : 'panel',
							border : false,
							items : attFileGrid
						} ]

					} ]
				} ]
			});
			var zwPanel = new Ext.Panel({
				id : "zwPanel",
				columnWidth : 0.6,
				border : false,
				fitToFrame : true,
				html : ''
			});

			var panelCenter = new Ext.Panel({

				region : 'center',
				title : '',
				layout : 'column',
				// height:500,
				border : false,
				autoScroll : true,
				items : [ applyForm, zwPanel ]
			});
			var panel = new Ext.Panel(
					{
						region : 'center',
						title : '',
						layout : 'border',
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
																	form_id : requestId,
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
																							+ '/officialdocument/applyDocument.jsp?form_id='
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

									},{
										xtype : 'button',
										text : '成文',
										hidden : true,
										ref : '../XWcreat_btn',
										iconCls : 'add',
										handler : function() {

											panel.XWcreat_btn.hide();
											panel.XWshow_btn.show();
											panel.XWdown_btn.show();
											panel.end_btn.show();
											var attFile="";
											for(var i =0;i<attFileGrid.store.data.length;i++){
												attFile =attFile+' ('+(i+1)+')'+attFileGrid.store.getAt(i).get(DBSchema.Attachment.AttachmentName);
											}
											var url = __cxtPath
													+ '/officialdocument/creatXW.jsp?'
													+ 'form_id='
													+ requestId
													+ '&title='
													+ applyForm.getForm()
															.findField('title')
															.getValue()
													+ '&approved_datetime='
													+ applyForm
															.getForm()
															.findField(
																	'app_direc_approved_datetime')
															.getValue()
													+'&attFile='+attFile
													+ '&file_no='
													+ applyForm
															.getForm()
															.findField(
																	'file_no')
															.getValue()
															+ '&file_flag='
													+ Ext.getCmp('file_flag')
															.getValue();
											window.open(url,'合成正文', 'height=100, width=200, top=400, left=400, toolbar=no, menubar=no, scrollbars=no, resizable=no, location=no, status=no');
										}

									},{
										xtype:'button',
										text:'行文预览',
										hidden:true,
										ref : '../XWshow_btn',
										iconCls : 'add',
										handler : function() {
											var url = __cxtPath
											+'/officialdocument/OpenWord.jsp?form_id='
												+ requestId+'&zw=1';
											parent.xMis.turn(
													'XWshow-doc', '行文预览',
													url, 't', true);
										}
									},{
										xtype:'button',
										text:'行文下载',
										hidden:true,
										ref : '../XWdown_btn',
										iconCls : 'add',
										handler : function() {
											var url = __cxtPath
											+ '/fileUpload.do?type=11'
											+ '&form_id='
											+ requestId
											+'&fileName=行文';
											location.href=url;
										}
									
									},
									{
										xtype : 'button',
										text : '拟稿预览',
										hidden:true,
										ref : '../show_btn',
										iconCls : 'add',
										handler : function() {
											var attFile="";
											for(var i =0;i<attFileGrid.store.data.length;i++){
												attFile =attFile+' ('+(i+1)+')'+attFileGrid.store.getAt(i).get(DBSchema.Attachment.AttachmentName);
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
													+ '&title='
													+ applyForm.getForm()
															.findField('title')
															.getValue()
													+'&attFile='+attFile;

											parent.xMis.turn(
													'showDocument-doc', '拟稿预览',
													url, 't', true);
										}

									},{
										xtype : 'button',
										text : '提交',
										hidden : true,
										ref : '../submit_btn',
										iconCls : 'save',
										handler : function() {
											if (applyForm.getForm()
													.isValid()) {
												Ext.MessageBox
														.confirm(
																"提示",
																"是否提交此条信息？",
																function(
																		btn) {
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
						items : [ panelCenter, approvalInfoGridPanel ]
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
				if (requestId == null) {
					panel.save_btn.show();
				} else {
					panel.attach_btn.show();
					panel.submit_btn.show();
					Ext.getCmp("zwPanel").html = '<iframe id="frame" src="/bengbuzha/officialdocument/CreateWord.jsp?form_id='
							+ requestId
							+ '&attachmentTypeID=18" frameborder="0" width="100%" height="600px"></iframe>';
				}
				if (type == 1) {
					applyForm.getForm().findField('document_type_id').setValue(
							1);
					applyForm.getForm().findField('document_type_name')
							.setValue("内部公文流转");
				} else if (type == 3) {
					applyForm.getForm().findField('document_type_id').setValue(
							3);
					applyForm.getForm().findField('document_type_name')
							.setValue("上行公文");
				}
				panel.saveDraft.show();
			} else if (step == 'ApplyDocumentCirculation.DeptManagerApproval.Task'
					|| step == 'ApplyDocumentCirculation.ChargeLeaderApproval.Task'
					|| step == 'ApplyDocumentCirculation.OfficeDirectorApproval.Task') {
				// 审批
				panel.agree_btn.show();
				panel.notagree_btn.show();
				panel.show_btn.show();
				Ext.getCmp("zwPanel").html = '<iframe id="frame"  src="/bengbuzha/officialdocument/OpenWord.jsp?form_id='
						+ requestId
						+ '" frameborder="0" width="100%" height="600px"></iframe>';
			} else if (step == 'ApplyDocumentCirculation.Activity7.Task') {
				// 上下文处理人 确定
				Ext.getCmp("agree").setText("任务确认");
				Ext.getCmp("agree").show();
				panel.show_btn.show();
				//panel.XWcreat_btn.show();
				Ext.getCmp("zwPanel").html = '<iframe id="frame" src="/bengbuzha/officialdocument/OpenWord.jsp?form_id='
						+ requestId
						+ '" frameborder="0" width="100%" height="600px"></iframe>';
			} else if (step == 'ApplyDocumentCirculation.SuppliesManagerConfirm.Task') {
				// 确定
				//panel.end_btn.show();
				panel.show_btn.show();
				panel.XWcreat_btn.show();
				Ext.getCmp("zwPanel").html = '<iframe id="frame"  src="/bengbuzha/officialdocument/OpenWord.jsp?form_id='
						+ requestId
						+ '" frameborder="0" width="100%" height="600px"></iframe>';
			} else if (step == 'ApplyDocumentCirculation.ApplicantApply.Task') {
				// 退回
				panel.goonsubmit_btn.show();
				panel.forcestop_btn.show();
				panel.attach_btn.show();
				panel.show_btn.show();
				Ext.getCmp("zwPanel").html = '<iframe id="frame"  src="/bengbuzha/officialdocument/OpenWord.jsp?form_id='
						+ requestId
						+ '" frameborder="0" width="100%" height="600px"></iframe>';
			}

			new Ext.Viewport({
				layout : 'border',
				items : [ panel ],
				border : false
			});

		});