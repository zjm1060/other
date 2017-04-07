Ext.onReady(function() {
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

	var taskPanel = com.bitc.xmis.ProjectTaskPanel.buildGrid({
				autoHeight : true,
				projectId : project.projectID
			});
	var center = new Ext.Panel({
		border : false,
		padding : 5,
		region : 'center',
		items : [{
					xtype : 'panel',
					title : '项目信息',
					layout : 'form',
					defaultType : 'displayfield',
					labelAlign : 'right',
					items : [{
								fieldLabel : '项目编码',
								value : project.projectCode,
								clickAction : linkToProject
							}, {
								fieldLabel : '项目名称',
								value : project.projectName,
								clickAction : linkToProject
							}]
				}, {
					xtype : 'panel',
					border : false,
					defaultType : 'displayfield',
					title : '项目任务',
					items : [taskPanel]
				}],
		tbar : [{
			iconCls : 'ok',
			text : '任务确定',
			ref : 'btn_accepttask',
			handler : function() {
				Ext.Ajax.request({
					url : String
							.format(
									'{0}/projectMain.do?method=acceptProjectTask&workItemId={1}&projectTaskId={2}',
									__cxtPath, request
											.getParameter('workItemId'),
									request.getParameter("projectTaskId")),
					success : function(response) {
						var result = Ext.decode(response.responseText);
						if (result.success) {
							Ext.Msg.alert(result.messageTitle, result.message,
									function() {
										parent.xMis.refresh('mytodo','myworkitemnewtodo');;
										// parent.xMis.closeTab(window);
										center.getTopToolbar().btn_accepttask
												.setDisabled(true);
										// 项目经理确认任务步骤中，用户
										// 有“添加项目成员”权限，并且项目成员数目为<=0,
										// 跳转到项目信息页面，弹出“添加项目成员”窗口
										if (request.getParameter("processTaskId") == "StartProject.Manager.confirm"
												&& !__emp.readOnly
												&& taskPanel.store.data.length <= 1) {
											Ext.MessageBox.confirm("提示",
													"是否添加项目成员?", function(btn) {
														if (btn == 'yes') {
															if (parent
																	&& parent.xMis) {
																parent.xMis
																		.turn(
																				'project-info-'
																						+ project.projectID,
																				'项目信息-'
																						+ project.projectName,
																				__cxtPath
																						+ '/projectMain.do?method=getProjectInfo&projectID='
																						+ project.projectID
																						+ '&addMember=true',
																				'menu-project-info');
															}
														}
													});

										}
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