
/**
 * @class com.bitc.xmis.Window.SendMessage 发送短信
 * @extends Ext.Window
 * 
 * isHidden : '选择公司'按钮是否隐藏 default false empID : 发送人（一般就是当前登录人）
 */
Ext.ns('com.bitc.xmis.Window');
com.bitc.xmis.Window.SendMessageWin = Ext.extend(Ext.Window, {
	id : 'sendMessageWin',
	width : 630,
	height : 380,
	title : '发送短信',
	isHidden : false,
	empID : '',
	initComponent : function() {
		var empID = this.empID;
		var compWin;

		var empids = new Array();// 收件人

		var comempName;
		var compRecord = Ext.data.Record.create([{
					name : 'compid',
					mapping : '公司ID'
				}, {
					name : 'compname',
					mapping : '公司名称'

				}, {
					name : 'abb',
					mapping : '名称缩写'
				}, {
					name : 'nodePath'
				}]);

		var compStore = new Ext.data.Store({
					url : __cxtPath
							+ '/contractMain.do?method=getCompanyListStroe',
					baseParams : {
						start : 0,
						limit : 20,
						sort : '公司名称',
						dir : 'ASC'
					},
					reader : new Ext.data.JsonReader({
								root : 'rows',
								totalProperty : 'totalCount'
							}, compRecord)
				});

		var compsm = new Ext.grid.CheckboxSelectionModel({
					header : '',
					checkOnly : true,
					singleSelect : true
				});

		var compColumn = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
							width : 35
						}), {
					header : '公司ID',
					hidden : true,
					dataIndex : 'compid'
				}, compsm, {
					header : '公司名称',
					width : 300,
					sort : true,
					dataIndex : 'compname'
				}, {
					header : '简称',
					width : 250,
					sort : true,
					dataIndex : 'abb'
				}, {
					header : 'nodepath',// 查找的指定人员在公司组织机构树的nodepath
					hidden : true,
					dataIndex : 'nodePath'
				}]);

		// 公司信息查询--通过名称、简称
		var getCompByCompNameInklingSearch = new Ext.ux.InklingSearchField({
			width : 120,
			store : compStore,
			scope : this,
			tooltip : {
				title : '模糊查询',
				text : '输入公司名称或简称进行模糊查询'
			},
			inklings : [{
						name : '公司名称',
						op : 'like',
						type : 'string'
					}, {
						name : '名称缩写',
						op : 'like',
						type : 'string'
					}],
			onTrigger1Click : function() {
				this.el.dom.value = '';
				this.triggers[0].hide();

				this.store.baseParams = this.store.baseParams || {};
				delete this.store.baseParams['inklings'];
				Ext.ux.InklingSearchField.superclass.onTrigger1Click.call(this);
			},
			onTrigger2Click : function() {
				if (this.getRawValue() != null
						&& this.getRawValue().trim() != '') {
					var params = '';
					Ext.iterate(this.inklings, function(item, index, allItems) {
								params += item.name + ',' + item.op + ','
										+ item.type;
								if (index < allItems.length - 1) {
									params += '|';
								}
							}, this);
					this.store.baseParams = this.store.baseParams || {};
					this.store.baseParams['inklings'] = params;
					var v = Ext.util.Format.trim(this.getRawValue());
					if (v.length < 1) {
						this.onTrigger1Click();
						return;
					}
					var o = {
						start : 0
					};
					this.store.baseParams = this.store.baseParams || {};
					this.store.baseParams[this.paramName] = v;
					this.store.reload({
								params : o,
								callback : function(record, options, success) {
								}
							});
					this.hasSearch = true;
					this.triggers[0].show();
				} else {
					Ext.Msg.alert("提示", "查询条件不能为空！");
				}
			},
			listeners : {
				'focus' : function() {
					getCompByEmpNameInklingSearch.onTrigger1Click();
				}
			}
		});

		// 公司信息查询--通过员工姓名
		var getCompByEmpNameInklingSearch = new Ext.ux.form.SearchField({
			width : 120,
			emptyText : '请输入员工姓名',
			tooltip : {
				title : '查询',
				text : '输入员工姓名查找所在公司信息'
			},
			onTrigger1Click : function() {
				this.el.dom.value = '';
				this.triggers[0].hide();
				compStore.proxy
						.setUrl(
								__cxtPath
										+ '/contractMain.do?method=getCompanyListStroe',
								true);
				this.store = compStore;
				Ext.ux.InklingSearchField.superclass.onTrigger1Click.call(this);
			},
			onTrigger2Click : function() {
				var input = this.el.dom.value;
				if (input != null && '' != input.trim()
						&& this.emptyText != input) {
					comempName = input;
					compStore.setBaseParam('empName', input);
					compStore.proxy
							.setUrl(
									__cxtPath
											+ '/orgMaint.do?method=getCompStoreByEmpName',
									true);
					compStore.load({
								params : {
									start : 0,
									limit : 20,
									empName : input
								}
							});
					this.hasSearch = true;
					this.triggers[0].show();
				} else {
					Ext.MessageBox.alert("提示", "查询条件不能为空！");
				}
			},
			listeners : {
				'focus' : function() {
					getCompByCompNameInklingSearch.onTrigger1Click();
				}
			}
		});

		// 公司信息--compGrid
		var compGrid = new Ext.grid.GridPanel({
			region : 'center',
			store : compStore,
			autoScroll : true,
			cm : compColumn,
			sm : compsm,
			bbar : new Ext.PagingToolbar({
						pageSize : 20,
						store : compStore,
						displayInfo : true,
						displayMsg : '当前显示{0}-{1}条记录/共{2}条记录',
						emptyMsg : '无显示数据'
					}),
			tbar : [
					{
						xtype : 'button',
						iconCls : 'ok',
						text : '选定',
						scope : this,
						handler : function() {
							// 选定公司，改变左侧公司树的根节点，重新遍历出选定公司的组织机构树
							var record = compGrid.getSelectionModel()
									.getSelected();
							if (Ext.isEmpty(record)) {
								Ext.MessageBox.alert("提示", "请先选择");
							} else {
								compWin.hide();
								var compID = record.data.compid;
								var compName = record.data.compname;
								compTreeId = compID;
								compTreeName = compName;

								if (Ext.isEmpty(comempName)) {
									if (compTree.getLoader().baseParams.rootFilter != compID) {
										compTree.getLoader().baseParams.rootFilter = compID;
										compTree.root.reload();
									}
								} else {
									var path = "/root-" + record.data.nodePath;
									var userID = path.substring(path
											.lastIndexOf("/")
											+ 1);

									if (compTree.getLoader().baseParams.rootFilter == compID) {
										compTree.expandPath(path, "",
												function() {
													compTree
															.getNodeById(userID)
															.select();
												});
									} else {
										compTree.getLoader().baseParams.rootFilter = compID;
										compTree.getLoader().load(
												compTree.getRootNode(),
												function() {
													compTree.expandPath(path,
															"", function() {
																compTree
																		.getNodeById(userID)
																		.select();
															});
												});
									}
								}
								compGrid.getSelectionModel()
										.clearSelections(false);
							}
						}
					},
					{
						xtype : 'button',
						iconCls : 'remove',
						text : '取消',
						handler : function() {
							compWin.hide();
							comempName = "";
							compGrid.getSelectionModel().clearSelections(false);
						}
					}, '按公司名称查询：', getCompByCompNameInklingSearch, '  ',
					'按员工姓名查询：',
					getCompByEmpNameInklingSearch]
		});

		var compTree = new com.bitc.xmis.OrgTreePanel({
			title : '公司组织机构树',
			region : 'west',
			height : 350,
			width : 230,
			split : true,
			checkable : true,
			singleCheck : false,
			rootFilter : 1,// 默认中水科技
			attributor : 'treeNodeEmailAttributor',
			leafLevel : com.bitc.xmis.OrgTree.LEVEL.EMP,
			autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.COMP,
			tbar : [{
						text : '查询',// 公司查询
						hidden : this.isHidden,
						iconCls : 'searchIcon',
						handler : function() {
							if (compWin) {
								getCompByCompNameInklingSearch
										.onTrigger1Click();
								getCompByEmpNameInklingSearch.onTrigger1Click();
								compWin.show();
							}
							if (!compWin) {
								compStore.load();
								compWin = new Ext.Window({
											title : '公司信息',
											height : 400,
											width : 650,
											closeAction : 'hide',
											layout : 'border',
											items : [compGrid]
										}).show();
							}
						}
					}],
			listeners : {
				'checkchange' : function() {

					var nodes = compTree.getChecked();
					var length = nodes.length;
					if (length == 0) {
						Ext.Msg.alert("提示", "请选择人员！");
					} else {
						var a = "";
						// var havenonum = "";
						for (var i = 0; i < length; i++) {
							if (i == 0) {
								a = nodes[i].attributes.attributes.name;
							} else {
								a = a + ', '
										+ nodes[i].attributes.attributes.name;
							}
							// if (Ext
							// .isEmpty(nodes[i].attributes.attributes.mobileNo))
							// {
							// havenonum = havenonum + ', '
							// + nodes[i].attributes.attributes.name;
							// }
							empids.push(nodes[i].attributes.attributes.id);
						}
						Ext.getCmp('recipients').setValue(a);
						// if (havenonum.length > 0) {
						// Ext.Msg.alert("提示", havenonum.substring(1)
						// + "没有手机号码信息");
						// }
					}

				}
			}
		});

		this.layout = 'border';
		this.items = [compTree, {
			xtype : 'panel',
			layout : 'border',
			region : 'center',
			items : [{
				xtype : 'panel',
				layout : 'form',
				region : 'north',
				border : false,
				height : 120,
				items : [{
					xtype : 'textarea',
					fieldLabel : '收件人',
					name : 'recipients',
					id : 'recipients',
					readOnly : true,
					style : 'padding :0px 0px 0px 0px; margin: 20 0 0 -50px;!important',
					heght : 100,
					width : 280
				}]
			}, {
				xtype : 'panel',
				layout : 'form',
				region : 'center',
				border : false,
				items : [{
							xtype : 'textarea',
							fieldLabel : '内容',
							name : 'message',
							id : 'message',
							style : ' margin: 20 0 0 -50px;!important',
							height : 150,
							width : 280,
							listeners : {
								'render' : function(c) {
									Ext.QuickTips.register({
												target : c.getEl(),
												text : '"发送内容"最多可输入50字'
											});
								}
							}
						}

						, {
							xtype : 'panel',
							border : false,
							height : 50,
							buttonAlign : 'center',
							buttons : [{
								xtype : 'button',
								text : '发送',
								handler : function() {
									var nodes = compTree.getChecked();
									var length = nodes.length;
									var havenonum = "";
									for (var i = 0; i < length; i++) {
										if (Ext
												.isEmpty(nodes[i].attributes.attributes.mobileNo)) {
											havenonum = havenonum
													+ ', '
													+ nodes[i].attributes.attributes.name;
										}
									}
									if (havenonum.length > 0) {
										Ext.Msg.alert("提示", havenonum
														.substring(1)
														+ "没有手机号码信息",
												function() {

													if ((!Ext.isEmpty(Ext
															.getCmp('message')
															.getValue()))
															&& (!Ext
																	.isEmpty(Ext
																			.getCmp('recipients')
																			.getValue()))) {

														Ext.Ajax.request({
															url : __cxtPath
																	+ '/orgMaint.do?method=sendMessage',
															params : {
																empids : empids
																		.join(','),
																text : Ext
																		.getCmp('message')
																		.getValue()
															},
															success : function(
																	response,
																	opt) {
																var obj = Ext
																		.decode(response.responseText);
																Ext.Msg
																		.alert(
																				obj.messageTitle,
																				obj.message,
																				function() {
																					Ext
																							.getCmp('message')
																							.setValue('');
																					Ext
																							.getCmp('recipients')
																							.setValue('');
																					var nodes = compTree
																							.getChecked();
																					for (var i = 0; i < nodes.length; i++) {
																						nodes[i]
																								.getUI().checkbox.checked = false;
																					}
																				});
															},
															failure : function(
																	response,
																	opt) {
																var obj = Ext
																		.decode(response.responseText);
																Ext.Msg
																		.alert(
																				obj.messageTitle,
																				obj.message);
															}
														});
													} else {
														Ext.MessageBox
																.alert("提示",
																		"请选择收件人，并填写发送内容");
													}
												});
									}
								}
							}]
						}]
			}]
		}];

		com.bitc.xmis.Window.SendMessageWin.superclass.initComponent.call(this);
	}
});

Ext.ns('com.bitc.xmis.util');
com.bitc.xmis.util.showSendMessageWin = function(empID, isHidden) {
	if (Ext.getCmp('sendMessageWin')) {
		Ext.getCmp('sendMessageWin').close();
	}
	new com.bitc.xmis.Window.SendMessageWin({
				isHidden : isHidden,
				empID : empID
			}).show();
}