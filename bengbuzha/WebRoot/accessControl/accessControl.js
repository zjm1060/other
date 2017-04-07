Ext.onReady(function() {
	Ext.QuickTips.init();
	var exportname = '蚌埠闸通讯录';
	var compWin;
	var empID;// 人员ID
	var emproleID;// 人员角色ID
	var selectrowlistener = function(csm, rowIndex, keepExisting, record) {
		if (record.get('roleID') == '-5' || record.get('roleID') == '-6') {
			return false;
		}
		return true;
	};

	function loadGYS(empid) {
		storeGYS.load();
		Ext.Ajax.request({
					url : __cxtPath + '/crmMaint.do?method=getStaffRoles',
					params : {
						userID : empid
					},
					success : function(response, action) {
						var res = Ext.decode(response.responseText);
						var rows = res.rows;
						var records = new Array();
						for (var k = 0; k < rows.length; k++) {
							var jid = rows[k].jID;
							records.push(storeGYS.getById(jid));
						}
						var selectionModel = Ext.getCmp('supplierGird')
								.getSelectionModel();
						selectionModel.un('beforerowselect', selectrowlistener);
						selectionModel.selectRecords(records, false);
						selectionModel.on('beforerowselect', selectrowlistener);
					}
				});
	}
	var compTree = new com.bitc.xmis.OrgTreePanel({
				ddGroup : 'email-dd-group',
				title : '公司组织机构树',
				region : 'center',
				// height : 230,
				width : 220,
				enableDrag : true,
				split : true,
				rootFilter : 1,// 默认中水科技
				attributor : 'treeNodeEmailAttributor',
				leafLevel : com.bitc.xmis.OrgTree.LEVEL.EMP,
				autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.COMP,
				tbar : [{/*
							text : '查询',// 公司查询
							iconCls : 'searchIcon',
							handler : function() {
								if (compWin) {
									getCompByCompNameInklingSearch
											.onTrigger1Click();
									getCompByEmpNameInklingSearch
											.onTrigger1Click();
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
						*/}],
				listeners : {
					'click' : function(node) {
						if (!node.isLeaf()) {
							return;
						}
						empID = node.id.substring(node.id.length - 4);
						Ext.getCmp('isExecutedName').setValue('正在对<font color="red">'+node.text+'</font>的权限进行修改');
						Ext.getCmp('isExecutedName1').setValue('正在对<font color="red">'+node.text+'</font>的权限进行修改');
						sm.clearSelections();
						loadGYS(empID);
						storeXM.load({
									params : {
										userID : empID.substring(empID.length
												- 4)
									}
								});

						storeXM.on('load', function(store, records, options) {
									sm2.clearSelections();
									params : {
										userID : empID.substring(empID.length
												- 4);
									}
									Ext.each(records, function(rec) {
												if (rec.get('JSID')) {
													sm2.selectRow(
															store.indexOf(rec),
															true);
												}
											});
								});

					}

				}
			});
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
				url : __cxtPath + '/contractMain.do?method=getCompanyListStroe',
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
					Ext.ux.InklingSearchField.superclass.onTrigger1Click
							.call(this);
				},
				onTrigger2Click : function() {
					if (this.getRawValue() != null
							&& this.getRawValue().trim() != '') {
						var params = '';
						Ext.iterate(this.inklings, function(item, index,
										allItems) {
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
									callback : function(record, options,
											success) {
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
			compStore.proxy.setUrl(__cxtPath
							+ '/contractMain.do?method=getCompanyListStroe',
					true);
			this.store = compStore;
			Ext.ux.InklingSearchField.superclass.onTrigger1Click.call(this);
		},
		onTrigger2Click : function() {
			var input = this.el.dom.value;
			if (input != null && '' != input.trim() && this.emptyText != input) {
				comempName = input;
				compStore.setBaseParam('empName', input);
				compStore.proxy.setUrl(__cxtPath
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
						var record = compGrid.getSelectionModel().getSelected();
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
									compTree.expandPath(path, "", function() {
												compTree.getNodeById(userID)
														.select();
											});
								} else {
									compTree.getLoader().baseParams.rootFilter = compID;
									compTree.getLoader().load(
											compTree.getRootNode(), function() {
												compTree.expandPath(path, "",
														function() {
															compTree
																	.getNodeById(userID)
																	.select();
														});
											});
								}
							}
							compGrid.getSelectionModel().clearSelections(false);
						}
					}
				}, {
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

	var sm = new Ext.grid.CheckboxSelectionModel({
				header : '',
				handleMouseDown : Ext.emptyFn,
				deselectRow : function(index, preventViewNotify) {
					if (this.isLocked()) {
						return;
					}
					if (this.last == index) {
						this.last = false;
					}
					if (this.lastActive == index) {
						this.lastActive = false;
					}
					var r = this.grid.store.getAt(index);
					if (r) {
						if (r.get('roleID') == '-5' || r.get('roleID') == '-6') {
							return false;

						}
						this.selections.remove(r);
						if (!preventViewNotify) {
							this.grid.getView().onRowDeselect(index);
						}
						this.fireEvent('rowdeselect', this, index, r);
						this.fireEvent('selectionchange', this);
					}
				}
			});
	/**
	 * 最大供应商
	 */
	var maximumGYS = new Ext.grid.ColumnModel([sm, {
				header : '角色ID',
				dataIndex : 'roleID',
				hidden : true,
				width : 230
			}, {
				header : '角色名称',
				dataIndex : 'roleName',
				sortable : false,
				width : 200
			}, {
				header : '角色描述',
				dataIndex : 'description',
				sortable : false,
				width : 400
			}]);
	var reRecordGYS = Ext.data.Record.create([{
				name : 'roleID'
			}, {
				name : 'roleName'
			}, {
				name : 'description'
			}]);
	var storeGYS = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/securityMaint.do?method=getSecurityRoles',// 查询角色
							method : "POST"
						}),
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount',
							idProperty : 'roleID'
						}, reRecordGYS),
				remoteSort : true,
				baseParams : {
					start : 0,
					limit : 20
				}
			});

	var supplierGird = new Ext.grid.GridPanel({
		region : 'center',
		id : 'supplierGird',
		title : '人员角色',
		border : false,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : storeGYS,
		cm : maximumGYS,
		sm : sm,
		tbar : {
			xtype : 'toolbar',
			items : [{
				xtype : 'button',
				text : '保存',
				iconCls : 'ok',
				ref : '../okButton',
				handler : function() {
					if (Ext.isEmpty(empID)) {
						Ext.MessageBox.alert("提示", "请先选择人员");
					} else {

						var record = supplierGird.getSelectionModel()
								.getSelections();
						if (Ext.isEmpty(record)) {
							Ext.MessageBox.confirm('提示',
									'没有分配用户角色，点击“是”按钮，将取消此人所有角色权限，是否确定提交？', function(
											btn, text) {
										if (btn == 'yes') {


											var mycars = new Array();
											for (var i = 0; i < record.length; i++) {
												mycars[i] = record[i].data.roleID;
											}
											Ext.Ajax.request({
												url : __cxtPath
														+ '/crmMaint.do?method=doStaffRoles',
												params : {
													userID : empID
															.substring(empID.length
																	- 4),
													mycars : mycars.join(','),
													ID : 'bbz-'
															+ empID
																	.substring(empID.length
																			- 4)
															+ '-' + mycars
												},
												success : function(response,
														action) {
													var rows = Ext
															.decode(response.responseText);
													Ext.Msg.alert(
															rows.messageTitle,
															rows.message,
															function() {
																sm
																		.clearSelections();
																loadGYS(empID
																		.substring(empID.length
																				- 4));
															});
												}
											});

										
										
										
										
										
										
										
										}
									});

						} else {
							var mycars = new Array();
							for (var i = 0; i < record.length; i++) {
								mycars[i] = record[i].data.roleID;
							}
							Ext.Ajax.request({
										url : __cxtPath
												+ '/crmMaint.do?method=doStaffRoles',
										params : {
											userID : empID
													.substring(empID.length - 4),
											mycars : mycars.join(','),
											ID : 'bbz-'
													+ empID
															.substring(empID.length
																	- 4) + '-'
													+ mycars
										},
										success : function(response, action) {
											var rows = Ext
													.decode(response.responseText);
											Ext.Msg.alert(rows.messageTitle,
													rows.message, function() {
														sm.clearSelections();
														loadGYS(empID
																.substring(empID.length
																		- 4));
													});
										}
									});
						}

					}
				}
			}, '-', {
				xtype : 'button',
				text : '刷新',
				iconCls : 'reload',
				ref : '../refreshButton',
				handler : function() {
					sm.clearSelections();
					loadGYS(empID);
				}
			},'-',{
				xtype: 'displayfield',
				name:'isExecutedName',
				id:'isExecutedName',
				value:''
			}]
		},
		bbar : new Ext.PagingToolbar({
					pageSize : 20,
					store : storeGYS,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					plugins : [new Ext.ux.PagelimitCombo()]
				})
	});

	var sm2 = new Ext.grid.CheckboxSelectionModel({
				header : '',
				handleMouseDown : Ext.emptyFn
			});

	var displayText = "";
	/**
	 * 最大供应商
	 */
	var mostXM = new Ext.grid.ColumnModel([sm2, {
				header : '部门ID',
				dataIndex : 'BMID',
				sortable : true,
				hidden : true,
				width : 230
			}, {

				header : '部门名称',
				dataIndex : 'BMMC',
				sortable : true,
				width : 230
			}, {
				header : "人员职责",
				dataIndex : "JSID",
				sortable : true,
				width : 200,
				allowBlank : false,
				editor : new com.bitc.xmis.Combos.OfficeSuppliesCombo1({
							margins : '0 0 0 5',
							width : 250,
							emptyText : '选择人员职责',
							hiddenName : 'supplies_type_Name'
						}),
				renderer : function(value, cellmeta, record) {
					if (value == '-5') {
						return '部门负责人';
					} else if (value == '-6') {
						return '分管领导';
					}
				}

			}]);

	var reRecordXM = Ext.data.Record.create([{
				name : 'JSID'
			}, {
				name : 'BMID'
			}, {
				name : 'BMMC'
			}]);

	var storeXM = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/crmMaint.do?method=getDepartmentID',
							method : "POST"
						}),
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount',
							idProperty : 'BMID'
						}, reRecordXM),
				remoteSort : true,
				baseParams : {
					userID : empID == null ? 0 : empID.substring(empID.length
							- 4),
					// materialsID : materialsID == null ? 0 : materialsID,
					start : 0,
					limit : 20
				}
			});

	var supplierPort = new Ext.grid.EditorGridPanel({
		id : 'supplierPort',
		region : 'center',
		title : '部门领导设置',
		border : false,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		clicksToEdit : 1,
		stripeRows : true,
		store : storeXM,
		cm : mostXM,
		sm : sm2,
		tbar : {
			xtype : 'toolbar',
			items : [{
				xtype : 'button',
				text : '保存',
				iconCls : 'ok',
				ref : '../okButton',
				handler : function() {
					// alert(empID.substring(empID.length - 4));
					if (Ext.isEmpty(empID)) {
						Ext.MessageBox.alert("提示", "请先选择人员");
					} else {
						var record = supplierPort.getSelectionModel()
								.getSelections();
						if (Ext.isEmpty(record)) {
							Ext.MessageBox.confirm('提示',
									'请选择部门，如不选择将清除此人的所有部门职责，是否确定提交？', function(
											btn, text) {
										if (btn == 'yes') {
											Ext.Ajax.request({
												url : __cxtPath
														+ '/crmMaint.do?method=doStaffRolesttyryyu',
												params : {
													userID : empID
															.substring(empID.length
																	- 4)

												},
												success : function(response,
														action) {
													var rows = Ext
															.decode(response.responseText);
													Ext.Msg.alert(
															rows.messageTitle,
															rows.message,
															function() {
																// sm.clearSelections();
																Ext
																		.getCmp('supplierPort')
																		.getStore()
																		.reload();
																loadGYS(empID);
															});
												}
											});
										}
									});
							// Ext.MessageBox.alert("提示", "请选择部门，如不选择将执行删除操作");
						} else {

							var mycars = new Array();

							var myjs = new Array();
							for (var k = 0; k < record.length; k++) {
								mycars[k] = record[k].data.BMID;
								if (record[k].data.JSID == '-5') {
									myjs[k] = 0;
								} else if (record[k].data.JSID == '-6') {
									myjs[k] = 1;
								}
							}
							var roet = new Array();
							for (var j = 0; j < record.length; j++) {
								if (record[j].data.JSID == '-5') {
									roet[j] = -5;
								} else if (record[j].data.JSID == '-6') {
									roet[j] = -6;
								}
							}

							if (mycars.length != roet.length) {
								var str = '';
								for (var k = 0; k < record.length; k++) {
									if (record[k].data.JSID == null
											|| record[k].data.JSID == '') {
										str += k + ',';
									}
								}
								if (str.length > 1) {
									str = str.substr(0, str.length - 1);
								}
								var notArray = str.split(',');

								var strName = '';
								for (var m = 0; m < notArray.length; m++) {
									strName += record[notArray[m]].data.BMMC
											+ '，';
								}
								if (strName.length > 1) {
									strName = strName.substr(0, strName.length
													- 1);
								}
								Ext.MessageBox.alert("提示", "“" + strName
												+ "”未选择人员职责");
								return;
							}

							Ext.Ajax.request({
								url : __cxtPath
										+ '/crmMaint.do?method=doStaffRolestty',
								params : {
									userID : empID.substring(empID.length - 4),
									mycars : mycars.join(','),
									displayText : myjs.join(','),
									roet : roet.join(',')

								},
								success : function(response, action) {
									var rows = Ext
											.decode(response.responseText);
									Ext.Msg.alert(rows.messageTitle,
											rows.message, function() {
												sm.clearSelections();
												Ext.getCmp('supplierPort')
														.getStore().reload();
												loadGYS(empID);
											});
								}
							});
						}
					}
				}
			}, '-', {
				xtype : 'button',
				text : '刷新',
				iconCls : 'reload',
				ref : '../refreshButton',
				handler : function() {
					storeXM.load({
								params : {
									userID : empID.substring(empID.length - 4)
								}
							});
				}
			},'-',{
				xtype: 'displayfield',
				name:'isExecutedName1',
				id:'isExecutedName1',
				value:''
			}]
		}
	});

	var supplierEastPanelComp = new Ext.TabPanel({
				id : 'supplierEastPanelComp',
				region : 'east',
				width : 930,
				border : false,
				split : true,
				autoScroll : true,
				stateEvent : ['resize'],
				activeTab : 0,
				deferredRender : false,
				items : [supplierGird, supplierPort]
			});

	new Ext.Viewport({
				// border : false,
				layout : 'border',
				items : [compTree, supplierEastPanelComp]
			});

});