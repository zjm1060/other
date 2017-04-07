Ext.ns("com.bitc.xmis");
com.bitc.xmis.DeviceImportWin = Ext.extend(Ext.Window, {
	/**
	 * @cfg {String} url 文件提交的URL
	 */
	initComponent : function() {
		var url = this.url;
		var infoPanel = this.infoPanel = new Ext.Panel({
			padding : 5,
			baseCls : 'x-plain',
			html : String
					.format(
							'说明：请<a href="{0}/projectMain.do?method=downloadTemplateFile" class="red-td-data">'
									+ '点击这里下载模板文件</a>'
									+ '（不要使用迅雷等下载工具下载），填写本项目的设备清单，'
									+ '保存为<span class="red-td-data">Excel97-2003格式</span>的xls文件，'
									+ '<span class="red-td-data">关闭Excel后</span>将文件上传。',
							__cxtPath)
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
								var path = form.getForm().findField('userfile')
										.getValue();
								var fileName = path.substr(path
												.lastIndexOf('\\')
												+ 1, path.length);
								Ext.Ajax.request({
									url : String.format(
											'{0}/importExcel.do?type=3',// 判断重复
											__cxtPath),
									scope : this,
									success : function(response) {
										var action = Ext
												.decode(response.responseText);
										var flag = action.attributes.flag;
										var attID = action.attributes.attID;
										var fileName = action.attributes.fileName;
										if (flag) {// 存在
											Ext.Msg
													.confirm(
															'确认',
															fileName
																	+ '已存在，继续操作将删除已有的'
																	+ fileName
																	+ '文件及其已导入的工作表内容，是否继续?',
															function(buttonId) {
																if (buttonId == 'yes') {
																	// 1.删除已导入的sheet内容 ；导入内容；删除原有的文件

																	Ext.Ajax
																			.request(
																					{
																						url : __cxtPath
																								+ '/projectMain.do?method=delSheetInfoByAttID',
																						method : 'post',
																						params : {
																							attID : attID
																						},
																						success : function(
																								response,
																								action) {

																							// 2.导入
																							Ext.Msg
																									.wait(
																											'正在上传，请稍候......',
																											'上传中');
																							form
																									.getForm()
																									.submit(
																											{
																												url : url,
																												method : 'post',
																												scope : this,
																												success : function(
																														form,
																														action) {
																													Ext.Msg
																															.hide();
																													Ext.Msg
																															.alert(
																																	action.result.messageTitle,
																																	action.result.message,
																																	function() {
//																																		this.form
//																																				.getForm()
//																																				.reset();
																																		form.reset();
//																																		this
//																																				.hide();
																																	},
																																	this);
//																													this
//																															.fireEvent(
//																																	'uploadsuccess',
//																																	this,
//																																	action);
																															parent.xMis.refresh('importDeviceGrid');		
																															parent.xMis.refresh('deviceImportPanelGrid');		
																												},
																												failure : function(
																														form,
																														action) {
																													com.bitc.xmis.util.AlertUtil
																															.formSubmitFailedAlert(action);
																												}
																											});

																						},
																						failure : function(
																								response,
																								action) {
																							Ext.MessageBox
																									.hide();
																							Ext.MessageBox
																									.alert(
																											"提示",
																											"操作失败！");
																						}
																					});

																} else {
																	// 关闭提示窗口
																}
															}, this);
										} else {// 不存在
											// 上传
											Ext.Msg.wait('正在上传，请稍候......',
													'上传中');
											form.getForm().submit({
												url : url,
												method : 'post',
												scope : this,
												success : function(form, action) {
													Ext.Msg.hide();
													Ext.Msg
															.alert(
																	action.result.messageTitle,
																	action.result.message,
																	function() {
																		this.form
																				.getForm()
																				.reset();
																		this
																				.hide();
																	}, this);
//													this.fireEvent(
//															'uploadsuccess',
//															this, action);
																	parent.xMis.refresh('importDeviceGrid');
												},
												failure : function(form, action) {
													com.bitc.xmis.util.AlertUtil
															.formSubmitFailedAlert(action);
												}
											});
										}
									},
									failure : Ext.emptyFn,
									method : "post",
									params : {
										fileName : fileName,
										REFID : this.REFID,
										attachmentTypeID : this.attachmentTypeID
									}
								});

							}
						}
					}]
		});
		this.items = [form, infoPanel];

		com.bitc.xmis.DeviceImportWin.superclass.initComponent.call(this);
		this.addEvents('uploadsuccess');
	}
});