Ext.ns('com.bitc.xmis');
com.bitc.xmis.OrgTreeChooseField = Ext.extend(Ext.form.TwinTriggerField, {
	initComponent : function() {
		com.bitc.xmis.OrgTreeChooseField.superclass.initComponent.call(this);
		this.addEvents('beforeshowwin');
		this.addEvents('aftershowwin');
	},
	onRender : function(ct, position) {
		com.bitc.xmis.OrgTreeChooseField.superclass.onRender.call(this, ct,
				position);
		if (this.tooltip && this.tooltip.text)
			new Ext.ToolTip({
						target : this.id,
						trackMouse : false,
						draggable : false,
						maxWidth : 200,
						minWidth : 100,
						title : this.tooltip.title,
						html : this.tooltip.text
					});
	},

	validationEvent : false,
	validateOnBlur : false,
	trigger1Class : 'x-form-clear-trigger',
	trigger2Class : 'x-form-search-trigger',
	hideTrigger1 : true,
	editable : false,
	hasSearch : false,
	hasInit : false,
	autoLoad : true,
	/**
	 * @cfg {Object} scope 函数执行域
	 */
	/**
	 * @cfg {Function} setValueFun 返回选择结果的回调函数
	 */
	setValueFun : function(nodes) {
	},
	/**
	 * @cfg {Function} clearValueFun 清除选择结果的回调函数
	 */
	clearValueFun : function() {
	},
	/**
	 * @cfg {Function} baseParamFun 用于获取数据基础参数的回调函数，返回值将会用于Store的baseParams属性
	 *      应该返回一个JSON对象 例： <code> function() {
	 * return searchName : '', contractTypeID : Ext .getCmp('contractType')
	 * .getValue(); }
	 * </code>
	 */
	baseParamFun : function() {
		return {};
	},
	/**
	 * @cfg {Function} paramFun 用于获取数据参数的回调函数应该返回一个JSON对象 例：<code> function() { return { start : 0, limit :
	 * 20}; }</code>
	 */
	paramFun : function() {
		return {};
	},
	/**
	 * 用于配置选择窗口
	 * 
	 * @type Mixed
	 */
	winConfig : {},

	onTrigger1Click : function() {
		this.clearValueFun();
		this.triggers[0].hide();
	},

	onTrigger2Click : function() {
		if (this.disabled) {
			return;
		}
		if (!this.fireEvent('beforeshowwin', this)) {
			return;
		}
		if (this.hasInit) {
			this.window.show();
			this.fireEvent('aftershowwin', this);
			return;
		}
		var url = this.chooseUrl;
		var baseParam = this.baseParamFun.call(this.scope || window);
		var param = this.paramFun.call(this.scope || window);
		var winConfig = this.winConfig;
		var setValueFun = this.setValueFun;
		var orgTreePanel = this.orgTreePanel = new com.bitc.xmis.OrgTreePanel(Ext
				.apply(this.orgTreePanelConfig || {}, {
							region : 'center'
						}));
		var wintbar = [{
					xtype : 'button',
					ref : '../okBtn',
					text : '选定',
					iconCls : 'ok',
					scope : this,
					handler : this.ok
				}, '-', {
					ref : '../cancelBtn',
					iconCls : 'remove',
					text : '取消',
					scope : this,
					handler : function() {
						this.window.hide();
					}
				}];
		var windowConfig = {
			layout : 'border',
			closeAction : 'hide',
			width : 200,
			height : 300,
			items : [orgTreePanel],
			tbar : {
				xtype : 'toolbar',
				items : wintbar
			}
		};
		Ext.apply(windowConfig, winConfig);
		this.window = new Ext.Window(windowConfig);
		this.hasInit = true;
		this.window.show();
		this.fireEvent('aftershowwin', this);
	},
	ok : function() {
		var nodes = this.orgTreePanel.getChecked();
		if (nodes.length <= 0) {
			return;
		}
		this.setValueFun.call(this.scope || this, nodes);
		this.triggers[0].show();
		this.window.hide();
	}
});