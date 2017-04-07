Ext.onReady(function() {
	Ext.QuickTips.init();

	var reRecord = Ext.data.Record.create([{
				"mapping" : "ID",
				"name" : "ID",
				"type" : "string"
			}, {
				"name" : "process_ID",
				"type" : "string"
			}, {
				"name" : "processName",
				"type" : "string"
			}, {
				"name" : "authorizerID",
				"type" : "int"
			}, {
				"name" : "authorizerName",
				"type" : "string"
			}, {
				"name" : "authorizeeID",
				"type" : "int"
			}, {
				"name" : "authorizeeName",
				"type" : "string"
			}, {
				"name" : "createDate",
				"type" : "string"
			}, {
				"name" : "availableDate",
				"type" : "string"
			}, {
				"name" : "cancelDate",
				"type" : "string"
			}, {
				"name" : "flag",
				"type" : "int"
			}, {
				"name" : "results",
				"type" : "string"
			}, {
				"name" : "comment",
				"type" : "string"
			}]);
	// store org_公司
	var store = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/workflow.do?method=getAuthorizeList',
							method : "POST"
						}),
				baseParams : {
					limit : 20
				},
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, reRecord),
				sortInfo : {
					field : 'cancelDate,availableDate',
					direction : 'ASC,DESC'
				},
				remoteSort : true
			});

	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						width : 20
					}),{
				xtype : 'actioncolumn',
				width : 40,
				header : '撤销',
				align : 'center',
				items : [{
                    getClass: function(v, meta, rec) {          // Or return a class from a function
                    var a = rec.get('cancelDate') ;
                    	if ((rec.get('authorizerID') == __emp.userID||isAdmin)&&(rec.get('cancelDate') == null ||rec.get('cancelDate')== '')) {
                            this.items[0].tooltip = '立即撤销授权';
                            return 'deleteIcon';
                        }
                    },
                    handler: function(grid, rowIndex, colIndex) {
						var rec = store.getAt(rowIndex);
						Ext.MessageBox.confirm("确定", "是否撤销此条授权信息？", function(
								btn) {
							if (btn == 'yes') {
								Ext.Ajax.request({
									url : __cxtPath
											+ '/workflow.do?method=cancelAuthorize',
									method : 'post',
									params : {
										id : rec.get('ID')
									},
									success : function(response, action) {
										var result = Ext
												.decode(response.responseText);
										if (result.success) {
											Ext.Msg.alert(result.messageTitle,
													result.message);
										} else {
											Ext.Msg.alert(result.messageTitle,
													result.message)
										}
										store.reload();
									},
									failure : function(response, action) {
										Ext.MessageBox.alert("提示", "操作失败！");
									}

								});

							}
						});

					}
                }]
			}, {
				header : "ID",
				hidden : true,
				dataIndex : 'ID'
			}, {
				dataIndex : 'processName',
				sortable : true,
				header : '流程名字'
			}, {
				dataIndex : 'authorizerID',
				sortable : true,
				hidden : true,
				header : '授权人ID'
			}, {
				dataIndex : 'authorizerName',
				sortable : true,
				header : '授权人'
			}, {
				dataIndex : 'authorizeeID',
				sortable : true,
				hidden : true,
				header : '被授权人ID'
			}, {
				dataIndex : 'authorizeeName',
				sortable : true,
				header : '被授权人'
			}, {
				dataIndex : 'createDate',
				sortable : true,
				width : 150,
//				renderer : Ext.util.Format.dateRenderer('Y-m-d H:m:s'), 
				header : '创建时间'
			}, {
				dataIndex : 'availableDate',
				sortable : true,
				width : 150,
				header : '生效日期'
			}, {
				dataIndex : 'results',
				sortable : true,
				width : 150,
				header : '授权原因'
			}, {
				dataIndex : 'comment',
				sortable : true,
				header : '备注'
			}, {
				dataIndex : 'cancelDate',
				sortable : true,
				width : 150,
				header : '撤销日期'
			}]);
	// gridpanel org_公司
	var gridPanel = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		loadMask : true,
		title : '已授权信息',
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : store,
		cm : cm,
		sm : new Ext.grid.RowSelectionModel({
					singleSelect : true,
					listeners : {
						rowselect : function(sm, row, rec) {
							if ((rec.get('authorizerID') == __emp.userID)&&(rec.get('cancelDate') == null ||rec.get('cancelDate')== '')|| isAdmin) {
								formpanel.getForm().loadRecord(rec);
								formpanel.getForm().findField('createDate')
										.setValue(rec.get('createDate'));
								formpanel.getForm().findField('availableDate')
										.setValue(rec.get('availableDate'));

							} else {
								formpanel.getForm().reset();
							}

						}
					}
				}),
		tbar : ['模糊查询：', new Ext.ux.InklingSearchField({
							width : 120,
							store : store,
							tooltip : {
								title : '模糊查询',
								text : '您可以输入“流程名字、授权人、被授权人、授权原因、备注 ”中的任意一个关键词进行查询'
							},
							inklings : [{
												name : 'authorizeeName',
												op : 'like',
												type : 'string'
											}, {
												name : 'processName',
												op : 'like',
												type : 'string'
											}, {
												name : 'authorizerName',
												op : 'like',
												type : 'string'
											}, {
												name : 'results',
												op : 'like',
												type : 'string'
											}, {
												name : 'comment',
												op : 'like',
												type : 'string'
											}]
						})],
		bbar : new Ext.PagingToolbar({
					pageSize : 20,
					store : store,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					plugins : [new Ext.ux.PagelimitCombo()]
				})

	});
	var formpanel = new Ext.form.FormPanel({
		region : 'east',
		// frame:true,
		width : 300,
		collapseMode : 'mini',
		title : '授权',
		split : true,
		labelAlign : 'left',
		padding : 20,
		labelWidth : 60,
		tbar : {
			xtype : 'toolbar',
			items : [{
				ref : '../saveBtn',
				xtype : 'button',
				text : '保存',
				iconCls : 'ok',
				handler : function() {
					
					
					if(formpanel.getForm().findField('cancelDate').getValue()!=null &&formpanel.getForm().findField('cancelDate').getValue()!='' ){
						 Ext.MessageBox.alert('提示','已撤销的不允许修改！');
						return;
					}
					if(formpanel.getForm().findField('process_ID').getValue()=='' ||formpanel.getForm().findField('process_ID').getValue()==null){
						 Ext.MessageBox.alert('提示','流程名字不能为空！');
						return;
					}
					
					formpanel.getForm().submit({
						url : __cxtPath + '/workflow.do?method=addAuthorize',
						params : {
							id : formpanel.getForm().findField('ID').getValue()
						},
						success : function(response, action) {
							Ext.MessageBox.hide();
							Ext.MessageBox.alert(action.result.messageTitle,
									action.result.message);
							store.reload();
						},
						failure : function(response, action) {
							com.bitc.xmis.util.AlertUtil
									.formSubmitFailedAlert(action)
						}
					});
				}
			}, {
				ref : '../saveBtn',
				xtype : 'button',
				text : '清空',
				iconCls : 'ok',
				handler : function() {
					formpanel.getForm().reset();
				}
			}]
		},
		items : [{
					xtype : 'hidden',
					name : 'ID',
					fieldLabel : 'ID'
				}, new com.bitc.xmis.Combos.ProcessListCombo({
							fieldLabel : '流程名字',
							width : 150,
							value:'MissAttence',
							hiddenName : 'process_ID'
				}), {
					xtype : 'hidden',
					name : 'authorizerID',
					value:__emp.userID,
					fieldLabel : '授权人ID'// 授权人
				}, new Ext.ux.form.ChooseOrgTreeField({
					fieldLabel : '授权人',
					name : 'authorizerName',
					modal : true,
					allowBlank:false,
					value:__emp.userName,
					hidden:isAdmin?false:true,
					width : 150,
					winConfig : {
						title : '授权人设置',
						bbar : ['<font color="#003366">*双击人员节点，可以查看此人的详细信息</font>']
					},
					singleSelect : true,
					onlyPerson : true,
					getExpandPath : function() {
						return {
							empId : formpanel.getForm()
									.findField('authorizerID').getValue()
						};
					},
					rootConfig : function() {
						return {
							id : 'comp-old-1'
						}
					},
					dataUrl : '/orgMaint.do?method=getOrganizationTree',
					setValueFun : function(result) {

						var empName = '';
						var empID = '';
						for (var i = 0; i < result.length; i++) {
							if (i == result.length - 1) {
								empID = empID + result[i].id;
								empName = empName + result[i].text;
							} else {
								empID = empID + result[i].id + ',';
								empName = empName + result[i].text + ','
							}
						}
						formpanel.getForm().findField('authorizerName')
								.setValue(empName);
						formpanel.getForm().findField('authorizerID')
								.setValue(empID.split('-')[2]);
					},
					clearValueList : ''
				}),{
					xtype : 'hidden',
					name : 'authorizeeID',//清空时候用
					id : 'authorizeeID',
					fieldLabel : '被授权人ID'// 被授权人
				}, new Ext.ux.form.ChooseOrgTreeField({
					fieldLabel : '被授权人',
					name : 'authorizeeName',
					id : 'authorizeeName',//清空时候用
					modal : true,
					allowBlank:false,
					width : 150,
					winConfig : {
						title : '被授权人设置'
//						,
//						bbar : ['<font color="#003366">*双击人员节点，可以查看此人的详细信息</font>']
					},
					singleSelect : true,
					onlyPerson : true,
					getExpandPath : function() {
						return {
							empId : formpanel.getForm()
									.findField('authorizeeID').getValue()
						};
					},
					rootConfig : function() {
						return {
							id :  'comp-old-1'
						}
					},
					dataUrl : '/orgMaint.do?method=getOrganizationTree',
					setValueFun : function(result) {

						var empName = '';
						var empID = '';
						for (var i = 0; i < result.length; i++) {
							if (i == result.length - 1) {
								empID = empID + result[i].id;
								empName = empName + result[i].text;
							} else {
								empID = empID + result[i].id + ',';
								empName = empName + result[i].text + ','
							}
						}
						formpanel.getForm().findField('authorizeeName')
								.setValue(empName);
						formpanel.getForm().findField('authorizeeID')
								.setValue(empID.split('-')[2]);
					},
					clearValueList : 'authorizeeID,authorizeeName'
				}), {
					xtype : 'datefield',
					format : 'Y-m-d',
					name : 'availableDate',
					allowBlank:false,
					fieldLabel : '生效日期',
					editable : false,
					width : 150
				}, {
					xtype : 'textarea',
					name : 'results',
					allowBlank:false,
					width : 150,
					fieldLabel : '授权原因'// 
				}, {
					xtype : 'textarea',
					name : 'comment',
					width : 150,
					fieldLabel : '备注'// 

				}, {
					xtype : 'textfield',
					name : 'createDate',
					fieldLabel : '创建时间',
					editable : false,
					hidden:true,
					width : 150
				}, {
					xtype : 'datefield',
					format : 'Y-m-d',
					name : 'cancelDate',
					fieldLabel : '撤销日期',
					hidden:true,
					width : 150
				}]

	});



	store.load({
				params : {
					start : 0,
					limit : 20
				}
			});
	// main
	new Ext.Viewport({
				layout : 'border',
				items : [gridPanel, formpanel]
			});

});
