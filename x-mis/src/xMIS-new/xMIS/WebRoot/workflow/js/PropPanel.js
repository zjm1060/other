/*
 * 属性设置窗口 $Id: PropWin.js,
 */

Ext.util.Format.comboRenderer = function(combo) {
	return function(value) {
		var record = combo.findRecord(combo.valueField, value);
		return record
				? record.get(combo.displayField)
				: combo.valueNotFoundText;
	}
};
var Pro = Ext.data.Record.create([{
			name : 'id'
		}, {
			name : 'name'
		}, {
			name : 'assigneeType'
		}, {
			name : 'assignee'
		}, {
			name : 'assigneeDesc'
		}, {
			name : 'candidate'
		}, {
			name : 'candidateDesc'
		}, {
			name : 'condition'
		}, {
			name : 'formName'
		}, {
			name : 'form'
		}, {
			name : 'permission'
		}]);

var formTreeRoots = new Ext.util.MixedCollection();

formElemsTree = new Ext.tree.TreePanel({
			animate : true,
			enableDD : true,
			containerScroll : true,
			rootVisible : false,
			region : 'west',
			border : false,
			elementInfos : null,
			width : 150,
			split : true,
			autoScroll : true,
			lines : true,
			activeNode : null,
			removeRoot : function() {
				while (this.root.childNodes.length > 0) {
					this.root.childNodes[0].remove();
				}
			},
			loadTreeData : function(elem) {
				this.removeRoot();
				var formElem = new Ext.tree.TreeNode({
							text : "表单元素",
							id : 'formElement',
							allowDrag : false,
							allowDrop : false,
							expanded : true
						});
				this.root.appendChild(formElem);
				var btnElem = new Ext.tree.TreeNode({
							text : "按钮",
							id : 'btnElement',
							allowDrag : false,
							allowDrop : false,
							expanded : true
						});
				this.root.appendChild(btnElem);
				appendTreeNode(formElem, elem.formElems, 1);
				appendTreeNode(btnElem, elem.buttons, 2);
				if (elem.embedReps.length > 0) {
					var embedRep = new Ext.tree.TreeNode({
								text : "嵌入报表",
								id : 'embedRep',
								allowDrag : false,
								allowDrop : false,
								expanded : true
							});
					this.root.appendChild(embedRep);
					appendTreeNode(embedRep, elem.embedReps, 3);
				}
				if (elem.tabPages.length > 0) {
					var tabPage = new Ext.tree.TreeNode({
								text : "标签页",
								id : 'tabPage',
								allowDrag : false,
								allowDrop : false,
								expanded : true
							});
					this.root.appendChild(tabPage);
					appendTreeNode(tabPage, elem.tabPages, 4);
				}
			}
		});
formElemsTree.on('click', function(node, e) {
			e.stopEvent();
			if (node.attributes.elementType) {
				var stepid = formPanel.getForm().getFieldValues().id;
				if (formElemsTree.activeNode == null
						|| formElemsTree.activeNode == undefined) {
					formElemsTree.activeNode = node;
					viewElementInfo(stepid, node);
				} else {
					saveElementInfo(stepid, formElemsTree.activeNode);
					viewElementInfo(stepid, node);
					formElemsTree.activeNode = node;
				}
			}
		});
formElemsTree.setRootNode(new Ext.tree.TreeNode({
			text : '表单元素',
			allowDrag : false,
			allowDrop : false,
			expanded : true,
			formid : "0"
		}));

