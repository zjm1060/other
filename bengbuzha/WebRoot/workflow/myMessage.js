Ext.onReady(function() {
	Ext.QuickTips.init();
	var newMsgList = new com.bitc.xmis.workflow.MessageInfoList({
				emptyText : '您暂时没有未读的传阅通知',
				hideHeaders : true,
				state : 1,
				listSize : 0
			});
	var myMsgList = new com.bitc.xmis.workflow.MessageInfoList({
				emptyText : '您暂时没有已读的传阅通知',
				state : 0,
				listSize : 0
			});
	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [{
							xtype : 'tabpanel',
							region : 'center',
							activeTab : 0,
							autoScroll : true,
							deferredRender : false,
							items : [{
										xtype : 'panel',
										border : false,
										title : '未读的传阅通知',
										items : [newMsgList]
									}, {
										xtype : 'panel',
										border : false,
										title : '已读的传阅通知',
										items : [myMsgList]
									}]
						}]
			});
});