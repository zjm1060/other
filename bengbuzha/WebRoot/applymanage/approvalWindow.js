Ext.ns("com.bitc.xmis.workflow");
/**
 * @param gridpanel
 *            用于grid的刷新
 * @param isAgreeInt
 *            int 是否同意的标识
 * @param ids
 *            多个workItemId拼成的字符串
 * @param closewin
 *            操作成功后是否需要关闭此窗口
 */
com.bitc.xmis.workflow.DoApprovalTaskIsAgree = function(isAgreeInt, workItemId,
		closewin) {
	var win = new Ext.Window({
		title : '请确定是否同意？',
		layout : 'border',
		width : 300,
		height : 180,
		modal : true,
		border : false,
		items : [new Ext.Panel({
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
				id:'sure_btn',
				handler : function() {
					var text = win.p.textID.getValue();
					if (isAgreeInt == '1' && text == '') {
						Ext.MessageBox.alert('提示', '不同意的原因不可为空，请填写原因！');
						return;
					}
					Ext.Ajax.request({
						url : String
								.format(
										'{0}/applyManageMaint.do?method=doApprovalTask',
										__cxtPath),
						method : 'post',
						params : {
							isAgree : isAgreeInt,
							comment : text,
							workItemId : workItemId
						},
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								Ext.Msg.alert(result.messageTitle,
										result.message, function() {
											parent.xMis.refresh('mytodo',
													'myworkitemnewtodo');
											if (closewin) {
												parent.xMis.closeTab(window);
											}
										});
							} else {
								Ext.Msg.alert(result.messageTitle,
										result.message)
							}
							win.hide();
						},
						failure : function(response) {
							Ext.Msg.alert('失败', '操作失败');
						}
					});

				}
			}, {
				text : '取消',
				handler : function() {
					win.hide();
				}
			}],
			items : [new Ext.form.Label({
								width : 100,
								hideMode : "visibility",// 默认display,可以取值：display，offsets，visibility
								html : "<font size='2px'>请填写备注:</font><br/>"
							}), {
						xtype : 'textarea',
						ref : 'textID',
						width : 250,
						name : 'text'
					}]
		})]
	}).show();

}
com.bitc.xmis.workflow.DoApprovalTaskIsAgree2 = function(isAgreeInt, workItemId,
		closewin) {
	var win = new Ext.Window({
		title : '请确定是否同意？',
		layout : 'border',
		width : 300,
		height : 180,
		modal : true,
		border : false,
		items : [new Ext.Panel({
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
					var text = win.p.textID.getValue();
					if (isAgreeInt == '1' && text == '') {
						Ext.MessageBox.alert('提示', '不同意的原因不可为空，请填写原因！');
						return;
					}
					Ext.Ajax.request({
						url : String
								.format(
										'{0}/applyManageMaint.do?method=doApprovalTask2',
										__cxtPath),
						method : 'post',
						params : {
							isAgree : isAgreeInt,
							comment : text,
							workItemId : workItemId
						},
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								Ext.Msg.alert(result.messageTitle,
										result.message, function() {
											parent.xMis.refresh('mytodo',
													'myworkitemnewtodo');
											if (closewin) {
												parent.xMis.closeTab(window);
											}
										});
							} else {
								Ext.Msg.alert(result.messageTitle,
										result.message)
							}
							win.hide();
						},
						failure : function(response) {
							Ext.Msg.alert('失败', '操作失败');
						}
					});

				}
			}, {
				text : '取消',
				handler : function() {
					win.hide();
				}
			}],
			items : [new Ext.form.Label({
								width : 100,
								hideMode : "visibility",// 默认display,可以取值：display，offsets，visibility
								html : "<font size='2px'>请填写备注:</font><br/>"
							}), {
						xtype : 'textarea',
						ref : 'textID',
						width : 250,
						name : 'text'
					}]
		})]
	}).show();

}

