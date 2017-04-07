var explorerWindow = new Ext.app.Module({
			id : 'doc-win',
			init : function() {
				this.launcher = {
					text : '我的文档',
					iconCls : 'icon-grid',
					handler : this.createWindow,
					scope : this
				}
			},

			createWindow : function() {
				var desktop = this.app.getDesktop();
				var win = desktop.getWindow('doc-win');
				if (!win) {
					win = desktop.createWindow({
								id : 'doc-win',
								title : '我的文档',
								width : 740,
								height : 480,
								iconCls : 'tabs',
								shim : false,
								animCollapse : false,
								border : false,
								constrainHeader : true,
								layout : 'fit',
								items : new Ext.Panel({
											border : false,
											autoScroll : true,
											autoLoad : {
												url : __cxtPath
														+ '/explorer/explorer.jsp',
												scripts : true
											}
										})
							});
				}
				win.show();
			}
		});

var menus = __menus || [];
menus.sort(function(item1, item2) {
			return item1.sequenceNo - item2.sequenceNo;
		});

MyDesktop = new Ext.app.App({
			init : function() {
				Ext.QuickTips.init();
			},

			getModules : function() {
				var modules = new Array();
				Ext.each(menus, function(item, index) {
							modules.push(new MyDesktop.Module({
										text : item.text,
										cls : item.cls,
										id : 'mod_' + item.id,
										children : item.children
									}));
						}, this);
				modules.push(explorerWindow);
				modules.push(new MyDesktop.AccordionWindow());
				return modules;
			},

			// config for the start menu
			getStartConfig : function() {
				return {
					title : __emp.userName,
					iconCls : 'user',
					toolItems : [{
								text : '风格切换',
								iconCls : 'settings',
								scope : this,
								handler : function() {
									location.href = __cxtPath;
								}
							}, '-', {
								text : '注销',
								iconCls : 'logout',
								scope : this,
								handler : function() {
									location.href = __cxtPath + "/logout.do";
								}
							}]
				};
			}
		});

var xMis = new Object();
xMis.maxTabSize = 5;
xMis.tabCount = 1;
xMis.menus = new Object();
for (var i = 0; i < menus.length; i++) {
	var item = menus[i];
	// var m = new Object();
	// m.id = item.identifies;
	// m.title = item.text;
	// m.url = item.url;
	// m.iconCls = item.cls;
	// xMis.menus[m.id] = m;
	for (var j = 0; item.children && j < item.children.length; j++) {
		var c = item.children[j];
		var n = new Object();
		n.id = c.identifies;
		n.title = c.text;
		n.url = c.url;
		n.iconCls = c.cls;
		xMis.menus[n.id] = n;
	}
}
xMis.turnToMenu = function(identifies, url) {
	if (xMis.menus.hasOwnProperty(identifies)) {
		var m = xMis.menus[identifies];
		return xMis.turn(m.id, m.title, url || m.url, m.iconCls);
	}
};
xMis.turn = function(id, title, url, iconCls) {
	if (url == "") {
		return;
	}
	var tUrl;
	if (url.indexOf(__cxtPath + '/') >= 0) {
		tUrl = url;
	} else {
		tUrl = __cxtPath + "/" + url;
	}
	var desktop = MyDesktop.getDesktop();
	var win = desktop.getWindow('win-' + id);
	if (win) {
		win.show();
		var ifr = document.frames['iframe-' + id];
		if (ifr.location.pathname + ifr.location.search == tUrl
				|| ifr.location.href == tUrl) {
			return;
		}
		win.setIconClass('tab-loading');
		ifr.location.href = tUrl;
		return;
	} else {
		win = desktop.createWindow({
			id : 'win-' + id,
			title : title,
			width : 600,
			height : 480,
			layout : 'border',
			items : new Ext.Panel({
				border : false,
				region : 'center',
				autoScroll : true,
				html : '<iframe id="iframe-'
						+ id
						+ '" src="'
						+ tUrl
						+ '" scrolling="auto" frameborder="0" width="100%" height="100%" onload="javascript:changeicon(\'win-'
						+ id + '\',\'' + iconCls + '\');"></iframe>'
			}),
			iconCls : 'tab-loading',
			shim : false,
			animCollapse : false,
			constrainHeader : true,
			cmenu : new Ext.menu.Menu({
						items : [{
									text : '刷新',
									handler : function() {
										var ifr = document.frames['iframe-'
												+ id];
										if (ifr) {
											ifr.location.reload();
										}
									}
								}, {
									text : '关闭',
									handler : function() {
										win.close();
									}
								}]
					})
		});
	}
	win.show();
};
function changeicon(tabid, iconCls) {
	var panel = Ext.getCmp(tabid);
	panel.setIconClass(iconCls);
	//var win=desktop.getWindow('win-id');
	//win.setIconClass(iconCls);
}

