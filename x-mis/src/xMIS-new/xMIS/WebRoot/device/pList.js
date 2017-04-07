/**
 * 采购清单列表
 */
Ext.onReady(function() {
			Ext.QuickTips.init();
			var request = com.bitc.xmis.device.PurchasingList.request;
			var purchasingId = request.getParameter('purchasingId');
			var grid = new com.bitc.xmis.device.PurchasingList.List({
						region : 'center',
						border : false,
						autoScroll : true
					});

			new Ext.Viewport({
						layout : 'border',
						items : [grid],
						border : false
					});
			grid.store.load();
		});