var detailPanel = new Ext.Panel({
			region : 'center',
			split : true,
			bodyStyle : 'padding:5 5 5 5;border-width:0px;border-left-width:1px',
			margin : '5 5 5 5',
			items : [{
						xtype : 'fieldset',
						title : '属性设置',
						autoHeight : true,
						show : true,
						layout : 'fit',
						items : [{
									xtype : 'radiogroup',
									id : 'dispTypeGroup',
									items : [{
												boxLabel : '默认',
												name : 'rb-auto',
												inputValue : 0,
												checked : true
											}, {
												boxLabel : '只读',
												name : 'rb-auto',
												inputValue : 1
											}, {
												boxLabel : '编辑',
												name : 'rb-auto',
												inputValue : 2
											}, {
												boxLabel : '隐藏',
												name : 'rb-auto',
												inputValue : 3
											}]
								}]
					}, {
						xtype : 'fieldset',
						title : '样式设置',
						autoHeight : true,
						show : true,
						layout : 'fit',
						items : [{
									id : 'formElemStype',
									xtype : 'textarea',
									name : 'style'
								}]
					}],
			lines : true
		});

var formPanel = new Ext.FormPanel({
	height : 400,
	labelAlign : 'right',
	labelWidth : 60,
	border : false,
	reader : new Ext.data.JsonReader({}, Pro),
	layout : 'fit',
	formRecord : null,
	loadRecord : function(record) {
		this.setValues(record.data);
		this.formRecord = record;
		if (record.data.form != null) {
			this.findField("setpform_disp").setValue({
						id : record.data.form,
						name : record.data.formName
					});
		} else {
			this.findField("setpform_disp").setValue({
						id : '',
						name : ''
					});
		}
		if (record.data.assignee != null) {
			this.findField("assignee_disp").setValue({
						id : record.data.assignee,
						name : record.data.assigneeDesc
					});
		} else {
			this.findField("assignee_disp").setValue({
						id : '',
						name : ''
					});
		}

        var permission = record.data.permission;
        var perItems = Ext.getCmp("permissionItems");
        var items = perItems.items;
        for(var i=0;i<items.length;i++){
           var checked = (permission.indexOf(i+1+'') != -1);
           if(items[i]){
              items[i].checked = checked;
           } else {
             items.get(i).el.dom.checked = checked;
           }
        }
		return this;
	},
	items : [new Ext.TabPanel({
		border : false,
		activeTab : 0,
		id : 'nodeProTabs',
		listeners : {
			"tabchange" : function(tdemo, curItem, e) {
				if (curItem.id == 'formFieldset') {
					var formid = Ext.get("setpform").getValue();
					if (formid == "null") {
						formid = null;
					}
					var isNull = (formid == "" || formid == null || formid == "undefined");
					if (isNull) {
						formElemsTree.removeRoot();
						initTFNodeInfo();
					} else {
						fillFormElements(formid);
					}
				}
			}
		},
		items : [{
			title : '基本属性',
			layout : 'form',
			padding : '5 5 5 5',
			items : [{
						xtype : 'hidden',
						name : 'id'
					}, {
						xtype : 'textfield',
						fieldLabel : '名称',
						name : 'name'
					}, {
						title : '流转信息',
						id : 'transitionFieldset',
						xtype : 'fieldset',
						layout : 'form',
						padding : '5 0 0 0',
						items : [{
									xtype : 'textfield',
									fieldLabel : '流转条件',
									anchor : '95%',
									name : 'condition'
								}]
					}, {
						title : '任务属性',
						xtype : 'fieldset',
						id : 'taskFieldset',
						layout : 'form',
						labelWidth : 85,
						items : [{
									xtype : 'radiogroup',
									fieldLabel : '任务类型',
									name : 'assigneeType',
									items : [{
												xtype : 'radio',
												name : 'assigneeType',
												boxLabel : '单人任务',
												inputValue : 'singleuser',
												checked : true
											}, {
												xtype : 'radio',
												name : 'assigneeType',
												boxLabel : '多人任务',
												inputValue : 'multiuser'
											}, {
												xtype : 'radio',
												name : 'assigneeType',
												inputValue : 'group',
												boxLabel : '组任务'
											}]
								}, new Ext.ux.form.SearchField({
									anchor : '100%',
									name : 'assignee_disp',
									fieldLabel : '任务人/组',
									hiddenName : 'assignee',
									hiddenValue : '',
									listeners : {
										"afterrender" : function(search,
												search1, e) {
											formPanel.form
													.findField("assignee_disp")
													.setValue({
														id : formPanel.form.formRecord.data.assignee,
														name : formPanel.form.formRecord.data.assigneeDesc
													});
										}
									},
									onTrigger2Click : function(e) {
										var imgEl = this.trigger.last();
										imgEl.dom.setAttribute("elemname",
												"assignee");
										this.el.dom.id = "assignee_Disp";
										AppUtil
												.popSetDic2(
														"sysmng.report.do?subSys=sysmng&repID=237&isQuery=1&isTopBar=0&isBottomBar=0&isShowCnd=0",
														imgEl.dom);
									}
								}), new Ext.ux.form.SearchField({
									anchor : '100%',
									name : 'candidate_disp',
									fieldLabel : '候选人/组',
									hiddenName : 'candidate',
									hiddenValue : '',
									listeners : {
										"afterrender" : function(search,
												search1, e) {
											formPanel.form
													.findField("candidate_disp")
													.setValue({
														id : formPanel.form.formRecord.data.candidate,
														name : formPanel.form.formRecord.data.candidateDesc
													});
										}
									},
									onTrigger2Click : function(e) {
										var imgEl = this.trigger.last();
										imgEl.dom.setAttribute("elemname",
												"candidate");
										this.el.dom.id = "candidate_Disp";
										AppUtil
												.popSetDic2(
														"sysmng.report.do?subSys=sysmng&repID=237&isQuery=1&isTopBar=0&isBottomBar=0",
														imgEl.dom);
									}
								})]
					}, {
						title : '发起人',
						id : 'sponsorFieldset',
						xtype : 'fieldset',
						layout : 'form',
						padding : '5 0 0 0',
						items : [{
									xtype : 'textfield',
									fieldLabel : '发起人',
									anchor : '95%',
									name : 'sponsor'
								}]
					}, {
						title : '权限选项',
						xtype : 'fieldset',
						layout : 'fit',
						items : [{
									xtype : 'checkboxgroup',
									id : 'permissionItems',
									columns : 4,
									items : [{
												inputValue : 1,
												boxLabel : '取回',
												checked : false,
												name : 'permission'
											}, {
												inputValue : 2,
												boxLabel : '退回',
												checked : false,
												name : 'permission'
											},{
												inputValue : 3,
												boxLabel : '查看流程图',
												checked : false,
												name : 'permission'
											}]
								}]
					}, {
						title : '待办显示',
						id : 'todoFieldset',
						xtype : 'fieldset',
						layout : 'fit',
						padding : '5 0 0 0',
						items : [{
									xtype : 'textarea',
									anchor : '95%',
									name : 'todo'
								}]
					}]
		}, {
			title : '表单信息',
			id : 'formFieldset',
			layout : 'fit',
			items : [{
				id : 'form',
				layout : 'border',
				border : false,
				bodyborder : false,
				items : [{
					region : 'north',
					border : true,
					bodyborder : false,
					layout : 'form',
					height : 28,
					padding : '2 2 2 2',
					bodyStyle : 'border-left-width:0px;border-right-width:0px;',
					items : [new Ext.ux.form.SearchField({
						fieldLabel : '选定表单',
						anchor : '100%',
						name : 'setpform_disp',
						hiddenName : 'setpform',
						hiddenValue : '',
						onHiddenChange : function(e) {
							fillFormElements(Ext.get("setpform").getValue());
						},
						listeners : {
							"afterrender" : function(search, search1, e) {
								formPanel.form.findField("setpform_disp")
										.setValue({
											id : formPanel.form.formRecord.data.form,
											name : formPanel.form.formRecord.data.formName
										});
							}
						},
						onTrigger2Click : function(e) {
							var imgEl = this.trigger.last();
							imgEl.dom.setAttribute("elemname", "setpform");
							this.el.dom.id = "setpform_Disp";
							AppUtil
									.popSetDic2(
											"sysmng.report.do?subSys=console&repID=223&isQuery=1",
											imgEl.dom);
						}
					})]
				}, formElemsTree, detailPanel]
			}]
		}]
	})]
});
PropWin = function() {
	PropWin.superclass.constructor.call(this, {
				stateful : false, // 不被stateManager管理
				title : '属性设置',
				width : 500,
				closeAction : 'hide',
				shadow : false,
				activeCell : null,
				items : [formPanel],
				hide : function() {
					PropWin.superclass.hide.call(this);
					if (this.activeCell != null) {
						saveSetupInfo(this.activeCell);
					}
				}
			});
}
Ext.extend(PropWin, Ext.Window);

