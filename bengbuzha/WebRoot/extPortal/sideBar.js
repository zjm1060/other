
Ext.onReady(function(){
    Ext.QuickTips.init();

//		var projectTree = new Ext.tree.TreePanel( {
//			id: 'projectMenu',
//			border : false,
//			rootVisible : false,
//			title: "项目管理",
//			root : new Ext.tree.AsyncTreeNode( {
//				text : "主数据管理",
//				expanded : true,
//				children : [ {
//					id : Menu_ID_ProjectManagement_ProjectStart,
//					text : "项目启动",
//					leaf : true
//				}, {
//					id : Menu_ID_ProjectManagement_TaskAssign,
//					text : "任务分配",
//					leaf : true
//				}, {
//					id : "permissions",
//					text : "权限管理"
//				}]
//			})
//
//		});
	
//		var contractTree = new Ext.tree.TreePanel( {
//			id: 'contractTree',
//			border : false,
//			rootVisible : false,
//			title: "合同管理",
//			root : new Ext.tree.AsyncTreeNode( {
//				text : "合同管理",
//				expanded : false,
//				children : [ {
//					id : "docRec",
//					text : "合同录入",
//					leaf : true
//				}, {
//					id : "docSend",
//					text : "发送公文",
//					leaf : true
//				}, {
//					id : "docManage",
//					text : "公文管理",
//					leaf : true
//				}]
//			})
//		});	
	
		var projectTree = new Ext.tree.TreePanel( {
			id: treePre+Menu_ID_ProjectManagement_TreeID,
			border : false,
			rootVisible : false,
			title: "项目管理",
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_ProjectMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_ProjectManagement_TreeID);
	        	},
	        	
	            'click':function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode( {
					id: 'projectRoot',
					expanded : true
					
				})
		});

		var contractTree = new Ext.tree.TreePanel( {
			id: treePre+Menu_ID_ContractManagement_TreeID,
			border : false,
			rootVisible : false,
			title: "合同管理",
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_ContractMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_ContractManagement_TreeID);
	        	},
	        	
	            'click':function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode( {
					id: 'contractRoot',
					expanded : true
					
				})
		});		
		
		var myDeskTree = new Ext.tree.TreePanel( {
			id: treePre+Menu_ID_myDesk_TreeID,
			border : false,
			rootVisible : false,
			title: "我的桌面",
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_myDeskMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_myDesk_TreeID);
	        	},
	        	
	            'click':function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode( {
					id: 'myDeskRoot',
					expanded : true
					
				})
		});		
		
		var myTaskTree = new Ext.tree.TreePanel( {
			id: treePre+Menu_ID_myTask_TreeID,
			border : false,
			rootVisible : false,
			title: "我的任务",
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_myTaskMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_myTask_TreeID);
	        	},
	        	
	            'click':function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode( {
					id: 'myTaskRoot',
					expanded : true
					
				})
		});				

		var materialTree = new Ext.tree.TreePanel( {
			id: treePre+Menu_ID_MaterialManagement_TreeID,
			border : false,
			rootVisible : false,
			title: "物资管理",
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_MaterialMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_MaterialManagement_TreeID);
	        	},
	        	
	            'click':function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode( {
					id: 'materialRoot',
					expanded : true
					
				})
		});				

		var meetingTree = new Ext.tree.TreePanel( {
			id: treePre+Menu_ID_MeetingManagement_TreeID,
			border : false,
			rootVisible : false,
			title: "会议管理",
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_MeetingMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_MeetingManagement_TreeID);
	        	},
	        	
	            'click':function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode( {
					id: 'meetingRoot',
					expanded : true
					
				})
		});				

		var crmTree = new Ext.tree.TreePanel( {
			id: treePre+Menu_ID_CrmManagement_TreeID,
			border : false,
			rootVisible : false,
			title: "客户关系管理",
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_CrmMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_CrmManagement_TreeID);
	        	},
	        	
	            'click':function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode( {
					id: 'crmRoot',
					expanded : true
					
				})
		});				

		var issueTree = new Ext.tree.TreePanel( {
			id: treePre+Menu_ID_IssueManagement_TreeID,
			border : false,
			rootVisible : false,
			title: "缺陷管理",
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_IssueMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_IssueManagement_TreeID);
	        	},
	        	
	            'click':function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode( {
					id: 'issueRoot',
					expanded : true
					
				})
		});				
		
		var orgTree = new Ext.tree.TreePanel( {
			id: treePre+Menu_ID_OrganizationManagement_TreeID,
			border : false,
			rootVisible : false,
			title: "人员管理",
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_OrganizationMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_OrganizationManagement_TreeID);
	        	},
	        	
	            'click':function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode( {
					id: 'orgRoot',
					expanded : true
					
				})
		});				
		
		var productionTree = new Ext.tree.TreePanel( {
			id: treePre+Menu_ID_ProductionManagement_TreeID,
			border : false,
			rootVisible : false,
			title: "产品管理",
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_ProductionMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_ProductionManagement_TreeID);
	        	},
	        	
	            'click':function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode( {
					id: 'productionRoot',
					expanded : true
					
				})
		});		
		
		var wikiTree = new Ext.tree.TreePanel( {
			id: treePre+Menu_ID_WiKiManagement_TreeID,
			border : false,
			rootVisible : false,
			title: "知识库管理",
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_WiKiMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_WiKiManagement_TreeID);
	        	},
	        	
	            'click':function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode( {
					id: 'wikiRoot',
					expanded : true
					
				})
		});	
		
		var suggestionTree = new Ext.tree.TreePanel({
			
			id: treePre+Menu_ID_SuggestionManagement_TreeID,
			border: false,
			rootVisible: false,
			title: '意见建议',
			request: 'post',
			loader: new Ext.tree.TreeLoader({url: 'loginAction.do?method=buildLeftMenu&treeName='+Tree_Menu_Name_SuggestionMenu}),
	        listeners:{
	        	'expand': function(treePanel){
	            	linkToApplicationWeb(Menu_ID_SuggestionManagement_TreeID);
	        	},
	        	
	            'click': function(node ){
	            	linkToApplicationWeb(node.id);
	            } 
	        },
			root: new Ext.tree.AsyncTreeNode({
					id: 'suggestionRoot',
					expanded : true
					
			})
		});			

		



			
});    














