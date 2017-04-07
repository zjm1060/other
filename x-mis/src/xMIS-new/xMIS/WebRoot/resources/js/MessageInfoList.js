Ext.ns('com.bitc.xmis.workflow');
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
			emptyText : '您暂时没有新消息',
			initComponent : function() {
				this.store = new Ext.data.JsonStore({
							url : String.format('{0}/msgMaint.do?method=getMessageInfoList', __cxtPath),
							root : 'rows',
							baseParams : {
								state : this.state,
								start : 0,
								limit : this.listSize
							},
							fields : ['id', 'title', 'content', 'sender', 'received', 'sendTime', 'readed'],
							sortInfo : {
									field : 'sendTime',
									direction : 'DESC'
								},
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
//									return com.bitc.xmis.workflow.TodoActions.getActions(value);
								}
							});
					this.columns.push(actioncol);
					this.plugins = actioncol;
				}
				com.bitc.xmis.workflow.MessageInfoList.superclass.initComponent.call(this);
			},
			onRender : function(a, b) {
				this.on('click', function(listview, index) {
							var data = this.getStore().getAt(index).data;
							var result = "发送人："+data.sender.userName+";<br>审批结果："+data.content+";<br>发送时间："+data.sendTime;
							Ext.Msg.alert("提示",result,function(){
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
//											Ext.Msg.alert(result.messageTitle,
//													result.message, function() {
//														parent.xMis.refresh('messageTodo');
//														parent.xMis.closeTab(window);
//													});
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
						}, this);
				com.bitc.xmis.workflow.MessageInfoList.superclass.onRender.call(this, a, b);
			},
			reload : function(options) {
				this.getStore().reload(options);
			}
		});