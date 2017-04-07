﻿﻿Ext.ns('xMis');
xMis.maxTabSize = 10;// 工作区Tab最大数量
xMis.tabCount = 1;// 初始的Tab数量
xMis.menus = new Object();
if (__menus) {
	for (var i = 0; i < __menus.length; i++) {
		var item = __menus[i];
		var m = new Object();
		if (item.id < 0) {
			continue;
		}
		m.id = item.identifies;
		m.title = item.text;
		m.url = item.url;
		m.iconCls = item.cls;
		xMis.menus[m.id] = m;
		for (var j = 0; item.children && j < item.children.length; j++) {
			var c = item.children[j];
			var n = new Object();
			if (c.id < 0) {
				continue;
			}
			n.id = c.identifies;
			n.title = c.text;
			n.url = c.url;
			n.iconCls = c.cls;
			xMis.menus[n.id] = n;
		}
	}
}
xMis.turnToMenu = function(identifies, url, title) {
	if (xMis.menus.hasOwnProperty(identifies)) {
		var m = xMis.menus[identifies];
		return xMis.turn(m.id, title || m.title, url || m.url, m.iconCls);
	} else {
		return xMis.turn(url, title, url, null);
	}
};
/**
 * 在工作区打开一个标签，以ID为标识，如果已经存在标签则激活标签，如果不存在则新建
 * 
 * @param {String}
 *            id ID标识
 * @param {String}
 *            title 标签的标题
 * @param {String}
 *            url 标签中页面的url
 * @param {String}
 *            iconCls 标签标题的图标class
 * @param {String}
 *            refresh
 *            是否强制刷新。如果标签已经存在，会判断当前URL与传入的URL是否一致，refresh不为true时，如果一致不会刷新标签，反之则刷新
 */
xMis.turn = function(id, title, url, iconCls, refresh) {
	if (typeof id)
		if (url == "") {
			return;
		}
	var tUrl;
	if (url.indexOf(__cxtPath + '/') >= 0) {
		tUrl = url;
	} else {
		tUrl = __cxtPath + "/" + url;
	}
	var tip = title;
	title = Ext.util.Format.ellipsis(Ext.util.Format.stripTags(title), 15);
	var workspace = Ext.getCmp('workspace');
	var tab = workspace.findById('tab-' + id);
	if (tab) {
		workspace.activate(tab);
		var ifr = window.frames['iframe' + id].location?window.frames['iframe' + id]:window.frames['iframe' + id].contentWindow;
		if (refresh === true) {
			if (ifr.location.pathname + ifr.location.search == tUrl
					|| ifr.location.href == tUrl) {
				tab.setTitle(title, 'tab-loading');
				ifr.location.reload();
				return;
			} else {
				tab.setTitle(title, 'tab-loading');
				ifr.location.href = tUrl;
				return;
			}
		}
		if (ifr.location.href.substring(ifr.location.href.indexOf(__cxtPath)) == tUrl
				.substring(tUrl.indexOf(__cxtPath))) {
			return;
		}
		tab.setTitle(title, 'tab-loading');
		ifr.location.href = tUrl;
		return;
	}
	// if (xMis.tabCount >= xMis.maxTabSize) {
	// Ext.Msg.alert("", "最多只能有" + xMis.maxTabSize + "个页签，请关闭部分页签后重新操作。");
	// return;
	// }
	workspace.add(new Ext.Panel({
		id : 'tab-' + id,
		title : title,
		tabTip : tip,
		iconCls : 'tab-loading',
		html : '<iframe id="iframe-'
				+ id
				+ '" src="'
				+ tUrl
				+ '" scrolling="auto" frameborder="0" width="100%" height="100%" onload="javascript:xMis.changeicon(\'tab-'
				+ id + '\',\'' + iconCls + '\');"></iframe>',
		closable : true,
		listeners : {
			'close' : function(p) {
				setTimeout(function() {
							if (Ext.isIE) {
								CollectGarbage();
							}
						}, 2000)
				xMis.tabCount--;
			}
		}
	})).show();
	xMis.tabCount++;
};
xMis.closeTab = function(iwindow) {
	var aFrms = document.getElementsByTagName("IFRAME");
	for (i = 0; i < aFrms.length; i++) {
		if (aFrms[i].contentWindow == iwindow) {
			var fid = aFrms[i].id;
			var workspace = Ext.getCmp('workspace');
			var tab = workspace.findById('tab-' + fid.substring(7));
			if (tab) {
				workspace.remove(tab, true);
			}
			break;
		}
	}

};
xMis.changeicon = function(tabid, iconCls) {
	var panel = Ext.getCmp(tabid);
	if (panel) {
		panel.setIconClass(iconCls);
	}
};
xMis.logout = function() {
	com.bitc.xmis.util.RemoteProvider.single.saveState(function() {
				location.href = __cxtPath + '/logout.do';
			});
};
xMis.saveUserHabit = function() {
	Ext.Msg.confirm("请确认", "确定要保存用户习惯吗？", function(btn) {
				if (btn == 'yes') {
					com.bitc.xmis.util.RemoteProvider.single.saveState(
							function() {
								Ext.Ajax.request({
											url : __cxtPath
													+ '/loginAction.do?method=saveUH',
											success : function(response) {
												var action = Ext
														.decode(response.responseText);
												Ext.Msg.alert(
														action.messageTitle,
														action.message);
											},
											failure : function() {
												var action = Ext
														.decode(response.responseText);
												Ext.Msg.alert(
														action.messageTitle,
														action.message);
											}
										});
							});
				}
			});
};
/**
 * 窗口提示的用户习惯保存
 */