com.bitc.xmis.workflow.DoTaskIsOrNotAgree = function(isAgreeInt,earlyOrend, workItemId,formId,
		closewin) {
	var win = new Ext.Window({
		title : '请确定是否同意？',
		layout : 'border',
		width : 300,
		height : 180,
		modal : true,
		border : false,
		items : [new Ext.Panel({
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
					var text = win.p.textID.getValue();
					if (text == ''&&isAgreeInt==1) {
						Ext.MessageBox.alert('提示', '意见不能为空，请填写意见！');
						return;
					}
					Ext.Ajax.request({
						url : String
								.format(
										'{0}/applyManageMaint.do?method=updateMonthPlanComments',
										__cxtPath),
						method : 'post',
						params : {
							earlyOrEnd:earlyOrend,
							isAgree : isAgreeInt,
							comment : text,
							workItemId : workItemId,
							form_id:formId
						},
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								Ext.Msg.alert(result.messageTitle,
										result.message, function() {
											parent.xMis.refresh('mytodo',
													'myworkitemnewtodo');
											if (closewin) {
												parent.xMis.closeTab(window);
											}
										});
							} else {
								Ext.Msg.alert(result.messageTitle,
										result.message)
							}
							win.hide();
						},
						failure : function(response) {
							Ext.Msg.alert('失败', '操作失败');
						}
					});

				}
			}, {
				text : '取消',
				handler : function() {
					win.hide();
				}
			}],
			items : [new Ext.form.Label({
								width : 100,
								hideMode : "visibility",// 默认display,可以取值：display，offsets，visibility
								html : "<font size='2px'>请填写意见:</font><br/>"
							}), {
						xtype : 'textarea',
						ref : 'textID',
						width : 250,
						name : 'text'
					}]
		})]
	}).show();

}
com.bitc.xmis.workflow.ApprovalInfoRecord = Ext.data.Record.create(['ID', {
			name : '是否通过',
			type : 'int'
		}, {
			name : '审批人ID',
			type : 'int'
		}, {
			name : '审批时间',
			type : 'date',
			dateFormat : 'Y-m-d H:i:s'
		}, '任务名称', '审批人', '审批意见', 'workItemID', 'actorID', 'actorName']);
com.bitc.xmis.workflow.approvalInfoList = Ext.extend(Ext.grid.GridPanel, {
			autoLoadData : true,
			autoExpandColumn : 'c_comments',
			sortInfo : {
				field : '审批时间',
				direction : 'ASC'
			},
			initComponent : function() {
				this.store = new Ext.data.Store({
							url : __cxtPath
									+ '/workflow.do?method=getApprovalInfos',
							baseParams : {
								processInstanceId : this.processInstanceId,
								limit : 20
							},
							autoLoad : this.autoLoadData,
							remoteSort : true,
							sortInfo : this.sortInfo || {
								field : '审批时间',
								direction : 'ASC'
							},
							reader : new Ext.data.JsonReader({
										totalProperty : 'totalCount',
										root : 'rows'
									},
									com.bitc.xmis.workflow.ApprovalInfoRecord)
						});
				this.columns = [new Ext.grid.RowNumberer()];
				this.columns = this.columns.concat([{
							header : 'ID',
							hidden : true,
							dataIndex : 'ID'
						}, {
							header : 'workItemID',
							hidden : true,
							dataIndex : 'workItemID'
						}, {
							header : '任务名称',
							width : 200,
							dataIndex : '任务名称'
						}, {
							header : '审批人ID',
							hidden : true,
							dataIndex : '审批人ID'
						}, {
							header : '审批人',
							dataIndex : '审批人',
							renderer : function(value, metaData, record) {
								if (record.data.actorID == record.data.审批人ID) {
									return value;
								} else {
									return record.data.actorName + "(" + value
											+ "代理)";
								}
							}
						}, {
							header : '审批时间',
							dataIndex : '审批时间',
							xtype : 'datecolumn',
							format : 'm-d H:i',
							sortable : true
						}, {
							header : '是否通过',
							dataIndex : '是否通过',
							type : 'int',
							renderer : function(value) {
								if (value == 0) {
									return '<font  color="green">通过</font>';
								} else {
									return '<font  color="#CC0000">未通过</font>';
								}
							}
						}, {
							id : 'c_comments',
							header : '审批意见',
							dataIndex : '审批意见'
						}]);
				com.bitc.xmis.workflow.approvalInfoList.superclass.initComponent
						.call(this);
			}
		});
/**
 * 审批信息窗口
 */