PropWin.prototype.showProp = function(graph) {
	formPanel.graph = graph;
	var cell = graph.getSelectionCell();
	if (this.activeCell != null) {
		saveSetupInfo(this.activeCell);
	}
	this.activeCell = cell;
	initProPanel(cell);
	if (this.hidden) {
		this.show();
	}
}

function saveElementInfo(setupid, node) {
	if (node == null || node == undefined)
		return;
	var elementInfos = setupFormInfos.get(setupid);
	if (elementInfos == undefined) {
		setupFormInfos.add(setupid, new Ext.util.MixedCollection());
		elementInfos = setupFormInfos.get(setupid);
	}
	var dispType = Ext.getCmp("dispTypeGroup").items.get(0).getGroupValue();
	var style = Ext.getCmp("formElemStype").getValue();
	if (dispType == 0 && style == "") {
		elementInfos.removeKey(node.id);
	} else {
		elementInfos.add(node.id, {
					id : node.id,
					dispType : dispType,
					style : style,
					type : node.attributes.elementType
				});
	}
}

function viewElementInfo(setupid, node) {
	var elementInfos = setupFormInfos.get(setupid);
	if (elementInfos == undefined || elementInfos == null)
		return;
	var elemInfo = elementInfos.get(node.id);
	var dispType = Ext.getCmp("dispTypeGroup").items.get(0);
	var style = Ext.getCmp("formElemStype");
	Ext.getCmp("dispTypeGroup").reset();
	if (elemInfo == undefined) {
		style.setValue("");
	} else {
		style.setValue(elemInfo.style);
		Ext.getCmp("dispTypeGroup").items.get(0).setValue(false);
		Ext.getCmp("dispTypeGroup").items.get(parseInt(elemInfo.dispType))
				.setValue(true);
	}

}

