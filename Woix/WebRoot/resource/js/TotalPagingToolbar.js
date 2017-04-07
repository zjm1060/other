Ext.ns('Ext.ux');

Ext.ux.TotalPagingToolbar = Ext.extend(Ext.PagingToolbar, {
			promptMsg : '',
			updateInfo : function() {
				if (this.displayItem) {
					var count = this.store.getCount() - 1;
					var dmsg = (this.promptMsg.length > 0) ? this.promptMsg
							+ this.displayMsg : this.displayMsg;
					var msg = (count == 0 || count == -1)
							? this.emptyMsg
							: String
									.format(dmsg, this.cursor + 1, this.cursor
													+ count, this.store
													.getTotalCount());
					this.displayItem.setText(msg);
				}
			},
			bindStore : function(store, initial) {
				var doLoad;
				this.promptMsg = '';
				if (!initial && this.store) {
					if (store !== this.store && this.store.autoDestroy) {
						this.store.destroy();
					} else {
						this.store.un('beforeload', this.beforeLoad, this);
						this.store.un('load', this.onLoad, this);
						this.store.un('exception', this.onLoadError, this);
					}
					if (!store) {
						this.store = null;
					}
				}
				if (store) {
					store = Ext.StoreMgr.lookup(store);
					store.on({
								scope : this,
								beforeload : this.beforeLoad,
								load : this.onLoad,
								exception : this.onLoadError
							});
					doLoad = true;
				}
				this.store = store;
				if (doLoad) {
					this.onLoad(store, null, {});
				}
			}
		});

/**
 * @class Ext.ux.PagelimitCombo
 * @extends Object 用于PagingToolbar的扩展，使用户可以定制每一页显示的记录数
 * @plugins
 */
Ext.ux.PagelimitCombo = Ext.extend(Object, {
			width : 50,
			/**
			 * @cfg {Array} defaultData 选择框里默认的选择项。整型数组。（默认为[20, 30, 40, 50]）
			 */
			defaultData : [20, 30, 40, 50],
			/**
			 * @constructor
			 * @param {Object}
			 *            config The config object
			 */
			constructor : function(config) {
				if (config) {
					Ext.apply(this, config);
				}
			},
			// private
			init : function(parent) {
				this.parent = parent;
				this.combo = new Ext.form.ComboBox({
							store : this.defaultData,
							width : this.width,
							typeAhead : true,
							editable : true,
							triggerAction : 'all',
							value : parent.pageSize,
							onKeyUp : function(e) {
								var k = e.getKey();
								if (this.editable !== false
										&& this.readOnly !== true
										&& (k == e.BACKSPACE || !e
												.isSpecialKey())) {
									this.lastKey = k;
								}
								Ext.form.ComboBox.superclass.onKeyUp.call(this,
										e);
							}
						});
				this.combo.on('select', function(combo) {
							var value = combo.getRawValue();
							if (isNaN(value)) {
								Ext.Msg.alert("提示", "请输入数字。", function() {
											combo.focus(true);
										});
								return;
							}

							if (this.parent.store.lastOptions == undefined) {
								this.parent.limitCount = value;
							} else {
								Ext.apply(this.parent.store.lastOptions.params,
										{
											limit : value
										});
							}
							this.parent.store.baseParams['limit'] = value;
							this.parent.pageSize = parseInt(value);
							this.parent.moveFirst();
						}, this);
				this.combo.on('specialkey', function(f, e) {
							if (e.getKey() == e.ENTER) {
								this.combo.fireEvent('select', this.combo);
							}
						}, this);
				var index = parent.items.indexOf(parent.refresh);
				parent.insertButton(index + 1, ['-', '每页', this.combo, '条记录']);
				parent.doLayout();
			}
		});
