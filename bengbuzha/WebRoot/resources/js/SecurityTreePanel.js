Ext.ns('com.bitc.xmis');
com.bitc.xmis.SecurityTreePanel = {
	LEVEL_ROOT : 0,
	LEVEL_BIZMODULE : 1,
	LEVEL_BIZSUBMODULE : 2,
	LEVEL_BIZFUNCTION : 3,
	LEVEL_BIZDATA : 4
};
com.bitc.xmis.SecurityTreePanel = Ext.extend(Ext.tree.TreePanel, {
	/**
	 * @cfg {Integer} 最高级别
	 */
	rootLevel : com.bitc.xmis.SecurityTreePanel.LEVEL_ROOT,
	/**
	 * @cfg {Integer} 最低级别
	 */
	leafLevel : com.bitc.xmis.SecurityTreePanel.LEVEL_BIZDATA,
	autoScroll : true, // 滚动条
	animate : true, // 展开,收缩动画
	rootVisible : false, // 是否显示根节点
	width : 200,
	maxWidth : 250,
	minWidth : 200,
	initComponent : function() {
		this.root = Ext.apply({
			nodeType : 'async',
			draggable : false, // 可拖动的
			id : '0-0-0' //根节点
		}, this.root);
		var params = {
			leafLevel : this.leafLevel
		};
		com.bitc.xmis.SecurityTreePanel.superclass.initComponent.call(this);
	},
	onRender : function(ct, position) {
		com.bitc.xmis.SecurityTreePanel.superclass.onRender.call(this, ct, position);
	},
	listeners : {
		'append' : function(tree,n,node,index){
        if(!node.isLeaf() &&node.getDepth() <2){  
        node.expand();
     }}
	}
	
	
});