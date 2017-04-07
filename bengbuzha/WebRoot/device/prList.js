Ext.onReady(function() {
			var type = com.bitc.xmis.device.PurchaseRequisition.request
					.getParameter('type');
			var grid = new com.bitc.xmis.device.PurchaseRequisition.List({
						region : 'center',
						storeBaseParams : {
							type : type
						}
					});
			new Ext.Viewport({
						border : false,
						layout : 'border',
						items : [grid]
					});
			grid.getStore().load();
		});