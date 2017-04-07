Ext.onReady(function() {
	Ext.QuickTips.init();
	var projectId = reuqest.getParameter('projectId');
	var projectName = reuqest.getParameter('projectName');
	var projectCode = reuqest.getParameter('projectCode');

	var projectScheduleEditPanel = new com.bitc.xmis.project.ProjectScheduleEditPanel(
			{
				projectId : projectId,
				border : false
			});
	var schedulePlanGrid = projectScheduleEditPanel.schedulePlanGrid;
	var defaultStatusGrid = projectScheduleEditPanel.defaultStatusGrid;
	var centerPanel = new Ext.form.FormPanel({
				border : false,
				region : 'center',
				autoScroll : true,
				padding : 10,
				defaultType : 'panel',
				items : [{
					xtype : 'fieldset',
					width : 800,
					title : '项目信息',
					items : [{
								xtype : 'hidden',
								name : 'projectId',
								value : projectId
							}, {
								xtype : 'displayfield',
								fieldLabel : '项目编码',
								name : 'projectCode',
								value : projectCode,
								clickAction : function() {
									parent.xMis.turn('project-info-'
													+ projectId, projectName,
											'projectMain.do?method=getProjectInfo&projectID='
													+ projectId,
											'menu-project-info');
								}
							}, {
								xtype : 'displayfield',
								name : 'projectName',
								fieldLabel : '项目名称',
								value : projectName,
								clickAction : function() {
									parent.xMis.turn('project-info-'
													+ projectId, projectName,
											'projectMain.do?method=getProjectInfo&projectID='
													+ projectId,
											'menu-project-info');
								}
							}]
				}, {
					xtype : 'fieldset',
					title : '进度计划',
					layout : 'auto',
					width : 1000,
					items : [projectScheduleEditPanel]
				}],
				tbar : [{
							text : '保存',
							iconCls : 'ok',
							handler : function() {
								projectScheduleEditPanel.savePlan();
							}
						}]
			});
	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [centerPanel]
			});
	projectScheduleEditPanel.initDragDrop();
});