/*
 * Example windows
 */
MyDesktop.GridWindow = Ext.extend(Ext.app.Module, {
	id : 'grid-win',
	init : function() {
		this.launcher = {
			text : 'Grid Window',
			iconCls : 'icon-grid',
			handler : this.createWindow,
			scope : this
		};
	},

	createWindow : function() {
		var desktop = this.app.getDesktop();
		var win = desktop.getWindow('grid-win');
		if (!win) {
			win = desktop.createWindow({
						id : 'grid-win',
						title : 'Grid Window',
						width : 740,
						height : 480,
						iconCls : 'icon-grid',
						shim : false,
						animCollapse : false,
						constrainHeader : true,

						layout : 'fit',
						items : new Ext.Panel({
									border : false,
									autoScroll : true,
									autoLoad : {
										url : __cxtPath
												+ '/contractMain.do?method=contract',
										scripts : true
									}
								})
					});
		}
		win.show();
	}
});

MyDesktop.TabWindow = Ext.extend(Ext.app.Module, {
	id : 'tab-win',
	init : function() {
		this.launcher = {
			text : 'Tab Window',
			iconCls : 'tabs',
			handler : this.createWindow,
			scope : this
		}
	},

	createWindow : function() {
		var desktop = this.app.getDesktop();
		var win = desktop.getWindow('tab-win');
		if (!win) {
			win = desktop.createWindow({
				id : 'tab-win',
				title : 'Tab Window',
				width : 740,
				height : 480,
				iconCls : 'tabs',
				shim : false,
				animCollapse : false,
				border : false,
				constrainHeader : true,

				layout : 'fit',
				items : new Ext.TabPanel({
					activeTab : 0,

					items : [{
								title : 'Tab Text 1',
								header : false,
								html : '<p>Something useful would be in here.</p>',
								border : false
							}, {
								title : 'Tab Text 2',
								header : false,
								html : '<p>Something useful would be in here.</p>',
								border : false
							}, {
								title : 'Tab Text 3',
								header : false,
								html : '<p>Something useful would be in here.</p>',
								border : false
							}, {
								title : 'Tab Text 4',
								header : false,
								html : '<p>Something useful would be in here.</p>',
								border : false
							}]
				})
			});
		}
		win.show();
	}
});

MyDesktop.AccordionWindow = Ext.extend(Ext.app.Module, {
	id : 'acc-win',
	init : function() {
		this.launcher = {
			text : '联系人',
			iconCls : 'accordion',
			handler : this.createWindow,
			scope : this
		}
	},

	createWindow : function() {
		var desktop = this.app.getDesktop();
		var win = desktop.getWindow('acc-win');
		if (!win) {
			win = desktop.createWindow({
				id : 'acc-win',
				title : 'Accordion Window',
				width : 250,
				height : 400,
				iconCls : 'accordion',
				shim : false,
				animCollapse : false,
				constrainHeader : true,

				tbar : [{
							tooltip : {
								title : 'Rich Tooltips',
								text : 'Let your users know what they can do!'
							},
							iconCls : 'connect'
						}, '-', {
							tooltip : 'Add a new user',
							iconCls : 'user-add'
						}, ' ', {
							tooltip : 'Remove the selected user',
							iconCls : 'user-delete'
						}],

				layout : 'accordion',
				border : false,
				layoutConfig : {
					animate : false
				},

				items : [new Ext.tree.TreePanel({
					id : 'im-tree',
					title : 'Online Users',
					loader : new Ext.tree.TreeLoader(),
					rootVisible : false,
					lines : false,
					autoScroll : true,
					tools : [{
								id : 'refresh',
								on : {
									click : function() {
										var tree = Ext.getCmp('im-tree');
										tree.body.mask('Loading',
												'x-mask-loading');
										tree.root.reload();
										tree.root.collapse(true, false);
										setTimeout(function() { // mimic a
													// server call
													tree.body.unmask();
													tree.root
															.expand(true, true);
												}, 1000);
									}
								}
							}],
					root : new Ext.tree.AsyncTreeNode({
								text : 'Online',
								children : [{
											text : 'Friends',
											expanded : true,
											children : [{
														text : 'Jack',
														iconCls : 'user',
														leaf : true
													}, {
														text : 'Brian',
														iconCls : 'user',
														leaf : true
													}, {
														text : 'Jon',
														iconCls : 'user',
														leaf : true
													}, {
														text : 'Tim',
														iconCls : 'user',
														leaf : true
													}, {
														text : 'Nige',
														iconCls : 'user',
														leaf : true
													}, {
														text : 'Fred',
														iconCls : 'user',
														leaf : true
													}, {
														text : 'Bob',
														iconCls : 'user',
														leaf : true
													}]
										}, {
											text : 'Family',
											expanded : true,
											children : [{
														text : 'Kelly',
														iconCls : 'user-girl',
														leaf : true
													}, {
														text : 'Sara',
														iconCls : 'user-girl',
														leaf : true
													}, {
														text : 'Zack',
														iconCls : 'user-kid',
														leaf : true
													}, {
														text : 'John',
														iconCls : 'user-kid',
														leaf : true
													}]
										}]
							})
				}), {
					title : 'Settings',
					html : '<p>Something useful would be in here.</p>',
					autoScroll : true
				}, {
					title : 'Even More Stuff',
					html : '<p>Something useful would be in here.</p>'
				}, {
					title : 'My Stuff',
					html : '<p>Something useful would be in here.</p>'
				}]
			});
		}
		win.show();
	}
});

