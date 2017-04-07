Ext.ns('com.bitc.xmis');
com.bitc.xmis.ProjectTaskPanel = function() {
	return {
		buildGrid : function(config) {
			var taskRecord = Ext.data.Record.create(['taskID', 'taskName',
					'projectID', {
						name : 'department',
						mapping : 'assigedToDept'
					}, 'members']);
			var taskReader = new Ext.data.JsonReader({
						idProperty : 'taskID',
						root : 'rows',
						totalProperty : 'totalCount'
					}, taskRecord);
			var taskStore = new Ext.data.Store({
				url : String
						.format(
								'{0}/projectMain.do?method=getProjectTaskByProjectID&projectID={1}',
								__cxtPath, config.projectId),
				reader : taskReader,
				autoSave : false,
				autoLoad : true
			});
			var tpl = new Ext.XTemplate('<tpl for="members">{values.member.userName}&nbsp;&nbsp;&nbsp;</tpl>');
			var tp2 = new Ext.XTemplate('<tpl for="members">{values.taskDesc}&nbsp;&nbsp;&nbsp;</tpl>');
			var initconifg = Ext.apply(config, {
						store : taskStore,
						autoExpandColumn : '__taskRen',
						columns : [new Ext.grid.RowNumberer(),{
									header : '任务部门',
									dataIndex : 'department',
									renderer:function(value){
										return value?value.departmentName:'';
									},
									width : 180
								}, {
									header : '任务名称',
									dataIndex : 'taskName',
									width : 200
								}, {
									header : '任务人',
									id : '__taskRen',
									dataIndex : 'taskName',
									renderer : function(value, metaData, record) {
										return tpl.apply(record.data);
									}
								}, {
									header : '任务描述',
									id : 'taskDesc',
									dataIndex : 'taskName',
									width:400,
									renderer : function(value, metaData, record) {
										return tp2.apply(record.data);
									}
								}]
					});
			return new Ext.grid.GridPanel(initconifg);
		}
	}
}();