Ext.onReady(function() {
	Ext.QuickTips.init();

	var ddRelationMapping = '';// 拖拽关系(groupid:groupname)
	var changeGroupName = '';// (groupid:groupname,)
	var newGroup = '';// (groupid:groupname,)
	var relationMapping = '';// (groupid:father_groupid,)

	var compWin;
	var customGroupListWin;
	var compTreeId = 1;// compTree公司树id
	var compTreeName = '北京中水科水电科技开发有限公司';// compTree公司树name

	var empName = ''; // 组查询的人员姓名
	var comempName = '';// 公司查询的人员姓名

	var videoWin; // 视频帮助窗口

	// 群发邮件--公司组织机构树
	var compTree = new com.bitc.xmis.OrgTreePanel({
				ddGroup : 'email-dd-group',
				title : '公司组织机构树',
				region : 'north',
				height : 230,
				minSize : 150,
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
						*/}]
			});

	// 群发邮件--自定义组树
	var treeLoader = new Ext.tree.TreeLoader({
				dataUrl : __cxtPath
						+ '/orgMaint.do?method=getCustomGroupTree&groupID='
						+ empId
			});

	var customGroupTree = new Ext.tree.TreePanel({
				title : '自定义组',
				region : 'center',
				animate : true,
				autoScroll : true,
				containerScroll : true,
				loader : treeLoader,
				enableDD : true,
				ddGroup : 'email-dd-group',
				rootVisible : true,
				singleExpand : true,
				contextMenu : customMenu,
				dropConfig : {
					appendOnly : true,
					ddGroup : 'email-dd-group'
				},
				root : {
					nodeType : 'async',
					text : '自定义组',
					draggable : false,
					id : 'root' + '_' + empId
				},
				tbar : [{
					text : '查询',// 组查询
					iconCls : 'searchIcon',
					handler : function() {
						if (customGroupListWin) {
							getCustomGroupListByEmpNameInklingSearch
									.onTrigger1Click();
							customGroupListWin.show();
						}
						if (!customGroupListWin) {
							customGroupListStore.load();
							customGroupListWin = new Ext.Window({
										title : '自定义组信息',
										height : 400,
										width : 650,
										closeAction : 'hide',
										layout : 'border',
										items : [customGroupListGrid]
									}).show();
						}
					}
				}]
			});

	customGroupTree.getRootNode().expanded = true;

	// 群发邮件--自定义组树右键菜单
	var customMenu = new Ext.menu.Menu({
				id : 'customMenuID',
				items : [{
							text : '新建组',
							iconCls : 'add',
							id : 'addGroup',
							pressed : true,
							handler : function() {
								addGroupHandler();
							}
						}, {
							text : '删除',
							iconCls : 'delete',
							id : 'deleteGroup',
							pressed : true,
							handler : function() {
								deleteGroupHandler();
							}
						}, {
							text : '保存全部',
							iconCls : 'ok',
							id : 'saveAll',
							pressed : true,
							handler : function() {
								saveGroupHandler();
							}
						}]
			});

	customGroupTree.on("contextmenu", function(node, e) {
				var model = customGroupTree.getSelectionModel();
				e.preventDefault();
				node.select();
				selected = model.select(node);
				customMenu.findById('addGroup').enable();
				customMenu.findById('deleteGroup').enable();
				if ("Emp" == selected.id.substring(0, 3)
						|| "Dept" == selected.id.substring(0, 4)) {
					customMenu.findById('addGroup').disable();
				}
				if ("root" == selected.id.substring(0, 4)) {
					customMenu.findById('deleteGroup').disable();
				}
				customMenu.showAt(e.getXY());
			}, this);

	// 自定义组双击编辑
	var treeEditor = new Ext.tree.TreeEditor(customGroupTree, {
				id : 'treeEdit',
				allowBlank : false
			});

	// 设置根节点、人员、部门不可编辑
	treeEditor.on('beforestartedit', function(editor) {
				var tnode = editor.editNode;
				if (tnode == customGroupTree.getRootNode()
						|| 'root' == tnode.id.substring(0, 4)
						|| 'Emp' == tnode.id.substring(0, 3)
						|| 'Dept' == tnode.id.substring(0, 4)) {
					return false;
				} else {
					return tnode;
				}
			});

	// 将编辑（重命名）的newValue传给changeGroupName
	treeEditor.on('complete', function(treeEditor, newValue, oldValue) {
				var nodeid = treeEditor.editNode.id;
				if ("Group" == nodeid.substring(0, 5)) {
					createChangeGroupName(nodeid, newValue);
				}
			});

	// 遍历currentNode，查看currentNode中是否含有node节点
	function ite(currentNode, node, result) {
		var nodeID = node.id;
		// var currentNodePath = createNodePath(currentNode.id);
		// customGroupTree.expandPath(currentNodePath);

		currentNode.expand(false);
		nodeID = nodeID.split("__")[0].split("_")[nodeID.split("__")[0]
				.split("_").length
				- 1];

		var childNodes = currentNode.childNodes;
		for (var i = 0; i < childNodes.length; i++) {
			var childNode = childNodes[i];
			var childNodeid = childNode.id;
			// 处理节点，只取groupid
			childNodeid = childNodeid.split("__")[0].split("_")[childNodeid
					.split("__")[0].split("_").length
					- 1];

			if (nodeID == childNodeid) {
				result = true;
				break;
			}
			if (childNode.hasChildNodes() && !result) {
				result = ite(childNode, node, result);
			}
			if (result)
				break;
		}
		return result;
	}

	customGroupTree.on('beforenodedrop', function(e) {
				// 加判断是否有重复的,有重复的不可拖拽
				if ("root" == e.target.id.substring(0, 4)) {
					return false;
				} else if ("comp" == e.dropNode.id.substring(0, 4)) {
					return false;
				} else if (e.dropNode.id.split('__')[0].indexOf(e.target.id
						.split('__')[0]) >= 0) {
					return false;
				} else if (e.dropNode.id.indexOf(e.target.id) >= 0) {
					return false;
				} else if ('new' == e.target.id.substring(0, 3)) {
					Ext.MessageBox.alert("提示", "请先保存，再拖拽！");
					return false;
				} else if ('drop_Dept' == e.target.id.substring(0, 9)) {
					return false;
				} else {
					// var newID = createNewId(e.dropNode, e.target);
					// 查看目标节点内是否含有newid节点，含有就不可以拖拽
					// var result = ite(e.target, newID, false);

					// 查看源节点内是否含有目标节点，含有不可拖拽
					var res = ite(e.dropNode, e.target, false);
					// 查看目标节点中是否含有源节点，含有不可拖拽
					var result = ite(e.target, e.dropNode, false);
					if (result || res) {
						Ext.MessageBox.alert("提示", "数据有误，请检查数据！");
						return false;
					} else {
						// copy
						e.dropNode = copyDropNode(e.dropNode, e.target)
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

	// 当前登录人的自定义组信息列表
	var customGroupListRecord = Ext.data.Record.create([{
				name : 'groupid'
			}, {
				name : 'father_groupid'
			}, {
				name : 'type'
			}, {
				name : 'groupname'
			}, {
				name : 'nodetype'
			}, {
				name : 'nodeid'
			}, {
				name : 'compid'
			}, {
				name : 'nodePath'
			}]);
	var customGroupListStore = new Ext.data.Store({
				url : __cxtPath + '/orgMaint.do?method=getCustomGroupList',
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, customGroupListRecord),
				listeners : {
					'load' : function() {
						if (!Ext.isEmpty(empName)) {
							var sm = customGroupListGrid.getSelectionModel();
							sm.selectFirstRow();
						}
					}
				}
			});
	var customGroupListsm = new Ext.grid.CheckboxSelectionModel({
				header : '',
				checkOnly : true,
				singleSelect : true,
				listeners : {
					'rowselect' : function(selectionModel, rowindex, record) {
						expandAndSelect(record.get('groupid'), record
										.get('nodeid'));
					}
				}
			});
	var customGroupListColumn = new Ext.grid.ColumnModel([
			new Ext.grid.RowNumberer({
						width : 35
					}), {
				header : '组ID',
				hidden : true,
				dataIndex : 'groupid'
			}, {
				header : '组父ID',
				hidden : true,
				dataIndex : 'father_groupid'
			}, {
				header : '类型',
				hidden : true,
				dataIndex : 'type'
			}, customGroupListsm, {
				header : '组名称',
				dataIndex : 'groupname',
				width : 350
			}, {
				header : '节点类型',// Emp,Dept
				hidden : true,
				dataIndex : 'nodetype'
			}, {
				header : '节点ID',// empId,deptId
				hidden : true,
				dataIndex : 'nodeid'
			}, {
				header : '公司ID',// 节点所在公司ID
				hidden : true,
				dataIndex : 'compid'
			}, {
				header : 'nodepath',// 节点在公司组织机构树的nodepath
				hidden : true,
				dataIndex : 'nodePath'
			}]);

	var getCustomGroupListByEmpNameInklingSearch = new Ext.ux.InklingSearchField(
			{
				width : 120,
				store : customGroupListStore,
				scope : this,
				emptyText : '请输入人员姓名',
				tooltip : {
					title : '查询',
					text : '输入人员姓名全称查找所在组信息'
				},
				onTrigger1Click : function() {
					this.el.dom.value = '';
					this.triggers[0].hide();

					this.store.baseParams = this.store.baseParams || {};
					delete this.store.baseParams['empName'];
					Ext.ux.InklingSearchField.superclass.onTrigger1Click
							.call(this);
				},
				onTrigger2Click : function() {
					var input = this.el.dom.value;
					empName = input;
					if (input != null && '' != input.trim()
							&& this.emptyText != input) {
						this.store.setBaseParam('empName', input);
						this.store.load({
									params : {
										empName : input
									}
								});
						this.hasSearch = true;
						this.triggers[0].show();
					} else {
						Ext.MessageBox.alert("提示", "查询条件不能为空！");
					}
				}
			});

	var customGroupListGrid = new Ext.grid.GridPanel({
				region : 'center',
				store : customGroupListStore,
				autoScroll : true,
				cm : customGroupListColumn,
				sm : customGroupListsm,
				tbar : [{
					xtype : 'button',
					iconCls : 'ok',
					text : '选定',
					scope : this,
					tooltip : '在公司组织机构树及自定义组中定位',
					handler : function() {
						// var childNode = expandAndSelect();
						// expandnode(childNode);
						var record = customGroupListGrid.getSelectionModel()
								.getSelected();
						expandAndSelect(record.get('groupid'), record
										.get('nodeid'));
						customGroupListWin.hide();

						if (!Ext.isEmpty(empName)) {// 组查询人员不为空时，在公司组织机构树上定位人员
							var record = customGroupListGrid
									.getSelectionModel().getSelected();
							var compID = record.data.compid;
							var path = "/root-" + record.data.nodePath;
							var userID = path.substring(path.lastIndexOf("/")
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
						empName = "";
					}
				}, {
					xtype : 'button',
					iconCls : 'remove',
					text : '取消',
					handler : function() {
						customGroupListWin.hide();
						empName = "";
					}
				}, '按人员姓名全称查询：', getCustomGroupListByEmpNameInklingSearch]

			});

	var data = {
		data : []
	};

	var emailStore = new Ext.data.JsonStore({
				root : 'data',
				data : data,
				idProperty : 'userId',
				fields : ['userId', 'userName', 'position', 'email',
						'mobileNo', 'deptId', 'tdeptId', 'compId', 'compName']
			});

	var emailColumn = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						width : 30
					}), {
				header : 'userID',
				hidden : true,
				dataIndex : 'userId'
			}, {
				header : '姓名',
				dataIndex : 'userName',
				sortable : true,
				width : 180,
				listeners : {
					'dblclick' : function(t, grid, rowIndex, e) {
						var r = grid.getStore().getAt(rowIndex);

						Ext.Ajax.request({
							url : __cxtPath
									+ '/orgMaint.do?method=getEmpListByName',
							method : 'post',
							params : {
								name : r.data.userName,
								compID : r.data.compId
							},
							success : function(response, action) {
								var nodePath = response.responseText;
								parent.xMis
										.turn(
												'company-info-' + r.data.compId,
												r.data.compName,
												__cxtPath
														+ '/crmMaint.do?method=getCompanyInfo&companyID='
														+ r.data.compId
														+ '&tab=1&path='
														+ nodePath,
												'menu-company-info', true);
							},
							failure : function(response, action) {
								Ext.MessageBox.hide();
								Ext.MessageBox.alert("提示", "操作失败！");
							}
						});

					}
				}
			}, {
				header : '职务',
				dataIndex : 'position',
				sortable : true,
				width : 180
			}, {
				header : 'email',
				dataIndex : 'email',
				sortable : true,
				width : 180
			}, {
				header : '移动电话',
				dataIndex : 'mobileNo',
				sortable : true,
				width : 180
			}, {
				header : '父部门ID',
				dataIndex : 'deptId',
				sortable : true,
				hidden : true
			}, {
				header : '父部门名称',
				dataIndex : 'deptName',
				sortable : true,
				hidden : true
			}, {
				header : '部门ID',
				dataIndex : 'tdeptId',
				sortable : true,
				hidden : true
			}, {
				header : '部门名称',
				dataIndex : 'tdeptName',
				sortable : true,
				hidden : true
			}, {
				header : '公司ID',
				dataIndex : 'compId',
				sortable : true,
				hidden : true
			}, {
				header : '公司名称',
				dataIndex : 'compName',
				sortable : true,
				hidden : true
			}, {
				xtype : 'actioncolumn',
				width : 40,
				items : [{
							iconCls : 'remove2',
							tooltip : '删除',
							handler : function(grid, rowIndex, colIndex) {
								var record = emailStore.getAt(rowIndex);
								emailStore.remove(record);
								emailGrid.getView().refresh();
							}
						}]
			}]);

	var videoData = [
			['<a href=' + __cxtPath + '"/公司组织机构树功能介绍.wmv">公司组织机构树功能介绍</a>'],
			['<a href="' + __cxtPath
					+ '/自定义组新建、删除、重命名、拖拽.wmv">自定义组新建、删除、重命名、拖拽</a>'],
			['<a href="' + __cxtPath + '/自定义组查询、定位功能.wmv">自定义组查询、定位功能</a>']];

	var videoStore = new Ext.data.ArrayStore({
				fields : [{
							name : 'name'
						}]
			});
	videoStore.loadData(videoData);
	var videoGrid = new Ext.grid.GridPanel({
				store : videoStore,
				columns : [new Ext.grid.RowNumberer({
									width : 30
								}), {
							header : '视频名称',
							width : 250,
							dataIndex : 'name'

						}],
				region : 'center'
			});

	// 群发邮件--emailGrid
	var emailGrid = new Ext.grid.GridPanel({
		region : 'center',
		title : '从左侧拖动成员',
		ddGroup : 'email-dd-group',
		store : emailStore,
		autoScroll : true,
		cm : emailColumn,
		tbar : [
				{
					text : '群发邮件',
					iconCls : 'email',
					tooltip : '邮件群发调用本机邮件客户端，请先行安装',
					handler : function() {
						var records = emailStore.getRange();
						if (records.length != 0) {
							var adds = new Array();
							var num = records.length / 10;
							for (var i = 0; i < records.length; i++) {
								if (records[i].data.email) {
									adds.push(records[i].data.email.trim());
								}
							}
							document.location = "mailto:" + adds.join(';');
						} else {
							Ext.Msg.alert('提示', '请先选择要发送的人员，以拖拽形式');
						}

						// var records = emailStore.getRange();
						// if (records.length != 0) {
						// var adds = new Array();
						//
						// for (var i = 0; i < records.length; i++) {
						// if (records[i].data.email) {
						// adds.push(records[i].data.email.trim());
						// }
						// }
						//
						// var everyone = 10;//每一组成员个数
						// var num = adds.length % everyone == 0 ? Math
						// .floor(adds.length / everyone) : Math
						// .floor(adds.length / everyone)
						// + 1;
						// for (var j = 0; j < num; j++) {
						// var s = new Array();
						// for (var m = 0; m < everyone; m++) {
						// s.push(adds[m + j * everyone]);
						// }
						// document.location = "mailto:" + s.join(';');
						// }
						// }
					}
				},
				{
					text : '发送短信',
					iconCls : 'sms',
					ref : '../smsButton',
					tooltip : '发送短信只提供发送给本公司人员',
					hidden : true,
					handler : function(b, e) {
						bulkSMSHandler();
					}
				},
				{
					text : '发送公告',
					iconCls : 'announcement',
					ref : '../announcementButton',
					tooltip : '发送公告将消息发送给公司全体人员',
					hidden : true,
					handler : function() {
						announcementHandler();
					}
				},
				{
					text : '个人消息',
					iconCls : 'personal_message',
					ref : '../personal_messageButton',
					tooltip : '个人消息将消息发送给本公司选定的人员',
					hidden : true,
					handler : function() {
						personalMessageHandler();
					}
				},
				{
					text : '清空',
					iconCls : 'remove2',
					handler : function() {
						emailStore.removeAll();
					}
				},
				'->',
				'<font color ="red">*邮件群发调用本机邮件客户端，请先行安装*</font>'
				/*'<font color ="blue">*群发邮件有查询、自定义组、拖拽等功能，详情请见<a href="'
						+ __cxtPath + '/群发邮件帮助.doc">帮助</a>!*</font>', {
					text : '视频帮助',
					iconCls : 'video',
					handler : function() {
						if (videoWin) {
							videoWin.show();
						}
						if (!videoWin) {
							videoWin = new Ext.Window({
										title : '视频帮助列表',
										height : 200,
										width : 300,
										closeAction : 'hide',
										layout : 'border',
										items : [videoGrid]
									}).show();
						}

					}
				}*/],
		listeners : {
			'afterrender' : function() {

				new Ext.dd.DropTarget(emailGrid.getView().scroller, {
					ddGroup : 'email-dd-group',
					notifyEnter : function(dd, e, data) {
						if (this.overClass) {
							this.el.addClass(this.overClass);
						}
						// if ('Dept' == data.node.attributes.attributes.type
						// || 'Emp' == data.node.attributes.attributes.type) {
						// return this.dropNotAllowed;
						// } else
						if ('drop' == data.node.id.substring(0, 4)
								|| 'new' == data.node.id.substring(0, 3)) {
							return this.dropNotAllowed;
						} else if ('dept' == data.node.id.substring(0, 4)
								&& data.node.leaf) {// compTree上没有成员的部门
							return this.dropNotAllowed;
						} else {
							return emailStore
									.getById(data.node.attributes.attributes.id)
									? this.dropNotAllowed
									: this.dropAllowed
						}
					},
					notifyOver : function(dd, e, data) {
						// if ('Dept' == data.node.attributes.attributes.type
						// || 'Emp' == data.node.attributes.attributes.type) {
						// return this.dropNotAllowed;
						// } else
						if ('drop' == data.node.id.substring(0, 4)
								|| 'new' == data.node.id.substring(0, 3)) {
							return this.dropNotAllowed;
						} else if ('dept' == data.node.id.substring(0, 4)
								&& data.node.leaf) {// compTree上没有成员的部门
							return this.dropNotAllowed;
						} else {
							return emailStore
									.getById(data.node.attributes.attributes.id)
									? this.dropNotAllowed
									: this.dropAllowed
						}
					},
					notifyDrop : function(ddSource, e, data) {
						var node = data.node;
						if ('drop' == node.id.substring(0, 4)
								|| 'new' == node.id.substring(0, 3)) {
							Ext.MessageBox.alert("提示", "请先保存!");
							return false;
						} else if (!node.leaf) {
							expandnode(node);
							return true;
						} else if ('dept' == node.id.substring(0, 4)
								&& node.leaf) {// compTree上没有成员的部门
							return false;
						} else if (emailStore
								.getById(node.attributes.attributes.id)) {
							return false;
						} else if ('Dept' == node.attributes.attributes.type
								|| 'Emp' == node.attributes.attributes.type) {
							// return false;
							expandnode(node);
							return true;
						}
						var user = node.attributes.attributes;
						insertToGrid(user.id, user.name, user.position,
								user.email, user.mobileNo, user.deptId,
								user.deptName, user.tdeptId, user.tdeptName,
								compTreeId, compTreeName);
						return (true);
					}
				});
			}
		}
	});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [emailGrid, {
							layout : 'border',
							region : 'west',
							border : false,
							split : true,
							margins : '2 0 5 0',
							width : 240,
							maxSize : 500,
							minSize : 150,
							items : [compTree, customGroupTree]
						}]
			});

	/**
	 * 发送短信、个人消息 监控、自动化 发送公告 -1
	 * 
	 */
	if (empDivisionId == 1 || empDivisionId == 2) {
		// emailGrid.smsButton.setVisible(true);
		emailGrid.personal_messageButton.setVisible(true);
	}
	if (empId == -1) {
		emailGrid.announcementButton.setVisible(true);
	}

	// 增加
	function addGroupHandler() {
		// 必须是root，或者group才能新建组成员
		node = new Ext.tree.TreeNode({
					text : '新组',
					iconCls : 'users',
					id : 'new_Group_' + new Date().format("YmdHis")
							+ parseInt(Math.random() * 1000000) + '__'
							+ selected.id

				});
		selected.expand();
		selected.appendChild(node);
		node.select();
	}

	// 删除自定义组
	function deleteCustomGroup(node, type) {
		Ext.Ajax.request({
					url : __cxtPath + '/orgMaint.do?method=doDeleteCustomGroup',
					params : {
						node : node,
						type : type
					},
					success : function(response, opt) {

						var obj = Ext.decode(response.responseText);
						Ext.Msg.alert(obj.messageTitle, obj.message);

						treeLoader.load(customGroupTree.getRootNode(),
								function(n) {
									customGroupTree.getRootNode().expand();
								});
					},
					failure : function(response, opt) {
						var obj = Ext.decode(response.responseText);
						Ext.Msg.alert(obj.messageTitle, obj.message);
					}
				});
	}

	// 删除
	function deleteGroupHandler() {
		if ('new' == selected.id.substring(0, 3)
				|| "drop" == selected.id.substring(0, 4)) {
			Ext.MessageBox.confirm('提示', '请确认删除' + selected.text,
					function(btn) {
						if (btn == 'yes') {
							selected.remove();
						}
					});
		} else {
			var type;
			if ('Emp' == selected.id.substring(0, 3))
				type = 'Emp';
			if ('Dept' == selected.id.substring(0, 4))
				type = 'Dept';
			if ('Group' == selected.id.substring(0, 5))
				type = 'Group';
			Ext.MessageBox.confirm('提示', '请确认删除' + selected.text,
					function(btn) {
						if (btn == 'yes') {
							deleteCustomGroup(selected.id, type);
						}
					});
		}
	}

	// 生成newid
	// function createNewId(dropNode, target) {
	// var newId;
	// var dropNodeId = dropNode.attributes.id;
	// var targetNodeId = target.attributes.id;
	// if ('emp' == dropNodeId.split('-')[0]) {
	// newId = 'drop_Emp_' + dropNodeId.split('-')[1] + "__"
	// + targetNodeId;
	// if ('drop_Group_' == targetNodeId.substring(0, 11)) {
	// newId = 'drop_Emp_' + dropNodeId.split('-')[1] + "__"
	// + targetNodeId.substring(5);
	// }
	// } else if ('dept' == dropNodeId.split('-')[0].substring(0, 4)) {
	// newId = 'drop_Dept_' + dropNodeId.split('-')[1] + "__"
	// + targetNodeId;
	// if ('drop_Group_' == targetNodeId.substring(0, 11)) {
	// newId = 'drop_Dept_' + dropNodeId.split('-')[1] + "__"
	// + targetNodeId.substring(5);
	// }
	// } else if ('Emp_' == dropNodeId.substring(0, 4)
	// || 'Dept_' == dropNodeId.substring(0, 5)) {
	// newId = 'drop_' + dropNodeId.split('__')[0] + '__' + targetNodeId;
	// }
	//
	// else {
	// if ('drop_Dept' == dropNodeId.substring(0, 9)
	// || 'drop_Emp' == dropNodeId.substring(0, 8)) {
	// newId = dropNodeId.split('__')[0] + "__" + targetNodeId;
	// } else if ('drop_Group_' == targetNodeId.substring(0, 11)) {
	// newId = dropNodeId.split("__")[0] + "__"
	// + targetNodeId.substring(5);
	// } else if ('new_Group_' == targetNodeId.substring(0, 10)) {
	// newId = 'drop_Group_' + dropNodeId.split('_')[1] + '__'
	// + targetNodeId;
	// if ('new_Group' == dropNodeId.substring(0, 9)) {
	// newId = 'drop_new_Group_' + dropNodeId.split('_')[2] + '__'
	// + targetNodeId;
	// }
	// } else {
	// newId = "drop_Group_" + dropNodeId.split('_')[1] + "__"
	// + targetNodeId;
	// if ('drop_new_Group' == targetNodeId.substring(0, 14)) {
	// newId = 'drop_' + dropNodeId.split('__')[0].substring(5)
	// + '__' + targetNodeId;
	// }
	//
	// if ('new_Group' == dropNodeId.substring(0, 9)) {
	// newId = 'drop_new_Group_' + dropNodeId.split('_')[2] + '__'
	// + targetNodeId;
	// // newId =
	// // 'drop_'+dropNodeId.split('__')[0]+'__'+targetNodeId;
	// }
	// }
	// }
	// return newId;
	// }

	function createNewId(dropNode, target) {
		var newId;
		var dropNodeId = dropNode.attributes.id;
		var targetNodeId = target.attributes.id;
		if ('emp' == dropNodeId.split('-')[0]) {
			newId = 'drop_Emp_' + dropNodeId.split('-')[1] + "__"
					+ targetNodeId;
		} else if ('dept' == dropNodeId.split('-')[0].substring(0, 4)) {
			newId = 'drop_Dept_' + dropNodeId.split('-')[1] + "__"
					+ targetNodeId;
		} else {
			newId = 'drop_' + dropNodeId.split('__')[0] + '__' + targetNodeId;
		}
		return newId;
	}

	// copy源节点，包括源节点的子节点
	function copyDropNode(dropNode, target) {
		var newNode = new Ext.tree.TreeNode(Ext.apply({}, dropNode.attributes));
		newNode.setId(createNewId(dropNode, target));
		if (dropNode.id.indexOf("dept1") > 0
				|| dropNode.id.indexOf("dept2") > 0
				|| dropNode.id.indexOf("emp") > 0) {
			dropNode.expand(true);
		}
		for (var i = 0; i < dropNode.childNodes.length; i++) {
			n = dropNode.childNodes[i];
			if (n) {
				newNode.appendChild(copyDropNode(n, newNode));
			}
		}
		return newNode;
	}

	// 遍历树(将新建的自定义组关系放到relationMapping中,拖拽的人员、部门、组关系放到ddRelationMapping中)
	function iterator(currentNode) {
		var childNodes = currentNode.childNodes;
		for (var i = 0; i < childNodes.length; i++) {
			var childNode = childNodes[i];
			if ("new" == childNode.id.substr(0, 3)) {
				if ("Group" == childNode.id.substring(4, 9)) {
					if ("" == newGroup) {
						newGroup = childNode.id + ":" + childNode.text;
					} else {
						newGroup = newGroup + "," + childNode.id + ":"
								+ childNode.text;
					}
				}

				if ("" == relationMapping) {
					relationMapping = relationMapping + childNode.id + ":"
							+ childNode.parentNode.id;
				} else {
					relationMapping = relationMapping + "," + childNode.id
							+ ":" + childNode.parentNode.id;
				}
			}

			if ("drop" == childNode.id.substring(0, 4)) {

				if ("" == ddRelationMapping) {
					ddRelationMapping = childNode.id + ":" + childNode.text;
				} else {
					var num = (childNode.id.split('__')).length - 1;
					ddRelationMapping = ddRelationMapping + "," + childNode.id
							+ ":" + childNode.text;
				}
				// var num = (childNode.id.split('__')).length - 1;
				// var len = 0;
				// for (var v = 0; v < num; v++) {
				// len = len + childNode.id.split('__')[v].length;
				// }
				// len = len + 2 * (num - 1);
				// if ("" == ddRelationMapping) {
				//
				// ddRelationMapping = childNode.id.substring(0, len) + ":"
				// + childNode.text;
				// } else {
				// ddRelationMapping = ddRelationMapping + ","
				// + childNode.id.substring(0, len) + ":"
				// + childNode.text;
				// }

			}
			if (childNode.hasChildNodes()) {
				iterator(childNode);
			}
		}
	}

	// 拼接changeGroupName数据字符串
	function createChangeGroupName(nodeid, newValue) {
		var index = changeGroupName.indexOf(nodeid + ":");
		if (index >= 0) {
			var front = changeGroupName.substring(0, index + nodeid.length + 1);
			var latter = changeGroupName.substring(index + nodeid.length + 1);
			var latterIndex = latter.indexOf(",");
			if (latterIndex != -1) {
				latter = latter.substring(latterIndex);
				changeGroupName = front + newValue + latter;
			} else {
				changeGroupName = front + newValue;
			}
		} else {
			if ("" == changeGroupName.trim()) {
				changeGroupName = changeGroupName + nodeid + ":" + newValue;
			} else {
				changeGroupName = changeGroupName + "," + nodeid + ":"
						+ newValue;
			}
		}
	}

	// 保存
	function saveGroupHandler() {
		iterator(customGroupTree.getRootNode());
		if ("" != changeGroupName || "" != newGroup || "" != ddRelationMapping) {
			Ext.Ajax.request({
						url : __cxtPath
								+ '/orgMaint.do?method=doSaveAllOfCustomGroup',
						params : {
							changeGroupName : changeGroupName,
							newGroup : newGroup,
							relationMapping : relationMapping,
							ddRelationMapping : ddRelationMapping
						},
						success : function(response, opt) {

							ddRelationMapping = '';
							changeGroupName = '';
							newGroup = '';
							relationMapping = '';

							var obj = Ext.decode(response.responseText);
							Ext.Msg.alert(obj.messageTitle, obj.message);

							treeLoader.load(customGroupTree.getRootNode(),
									function(n) {
										customGroupTree.getRootNode().expand();
									});
						},
						failure : function(response, opt) {
							var obj = Ext.decode(response.responseText);
							Ext.Msg.alert(obj.messageTitle, obj.message);
						}
					});
		} else {
			Ext.Msg.alert("提示", "您没有任何操作，无需保存")
		}
	}

	function getChildren(node, callback, scope) {
		Ext.Ajax.request({
					url : __cxtPath + '/orgMaint.do?method=getCustomGroupTree',
					params : {
						node : node.id,
						groupID : node.id
					},
					scope : scope,
					success : function(response) {
						var arr = Ext.decode(response.responseText);
						if (callback) {
							callback.call(scope || scope, arr);
						}
					}
				});
	}

	function getEmpsByEmpsID(empsID) {
		Ext.Ajax.request({
					url : __cxtPath + '/orgMaint.do?method=getEmpsInfo',
					params : {
						emps : empsID.join(',')
					},
					success : function(response, action) {
						var rows = Ext.decode(response.responseText).rows;
						for (var i = 0; i < rows.length; i++) {
							var user = rows[i];
							insertToGrid(user.empId, user.userName,
									user.position, user.emailAddress,
									user.mobileNo, user.deptId, user.deptName,
									user.tdeptId, user.tdeptName, user.compId,
									user.compName);
						}
					}
				});
	}

	function getEmpsByDeptsID(deptsID) {
		Ext.Ajax.request({
					url : __cxtPath + '/orgMaint.do?method=getDeptsInfo',
					params : {
						depts : deptsID.join(',')
					},
					success : function(response, action) {
						var rows = Ext.decode(response.responseText).rows;
						for (var i = 0; i < rows.length; i++) {
							var user = rows[i];
							insertToGrid(user.empId, user.userName,
									user.position, user.emailAddress,
									user.mobileNo, user.deptId, user.deptName,
									user.tdeptId, user.tdeptName, user.compId,
									user.compName);
						}
					}
				});
	}

	function expandnode(node) {
		if ("Group" == node.id.substring(0, 5)) {

			var empsID = new Array();
			var deptsID = new Array();
			getChildren(node, function(nodes) {
						for (var i = 0; i < nodes.length; i++) {
							if ('Dept' == nodes[i].attributes.type) {
								deptsID.push(nodes[i].id.substring(5,
										nodes[i].id.indexOf("__")));

							} else if ('Emp' == nodes[i].attributes.type) {

								empsID.push(nodes[i].id.substring(4,
										nodes[i].id.indexOf("__")));

							} else if ('Group' == nodes[i].attributes.type) {
								expandnode(nodes[i]);
							}
						}
						if (empsID.length > 0) {
							getEmpsByEmpsID(empsID);
						}

						if (deptsID.length > 0) {
							getEmpsByDeptsID(deptsID);
						}
					});

		} else if ('Dept' == node.id.substring(0, 4)) {
			var dept = new Array();
			dept.push(node.id.substring(5, node.id.indexOf("__")));
			getEmpsByDeptsID(dept);
		} else if ('Emp' == node.id.substring(0, 3)) {
			var emp = new Array();
			emp.push(node.id.substring(4, node.id.indexOf("__")));
			getEmpsByEmpsID(emp);
		} else {
			compTree.getChildren(node, function(nodes) {
						for (var i = 0; i < nodes.length; i++) {
							if (!nodes[i].leaf) {
								expandnode(nodes[i]);
							} else if (nodes[i].leaf
									&& "dept" == nodes[i].id.substring(0, 4)) {
								// // compTree上没有成员的部门
							} else {
								var user = nodes[i].attributes;

								insertToGrid(user.id, user.name, user.position,
										user.email, user.mobileNo, user.deptId,
										user.deptName, user.tdeptId,
										user.tdeptName, compTreeId,
										compTreeName);
							}
						}
					});
		}
	}

	// 向emailStore插入数据
	function insertToGrid(id, name, position, email, mobileNo, deptId,
			deptName, tdeptId, tdeptName, compId, compName) {
		if (emailStore.getById(id)) {
			return;
		}
		var record = new emailStore.recordType({
					userId : id,
					userName : name,
					position : position,
					email : email,
					mobileNo : mobileNo,
					deptId : deptId,
					deptName : deptName,
					tdeptId : tdeptId,
					tdeptName : tdeptName,
					compId : compId,
					compName : compName
				}, id);
		emailStore.insert(emailStore.data.length, record);
	}

	function foundEmp(currentNode, empname, nodeid) {
		var childNodes = currentNode.childNodes;
		var node;
		for (var i = 0; i < childNodes.length; i++) {
			var childNode = childNodes[i];
			var childNodeid = childNode.id;
			if ('Emp' == childNodeid.substring(0, 3)
					|| 'Dept' == childNodeid.substring(0, 4)) {
				if (childNodeid.split('_')[1] == nodeid) {
					node = childNode;
					break;
				}
			} else {
				var nodePath = createNodePath(childNodeid);

				customGroupTree.expandPath(nodePath, '', function() {
							customGroupTree.getNodeById(childNodeid).select();
						});
				node = foundEmp(childNode, empName, nodeid);
			}
			if (!Ext.isEmpty(node)) {
				break;
			}
		}
		return node;
	}

	// 自定义组展开并定位（选定组、以及node的empID或者deptID）
	function expandAndSelect(groupid, nodeid) {

		var childNodes = customGroupTree.getRootNode().childNodes;

		var node;
		for (var i = 0; i < childNodes.length; i++) {// 跟节点的child

			var childNode = childNodes[i];
			var childNodeid = childNode.id;
			if (childNodeid.split('__')[0].split('_')[1] == groupid) {
				var nodePath = "/" + childNodeid.split('__')[1] + "/"
						+ childNodeid;

				customGroupTree.expandPath(nodePath, '', function() {
							customGroupTree.getNodeById(childNodeid).select();
						});
				node = childNode;
				if (!Ext.isEmpty(empName)) {

					childNode = foundEmp(childNode, empName, nodeid);

					if (!Ext.isEmpty(childNode)) {
						var childNodePath = createNodePath(childNode.id);
						customGroupTree.expandPath(childNodePath, '',
								function() {
									customGroupTree.getNodeById(childNode.id)
											.select();
								});
					}
					node = childNode;
				}
				break;

			}
		}
		return node;
	}

	// 生成自定义组节点nodepath
	function createNodePath(childNodeid) {
		var childNodePath = "";
		var arr = new Array;
		for (var i = childNodeid.split('__').length; i > 0; i--) {
			if (arr.length == 0) {
				arr.push(childNodeid.split('__')[i - 1]);
			} else {
				arr.push(childNodeid.split('__')[i - 1] + '__'
						+ arr[arr.length - 1]);
			}
		}
		for (var j = 0; j < arr.length; j++) {
			childNodePath = childNodePath + "/" + arr[j];
		}
		return childNodePath;
	}

	// 发送短信
	function bulkSMSHandler() {

		var records = emailStore.getRange();
		var notOwnCompany = new Array();// 不是本公司的人员,人员姓名
		var haveNoPhoneNums = new Array();// 是本公司的，但是没有电话号码的，人员姓名
		var havePhoneNums = new Array();// 符合发送条件的，人员姓名
		var phoneNums = new Array();// 符合发送条件的，电话号码
		var empids = new Array();// 符合发送条件的，人员id

		var buikSMSWin = new Ext.Window({
			title : '发送短信',
			layout : 'border',
			width : 480,
			height : 300,
			items : [{
				xtype : 'panel',
				layout : 'form',
				region : 'center',
				border : false,
				items : [{
					xtype : 'panel',
					border : false,
					height : 40,
					items : [{
						xtype : 'displayfield',
						value : '<font color ="blue">&nbsp&nbsp发送短信只提供发送给本公司员工。<br/>&nbsp&nbsp非本公司及没有电话号码的员工系统会自动过滤。</font>'
					}]
				}, {
					xtype : 'textarea',
					fieldLabel : '内容',
					name : 'buikSMSMessage',
					id : 'buikSMSMessage',
					style : ' margin: 20 0 0 -20px;!important',
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
				}, {
					xtype : 'panel',
					border : false,
					height : 50,
					buttonAlign : 'center',
					buttons : [{
						xtype : 'button',
						text : '发送',
						handler : function() {
							var text = Ext.getCmp('buikSMSMessage').getValue();
							if (text == '' || text == null) {
								Ext.Msg.alert('提示', '请填写要发送的信息');
							} else {
								Ext.Ajax.request({
									url : __cxtPath
											+ '/orgMaint.do?method=sendMessage',
									params : {
										empids : empids.join(','),
										text : text
									},
									success : function(response, opt) {
										var obj = Ext
												.decode(response.responseText);
										Ext.Msg.alert(obj.messageTitle,
												obj.message, function() {
													Ext
															.getCmp('buikSMSMessage')
															.setValue('');
													buikSMSWin.close();
												});
									},
									failure : function(response, opt) {
										var obj = Ext
												.decode(response.responseText);
										Ext.Msg.alert(obj.messageTitle,
												obj.message);
									}
								});
							}
						}
					}]
				}]
			}]
		});

		if (records.length != 0) {
			var adds = new Array();
			for (var i = 0; i < records.length; i++) {
				if (records[i].data.compId == 1) {
					if (records[i].data.mobileNo) {
						phoneNums.push(records[i].data.mobileNo.trim());
						havePhoneNums.push(records[i].data.userName.trim());
						empids.push(records[i].data.userId);
					} else {
						haveNoPhoneNums.push(records[i].data.userName);
					}
				} else {
					notOwnCompany.push(records[i].data.userName.trim());
				}
			}

			var notOwnCompanyMessage = '非本公司员工：&nbsp' + notOwnCompany.join('，');
			var haveNoPhoneNumsMessage = '<br/><br/>没有电话号码信息员工：&nbsp'
					+ haveNoPhoneNums.join('，');
			var havePhoneNumbsMessage = '<br/><br/>符合发送条件员工：&nbsp'
					+ havePhoneNums.join('，');

			Ext.Msg.alert('提示', notOwnCompanyMessage + haveNoPhoneNumsMessage
							+ havePhoneNumbsMessage, function() {
						if (phoneNums.length > 0)
							buikSMSWin.show();
					});

		} else {
			Ext.Msg.alert('提示', '请先选择要发送的人员，以拖拽形式');
		}

	}

	// 发送公告
	function announcementHandler() {
		var records = emailStore.getRange();
		if (records.length > 0) {
			emailStore.removeAll();
		}

		var announcementWin = new Ext.Window({
			title : '发送公告',
			layout : 'border',
			width : 480,
			height : 300,
			items : [{
				xtype : 'panel',
				layout : 'form',
				region : 'center',
				border : false,
				items : [{
					xtype : 'panel',
					border : false,
					height : 30,
					items : [{
						xtype : 'displayfield',
						value : '<font color ="blue">&nbsp&nbsp发送公告将消息发送给本公司全体员工。</font>'
					}]
				}, {
					xtype : 'textarea',
					fieldLabel : '内容',
					name : 'announcementMessage',
					id : 'announcementMessage',
					style : ' margin: 20 0 0 -20px;!important',
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
				}, {
					xtype : 'panel',
					border : false,
					height : 50,
					buttonAlign : 'center',
					buttons : [{
						xtype : 'button',
						text : '发送',
						handler : function() {
							var text = Ext.getCmp('announcementMessage')
									.getValue();
							if (text == '' || text == null) {
								Ext.Msg.alert('提示', '请填写要发送的信息');
								return;
							} else {
								Ext.Ajax.request({
									url : __cxtPath
											+ '/orgMaint.do?method=sendAnnouncementMessage',
									params : {
										topic_type : 'home',
										text : text
									},
									success : function(response, opt) {
										var obj = Ext
												.decode(response.responseText);
										Ext.Msg.alert(obj.messageTitle,
												obj.message, function() {
													Ext
															.getCmp('announcementMessage')
															.setValue('');
													announcementWin.close();
												});
									},
									failure : function(response, opt) {
										var obj = Ext
												.decode(response.responseText);
										Ext.Msg.alert(obj.messageTitle,
												obj.message);
									}
								});
							}
						}
					}]
				}]
			}]
		}).show();

	}

	// 个人消息
	function personalMessageHandler() {

		var records = emailStore.getRange();
		var notOwnCompany = new Array();// 不是本公司的人员,人员姓名
		var empnames = new Array();// 符合发送条件的，人员姓名
		var empids = new Array();// 符合发送条件的，人员id

		var personalMessageWin = new Ext.Window({
			title : '发送消息',
			layout : 'border',
			width : 480,
			height : 300,
			items : [{
				xtype : 'panel',
				layout : 'form',
				region : 'center',
				border : false,
				items : [{
					xtype : 'panel',
					border : false,
					height : 40,
					items : [{
						xtype : 'displayfield',
						value : '<font color ="blue">&nbsp&nbsp发送消息只提供发送给本公司员工。<br/>&nbsp&nbsp非本公司员工系统会自动过滤。</font>'
					}]
				}, {
					xtype : 'textarea',
					fieldLabel : '内容',
					name : 'personalMessage',
					id : 'personalMessage',
					style : ' margin: 20 0 0 -20px;!important',
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
				}, {
					xtype : 'panel',
					border : false,
					height : 50,
					buttonAlign : 'center',
					buttons : [{
						xtype : 'button',
						text : '发送',
						handler : function() {
							var text = Ext.getCmp('personalMessage').getValue();
							if (text == '' || text == null) {
								Ext.Msg.alert('提示', '请填写要发送的信息');
								return;
							} else {
								Ext.Ajax.request({
									url : __cxtPath
											+ '/orgMaint.do?method=sendPersonalMessage',
									params : {
										topic_type : 'home',
										empids : empids.join(','),
										text : text
									},
									success : function(response, opt) {
										var obj = Ext
												.decode(response.responseText);
										Ext.Msg
												.alert(
														obj.messageTitle,
														obj.message
																+ '<br/>对方在下次登录时可以看到此消息',
														function() {
															Ext
																	.getCmp('personalMessage')
																	.setValue('');
															personalMessageWin
																	.close();
														});
									},
									failure : function(response, opt) {
										var obj = Ext
												.decode(response.responseText);
										Ext.Msg.alert(obj.messageTitle,
												obj.message);
									}
								});
							}
						}
					}]
				}]
			}]
		});

		if (records.length != 0) {
			var adds = new Array();
			for (var i = 0; i < records.length; i++) {
				if (records[i].data.compId == 1) {
					empnames.push(records[i].data.userName.trim());
					empids.push(records[i].data.userId);
				} else {
					notOwnCompany.push(records[i].data.userName);
				}
			}

			var notOwnCompanyMessage = '非本公司员工：&nbsp' + notOwnCompany.join('，');
			var empnamesMessage = '<br/><br/>符合发送条件员工：&nbsp'
					+ empnames.join('，');

			Ext.Msg.alert('提示', notOwnCompanyMessage + empnamesMessage,
					function() {
						if (empids.length > 0)
							personalMessageWin.show();
					});

		} else {
			Ext.Msg.alert('提示', '请先选择要发送的人员，以拖拽形式');
		}

	}

	if (Ext.state.Manager.get('bulkMail') !== true) {
		Ext.Msg.show({
					title : '提示',
					msg : '请您查看视频帮助。<br/><br/>下次是否提示',
					buttons : Ext.Msg.YESNO,
					closable : false,
					fn : function(btn) {
						if (btn == 'no') {
							Ext.state.Manager.set('bulkMail', true);
							parent.xMis.saveUserNoticeHabit();
						}
					}
				});
	}

		// var spot = new Ext.ux.Tour({
		// tours : [{
		// el : compTree.getEl().id,
		// title : '公司组织机构树',
		// tip : '公司组织机构树',
		// bounds : [100, 100, 100, 100]
		// }, {
		// el : customGroupTree.getEl().id,
		// title : '自定义树',
		// tip : '自定义树',
		// bounds : [200, 200, 200, 200]
		// }]
		// });

		/**
		 * 发送短信、个人消息 监控 自动化 发送公告 -1
		 * 
		 */
});