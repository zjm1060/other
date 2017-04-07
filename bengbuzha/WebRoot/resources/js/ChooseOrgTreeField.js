/*
 * ! Ext JS Library 3.2.0 Copyright(c) 2006-2010 Ext JS, Inc.
 * licensing@extjs.com http://www.extjs.com/license
 */

Ext.ns('Ext.ux.form');

Ext.ux.form.ChooseOrgTreeField = Ext.extend(Ext.form.TwinTriggerField, {
	initComponent : function() {
		this.addEvents('beforeshowwin', 'dblclick', 'nodedblclick');
		Ext.ux.form.ChooseOrgTreeField.superclass.initComponent.call(this);
	},
	initEvents : function() {
		Ext.ux.form.ChooseOrgTreeField.superclass.initEvents.call(this);
		this.mon(this.el, 'dblclick', this.onDblClick, this);
	},
	onDblClick : function() {
		if (!this.isTrigger1Hidden)
			this.fireEvent('dblclick', this);
		this.isTrigger1Hidden = false;
	},
	onRender : function(ct, position) {
		Ext.ux.form.ChooseOrgTreeField.superclass.onRender.call(this, ct,
				position);
	},
	getTreePanel : function() {
		return this.treepanel;
	},
	treepanel : null,
	id : '',
	modal : true,
	validationEvent : false,
	validateOnBlur : false,
	trigger1Class : 'x-form-clear-trigger',
	trigger2Class : 'x-form-search-trigger',
	singleExpand : true,
	hideTrigger1 : true,
	editable : false,

	singleSelect : true,
	onlyPerson : true,
	dataUrl : '',
	oldRoot : null,

	modifyNameStr : '',
	expandNodePath : function() {
		return {
			deptid : 0,
			path : ''
		}
	},
	getExpandPath:function() {
		return {
			empId : 0
		}
	},
	/**
	 * 清空值的列表，传参格式：‘id1，id2，id3...’
	 */
	clearValueList : '',

	winConfig : {},

	baseParamFun : function() {
		return {};
	},

	/**
	 * 返回选择结果的回调函数
	 */
	setValueFun : function(result, str) {
	},
	clearValueFun : function() {
	},

	/**
	 * 用于配置根节点
	 * 
	 * @type Mixed
	 */
	rootConfig : function() {
		return {}
	},

	lastIds : [],
	specifiedselect : '',

	isTrigger1Hidden : false,

	onTrigger1Click : function() {
		var id = this.id;
		var clearValueList = this.clearValueList;
		if (id.trim() != '' && id != null) {
			Ext.getCmp(id).setValue('');
		}
		if (clearValueList != null && clearValueList.trim() != '') {
			var ids = (clearValueList.trim()).split(',');
			for (var i = 0; i < ids.length; i++) {
				Ext.getCmp(ids[i]).setValue('');
			}
		}
		this.oldRoot = null;
		this.lastIds = [];
		this.clearValueFun();
		this.triggers[0].hide();
		this.isTrigger1Hidden = true;
	},

	onTrigger2Click : function() {
		function cascadeChildren(checked, node) {
			node.attributes.checked = checked;
			if (!Ext.isBoolean(checked))
				return;
			Ext.each(node.childNodes, function(n) {
						n.getUI().toggleCheck(checked);
						cascadeChildren(checked, n);
					});
		}
		function checkAllNodes(checked, treepanel, nodes) {
			cascadeChildren(checked, treepanel.getRootNode());
			Ext.each(nodes, function(n) {
						n = treepanel.getNodeById(n);
						n.getUI().toggleCheck(true);
					}, this);
		}
		if (!this.fireEvent('beforeshowwin', this)) {
			return;
		}
		if (this.window) {
			this.treepanel.getRootNode().setId(this.rootConfig.call(this).id);
			if (this.expandNodePath.call(this).path == ''&&this.getExpandPath.call(this).empId==0) {
				this.treepanel.getLoader().load(this.treepanel.getRootNode());
			}
			if (this.expandNodePath.call(this).path != '') {
				var expfun = this.expandNodePath;
				var tp = this.treepanel;
				if (this.expandNodePath.call(this).deptid != 0) {
					Ext.Ajax.request({
						url : __cxtPath
								+ '/contractMain.do?method=getParentDeptIdByDeptId',
						success : function(response, action) {
							var action = Ext.decode(response.responseText);
							var pid = action.deptID;
							if (pid == 0) {
								var treep = tp;
								var expandp = expfun.call(this).path;
								var rootn = tp.getRootNode();
								treep.getLoader().load(rootn, function() {
											treep.expandPath(expandp);
										});
							} else {
								var path = expfun.call(this).path;
								var paths = path.split("/");
								var ppath = 'dept-old-' + pid;
								var p = paths[0] + '/' + paths[1] + '/' + ppath
										+ '/' + paths[2];
								var treep = tp;
								var rootn = tp.getRootNode();
								treep.getLoader().load(rootn, function() {
											treep.expandPath(p);
										});
							}

						},
						failure : function() {
						},
						method : "post",
						params : {
							deptID : this.expandNodePath.call(this).deptid
						}
					});
				} else {
					var treep = this.treepanel;
					var expandp = this.expandNodePath.call(this).path;
					var rootn = this.treepanel.getRootNode();
					treep.getLoader().load(rootn, function() {
								treep.expandPath(expandp);
							});
				}
			}
			this.oldRoot = this.rootConfig.call(this).id;
			this.window.show();
					//通过页面 人员ID  得到此人的路径  展开 选中
			
			if(this.getExpandPath.call(this).empId != 0){
								var t = this.treepanel;
								var empId = this.getExpandPath.call(this).empId;
								Ext.Ajax.request({
										url : __cxtPath
												+ '/orgMaint.do?method=getEmpNodePathByEmpId',
										success : function(response, action) {
											var result = Ext.decode(response.responseText);
											var nodepath = result.attributes.nodepath;
											var rootn = t.getRootNode();
											t.getLoader().load(rootn, function() {
														t.expandPath(nodepath,"",function(){
																var node = t.getNodeById("empl-old-"+empId);
																node.getUI().toggleCheck(true);//节点选中
														});
												});
										},
										failure : function() {
										},
										method : "post",
										params : {
											empId : empId
										}
									});
						}
			
			return;
		}
		var selectNode = null;
		var array = null;
		var setValueFun = this.setValueFun;
		var singleSelect = this.singleSelect;
		var rootConfig = this.rootConfig.call(this);
		var dataUrl = this.dataUrl;
		var baseParams = this.baseParamFun.call(this);
		var singleExpand = this.singleExpand;
		var modal = this.modal;
		// 临时
		var modifyNameStr = this.modifyNameStr;
		var expandNodePath = this.expandNodePath.call(this).path;

		var rootDefaultConfig = {
			nodeType : 'async',
			draggable : false,
			id : 'root'
		};
		var winConfig = this.winConfig;
		var chooseFun = function() {
			if (selectNode != null) {
				var model = treepanel.getSelectionModel();
				var selected = model.select(selectNode);
				this.lastIds = [];
				this.lastIds[0] = selected.id;
				array = new Array(selected);
				this.window.callback(array);
				this.triggers[0].show();
				this.window.hide();
				// if(linkManDblclickFunction){
				// Ext.Ajax.request({
				// url : __cxtPath + '/crmMaint.do?method=getEmpInfo',
				// params : {
				// empID : selected.id.substring(9,selected.id.length)
				// },
				// autoLoad : true,
				// success : function(response, action) {
				// var act = Ext.decode(response.responseText);
				// if(modifyNameStr =='联系人'){
				// Ext.getCmp('deliveryPost').setValue(act.empPost1);
				// Ext.getCmp('deliveryPhone').setValue(act.empPhone1);
				// Ext.getCmp('deliveryEmail').setValue(act.empEmail1);
				// Ext.getCmp('deliveryMobilePhone').setValue(act.empMobilePhone1);
				// Ext.getCmp('deliveryPostCode').setValue(act.empPostCode1);
				// Ext.getCmp('deliveryAddr').setValue(act.empAddr1);
				// }
				// },
				// failure : function(resp, action) {
				// }
				// });
				// }
			} else {
				Ext.Msg.alert('提示', '您还没有选择，请选择...');
			}
		};

		Ext.apply(rootDefaultConfig, rootConfig);

		if (this.onlyPerson) {
			Ext.apply(baseParams, {
						type : 2
					});
		} else {
			Ext.apply(baseParams, {
						type : 1
					});
		}

		var treeLoader = new Ext.tree.TreeLoader({
			dataUrl : __cxtPath + dataUrl,
			baseParams : baseParams,
			listeners : {
				scope : this,
				'load' : function(tl, node, response) {
					Ext.each(node.childNodes, function(n) {
								if (this.lastIds.indexOf(n.id) >= 0) {
									n.getUI().toggleCheck(true);
								}
							}, this);
					if (this.specifiedselect != '') {
						Ext.each(node.childNodes, function(n) {
									if (this.specifiedselect.indexOf(n.id) >= 0) {
										n.getUI().toggleCheck(true);
									}
								}, this);
					}
				}
			}
		});

		var treepanel = new Ext.tree.TreePanel({
			region : 'center',
			autoScroll : true,
			ref : '../../treepanel',
			animate : true,
			enableDD : false,
			border : false,
			lines : true,
			useArrows : false,
			rootVisible : false,
			singleExpand : singleExpand,
			loader : treeLoader,
			root : rootDefaultConfig,

			listeners : {
				scope : this,
				'click' : function(node) {
					if (node.ui.checkbox) {
						node.getUI().toggleCheck(!node.ui.checkbox.checked);
					}
				},
				'dblclick' : function(node, e) {
					if (node.isLeaf()) {
						this.fireEvent('nodedblclick', node, e);
					}
					return;
					// 编辑联系人信息
					if (linkManDblclickFunction) {
						if (node.id.substring(0, 4) == 'empl') {
							var nodePath = node.getPath();
							var formRecord = Ext.data.Record.create([{
										name : 'id',
										type : 'string'
									}, {
										name : 'empName1',
										type : 'string'
									}, {
										name : 'empAddr1',
										type : 'string'
									}, {
										name : 'empPost1',
										type : 'string'
									}, {
										name : 'empMobilePhone1',
										type : 'string'
									}, {
										name : 'empPhone1',
										type : 'string'
									}, {
										name : 'empEmail1',
										type : 'string'
									}, {
										name : 'empPostCode1',
										type : 'string',
										convert : function(v) {
											if (v == 0 || v == '0') {
												return ''
											} else {
												return v;
											}
										}
									}]);

							var updateConsigneeFrom = new Ext.form.FormPanel({
								baseCls : 'x-plain',
								anchor : '100%',
								lableWidth : 10,
								reader : new Ext.data.JsonReader({}, formRecord),
								defaultType : 'textfield',
								defaults : {
									xtype : "textfield",
									width : 200
								},
								layout : 'form',
								items : [{
											id : 'id',
											xtype : 'hidden',
											name : 'id'
										}, {
											id : 'empName1',
											fieldLabel : modifyNameStr + "姓名",
											inputType : 'text',
											name : 'empName1',
											allowBlank : false,
											msgTarget : 'right'
										}, {
											id : 'empPhone1',
											fieldLabel : "办公电话",
											inputType : 'text',
											regex : /^[0-9]+([-]{1}[0-9]+)+$|^[0-9]*$/,
											invalidText : "办公电话格式有误，请检查！",
											name : 'empPhone1'
										}, {
											xtype : 'textfield',
											id : 'empMobilePhone1',
											fieldLabel : "手机号",
											regex : /^[0-9]{0,15}$/,
											invalidText : "手机号只能输入数字，最多15位，请检查！",
											name : 'empMobilePhone1'
										}, {
											id : 'empEmail1',
											fieldLabel : "Email",
											inputType : 'text',
											name : 'empEmail1'
										}, {
											id : 'empAddr1',
											fieldLabel : "办公地址",
											inputType : 'text',
											name : 'empAddr1'
										}, {
											id : 'empPostCode1',
											fieldLabel : "邮政编码",
											xtype : 'textfield',
											regex : /^[0-9]{6}$/,
											invalidText : "邮政编码只能输入6为数字，请检查！",
											name : 'empPostCode1'
										}, {
											id : 'empPost1',
											fieldLabel : "职务",
											inputType : 'text',
											name : 'empPost1'
										}],
								buttons : [{
									text : "确认",
									handler : function() {
										updateConsigneeFrom.getForm().submit({
											url : __cxtPath
													+ '/crmMaint.do?method=updateEmpInfo',
											success : function(form, action) {
												Ext.Msg
														.alert(
																action.result.messageTitle,
																action.result.message,
																function() {
																	if (node.id
																			.substring(
																					9,
																					node.id.length) == Ext
																			.getCmp('delivery_id')
																			.getValue()) {
																		if (modifyNameStr == '联系人') {
																			Ext
																					.getCmp('deliveryPost')
																					.setValue(updateConsigneeFrom
																							.getForm()
																							.findField('empPost1')
																							.getValue());
																			Ext
																					.getCmp('deliveryPhone')
																					.setValue(updateConsigneeFrom
																							.getForm()
																							.findField('empPhone1')
																							.getValue());
																			Ext
																					.getCmp('deliveryEmail')
																					.setValue(updateConsigneeFrom
																							.getForm()
																							.findField('empEmail1')
																							.getValue());
																			Ext
																					.getCmp('deliveryMobilePhone')
																					.setValue(updateConsigneeFrom
																							.getForm()
																							.findField('empMobilePhone1')
																							.getValue());
																			Ext
																					.getCmp('deliveryPostCode')
																					.setValue(updateConsigneeFrom
																							.getForm()
																							.findField('empPostCode1')
																							.getValue());
																			Ext
																					.getCmp('deliveryAddr')
																					.setValue(updateConsigneeFrom
																							.getForm()
																							.findField('empAddr1')
																							.getValue());
																			Ext
																					.getCmp('cEmp_name')
																					.setValue(updateConsigneeFrom
																							.getForm()
																							.findField('empName1')
																							.getValue());
																		}
																	}
																	updateConsigneeWin
																			.close();
																	treeLoader
																			.load(
																					treepanel
																							.getRootNode(),
																					function(
																							n) {
																						treepanel
																								.expandPath(nodePath);
																					});
																});
											},
											failure : function() {
											},
											method : "post",
											params : {
												empID : node.id.substring(9,
														node.id.length),
												empName : updateConsigneeFrom
														.getForm()
														.findField('empName1')
														.getValue(),
												empAddr : updateConsigneeFrom
														.getForm()
														.findField('empAddr1')
														.getValue(),
												empPost : updateConsigneeFrom
														.getForm()
														.findField('empPost1')
														.getValue(),
												empPhone : updateConsigneeFrom
														.getForm()
														.findField('empPhone1')
														.getValue(),
												empMobilePhone : updateConsigneeFrom
														.getForm()
														.findField('empMobilePhone1')
														.getValue(),
												empEmail : updateConsigneeFrom
														.getForm()
														.findField('empEmail1')
														.getValue(),
												empPostCode : updateConsigneeFrom
														.getForm()
														.findField('empPostCode1')
														.getValue()
											}
										});
									}
								}, {
									text : "取消",
									handler : function() {
										updateConsigneeWin.close();
									}
								}]
							});

							var updateConsigneeWin = new Ext.Window({
										layout : 'fit',
										modal : true,
										title : '修改' + modifyNameStr + '信息',
										frame : true,
										plain : true,
										resizable : false,
										buttonAlign : "center",
										bodyStyle : "padding:10px",
										width : 360,
										height : 270,
										layout : "form",
										defaults : {
											xtype : "textfield",
											width : 200
										},
										items : updateConsigneeFrom

									});

							updateConsigneeWin.show();

							updateConsigneeFrom.getForm().load({
								url : __cxtPath
										+ '/crmMaint.do?method=getEmpInfoById',
								params : {
									empID : node.id
											.substring(9, node.id.length)
								},
								autoLoad : true,
								success : function(form, action) {
								},
								failure : function(form, action) {
								}
							});

						}
					}
				}
			}
		});
		this.treepanel = treepanel;
		if (expandNodePath != '') {
			var expfun = this.expandNodePath;
			var tp = this.treepanel;
			Ext.Ajax.request({
						url : __cxtPath
								+ '/contractMain.do?method=getParentDeptIdByDeptId',
						success : function(response, action) {
							var action = Ext.decode(response.responseText);
							var pid = action.deptID;
							if (pid == 0) {
								var treep = tp;
								var expandp = expfun.call(this).path;
								var rootn = tp.getRootNode();
								treep.expandPath(expandp);
							} else {
								var path = expfun.call(this).path;
								var paths = path.split("/");
								var ppath = 'dept-old-' + pid;
								var p = paths[0] + '/' + paths[1] + '/' + ppath
										+ '/' + paths[2];
								var treep = tp;
								var rootn = tp.getRootNode();
								treep.expandPath(p);
							}
						},
						failure : function() {
						},
						method : "post",
						params : {
							deptID : expfun.call(this).deptid
						}
					});
		}
		
		//通过页面 人员ID  得到此人的路径  展开 选中
		if(this.getExpandPath.call(this).empId != 0){
							var t = this.treepanel;
							var empId = this.getExpandPath.call(this).empId;
							Ext.Ajax.request({
									url : __cxtPath
											+ '/orgMaint.do?method=getEmpNodePathByEmpId',
									success : function(response, action) {
										var result = Ext.decode(response.responseText);
										var nodepath = result.attributes.nodepath;
										var rootn = t.getRootNode();
													t.expandPath(nodepath,"",function(){
															var node = t.getNodeById("empl-old-"+empId);
															node.getUI().toggleCheck(true);//节点选中
													});
									},
									failure : function() {
									},
									method : "post",
									params : {
										empId : empId
									}
								});
					}
		if (singleSelect) {
			treepanel.on("checkchange", function(node) {
						selectNode = node;
						var nodes = treepanel.getChecked();
						if (nodes.length == 1) {
						} else {
							for (var i = 0; i < nodes.length; i++) {
								if (nodes[i].id != node.id) {
									nodes[i].ui.checkbox.checked = false;
								}
							}
						}
					});
		} else {
			chooseFun = function() {
				var nodes = treepanel.getChecked();
				if (nodes.length > 0) {
					array = nodes;
					this.lastIds = [];
					for (i = 0; i < nodes.length; i++) {
						this.lastIds[i] = nodes[i].id;
					}
					this.window.callback(array);
					this.triggers[0].show();
					this.window.hide();
				} else {
					Ext.Msg.alert('提示', '您还没有选择，请选择...');
				}
			};
		}

		var wintbar = [{
					xtype : 'button',
					ref : '../okBtn',
					text : '选定',
					iconCls : 'ok',
					scope : this,
					handler : chooseFun
				}, '-', {
					ref : '../cancelBtn',
					iconCls : 'remove',
					text : '取消',
					scope : this,
					handler : function() {
						this.window.hide();
					}
				}];

		if (winConfig.tbar) {
			wintbar.push('-');
			wintbar.push(winConfig.tbar);
		}

		var windowConfig = {
			callback : setValueFun,
			items : [treepanel],
			modal : modal,
			tbar : {
				xtype : 'toolbar',
				items : wintbar
			}
		};

		Ext.applyIf(windowConfig, winConfig);
		
		this.window = new Ext.ChooseOrgTreeWindow(windowConfig);
		this.oldRoot = this.rootConfig.call(this).id;
		this.window.show();

	}
});
Ext.reg('chooseOrgTreefield', Ext.ux.form.ChooseOrgTreeField);

Ext.ns('Ext');
Ext.ChooseOrgTreeWindow = Ext.extend(Ext.Window, {
			initComponent : function() {
				Ext.ChooseOrgTreeWindow.superclass.initComponent.call(this);
			},
			width : 300,
			height : 400,
			closeAction : 'hide',
			layout : 'border',
			callback : function() {
			}
		});