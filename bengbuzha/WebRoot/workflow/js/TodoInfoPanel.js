Ext.ns('com.bitc.xmis');
com.bitc.xmis.TodoInfoPanel = Ext.extend(Ext.Panel, {
	title : '待办任务信息',
	collapsible : true,
	width : 600,
	constructor : function(config) {
		config = config || {};
		Ext.apply(this, Ext.apply(config, {
							layout : 'table',
							layoutConfig : {
								columns : 2
							}
						}));
		com.bitc.xmis.TodoInfoPanel.superclass.constructor.call(this, config);
	},
	onRender : function(ct, p) {
		if (this.todoId) {
			this.on('afterrender', function(comp) {
				Ext.Ajax.request({
					url : String.format(
							'{0}/workflow.do?method=getTodoInfo&todoId={1}',
							__cxtPath, this.todoId),
					method : 'get',
					scope : this,
					success : function(response) {
						var o = Ext.decode(response.responseText);
						var tpl = new Ext.XTemplate(
								'<table style="width:100%" class="info">',
								'<tr class="tr1"><td class="name">任务名称：</td><td class="value">{text}</td></tr>',
								'<tr class="tr2"><td class="name">任务接收时间：</td><td class="value">{createTime}</td></tr>',
								'<tr class="tr1"><td class="name">任务发送人：</td><td class="value">{[values.sender.userName]}</td></tr>',
								'<tr class="tr2"><td class="name">任务到达时间：</td><td class="value">{sendTime}</td></tr>',
								'</table>');
						tpl.overwrite(this.body, o);
					}
				});
			}, this);
		}
		com.bitc.xmis.TodoInfoPanel.superclass.onRender.call(this, ct, p);
	}
});