Ext.onReady(function() {
	var task = {};
	for (var i = 0; i < projectTask.members.length; i++) {
		var m = projectTask.members[i];
		if (__emp.userID == m.member.userID) {
			task = m;
		}
	}
	function linkToProject() {
		if (parent && parent.xMis) {
			parent.xMis.turn('project-info-' + project.projectID, '项目信息-' + project.projectName, __cxtPath
							+ '/projectMain.do?method=getProjectInfo&projectID=' + project.projectID,
					'menu-project-info');
		}
	}
	var center = new Ext.Panel({
				border : false,
				padding : 5,
				region : 'center',
				items : [{
							xtype : 'panel',
							title : '任务信息',
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
									}, {
										fieldLabel : '任务名称',
										value : projectTask.taskName
									}, {
										fieldLabel : '任务说明',
										value : task.taskDesc
									}]
						}],
				tbar : [{
					iconCls : 'ok',
					text : '接收任务',
					handler : function() {
						Ext.Ajax.request({
									url : String
											.format(
													'{0}/projectMain.do?method=acceptProjectTask&workItemId={1}&projectTaskId={2}',
													__cxtPath, request.getParameter('workItemId'), request
															.getParameter("projectTaskId")),
									success : function(response) {
										var result = Ext.decode(response.responseText);
										if (result.success) {
											Ext.Msg.alert(result.messageTitle, result.message, function() {
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