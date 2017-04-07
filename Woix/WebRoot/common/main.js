Ext.onReady(function() {

	/**
	 * 首页主体画面
	 * */
	Ext.QuickTips.init();
	var tabContextMenu = new Ext.ux.TabContextMenu({
				items : [{
							itemId : 'refresh',
							text : '刷新',
							scope : this,
							handler : function(tabs, item) {
								var tabid = item.getId();
								var id = tabid.substring(3);
								var ifr = window.frames['iframe' + id];
								if (ifr) {
									ifr.location.reload();
								}
							}
						}, '-', {
							itemId : 'close',
							text : '关闭',
							scope : this,
							handler : function(tabs, item) {
								if (item.closable) {
									tabs.remove(item);
									Woix.tabCount--;
								}
							}
						}, '-', {
							itemId : 'closeOthers',
							text : '关闭其它',
							scope : this,
							handler : function(tabs, item) {
								tabs.items.each(function(tab) {
											if (tab.closable && tab != item) {
												tabs.remove(tab);
												Woix.tabCount--;
											}
										}, this);
							}
						}, {
							itemId : 'closeAll',
							text : '关闭所有',
							scope : this,
							handler : function(tabs, item) {
								tabs.items.each(function(tab) {
											if (tab.closable) {
												tabs.remove(tab, true);
											}
										}, this);
							}
						}]
			});

	var logoPanel = new Ext.Panel({
				region : 'north',
				layout : 'border',
				height : 77,
				border : false,
				collapseMode : 'mini',
				collapsible : true,
				hideCollapseTool : true,
				header : false,
				items : [{
							xtype : 'panel',
							bodyCssClass : 'main-head-logo-1',
							region : 'west',
							width : 662,
							border : false
						}, {
							xtype : 'panel',
							bodyCssClass : 'main-head-logo-2',
							region : 'center',
							border : false
						}, {
							xtype : 'panel',
							bodyCssClass : 'main-head-logo-3',
							region : 'east',
							width : 249,
							border : false

						}]
			});
	var pictureUri = '/woix5.aspx?src=';

	Ext.Ajax.request({
		url : contextPath + 'WoixService.asmx/GetConfigValue',
		method : 'post',
		params : {
			paramName : 'Woix.AlarmRefreshInterval'
		},
		success : function(response, opts) {
			// 首页画面
			var name = Ext.DomQuery.selectNode('string', response.responseXML).textContent;
			var url = contextPath + pictureUri + name;
			Ext.getCmp('tab0').body
					.update('<iframe src ="'
							+ contextPath
							+ pictureUri
							// + 'YC_HMSY.svg'
							+ name
							+ '" scrolling="auto" frameborder="0" width="100%" height="100%"></iframe>');
		},
		failure : function(response, opts) {
			Ext.Msg.alert("提示", "获取首页画面异常");
		}
	});

	var homePage = new Ext.Panel({
		id : 'tab0',
		title : '首页',
		tabTip : '双击最大化/还原窗口',
		iconCls : 'tab-home',
		closabl : false,
		margins : '35 5 5 0',
		bodyStyle : 'overflow-x:hidden;overflow:auto width ="100%" height="100%"',
		html : ''
	});

	var leftMenu = new Ext.Panel({
				id : 'menu-panel',
				region : 'west',
				layout : 'accordion',
				title : '菜单栏',
				width : 200,
				minSize : 150,
				maxSize : 500,
				split : true,
				collapsible : true,
				collapseFirst : false,
				items : []
			});

	var statusPanel = new Ext.Panel({
		region : 'south',
		coolapsible : true,
		hideCollapseToll : true,
		border : false,
		bodyCssClass : 'main-foot-status',
		height : 20,
		layout : 'border',
		items : [{
			xtype : 'displayfield',
			region : 'center',
			value : '<div style="height: 15px; float:left;font:normal 12px tahoma, arial, sans-serif, 宋体;margin:0 0 0 0;">'
					+ '<span>&nbsp;研发单位:&nbsp;&nbsp;北京中水科技&nbsp;&nbsp;</span></div>'
		}, {
			region : 'east',
			xtype : 'combo',
			id : 'svgUrlCheckID',
			name : 'svgUrlCheck',
			ref : '../svgUrl',
			width : 100,
			hiddenName : 'svgUrl',
			mode : 'local',
			value : '/woix5.aspx?src=',
			triggerAction : 'all',
			forceSelection : true,
			editable : false,
			displayField : 'display',
			valueField : 'url',
			store : new Ext.data.ArrayStore({
						fields : ['display', 'url'],
						data : [['自动', '/woix.aspx?src='],
								['IE9以下版本', '/woix4.aspx?src='],
								['IE9以上版本', '/woix5.aspx?src=']]
					}),
			listeners : {
				'select' : function(combo) {
					getLeftMenu(combo.getValue());
					pictureUri = combo.getValue;
				}
			}
		}]
	});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [logoPanel, leftMenu, {
					xtype : 'panel',
					region : 'center',
					layout : 'border',
					items : [{
						id : 'workspace',
						name : 'worksapce',
						xtype : 'extendedtabpanel',
						region : 'center',
						activeTab : 0,
						enableTabScroll : true,
						plugins : [tabContextMenu],
						items : [homePage],
						listeners : {
							'tabdblclick' : function() {
								var action = logoPanel.collapsed
										&& leftMenu.collapsed
										? 'expand'
										: 'collapse';
								logoPanel[action](false);
								leftMenu[action](false);
							},
							'beforeshowcontextmenu' : function(tabs, item, menu) {
								if (item == tabs.getItem('tab0')) {
									return false;
								}
							}
						}
					}, {
						// 实时报警信息
						xtype : 'panel',
						id : 'quickPanel',
						region : 'south',
						border : false,
						collapsiable : true,
						hideCollapseTool : true,
						collapseMode : 'mini',
						height : 55,
						split : true,
						bodyStyle : {
							background : '#ffffff'
						},
						items : getQuickAlarmGrid()
					}]
				}, statusPanel]
			});

	// 获取menu树
	function getMenuTree(uri) {
		Ext.Ajax.request({
					url : 'menutree.xml',
					success : function(response, opts) {
						/**
						 * response对象是XMLHttpRequest类型，因此有两个重要属性:
						 * 
						 * responseText，返回字符串，如果为JSON,可以使用Ext的decode方法将其转换为对象
						 * 
						 * responseXML,DOM
						 * Document类型对象，如果为XML，可以使用Ext.DomQuery类的方法进行遍历
						 * 
						 */

						// //递归遍历tree
						// var createTreeNode = function(node) {
						// var attr = {};
						// for (var i = 0; i < node.attributes.length; i++)
						// {
						// attr[node.attributes[i].name] =
						// node.attributes[i].value;
						// }
						// var treeNode = new Ext.tree.TreeNode(attr);
						// // 递归产生子节点
						// var nodes = Ext.DomQuery.select('/TREENODES',
						// node);
						// if (nodes.length == 0) {
						// nodes = Ext.DomQuery.select('/TREENODE', node);
						// }
						//
						// for (var i = 0; i < nodes.length; i++) {
						// treeNode.appendChild(createTreeNode(nodes[i]));
						// }
						// return treeNode;
						// }
						// 获取子节点
						var getChildren = function(node) {
							var childrenNode = Ext.DomQuery.select('/TREENODE',
									node);
							if (childrenNode.length == 0) {
								return [];
							}

							var children = [];
							for (var i = 0; i < childrenNode.length; i++) {
								var cnode = childrenNode[i];
								var treenode = {
									text : cnode.getAttribute('Text'),
									// leaf :
									// cnode.getAttribute('Level')=='leaf',
									url : Ext.isEmpty(cnode
											.getAttribute('NodeData')) ? cnode
											.getAttribute('Link') : contextPath
											+ uri
											+ cnode.getAttribute('NodeData'),
									id : cnode.getAttribute('ID'),
									isQuickAlarmHide : Ext.isEmpty(cnode
											.getAttribute('NodeData'))
											? true
											: false
								};
								var cchildren = getChildren(cnode);
								if (cchildren.length != 0) {
									treenode.children = cchildren;
									treenode.leaf = false;
									treenode.expanded = true;
								} else {
									treenode.leaf = true;
								}
								children.push(treenode);
							}
							return children;
						}

						var doc = response.responseXML;
						var rootNode = doc.documentElement || doc;
						var menus = Ext.DomQuery.select('/TREENODES', rootNode);

						for (var i = 0; i < menus.length; i++) {
							var menu = menus[i];
							leftMenu.add(new Ext.tree.TreePanel({
										title : menu.getAttribute('Text'),
										loader : new Ext.tree.TreeLoader(),
										root : new Ext.tree.AsyncTreeNode({
													expanded : true,
													children : getChildren(menu)
												}),
										rootVisible : false,
										listeners : {
											'click' : function(node) {
												// 有画面的要实时报警，其他把实时报警关闭
												if (node.attributes.isQuickAlarmHide) {
													Ext.getCmp('quickPanel')
															.collapse(true);
												} else {
													Ext.getCmp('quickPanel')
															.expand(true);
												}

												Ext.getCmp('quickPanel')
														.doLayout();
												Woix
														.turn(
																node.attributes.id,
																// node.id//需xml定义节点id
																node.attributes.text,
																node.attributes.url,
																'');
											}
										}
									}));
						}
						leftMenu.doLayout();
					}
				});
	}

	function getLeftMenu(uri) {
		if (leftMenu.items.length > 0) {
			leftMenu.removeAll();
			getMenuTree(uri);
		} else {
			getMenuTree(uri);
			leftMenu.doLayout();
		}
	}

	getLeftMenu('/woix5.aspx?src=');

});
