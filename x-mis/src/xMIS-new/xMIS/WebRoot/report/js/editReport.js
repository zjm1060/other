Ext.onReady(function() {
			var colCm = new Ext.grid.ColumnModel({
				
			});
			var columnGrid = new Ext.grid.GridPanel({
						region : 'south'
					});
			new Ext.Viewport({
						layout : 'border',
						items : [columnGrid]
					});
		});