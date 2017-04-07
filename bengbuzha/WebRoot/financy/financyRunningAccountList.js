Ext.onReady(function() {
	Ext.QuickTips.init();
	var centerPanel = new com.bitc.xmis.FinancyRunningAccountGrid({
		region : 'center',
		stripeRows : true,
		searchType : type,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		}
	});
	
	new Ext.Viewport(
		{
			layout : 'border',
			items : centerPanel
	});
	

}); //end onReady