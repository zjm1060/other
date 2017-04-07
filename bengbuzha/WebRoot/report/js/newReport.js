Ext.onReady(function() {
	var sqlForm = new Ext.FormPanel({
				renderTo : 'reportForm',
				url : String
						.format('{0}/report.do?method=newReport', __cxtPath),
				frame : true,
				width : '400',
				defaultType : 'textfield',
				items : [new Ext.form.ComboBox({
							allowBlank : false,
							name : 'dbN',
							hiddenName : 'dbName',
							fieldLabel : '数据库',
							displayField : 'dname',
							valueField : 'name',
							triggerAction : 'all',
							emptyText : '选择已注册的数据库',
							editable : false,
							store : new Ext.data.JsonStore({
										fields : ['name', 'dname'],
										url : String
												.format(
														'{0}/report.do?method=getRegisterDB',
														__cxtPath)
									})
						}), {
					fieldLabel : '报表标识',
					name : 'name',
					emptyText : '输入报表的唯一标识',
					allowBlank : false
				}, {
					fieldLabel : '报表标题',
					allowBlank : false,
					name : 'title'
				}, {
					xtype : 'textarea',
					fieldLabel : 'SQL语句',
					name : 'sql',
					allowBlank : false,
					anchor : '90%',
					height : '200'
				}],
				buttons : [{
					text : '提交',
					handler : function() {
						sqlForm.getForm().submit({
							waitMsg : '正在处理……',
							waitTitle : '请稍候',
							success : function(form, action) {
								Ext.Msg.alert(action.result.messageTitle,
										action.result.message, function() {
											action.result.attributes.reportId;
											action.result.attributes.reportName;
											location.href = String
													.format(
															'{0}/report.do?reportToken={1}',
															__cxtPath,
															reportName);
										});
							},
							failure : function(form, action) {
							}
						});
					}
				}],
				buttonAlign : 'center'
			});
});