function initTFNodeInfo() {
	Ext.getCmp("formElemStype").setValue("");
	Ext.getCmp("dispTypeGroup").reset();
	var form = formPanel.getForm();
	var setupid = form.findField("id").getValue();
	var elementInfos = setupFormInfos.get(setupid);
	if (elementInfos != undefined) {
		// elementInfos.clear();
	}
}

function appendTreeNode(parentNode, subNodes, type) {
	for (var i = 0; i < subNodes.length; i++) {
		var elemNode = new Ext.tree.TreeNode({
					text : subNodes[i].text,
					id : subNodes[i].name,
					allowDrag : false,
					allowDrop : false,
					expanded : true,
					elementType : type
				});
		parentNode.appendChild(elemNode);
	}
}

function initProPanel(cell) {
	var style = cell.getStyle();
	hideAll();
	if (style != null) {
		Ext.getCmp('nodeProTabs').unhideTabStripItem(1);
		if (style.indexOf('step;') == 0) {
			Ext.getCmp('taskFieldset').show();
			Ext.getCmp('todoFieldset').show();
			Ext.getCmp('nodeProTabs').setActiveTab(0);
		} else if (style.indexOf('start;') == 0) {
			Ext.getCmp('todoFieldset').show();
			Ext.getCmp('sponsorFieldset').show();
			Ext.getCmp('nodeProTabs').setActiveTab(0);
		} else if (style.indexOf('end;') == 0) {
			Ext.getCmp('nodeProTabs').hideTabStripItem(1);
			Ext.getCmp('nodeProTabs').setActiveTab(0);
		}
	} else if (cell.isEdge()) {
		Ext.getCmp('transitionFieldset').show();
		Ext.getCmp('nodeProTabs').hideTabStripItem(1);
	}
	var pro = new Pro({
				id : cell.id,
				name : cell.value,
				assigneeType : cell.assigneeType,
				assignee : cell.assignee,
				assigneeDesc : cell.assigneeDesc,
				candidate : cell.candidate,
				candidateDesc : cell.candidateDesc,
				condition : cell.condition,
				form : cell.form,
				formName : cell.formName,
				sponsor : cell.sponsor,
				todo : cell.todo,
                permission:cell.permission
			});
	formPanel.getForm().loadRecord(pro);

}
// candidate

