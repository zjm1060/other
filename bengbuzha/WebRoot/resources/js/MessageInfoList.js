Ext.ns('com.bitc.xmis.workflow');
//com.bitc.xmis.workflow.MessageInfoGridPanel = Ext.extend(Ext.grid.GridPanel, {
//	autoLoadData : true,
//	initComponent : function() {
//		var p = this;
//		this.store = new Ext.data.JsonStore({
//					url : String.format(
//							'{0}/msgMaint.do?method=getMessageInfoList',
//							__cxtPath),
//					root : 'rows',
//					baseParams : {
//						state : this.state,
//						start : 0,
//						limit : this.listSize
//					},
//					fields : ['id', 'title', 'content', 'sender', 'received',
//							'sendTime', 'readed'],
//					sortInfo : {
//						field : 'sendTime',
//						direction : 'DESC'
//					},
//					autoLoad : true
//				});
//		this.columns = [{
//					header : '标题',
//					dataIndex : 'title'
//				}, {
//					width : .1,
//					header : '状态',
//					dataIndex : 'readed'
//				}];
//		this.bbar = new Ext.PagingToolbar({
//					pageSize : 20,
//					store : this.store,
//					displayInfo : true,
//					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
//					emptyMsg : "无显示数据",
//					items : this.bbar || []
//				});
//		com.bitc.xmis.workflow.MessageInfoGridPanel.superclass.initComponent
//				.call(this);
//	},
//	onRender : function(a, b) {
//		this.on('click', function(listview, index) {
//			var data = this.getStore().getAt(index).data;
//			// var result = "跳转到详细信息页面……" ;
//			// var result = "发送人：" + data.sender.userName + ";<br>发送时间："
//			// + data.sendTime;
//			// Ext.Msg.alert("提示", result, function() {
//			Ext.Ajax.request({
//						url : String
//								.format(
//										'{0}/msgMaint.do?method=updateMessageInfoState',
//										__cxtPath),
//						method : 'post',
//						params : {
//							id : data.id,
//							state : 0
//						},
//						success : function(response) {
//							var result = Ext.decode(response.responseText);
//							if (result.success) {
//								parent.xMis.turn('doc-info-' + data.id,
//										data.title, __cxtPath + '/'
//												+ data.content,
//										'menu-doc-info', true);
//								parent.xMis.refresh('messageTodo');
//							} else {
//								Ext.Msg.alert(result.messageTitle,
//										result.message)
//							}
//						},
//						failure : function(response) {
//							Ext.Msg.alert('提示', '操作失败');
//						}
//					});
//
//		});
//		// }, this);
//		com.bitc.xmis.workflow.MessageInfoGridPanel.superclass.onRender.call(
//				this, a, b);
//	},
//	reload : function(options) {
//		this.getStore().reload(options);
//	}
//});
/**
 * @class com.bitc.xmis.workflow.MessageInfoList
 * @extends Ext.list.ListView 用户消息列表
 */
com.bitc.xmis.workflow.MessageInfoList = Ext.extend(Ext.list.ListView, {
	/**
	 * @cfg {Number} 列表显示的条数，如果要显示所有，设置为-1
	 */
	listSize : -1,
	hideAction : false,
	hideHeaders : true,
	emptyText : '您暂时没有新传阅通知',
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
					url : String.format(
							'{0}/msgMaint.do?method=getMessageInfoList',
							__cxtPath),
					root : 'rows',
					baseParams : {
						state : this.state,
						start : 0,
						limit : this.listSize
					},
					fields : ['id', 'title', 'content', 'sender', 'received',
							'sendTime', 'readed'],
					sortInfo : {
						field : 'sendTime',
						direction : 'DESC'
					},
					remoteSort:true,
					autoLoad : true
				});
		this.columns = [{
					header : '标题',
					dataIndex : 'title'
				}, {
					width : .1,
					header : '状态',
					dataIndex : 'readed'
				}];
		if (this.hideAction) {
			this.columns.push({
						width : .2,
						header : '日期',
						dataIndex : 'sendTime',
						align : 'center'
					});
		} else {
			var actioncol = new Ext.ux.list.ActionColumn({
						width : .2,
						header : '日期',
						dataIndex : 'sendTime',
						tpl : '{[values.sendTime.substr(5,5)]}',
						getActionsFun : function(value) {
							// return
							// com.bitc.xmis.workflow.TodoActions.getActions(value);
						}
					});
			this.columns.push(actioncol);
			this.plugins = actioncol;
		}
		com.bitc.xmis.workflow.MessageInfoList.superclass.initComponent
				.call(this);
	},
	onRender : function(a, b) {
		this.on('click', function(listview, index) {
			var data = this.getStore().getAt(index).data;
			// var result = "跳转到详细信息页面……" ;
			// var result = "发送人：" + data.sender.userName + ";<br>发送时间："
			// + data.sendTime;
			// Ext.Msg.alert("提示", result, function() {
			Ext.Ajax.request({
						url : String
								.format(
										'{0}/msgMaint.do?method=updateMessageInfoState',
										__cxtPath),
						method : 'post',
						params : {
							id : data.id,
							state : 0
						},
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								parent.xMis.turn('doc-info-' + data.id,
										data.title, __cxtPath + '/'
												+ data.content,
										'menu-doc-info', true);
								parent.xMis.refresh('messageTodo');
							} else {
								Ext.Msg.alert(result.messageTitle,
										result.message)
							}
						},
						failure : function(response) {
							Ext.Msg.alert('提示', '操作失败');
						}
					});

		});
		// }, this);
		com.bitc.xmis.workflow.MessageInfoList.superclass.onRender.call(this,
				a, b);
	},
	reload : function(options) {
		this.getStore().reload(options);
	}
});