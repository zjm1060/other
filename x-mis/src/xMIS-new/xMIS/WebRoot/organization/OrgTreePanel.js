Ext.ns('com.bitc.xmis.OrgTree');
/**
 * @class com.bitc.xmis.OrgTree.LEVEL 组织树各节点的级别。四个级别.
 * @singleton
 */
com.bitc.xmis.OrgTree.LEVEL = function() {
	return {
		/**
		 * 公司级
		 * 
		 * @type {Number}
		 * @property COMP
		 */
		COMP : 1,
		/**
		 * 一级部门级
		 * 
		 * @type {Number}
		 * @property DEPT1
		 */
		DEPT1 : 2,
		/**
		 * 二级部门级
		 * 
		 * @type {Number}
		 * @property DEPT2
		 */
		DEPT2 : 4,
		/**
		 * 人员级
		 * 
		 * @type {Number}
		 * @property EMP
		 */
		EMP : 8
	}
}();
/**
 * @class com.bitc.xmis.OrgTreePanel 组织机构树
 * @extends Ext.tree.TreePanel
 */
com.bitc.xmis.OrgTreePanel = Ext.extend(Ext.tree.TreePanel, {
	/**
	 * @cfg {Boolean} checkable 是否有复选框
	 */
	checkable : false,
	/**
	 * @cfg {Boolean} singleCheck 是否可多选
	 */
	singleCheck : false,
	/**
	 * @cfg {Number} rootLevel 最高级别 默认为公司级
	 */
	rootLevel : com.bitc.xmis.OrgTree.LEVEL.COMP,
	/**
	 * @cfg {Number} leafLevel 最低级别 默认为人员级
	 */
	leafLevel : com.bitc.xmis.OrgTree.LEVEL.EMP,
	/**
	 * @cfg {String} rootId
	 *      最高级别上一级节点的ID。比如最高级别为一级部门，这时应该指定其上一级别即公司级的ID，这样才知道是显示哪一个公司的一级部门
	 */
	rootId : '',
	/**
	 * @cfg {String} rootFilter 最高级别对应的ID范围，多个以逗号隔开，如：<code>1,1568,1571,1578</code>。默认为空。
	 */
	rootFilter : '',
	/**
	 * @cfg {Number} autoExpandLevel 自动展开的级别。如需多个级别都自动展开，使用多个级别相加。see also
	 *      com.bitc.xmis.OrgTree.LEVEL
	 */
	autoExpandLevel : 0,
	width : 200,
	maxWidth : 250,
	minWidth : 200,
	autoScroll : true, // 滚动条
	animate : true, // 展开,收缩动画
	rootVisible : false, // 是否显示根节点
	initComponent : function() {
		this.root = Ext.apply({
					nodeType : 'async',
					draggable : false, // 可拖动的
					id : 'root-' + this.rootId
				}, this.root);

		var params = {
			checkable : this.checkable,
			singleCheck : this.singleCheck,
			rootLevel : this.rootLevel,
			leafLevel : this.leafLevel,
			rootFilter : this.rootFilter,
			attributor : this.attributor
		};
		this.loader = new Ext.tree.TreeLoader({
					dataUrl : String.format(
							'{0}/orgMaint.do?method=orgTreeLoader', __cxtPath),
					baseParams : params
				});
		com.bitc.xmis.OrgTreePanel.superclass.initComponent.call(this);
	},
	onRender : function(ct, position) {
		if (this.singleCheck) {
			this.on('checkchange', function(node, checked) {
						if (!checked) {
							return;
						}
						var checkedNodes = this.getChecked();
						for (i = 0; i < checkedNodes.length; i++) {
							var n = checkedNodes[i];
							if (n.id != node.id) {
								n.getUI().toggleCheck(false);
								if (n.attributes.checked) {
									n.attributes.checked = false;
								}
								this.fireEvent('checkchange', n, false);
							}
						}
					}, this);
		}
		this.on('append', function(tree, parent, node) {
					var nodeLevel = node.attributes.attributes.level;
					if (!node.leaf
							&& (this.autoExpandLevel & nodeLevel) == nodeLevel) {
						node.expand();
					}
				}, this);
		com.bitc.xmis.OrgTreePanel.superclass.onRender.call(this, ct, position);
	},
	getChildren : function(node, callback, scope) {
		var params = {
			checkable : this.checkable,
			singleCheck : this.singleCheck,
			rootLevel : this.rootLevel,
			leafLevel : this.leafLevel,
			rootFilter : this.rootFilter,
			attributor : this.attributor,
			node : node.id
		};
		Ext.Ajax.request({
					url : String.format('{0}/orgMaint.do?method=orgTreeLoader',
							__cxtPath),
					params : params,
					scope : this,
					success : function(response) {
						var arr = Ext.decode(response.responseText);
						if (callback) {
							callback.call(scope || this, arr);
						}
					}
				})
	}
});
