Ext.define('Ext.ux.MenuItemDragDrop', {
	extend : 'Ext.AbstractPlugin',
	alias : 'plugin.menuitemdragdrop',
	uses : ['Ext.dd.DragZone'],
	enableDrag : true,
	enableDrop : false,
	ddGroup : 'MenuItemDD',
	dragText : '拖动项：{0}',
	init : function(view) {
		var me = this;
		view.on('render', me.onViewRender, me, {
			single : true
		});
	},
	onViewRender : function(view) {
		var me = this;
		if (me.enableDrag) {
			me.dragZone = new Ext.dd.DragZone(view.getEl(), {
				ddGroup : me.dragGroup || me.ddGroup,
				dragText : me.dragText,
				getDragData : function(e) {
					var menuItem = view.getItemFromEvent(e);
					var sourceEl = e.getTarget('.x-menu-item');
					if (sourceEl) {
						var d = sourceEl.cloneNode(true);
						d.id = Ext.id();
						return {
							ddel : d,
							sourceEl : sourceEl,
							repairXY : Ext.fly(sourceEl).getXY(),
							menuItem : menuItem
						}
					}
				},
				getRepairXY : function() {
					return this.dragData.repairXY;
				}
			});

		}
		if (me.enableDrop) {

		}
	}
});