MyDesktop.Module = Ext.extend(Ext.app.Module, {
			init : function() {
				this.launcher = {
					text : this.text,
					iconCls : this.cls,
					handler : function() {
						return false;
					},
					scope : this,
					windowId : this.id,
					menu : {
						items : []
					}
				};
				this.children.sort(function(item1, item2) {
							return item1.sequenceNo - item2.sequenceNo;
						});
				Ext.each(this.children, function(item, index) {
							this.launcher.menu.items.push({
										text : item.text,
										iconCls : item.cls,
										url : item.url,
										handler : this.createWindow,
										scope : this,
										windowId : (item.identifies || item.id)
									});
						}, this);

			},

			createWindow : function(src) {
				xMis.turnToMenu(src.windowId, src.url);
			}
		});

// Array data for the grid
Ext.grid.dummyData = [
		['3m Co', 71.72, 0.02, 0.03, '9/1 12:00am'],
		['Alcoa Inc', 29.01, 0.42, 1.47, '9/1 12:00am'],
		['American Express Company', 52.55, 0.01, 0.02, '9/1 12:00am'],
		['American International Group, Inc.', 64.13, 0.31, 0.49, '9/1 12:00am'],
		['AT&T Inc.', 31.61, -0.48, -1.54, '9/1 12:00am'],
		['Caterpillar Inc.', 67.27, 0.92, 1.39, '9/1 12:00am'],
		['Citigroup, Inc.', 49.37, 0.02, 0.04, '9/1 12:00am'],
		['Exxon Mobil Corp', 68.1, -0.43, -0.64, '9/1 12:00am'],
		['General Electric Company', 34.14, -0.08, -0.23, '9/1 12:00am'],
		['General Motors Corporation', 30.27, 1.09, 3.74, '9/1 12:00am'],
		['Hewlett-Packard Co.', 36.53, -0.03, -0.08, '9/1 12:00am'],
		['Honeywell Intl Inc', 38.77, 0.05, 0.13, '9/1 12:00am'],
		['Intel Corporation', 19.88, 0.31, 1.58, '9/1 12:00am'],
		['Johnson & Johnson', 64.72, 0.06, 0.09, '9/1 12:00am'],
		['Merck & Co., Inc.', 40.96, 0.41, 1.01, '9/1 12:00am'],
		['Microsoft Corporation', 25.84, 0.14, 0.54, '9/1 12:00am'],
		['The Coca-Cola Company', 45.07, 0.26, 0.58, '9/1 12:00am'],
		['The Procter & Gamble Company', 61.91, 0.01, 0.02, '9/1 12:00am'],
		['Wal-Mart Stores, Inc.', 45.45, 0.73, 1.63, '9/1 12:00am'],
		['Walt Disney Company (The) (Holding Company)', 29.89, 0.24, 0.81,
				'9/1 12:00am']];