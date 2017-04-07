Ext.onReady(function() {
	Ext.QuickTips.init();
	function linkToProject() {
		if (parent && parent.xMis) {
			parent.xMis
					.turn(
							'project-info-' + project.projectID,
							'项目信息-' + project.projectName,
							__cxtPath
									+ '/projectMain.do?method=getProjectInfo&projectID='
									+ project.projectID + '&isAgree='
									+ request.getParameter("isAgree"),
							'menu-project-info');
		}
	}
	var projectID = request.getParameter("projectID");
	var processTaskId = request.getParameter("processTaskId");// 任务ID
	var workItemId = request.getParameter("workItemId");// 
	var checkDeptID = request.getParameter("checkDeptID");// 
	var isCheckIdHidden = false;
	if (checkDeptID == null) {
		isCheckIdHidden = true;
	}
	var applicant = request.getParameter("applicant");// 
	var comment = request.getParameter("comment");// 

	var isApproved = request.getParameter("isApproved");
	var isHiddenManager = true;
	var isHiddenDept = true;
	var isHiddenAcceptor = true;
	var isHiddenAcceptorGoOn = true;
	var isHiddenCheckDept = true;
	if (processTaskId == "ProjectCheck.ManagerCheck.ManagerCheckTask" || processTaskId == "ProjectCheck.EngineerCheck.EngineerCheckTask") {
		isHiddenManager = false;
	}
	if (processTaskId == "ProjectCheck.DeptManagerAssign.DeptManagerAssignTask"
			|| processTaskId == "InterCheckAccept.AssignTaskToAccept.AssignTaskToAcceptTask") {
		isHiddenDept = false;
	}
	if (processTaskId == "ProjectCheck.AcceptorCheck.AcceptorCheckTask") {
		isHiddenAcceptor = false;
	}
	// 验收部门主任审批
	if (processTaskId == "ProjectCheck.Activity14.Task6") {
		isHiddenCheckDept = false;
	}
	// 退回的内审结果
	if (processTaskId == 'ProjectCheck.Activity1.Task1') {
		isHiddenAcceptorGoOn = false;
	}
	var task = {};
	if (projectTask != null) {
		for (var i = 0; i < projectTask.members.length; i++) {
			var m = projectTask.members[i];
			if (__emp.userID == m.member.userID) {
				task = m;
			}
		}
	}
	var reRecord = Ext.data.Record.create([{
			name : '步骤名称'
		}, {
			name : '任务名称'
		}, {
			name : '操作人'
		}, {
			name : '完成时间'
		}, {
			name : '备注'
			}]);
	var checkStore = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
									url : __cxtPath + '/projectMain.do?method=getInterCheckList'
								}),
						baseParams : {
								processInstanceId :request.getParameter("processInstanceId")
							},
						autoLoad : true,
						reader : new Ext.data.JsonReader({
								root : 'rows',
								totalProperty : 'totalCount'
							}, reRecord)
	});
	var checkCm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(),{
				header : "步骤名称",
				dataIndex : '步骤名称'
			} ,{
				header : "任务名称",
				dataIndex : '任务名称'
			} ,{
				header : "操作人",
				dataIndex : '操作人'
			} ,{
				header : "是否通过",
				dataIndex : '备注',
				renderer : function(value){
					if(value != null){
						if(value.indexOf('-')>-1){
							return value.split('-')[0];
						}
					}
				}
			} ,{
				header : "意见",
				dataIndex : '备注',
				renderer : function(value){
					if(value != null){
						var position = value.indexOf('-');
						if(position>-1){
							return value.substr(position+1);
						}else {
						return value;
					}
					} 
				}
			} ,{
				header : "完成时间",
				dataIndex : '完成时间'
			}]);
	var checkInfGrid = new Ext.grid.GridPanel({
				title : '内审情况',
				stripeRows : true,
				width : 600,
				height : 150,
				store : checkStore,
				cm : checkCm,
				collapsible : true,
				autoScorll :true
			});

	var attFileGrid = new com.bitc.xmis.AttachmentList({
				title : '内部验收相关文档',
				stripeRows : true,
				REFID : project.projectID,
				attachmentTypeID : com.bitc.xmis.Attachment.intercheck,
				width : 600,
				collapsible : true,
				autoHeight : true
			});

	var stepGrid = new Ext.grid.EditorGridPanel({
				height : 256,
				title : '从左侧拖动人员到表格中',
				ddGroup : 'project-org-dd-group',
				store : new Ext.data.JsonStore({
							root : 'data',
							data : {
								data : []
							},
							idProperty : 'userId',
							fields : ['member', 'taskDesc', 'role']
						}),
				autoScroll : true,
				clicksExpandColumn : 'taskDescCol',
				tbar : [{
							text : '删除',
							iconCls : 'remove',
							handler : function() {
								var s = stepGrid.getSelectionModel().selection;
								if (s == null) {
									Ext.Msg.alert('提示', '请选择要删除的记录',
											Ext.Msg.INFO);
								} else {
									stepGrid.store.remove(s.record);
								}
							}
						}],
				columns : [{
							header : '任务人ID',
							hidden : true,
							dataIndex : 'member.userID'
						}, {
							header : '任务人',
							dataIndex : 'member.userName'
						}, {
							id : 'taskDescCol',
							header : '任务描述',
							width : 400,
							dataIndex : 'taskDesc',
							editor : new Ext.form.TextArea()
						}],
				bbar : ['点击任务描述可编辑']
			});

	var projectTaskWindow = new Ext.Window({
		title : '分配任务',
		layout : 'border',
		width : 740,
		height : 400,
		modal : true,
		closeAction : 'hide',
		tbar : [{
			iconCls : 'ok',
			text : '提交',
			handler : function() {
				var f = projectTaskWindow.form.getForm();
				if (stepGrid.store.data.length == 0) {
					Ext.Msg.alert('提示', '请选择任务人员。');
					return;
				} else {
					for (var i = 0; i < stepGrid.store.data.length; i++) {
						if (stepGrid.store.getRange()[i].data.taskDesc == ''
								|| stepGrid.store.getRange()[i].data.taskDesc
										.trim() == '') {
							Ext.Msg.alert('提示', '请添加任务描述。');
							return;
						}
					}
				}
				var members = com.bitc.xmis.util.ExtUtil
						.encodeStoreData(stepGrid.store);
				f.submit({
					url : String.format(
							'{0}/projectMain.do?method=checkDeptAssignTask',
							__cxtPath),
					method : 'post',
					params : {
						projectID : project.projectID,
						workItemId : workItemId,
						comment:'无',
						divisionManagerID:a.sender.userID,
						members : members
					},
					waitMsg : '正在提交，请稍候……',
					waitTitle : '正在处理',
					scope : this,
					success : function(form, act) {
						Ext.Msg.alert(act.result.messageTitle,
								act.result.message);
						projectTaskWindow.hide();
						parent.xMis.refresh('mytodo','myworkitemnewtodo');
						//parent.xMis.closeTab(window);
						center.getTopToolbar().btn_assigntask.setDisabled(true);
					},
					failure : function(form, act) {
						com.bitc.xmis.util.AlertUtil.formSubmitFailedAlert(act);
					}
				});
			}
		}, {
			xtype : 'uploadButton',
			text : '上传验收大纲',
			hidden : isHiddenDept,
			REFID : project.projectID,
			attachmentTypeID : com.bitc.xmis.Attachment.intercheck,
			uploadPanelConfig : {
				listeners : {
					'uploadcomplete' : function() {
						attFileGrid.reload();
					}
				}
			}

		}],
		items : [new com.bitc.xmis.OrgTreePanel({
							ref : 'orgtree',
							title : '人员选择',
							region : 'west',
							ddGroup : 'project-org-dd-group',
							enableDrag : true,
							rootId : __emp.companyID,
							rootLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1,
							rootFilter : __emp.divisionID,
							autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1
						}), new Ext.form.FormPanel({
							ref : 'form',
							region : 'center',
							padding : 5,
							labelAlign : 'right',
							labelWidth : 80,
							items : [{
										xtype : 'textarea',
										fieldLabel : '任务名称',
										name : 'taskName',
										allowBlank : false,
										anchor : '95%'
									}, stepGrid]
						})],
		listeners : {
			show : function() {
				if (!this.dragInited) {
					new Ext.dd.DropTarget(stepGrid.getView().scroller, {
						ddGroup : 'project-org-dd-group',
						notifyEnter : function(dd, e, data) {
							if (this.overClass) {
								this.el.addClass(this.overClass);
							}
							return data.node.leaf
									&& stepGrid.store
											.getById(data.node.attributes.attributes.id) == undefined
									? this.dropAllowed
									: this.dropNotAllowed;
						},
						notifyOver : function(dd, e, data) {
							return data.node.leaf
									&& stepGrid.store
											.getById(data.node.attributes.attributes.id) == undefined
									? this.dropAllowed
									: this.dropNotAllowed;
						},
						notifyDrop : function(ddSource, e, data) {
							var node = data.node;
							if (!node.leaf
									|| node.leaf
									&& stepGrid.store
											.getById(data.node.attributes.attributes.id) !== undefined) {
								return false;
							}
							var record = new stepGrid.store.recordType({
										member : {
											userID : node.attributes.attributes.id,
											userName : node.text
										},
										taskDesc : '',
										role : 3
									}, node.attributes.attributes.id);
							stepGrid.store.insert(stepGrid.store.data.length,
									record);
							return (true);
						}
					});
					this.dragInited = true;
				}
			}
		}
	});
	var center = new Ext.Panel({
		border : false,
		padding : 5,
		region : 'center',
		items : [{
			xtype : 'panel',
			title : '验收信息',
			collapsible : true,
			style : 'padding: 5px 0 0 0',
			width : 600,
			layout : 'form',
			defaultType : 'displayfield',
			labelAlign : 'right',
			labelWidth : 128,
			items : [{
						fieldLabel : '项目编码',
						value : project.projectCode,
						clickAction : linkToProject
					}, {
						fieldLabel : '项目名称',
						value : project.projectName,
						clickAction : linkToProject
					}, {
						fieldLabel : '验收部门',
						hidden : isCheckIdHidden,
						value : dept != null ? dept.departmentName : ''
					}, {
						fieldLabel : '审批结果',
						hidden : isHiddenCheckDept,
						value : isApproved == 'yes' ? '同意' : '不同意'
					}, {
						fieldLabel : '备&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;注',
						value : comment
					}, {
						fieldLabel : '任务名称',
						hidden : isHiddenAcceptor,
						value : projectTask != null ? projectTask.taskName : ''
					}, {
						fieldLabel : '任务说明',
						hidden : isHiddenAcceptor,
						value : task.taskDesc
					}]
		}, attFileGrid
		, checkInfGrid
		],
		tbar : [{
			iconCls : 'ok',
			text : '同意', // 总工、主任审批
			hidden : isHiddenManager,
			ref:'btn_managerApprove',
			handler : function() {
				Ext.Msg.prompt('同意审批?', '请填写备注：', function(btn, text) {
					if (btn == 'ok') {
						Ext.Ajax.request({
							url : String
									.format(

											'{0}/projectMain.do?method=managerCheck&workItemId={1}&isAgree={2}&applicant={3}',
											__cxtPath, workItemId, 'yes',
											applicant),
							method : 'post',
							params : {
								comment : '同意- ' + text
							},
							success : function(response) {
								var result = Ext.decode(response.responseText);
								if (result.success) {
									Ext.Msg.alert(result.messageTitle,
											result.message, function() {
												parent.xMis.refresh('mytodo','myworkitemnewtodo');
												checkInfGrid.getStore().reload();
												//此标签不关闭，将审批按钮隐藏，使“流程跟踪”可用，能知道下一步的接收人
												center.getTopToolbar().btn_managerApprove.setDisabled(true);
												center.getTopToolbar().btn_managerNotApprove.setDisabled(true);
											});
								} else {
									Ext.Msg.alert(result.messageTitle,
											result.message)
								}
							},
							failure : function(response) {
								Ext.Msg.alert('失败', '操作失败');
							}
						});

					}
				}, this, true);

			}

		}, {
			iconCls : 'ok',
			text : '不同意',// 总工、主任审批
			hidden : isHiddenManager,
			ref:'btn_managerNotApprove',
			handler : function() {
				Ext.Msg.prompt('不同意审批？', '请填写不同意的原因：', function(btn, text) {
							if (btn == 'ok') {
								if (text == '' || text.trim() == '') {
									Ext.Msg.alert('提示', '请填写不同意的原因');
								}
								var url = __cxtPath
										+ '/projectMain.do?method=managerCheck';
								Ext.Ajax.request({
											url : url,
											method : 'post',
											params : {
												isAgree : 'no',
												workItemId : workItemId,
												applicant : applicant,
												comment : '不同意- ' + text
											},
											success : function(response, opts) {

												var obj = Ext
														.decode(response.responseText);
												Ext.Msg.alert(obj.messageTitle,
														obj.message);
												if (obj.success) {
													parent.xMis.refresh('mytodo','myworkitemnewtodo');
													checkInfGrid.getStore().reload();
													center.getTopToolbar().btn_managerApprove.setDisabled(true);
													center.getTopToolbar().btn_managerNotApprove.setDisabled(true);
												} else {

												}
											},
											failure : function(response, opts) {
												Ext.Msg
														.alert(
																'拒收失败',
																'服务器错误，状态码：'
																		+ response.status);
											}
										});
							}
						}, this, true);
			}
		}, {
			iconCls : 'ok',
			text : '分配任务',
			hidden : isHiddenDept,
			ref : 'btn_assigntask',
			handler : function() {
				projectTaskWindow.form.getForm().reset();
				projectTaskWindow.show();
				projectTaskWindow.form.getForm().findField('taskName')
						.setValue('项目' + project.projectCode + '的内部审批');
			}
		}, {
			iconCls : 'ok',
			text : '同意',// 验收员验收
			hidden : isHiddenAcceptor,
			ref:'btn_assignorapproved',
			handler : function() {
				// 同意后，上传验收报告
				var win = new Ext.Window({
					title : '确定同意验收?',
					layout : 'border',
					width : 300,
					height : 170,
					modal : true,
					items : [new Ext.FormPanel({
								ref : 'form',
								region : 'center', //
								layout : 'auto',
								labelAlign : 'right',
								padding : 10,
								items : [{
											xtype : 'displayfield',
											value : '请填写备注：'
										}, {
											xtype : 'textarea',
											fieldLabel : '备注',
											width : 250,
											id : 'myComment',
											name : 'text'
										}]
							})],
					tbar : [{
						iconCls : 'ok',
						text : '提交',
						handler : function() {
							Ext.Ajax.request({
								url : String
										.format(
												'{0}/projectMain.do?method=tellDeptManager&projectID={1}&checkDeptID={2}&workItemId={3}&applicant={4}&isApprove={5}&checkManagerID={6}',
												__cxtPath, projectID,
												dept.departmentID, workItemId,
												applicant, 'yes',a.sender.userID),
								method : 'post',
								params : {
									comment : '同意-'+win.form.getForm()
											.findField('text').getValue()
								},
								success : function(response) {
									var result = Ext
											.decode(response.responseText);
									if (result.success) {
										Ext.Msg.alert(result.messageTitle,
												result.message, function() {
													parent.xMis.refresh('mytodo','myworkitemnewtodo');
													win.close();
													center.getTopToolbar().btn_assignorapproved.setDisabled(true);
													center.getTopToolbar().btn_assignornotapproved.setDisabled(true);
												});
									} else {
										Ext.Msg.alert(result.messageTitle,
												result.message)
									}
								},
								failure : function(response) {
									Ext.Msg.alert('失败', '操作失败');
								}
							});

						}
					}, {
						xtype : 'uploadButton',
						text : '上传验收报告',
						REFID : project.projectID,
						attachmentTypeID : com.bitc.xmis.Attachment.intercheck,
						uploadPanelConfig : {
							listeners : {
								'uploadcomplete' : function() {
									attFileGrid.reload();
								}
							}
						}
					}]
				}).show();

			}
		}, {
			iconCls : 'ok',
			text : '不同意',//验收员审批
			hidden : isHiddenAcceptor,
			ref:'btn_assignornotapproved',
			handler : function() {

				var win = new Ext.Window({
					title : '不同意验收?',
					layout : 'border',
					width : 300,
					height : 170,
					modal : true,
					items : [new Ext.FormPanel({
								ref : 'form',
								region : 'center', //
								layout : 'auto',
								padding : 10,
								items : [{
											xtype : 'displayfield',
											value : '请填写不同意的原因：'
										}, {
											xtype : 'textarea',
											fieldLabel : '备注',
											width : 200,
											id : 'myComment',
											name : 'text'
										}]
							})],
					tbar : [{
						iconCls : 'ok',
						text : '提交',
						handler : function() {
							var text = win.form.getForm().findField('text')
									.getValue();
							if (text == '' || text.trim() == '') {
								Ext.Msg.alert('提示', '请填写不同意的原因');
								return;
							}
							var url = String
									.format(
											'{0}/projectMain.do?method=tellDeptManager&projectID={1}&checkDeptID={2}&workItemId={3}&applicant={4}&isApprove={5}&checkManagerID={6}',
											__cxtPath, projectID,
											dept.departmentID, workItemId,
											applicant, 'no',a.sender.userID);
							Ext.Ajax.request({
								url : url,
								method : 'post',
								params : {
									comment : '不同意-'+text
								},
								success : function(response, opts) {

									var obj = Ext.decode(response.responseText);
									Ext.Msg
											.alert(obj.messageTitle,
													obj.message);
									if (obj.success) {
										parent.xMis.refresh('mytodo','myworkitemnewtodo');
										win.close();
										center.getTopToolbar().btn_assignorapproved.setDisabled(true);
										center.getTopToolbar().btn_assignornotapproved.setDisabled(true);
									} else {

									}

								},
								failure : function(response, opts) {
									Ext.Msg.alert('拒收失败', '服务器错误，状态码：'
													+ response.status);
								}
							});

						}
					}, {
						xtype : 'uploadButton',
						text : '上传验收报告',
						REFID : project.projectID,
						attachmentTypeID : com.bitc.xmis.Attachment.intercheck,
						uploadPanelConfig : {
							listeners : {
								'uploadcomplete' : function() {
									attFileGrid.reload();
								}
							}
						}
					}]
				}).show();
			}
		}, {
			iconCls : 'ok',
			text : '同意',// (验收部门主任)
			hidden : isHiddenCheckDept,
			ref:'btn_telldivisionyes',
			handler : function() {
						Ext.Msg.prompt('确定允许通过内部验收?', '请填写备注：', function(btn, text) {
							if (btn == 'ok') {
								Ext.Ajax.request({
									url : String
											.format(
													'{0}/projectMain.do?method=tellDivisionDeptManager&projectID={1}&checkDeptID={2}&workItemId={3}&applicant={4}&isApprove={5}',
													__cxtPath, projectID,
													dept.departmentID,
													workItemId, applicant,
													'yes'),
									method : 'post',
									params : {
										comment : '同意-'+text
									},
									success : function(response) {
										var result = Ext
												.decode(response.responseText);
										if (result.success) {
											Ext.Msg.alert(result.messageTitle,
													result.message, function() {
														parent.xMis.refresh('mytodo','myworkitemnewtodo');
														center.getTopToolbar().btn_telldivisionyes.setDisabled(true);
														center.getTopToolbar().btn_telldivisionno.setDisabled(true);
													});
										} else {
											Ext.Msg.alert(result.messageTitle,
													result.message)
										}
									},
									failure : function(response) {
										Ext.Msg.alert('失败', '操作失败');
									}
								});
							}
				},this,true);
			}
		}, {
			iconCls : 'ok',
			text : '不同意',//(验收部门主任)
			hidden : isHiddenCheckDept,
			ref:'btn_telldivisionno',
			handler : function() {
				Ext.Msg.prompt('提示', '请填写不同意的原因：', function(btn, text) {
					if (btn == 'ok') {
						if (text == '' || text.trim() == '') {
							Ext.Msg.alert('提示', '请填写不同意的原因');
							return;
						}
						Ext.Ajax.request({
							url : String
									.format(
											'{0}/projectMain.do?method=tellDivisionDeptManager&projectID={1}&checkDeptID={2}&workItemId={3}&applicant={4}&isApprove={5}',
											__cxtPath, projectID,
											dept.departmentID, workItemId,
											applicant, 'no', text),
							method : 'post',
							params : {
								comment : '不同意-'+text
							},
							success : function(response) {
								var result = Ext.decode(response.responseText);
								if (result.success) {
									Ext.Msg.alert(result.messageTitle,
											result.message, function() {
												parent.xMis.refresh('mytodo','myworkitemnewtodo');
												center.getTopToolbar().btn_telldivisionyes.setDisabled(true);
												center.getTopToolbar().btn_telldivisionno.setDisabled(true);												
											});
								} else {
									Ext.Msg.alert(result.messageTitle,
											result.message)
								}
							},
							failure : function(response) {
								Ext.Msg.alert('失败', '操作失败');
							}
						});
					}
				}, this, true);
			}
		}, {
			iconCls : 'ok',
			text : '申请内部验收',//继续申请内部验收
			ref:'btn_goonask',
			hidden : isHiddenAcceptorGoOn,
			handler : function() {

				var wind = new Ext.Window({
					title : '验收部门',
					layout : 'border',
					closable : false,
					width : 400,
					height : 240,
					modal : true,
					items : [new Ext.FormPanel({
						ref : 'form',
						region : 'center',
						border : false,
						labelAlign : 'right',
						bodyStyle : 'padding:10px;',
						items : [{
										xtype : 'hidden',
										name : 'checkDept'
									}, new com.bitc.xmis.OrgTreeChooseField({
										name : 'checkDeptName',
										width : 200,
										fieldLabel : '选择验收部门',
										setValueFun : function(nodes) {
											this
													.setValue(nodes[0].attributes.attributes.name);
											wind.form
													.getForm()
													.findField('checkDept')
													.setValue(nodes[0].attributes.attributes.id);
										},
										clearValueFun : function(nodes) {
											this.setValue('');
											wind.form.getForm()
													.findField('checkDept')
													.setValue('');
										},
										winConfig : {
											width : 350,
											height : 450
										},
										orgTreePanelConfig : {
											title : '选择验收部门',
											rootFilter : '1',
											checkable : true,
											singleCheck : true,
											rootId : __emp.companyID,
											rootLevel : com.bitc.xmis.OrgTree.LEVEL.COMP,
											leafLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT2,
											autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1
										}
									}),{
							xtype : 'textarea',
							fieldLabel : '备&nbsp;&nbsp;&nbsp;&nbsp;注',
							width : 200,
							name : 'myComment'
						}]
					})],
					tbar : [{
						iconCls : 'ok',
						text : '提交',
						handler : function() {
							var f = wind.form.getForm();
							f.submit({
								url : String
										.format(
												'{0}/projectMain.do?method=goOnInterCheckToEngineer',
												__cxtPath),// 
								method : 'post',
								params : {
									checkDeptID : f.findField('checkDept')
											.getValue(),
									projectID : project.projectID,
									workItemId : workItemId,
									comment:wind.form.getForm().findField('myComment').getValue()
								},
								waitMsg : '正在提交，请稍候……',
								waitTitle : '正在处理',
								scope : this,
								success : function(form, act) {
									wind.hide();
									Ext.Msg.alert(act.result.messageTitle,
											act.result.message);
									parent.xMis.refresh('mytodo','myworkitemnewtodo');
									center.getTopToolbar().btn_goonask.setDisabled(true);
								},
								failure : function(form, act) {
									com.bitc.xmis.util.AlertUtil
											.formSubmitFailedAlert(act);
								}
							});
						}
					}, '-', {
						xtype : 'uploadButton',
						text : '上传内部验收文档',
						REFID : project.projectID,
						attachmentTypeID : com.bitc.xmis.Attachment.intercheck,
						uploadPanelConfig : {
							listeners : {
								'uploadcomplete' : function() {
									attFileGrid.reload();
								}
							}
						}
					}, '-', {
						text : '取消',
						handler : function() {
							wind.hide();
						}
					}]
				}).show();
				// 设置初始值为科技质量部
				wind.form.getForm().findField('checkDept').setValue('11');
				wind.form.getForm().findField('checkDeptName')
						.setValue('科技质量部');

			}
		}]
	});

	if (request.getParameter("todoId") != null) {
		center.insert(0, new com.bitc.xmis.TodoInfoPanel({
							todoId : request.getParameter("todoId"),
							frame : true
						}));
	}
	var step = request.getParameter("processTaskId");
	if (step != null) {
		var processInstanceId = request.getParameter("processInstanceId");
		center.getTopToolbar().add(new com.bitc.xmis.workflow.TraceButton({
					processInstanceId : processInstanceId
				}));
	}
	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [center]
			});
});