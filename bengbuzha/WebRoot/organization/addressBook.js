Ext.onReady(function() {
	Ext.QuickTips.init();
	var exportname = '通讯录';

	var totalCountRecord = Ext.data.Record.create([{
				name : 'empId',
				mapping : 'empId'
			}, {
				name : 'userName',
				mapping : 'userName'
			}, {
				name : 'deptId',
				mapping : 'deptId'
			}, {
				name : 'tdeptId',// 二级部门Id
				mapping : 'tdeptId'
			}, {
				name : 'departmentName'
			}, {
				name : 'position'
			}, {
				name : 'mobileNo'
			}, {
				name : 'switchboardNo'
			}, {
				name : 'extNo'
			}, {
				name : 'faxNo'
			}, {
				name : 'emailAddress'
			}, {
				name : 'officeAddress'
			}]);

	var store = new Ext.data.Store({
				autoLoad : true,
				url : __cxtPath + '/orgMaint.do?method=getLeaderInfo',
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, totalCountRecord)
			});

	var inklingSearch = new Ext.ux.InklingSearchField({
				store : store,
				tooltip : {
					title : '模糊查询',
					text : '您可以输入关键词进行查询。</br>包括姓氏、名字、数字等'
				},
				inklings : [{
							name : 'userName',
							op : 'like',
							type : 'string'
						}, {
							name : 'mobileNo',
							op : 'like',
							type : 'string'
						}, {
							name : 'switchboardNo',
							op : 'like',
							type : 'string'
						}, {
							name : 'extNo',
							op : 'like',
							type : 'string'
						}, {
							name : 'faxNo',
							op : 'like',
							type : 'string'
						}],
				onTrigger1Click : function() {
					this.store.baseParams = this.store.baseParams || {};
					delete this.store.baseParams['inklings'];
					if (this.hasSearch) {
						this.el.dom.value = '';
						var o = {
							start : 0
						};
						this.store.baseParams = this.store.baseParams || {};
						this.store.baseParams[this.paramName] = '';
						this.triggers[0].hide();
						this.hasSearch = false;
					}
				},
				onTrigger2Click : function() {
					if (inklingSearch.getRawValue() != null
							&& inklingSearch.getRawValue().trim() != '') {
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
						this.fireEvent('beforesearch', this, this.store);
						var v = Ext.util.Format.trim(inklingSearch
								.getRawValue());
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
										grid.getColumnModel().setHidden(2,
												false);
									}
								});
						this.hasSearch = true;
						this.triggers[0].show();

					} else {
						Ext.Msg.alert("提示", "查询条件不能为空！");
					}
				},
				listeners : {
					'beforesearch' : function(searchField, astore) {
						astore.proxy.setUrl(__cxtPath
										+ '/orgMaint.do?method=getEmps', true);
					}
				}
			});

	Ext.ns('Ext.ux.grid');
	Ext.ux.grid.RowNumberer = Ext.extend(Object, {
				id : 'cnumberer',
				width : 40,
				sortable : false,
				hideable : false,
				dataIndex : '',
				menuDisabled : true,
				renderer : function(value, cellmeta, record, rowIndex,
						columnIndex, store) {
					return rowIndex + 1;
				}
			});

	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.RowNumber'
						}]
			});

	var grid = new Ext.grid.GridPanel({
		store : store,
		cm : new Ext.grid.ColumnModel([new Ext.ux.grid.RowNumberer(), {
			header : '姓名',
			width : 85,
			sortable : true,
			dataIndex : 'userName',
			listeners : {
				'dblclick' : function(t, grid, rowIndex, e) {
					var r = grid.getStore().getAt(rowIndex);
					var contactPersonID = r.data.empId;
					if (contactPersonID == null || contactPersonID == '') {
						return;
					}
					Ext.Ajax.request({
						url : __cxtPath
								+ '/orgMaint.do?method=getEmpPathByEmpId',
						method : 'post',
						params : {
							id : contactPersonID
						},
						success : function(response, action) {
							var result = Ext.decode(response.responseText);
							var path = result.attributes.nodepath;
							parent.xMis
									.turn(
											'company-info-1',
											"蚌埠闸工程管理处",
											__cxtPath
													+ '/crmMaint.do?method=getCompanyInfo&companyID=1'
													+ '&tab=1&path=' + path,
											'menu-company-info', true);
						},
						failure : function(response, action) {
							Ext.MessageBox.alert("提示", "操作失败,请联系管理员！");
						}
					});
				}
			}
		}, {
			header : '部门',
			width : 130,
			sortable : true,
			dataIndex : 'departmentName'
		}, {
			header : '职位',
			width : 160,
			sortable : true,
			dataIndex : 'position'
		}, {
			header : '移动电话',
			width : 100,
			sortable : true,
			dataIndex : 'mobileNo'
		}, {
			header : '单位总机电话',
			width : 100,
			sortable : true,
			dataIndex : 'switchboardNo'
		}, {
			header : '分机电话',
			width : 85,
			sortable : true,
			dataIndex : 'extNo'
		}, {
			header : '传真号码',
			width : 85,
			sortable : true,
			dataIndex : 'faxNo'
		}, {
			header : '电子邮件',
			width : 200,
			sortable : true,
			dataIndex : 'emailAddress',
			renderer : function(value, cellmeta, record, rowIndex, columnIndex,
					store) {
				if (value != null && value != '') {
					return '<a href="mailto:' + value + '">' + value + '</a>';
				}
			}
		}, {
			header : '办公地址',
			width : 220,
			sortable : true,
			dataIndex : 'officeAddress'
		}]),
		stripeRows : true,
		height : 150,
		width : 850,
		region : 'center',
		tbar : ['查询：', inklingSearch, '-', {
					xtype : 'button',
					text : '导出',
					hidden : false,
					iconCls : 'xls',
					handler : function() {
						exp.onExport(grid, exportname, false);
					}
				}, '-', {
					xtype : 'button',
					text : '消息平台',
					iconCls : 'personal_message',
					hidden : false,
					handler : function() {
						// var emailWin = new Ext.Window({
						// title : '群发邮件',
						// height : 440,
						// width : 750,
						// closeAction : 'hide',
						// layout : 'border',
						// items : [emailGrid, new Ext.FormPanel({
						// region : 'west',
						// width : 240,
						// items : [compTree,
						// customGroupTree]
						// })]
						// });
						// return function() {
						// emailWin.show();
						// };
						parent.xMis.turn('bulkmail', '消息平台', __cxtPath
										+ '/organization/bulkMail.jsp',
								'personal_message', true);
					}
				}, '-',
				// {
				// xtype : 'button',
				// text : '发送短信',
				// iconCls : 'email',
				// hidden : false,
				// handler : function() {
				// com.bitc.xmis.util.showSendMessageWin(empId, false);
				// }
				// },
				'<font color ="blue">*双击姓名可查看详细信息*</font>']
	});

	var treepanel = new com.bitc.xmis.OrgTreePanel({
		width : 220,
		region : 'west',
		rootFilter : 1,
		leafLevel : com.bitc.xmis.OrgTree.LEVEL.DEPT2,
		autoExpandLevel : com.bitc.xmis.OrgTree.LEVEL.COMP,
		listeners : {
			'click' : function(node) {
				var nodeId = node.attributes.attributes.id;// 部门编号
				var level = node.attributes.attributes.level;// 节点级别
				if (com.bitc.xmis.OrgTree.LEVEL.COMP == level) {
					store.proxy
							.setUrl(
									__cxtPath
											+ '/orgMaint.do?method=getLeaderInfo',
									true);
					delete store.baseParams['deptId'];
					store.load();
					grid.getColumnModel().setHidden(2, false);
				} else if (com.bitc.xmis.OrgTree.LEVEL.DEPT1 == level
						|| com.bitc.xmis.OrgTree.LEVEL.DEPT2 == level) {
					if (!node.leaf) {
						node.expand();
						store.removeAll();
						return;
					}
					store.setBaseParam('deptId', nodeId);
					store.proxy.setUrl(__cxtPath
									+ '/orgMaint.do?method=getEmpInfoByID',
							true);
					store.load({
								params : {
									deptNo : nodeId
								}
							});
					grid.getColumnModel().setHidden(2, true);
				}
				inklingSearch.setValue(null); // 清空查询内容
				inklingSearch.store.baseParams = inklingSearch.store.baseParams
						|| {};
				inklingSearch.store.baseParams[inklingSearch.paramName] = '';
				inklingSearch.triggers[0].hide();
				inklingSearch.hasSearch = false;
			}
		}
	});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [grid, treepanel]
			});

});