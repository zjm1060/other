Ext.onReady(function() {
	Ext.QuickTips.init();
	var searchPanel;
	var attKzhbRecord = Ext.data.Record.create([{
				name : 'id',
				mapping : '附件ID'
			}, {
				name : 'fileName',
				mapping : '文件名'
			}, {
				name : 'desc',
				mapping : '内容描述'
			}, {
				name : 'upload_date'
			}, {
				name : 'uploader_id'
			}, {
				name : 'uploader_name'
			}, {
				name : 'keyWords',
				mapping : '关键字'
			}]);

	var attKzhbReader = new Ext.data.JsonReader({
				idProperty : 'id',
				root : 'rows',
				totalProperty : 'totalCount'
			}, attKzhbRecord);

	var attKzhbStore = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							// url : __cxtPath
							// +
							// '/fileUpload.do?method=getAttachmentListForKZHB'
							url : __cxtPath
									+ '/kzhbMaint.do?method=getAttchedFilesList'
						}),
				baseParams : {
					limit : 20
				},

				remoteSort : true,
				sortInfo : {
					field : '附件ID',
					direction : 'DESC'
				},
				reader : new Ext.data.JsonReader({
							idProperty : 'id',
							root : 'rows',
							totalProperty : 'totalCount'
						}, attKzhbRecord)
			});

	Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
				renderer : function(value, cellmeta, record, rowIndex,
						columnIndex, store) {
					return store.lastOptions.params.start + rowIndex + 1;
				}
			});

	var action = new Ext.ux.grid.RowActions({
		dataIndex : 'id',
		actions : [{
			iconCls : 'download',
			tooltip : '下载',
			callback : function(grid, record, action, rowIndex, colIndex) {
				location.href = __cxtPath + '/fileUpload.do?type=3&attID='
						+ record.data.id;
			}
		}, {
			iconCls : 'delete',
			tooltip : '删除',
			callback : function(grid, record, action, rowIndex, colIndex) {
				Ext.Msg.confirm('确认', '您确定要删除' + record.data.fileName + '?',
						function(buttonId) {
							if (buttonId == 'yes') {
								Ext.Ajax.request({
									url : __cxtPath
											+ '/fileUpload.do?method=deleteKzhbFileById',
									scope : this,
									success : function() {
										attKzhbStore.reload();
									},
									method : "post",
									params : {
										attachmentID : record.data.id
									}
								});
							}
						}, this);
			}

		}],
		exportFormater : function(value, row, index, pageinfo) {
			return '';
		}

	});

	var attKzhbColM = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(), {
				id : 'attName',
				header : "文件名称",
				dataIndex : 'fileName',
				sortable : true
			}, {
				header : "描述",
				dataIndex : 'desc',
				width : 300
			}, {
				header : "上传日期",
				dataIndex : 'upload_date',
				sortable : true,
				width : 100
			}, {
				header : "上传人",
				dataIndex : 'uploader_name',
				sortable : true,
				width : 100,
				listeners : {
					'dblclick' : function(t, grid, rowIndex, e) {
						var r = grid.getStore().getAt(rowIndex);
						uploader = r.data.uploader_id;
						com.bitc.xmis.util.EmpInfoWin(uploader,'上传人信息');
					}
				}
			}, {
				header : "关键字",
				dataIndex : 'keyWords',
				sortable : true,
				width : 100
			}, action]);

	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : "com.bitc.jk.xmis.export.PagingRowNumber"
						}]
			});

	var kzhbAttUplFrm;
	var winUpl;

	var attKzhbGridPanel = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		autoExpandColumn : 'attName',
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : attKzhbStore,
		cm : attKzhbColM,
		plugins : [action],
		tbar : [{
					ref : '../conditionBtn',
					iconCls : 'condition',
					text : '查询',
					handler : function(b, e) {
						searchPanel.store = attKzhbStore;
						searchConditionWin.x = e.getPageX() + 5;
						searchConditionWin.y = e.getPageY() + 5;
						searchConditionWin.show();
					}
				}, '-', {
					iconCls : "updateAttc",
					text : '文件上传',
					disabled : false,
					handler : function() {
						if (!kzhbAttUplFrm) {
							kzhbAttUplFrm = new Ext.form.FormPanel({
										id : 'invoiceUpload',
										baseCls : 'x-plain',
										labelWidth : 70,
										labelHight : 70,
										fileUpload : true,
										defaultType : 'textfield',
										items : [{
													xtype : 'textfield',
													autoCreate : {
														tag : "input",
														type : "file",
														contenteditable : false
													},
													fieldLabel : '上传文件名',
													id : 'fileName',
													name : 'fileName',
													inputType : 'file',
													contenteditable : false,
													anchor : '100%'
												}, {
													fieldLabel : "文件描述",
													name : 'desc',
													id : 'desc',
													anchor : '100%'
												}, {
													fieldLabel : "关键字",
													name : 'keywords',
													id : 'keywords',
													anchor : '100%'
												}]
									})
						}
						if (!winUpl) {
							winUpl = new Ext.Window({
										modal : true,
										title : '文件上传',
										width : 400,
										height : 150,
										minWidth : 300,
										minHeight : 150,
										layout : 'fit',
										plain : true,
										bodyStyle : 'padding:5px;',
										buttonAlign : 'center',
										items : kzhbAttUplFrm,
										buttons : [{
											text : '上传',
											handler : function() {

												if (kzhbAttUplFrm.form
														.isValid()) {
													if (Ext.getCmp('fileName')
															.getValue() == '') {
														Ext.Msg
																.alert('提示',
																		'您还没有选择文件，请选择...');
														return;

													} else {
														check();
													}
												}
											}
										}, {
											text : '关闭',
											handler : function() {
												winUpl.hide();
											}
										}]
									})
						};
						winUpl.setPosition(400, 200);
						winUpl.show();
					}
				}, '->', '<font color ="blue">*双击上传人姓名可查看详细信息*</font>'],
		bbar : new Ext.PagingToolbar({
					pageSize : 20,
					store : attKzhbStore,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					plugins : [exp, new Ext.ux.PagelimitCombo()]
				})
	});

	attKzhbStore.load({
				params : {
					start : 0,
					limit : 20
				}
			});

	var win = new Ext.Viewport({
				layout : 'border',
				items : [attKzhbGridPanel]
			});

	// 查询

	var searchConditionWin = new Ext.Window({
				title : '查询',
				renderTo : Ext.getBody(),
				frame : true,
				plain : true,
				modal : true,
				resizable : true,
				closeAction : "hide",
				closable : true,
				layout : "form",
				bodyStyle : "padding:10px",
				width : 280,
				autoHeight : true,
				listeners : {
					hide : function(e) {
						searchPanel.clear();
					}
				}
			});

	Ext.ux.MultiSearchPanel.create(3, function(panel) {
				searchPanel = panel;
				panel.on('onsearch', function(p) {
							searchConditionWin.hide();
							p.clear();
						});
				searchConditionWin.add(panel);
			}, this);

	attKzhbStore.load({
				params : {
					start : 0,
					limit : 20
				}
			});

	function doUpload() {
		kzhbAttUplFrm.getForm().submit({
			url : __cxtPath + '/fileUpload.do?type='
					+ Upload_Application_KzhbFile_Type + "&desc="
					+ Ext.getCmp('desc').getValue() + "&keywords="
					+ Ext.getCmp('keywords').getValue(),

			method : 'post',
			success : function(form, action) {
				attKzhbStore.reload();
				Ext.Msg.alert(action.result.attributes.messageTitle,
						action.result.attributes.messages);
				kzhbAttUplFrm.getForm().clear();
				winUpl.hide();
			},
			failure : function(form, action) {
				Ext.Msg.alert(action.result.attributes.messageTitle,
						action.result.attributes.messages);

			}
		});

	}

	function doDelete(fileName) {
		Ext.Ajax.request({
					url : String.format('{0}/fileUpload.do?type=10', __cxtPath),
					scope : this,
					success : function() {
						doUpload();
					},
					failure : Ext.emptyFn,
					method : "post",
					params : {
						attachmentTypeID : -1,
						REFID : -1,
						fileName : fileName
					}
				});
	}

	function check() {
		Ext.Ajax.request({
					url : String.format('{0}/fileUpload.do?type=9', __cxtPath),
					scope : this,
					success : function(response) {
						var action = Ext.decode(response.responseText);
						var flag = action.attributes.flag;
						var filename = action.attributes.fileName;

						if (flag) {// 存在
							Ext.Msg.confirm('确认', filename + '已存在,是否覆盖?',
									function(buttonId) {
										if (buttonId == 'yes') {
											doDelete(filename);
										} else {
											kzhbAttUplFrm.getForm().clear();
											winUpl.hide();
										}
									}, this);
						} else {
							doUpload();
						}
					},
					failure : Ext.emptyFn,
					method : "post",
					params : {
						attachmentTypeID : -1,
						REFID : -1,
						fileName : Ext.getCmp('fileName').getValue(),
						type : -1
					}
				});
	}

});
