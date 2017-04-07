/*
 * ! Ext JS Library 3.2.0 Copyright(c) 2006-2010 Ext JS, Inc.
 * licensing@extjs.com http://www.extjs.com/license
 */
Ext.ns('Ext.ux.form');
/**
 * @class Ext.ux.form.SearchField
 *        查询输入框。可用于对Grid等Store的查询。用户输入条件后，点击查询按扭或者使用回车键时，该组件将在Store的baseParams中加入用户输入的查询条件，并调用Store的reload方法：
 * 
 * <pre><code>
 *        	...
 *        	var o = {
 * 				start : 0
 * 			};
 * 		this.store.baseParams = this.store.baseParams || {};
 * 		this.store.baseParams[this.paramName] = v;
 * 		this.store.reload({
 * 				params : o
 * 			});
 *        	...
 * </code>
 * v 为用户输入的值
 * </pre>
 * 
 * @extends Ext.form.TwinTriggerField
 */
Ext.ux.form.SearchField = Ext.extend(Ext.form.TwinTriggerField, {
			/**
			 * @cfg {Ext.data.Store} store 绑定查询的Store
			 */
			initComponent : function() {
				Ext.ux.form.SearchField.superclass.initComponent.call(this);
				this.on('specialkey', function(f, e) {
							if (e.getKey() == e.ENTER) {
								this.onTrigger2Click();
							}
						}, this);
			},

			validationEvent : false,
			validateOnBlur : false,
			trigger1Class : 'x-form-clear-trigger',
			trigger2Class : 'x-form-search-trigger',
			hideTrigger1 : true,
			width : 180,
			hasSearch : false,
			/**
			 * @cfg {String} paramName
			 *      相当于Field的name属性，指定该查询发送到后台时使用的名称。（默认为"query"，即后台使用request.getParameter("query")可获得用户输入的查询条件）
			 */
			paramName : 'query',

			onTrigger1Click : function() {
				if (this.hasSearch) {
					this.el.dom.value = '';
					var o = {
						start : 0
					};
					this.store.baseParams = this.store.baseParams || {};
					this.store.baseParams[this.paramName] = '';
					this.store.reload({
								params : o
							});
					this.triggers[0].hide();
					this.hasSearch = false;
				}
			},

			onTrigger2Click : function() {
				var v = this.getRawValue();
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
							params : o
						});
				this.hasSearch = true;
				this.triggers[0].show();
			}
		});