xMis.saveUserNoticeHabit = function() {
	com.bitc.xmis.util.RemoteProvider.single.saveState(function() {
				Ext.Ajax.request({
							url : __cxtPath + '/loginAction.do?method=saveUH',
							success : function(response) {
							},
							failure : function() {
								Ext.Msg.alert("提示", "保存失败,请重新操作");
							}
						});
			});
};
xMis.showSaveHabitLink = function(b) {
	Ext.get('saveHabitLink').setVisible(b);
};
var settingWin;
function showSetting() {
	var cookie = readCookie("style");
	var title = cookie ? cookie : getPreferredStyleSheet();
	if (!settingWin) {
		settingWin = new Ext.Window({
					title : '个人设置',
					modal : true,
					width : 400,
					height : 200,
					layout : 'border',
					closeAction : 'hide',
					items : [new Ext.form.FormPanel({
								region : 'center',
								baseCls : 'plain',
								border : false,
								labelAlign : 'right',
								padding : '5px',
								items : [{
									xtype : 'fieldset',
									title : '风格设置',
									items : [{
										xtype : 'radiogroup',
										name : 'mode',
										fieldLabel : '模式',
										items : [{
													boxLabel : '普通模式',
													inputValue : 0,
													name : 'mode',
													checked : true
												}, {
													boxLabel : '桌面模式',
													name : 'mode',
													inputValue : 1
												}],
										listeners : {
											change : function(rp, radio) {
												if (rp.getValue() == 1) {
													location.href = __cxtPath
															+ '/desktop/desktop.jsp';
												}
											}
										}
									}, {
										xtype : 'radiogroup',
										name : 'style',
										fieldLabel : '样式',
										items : [{
													boxLabel : '蓝色主题',
													inputValue : 'blue',
													name : 'style',
													checked : title == 'blue'
												}, {
													boxLabel : '灰色主题',
													name : 'style',
													inputValue : 'gray',
													checked : title == 'gray'
												}],
										listeners : {
											change : function(rp, radio) {
												setActiveStyleSheet(rp
														.getValue());
											}
										}
									}]
								}]
							})]
				});
	}
	settingWin.show();
}

//点击自己的用户名，跳转到个人信息
function showSelfInfo(){
	var empID = __emp.userID;
	if (empID == null
			|| empID == '') {
		return;
	}
	Ext.Ajax.request({
			url : __cxtPath
					+ '/orgMaint.do?method=getEmpPathByEmpId',
			method : 'post',
			params : {
				id : empID
			},
			success : function(response, action) {
				var result = Ext.decode(response.responseText);
				var path = result.attributes.nodepath;
				parent.xMis
						.turn(
								'company-info-'+__emp.companyID,
								__emp.companyName,
								__cxtPath
										+ '/crmMaint.do?method=getCompanyInfo&companyID='+__emp.companyID
										+ '&tab=1&path=' + path,
								'menu-company-info', true);
			},
			failure : function(response, action) {
				Ext.MessageBox.alert("提示", "操作失败,请联系管理员！");
			}
		});
}

