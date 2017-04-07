Ext.onReady(
	function() {
		Ext.QuickTips.init();
		
		var incomeGrid = new com.bitc.xmis.ContractGridPanel({
			id : 'incomeGrid',
			contractTypeID : 1,
			region : 'center',
			decisionFlag : decisionFlag
		});
		var outGrid = new com.bitc.xmis.ContractGridPanel({
			id : 'outGrid',
			contractTypeID : 2,
			region : 'center',
			decisionFlag : decisionFlag
		});
		
		var tabs = new Ext.TabPanel(
		{
			region : 'center',
			activeTab : 1,
			tabPosition : 'top',
			height : 535,
			margins : '3 3 3 0',
			bodyStyle : 'border-left:0',
			activeTab : 1,
			items : [{
				id : Type_ContractType_Income,
				title : '收入合同综合查询',
				layout : 'border',
				items : [incomeGrid]
			}, {
				id : Type_ContractType_Payout,
				title : '支出合同综合查询',
				layout : 'border',
				items : [outGrid]
			}]
		});
		
		new Ext.Viewport(
		{
			layout : 'border',
			items : tabs
		});
		
		if(deptLimitsFlag){
			Ext.getCmp('incomeGrid').getColumnModel().setHidden(7, false);
			Ext.getCmp('incomeGrid').getColumnModel().setHidden(9, false);
			Ext.getCmp('incomeGrid').getColumnModel().setHidden(11, false);
			Ext.getCmp('outGrid').getColumnModel().setHidden(7, false);
			Ext.getCmp('outGrid').getColumnModel().setHidden(9, false);
			Ext.getCmp('outGrid').getColumnModel().setHidden(11, false);
		}
		
});