Ext.ns('com.bitc.xmis.workflow');
/**
 * @class com.bitc.xmis.workflow.Attention
 * @extends Ext.util.Observable 关注
 * @author bomwu
 * @version 1.0
 */
com.bitc.xmis.workflow.Attention = function(config) {
	Ext.apply(this, config);
};
Ext.extend(com.bitc.xmis.workflow.Attention, Ext.util.Observable, {
			/**
			 * @cfg {String} id 关注ID
			 */
			/**
			 * @cfg {String} model 关注的模块
			 */
			/**
			 * @cfg {String} appId 关注的应用ID
			 */
			/**
			 * @cfg {String} userID 此关注的用户ID
			 */
			/**
			 * @cfg {String} url 关注的URL
			 */
			/**
			 * @cfg {String} text 关注的标题
			 */
			/**
			 * @cfg {String} addText 添加关注按钮的text
			 */
			/**
			 * @cfg {String} cancelText 取消关注按钮的text
			 */
			/**
			 * @property isAttention 当前是否已经关注。只读属性
			 * @type Boolean
			 */
			isAttention : false,
			/**
			 * 向指定的工具栏添加按钮
			 * 
			 * @param {Ext.Toolbar}
			 *            tbar
			 */
			addButtonToBar : function(tbar) {
				var button = this.button = tbar.add({
							text : '',
							iconCls : 'star',
							hidden : true,
							handler : this.payAttention.createDelegate(this,[]),
							tooltip : '添加到首页的关注列表中',
							scope : this
						});
				Ext.Ajax.request({
							url : String.format(
									'{0}/workflow.do?method=getAttention',
									__cxtPath),
							scope : this,
							params : {
								id : this.id,
								model : this.model,
								appId : this.appId,
								userID : this.userID
							},
							success : function(response, opts) {
								var result = Ext.decode(response.responseText);
								var attention = result.attributes.attention;
								if (Ext.isEmpty(attention.id)) {
									this.isAttention = false;
								} else {
									this.isAttention = true;
									this.id = attention.id;
								}
								this.setButtonText();
								button.show();
							},
							failure : function() {
							}
						});
			},
			// private
			setButtonText : function() {
				if (this.button) {
					this.button.setText(this.isAttention ? this.cancelText
							|| '取消关注' : this.addText || '添加关注');
				}
			},
			/**
			 * 向服务器发送关注请求。如果已经关注则取消关注，如果未关注则添加关注
			 * 
			 * @param {Function}
			 *            callback 回调函数，参数：
			 *            <ul>
			 *            <li><b>result</b>: Ext.decode(response.responseText)</li>
			 *            </ul>
			 * @param {Object}
			 *            scope
			 */
			payAttention : function(callback, scope) {
				Ext.Ajax.request({
							url : String.format(
									'{0}/workflow.do?method=payAttention',
									__cxtPath),
							params : {
								id : this.id,
								model : this.model,
								appId : this.appId,
								userID : this.userID,
								url : this.url,
								text : this.text,
								isAttention : this.isAttention
							},
							scope : this,
							success : function(response, opts) {
								var result = Ext.decode(response.responseText);
								if (result.success) {
									if (this.isAttention) {
										this.isAttention = false;
									} else {
										var attention = result.attributes.attention;
										this.isAttention = true;
										this.id = attention.id;
									}
								}
								Ext.Msg.alert(result.messageTitle,
										result.message);
								this.setButtonText();
								if (window.parent.xMis
										&& window.parent.xMis.refresh) {
									window.parent.xMis.refresh('myattention');
								}
								if (callback) {
									callback.call(scope || window, result);
								}
							},
							failure : function() {
							}
						});
			}
		});
/**
 * 
 * 
 * @class com.bitc.xmis.workflow.AttentionList
 * @extends Ext.list.ListView 用户关注列表
 * @author bomwu
 * @version 1.0
 */
com.bitc.xmis.workflow.AttentionList = Ext.extend(Ext.list.ListView, {
	/**
	 * @cfg {Number} 列表显示的条数，如果要显示所有，设置为0，默认10
	 */
	listSize : 10,
	hideHeaders : true,
	emptyText : '您还未添加任何关注',
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
					url : String.format(
							'{0}/workflow.do?method=getAttentionList',
							__cxtPath),
					root : 'rows',
					baseParams : {
						start : 0,
						limit : this.listSize
					},
					fields : ['id', 'model', 'appId', 'text', 'url',
							'createTime', 'userID'],
					autoLoad : true
				});
		var actions = [{
			cls : 'delete',
			scope : this,
			handler : function(listview, record, value) {
				Ext.Msg.confirm('确认', '您确定要取消关注？', function(buttonId) {
							if (buttonId == 'yes') {
								var attention = new com.bitc.xmis.workflow.Attention();
								Ext.apply(attention, record.data);
								attention.isAttention = true;
								attention.payAttention(function() {
											this.reload();
										}, this);
							}
						}, this);
			},
			qtip : '取消关注'
		}];
		var actioncol = new Ext.ux.list.ActionColumn({
					width : .2,
					header : '日期',
					dataIndex : 'createTime',
					tpl : '&nbsp;',
					getActionsFun : function() {
						return actions;
					}
				});
		this.columns = [{
					header : '标题',
					dataIndex : 'text'
				}, actioncol];
		this.plugins = actioncol;
		com.bitc.xmis.workflow.AttentionList.superclass.initComponent
				.call(this);
	},
	onRender : function(a, b) {
		this.on('click', function(listview, index) {
					var r = this.getStore().getAt(index);
					xMis.turn(r.get('model') + '-' + r.get('appId'), r
									.get('text'), r.get('url'));
				}, this);
		com.bitc.xmis.workflow.AttentionList.superclass.onRender.call(this, a,
				b);
	},
	/**
	 * 重新加载列表
	 * 
	 * @param {Object}
	 *            options 加载时使用的参数
	 */
	reload : function(options) {
		this.getStore().reload(options);
	}
});