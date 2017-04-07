Ext.ns('Ext.ux');
/**
 * @class Ext.ux.GridRowEditForm
 * @extends Ext.util.Observable 按行编辑Grid数据的插件。会弹出一个自动生成Form的Window
 *          用于GridPanel。必须为需要编辑的列配置editor
 */
Ext.ux.GridRowEditForm = Ext.extend(Ext.util.Observable, {
	/**
	 * @cfg {Object} windowConfig config for the window
	 */
	windowConfig : {},
	/**
	 * @cfg {String} newTitlle 新建记录时window显示Title（默认“新建”）
	 */
	newTitle : '新建',
	/**
	 * @cfg {String} editTitle 编辑记录时window显示Title（默认“编辑”）
	 */
	editTitle : '编辑',
	/**
	 * @cfg {String} titleDataIndex
	 */
	titleDataIndex : null,
	/**
	 * @constructor
	 * @param {Object}
	 *            config The config object
	 */
	constructor : function(config) {
		Ext.apply(this, config || {});
		this.addEvents(
				/**
				 * @event beforeshow this event fires before the edit from show
				 * @param {Ext.form.FormPanel}
				 *            form this edit form
				 * @param {Ext.data.Record}
				 *            record the editing record
				 */
				'beforeshow',
				/**
				 * @event aftershow this event fires after the edit form show
				 * @param {Ext.form.FormPanel}
				 *            form this edit form
				 * @param {Ext.data.Record}
				 *            record the editing record
				 */
				'aftershow',
				/**
				 * @event afteredit this event fires after update the record
				 * @param {Ext.form.FormPanel}
				 *            form this edit form
				 * @param {Ext.data.Record}
				 *            record the editing record*
				 */
				'afteredit',
				/**
				 * @event afteredit this event fires after update the record
				 * @param {Ext.form.FormPanel}
				 *            form this edit form
				 * @param {Ext.data.Record}
				 *            record the editing record
				 */
				'canceledit');
		Ext.ux.GridRowEditForm.superclass.constructor.call(this);
	},
	// private
	init : function(grid) {
		this.grid = grid;
		grid.on('render', this.onRender, this);
		this.initWindow();
		grid.on('destroy', this.onDestroy, this);
	},
	initWindow : function() {
		var grid = this.grid;
		var colModel = grid.getColumnModel();
		var items = [];
		var height = 4 * 23;
		Ext.each(colModel.config, function(col) {
					if (!Ext.isEmpty(col.dataIndex) && col.editor) {
						var item = Ext.apply(Ext.apply({
											fieldLabel : col.header
										}, col.editor), {
									name : col.dataIndex
								});
						items.push(item);
						height += 23
								* ((item.xtype && item.xtype === 'textarea')
										? 3
										: 1);
					}
				}, this);
		this.form = new Ext.FormPanel({
					border : false,
					defaultType : 'textfield',
					labelWidth : 80,
					defaults : {
						msgTarget : 'under'
					},
					items : items,
					unstyled : true
				});

		var form = this.form;
		var winconfig = Ext.apply({
					layout : 'fit',
					items : form,
					width : 300,
					bodyStyle : 'padding:5px 5px 5px 5px',
					height : height,
					closeAction : 'hide',
					closable : false,
					modal : true,
					tbar : [{
								iconCls : 'ok',
								text : '确定',
								scope : this,
								handler : this.onUpdate
							}, {
								iconCls : 'cancel',
								text : '取消',
								scope : this,
								handler : function() {
									this.fireEvent('canceledit', this.form,
											this.record);
									this.window.hide();
								}
							}]
				}, this.windowConfig);
		this.window = new Ext.Window(winconfig);
	},
	onRender : function() {
		var grid = this.grid;
		// 为Grid添加newRecord方法
		grid.newRecord = this.editRecord.createDelegate(this, [null, true]);
		grid.on('rowdblclick', this.onRowDblClick, this);
	},
	onRowDblClick : function(grid, rowIdx, e) {
		var r = grid.getStore().getAt(rowIdx);
		this.editRecord(r, false, r.get(this.titleDataIndex) || this.editTitle);
	},
	/**
	 * 编辑记录
	 * 
	 * @param {Record}
	 *            r 要编辑的记录
	 * @param {Boolean}
	 *            isNew 是否为新建
	 * @param {String}
	 *            windowTitle 窗口标题
	 */
	editRecord : function(r, isNew, windowTitle) {
		this.record = r;
		this.isNew = isNew || false;
		if (this.fireEvent('beforeshow', this.form, r) === false) {
			return;
		};
		if (windowTitle) {
			this.window.setTitle(windowTitle);
		} else {
			this.window.setTitle(isNew ? this.newTitle : this.editTitle);
		}
		this.window.show(this.grid.getGridEl());
		if (!isNew) {
			this.form.getForm().loadRecord(r);
		} else {
			this.form.getForm().clear();
		}
		this.fireEvent('aftershow', this.form, r);
	},
	onUpdate : function() {
		if (!this.form.getForm().isValid()) {
			return;
		}
		this.form.getForm().updateRecord(this.record);
		this.fireEvent('afteredit', this.form, this.record);
		this.window.hide();
	},
	onDestroy : function() {
		Ext.destroy(this.window);
	}
});