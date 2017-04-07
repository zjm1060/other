
Ext.onReady(function() {
//	Ext.QuickTips.init();
//	Ext.lib.Ajax.defaultPostHeader += ";charset=utf-8";
	
	// 1、创建head部分
		var head = new Ext.Panel( {
			region : 'north',
//			border : false,
			layout: 'fit',
			frame: true,
			autoLoad: {
				url: Ext_Portal_Head_File, 
				scripts: true, 
				scope: this
			},
			margins: '3 3 0 3',
			height : 85
		});
		
		// 2、创建foot部分
		var foot = new Ext.Panel( {
			region : 'south',
			html : '<div style=" height:15px; ">'
					+ '<div style="float:left;font:normal 12px tahoma, arial, sans-serif, 宋体;margin:0 0 0 0;">'
					+ '<span style="color:blue">研发单位:	 北京中水科技&nbsp;&nbsp;监控事业部&nbsp;技术开发部</span> &nbsp;</div>'
					+ '<div	style="float:right;margin:0 0 0 20px;font:normal 12px tahoma, arial, sans-serif, 宋体;" >'
					+ '<span style="color:blue">版本号：V0.1</span> &nbsp;</div>'
					+ '</div>',
//			layout: 'fit',
			header:false,
			frame: true,
//			title: '版本：V0.1'//,
			margins: '1 3 3 3',
			height : 27
		});


		// 3、创建leftMenu部分
		 var leftmenu = new Ext.Panel( {
		 	region : 'west',
		 	title: menuTitle,
		 	split: true,
//		 	html : '<div>导航菜单</div>',
		 	width : 200,
            minSize: 175,
            maxSize: 400,
//            frame: true,
            margins: '1 0 0 3',
            collapsible: true,
            
        	autoScroll:true, 
			
        	layout: {
                type: 'accordion',
                animate: true
            }
		 	

		 });

		 leftmenu.add(
		 		Ext.getCmp(treePre+Menu_ID_myDesk_TreeID), 
		 		Ext.getCmp(treePre+Menu_ID_ProjectManagement_TreeID),
		 		Ext.getCmp(treePre+Menu_ID_ContractManagement_TreeID),
		 		Ext.getCmp(treePre+Menu_ID_MaterialManagement_TreeID),
		 		Ext.getCmp(treePre+Menu_ID_MeetingManagement_TreeID),
		 		Ext.getCmp(treePre+Menu_ID_CrmManagement_TreeID),
		 		Ext.getCmp(treePre+Menu_ID_IssueManagement_TreeID),
		 		Ext.getCmp(treePre+Menu_ID_OrganizationManagement_TreeID),
		 		Ext.getCmp(treePre+Menu_ID_ProductionManagement_TreeID),
		 		Ext.getCmp(treePre+Menu_ID_WiKiManagement_TreeID),
		 		Ext.getCmp(treePre+Menu_ID_SuggestionManagement_TreeID),
		 		Ext.getCmp(treePre+Menu_ID_myTask_TreeID)
		 	
		 );

		// 4、创建主内容部分
		 var sumMain = new Ext.Panel( {
			 region : 'center',
	         margins: '1 3 0 0',
        	 autoScroll:true, 
//	         frame: true,
//				autoLoad: {
//					url: 'contractMain.do?method=getContractList&searchtype=integration&layout=ext',
////					url: 'contractMain.do?method=getContractList&layout=ext',
////					url: 'contractMain.do?method=contract&contractID=&layout=ext',
//					scripts: true, 
//					scope: this
//				},
				
	         layout: {
	            type: 'accordion',
	            animate: true
	         }
		 });






		// 6、创建布局
		var viewport = new Ext.Viewport( {
			layout : 'border',
//			style : 'border:#024459 2px solid;',
			items : [head, foot, leftmenu, sumMain]
		});
		

		
		this.linkToApplicationWeb = function(menuID){
//			alert(nodeID);	
			
			
			var urlStr;
			switch(menuID){
				case Menu_ID_myDesk_TreeID:
					urlStr = "/xMIS/portal/xMisPortal.jsp";
					break;
				case Menu_ID_myDesk_MyHomePage:
					urlStr = "/xMIS/portal/xMisPortal.jsp";
					break;
				
				case Menu_ID_ContractManagement_TreeID:
					urlStr = 'contractMain.do?method=getContractList';
					break;
				
				case Menu_ID_ContractManagement_ContractEntry:
					urlStr = 'contractMain.do?method=contract&contractID=';
					break;

				case Menu_ID_ContractManagement_InvoiceEntry:
					urlStr = 'contractMain.do?method=invoiceEdit';
					break;

				default:
					urlStr = "/xMIS/portal/operationPart.jsp";
					break;
				
			};
			
			sumMain.load({
				url: urlStr,  
				scripts: true,
				scope: this
				});
		}		
		
		

			
	
		
		
		
		
		
		
		
		

});