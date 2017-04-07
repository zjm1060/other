/**
 * *2013/10/16 朱传豪修改grid滚动条，调整宽高，加序号
 */
Ext.ns('com.bitc.xmis');
com.bitc.xmis.ProjectInfo = function() {
	var project_contractName = '';
	return {
		buildToolbar : function(devicePanel, taskPanel, attGrid,
				importDeviceGrid, tabProject) {
			var step = com.bitc.xmis.ProjectInfo.request.getParameter("step");
			var workItemId = com.bitc.xmis.ProjectInfo.request
					.getParameter("workItemId");
			var projectID = com.bitc.xmis.ProjectInfo.request
					.getParameter("projectID");

			var bar = new Ext.Toolbar({
						id : 'btn_all',
						enableOverflow : true
					});
			bar.add({
						text : '刷新',
						iconCls : 'refresh',
						tooltip : '刷新本页',
						ref : 'btn_refresh',
						handler : function() {
							location.reload();
						}
					});
			if (step == "StartProject.memberConfirm.confirm"
					|| step == "StartProject.Manager.confirm") {
				bar.add({
					text : '接收任务',
					ref : 'btn_accepttask',
					iconCls : 'ok',
					handler : function() {
						Ext.Ajax.request({
							url : String
									.format(
											'{0}/projectMain.do?method=acceptProjectTask&workItemId={1}',
											__cxtPath, workItemId),
							success : function(response) {
								var result = Ext.decode(response.responseText);
								if (result.success) {
									Ext.Msg.alert(result.messageTitle,
											result.message, function() {
												parent.xMis.refresh('mytodo',
														'myworkitemnewtodo');;
												// parent.xMis.closeTab(window);
												bar.btn_refresh
														.setDisabled(true);
												bar.btn_accepttask
														.setDisabled(true);
											});
								} else {
									Ext.Msg.alert(result.messageTitle,
											result.message)
								}
							},
							failure : function(response) {

							}

						});
					}
				});
			}

			if (__emp.readOnly) {
				return bar;
			}
			var step2Grid = new Ext.grid.EditorGridPanel({
				height : 265,
				title : '从左侧拖动人员到表格中：',
				ddGroup : 'project-org-dd-group',
				store : new Ext.data.JsonStore({
							root : 'data',
							data : {
								data : []
							},
							idProperty : 'userId',
							fields : ['member', 'taskDesc', 'role']
						}),
				autoScroll : true,
				clicksToEdit : 1,
				autoExpandColumn : 'taskDescCol',
				tbar : [{
							text : '删除',
							iconCls : 'remove',
							handler : function() {
								var s = step2Grid.getSelectionModel().selection;
								if (s == null) {
									Ext.Msg.alert('提示', '请选择要删除的记录',
											Ext.Msg.INFO);
								} else {
									step2Grid.store.remove(s.record);
								}
							}
						}],
				columns : [{
							header : '任务人ID',
							hidden : true,
							dataIndex : 'member.userID'
						}, {
							header : '任务人',
							dataIndex : 'member.userName'
						}, {
							id : 'taskDescCol',
							header : '任务描述',
							dataIndex : 'taskDesc',
							editor : new Ext.form.TextArea()
						}, {
							header : '角色',
							dataIndex : 'role',
							renderer : function(value) {
								return ['项目经理', '项目副经理', '项目成员'][value - 1];
							},
							editor : new Ext.form.ComboBox({
										xtype : 'combo',
										displayField : 'text',
										valueField : 'value',
										mode : 'local',
										triggerAction : 'all',
										emptyText : '请选择类型',
										editable : false,
										store : new Ext.data.ArrayStore({
													fields : ['value', 'text'],
													data : [[2, '项目副经理'],
															[3, '项目成员']]
												}),
										width : 200
									})
						}],
				bbar : ['点击任务描述可编辑，点击角色可选择']
			});
			var projectTaskWindow = new Ext.Window({
				title : '添加项目成员-生成项目任务',
				layout : 'border',
				width : 740,
				height : 400,
				modal : true,
				closeAction : 'hide',
				tbar : [{
					iconCls : 'ok',
					text : '提交',
					handler : function() {
						var f = projectTaskWindow.form.getForm();
						if (step2Grid.store.data.length == 0) {
							Ext.Msg.alert('提示', '请选择任务人员。');
							return;
						} else {
							for (var i = 0; i < step2Grid.store.data.length; i++) {
								if (step2Grid.store.getRange()[i].data.taskDesc == ''
										|| step2Grid.store.getRange()[i].data.taskDesc
												.trim() == '') {
									Ext.Msg.alert('提示', '请添加任务描述。');
									return;
								}
							}
						}
						var members = com.bitc.xmis.util.ExtUtil
								.encodeStoreData(step2Grid.store);
						f.submit({
									url : String
											.format(
													'{0}/projectMain.do?method=addProjectTask',
													__cxtPath),
									method : 'post',
									params : {
										projectID : project.projectID,
										members : members
									},
									waitMsg : '正在提交，请稍候……',
									waitTitle : '正在处理',
									scope : this,
									success : function(form, act) {
										taskPanel.getStore().reload();
										Ext.Msg.alert(act.result.messageTitle,
												act.result.message);
										projectTaskWindow.close();
									},
									failure : function(form, act) {
										com.bitc.xmis.util.AlertUtil
												.formSubmitFailedAlert(act);
									}
								});
					}
				}],
				items : [new com.bitc.xmis.OrgTreePanel({
									ref : 'orgtree',
									title : '人员选择',
									region : 'west',
									ddGroup : 'project-org-dd-group',
									enableDrag : true,
									rootId : __emp.companyID,
									rootLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1,
									rootFilter : __emp.divisionID,
									autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1
								}), new Ext.form.FormPanel({
									ref : 'form',
									region : 'center',
									padding : 5,
									labelAlign : 'right',
									labelWidth : 80,
									items : [{
												xtype : 'textarea',
												fieldLabel : '任务名称',
												name : 'taskName',
												allowBlank : false,
												anchor : '95%'
											}, step2Grid]
								})],
				listeners : {
					show : function() {
						if (!this.dragInited) {
							new Ext.dd.DropTarget(step2Grid.getView().scroller,
									{
										ddGroup : 'project-org-dd-group',
										notifyEnter : function(dd, e, data) {
											if (this.overClass) {
												this.el
														.addClass(this.overClass);
											}
											return data.node.leaf
													&& step2Grid.store
															.getById(data.node.attributes.attributes.id) == undefined
													? this.dropAllowed
													: this.dropNotAllowed;
										},
										notifyOver : function(dd, e, data) {
											return data.node.leaf
													&& step2Grid.store
															.getById(data.node.attributes.attributes.id) == undefined
													? this.dropAllowed
													: this.dropNotAllowed;
										},
										notifyDrop : function(ddSource, e, data) {
											var node = data.node;
											// //执行部门列数据=node.parentNode.attributes.attributes.id;
											if (!node.leaf
													|| node.leaf
													&& step2Grid.store
															.getById(data.node.attributes.attributes.id) !== undefined) {
												return false;
											}
											var record = new step2Grid.store.recordType(
													{
														member : {
															userID : node.attributes.attributes.id,
															userName : node.text
														},
														taskDesc : '',
														role : 3
													},
													node.attributes.attributes.id);
											step2Grid.store
													.insert(
															step2Grid.store.data.length,
															record);
											return (true);
										}
									});
							this.dragInited = true;
						}
					}
				}
			});
			bar.add({
						text : '添加项目成员',
						iconCls : 'add',
						handler : function() {
							tabProject.setActiveTab(1);
							projectTaskWindow.form.getForm().reset();
							step2Grid.store.removeAll();// 第二次打开之前清空拖拽grid的数据
							projectTaskWindow.show();
						}
					});
			var addMember = com.bitc.xmis.ProjectInfo.request
					.getParameter('addMember');
			if (addMember) {
				projectTaskWindow.form.getForm().reset();
				step2Grid.store.removeAll();// 第二次打开之前清空拖拽grid的数据
				projectTaskWindow.show();
			}

			var isChangeDept = false;
			var isChangeManager = false;
			bar.add({
				text : '变更执行部门/项目经理',
				iconCls : 'edit',
				handler : function() {
					var win = new Ext.Window({
						title : '变更执行部门/项目经理',
						layout : 'border',
						width : 400,
						height : 400,
						modal : true,
						items : [new Ext.FormPanel({
							ref : 'form',
							region : 'center',
							border : false,
							labelAlign : 'right',
							bodyStyle : 'padding:10px;',
							items : [{
								xtype : 'fieldset',
								title : '变更执行部门',
								items : [{
									xtype : 'displayfield',
									fieldLabel : '当前执行部门',
									value : project.executiveDept.departmentName
								}, {
									xtype : 'hidden',
									name : 'executiveDept'
								}, new com.bitc.xmis.OrgTreeChooseField({
									name : 'executiveDeptName',
									width : 200,
									fieldLabel : '变更为',
									setValueFun : function(nodes) {
										this
												.setValue(nodes[0].attributes.attributes.name);
										win.form
												.getForm()
												.findField('executiveDept')
												.setValue(nodes[0].attributes.attributes.id);
									},
									clearValueFun : function(nodes) {
										this.setValue('');
										win.form.getForm()
												.findField('executiveDept')
												.setValue('');
									},
									winConfig : {
										width : 400,
										height : 450
									},
									orgTreePanelConfig : {
										title : '选择执行部门',
										rootFilter : '1',
										checkable : true,
										singleCheck : true,
										rootId : __emp.companyID,
										rootLevel : com.bitc.xmis.OrgTree.LEVEL.COMP,
										leafLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT2,
										autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1
									}
								}), {
									xtype : 'textarea',
									fieldLabel : '变更原因',
									width : 200,
									name : 'executiveDeptChangeReason'
								}]
							}, {
								xtype : 'fieldset',
								title : '变更项目经理',
								items : [{
											xtype : 'displayfield',
											fieldLabel : '当前项目经理',
											value : project.manager.userName
										}, {
											xtype : 'hidden',
											name : 'manager'
										},
										new com.bitc.xmis.OrgTreeChooseField({
											name : 'managerName',
											width : 200,
											fieldLabel : '变更为',
											setValueFun : function(nodes) {
												this
														.setValue(nodes[0].attributes.attributes.name);
												win.form
														.getForm()
														.findField('manager')
														.setValue(nodes[0].attributes.attributes.id);
											},
											clearValueFun : function(nodes) {
												this.setValue('');
												win.form.getForm()
														.findField('manager')
														.setValue('');
											},
											winConfig : {
												width : 400,
												height : 450
											},
											orgTreePanelConfig : {
												title : '选择项目经理',
												rootFilter : '1',
												checkable : true,
												singleCheck : true,
												rootId : __emp.companyID,
												rootLevel : com.bitc.xmis.OrgTree.LEVEL.COMP,
												leafLevel : com.bitc.xmis.OrgTree.LEVEL.EMP,
												autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT2
											}
										}), {
											xtype : 'textarea',
											fieldLabel : '变更原因',
											width : 200,
											name : 'managerChangeReason'
										}]
							}]
						})],
						tbar : [{
							iconCls : 'ok',
							text : '提交',
							handler : function() {
								var oldDeptID = project.executiveDept.departmentID != null
										? project.executiveDept.departmentID
										: '';
								var oldManagerID = project.manager.userID != null
										? project.manager.userID
										: '';

								var newDeptID = win.form.getForm()
										.findField('executiveDept').getValue();
								var newManagerID = win.form.getForm()
										.findField('manager').getValue();

								if (newDeptID == '' && newManagerID == '') {
									Ext.Msg.alert('提示', '您没有修改，无需提交。');
									return;
								} else if (oldDeptID == newDeptID
										&& newDeptID != '') {
									Ext.Msg.alert('提示', '执行部门与当前相同，不需修改。');
									return;
								} else if (oldManagerID == newManagerID
										&& newManagerID != '') {
									Ext.Msg.alert('提示', '项目经理与当前相同，不需修改。');
									return;
								}
								var f = win.form.getForm();
								f.submit({
									url : String
											.format(
													'{0}/projectMain.do?method=updateProjectExecutiveDept',
													__cxtPath),// 提交变更执行部门
									method : 'post',
									params : {
										projectID : project.projectID,
										executiveDeptID : win.form.getForm()
												.findField('executiveDept')
												.getValue(),
										executiveDeptChangeReason : win.form
												.getForm()
												.findField('executiveDeptChangeReason')
												.getValue(),
										managerChangeReason : win.form
												.getForm()
												.findField('managerChangeReason')
												.getValue(),
										managerID : win.form.getForm()
												.findField('manager')
												.getValue()
									},
									waitMsg : '正在提交，请稍候……',
									waitTitle : '正在处理',
									scope : this,
									success : function(form, act) {
										win.hide();
										Ext.Msg.alert(act.result.messageTitle,
												act.result.message);
									},
									failure : function(form, act) {
										com.bitc.xmis.util.AlertUtil
												.formSubmitFailedAlert(act);
									}
								});
							}
						}]
					}).show();
				}
			});
			// 启动内部验收 未使用

			/**
			 * 附件列表
			 */
			var attFileGrid = new com.bitc.xmis.AttachmentList({
						title : '内部验收文档',
						stripeRows : true,
						REFID : project.projectID,
						attachmentTypeID : 9,
						autoHeight : true
					});

			bar.add({
				text : '启动内部验收',
				iconCls : 'edit',
				hidden : true,// 暂时隐藏
				// hidden : project.status.id == 301 ? true : false,
				handler : function(b, e) {
					var wind = new Ext.Window({
						title : '验收部门',
						layout : 'border',
						closable : false,
						width : 400,
						height : 300,
						modal : true,
						items : [new Ext.FormPanel({
							ref : 'form',
							region : 'center',
							border : false,
							labelAlign : 'right',
							bodyStyle : 'padding:10px;',
							items : [{
										xtype : 'hidden',
										name : 'checkDept'
									}, new com.bitc.xmis.OrgTreeChooseField({
										name : 'checkDeptName',
										width : 200,
										fieldLabel : '选择验收部门',
										setValueFun : function(nodes) {
											this
													.setValue(nodes[0].attributes.attributes.name);
											wind.form
													.getForm()
													.findField('checkDept')
													.setValue(nodes[0].attributes.attributes.id);
										},
										clearValueFun : function(nodes) {
											this.setValue('');
											wind.form.getForm()
													.findField('checkDept')
													.setValue('');
										},
										winConfig : {
											width : 350,
											height : 350
										},
										orgTreePanelConfig : {
											title : '选择验收部门',
											rootFilter : '1',
											checkable : true,
											singleCheck : true,
											rootId : __emp.companyID,
											rootLevel : com.bitc.xmis.OrgTree.LEVEL.COMP,
											leafLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT2,
											autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1
										}
									}), {
										xtype : 'textarea',
										fieldLabel : '备&nbsp;&nbsp;&nbsp;&nbsp;注',
										width : 200,
										name : 'myComment'
									}, attFileGrid]
						})],
						tbar : [{
							iconCls : 'ok',
							text : '提交',
							handler : function() {
								if (attFileGrid.store.data.length < 1) {
									Ext.Msg.alert('提示', '您未上传内部验收文档模板文件');
									return;
								}
								var f = wind.form.getForm();
								f.submit({
									url : String
											.format(
													'{0}/projectMain.do?method=startInterCheckToEngineer',
													__cxtPath),// 
									method : 'post',
									params : {
										checkDeptID : f.findField('checkDept')
												.getValue(),
										projectID : project.projectID,
										comment : f.findField('myComment')
												.getValue()
									},
									waitMsg : '正在提交，请稍候……',
									waitTitle : '正在处理',
									scope : this,
									success : function(form, act) {
										wind.hide();
										Ext.Msg.alert(act.result.messageTitle,
												act.result.message, function() {
													location.reload();
												});
										parent.xMis.refresh('mytodo',
												'myworkitemnewtodo');;
									},
									failure : function(form, act) {
										com.bitc.xmis.util.AlertUtil
												.formSubmitFailedAlert(act);
									}
								});
							}
						}, '-', {
							xtype : 'uploadButton',
							text : '上传内部验收文档',
							REFID : project.projectID,
							attachmentTypeID : 9,
							uploadPanelConfig : {
								listeners : {
									'uploadcomplete' : function() {
										attFileGrid.reload();
									}
								}
							}
						}, '-', {
							text : '取消',
							handler : function() {
								wind.hide();
							}
						}]
					}).show();
					// 设置初始值为科技质量部
					wind.form.getForm().findField('checkDept').setValue('11');
					wind.form.getForm().findField('checkDeptName')
							.setValue('科技质量部');

				}
			});

			var ds = new Ext.data.ArrayStore({
						fields : ['value', 'text']
					});

			var isForm = new Ext.FormPanel({
						region : 'center', //
						layout : 'auto',
						padding : 10,
						items : [{
									xtype : 'displayfield',
									value : '可以双击、拖动或选中后使用中间按钮选择'
								}, {
									xtype : 'itemselector',
									name : 'seldeviceids',
									imagePath : String.format(
											'{0}/resources/images/', __cxtPath),
									multiselects : [{
												width : 250,
												height : 220,
												store : ds,
												displayField : 'text',
												valueField : 'value'
											}, {
												width : 250,
												height : 220,
												store : new Ext.data.ArrayStore(
														{
															fields : ['value',
																	'text']
														}),
												displayField : 'text',
												valueField : 'value'
											}]
								}],
						buttons : [{
							xtype : 'button',
							text : '全选',
							handler : function() {
								var sel = isForm.getForm()
										.findField('seldeviceids');
								sel.fromMultiselect.view.selectRange(0,
										sel.fromMultiselect.view.store
												.getCount());
								sel.fromTo();
							}
						}, {
							text : '重选',
							handler : function() {
								isForm.getForm().findField('seldeviceids')
										.reset();
							}
						}, {
							text : '提交',
							handler : function() {
								var url = String.format(
										'{0}/device/pr.jsp?projectId={1}&{2}',
										__cxtPath, project.projectID, isForm
												.getForm().getValues(true));
								isWin.hide();
								parent.xMis.turn('purchaseRequisition', '采购申请-'
												+ project.projectName, url, '');
							}
						}]
					});
			var isWin = new Ext.Window({
						title : '选择设备',
						width : 550,
						height : 350,
						modal : true,
						layout : 'border',
						items : [isForm],
						closeAction : 'hide'
					});

			var statusWin = new Ext.Window({
				modal : true,
				autoHeight : true,
				width : 350,
				title : '修改项目状态',
				closeAction : 'hide',
				items : [new Ext.FormPanel({
					ref : 'form',
					baseCls : 'x-plain',
					padding : 10,
					labelWidth : 100,
					items : [{
								xtype : 'hidden',
								name : 'projectId',
								value : project.projectID
							}, new com.bitc.xmis.Combos.ProjectStatusCombo({
								ref : '../combo',
								fieldLabel : '进度计划节点',
								// editable : false,
								hiddenName : 'status',
								anchor : '90%',
								lazyInit : false,
								alowBlank : false,
								baseParams : {
									projectId : project.projectID
								},
								listeners : {
									'select' : function(comb, record) {
										statusWin.form
												.getForm()
												.findField('statusDesc')
												.setValue(record.data.statusDesc);
										statusWin.form.getForm()
												.findField('startDate')
												.setValue(record.data.startDate
														.format('Y-m-d'));
										statusWin.form
												.getForm()
												.findField('actualStartDate')
												.setValue(record.data.actualStartDate);
									}
								}
							}), {
								xtype : 'displayfield',
								name : 'startDate',
								fieldLabel : '预计进入日期',
								value : ''
							}, {
								xtype : 'datefield',
								name : 'actualStartDate',
								format : 'Y-m-d',
								value : new Date(),
								anchor : '90%',
								fieldLabel : '实际进入日期',
								allowBlank : false
							}, {
								xtype : 'textarea',
								anchor : '90%',
								name : 'statusDesc',
								fieldLabel : '项目状态解释'
							}, {
								xtype : 'displayfield',
								anchor : '90%',
								value : '如无选择项，请先制定项目进度计划，然后刷新页面'
							}],
					buttonAlign : 'center',
					buttons : [{
						text : '确定',
						handler : function(b, e) {
							statusWin.form.getForm().submit({
								url : String
										.format(
												'{0}/projectMain.do?method=updateProjectStatus',
												__cxtPath),
								success : function(form, act) {
									Ext.Msg.alert('成功', '项目状态设置成功！',
											function() {
												Ext.select('td.status')
														.update(statusWin.combo
																.getRawValue());
												statusWin.hide();
											});
								},
								failure : function(form, act) {
									com.bitc.xmis.util.AlertUtil
											.formSubmitFailedAlert(act);
								}
							});
						}
					}, {
						text : '取消',
						handler : function(b, e) {
							statusWin.hide(b.el.dom);
						}
					}]
				})]
			});
			bar.add({
						text : '修改项目状态',
						iconCls : 'edit',
						handler : function(b, e) {
							statusWin.x = e.getPageX();
							statusWin.y = e.getPageY() + 20;
							statusWin.show(b.el.dom);
						}
					});
			if ((project.status.id == 0 || project.status.id == 190)
					&& com.bitc.xmis.util.OrgUtil
							.hasRole(com.bitc.xmis.Roles.startproject)) {
				bar.add({
					text : '启动项目',
					handler : function(b, e) {
						var url = String
								.format(
										'{0}/projectMain.do?method=startProject&show=true&projectID={1}&projectName={2}&projectCode={3}',
										__cxtPath, project.projectID,
										project.projectName,
										project.projectCode);
						parent.xMis.turn('project-start-' + project.projectID,
								'项目启动-' + project.projectName, url, '');
					}
				});
			}
			bar.add('-');
			var first = true;

			bar.add({
						text : '采购申请',
						iconCls : 'add',
						handler : function() {
							tabProject.setActiveTab(2);
							if (first) {
								var data = [];
								var dstore = devicePanel.getStore();
								for (i = 0; i < dstore.getCount(); i++) {
									var r = dstore.getAt(i);
									if (Ext.isEmpty(r.data.purchaseStatus)) {
										data.push([r.data.id, r.data.name]);
									} else if (r.data.number > r.data.statusCount[3]) {
										data.push([r.data.id, r.data.name]);
									}
								}
								ds.loadData(data);
								if (ds.data.length > 0) {
									first = false;
								}
							} else {
								isForm.getForm().findField('seldeviceids')
										.reset();
							}
							isWin.show();
						}
					});
			bar.add({
						text : '添加设备',
						hidden : true,// 2014-02-28
						iconCls : 'add',
						scope : devicePanel,
						handler : devicePanel.newDevice
					});
			var importWin = new com.bitc.xmis.DeviceImportWin({
				width : 500,
				height : 120,
				modal : true,
				title : '上传设备清单',
				closeAction : 'hide',
				REFID : project.projectID,
				attachmentTypeID : com.bitc.xmis.Attachment.importDevice,
				url : String
						.format(
								'{0}/projectMain.do?method=importDeviceList&attachmentTypeID={1}&REFID={2}&type={3}',
								__cxtPath,
								com.bitc.xmis.Attachment.importDevice,
								project.projectID, '2'),
				listeners : {
					'uploadsuccess' : function() {
						importDeviceGrid.reload();
					}
				}
			});
			bar.add({
						iconCls : "attatchment",
						text : '上传设备清单',
						disabled : false,
						handler : function() {
							importWin.form.getForm().reset();
							importWin.show();
							tabProject.setActiveTab(2);
						}
					});
			bar.add('-');
			var psEditPanel = new com.bitc.xmis.project.ProjectScheduleEditPanel(
					{
						region : 'center',
						projectId : project.projectID,
						border : false,
						scheduleTitle : '项目进度计划(双击编辑日期)',
						statusTitle : '可用于进度计划的项目状态<br/>（拖动一个状态到进度计划中）',
						dragDropInited : false
					});
			var projectScheduleWin = new Ext.Window({
						title : '创建项目进度计划',
						modal : true,
						width : 940,
						height : 455,
						autoScroll : true,
						closeAction : 'hide',
						items : [psEditPanel],
						layout : 'border',
						listeners : {
							show : function() {
								if (!psEditPanel.dragDropInited) {
									psEditPanel.initDragDrop();
								}
							}
						}
					});
			bar.add({
						text : '项目进度计划',
						handler : function() {
							tabProject.setActiveTab(0);
							projectScheduleWin.show();
							psEditPanel.schedulePlanGrid.getStore().reload();
						}
					});

			// 关注
			var attention = new com.bitc.xmis.workflow.Attention({
						model : 'project-info',
						url : '/projectMain.do?method=getProjectInfo&projectID='
								+ project.projectID,
						text : '【项目】' + project.projectName,
						appId : project.projectID,
						userID : __emp.userID,
						addText : '关注此项目'
					});
			var g = window.g = new com.bitc.xmis.ProjectPaymentConditionGrid({
						title : "收款条件定义",
						region : 'center',
						border : false,
						prjID : project.projectID,
						contID : project.mainContractID,
						stripeRows : true,
						enableColumnResize : true,
						loadMask : {
							msg : '正在加载数据，请稍侯……'
						}
					});

			var win = new Ext.Window({
						modal : true,
						layout : 'border',
						title : '收款计划',
						width : 986,
						closeAction : 'hide',
						resizable : false,
						height : 400,
						items : [new Ext.Panel({
									region : 'center',
									layout : 'border',
									items : [g]
								})]
					});
			bar.add('-');
			bar.add({
						text : '收款计划',
						handler : function() {
							g.getStore().reload();
							win.show();
						}
					});

			attention.addButtonToBar(bar);
			bar.add({
						xtype : 'uploadButton',
						REFID : project.projectID,
						attachmentTypeID : com.bitc.xmis.Attachment.project,
						id : 'uploadButton',
						uploadPanelConfig : {
							listeners : {
								'uploadcomplete' : function() {
									attGrid.reload();
								}
							}
						}
					});
			Ext.getCmp('uploadButton').on({
						'click' : function() {
							tabProject.setActiveTab(3);
						}
					});
			return bar;
		}
	};
}();
function linkRenderer(value, contractID, contractName, contractType) {
	if (Ext.isEmpty(value)) {
		value = "_";
	}
	if (contractName.indexOf("^") > 0) {
		contractName = contractName.substring(0, contractName.indexOf("^"));
	}
	var index = value.indexOf("^");
	var tmp = "<a href='javascript:;' onclick=\"javascript:turnToEditContract('{0}','{1}',{2});\">{3}</a>";
	if (index >= 0) {
		var text = value.substring(0, index);
		var url = value.substring(index + 1, value.length);
		return String.format(tmp, contractID, contractName, contractType
						? contractType
						: 'undefined', text);
	}
	return String.format(tmp, contractID, contractName, contractType
					? contractType
					: 'undefined', value);
}
function turnToEditContract(contractID, contractName, contractType) {
	parent.xMis.turn('contract-' + contractID, contractName,
			'contractMain.do?method=contract&contractID=' + contractID, null);
}
Ext.onReady(function() {
	window.contractAmtInPay = 0.00;
	Ext.QuickTips.init(true);
	if (project.executiveDept == null) {
		project.executiveDept = {
			departmentName : ''
		};
	}
	var tplInfo = new Ext.XTemplate(
			'<table summary=""  class="info" >'
					+ '<tr class="tr1"><td class="name"  >项目编码：</td>'
					+ '<td class="value" colspan="3" style="font-size:12px;">{projectCode}&nbsp;</td></tr>'
					+ '<tr class="tr2"><td class="name">项目名称：</td>'
					+ '<td class="value" colspan="3" style="font-size:12px;">{projectName}&nbsp;</td></tr>'
					+ '<tr class="tr2"><td class="name">执行部门：</td>'
					+ '<td class="value" colspan="3" style="font-size:12px;">{[values.executiveDept.departmentName]}&nbsp;</td></tr>'
					+ '<tr class="tr2"><td class="name">项目经理：</td>'
					+ '<td class="value" style="font-size:12px;">{[values.manager.userName]}&nbsp;</td>'
					// + '<td class="value"><span
					// id="director">&nbsp;</span>&nbsp;</td>'
					+ '<td class="name" >启动日期：</td> '
					+ '<td class="value" style="font-size:12px;"><span id="startDate"></span>{startDate}&nbsp;</td>'
					+ '</tr><tr><td class="name">项目类型：</td>'
					+ '<td class="value" style="font-size:12px;">{[values.type.name]}&nbsp;</td>'
					+ '<td  class="name">项目实时状态：</td>'
					+ '<td class="value status" style="font-size:12px;">{[values.status.name]}({values.status.description})&nbsp;</td></tr>'
					+ '<tr><td class="name">收入合同签订额：</td>'
					+ '<td class="value" id="incomeAmt" style="font-size:12px;">&nbsp;</td>'
					+ '<td  class="name">支出合同签订额合计：</td>'
					+ '<td class="value" id="outAmt" style="font-size:12px;">&nbsp;</td></tr>'
					+ '<tr><td class="name">账务来款合计：</td>'
					+ '<td class="value" style="font-size:12px;"><span id="sumMB">&nbsp;</span>&nbsp;</td>'
					+ '<td  class="name">财务采购支出合计：</td>'
					+ '<td class="value" style="font-size:12px;"><span id="sumPayAmt">&nbsp;</span>&nbsp;</td></tr>'
					+ '</table>', {
				disableFormats : true
			});

	var contractRecord = Ext.data.Record.create([{
				name : DBSchema.Contract.ContractTypeID
			}, {
				name : DBSchema.Contract.ContractID
			}, {
				name : DBSchema.Contract.ContractNo
			}, {
				name : DBSchema.Contract.ContractName
			}, {
				name : DBSchema.Contract.Abbreviation
			}, {
				name : DBSchema.Contract.ParentID
			}, {
				name : DBSchema.Contract.ParentContractAbb
			}, {
				name : DBSchema.Contract.SignatureDate,
				type : 'date',
				dateFormat : 'Y-m-d H:i:s.u'
			}, {
				name : DBSchema.Contract.BuyerID
			}, {
				name : DBSchema.Contract.BuyerAbb
			}, {
				name : DBSchema.Contract.DeptIDOfBuyer
			}, {
				name : DBSchema.Contract.DeptNameOfBuyer
			}, {
				name : DBSchema.Contract.VendorID
			}, {
				name : DBSchema.Contract.VendorAbb
			}, {
				name : DBSchema.Contract.DeptIDOfVendor
			}, {
				name : DBSchema.Contract.DeptNameOfVendor
			}, {
				name : DBSchema.Contract.ContractAmt
			}, {
				name : DBSchema.Contract.BuyerRepNameDirectorName
			}, {
				name : DBSchema.Contract.VendorRepName
			}, {
				name : DBSchema.Contract.GovermentContractSourceName
			}, {
				name : DBSchema.Contract.ContractSummary
			}, {
				name : DBSchema.Contract.Remark
			}, {
				name : DBSchema.Contract.RepName
			}, {
				name : DBSchema.Contract.UpdateDate
			}, {
				name : DBSchema.Project.ProjectID
			}, {
				name : DBSchema.Project.ProjectCode
			}, {
				name : DBSchema.Project.ProjectName
			}, {
				name : DBSchema.CurrencyType.CurrenyGraph
			}, {
				name : DBSchema.ChargedDetail.TotalAmtByID
			}, {
				name : DBSchema.ChargedDetail.UnChargedAmtTotal
			}, {
				name : DBSchema.InvoiceDetail.TotalAmtByID
			}, {
				name : DBSchema.InvoiceDetail.UnAccountAmtTotalSum
			}, {
				name : 'warning_date'
			}, {
				name : 'warning_amt'
			}, {
				name : 'warning_days'
			}, {
				name : 'MB'
			}, {
				name : 'pay_amt'
			}, {
				name : 'unMB'
			}, {
				name : DBSchema.Contract.DirectorName
			}]);
	var contractStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
							+ '/contractMain.do?method=getContractByProjectID'
				}),
		baseParams : {
			projectID : project.projectID
		},
		autoLoad : false,
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, contractRecord),
		listeners : {
			load : function(store, records) {
				var sum = 0;
				var sumMB = 0
				var sumPayAmt = 0;
				Ext.iterate(records, function(record) {
							contID = record.get('合同ID');
							var amt = record.get(DBSchema.Contract.ContractAmt);
							amt = amt.substring(0, amt.indexOf('^'));
							sum += parseFloat(amt);
							sumMB += new Number(record.get('MB'));
							sumPayAmt += new Number(record.get('pay_amt'));
						});
				contractAmtInPay = sum;
				if (window.g) {
					window.g.setContractAmt(contractAmtInPay);
				}
				Ext.get('incomeAmt').update(Ext.util.Format.number(sum,
						'￥0,0.00'));
				Ext.get('sumMB').update(Ext.util.Format
						.number(sumMB, '￥0,0.00'));
				Ext.get('sumPayAmt').update(Ext.util.Format.number(sumPayAmt,
						'￥0,0.00'));
				if (records.length > 0) {
					// Ext.get('director').update(records[0]
					// .get(DBSchema.Contract.DirectorName));
					if (Ext.isEmpty(project.startDate, false)) {
						var dd = records[0]
								.get(DBSchema.Contract.SignatureDate);
						Ext.get("startDate").update(Ext.util.Format.date(dd,
								'Y-m-d'));
					}
				}
			}
		}
	});

	var contractCM = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(), {
		header : "合同编号",
		dataIndex : DBSchema.Contract.ContractNo,
		width : 125,
		renderer : function(value, cellmeta, record) {
			return linkRenderer(value,
					record.get(DBSchema.Contract.ContractID), record
							.get(DBSchema.Contract.Abbreviation));
		}
	}, {
		header : "合同名称",
		width : 200,
		hidden : true,
		dataIndex : DBSchema.Contract.ContractName,
		renderer : function(value, cellmeta, record) {
			return linkRenderer(value,
					record.get(DBSchema.Contract.ContractID), record
							.get(DBSchema.Contract.Abbreviation));
		}
	}, {
		header : "简称",
		dataIndex : DBSchema.Contract.Abbreviation,
		renderer : function(value, cellmeta, record, rowIndex) {
			return linkRenderer(value,
					record.get(DBSchema.Contract.ContractID), record
							.get(DBSchema.Contract.Abbreviation));
		},
		width : 100
	}, {
		header : "签订日期",
		dataIndex : DBSchema.Contract.SignatureDate,
		width : 100,
		align : 'center',
		renderer : Ext.util.Format.dateRenderer('Y-m-d')
	}, {
		header : "甲方公司",
		dataIndex : DBSchema.Contract.BuyerAbb,
		width : 150,
		hidden : true
	}, {
		header : "乙方公司",
		dataIndex : DBSchema.Contract.VendorAbb,
		width : 150,
		hidden : true
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Amt
				+ ">合同金额</font>",
		dataIndex : DBSchema.Contract.ContractAmt,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			value = value.substring(0, value.indexOf('^'));
			return Ext.util.Format.number(new Number(value),
					record.data[DBSchema.CurrencyType.CurrenyGraph] + '0,0.00');
		},
		align : 'right'
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Charged_Amt
				+ ">已收合同款</font>",
		dataIndex : DBSchema.ChargedDetail.TotalAmtByID,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			return formatCurrency(
					record.data[DBSchema.CurrencyType.CurrenyGraph], value,
					Color_Font_Contract_Total_Charged_Amt);
		},
		align : 'right'
	}, {
		header : "财务来款合计",
		dataIndex : 'MB',
		align : 'right',
		width : 150,
		renderer : Ext.util.Format.numberRenderer('￥0,000.00')
	}, {
		header : "采购支出合计",
		dataIndex : 'pay_amt',
		align : 'right',
		width : 150,
		renderer : Ext.util.Format.numberRenderer('￥0,000.00')
	}, {
		header : "合同欠款",
		dataIndex : 'unMB',
		align : 'right',
		width : 150,
		renderer : Ext.util.Format.numberRenderer('￥0,000.00')
	}, {
		header : "登记人",
		dataIndex : DBSchema.Contract.RepName,
		width : 150,
		hidden : true
	}, {
		header : "更新日期",
		dataIndex : DBSchema.Contract.UpdateDate,
		renderer : customer_formatDate,
		width : 150,
		align : 'center',
		hidden : true
	}]);

	var contractGrid = new Ext.grid.GridPanel({
				title : '收入合同',
				cm : contractCM,
				store : contractStore,
				stripeRows : true,
				height : 120,
				autoScroll : true
			});

	var outContractCM = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer(), {
		header : "合同编号",
		dataIndex : DBSchema.Contract.ContractNo,
		width : 125,
		renderer : function(value, cellmeta, record) {
			return linkRenderer(value,
					record.get(DBSchema.Contract.ParentID) == "" ? record
							.get(DBSchema.Contract.ContractID) : record
							.get(DBSchema.Contract.ParentID), record
							.get(DBSchema.Contract.Abbreviation), record
							.get(DBSchema.Contract.ContractTypeID));
		}
	}, {
		header : "合同名称",
		width : 150,
		dataIndex : DBSchema.Contract.ContractName,
		renderer : function(value, cellmeta, record) {
			return linkRenderer(value,
					record.get(DBSchema.Contract.ParentID) == "" ? record
							.get(DBSchema.Contract.ContractID) : record
							.get(DBSchema.Contract.ParentID), record
							.get(DBSchema.Contract.Abbreviation), record
							.get(DBSchema.Contract.ContractTypeID));
		}
	}, {
		header : "简称",
		dataIndex : DBSchema.Contract.Abbreviation,
		renderer : function(value, cellmeta, record, rowIndex) {
			return linkRenderer(value,
					record.get(DBSchema.Contract.ParentID) == "" ? record
							.get(DBSchema.Contract.ContractID) : record
							.get(DBSchema.Contract.ParentID), record
							.get(DBSchema.Contract.Abbreviation), record
							.get(DBSchema.Contract.ContractTypeID));
		},
		width : 100
	}, {
		xtype : 'numbercolumn',
		header : "支出金额",
		dataIndex : DBSchema.ContractRelated.Amt,
		width : 100,
		align : 'right',
		format : '￥0,000.00'
	}, {
		header : "签订日期",
		dataIndex : DBSchema.Contract.SignatureDate,
		width : 100,
		align : 'center',
		renderer : Ext.util.Format.dateRenderer('Y-m-d')
	}, {
		id : 'remark',
		header : "备注",
		width : 150,
		dataIndex : DBSchema.ContractRelated.Remark
	}]);
	var outContractStore = new Ext.data.JsonStore({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
							+ '/contractMain.do?method=getOutContractListByProjectID'
				}),
		baseParams : {
			projectID : project.projectID
		},
		autoLoad : true,

		root : 'rows',
		totalProperty : 'totalCount',
		fields : [DBSchema.Contract.ContractID, DBSchema.Contract.ContractNo,
				DBSchema.Contract.ContractName, DBSchema.Contract.Abbreviation,
				DBSchema.ContractRelated.Amt, {
					name : DBSchema.Contract.SignatureDate,
					type : 'date',
					dateFormat : 'Y-m-d H:i:s.u'
				}, {
					name : DBSchema.ContractRelated.Remark
				}, DBSchema.Contract.ParentID,
				DBSchema.Contract.ParentContractAbb],
		listeners : {
			load : function(store, records) {
				var sum = 0;
				Ext.iterate(records, function(record) {
							var amt = record.get(DBSchema.ContractRelated.Amt);
							sum += parseFloat(amt);
						});
				Ext.get('outAmt')
						.update(Ext.util.Format.number(sum, '￥0,0.00'));
			}
		}
	});
	var outContractGrid = new Ext.grid.GridPanel({
				title : '支出合同',
				stripeRows : true,
				autoScroll : true,
				// autoWidth : true,
				// width : 1000,
				height : 120,
				// layout : 'fit',
				cm : outContractCM,
				// region: 'south',
				// autoExpandColumn : 'remark',
				store : outContractStore
			});

	var infoPanel = new Ext.Panel({
				border : false,
				html : '',
				listeners : {
					'render' : {
						fn : function() {
							if (project) {
								if (project.type == null) {
									project.type = {
										name : '未设置'
									};
								}
								if (project.manager == null) {
									project.manager = {
										userName : ''
									};
								}
								tplInfo.overwrite(infoPanel.body, project);
							}
						},
						scope : this
					}
				}
			});
	var devicePanel = new com.bitc.xmis.ProjectDevicePanel({
				frame : true,
				region : 'center',
				autoScroll : true,
				project : project,
				border : false,
				editable : true
			});
	var devicePanel2 = new com.bitc.xmis.ProjectDevicePanelGrid({
				projectId : project.projectID,
				editable : true,
				autoScroll : true,
				border : false,
				region : 'center'

			});
	var deviceImportPanel = new com.bitc.xmis.DeviceImportPanelGrid({
				projectId : project.projectID,
				editable : true,
				autoScroll : true,
				border : false,
				region : 'center'
			});
		top.xMis.addRefresh('deviceImportPanelGrid', function() {
				deviceImportPanel.store.reload();
			});
			
	var importDeviceGrid = new com.bitc.xmis.DeviceAttachmentList({
				title:'设备清单导入文件',
				REFID : project.projectID,
				attachmentTypeID : com.bitc.xmis.Attachment.importDevice,
				autoScroll : true,
				width:300,
				split : true,
				collapsible : true, // 是否有右上方展开收缩条
				collapseMode : 'mini', // 将滚动收缩条模式‘迷你’
				region : 'east'
			});
		top.xMis.addRefresh('importDeviceGrid', function() {
				importDeviceGrid.store.reload();
			});
	var twoPanel = new Ext.Panel({
				region : 'center',
				autoScroll : true,
				layout:'border',
				height : 320,
				border : false,
				items : [deviceImportPanel,importDeviceGrid]
			});
	var taskPanel = com.bitc.xmis.ProjectTaskPanel.buildGrid({
				projectId : project.projectID,
				region : 'center',
				border : false,
				autoScroll : true

			});

	var scheduleGrid = new com.bitc.xmis.project.ProjectSchedulePlanGrid({
				projectId : project.projectID,
				showActualStartDate : false,
				editable : false,
				editableColumns : ['actualStartDate'],
				region : 'center',
				border : false,
				autoScroll : true
			});
	var attGrid = new com.bitc.xmis.AttachmentList({
				REFID : project.projectID,
				attachmentTypeID : com.bitc.xmis.Attachment.project,
				autoScroll : true,
				border : false,
				region : 'center'
			});
	var tabProject = new Ext.TabPanel({
				region : 'center',
				margins : '3 3 3 0',
				activeTab : 0,
				autoScroll : true,
				deferredRender : false,
				items : [{
							title : '项目进度',
							layout : 'border',
							height : 320,
							border : false,
							items : [scheduleGrid]
						}, {
							title : '项目任务',
							layout : 'border',
							height : 320,
							items : [taskPanel]
							// }, {
						// title : '导入的设备清单',
						// layout : 'border',
						// height : 120,
						// items : [devicePanel]
					}	, {
							title : '设备导入信息',
							layout : 'border',
							height : 320,
							items : [twoPanel]
						}, {
							title : '附件列表',
							layout : 'border',
							height : 320,
							items : [attGrid]
						}, {
							title : '设备清单',
							layout : 'border',
							height : 320,
							items : [devicePanel2]
						}]

			});
	var centerPanel = new Ext.Panel({
				// border : false,
				region : 'center',
				autoScroll : true,
				padding : 10,
				items : [{
							xtype : 'fieldset',
							title : '基本信息',
							width : 700,
							items : [infoPanel]
						}, {
							xtype : 'fieldset',
							collapsible : true,
							title : '合同信息',
							height : 180,
							// layout: 'border', //表格布局
							items : [new Ext.TabPanel({
										region : 'center',
										ref : 'tabContract',
										margins : '3 3 3 0',
										activeTab : 0,
										autoScroll : true,
										deferredRender : false,
										items : [contractGrid, outContractGrid]

									})]
						}, {
							xtype : 'fieldset',
							collapsible : true,
							title : '项目管理',
							height : 380,
							items : [tabProject]
						}],
				tbar : com.bitc.xmis.ProjectInfo.buildToolbar(devicePanel,
						taskPanel, attGrid, importDeviceGrid, tabProject)
			});
	contractStore.load();

	if (com.bitc.xmis.ProjectInfo.request.getParameter("todoId") != null) {
		centerPanel.insert(0, new com.bitc.xmis.TodoInfoPanel({
							todoId : com.bitc.xmis.ProjectInfo.request
									.getParameter("todoId"),
							frame : true
						}));
	}

	var processTaskId = com.bitc.xmis.ProjectInfo.request
			.getParameter("processTaskId");
	if (processTaskId != null) {
		var processInstanceId = com.bitc.xmis.ProjectInfo.request
				.getParameter("processInstanceId");
		// version1项目启动的接收任务url多传了processTaskId问题
		if (processInstanceId.length == 2) {
			processInstanceId = processInstanceId[0];
		}
		centerPanel.getTopToolbar().add(new com.bitc.xmis.workflow.TraceButton(
				{
					processInstanceId : processInstanceId
				}));
	}

	var main = new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [centerPanel]
			});
	/*
	 * scheduleGrid.store.on('update', function(s, record, opt) { if (opt !=
	 * Ext.data.Record.EDIT) { return; } var params = { id : record.data.id,
	 * actualStartDate : record.data.actualStartDate.format('Y-m-d') }
	 * Ext.Ajax.request({ url : String .format(
	 * '{0}/projectMain.do?method=updateProjectSchedulePlan', __cxtPath), method :
	 * 'post', params : params, success : function() { record.commit(); } });
	 * });
	 */
	scheduleGrid.store.load();
});