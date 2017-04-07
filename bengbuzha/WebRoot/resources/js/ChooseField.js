/*
 * ! Ext JS Library 3.2.0 Copyright(c) 2006-2010 Ext JS, Inc.
 * licensing@extjs.com http://www.extjs.com/license
 */
Ext.ns('Ext.ux.form');
/**
 * @class Ext.ux.form.ChooseField
 * @extends Ext.form.TwinTriggerField 选择组件
 * @author bomwu
 * @version 1.0
 */
Ext.ux.form.ChooseField = Ext.extend(Ext.form.TwinTriggerField, {
			initComponent : function() {
				Ext.ux.form.ChooseField.superclass.initComponent.call(this);
				this.addEvents(
						/**
						 * @event beforeshowwin 选择窗口显示前触发
						 * @param {Ext.ux.form.ChooseField}
						 *            choosefield this field
						 */
						'beforeshowwin',
						/**
						 * @event aftershowwin 选择窗口显示后触发
						 * @param {Ext.ux.form.ChooseField}
						 *            choosefield this field
						 */
						'aftershowwin');
			},
			// private
			onRender : function(ct, position) {
				Ext.ux.form.ChooseField.superclass.onRender.call(this, ct,
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
			
			onlySearch : false,
			validationEvent : false,
			validateOnBlur : false,
			trigger1Class : 'x-form-clear-trigger',
			trigger2Class : 'x-form-search-trigger',
			hideTrigger1 : true,
			editable : false,
			/**
			 * @property hasSearch 是否已经查询
			 * @type Boolean
			 */
			hasSearch : false,
			// private
			hasInit : false,
			autoLoad : true,
			/**
			 * @cfg {Function} setValueFun 返回选择结果的回调函数
			 */
			setValueFun : Ext.emptyFn,
			/**
			 * @cfg {Function} clearValueFun 清除选择结果的回调函数
			 */
			clearValueFun : Ext.emptyFn,
			/**
			 * @cfg {String} chooseUrl 选择资源的URL，此URL请求的Response应该是一个JSON对象
			 */
			chooseUrl : '',
			/**
			 * @cfg {Function} baseParamFun
			 *      用于获取数据基础参数的回调函数，返回值将会用于Store的baseParams属性 应该返回一个JSON对象 例：
			 * 
			 * <pre><code>
			 *  function() {
			 * 		return { 
			 * 			contractTypeID : Ext .getCmp('contractType').getValue()
			 * 		}; 
			 * }
			 * </code></pre>
			 * 
			 */
			baseParamFun : function() {
				return {};
			},
			/**
			 * @cfg {Function} paramFun 用于获取数据参数的回调函数应该返回一个JSON对象 例：
			 * 
			 * <pre><code>
			 *  function() { 
			 *  	return { 
			 *  		start : 0, 
			 *  		limit : 20
			 *  	}; 
			 *  }
			 * </code>
			 * </pre>
			 */
			paramFun : function() {
				return {};
			},
			/**
			 * @cfg {Object} winConfig 用于配置选择窗口
			 */
			winConfig : {},

			search : function() {
				if (this.searchField) {
					this.searchField.onTrigger2Click();
					this.hasSearch = true;
				}
			},

			onTrigger1Click : function() {
				this.clearValueFun.call(this.scope || window);
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
				var setValueFun = this.setValueFun;
				Ext.Ajax.request({
							url : url,
							scope : this,
							success : this.initWindow,
							failure : Ext.emptyFn
						});

			},
			// private
			initWindow : function(response, config) {
				json = Ext.util.JSON.decode(response.responseText);
				var sm = new Ext.grid.CheckboxSelectionModel({
							header : '',
							singleSelect : true
						});
				json.columnModle = [sm].concat(json.columnModle);
				var cm = new Ext.grid.ColumnModel(json.columnModle);
				var store = new Ext.data.Store({
							proxy : new Ext.data.HttpProxy({
										url : __cxtPath + '/' + json.dataUrl
									}),
							remoteSort : true,
							sortInfo : json.sortInfo,
							reader : new Ext.data.JsonReader({
										root : 'rows',
										totalProperty : 'totalCount',
										remoteSort : true,
										fields : json.fieldsNames
									}),
							baseParams : this.baseParamFun.call(this.scope
									|| window)
						});
				var gridConfig = {
					sm : sm,
					cm : cm,
					store : store,
					disableSelection : false,
					region : 'center',
					layout : 'fit',
					enableColumnMove : false,
					enableColumnResize : true,
					stripeRows : true,
					height : 340,
					loadMask : {
						msg : '正在加载数据，请稍侯……'
					},
					bbar : new Ext.PagingToolbar({
								pageSize : 20,
								store : store,
								displayInfo : true,
								displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
								emptyMsg : "无显示数据"
							})
				};
				var tbar;
				if (json.gridConfig) {
					if (json.gridConfig.tbar) {
						tbar = json.gridConfig.tbar;
						json.gridConfig.tbar = undefined;
						if (tbar instanceof Array) {
							for (var i = 0; i < tbar.length; i++) {
								tbar[i].scope = this;
							}
						} else if (tbar instanceof Object) {
							tbar.scope = this;
						}
					}
					Ext.apply(gridConfig, json.gridConfig);
				}
				var grid = this.grid = new Ext.grid.GridPanel(gridConfig);
				var wintbar = [{
							xtype : 'button',
							ref : '../okBtn',
							text : '选定',
							iconCls : 'ok',
							scope : this,
							handler : function() {
								var record = grid.getSelectionModel()
										.getSelected();
								this.setValueFun.call(this.scope || window,
										record);
								this.triggers[0].show();
								this.window.hide();
							}
						}, '-', {
							ref : '../cancelBtn',
							iconCls : 'remove',
							text : '取消',
							scope : this,
							handler : function() {
								this.window.hide();
							}
						}];
				var param = this.paramFun.call(this.scope || window);
				if (tbar && param.compID != 1) {
					wintbar.push('-');
					wintbar.push(tbar);
				}
				if (json.searchBar) {
					var searchConfig = Ext.apply(json.searchFieldConfig || {},
							{
								store : grid.getStore()
							});
					wintbar.push('-');
					this.searchField = new Ext.ux.InklingSearchField(searchConfig);
					wintbar.push(this.searchField);
				}
				var windowConfig = {
					items : [grid],
					tbar : {
						xtype : 'toolbar',
						items : wintbar
					}
				};
				Ext.apply(windowConfig, this.winConfig);
				this.window = new Ext.ChooseWindow(windowConfig);
				this.hasInit = true;
				this.window.show();
				if (this.autoLoad) {
					grid.getStore().load({
								params : param
							});
					this.hasSearch = true;
				}
				grid.getView().refresh();
				if(this.onlySearch)
					this.window.addBtn.hide();
				this.fireEvent('aftershowwin', this);
			},
			// private
			onDestroy : function() {
				Ext.destroy(this.window);
				Ext.ux.form.ChooseField.superclass.onDestroy.call(this);
			}
		});
Ext.reg('choosefield', Ext.ux.form.ChooseField);

Ext.ns('Ext');
/**
 * @class Ext.ChooseWindow
 * @extends Ext.Window 选择窗口
 */
Ext.ChooseWindow = Ext.extend(Ext.Window, {
			initComponent : function() {
				Ext.ChooseWindow.superclass.initComponent.call(this);
			},
			width : 500,
			height : 425,
			closeAction : 'hide',
			layout : 'border',
			callback : function() {
			}
		});
