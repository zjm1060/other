Ext.onReady(function() {
			var tools = [{
						id : 'gear',
						handler : function() {
							Ext.Msg.alert('Message',
									'The Settings tool was clicked.');
						}
					}, {
						id : 'close',
						handler : function(e, target, panel) {
							panel.ownerCt.remove(panel, true);
						}
					}];
			var mainPanel = new Ext.Panel({
						renderTo : Ext.getBody(),
						height : 600,
						width : 900,
						border : false,
						layout : 'border',
						items : [{
									xtype : 'portal',
									region : 'center',
									margins : '35 5 5 0',
									items : [{
										title : '新的任务',
										tools : tools,
										html : '<h1>jakson </h1>'
											// autoLoad:__cxtPath+'/portal/newTaskListOnPortal.jsp'
										}]
								}]
					});
		});