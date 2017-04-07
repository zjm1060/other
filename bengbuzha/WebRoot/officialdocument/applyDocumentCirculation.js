Ext.onReady(function() {
	Ext.QuickTips.init();
	var type = 2;
	var requestId = request.getParameter('form_id');
	var step = request.getParameter('step');
	var workItemId = request.getParameter('workItemId');
	var processInstanceId = request.getParameter('processInstanceId');
	var read = request.getParameter('read');
	var readOnly = request.getParameter('readOnly');
	if (readOnly != null && readOnly != '') {
		readOnly = true;
	} else {
		readOnly = false;
	}

	var attFileGrid = new com.bitc.xmis.AttachmentList({
				title : '相关文件列表',
				width : 300,
				height : 100,
				stripeRows : true,
				REFID : requestId == null ? 0 : requestId,
				attachmentTypeID : com.bitc.xmis.Attachment.document,
				NOT_DELETE:readOnly,
				READONLY : readOnly
			});
	var draft = new com.bitc.xmis.sys.Draft({
		model : 'doc',
		appId : 'step1-',
		userID : __emp.userID,
		text : '【公文编辑】来文传阅  创建时间' + new Date().format('m-d H:i'),
		url : String
				.format('officialdocument/applyDocumentCirculation.jsp?&form_id='
						+ requestId)
	});

	var handleStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/applyManageMaint.do?method=getDocumentCirculatedInstancesList',
			method : "POST"
		}),
		baseParams : {
			form_id : requestId,
			limit : 20
		},
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, Ext.data.Record.create([{
							"mapping" : "from_id",
							"name" : "from_id",
							"type" : "int"
						}, {
							"mapping" : "from_name",
							"name" : "from_name",
							"type" : "string"
						}, {

							"mapping" : "from_datetime",
							"name" : "from_datetime"
						}, {
							"mapping" : "to_id",
							"name" : "to_id",
							"type" : "int"
						}, {
							"mapping" : "to_name",
							"name" : "to_name",
							"type" : "string"
						}, {
							"mapping" : "comments",
							"name" : "comments",
							"type" : "string"
						}])),
		autoLoad : true,
		sortInfo : {
			field : 'from_datetime',
			direction : 'ASC'
		},
		remoteSort : true
	});

	var handleGrid = new Ext.grid.GridPanel({
				region : 'center',
				title : '传阅人记录',
				stripeRows : true,
				store : handleStore,
				autoExpandColumn : 'datatimeId',
				cm : new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(), {
							dataIndex : 'from_id',
							hidden : true,
							header : '处理人ID'
						}, {
							dataIndex : 'from_name',
							header : '处理人'
						}, {
							id : 'datatimeId',
							dataIndex : 'from_datetime',
							header : '处理时间'
						}, {
							dataIndex : 'comments',
							header : '处理人的意见'
						}, {
							dataIndex : 'to_id',
							hidden : true,
							header : '下一步处理人ID'
						}, {
							dataIndex : 'to_name',
							header : '下一步处理人'
						}])
			});
	var treePanel = new com.bitc.xmis.OrgTreePanel({
				ddGroup : 'doc-dd-group',
				title : '公司组织机构树',
				region : 'west',
				enableDrag : true,
				rootId : __emp.companyID,
				rootLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1
				//,
				//autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1
			});
	var data = {
		data : []
	};
	var step2Store = new Ext.data.JsonStore({
				root : 'data',
				data : data,
				idProperty : 'userId',
				fields : ['userId', 'deptId', 'deptName', 'userName']
			});
	var step2Grid = new Ext.grid.EditorGridPanel({
		title : '从左侧拖动人员到表格中：',
		ddGroup : 'doc-dd-group',
		height : 270,
		store : step2Store,
		autoScroll : true,
		tbar : [{
					text : '删除',
					iconCls : 'remove',
					handler : function() {
						var s = step2Grid.getSelectionModel().selection;
						if (s == null) {
							Ext.Msg.alert('提示', '请选择要删除的记录', Ext.Msg.INFO);
						} else {
							step2Store.remove(s.record);
						}
					}
				}],
		columns : [{
					header : '部门名称',
					width : 150,
					dataIndex : 'deptName'
				}, {
					header : '任务人ID',
					hidden : true,
					dataIndex : 'userId'
				}, {
					header : '接收人',
					width : 200,
					dataIndex : 'userName'
				}],
		listeners : {
			'afterrender' : function() {

				new Ext.dd.DropTarget(step2Grid.getView().scroller, {
					ddGroup : 'doc-dd-group',
					notifyEnter : function(dd, e, data) {
						if (this.overClass) {
							this.el.addClass(this.overClass);
						}
						return data.node.leaf
								? this.dropAllowed
								: this.dropNotAllowed;
					},
					notifyOver : function(dd, e, data) {
						return data.node.leaf
								? this.dropAllowed
								: this.dropNotAllowed;
					},
					notifyDrop : function(ddSource, e, data) {
						var node = data.node;
						if (!node.leaf) {
							return false;
						}
						var record = new step2Store.recordType({
							deptId : node.parentNode.attributes.attributes.id,
							deptName : node.parentNode.attributes.attributes.name,
							userId : node.attributes.attributes.id,
							userName : node.text
						}, node.attributes.attributes.id);
						step2Store.insert(step2Store.data.length, record);
						return (true);
					}
				});
			}

		}
	});

	var documentCirculationWindow = new Ext.Window({
		title : '选择要传阅人员',
		layout : 'border',
		width : 740,
		height : 400,
		modal : true,
		closeAction : 'hide',
		tbar : [{
			xtype : 'button',
			text : '提交',
			ref : '../submit_btn',
			iconCls : 'save',
			handler : function() {

				if (step2Store.data.length < 1) {
					Ext.Msg.alert('提示', '传阅的接收人不允许为空！');
					return;
				} else {
					var to_id_str = '';
					step2Store.each(function(record) {
								to_id_str += record.get('userId') + ',';
							});
					if (applyForm.getForm().isValid()) {
						Ext.MessageBox.confirm("提示", "是否提交此条信息？",
								function(btn) {
									if (btn == 'yes') {
										Ext.Ajax.request({
											url : __cxtPath
													+ '/applyManageMaint.do?method=doUpdateDocumentCirculatedInstances',
											method : 'post',
											params : {
												instanc_id : requestId,
												from_id : __emp.userID,
												from_datetime : Ext.util.Format
														.date(new Date(),
																'Y-m-d H:i:s'),
												to_id : to_id_str.substring(0,
														to_id_str.length - 1),

												comments : Ext
														.getCmp('comments').form
														.findField('comments')
														.getValue()

											},
											success : function(response, action) {

												var result = Ext
														.decode(response.responseText);
												if (result.success) {
													Ext.Msg
															.alert(
																	result.messageTitle,
																	result.message);

												} else {
													Ext.Msg
															.alert(
																	result.messageTitle,
																	result.message);
												}
												documentCirculationWindow
														.hide();
												handleStore.reload();
												panel.tbar.hide();

											},
											failure : function(response, action) {
												var result = Ext
														.decode(response.responseText);
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
			}
		}],
		items : [treePanel, new Ext.form.FormPanel({
							id : 'comments',
							ref : 'form',
							region : 'center',
							labelAlign : 'right',
							labelWidth : 80,
							items : [step2Grid, {
										xtype : 'textarea',
										fieldLabel : '意见',
										name : 'comments',
										anchor : '95%'
									}]
						})]
	});
	var applyRecord = Ext.data.Record.create([{
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
				name : 'app_datetime',
				type : 'string'
			}, {
				name : 'app_user_id',
				type : 'int'
			}, {
				name : 'app_user_name',
				type : 'string'
			},{
				name : 'app_dept_id',
				type : 'int'
			}, {
				name : 'app_dept_name',
				type : 'string'
			},  {
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
			}]);

	var applyForm = new Ext.form.FormPanel({
				ref : 'form',
				region : 'center',
				padding : 10,
				autoScroll : true,
				border : false,
				labelWidth : 90,
				labelAlign : 'right',
				reader : new Ext.data.JsonReader({
							root : 'rows'
						}, applyRecord),
				items : [{
					xtype : 'panel',
					layout : 'column',
					border : false,
					items : [{
								xytpe : 'panel',
								layout : 'form',
								columnWidth : 0.4,
								border : false,
								items : [{
											xtype : 'hidden',
											value : 2,
											name : 'document_type_id'
										}, {
											xtype : 'displayfield',
											fieldLabel : '公文类别',
											width : 150,
											value : '来文传阅',
											name : 'document_type_name'
										},{
											xtype : 'textfield',
											fieldLabel : '来文单位名称',
											width : 150,
											allowBlank:false,
											name : 'external_name'
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
											width : 150,
											store : new Ext.data.ArrayStore({
														fields : ['value',
																'text'],
														data : [[1, '紧急'],
																[0, '普通']]
													})
										}, {
											xtype : 'textfield',
											fieldLabel : '标题',
											allowBlank : false,// 不允许为空
											width : 150,
											name : 'title'
										},{
											xtype : 'textfield',
											fieldLabel : '公文编号',
											width : 150,
											name : 'document_code'
										}, {
											xtype : 'textarea',
											fieldLabel : '摘要说明',
											allowBlank : false,// 不允许为空
											width : 150,
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
										},{
											xtype : 'panel',
											layout : 'form',
											border : false,
											labelAlign : 'rigth',
											width:300,
											items : [{
														xtype : 'hidden',
														fieldLabel : '申请部门ID',
														id : 'app_dept_id',
														name : 'app_dept_id'
													},{
														xtype : 'displayfield',
														fieldLabel : '申请部门',
														name : 'app_dept_name',
														id:'app_dept_name'
													}]
										
										}, attFileGrid]
							}, {
								xytpe : 'panel',
								layout : 'form',
								columnWidth : 0.6,
								border : false,
								items : [{
											xtype : 'panel',
											border : false,
											height : 450,
											width : 600,
											items : [handleGrid],
											layout : 'border'
										}]
							}]

				}]
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
				text : '保存草稿',
				ref : '../saveDraft',
				// hidden : true,
				iconCls : 'save',
				handler : function() {

					var content = {
						form : applyForm.getForm().getValues(),
						grid : com.bitc.xmis.util.ExtUtil
								.getStoreData(attFileGrid.store),
						commentsParam : Ext.getCmp('comments').form
								.findField('comments').getValue(),
						grid2 : com.bitc.xmis.util.ExtUtil
								.getStoreData(step2Store)

					};

					draft.save(function(d, result) {
								if (parent && parent.xMis) {
									parent.xMis.refresh('mydraft');
								}
								Ext.Msg.alert('提示',
										'保存草稿成功！<br/>你可以在首页的草稿箱中找到。');

							}, applyForm, content);
				}
			}, {
				xtype : 'button',
				text : '保存',
				// hidden : true,
				ref : '../save_btn',
				iconCls : 'add',
				handler : function() {
					applyForm.getForm().submit({
						url : __cxtPath
								+ '/applyManageMaint.do?method=saveDocumentAppFormDetail',
						params : {
							app_user_id : __emp.userID,
							app_datetime : Ext.util.Format.date(new Date(),
									'Y-m-d H:i:s'),
							form_id : requestId,
							priority : applyForm.getForm()
									.findField('priority').getValue()
						},
						scope : this,
						waitTitle : "请稍候",
						waitMsg : '提交数据，请稍候...',
						success : function(response, action) {
							Ext.MessageBox.alert(action.result.messageTitle,
									action.result.message);

							parent.xMis
									.turn(
											'doc-info-'
													+ action.result.attributes.form_id,
											'公文编辑',
											__cxtPath
													+ '/officialdocument/applyDocumentCirculation.jsp?&form_id='
													+ action.result.attributes.form_id,
											'menu-doc-info', true);
							if (requestId == null) {
								parent.xMis.closeTab(window);
							}

						},
						failure : function(response, action) {
							com.bitc.xmis.util.AlertUtil
									.formSubmitFailedAlert(action);
						}
					});
				}
			}, {
				xtype : 'uploadButton',
				text : '附件上传',
				ref : '../attach_btn',
				// hidden : true,
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
			}, {
				xtype : 'button',
				text : '选择要传阅人员',
				// hidden : true,
				ref : '../submit_btn',
				iconCls : 'save',
				handler : function() {
					if (attFileGrid.store.data.length < 1) {
						Ext.Msg.alert('提示', '附件列表不允许为空！');
						return;
					}
					documentCirculationWindow.form.getForm().reset();
					step2Grid.store.removeAll();// 第二次打开之前清空拖拽grid的数据
					documentCirculationWindow.show();
				}
					// }, {
					// xtype : 'button',
					// text : '提交',
					// // hidden : true,
					// ref : '../submit_btn',
					// iconCls : 'save',
					// handler : function() {
					// if (attFileGrid.store.data.length < 1) {
					// Ext.Msg.alert('提示', '附件列表不允许为空！');
					// return;
					// } else if (step2Store.data.length < 1) {
					// Ext.Msg.alert('提示', '传阅的接收人不允许为空！');
					// return;
					// } else {
					// var to_id_str = '';
					// step2Store.each(function(record) {
					// to_id_str += record.get('userId') + ',';
					// });
					// if (applyForm.getForm().isValid()) {
					// Ext.MessageBox.confirm("提示", "是否提交此条信息？", function(
					// btn) {
					// if (btn == 'yes') {
					// Ext.Ajax.request({
					// url : __cxtPath
					// +
					// '/applyManageMaint.do?method=doUpdateDocumentCirculatedInstances',
					// method : 'post',
					// params : {
					// instanc_id : requestId,
					// from_id : __emp.userID,
					// from_datetime : Ext.util.Format
					// .date(new Date(),
					// 'Y-m-d H:i:s'),
					// to_id : to_id_str.substring(0,
					// to_id_str.length - 1),
					//
					// comments : applyForm.getForm()
					// .findField('comments')
					// .getValue()
					// },
					// success : function(response, action) {
					//
					// var result = Ext
					// .decode(response.responseText);
					// if (result.success) {
					// Ext.Msg.alert(
					// result.messageTitle,
					// result.message);
					//
					// } else {
					// Ext.Msg.alert(
					// result.messageTitle,
					// result.message);
					// }
					// panel.tbar.hide();
					//
					// },
					// failure : function(response, action) {
					// var result = Ext
					// .decode(response.responseText);
					// Ext.MessageBox.hide();
					// Ext.Msg.alert(result.messageTitle,
					// result.message);
					// }
					// });
					//
					// }
					// });
					// }
					// }
					// }
				}]
		},
		items : [applyForm]
	});

	if (requestId != null && requestId != '') {

		applyForm.getForm().load({
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

		panel.attach_btn.show();
		panel.submit_btn.show();
		panel.save_btn.hide();
	} else {
		panel.attach_btn.hide();
		panel.submit_btn.hide();
		panel.save_btn.show();

		applyForm.getForm().findField('app_user_id').setValue(__emp.userID);
		applyForm.getForm().findField('app_user_name').setValue(__emp.userName);
		applyForm.getForm().findField('app_dept_id').setValue(__emp.departmentID);
		applyForm.getForm().findField('app_dept_name').setValue(__emp.departmentName);
		applyForm.getForm().findField('app_datetime').setValue(Ext.util.Format
				.date(new Date(), 'Y-m-d H:i:s'));
	}
	// 初始化草稿
	if (request.getParameter("draftId") != null) {
		var draftId = request.getParameter("draftId");
		draft.id = draftId;
		draft.load(function(d) {
					var c = d.content;

					applyForm.getForm().setValues(c.form);
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
					Ext.getCmp('comments').form.findField('comments')
							.setValue(c.commentsParam);

					Ext.iterate(c.grid, function(item) {
								records
										.push(new attFileGrid.store.recordType(item));
							}, this);
					Ext.iterate(c.grid2, function(item) {
								records
										.push(new step2Grid.store.recordType(item));
							}, this);
					attFileGrid.store.add(records);
				}, this);

	}
	new Ext.Viewport({
				layout : 'border',
				items : [panel],
				border : false
			});

});