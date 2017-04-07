Ext.ns("com.bitc.xmis.workflow");
com.bitc.xmis.workflow.ProcessDeployWin = Ext.extend(Ext.Window, {
	/**
	 * @cfg {String} url 文件提交的URL
	 */
	initComponent : function() {
		var infoPanel = this.infoPanel = new Ext.Panel({
					padding : 5,
					baseCls : 'x-plain',
					html : '说明：请使用流程设计器生成流程定义文件（XML）'
				});
		var form = this.form = new Ext.form.FormPanel({
			baseCls : 'x-plain',
			padding : 5,
			labelWidth : 80,
			fileUpload : true,
			layout : 'column',
			items : [{
						xtype : 'panel',
						border : false,
						baseCls : 'x-plain',
						layout : 'form',
						columnWidth : .8,
						items : {
							xtype : 'textfield',
							fieldLabel : '上传文件名',
							name : 'userfile',
							inputType : 'file',
							contenteditable : false,
							allowBlank : false,
							anchor : '95%'
						}
					}, {
						xtype : 'panel',
						border : false,
						baseCls : 'x-plain',
						columnWidth : .2,
						items : {
							xtype : 'button',
							text : '上传',
							width : 80,
							scope : this,
							handler : function() {
								Ext.Msg.wait('正在上传，请稍候......', '上传中');
								form.getForm().submit({
									url : this.url,
									method : 'post',
									scope : this,
									success : function(form, action) {
										Ext.Msg.hide();
										Ext.Msg.alert(
												action.result.messageTitle,
												action.result.message,
												function() {
													this.form.getForm().reset();
													this.hide();
												}, this);
										this.fireEvent('uploadsuccess', this,
												action);
									},
									failure : function(form, action) {
										com.bitc.xmis.util.AlertUtil
												.formSubmitFailedAlert(action);
									}
								});
							}
						}
					}]
		});
		this.items = [form, infoPanel];

		com.bitc.xmis.workflow.ProcessDeployWin.superclass.initComponent
				.call(this);
		this.addEvents('uploadsuccess');
	}
});
Ext.onReady(function() {
	var importWin = new com.bitc.xmis.workflow.ProcessDeployWin({
				width : 500,
				height : 120,
				modal : true,
				title : '上传流程定义文件',
				closeAction : 'hide',
				url : String.format('{0}/workflow.do?method=deployProcess',
						__cxtPath),
				listeners : {
					'uploadsuccess' : function() {
						// devicePanel.reload();
						center.store.reload();
					}
				}
			});
	var autoExId = Ext.id();
	var center = new Ext.grid.GridPanel({
				autoExpandColumn : autoExId,
				stripeRows : true,
				border : false,
				region : 'center',
				store : new Ext.data.JsonStore({
							method : 'post',
							url : __cxtPath
									+ '/workflow.do?method=getDeployedProcessList',
							root : 'rows',
							fields : ["id", "processId", "name", "displayName",
									"description", {
										"mapping" : "version",
										"name" : "version",
										"type" : "int"
									}, {
										"mapping" : "state",
										"name" : "state",
										"type" : "boolean"
									}, "uploadUser", {
										"dateFormat" : "Y-m-d H:i:s",
										"mapping" : "uploadTime",
										"name" : "uploadTime",
										"type" : "date"
									}, "publishUser", {
										"dateFormat" : "Y-m-d H:i:s",
										"mapping" : "publishTime",
										"name" : "publishTime",
										"type" : "date"
									}, "definitionType"]
						}),
				columns : [{
							header : '流程ID',
							width : 200,
							dataIndex : 'processId'
						}, {
							header : '流程名称',
							dataIndex : 'displayName'
						}, {
							header : '当前版本',
							dataIndex : 'version',
							align : 'right'
						}, {
							xtype : 'datecolumn',
							header : '发布时间',
							width : 180,
							dataIndex : 'publishTime',
							format : 'Y-m-d H:i:s',
							align : 'center'
						}, {
							id : autoExId,
							header : '描述',
							dataIndex : 'description'
						}],
				tbar : [{
							iconCls : 'add',
							text : '提交新流程',
							handler : function() {
								importWin.show();
							}
						}, {
							iconCls : 'refresh',
							text : '刷新',
							handler : function() {
								center.store.reload();
							}
						}]
			});
	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [center]
			})
	center.store.load();
});