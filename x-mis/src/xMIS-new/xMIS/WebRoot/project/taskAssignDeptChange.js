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
									+ project.projectID, 'menu-project-info');
		}
	}
	var projectID = request.getParameter("projectID");
	var processTaskId = request.getParameter("processTaskId");// 接收变更执行部门任务ID
	var isHiddenDept = false;
	var isHiddenManager = false;
	var isReturn = false;
	var isChangeDept = request.getParameter("isChangeDept");
	var isChangeManager = request.getParameter("isChangeManager");
	var isChangeManager = request.getParameter("isChangeManager");
	
	
	if (processTaskId == "ProjectExecutiveDeptChange.NoticeDeptDirector.NoticeDeptDirectorTask") {
		isHiddenManager = true;
	}
	if (processTaskId == "ProjectExecutiveDeptChange.NoticeManager.NoticeManagerTask") {
		isHiddenDept = true;
	}
	if (processTaskId == "ProjectExecutiveDeptChange.ProjectExecutiveDeptChange.ExecutiveDeptChangeTask") {
		isReturn = true;
		if (isChangeManager == "true") {
			isHiddenDept = true;
		}
		if (isChangeDept == "true") {
			isHiddenManager = true;
		}

	}

	var executiveDeptID = request.getParameter("executiveDeptID");// 变更的执行部门ID
	if (executiveDeptID == null) {
		executiveDeptID = '';
	}
	var executiveDeptChangeReason = request
			.getParameter("executiveDeptChangeReason");// 变更的执行部门原因

	var managerID = request.getParameter("managerID");// 变更的项目经理ID
	if (managerID == null) {
		managerID = '';
	}
	var managerChangeReason = request.getParameter("managerChangeReason");// 变更项目经理的原因
	var workItemId = request.getParameter("workItemId");
	var center = new Ext.Panel({
		border : false,
		padding : 5,
		region : 'center',
		items : [{
			xtype : 'panel',
			title : '变更信息',
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
						fieldLabel : '当前的部门',
						hidden : isHiddenDept,
						value : project.executiveDept != null
								? project.executiveDept.departmentName
								: ''
					}, {
						fieldLabel : '变更后的部门',
						hidden : isHiddenDept,
						value : dept != null ? dept.departmentName : ''
					}, {
						fieldLabel : '变更部门原因',
						hidden : isHiddenDept,
						value : executiveDeptChangeReason
					}, {
						fieldLabel : '当前的项目经理',
						hidden : isHiddenManager,
						value : project.manager != null
								? project.manager.userName
								: '',
						initEvents : function() {
							this
									.mon(this.el, 'dblclick', this.onDblClick,
											this);
						},
						onDblClick : function() {
							var oldManagerID = project.manager != null
									? project.manager.userID
									: '';
							new com.bitc.xmis.util.EmpInfoWin(oldManagerID,
									'详细信息');
						},
						listeners : {
							'render' : function(c) {
								Ext.QuickTips.register({
											target : c.getEl(),
											text : '双击姓名可显示详细信息'
										});
							}
						}
					}, {
						fieldLabel : '变更后项目经理',
						hidden : isHiddenManager,
						value : manager != null ? manager.userName : '',
						initEvents : function() {
							this
									.mon(this.el, 'dblclick', this.onDblClick,
											this);
						},
						onDblClick : function() {
							var newManagerID = manager != null
									? manager.userID
									: '';
							new com.bitc.xmis.util.EmpInfoWin(newManagerID,
									'详细信息');
						},
						listeners : {
							'render' : function(c) {
								Ext.QuickTips.register({
											target : c.getEl(),
											text : '双击姓名可显示详细信息'
										});
							}
						}
					}, {
						fieldLabel : '变更项目经理原因',
						hidden : isHiddenManager,
						value : managerChangeReason
					}]
		}],
		tbar : [{
			iconCls : 'ok',
			text : '任务确定',
			// tooltip : '接收变更执行部门通知',
			hidden : isReturn,
			handler : function() {
				Ext.Ajax.request({
					url : String
							.format(
									'{0}/projectMain.do?method=getExecutiveDeptChangeNotice&workItemId={1}&projectID={2}&executiveDeptID={3}&executiveDeptChangeReason={4}'
											+ '&managerID={5}&managerChangeReason={6}',
									__cxtPath, workItemId, projectID,
									executiveDeptID, executiveDeptChangeReason,
									managerID, managerChangeReason),
					success : function(response) {
						var result = Ext.decode(response.responseText);
						if (result.success) {
							Ext.Msg.alert(result.messageTitle, result.message,
									function() {
										parent.xMis.refresh('mytodo','myworkitemnewtodo');;
										parent.xMis.closeTab(window);
									});
						} else {
							Ext.Msg.alert(result.messageTitle, result.message)
						}
					},
					failure : function(response) {
						Ext.Msg.alert('失败', '操作失败');
					}
				});
			}
		}, {
			iconCls : 'ok',
			text : '修改后提交',
			hidden : !isReturn,
			handler : function() {
				var win = new Ext.Window({
					title : '变更执行部门/项目经理',
					layout : 'border',
					width : 400,
					height : 400,
					modal : true,
					items : [new Ext.FormPanel({
						ref : 'form',
						region : 'center',
						border : false,
						labelAlign : 'right',
						bodyStyle : 'padding:10px;',
						items : [{
							xtype : 'fieldset',
							title : '变更执行部门',
							hidden : isHiddenDept,
							items : [{
								xtype : 'displayfield',
								fieldLabel : '当前执行部门',
								value : project.executiveDept != null
										? project.executiveDept.departmentName
										: ''
							}, {
								xtype : 'hidden',
								name : 'executiveDept'
							}, new com.bitc.xmis.OrgTreeChooseField({
								name : 'executiveDeptName',
								width : 200,
								fieldLabel : '变更为',
								setValueFun : function(nodes) {
									this
											.setValue(nodes[0].attributes.attributes.name);
									win.form
											.getForm()
											.findField('executiveDept')
											.setValue(nodes[0].attributes.attributes.id);
								},
								clearValueFun : function(nodes) {
									this.setValue('');
									win.form.getForm()
											.findField('executiveDept')
											.setValue('');
								},
								winConfig : {
									width : 400,
									height : 450
								},
								orgTreePanelConfig : {
									title : '选择执行部门',
									rootFilter : '1',
									checkable : true,
									singleCheck : true,
									rootId : __emp.companyID,
									rootLevel : com.bitc.xmis.OrgTree.LEVEL.COMP,
									leafLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT2,
									autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1
								}
							}), {
								xtype : 'textarea',
								fieldLabel : '变更原因',
								width : 200,
								name : 'executiveDeptChangeReason'
							}]
						}, {
							xtype : 'fieldset',
							title : '变更项目经理',
							hidden : isHiddenManager,
							items : [{
								xtype : 'displayfield',
								fieldLabel : '当前项目经理',
								value : project.manager != null
										? project.manager.userName
										: ''
							}, {
								xtype : 'hidden',
								name : 'manager'
							}, new com.bitc.xmis.OrgTreeChooseField({
								name : 'managerName',
								width : 200,
								fieldLabel : '变更为',
								setValueFun : function(nodes) {
									this
											.setValue(nodes[0].attributes.attributes.name);
									win.form
											.getForm()
											.findField('manager')
											.setValue(nodes[0].attributes.attributes.id);
								},
								clearValueFun : function(nodes) {
									this.setValue('');
									win.form.getForm().findField('manager')
											.setValue('');
								},
								winConfig : {
									width : 400,
									height : 450
								},
								orgTreePanelConfig : {
									title : '选择项目经理',
									rootFilter : '1',
									checkable : true,
									singleCheck : true,
									rootId : __emp.companyID,
									rootLevel : com.bitc.xmis.OrgTree.LEVEL.COMP,
									leafLevel : com.bitc.xmis.OrgTree.LEVEL.EMP,
									autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT2
								}
							}), {
								xtype : 'textarea',
								fieldLabel : '变更原因',
								width : 200,
								name : 'managerChangeReason'
							}]
						}]
					})],
					tbar : [{
						iconCls : 'ok',
						text : '提交',
						handler : function() {
							var oldDeptID = project.executiveDept.departmentID != null
									? project.executiveDept.departmentID
									: '';
							var oldManagerID = project.manager.userID != null
									? project.manager.userID
									: '';

							var newDeptID = win.form.getForm()
									.findField('executiveDept').getValue();
							var newManagerID = win.form.getForm()
									.findField('manager').getValue();

							if (newDeptID == '' && newManagerID == '') {
								Ext.Msg.alert('提示', '您没有修改，无需提交。');
								return;
							} else if (oldDeptID == newDeptID
									&& newDeptID != '') {
								Ext.Msg.alert('提示', '执行部门与当前相同，不需修改。');
								return;
							} else if (oldManagerID == newManagerID
									&& newManagerID != '') {
								Ext.Msg.alert('提示', '项目经理与当前相同，不需修改。');
								return;
							}
							var f = win.form.getForm();
							f.submit({
								url : String
										.format(
												'{0}/projectMain.do?method=updateProjectExecutiveDeptByReturn',
												__cxtPath),// 提交变更执行部门
								method : 'post',
								params : {
									projectID : project.projectID,
									executiveDeptID : win.form.getForm()
											.findField('executiveDept')
											.getValue(),
									executiveDeptChangeReason : win.form
											.getForm()
											.findField('executiveDeptChangeReason')
											.getValue(),
									managerChangeReason : win.form.getForm()
											.findField('managerChangeReason')
											.getValue(),
									managerID : win.form.getForm()
											.findField('manager').getValue(),
									workItemId : workItemId
								},
								waitMsg : '正在提交，请稍候……',
								waitTitle : '正在处理',
								scope : this,
								success : function(form, act) {
									win.hide();
									Ext.Msg.alert(act.result.messageTitle,
											act.result.message, function() {
												parent.xMis.refresh('mytodo','myworkitemnewtodo');;
												parent.xMis.closeTab(window);
											});
								},
								failure : function(form, act) {
									com.bitc.xmis.util.AlertUtil
											.formSubmitFailedAlert(act);
								}
							});
						}
					}]
				}).show();
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