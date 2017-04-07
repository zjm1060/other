Ext.ns('com.bitc.xmis.project');
com.bitc.xmis.project.StartProject = function(config) {
	Ext.apply(this, config || {});
};
com.bitc.xmis.project.StartProject.prototype.init = function() {
	var draft = this.draft = new com.bitc.xmis.sys.Draft({
		model : 'startproject',
		appId : 'step1-' + this.projectID,
		userID : __emp.userID,
		text : '【项目启动】' + this.projectName,
		url : String
				.format(
						'projectMain.do?method=startProject&show=true&projectID={0}&projectName={1}&projectCode={2}',
						this.projectID, this.projectName, this.projectCode)
	});
	var treePanel = new com.bitc.xmis.OrgTreePanel({
				ddGroup : 'project-org-dd-group',
				title : '人员选择',
				region : 'west',
				enableDrag : true,
				rootId : __emp.companyID,
				rootLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1,
				rootFilter : __emp.divisionID,
				autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1
			});
	var data = {
		data : []
	};
	var step2Store = this.taskStore = new Ext.data.JsonStore({
				root : 'data',
				data : data,
				idProperty : 'userId',
				fields : ['userId', 'deptId', 'deptName', 'userName',
						'taskDesc', 'role']
			});
	var step2Grid = new Ext.grid.EditorGridPanel({
				region : 'center',
				title : '从左侧拖动人员到表格中：',
				ddGroup : 'project-org-dd-group',
				store : step2Store,
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
									step2Store.remove(s.record);
								}
							}
						}],
				columns : [{
							header : '部门名称',
							dataIndex : 'deptName'
						}, {
							header : '任务人ID',
							hidden : true,
							dataIndex : 'userId'
						}, {
							header : '任务人',
							dataIndex : 'userName'
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
													data : [[1, '项目经理'],
															[2, '项目副经理'],
															[3, '项目成员']]
												}),
										width : 200
									})
						}],
				bbar : ['点击任务描述可编辑，点击角色可选择']
			});
	var deptTree = this.deptTree = new com.bitc.xmis.OrgTreePanel({
				ddGroup : 'project-org-dd-group',
				title : '选择部门',
				enableDrag : true,
				width : 400,
				height : 350,
				checkable : true,
				singleCheck : true,
				rootId : __emp.companyID,
				rootLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1,
				leafLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT2,
				rootFilter : __emp.divisionID,
				autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1
			});
	var attGrid = new com.bitc.xmis.AttachmentList({
				title : '附件列表',
				region : 'east',
				width : 200,
				attachmentTypeID : 6,
				REFID : this.projectID
			});
	var baseForm = this.form = new Ext.FormPanel({
		region : 'center',
		padding : '5 5 5 5',
		autoScroll : true,
		tbar : [{
					text : '保存草稿',
					ref : '../saveButton',
					iconCls : 'save',
					handler : function() {
						var content = {
							form : baseForm.getForm().getValues(),
							grid : com.bitc.xmis.util.ExtUtil
									.getStoreData(step2Store)
						}
						draft.save(function(d, result) {
									if (parent && parent.xMis) {
										parent.xMis.refresh('mydraft');
									}
									Ext.Msg.alert('提示',
											'保存草稿成功！<br/>你可以在首页的草稿箱中找到。');
								}, baseForm, content);
					}
				}, {
					text : '启动',
					ref : '../submitButton',
					iconCls : 'ok',
					scope : this,
					handler : this.submit
				}, new Ext.ux.UploadButton({
							REFID : this.projectID,
							attachmentTypeID : com.bitc.xmis.Attachment.project,
							uploadPanelConfig : {
								listeners : {
									'uploadcomplete' : function() {
										attGrid.reload();
									}
								}
							}
						})],
		items : [new Ext.form.FieldSet({
					title : '项目基本信息',
					width : 780,
					defaults : {
						msgTarget : 'under'
					},
					items : [{
								xtype : 'hidden',
								name : 'projectID',
								value : this.projectID
							}, {
								xtype : 'textfield',
								readOnly : true,
								fieldLabel : '项目编码',
								name : 'projectCode',
								width : 300,
								value : this.projectCode
							}, {
								xtype : 'textarea',
								fieldLabel : '项目名称',
								name : 'projectName',
								allowBlank : false,
								invalidText : '请输入项目名称',
								width : 300,
								height : 40,
								value : this.projectName
								// }, {
							// xtype : 'combo',
							// fieldLabel : '项目类型',
							// id : 'projectType',
							// hiddenName : 'projectTypeID',
							// displayField : 'text',
							// valueField : 'value',
							// mode : 'local',
							// triggerAction : 'all',
							// emptyText : '请选择类型',
							// editable : false,
							// value : 1,
							// store : new Ext.data.ArrayStore({
							// fields : ['value', 'text'],
							// data : [[1, '合同项目'], [2, '自研项目']]
							// }),
							// width : 200,
							// allowBlank : false,
							// invalidText : '请选择项目类型'
						}	, {
								xtype : 'hidden',
								name : 'contractID'
							}, {
								xtype : 'datefield',
								fieldLabel : '启动日期',
								name : 'projectStartDate',
								format : 'Y-m-d',
								value : Ext.util.Format.date(new Date(),
										'Y-m-d'),
								width : 200
							}, {
								xtype : 'textfield',
								readOnly : true,
								hidden : true,
								fieldLabel : '任务备注',
								name : 'deptTaskDesc__',
								width : 300,
								value : ''
							}]
				}), new Ext.form.FieldSet({
			title : '项目任务分配',
			width : 780,
			items : [{
						xtype : 'hidden',
						name : 'executiveDept'
					}, new com.bitc.xmis.OrgTreeChooseField({
						name : 'executiveDeptName',
						fieldLabel : '项目执行部门',
						setValueFun : function(nodes) {
							this.setValue(nodes[0].attributes.attributes.name);
							baseForm
									.getForm()
									.findField('executiveDept')
									.setValue(nodes[0].attributes.attributes.id);
						},
						clearValueFun : function(nodes) {

						},
						winConfig : {
							width : 400,
							height : 350
						},
						orgTreePanelConfig : {
							title : '选择部门',
							rootFilter : '1',
							checkable : true,
							singleCheck : true,
							rootId : __emp.companyID,
							rootLevel : com.bitc.xmis.OrgTree.LEVEL.COMP,
							leafLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT2,
							autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT1
						}
					}), {
						xtype : 'radiogroup',
						fieldLabel : '分配方式',
						ref : '../radio',
						columns : [100, 100],
						name : 'mode',
						items : [{
									boxLabel : '分配到人员',
									inputValue : 0,
									name : 'mode',
									checked : true
								}, {
									boxLabel : '分配到部门',
									name : 'mode',
									inputValue : 1
								}],
						listeners : {
							change : function(rg, r) {
								baseForm.card.getLayout()
										.setActiveItem(r.inputValue);
							}
						}
					}, {
						xtype : 'hidden',
						name : 'taskDept'
					}, {
						xtype : 'hidden',
						name : 'selectpath'
					}, {
						xtype : 'panel',
						border : false,
						layout : 'card',
						ref : '../card',
						activeItem : 0,
						items : [{
									xtype : 'panel',
									border : false,
									height : 350,
									items : [step2Grid, treePanel],
									layout : 'border'
								}, {
									xtype : 'panel',
									ref : 'deptCard',
									border : false,
									layout : 'table',
									layoutConfig : {
										columns : 2
									},
									items : [deptTree, {
												xtype : 'panel',
												border : false,
												width : 300,
												height : 350,
												labelWidth : 60,
												layout : 'form',
												items : [{
															xtype : 'textarea',
															name : 'deptTaskDesc',
															fieldLabel : '任务描述',
															labelAlign : 'right',
															width : 200,
															height : 300
														}]
											}]
								}]
					}]
		})]
	});
	deptTree.on('checkchange', function(n, checked) {
				if (checked) {
					baseForm.getForm().findField('taskDept')
							.setValue(n.attributes.attributes.id);
					baseForm.getForm().findField('selectpath').setValue(n
							.getPath('id'));
				}
			});
	new Ext.Viewport({
				layout : 'border',
				border : false,
				items : [baseForm, attGrid]
			});
	new Ext.dd.DropTarget(step2Grid.getView().scroller, {
				ddGroup : 'project-org-dd-group',
				notifyEnter : function(dd, e, data) {
					if (this.overClass) {
						this.el.addClass(this.overClass);
					}
					return data.node.leaf
							? this.dropAllowed
							: this.dropNotAllowed;
				},
				notifyOver : function(dd, e, data) {
					return data.node.leaf
							? this.dropAllowed
							: this.dropNotAllowed;
				},
				notifyDrop : function(ddSource, e, data) {
					var node = data.node;
					if (!node.leaf) {
						return false;
					}
					var record = new step2Store.recordType({
								deptId : node.parentNode.attributes.attributes.id,
								deptName : node.parentNode.attributes.attributes.name,
								userId : node.attributes.attributes.id,
								userName : node.text,
								taskDesc : '',
								role : 3
							}, node.attributes.attributes.id);
					step2Store.insert(step2Store.data.length, record);
					return (true);
				}
			});
};
com.bitc.xmis.project.StartProject.prototype.validate = function() {
	// 验证项目经理所在的部门与执行部门不一致的提示
	// var executiveDeptId = this.form.getForm().findField('executiveDept')
	// .getValue();
	var mode = this.form.getForm().findField('mode').getValue();
	if (mode == 0 || this.step == 'StartProject.DirectorAssign.AssignTask') {
		var managercount = 0;
		var taskDescName = '';
		this.taskStore.each(function(record) {
					if (record.get('role') == 1) {
						managercount++;
					}
					taskDescName = record.get('taskDesc');
				});
		if (taskDescName == '' || taskDescName.trim() == '') {
			Ext.Msg.alert('提示', '请添加任务描述。');
			return false;
		}
		if (managercount > 1) {
			Ext.Msg.alert('提示', '只能指定一个项目经理，请重新选择。');
			return false;
		} else if (managercount == 0) {
			Ext.Msg.alert('提示', '还没有指定项目经理，请重新选择。');
			return false;
		}
	} else if (mode == 1) {
		if (Ext.isEmpty(this.form.getForm().findField('taskDept').getValue())) {
			Ext.Msg.alert('提示', '请选择部门。');
			return false;
		}
	}
	return true;
};
com.bitc.xmis.project.StartProject.prototype.submit = function() {
	if (!this.validate()) {
		return;
	}
	var f = this.form;
	var d = this.draft;
	var tasks = com.bitc.xmis.util.ExtUtil.encodeStoreData(this.taskStore);
	// --

	// 验证项目经理所在的部门与执行部门不一致的提示
	var executiveDeptId = this.form.getForm().findField('executiveDept')
			.getValue();
	var mode = this.form.getForm().findField('mode').getValue();
	if (mode == 0 || this.step == 'StartProject.DirectorAssign.AssignTask') {
		var managercount = 0;
		var taskDescName = '';
		this.taskStore.each(function(record) {
					if (record.get('role') == 1) {
						managercount++;
					}
					taskDescName = record.get('taskDesc');
				});
		// 验证项目经理所在的部门与执行部门不一致的提示
		var managerDeptId = ""
		this.taskStore.each(function(record) {
					if (record.get('role') == 1) {
						managerDeptId = record.get('deptId');
					}
				});
		var step = this.step;
		var workItemId = this.workItemId;
		if (executiveDeptId != '' && executiveDeptId != managerDeptId) {
			Ext.MessageBox.confirm("提示", "您选择的项目执行部门，与项目经理所在部门不一致，是否修改？",
					function(btn) {
						if (btn == 'no') {
							f.getForm().submit({
								url : __cxtPath + '/projectMain.do',
								params : {
									method : 'startProject',
									tasks : tasks,
									step : step,
									workItemId : workItemId
								},
								waitMsg : '正在提交，请稍候……',
								waitTitle : '正在处理',
								scope : this,
								success : function(form, act) {
									if (f.saveButton) {
										f.saveButton.setDisabled(true);
									}
									f.submitButton.setDisabled(true);
									d.remove(function() {
												if (parent && parent.xMis) {
													parent.xMis
															.refresh('mydraft');
												}
											});
									if (parent && parent.xMis) {
										parent.xMis.refresh('mytodo','myworkitemnewtodo');
									}
									Ext.Msg.alert(act.result.messageTitle,
											act.result.message);
								},
								failure : function(form, act) {
									com.bitc.xmis.util.AlertUtil
											.formSubmitFailedAlert(act);
								}
							});
						}
					});
		} else {
			f.getForm().submit({
						url : __cxtPath + '/projectMain.do',
						params : {
							method : 'startProject',
							tasks : tasks,
							step : step,
							workItemId : workItemId
						},
						waitMsg : '正在提交，请稍候……',
						waitTitle : '正在处理',
						scope : this,
						success : function(form, act) {
							if (f.saveButton) {
								f.saveButton.setDisabled(true);
							}
							f.submitButton.setDisabled(true);
							d.remove(function() {
										if (parent && parent.xMis) {
											parent.xMis.refresh('mydraft');
										}
									});
							if (parent && parent.xMis) {
								parent.xMis.refresh('mytodo','myworkitemnewtodo');
							}
							Ext.Msg.alert(act.result.messageTitle,
									act.result.message);
						},
						failure : function(form, act) {
							com.bitc.xmis.util.AlertUtil
									.formSubmitFailedAlert(act);
						}
					});
		}

	} else if (mode == 1) {
		// 验证所选的部门与执行部门不一致的提示
		var selectDeptId = f.getForm().findField('taskDept').getValue();
		if (executiveDeptId != '' && executiveDeptId != selectDeptId) {
			Ext.MessageBox.confirm("提示", "您选择的项目执行部门，与所选择的部门不一致，是否修改？",
					function(btn) {
						if (btn == 'no') {
							f.getForm().submit({
								url : __cxtPath + '/projectMain.do',
								params : {
									method : 'startProject',
									tasks : tasks,
									step : step,
									workItemId : workItemId
								},
								waitMsg : '正在提交，请稍候……',
								waitTitle : '正在处理',
								scope : this,
								success : function(form, act) {
									if (f.saveButton) {
										f.saveButton.setDisabled(true);
									}
									f.submitButton.setDisabled(true);
									d.remove(function() {
												if (parent && parent.xMis) {
													parent.xMis
															.refresh('mydraft');
												}
											});
									if (parent && parent.xMis) {
										parent.xMis.refresh('mytodo','myworkitemnewtodo');
									}
									Ext.Msg.alert(act.result.messageTitle,
											act.result.message);
								},
								failure : function(form, act) {
									com.bitc.xmis.util.AlertUtil
											.formSubmitFailedAlert(act);
								}
							});
						}
					});
		} else {
			f.getForm().submit({
						url : __cxtPath + '/projectMain.do',
						params : {
							method : 'startProject',
							tasks : tasks,
							step : step,
							workItemId : workItemId
						},
						waitMsg : '正在提交，请稍候……',
						waitTitle : '正在处理',
						scope : this,
						success : function(form, act) {
							if (f.saveButton) {
								f.saveButton.setDisabled(true);
							}
							f.submitButton.setDisabled(true);
							d.remove(function() {
										if (parent && parent.xMis) {
											parent.xMis.refresh('mydraft');
										}
									});
							if (parent && parent.xMis) {
								parent.xMis.refresh('mytodo','myworkitemnewtodo');
							}
							Ext.Msg.alert(act.result.messageTitle,
									act.result.message);
						},
						failure : function(form, act) {
							com.bitc.xmis.util.AlertUtil
									.formSubmitFailedAlert(act);
						}
					});
		}
	}

};

