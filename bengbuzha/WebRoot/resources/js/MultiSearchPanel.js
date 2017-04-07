Ext.ns('Ext.ux');
Ext.ux.MultiSearchPanel = Ext.extend(Ext.Panel, {
			/**
			 * @cfg {Array}
			 *      包含的可选条件,里面的元素应为JSON对象，对象的属性参见Ext.ux.form.CheckTextField
			 */
			searchItems : undefined,
			/**
			 * @cfg {Object} 查询按钮的配置
			 */
			buttonConfig : undefined,
			store : undefined,
			border : false,
			baseCls : 'x-plain',
			initComponent : function() {
				var ctfPanels = [];
                this.ctfs=new Array();
				Ext.each(this.searchItems, function(item, index) {
							var ctf = new Ext.ux.form.CheckTextField(item);
							var ctfPanel = new Ext.Panel({
										border : false,
										baseCls : 'x-plain',
										layout : 'form',
										items : [ctf]
									});
							this.ctfs.push(ctf);
							ctfPanels.push(ctfPanel);
						}, this);
				var searchPanel = new Ext.Panel({
							border : false,
							baseCls : 'x-plain',
							items : ctfPanels
						});
				this.searchButton = new Ext.Button(Ext.apply({
							text : '查询',
							scope : this,
							handler : function() {
								if (!this.store) {
									Ext.Msg.alert('错误', '查询面板无关联的Store，无法查询');
									return;
								}
								this.setBaseParam();
								this.fireEvent('beforesearch', this);
								var o = {
									start : 0
								};
								this.store.reload({
											params : o
										});
								this.fireEvent('onsearch', this);
							}
						}, this.buttonConfig || {}));
				this.buttonPanel = new Ext.Panel({
							border : false,
							region : 'south',
							layout : 'hbox',
							height : 28,
							baseCls : 'x-plain',
							layoutConfig : {
								pack : 'center'
							},
							items : [this.searchButton]
						});
				this.items = this.items || [];
				this.items.push([searchPanel, this.buttonPanel]);
				Ext.ux.MultiSearchPanel.superclass.initComponent.call(this);
				this.addEvents(
						/**
						 * @event onsearch Fires when the searchbutton is
						 *        clicked.
						 * @param {Ext.ux.MultiSearchPanel}
						 *            this This SearchPanel
						 */
						'onsearch');
				this.addEvents(
						'beforesearch');
			},
			getParamString : function() {
				var selIds = [];
				var str = [];
				Ext.each(this.ctfs, function(item, index) {
							if (item.isChecked) {
								selIds.push(item.condition);
								str.push(item.getParamString());
							}
						});
				str.push('selIds=' + selIds.join('|'));
				return str.join('&');
			},
			setBaseParam : function(store) {
				var targetStore = store || this.store;
				if (!targetStore) {
					return;
				}
				targetStore.baseParams = targetStore.baseParams || {};
				var selIds = [];
				Ext.each(this.ctfs, function(item, index) {
							if (item.isChecked()) {
								targetStore.baseParams[item.condition] = item
										.getValue();
								selIds.push(item.condition);
							} else {
								delete targetStore.baseParams[item.condition];
							}
						}, this);
				targetStore.baseParams['selIds'] = selIds.join('|');
			},
			getParam : function() {
				var selIds = [];
				var param = {};
				Ext.each(this.ctfs, function(item, index) {
							if (item.isChecked) {
								selIds.push(item.condition);
								param[item.condition] = item.getValue();
							}
						});
				param['selIds'] = selIds.join('|');
				return param;
			},
			hideButtonPanel : function() {
				this.buttonPanel.setVisible(false);
			},
			showButtonPanel : function() {
				this.buttonPanel.setVisible(true);
			},
			clear : function(){//清空
				Ext.each(this.ctfs, function(item, index) {
					if (item.isChecked()) {
						item.checkbox.setValue(false);
						item.field.setValue('');
					}
					if(!item.isChecked()&&item.field.getValue()!=null&&item.field.getValue().trim!=''){
						item.field.setValue('');
					}
				}, this);
			}
		});

Ext.ux.MultiSearchPanel.create = function(id, fn, scope) {
	Ext.Ajax.request({
		url : __cxtPath + '/sys.do',
		params : {
			method : 'getCondition',
			id : id
		},
		success : function(response) {
			var result = Ext.decode(response.responseText);
			var searchItems = new Array();
			if (result && result.items) {
				Ext.each(result.items, function(item, index) {
							if (item != null) {
								var config = undefined;
								if (item.typeConfig && item.typeConfig != "") {
									config = Ext.decode(item.typeConfig);
									config.xtype = item.conditionType.name;
								} else {
									config = {
										xtype : item.conditionType.name
										
									};
								}
								searchItems.push({
											fieldConfig : config,
											condition : 'map_' + item.id,
											text : item.conditionItem.description
										});
							}
						}, this);
			}
			var searchPanel = new Ext.ux.MultiSearchPanel({
						searchItems : searchItems
					});
			fn.call(scope, searchPanel);
		},
		failure : function(response) {
		}
	});
};

Ext.ns('Ext.ux.form');
Ext.ux.form.CheckTextField = Ext.extend(Ext.form.CompositeField, {
			/**
			 * @cfg {Object} 组件中的CheckBox配置
			 */
			checkboxConfig : undefined,
			/**
			 * @cfg {Object} 组件中的TextField配置
			 */
			fieldConfig : undefined,
			/**
			 * @cfg {Object} 条件的名称
			 */
			condition : 'condition',
			/**
			 * @cfg {Object} 要显示的名称
			 */
			text : '条件',

			hideLabel : true,

			initComponent : function() {
				this.checkbox = new Ext.form.Checkbox(Ext.apply({
							id : this.id + '-checkbox',
							boxLabel : this.text
						}, this.checkboxConfig || {}));
				this.field = Ext.create(Ext.apply({
							id : this.id + '-textfield',
							name : this.condition
						}, this.fieldConfig || {}));
				this.items = [this.checkbox, this.field];
				Ext.ux.form.CheckTextField.superclass.initComponent.call(this);
				this.field.on('change', function(field, newValue, oldValue) {
							if (Ext.isEmpty(newValue)) {
								this.checkbox.setValue(false);
							} else {
								this.checkbox.setValue(true);
							}
						}, this);
			},
			getValue : function() {
				return this.checkbox.getValue() ? this.formatValue() : '';
			},
			setValue : function(value) {
				if (Ext.isEmpty(value)) {
					this.checkbox.setValue(false);
					this.field.setValue('');
				} else {
					this.checkbox.setValue(true);
					this.field.setValue(value);
				}
			},
			formatValue : function() {
				var fvalue = this.field.getValue();
				if (Ext.isEmpty(this.field.format)) {
					return fvalue;
				}
				if (Ext.isString(fvalue)) {
					return fvalue;
				} else if (Ext.isDate(fvalue)) {
					return Ext.util.Format.date(fvalue, this.field.format);
				} else if (Ext.isNumber(fvalue)) {
					return Ext.util.Format.number(fvalue, this.field.format);
				}
			},
			isChecked : function() {
				return this.checkbox.getValue();
			},
			getParamString : function() {
				return this.checkbox.getValue() ? this.condition + '='
						+ this.formatValue() : "";
			},
			getParam : function() {
				var obj = new Object();
				if (this.checkbox.getValue()) {
					obj[this.condition] = this.formatValue();
				}
				return obj;
			}
		});
Ext.reg('checktextfield', Ext.ux.form.CheckTextField);