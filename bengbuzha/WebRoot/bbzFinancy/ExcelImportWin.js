Ext.ns("com.bitc.xmis");
com.bitc.xmis.ExcelImportWin = Ext.extend(Ext.Window, {
	/**
	 * @cfg {String} url 文件提交的URL
	 */
	
	
	initComponent : function() {
		var url = this.url;
		var infoPanel = this.infoPanel = new Ext.Panel({
			padding : 5,
			baseCls : 'x-plain',
			html : String.format('说明：请<a href="{0}/bbzFinancyMain.do?method=downloadTemplateFile" class="red-td-data">'
				+ '点击这里下载模板文件</a>' + '（不要使用迅雷等下载工具下载），' + '保存为<span class="red-td-data">Excel97-2003格式</span>的xls文件，'
				+ '<span class="red-td-data">关闭Excel后</span>将文件上传。', __cxtPath)
				+ "</br>上传的工资文件中必须含有“姓名”及“实发”（或者“实发金额”或者“实发工资”）列。"
		});
		var form = this.form = new Ext.form.FormPanel({
			baseCls : 'x-plain',
			padding : 5,
			labelWidth : 80,
			fileUpload : true,
			layout : 'column',
			items : [
				{
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
				},
				{
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

							var path = form.getForm().findField('userfile').getValue();
							var fileName = path.substr(path.lastIndexOf('\\') + 1, path.length);
							Ext.Ajax.request({
								url : __cxtPath + '/bbzFinancyMain.do?method=existValidate',
								scope : this,
								params : {
									type : 3,
									fileName : fileName,
									REFID : this.REFID,
									attachmentTypeID : this.attachmentTypeID
								},
								success : function(response) {
									var action = Ext.decode(response.responseText);
									var flag = action.attributes.flag;
									var attID = action.attributes.attID;
									var fileName = action.attributes.fileName;
									if (flag) {// 存在
										Ext.Msg.confirm('确认', fileName + '已存在，继续操作将删除已有的' + fileName
											+ '文件及其已导入的工作表内容，是否继续?', function(buttonId) {
											if (buttonId == 'yes') {
												Ext.Ajax.request({
													url : __cxtPath + '/bbzFinancyMain.do?method=delSheetInfoByAttID',
													method : 'post',
													params : {
														attID : attID
													},
													scope : this,
													success : function(response, action) {

														Ext.Msg.wait('正在上传，请稍候......', '上传中');
														form.getForm().submit(
															{
																url : url,
																method : 'post',
																scope : this,
																success : function(form, action) {
																	Ext.Msg.hide();
																	Ext.Msg.alert(action.result.messageTitle,
																		action.result.message, function() {
																			form.reset();
																		}, this);

																	this.fireEvent('uploadsuccess', this);
																},
																failure : function(form, action) {
																	com.bitc.xmis.util.AlertUtil
																			.formSubmitFailedAlert(action);
																}
															});

													},
													failure : function(response, action) {
														Ext.MessageBox.hide();
														Ext.MessageBox.alert("提示", "操作失败！");
													}
												});

											} else {
											}
										}, this);
									} else {// 不存在
										Ext.Msg.wait('正在上传，请稍候......', '上传中');
										form.getForm().submit(
											{
												url : url,
												method : 'post',
												scope : this,
												success : function(form, action) {
													Ext.Msg.hide();
													Ext.Msg.alert(action.result.messageTitle, action.result.message,
														function() {
															this.form.getForm().reset();
															this.hide();
														}, this);
													this.fireEvent('uploadsuccess', this);
												},
												failure : function(form, action) {
													com.bitc.xmis.util.AlertUtil.formSubmitFailedAlert(action);
												}
											});
									}
								},
								failure : Ext.emptyFn,
								method : "post"
							});
						}
					}
				} ]
		});
		this.items = [ form, infoPanel ];

		com.bitc.xmis.ExcelImportWin.superclass.initComponent.call(this);
		this.addEvents('uploadsuccess');
	}
});