Ext.ux.TabContextMenu = Ext.extend(Object, {

			items : [],

			constructor : function(config) {
				Ext.apply(this, config || {});
			},

			// public
			init : function(tabs) {
				this.tabs = tabs;
				tabs.addEvents('beforeshowcontextmenu');
				tabs.on({
							scope : this,
							contextmenu : this.onContextMenu,
							destroy : this.destroy
						});
			},

			destroy : function() {
				Ext.destroy(this.menu);
				delete this.menu;
				delete this.tabs;
				delete this.active;
			},

			// private
			onContextMenu : function(tabs, item, e) {
				this.active = item;
				var m = this.createMenu();
				e.stopEvent();
				if (tabs.fireEvent('beforeshowcontextmenu', tabs, item, m) !== false) {
					m.showAt(e.getPoint());
				}
			},

			createMenu : function() {
				if (!this.menu) {
					var menus = [];
					Ext.each(this.items, function(item) {
								if (item == '-') {
									menus.push('-');
								} else {
									menus.push({
												itemId : item.itemId,
												text : item.text,
												scope : this,
												handler : function() {
													item.handler.call(
															item.scope,
															this.tabs,
															this.active);
												}
											});
								}
							}, this);

					this.menu = new Ext.menu.Menu({
								items : menus
							});
				}
				return this.menu;
			}
		});

Ext.preg('tabcontextmenu', Ext.ux.TabContextMenu);