com.bitc.xmis.project.StartProject.prototype.loadDraft = function(draftId) {
	this.draft.id = draftId;
	this.draft.load(function(d) {
				var c = d.content;
				this.form.getForm().setValues(c.form);
				var records = [];
				Ext.iterate(c.grid, function(item) {
							records.push(new this.taskStore.recordType(item));
						}, this);
				this.taskStore.add(records);
				if (!Ext.isEmpty(c.form.selectpath)) {
					this.deptTree.expandPath(c.form.selectpath, 'id', function(
									bSucess, oLastNode) {
								oLastNode.getUI().toggleCheck(true);
								if (!oLastNode.attributes.checked) {
									oLastNode.attributes.checked = true;
								}
							});
				}
			}, this);
}
Ext.onReady(function() {
			Ext.QuickTips.init();
			var projectID = request.getParameter("projectID")
					|| request.getParameter('projectId');
			var projectCode = request.getParameter("projectCode");
			var projectName = request.getParameter("projectName");

			var startProject = new com.bitc.xmis.project.StartProject({
						projectID : projectID,
						projectCode : projectCode,
						projectName : projectName
					});
			startProject.init();

			// 初始化草稿
			if (request.getParameter("draftId") != null) {
				startProject.loadDraft(request.getParameter("draftId"));
			}
			var baseForm = startProject.form;
			if (dept != null) {
				baseForm.getForm().findField('executiveDept')
						.setValue(dept.departmentID);
				baseForm.getForm().findField('executiveDeptName')
						.setValue(dept.departmentName);
			}
			// 初始化任务
			var step = request.getParameter("step");
			var workItemId = request.getParameter("workItemId");
			startProject.step = step;
			startProject.workItemId = workItemId;
			startProject.projectID = projectID;
			baseForm.getForm().findField('projectID')
					.setValue(startProject.projectID);
			baseForm.submitButton.setText('提交');
			if (step == 'StartProject.TaskAssignment.AssignTask') { // 第一个步骤
			} else if (step == 'StartProject.DirectorAssign.AssignTask') { // 第二个步骤
				baseForm.getForm().findField('deptTaskDesc__').setValue(request
						.getParameter("deptTaskDesc"));
				baseForm.getForm().findField('deptTaskDesc__').setVisible(true);
				baseForm.radio.hide();
				baseForm.card.remove(baseForm.card.deptCard, true);
				baseForm.getTopToolbar().remove(baseForm.saveButton);
				// Ext.apply(draft, {
				// model : 'startproject',
				// appId : 'step2' + projectID,
				// userID : __emp.userID,
				// text : '【】' + projectName,
				// url : String
				// .format(
				// 'projectMain.do?method=startProject&show=true&projectID={0}&projectName={1}&projectCode={2}',
				// projectID, projectName, projectCode)
				// });
			}
			if (request.getParameter("todoId") != null) {
				baseForm.insert(0, new com.bitc.xmis.TodoInfoPanel({
									todoId : request.getParameter("todoId"),
									frame : true
								}));
			}
			if (step != null) {
				var processInstanceId = request
						.getParameter("processInstanceId");
				// version1项目启动的接收任务url多传了processTaskId问题
				if (processInstanceId.length ==2) {
					processInstanceId = processInstanceId[0];
				}
				baseForm.getTopToolbar()
						.add(new com.bitc.xmis.workflow.TraceButton({
									processInstanceId : processInstanceId
								}));
			}
		});
