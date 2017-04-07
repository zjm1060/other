//页面布局
Ext.onReady(function(){
	
	var centerPanel = new com.bitc.xmis.contract.incomeAssistSearchPanel({
		region : 'center'
	});
	
	new Ext.Viewport(
		{
			layout : 'border',
			items : centerPanel
	});
})