com.bitc.xmis.workflow.ApprovalInfosButton = Ext.extend(Ext.Button, {
	iconCls : 'edit',
	text : '审批记录',
	initComponent : function() {
		/** 每次点击流程跟踪，数据库做一次查询操作 */
		this.handler = function() {
			var approvalInfoPanel = new com.bitc.xmis.workflow.approvalInfoList(
					{
						collapsible : true,
						region : 'center',
						title : '审批记录',
						autoLoadData : true,
						processInstanceId : this.processInstanceId,
						sortInfo : {
							field : '审批时间',
							direction : 'ASC'
						}
					});
			var approvalInfoWin = new Ext.Window({
						x : 25,
						y : 25,
						width : 700,
						height : 200,
						items : approvalInfoPanel,
						layout : 'border',
						closeAction : 'hide',
						tools : [{
									qtip : '刷新',
									id : 'refresh',
									handler : function() {
										approvalInfoPanel.reload();
									}
								}]
					});
			approvalInfoWin.show();
		};
		com.bitc.xmis.workflow.ApprovalInfosButton.superclass.initComponent
				.call(this);
	}
});
/**
 * 用于公文流转
 */
com.bitc.xmis.workflow.DoApprovalTaskIsAgreeWithFormInfo = function(isAgreeInt,
		workItemId, closewin, step, form_id) {
	var win = new Ext.Window({
		title : '请确定是否同意？',
		layout : 'border',
		width : 300,
		height : 180,
		modal : true,
		border : false,
		items : [new Ext.Panel({
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
					var text = win.p.textID.getValue();
					if (isAgreeInt == '1' && text == '') {
						Ext.MessageBox.alert('提示', '不同意的原因不可为空，请填写原因！');
						return;
					}
					Ext.Ajax.request({
						url : String
								.format(
										'{0}/applyManageMaint.do?method=doApprovalTaskWithFormInfo',
										__cxtPath),
						method : 'post',
						params : {
							isAgree : isAgreeInt,
							comment : text,
							workItemId : workItemId,
							step : step,
							form_id : form_id
						},
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								Ext.Msg.alert(result.messageTitle,
										result.message, function() {
											parent.xMis.refresh('mytodo',
													'myworkitemnewtodo');
											if (closewin) {
												parent.xMis.closeTab(window);
											}
										});
							} else {
								Ext.Msg.alert(result.messageTitle,
										result.message)
							}
							win.hide();
						},
						failure : function(response) {
							Ext.Msg.alert('失败', '操作失败');
						}
					});

				}
			}, {
				text : '取消',
				handler : function() {
					win.hide();
				}
			}],
			items : [new Ext.form.Label({
								width : 100,
								hideMode : "visibility",// 默认display,可以取值：display，offsets，visibility
								html : "<font size='2px'>请填写备注:</font><br/>"
							}), {
						xtype : 'textarea',
						ref : 'textID',
						width : 250,
						name : 'text'
					}]
		})]
	}).show();

};

Ext.ns("com.bbz");
com.bbz.AttachListWindow = function(form_id) {
	var tracePanel = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		autoScorll : true,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		cm : new Ext.grid.ColumnModel([// bsm,
		new Ext.grid.RowNumberer(), {
					dataIndex : '文件名',
					sortable : true,
					width : 280,
					header : '文件名'
				}, {
					xtype : 'actioncolumn',
					width : 40,
					header : '预览',
					align : 'center',
					items : [{
						getClass : function(v, meta, rec) {
							return 'search';
						},
						handler : function(grid, rowIndex, colIndex) {
							var record = grid.getStore().getAt(rowIndex);
							var attachementId = record
									.get(DBSchema.Attachment.AttachmentID);
							var xMis = xMis || parent.xMis;
							xMis
									.turn(
											'attachement-edit-' + attachementId,
											'文档预览-'
													+ record
															.get(DBSchema.Attachment.AttachmentName),
											__cxtPath
													+ '/weboffice.do?method=openAttachment&readOnly=true&attID='
													+ attachementId, '', false);
						}
					}]
				}]),
		store : new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
				url : __cxtPath
						+ '/applyManageMaint.do?method=getAttachmentListByAbsentFormId&form_id='
						+ form_id,
				method : "POST"
			}),
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, new Ext.data.Record.create([{
								"name" : DBSchema.Attachment.AttachmentName,
								"type" : "string"
							},{
								"name" : DBSchema.Attachment.AttachmentID,
								"type" : "string"
							}])),
			autoLoad : true,
			remoteSort : true,
			baseParams : {
				limit : 20
			}
		})
	});
	var traceWin = new Ext.Window({
				title : '附件列表',
				width : 400,
				height : 150,
				items : tracePanel,
				modal : true,
				layout : 'border'
			});
	traceWin.show();
}
