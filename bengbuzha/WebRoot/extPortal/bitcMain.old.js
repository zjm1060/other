Ext.onReady(function() {
//	Ext.QuickTips.init();
//	Ext.lib.Ajax.defaultPostHeader += ";charset=utf-8";
	
	// 1銆佸垱寤篽ead閮ㄥ垎
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
		
		// 2銆佸垱寤篺oot閮ㄥ垎
		var foot = new Ext.Panel( {
			region : 'south',
			html : '<div style=" height:15px; ">'
					+ '<div style="float:left;font:normal 12px tahoma, arial, sans-serif, 瀹嬩綋;margin:0 0 0 0;">'
					+ '<span style="color:blue">鐮斿彂鍗曚綅:	 鍖椾含涓按绉戞妧&nbsp;&nbsp;鐩戞帶浜嬩笟閮�nbsp;鎶�湳寮�彂閮�/span> &nbsp;</div>'
					+ '<div	style="float:right;margin:0 0 0 20px;font:normal 12px tahoma, arial, sans-serif, 瀹嬩綋;" >'
					+ '<span style="color:blue">鐗堟湰鍙凤細V0.1</span> &nbsp;</div>'
					+ '</div>',
//			layout: 'fit',
			header:false,
			frame: true,
//			title: '鐗堟湰锛歏0.1'//,
			margins: '1 3 3 3',
			height : 27
		});


		// 3銆佸垱寤簂eftMenu閮ㄥ垎
		 var leftmenu = new Ext.Panel( {
		 	region : 'west',
		 	title: menuTitle,
		 	split: true,
//		 	html : '<div>瀵艰埅鑿滃崟</div>',
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

		// 4銆佸垱寤轰富鍐呭閮ㄥ垎
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






		// 6銆佸垱寤哄竷灞�
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
					urlStr = __cxtPath+"/portal/xMisPortal.jsp";
					break;
				case Menu_ID_myDesk_MyHomePage:
					urlStr = __cxtPath+"/portal/xMisPortal.jsp";
					break;
				
				case Menu_ID_ContractManagement_TreeID:
					urlStr = __cxtPath+'/contractMain.do?method=getContractList';
					break;
				
				case Menu_ID_ContractManagement_ContractEntry:
					urlStr = __cxtPath+'/contractMain.do?method=contract&contractID=';
					break;

				case Menu_ID_ContractManagement_InvoiceEntry:
					urlStr = __cxtPath+'/contractMain.do?method=invoiceEdit';
					break;

				default:
					urlStr = __cxtPath+"/portal/operationPart.jsp";
					break;
				
			};
			
			sumMain.load({
				url: urlStr,  
				scripts: true,
				scope: this
				});
		}		
		
		

			
	
		
		
		
		
		
		
		
		

});