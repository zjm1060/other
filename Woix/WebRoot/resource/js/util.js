// 窗口双击最大化/还原
Ext.ns('Ext.ux');
Ext.ux.ExtendedTabPanel = Ext.extend(Ext.TabPanel, {
			initComponent : function() {
				Ext.ux.ExtendedTabPanel.superclass.initComponent.call(this);
				this.addEvents('tabdblclick');
			},
			initEvents : function() {
				Ext.ux.ExtendedTabPanel.superclass.initEvents.call(this);
				this.mon(this.strip, 'dblclick', function(e) {
							var t = this.findTargets(e);
							if (!t.close) {
								this.fireEvent('tabdblclick');
							}
						}, this);
			}
		});
Ext.reg('extendedtabpanel', Ext.ux.ExtendedTabPanel);

Ext.ns('Woix');
// 工作区Tab最大数量
Woix.maxTabSize = 6;
// 初始的Tab数量
Woix.tabCount = 1;

Woix.changeicon = function(tabid, iconCls) {
	var panel = Ext.getCmp(tabid);
	if (panel) {
		panel.setIconClass(iconCls);
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
Woix.turn = function(id, title, url, iconCls, refresh) {
	if (typeof id)
		if (url == "") {
			return;
		}
	var tUrl;
	if (url.indexOf(contextPath + '/') >= 0) {
		tUrl = url;
	} else {
		tUrl = contextPath + "/" + url;
	}
	tUrl = url;
	var tip = title;
	title = Ext.util.Format.ellipsis(Ext.util.Format.stripTags(title), 15);
	var workspace = Ext.getCmp('workspace');
	var tab = workspace.findById('tab-' + id);
	if (tab) {
		workspace.activate(tab);
		var ifr = document.getElementsByTagName('iframe')['iframe-' + id].contentWindow;
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
		if (ifr.location.href.substring(ifr.location.href.indexOf(contextPath)) == tUrl
				.substring(tUrl.indexOf(contextPath))) {
			return;
		}
		tab.setTitle(title, 'tab-loading');
		ifr.location.href = tUrl;
		return;
	}

	workspace.add(new Ext.Panel({
		id : 'tab-' + id,
		title : title,
		tabTip : tip,
		iconCls : 'tab-loading',
		html : '<iframe id="iframe-'
				+ id
				+ '" src="'
				+ tUrl
				+ '" scrolling="auto" frameborder="0" width="100%" height="100%" onload="javascript:Woix.changeicon(\'tab-'
				+ id + '\',\'' + iconCls + '\');"></iframe>',
		closable : true,// 是否允许关闭
		listeners : {
			'close' : function(p) {
				setTimeout(function() {
							if (Ext.isIE) {
								CollectGarbage();
							}
						}, 2000);
				Woix.tabCount--;
			}
		}
	})).show();
	Woix.tabCount++;
};

// 关闭Tab页
Woix.closeTab = function(iwindow) {
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
}

// 刷新
Woix.refreshes = {};

Woix.addRefresh = function(key, fun, scope) {
	Woix.refreshes[key] = {
		callback : fun,
		scope : scope
	}
}

Woix.refresh = function(key) {
	var a = arguments;
	if (a.length > 1) {
		for (var i = 0, len = a.length; i < len; i++) {
			Woix.refresh(a[i]);
		}
	} else if (key) {

		var item = Woix.refreshes[key];
		if (item) {
			item.callback.call(item.scope);
		}
		return;
	} else {
		for (var j in Woix.refreshes) {
			Woix.refresh(j);
		}
	}
}

/**
 * 导出用
 */
function turnStringForColumn(column, fields) {
	var items = column.config;
	var result = [];
	var fieldresult = [];
	for (var i = 0; i < items.length; i++) {
		if (items[i]['hidden'] === true) {
			continue;
		}
		if (items[i]['dataIndex'] == '') {
			continue;
		}
		if (items[i]['excel']) {
			result.push(items[i]['excel']);
		} else {
			result.push(items[i]['header']);
		}
		fieldresult.push(fields.get(items[i]['dataIndex'])['mapping']);
	}
	return {
		columns : result.join(","),
		fields : fieldresult.join(',')
	};
}