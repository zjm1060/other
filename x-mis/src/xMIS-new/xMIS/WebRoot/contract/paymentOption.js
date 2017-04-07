Ext.onReady(function(){
    //Ext.QuickTips.init();
    
    var paymentOptionTabs = new Ext.TabPanel({
		id: 'paymentOptionTabs',
		
//		renderTo: Ext.getBody(),
		renderTo: "paymentOptionDiv", 
        height: 310,
        region: 'center',                    
        margins:'3 3 3 0',                   
        activeTab: 0   //,                    
//        defaults:{autoScroll:true},          
                                             
//        items:[
//    	    {id:'paymentOption1', title: '付款条件手工设置', items: [Ext.getCmp('paymentConditionFrm')], closable : true},
//	    	{id:'paymentOption2', title:'收款信息登记', items: [Ext.getCmp('paymentEditPanel')]}
//        ]                                  
    });   
    
    var paymentOption2Title = "";
    if(contractTypeID == Type_ContractType_Income){
		paymentOptionTabs.add(
//			{
//			items: 
				{id:'paymentOption1', title: '付款条件手工设置', items: Ext.getCmp('paymentConditionFrm')}
		    
//		}
		);
		paymentOption2Title = '收款信息登记';
    }else{
		showPaymentEditGrid(contractID,abb,contractTypeID,currencyGraph);
		paymentOption2Title = '付款信息登记';
    }		

		paymentOptionTabs.add(
//			{
//			items: 
				{id:'paymentOption2', title:paymentOption2Title, items: [Ext.getCmp('paymentEditPanel')]}
		    
//		}
		);
		

    paymentOptionTabs.setActiveTab(0);
    paymentOptionTabs.doLayout();
    
    
     paymentOptionTabs.on('tabchange',function(tabpanel,panel){
		if(panel.getId() == 'paymentOption2'){
			showPaymentEditGrid(contractID,abb,contractTypeID,currencyGraph);
//			panel.removeAll();
//			panel.add({items: paymentListEditFrm});
//			tabpanel.doLayout();
		}
	 });



 });    