// 首页标签的刷新功能
xMis.refreshes = {};
/**
 * 
 * @param {String}
 *            key 刷新项的标识
 * @param {Function}
 *            fun 刷新项的回调函数
 * @param {Object}
 *            scope 域
 */
xMis.addRefresh = function(key, fun, scope) {
	xMis.refreshes[key] = {
		callback : fun,
		scope : scope
	};
};
/**
 * 
 * @param {String}
 *            key 刷新项的标识，如果未指定，则刷新所有项。多项使用逗号隔开
 */
xMis.refresh = function(key) {
	var a = arguments;
	if (a.length > 1) {
		for (var i = 0, len = a.length; i < len; i++) {
			xMis.refresh(a[i]);
		}
	} else if (key) {
		var item = xMis.refreshes[key];
		if (item) {
			item.callback.call(item.scope);
		}
		return;
	} else {
		for (var k in xMis.refreshes) {
			xMis.refresh(k);
		}
	}
};
Ext.onReady(function() {
	Ext.QuickTips.init();
	var displayPram = '';// 监控事业部 水利自动化
	if (__emp.divisionID == 1 || __emp.divisionID == 2) {
		displayPram = '';
	} else {
		displayPram = 'none';
	}
	var FTPdisplayPram = '';// 监控事业部 水利自动化 市场部
	if (__emp.divisionID == 1 || __emp.divisionID == 2|| __emp.divisionID == 9) {
		FTPdisplayPram = '';
	} else {
		FTPdisplayPram = 'none';
	}
	var loginfo = '<div>'
			+ '<div style="position: absolute; bottom: 1px; right:360px;display:'
			+ displayPram
			+ ';">'
			+ '<a href = "extPortal/forward_bug.jsp" target="_blank"><img src="resources/images/dbug.png" width="50" height="50"  alt="缺陷管理" title="缺陷管理"/></a>'
			+ '</div>' 
			+ '<div style="position: absolute; bottom: 1px; right:310px;display:'
			+ FTPdisplayPram
			+ ';">'
			+ '<a href = "ftp://192.169.11.39" target="_blank"><img src="resources/images/dftp.png" width="50" height="50"  alt="FTP管理" title="FTP"/></a>'
			+ '</div>' 
			+ '<div style="position: absolute; bottom: 1px; right:260px;display:;">'
			+ '<a href = "http://inner.iwhr.com/innerlogin/" target="_blank"><img src="resources/images/diwhr.png" width="50" height="50"  alt="水科院内网" title="水科院内网"/></a></div>'
			+ '<div style="position: absolute; bottom: 1px; right:10px;">'
			+ '<div style="margin: 0 0 0 auto;color: white;text-align:center;">'
			+ '<div style="padding-top:5px;padding-bottom: 5px;border-bottom:2px solid #2e8fba;font-weight: bold;font-size:16px;">'
			+ __emp.divisionName
			+ '&nbsp;&nbsp;<span style="font-size:14px;">'
			+ '</span></div><div style="padding-top:5px;padding-bottom: 5px;font-size:14px;">'
//			+ '<a class="loginfo" href="javascript:;" onclick="showSetting();">'
			+ '<a class="loginfo" href="javascript:;" title="进入个人信息" onclick="showSelfInfo();">'
			+ __emp.userName
			+ '</a>&nbsp;&nbsp;<a class="loginfo" href="javascript:;" onclick="xMis.saveUserHabit();" id="saveHabitLink" >保存用户习惯</a>&nbsp;&nbsp;<a class="loginfo" href="javascript:;" onclick="xMis.logout();">注销</a></div></div></div>';

	var menus = __menus || [];
	menus.sort(function(item1, item2) {
				return item1.sequenceNo - item2.sequenceNo;
			});
	var items = new Array();
	Ext.each(menus, function(item, index) {
		if (item.id < 0) {
			return;
		}
		var panel = new Ext.Panel({
					id : item.identifies,
					iconCls : item.cls || 'left-menu-document',
					title : item.text,
					border : false,
					listeners : {
						'expand' : function(treePanel) {
							// xMis.turn(this.id, item.text, item.url, item.cls
							// || 'left-menu-document');
						}
					}
				});
		if (item.children && item.children.length > 0) {
			panel.add(new Ext.tree.TreePanel({
						border : false,
						rootVisible : false,
						request : 'post',
						loader : new Ext.tree.TreeLoader({
									url : __cxtPath
											+ '/sys.do?method=getLeftMenu&moduleId='
											+ item.id
								}),
						listeners : {
							'click' : function(node) {
								xMis.turn(
										node.attributes.identifies || node.id,
										node.text, node.attributes.url,
										node.attributes.iconCls);
							}
						},
						root : new Ext.tree.AsyncTreeNode({
									id : Ext.id(),
									expanded : true
								})
					}));
		}
		items.push(panel);
	}, this);
	var leftMenu = new Ext.Panel({
				title : '菜单',
				region : 'west',
				layout : 'accordion',
				// collapseMode : 'mini',
				collapseFirst : false,
				// activeItem : 1,
				split : true,
				collapsible : true,
				minSize : 175,
				maxSize : 500,
				width : 200,
				items : items
			});
	var statusPanel = new Ext.Panel({
		region : 'south',
		collapsible : true,
		hideCollapseTool : true,
		collapseMode : 'mini',
		bodyCssClass : 'main-foot-status',
		html : '<div style=" height:15px; ">'
				+ '<div style="float:left;font:normal 12px tahoma, arial, sans-serif, 宋体;margin:0 0 0 0;">'
				+ '<span style="color:white">&nbsp;研发单位:&nbsp;&nbsp;北京中水科技&nbsp;&nbsp;监控事业部&nbsp;<a href="mailto:jklix@iwhr.com">技术开发部</a>&nbsp;&nbsp;&nbsp;&nbsp;联系电话 1190&nbsp;分机&nbsp;11,18,20</span> &nbsp;</div>'
				+ '<div	style="float:right;margin:0 0 0 20px;font:normal 12px tahoma, arial, sans-serif, 宋体;" >'
				+ '<span style="color:white"><a href="X-MIS操作手册.doc">操作手册</a>&nbsp;&nbsp;<a href="javascript:;" onClick="xMis.turnToMenu(\'\',\'organization/addressBook.jsp\',\'公司通讯录\');">通讯录</a>&nbsp;&nbsp;版本号：'
				+ __version + '</span> &nbsp;</div>' + '</div>'
	});

	var tools = [{
				id : 'gear',
				handler : function() {
					Ext.Msg.alert('Message', 'The Settings tool was clicked.');
				}
			}, {
				id : 'close',
				handler : function(e, target, panel) {
					panel.ownerCt.remove(panel, true);
				}
			}];
	var tabContextMenu = new Ext.ux.TabContextMenu({
				items : [{
							itemId : 'refresh',
							text : '刷新',
							scope : this,
							handler : function(tabs, item) {
								var tabid = item.getId();
								var id = tabid.substring(3);
								var ifr = window.frames['iframe' + id].location?window.frames['iframe' + id]:window.frames['iframe' + id].contentWindow;
								if (ifr) {
									item.setIconClass('tab-loading');
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
									xMis.tabCount--;
								}
							}
						}, {
							itemId : 'closeOthers',
							text : '关闭其它',
							scope : this,
							handler : function(tabs, item) {
								tabs.items.each(function(tab) {
											if (tab.closable && tab != item) {
												tabs.remove(tab);
												xMis.tabCount--;
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
	var newMessageInfoList = new com.bitc.xmis.workflow.MessageInfoList({
				state : 1,
				listSize : 0
			});
	var newTodoList = new com.bitc.xmis.workflow.TodoList({
				state : 0
			});
	var myTodoList = new com.bitc.xmis.workflow.TodoList({
				state : 1
			});
			/**
	var newTodoList = new com.bitc.xmis.workflow.TodoList({
				state : 0,
				showHeight:true,
				listSize : 0
			});
	var myTodoList = new com.bitc.xmis.workflow.TodoList({
				state : 1,
				showHeight:true,
				listSize : 0
			});
			*/
	xMis.addRefresh('messageTodo', function() {
				this.reload();
			}, newMessageInfoList);
	xMis.addRefresh('newtodo', function() {
				this.reload();
			}, newTodoList);
	xMis.addRefresh('mytodo', function() {
				this.reload();
			}, myTodoList);
	// var myHistTaskList=new com.bitc.xmis.workflow.WorkItemHistoryList();
	var myAttentionList = new com.bitc.xmis.workflow.AttentionList();
	xMis.addRefresh('myattention', function() {
				this.reload()
			}, myAttentionList);
	var contractStateList = new com.bitc.xmis.ContractStateGroupGrid();
	xMis.addRefresh('contractState', function() {
				this.reload()
			}, contractStateList);
	// var paymentPromptList = new com.bitc.xmis.PaymentPromptGroupGrid();
	// xMis.addRefresh('paymentPrompt', function() {
	// this.reload()
	// }, paymentPromptList);
	var myDraftList = new com.bitc.xmis.sys.DraftList({
				listSize : 0
			});
	xMis.addRefresh('mydraft', function() {
				this.reload();
			}, myDraftList);

	var date_space = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;-&nbsp&nbsp;&nbsp;&nbsp;-&nbsp;&nbsp;&nbsp;';
	var time_space = '&nbsp;&nbsp;&nbsp;&nbsp;:&nbsp;&nbsp;&nbsp;:&nbsp;&nbsp;&nbsp;&nbsp;';
	if (yesterdayDate == '') {
		yesterdayDate = date_space;
	}
	if (yesterdayEarly == '') {
		yesterdayEarly = time_space;
	}
	if (yesterdayLate == '') {
		yesterdayLate = time_space;
	}
	if (todayEarly == '') {
		todayEarly = time_space;
	}
	if (todayLate == '') {
		todayLate = time_space;
	}
	var homePage = {
		id : 'tab0',
		xtype : 'portal',
		title : '首页',
		tabTip : '双击最大化/还原窗口',
		iconCls : 'tab-sy',
		closable : false,
		margins : '35 5 5 0',
		bodyStyle : 'overflow-x:hidden;overflow-y:auto',// 去掉横向滚动条
		items : [{
			columnWidth : .5,
			style : 'padding:5px 5px 5px 5px',
			items : [{
				title : '提示',
				html : '<div style=font-size:13px;padding-top:0px;padding-bottom: 5px;sans-serif, 宋体;border-bottom:2px solid #2e8fba;>上次登录：'
						+ lastDate
						+ ' &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;本周登录：'
						+ logCount + '&nbsp;次<br/><br/><font color=#FF3300>之前考勤打卡：'+yesterdayDate+'&nbsp;&nbsp;首次 '+yesterdayEarly+'&nbsp;&nbsp;末次 '+yesterdayLate+'<br/><br/>本日考勤打卡：'+todayDate+'&nbsp;&nbsp;首次 '+todayEarly+'&nbsp;&nbsp;末次 '+todayLate+'</font><br/><br/><a href="http://192.168.103.241:5880/download/"  target="_blank"><img src="resources/images/fastmsgicon.jpg" width="20" height="20"  alt="FastMsg" title="FastMsg"/></img>FastMsg企业即时通讯客户端软件下载</a></div>'
					//	+ '<div><a href="javascript:;" onclick="xMis.turn(\'biddinglist\',\'投标分析\',\'/bid/biddingList.jsp\')">投标分析</a></div>'
						// }, {
					// title : '我的消息',
					// tools : [{
					// id : 'refresh',
					// qtip : '刷新',
					// handler : function() {
					// newMessageInfoList.reload();
					// }
					// }, {
					// id : 'plus',
					// qtip : '查看所有消息',
					// handler : function() {
					// xMis.turn('my-messageInfo', '所有消息', __cxtPath
					// + '/workflow/myMessage.jsp',
					// 'messsage', true);
					// }
					// }],
					// items : newMessageInfoList
			}, {
				title : '我的任务',
				collapseFirst : false,
				tools : [{
							id : 'refresh',
							qtip : '刷新',
							handler : function() {
								myTodoList.reload();
							}
						}, {
							id : 'plus',
							qtip : '查看所有任务',
							handler : function() {
								xMis.turn('my-workitems', '所有任务', __cxtPath
												+ '/workflow/myWorkItems.jsp',
										'task', true);
								// myTodoList.reload({
								// params : {
								// limit : -1
								// }
								// });
							}
			 
						}, {
							id : 'save',
							qtip : '工作流查询',
							hidden:__emp.userID==-1?false:true,
							handler : function() {
								xMis.turn('my-workitems3', '工作流查询', __cxtPath
												+ '/workflow/processInstancesList.jsp',
										'task', true);
							}
						 
						}],
				items : myTodoList
//			}, {
//				title : '我的任务 ( 签收未完成的任务 )',
//				collapseFirst : false,
//				tools : [{
//							id : 'refresh',
//							qtip : '刷新',
//							handler : function() {
//								myTodoList.reload();
//							}
////						},{
////							qtip : '考勤审批的批量处理',
////							hidden:true,
////							handler : function() {
////								xMis.turn('my-workitems2', '考勤审批的批量处理', __cxtPath
////												+ '/workflow.do?method=getTileAndTaskIDByUserID',
////										'task', true);
////							}
//						}],
//				items : myTodoList
			}, {
				title : '草稿箱',
				items : myDraftList
			}, {
				title : '日程安排',
				hidden:true,
				tools : tools
					// ,
					// autoLoad : __cxtPath
					// + '/portal/scheduleListOnPortal.jsp'
				}, {
				title : '外出提示',
				hidden:true,
				tools : tools
				},{
				collapsed :true,
				title : '<font color=#FF3300>发票真伪查询试用</font>&nbsp;&nbsp;<a href = "http://fapiao.youshang.com/mobile.html" target="_blank">手机版</a>',
				html : '<iframe name="fapiao" src="http://fapiao.youshang.com/app/app.html" width="540" height="480" marginwidth="0" marginheight="0" hspace="0" vspace="0" frameborder="0" scrolling="no"></iframe>'
			}]
		}, {
			columnWidth : .49,
			style : 'padding:5px 5px 5px 0px',
			items : [{
						title : '关注',
						collapseFirst : false,
						tools : [{
									id : 'refresh',
									qtip : '刷新',
									handler : function() {
										myAttentionList.reload();
									}
								}, {
									id : 'plus',
									qtip : '查看所有关注',
									handler : function() {
										myAttentionList.reload({
													params : {
														limit : -1
													}
												});
									}
								}],
						items : myAttentionList
					}, {
						title : '项目进度',
						tools : tools

						// ,
					// autoLoad : __cxtPath
					// + '/portal/projectListOnPortal.jsp'
				}	, {
						title : '合同状态',
						// tools : tools
						collapseFirst : false,
						tools : [{
									id : 'refresh',
									qtip : '刷新',
									handler : function() {
										contractStateList.reload();
									}
								}],
						items : contractStateList,
						listeners : {
							resize : function() {
								contractStateList.view.refresh();

							}
						}
						// ,
					// autoLoad : __cxtPath
					// + '/portal/contractListOnPortal.jsp'
				}	, {
						title : '收付款提示',
						tools : tools
						// collapseFirst : false,
					// tools : [{
					// id : 'refresh',
					// qtip : '刷新',
					// handler : function() {
					// paymentPromptList.reload();
					// }
					// }],
					// items : paymentPromptList
					// ,
					// autoLoad : __cxtPath
					// + '/portal/paymentListOnPortal.jsp'
				}]
		}]
	};
	var logoPanel = new Ext.Panel({
				region : 'north',
				height : 77,
				layout : 'border',
				border : false,
				collapseMode : 'mini',
				collapsible : true,
				hideCollapseTool : true,
				header : false,
				items : [{
							xtype : 'panel',
							bodyCssClass : 'main-head-logo',
							region : 'west',
							width : 524,
							border : false
						}, {
							xtype : 'panel',
							region : 'center',
							border : false,
							bodyCssClass : 'main-head-loginfo',
							html : loginfo
						}]
			});
	var mainViewport = new Ext.Viewport({
				layout : 'border',
				items : [logoPanel, {
					id : 'workspace',
					xtype : 'extendedtabpanel',
					activeTab : 0,
					region : 'center',
					width : 100,
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
							statusPanel[action](false);
						},
						'beforeshowcontextmenu' : function(tabs, item, menu) {
							if (item == tabs.getItem('tab0')) { // 首页，不显示菜单
								return false;
							}
						}
					}
				}, leftMenu, statusPanel]
			});
	// xMis.turnToMenu('contract-list');
	Ext.state.Manager.getProvider().on('init', function() {
		if (Ext.state.Manager.get('home') !== 12) {
			Ext.Msg.show({
				title : '提示',
				msg : '在人力资源模块可以自行修改登录名。<br/><br/>下次是否提示',
				buttons : Ext.Msg.YESNO,
				closable : false,
				fn : function(btn) {
					if (btn == 'no') {
						Ext.state.Manager.set('home', 12);
						parent.xMis.saveUserNoticeHabit();
					}
				}
			});
		}
	});
	Ext.state.Manager.getProvider().readState();
	/**消息*/
	showTopicMessageWinList('home', empId);
});