function filterNull(str) {
	if (str == null || str == "null") {
		str = "";
	}
	return str;
}

function saveSetupInfo(cell) {
	saveElementInfo(cell.id, formElemsTree.activeNode);
	formElemsTree.activeNode = null;
	var form = formPanel.getForm();
	var assigneeEl = Ext.get("assignee");
	if (assigneeEl) {
		var assignee = filterNull(assigneeEl.getValue());
		var assigneeDesc = filterNull(form.findField("assignee_disp")
				.getValue());
		cell.assigneeDesc = assigneeDesc;
		cell.assignee = assignee;
	}

	var candidateEl = Ext.get("candidate");
	if (candidateEl) {
		var candidate = filterNull(candidateEl.getValue());
		var candidateDesc = filterNull(form.findField("candidate_disp")
				.getValue());
		cell.candidateDesc = candidateDesc;
		cell.candidate = candidate
	}

	cell.condition = filterNull(form.findField("condition").getValue());
	formPanel.graph.getModel()
			.setValue(cell, form.findField("name").getValue());
	var assigneegrGoup = form.findField("assigneeType").getValue();
	if (assigneegrGoup != null) {
		cell.assigneeType = assigneegrGoup.getGroupValue();
	}
	if (Ext.get("setpform")) {
		cell.form = Ext.get("setpform").getValue();
		cell.formName = form.findField("setpform_disp").getValue();
	}
	var sponsor = form.findField('sponsor').getValue();
	cell.sponsor = sponsor;
	var todo = form.findField('todo').getValue();
	cell.todo = todo;
	var permission = Ext.getCmp('permissionItems').getValue();
	if (permission != null) {
		var vs = [];
		for (i = 0; i < permission.length; i++) {
			vs.push(permission[i].inputValue);
		}
		cell.permission = vs.join(",");
	}
	formPanel.graph.mainPanel.modified = true;
}

function hideAll() {
	Ext.getCmp('taskFieldset').hide();
	Ext.getCmp('transitionFieldset').hide();
	Ext.getCmp('todoFieldset').hide();
	Ext.getCmp('sponsorFieldset').hide();
}

function fillFormElements(formid) {
	var elems = formTreeRoots.get(formid);
	if (elems != null) {
		formElemsTree.loadTreeData(elems);
		initTFNodeInfo();
		return;
	}
	formElemsTree.root.formid = formid;
	var tmps = formid.split('.');
	var actionUrl = "console.wfFormAction.do?subSys=" + tmps[0] + "&formID="
			+ tmps[1];
	var myMask = new Ext.LoadMask(Ext.getBody(), {
				msg : "获取表单数据中..."
			});
	myMask.show();
	var requestConfig = {
		url : actionUrl,
		jsonData : {},
		method : 'post',
		success : function(response, opts) {
			var result = eval("(" + response.responseText + ")");
			if (result.err == undefined || result.err == null) {
				formElemsTree.loadTreeData(result.elems);
				initTFNodeInfo();
				formTreeRoots.add(formid, result.elems);
			} else {
				var text = '保存流程信息失败:<br>' + result.err.msg;
				Ext.Msg.alert('提示', text);
			}
			myMask.hide();
		}
	}
	Ext.Ajax.request(requestConfig);
}
