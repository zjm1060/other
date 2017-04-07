Ext.ns('com.bitc.xmis.sys');
/**
 * 草稿功能的Record
 */
com.bitc.xmis.sys.DraftRecord = Ext.data.Record.create(['id', 'model', 'appId',
		'userID', 'text', 'url', {
			name : 'lastModified',
			type : 'date',
			dateFormat : 'Y-m-d H:i:s'
		}]);
/**
 * @class com.bitc.xmis.sys.Draft 草稿
 * @author bomwu
 * @version 1.0
 */
com.bitc.xmis.sys.Draft = function(config) {
	/**
	 * @cfg {String} id ID
	 */
	/**
	 * @cfg {String} model 模块
	 */
	/**
	 * @cfg {String} appId 应用ID
	 */
	/**
	 * @cfg {String} text 标题
	 */
	/**
	 * @cfg {String} url URI
	 */
	/**
	 * @cfg {String} userID 用户ID
	 */
	/**
	 * @cfg {String} content 草稿内容
	 */
	Ext.apply(this, config);
};
/**
 * 保存草稿
 * 
 * @param {Function}
 *            fn 回调函数。参数： <div class="mdetail-params">
 *            <ul>
 *            <li><code>draft</code> : Draft<div class="sub-desc">This
 *            Draft.</div></li>
 *            <li><code>result</code> : Object<div>
 *            class="sub-desc">Ext.decode(response.responseText)</div></li>
 *            </ul>
 *            </div>
 * @param {Object}
 *            scope the scope of fn
 * @param {Object|String}
 *            content 要保存的内容，字符串或者Object
 */
com.bitc.xmis.sys.Draft.prototype.save = function(fn, scope, content) {
	Ext.Ajax.request({
				url : String.format('{0}/sys.do?method=saveDraft', __cxtPath),
				params : this.buildParams(content),
				scope : this,
				success : function(response, opt) {
					var result = Ext.decode(response.responseText);
					if (result.success) {
						this.content = content;
						this.id = result.attributes.draftId;
						fn.call(scope || window, this, result);
					} else {
						Ext.Msg.alert(result.messageTitle, result.message);
					}
				},
				failure : Ext.emptyFn
			});
};
/**
 * 加载草稿
 * 
 * @param {Function}
 *            fn 回调函数，有一个返回参数，this draft
 * @param {Object}
 *            scope the scope of fn
 */
com.bitc.xmis.sys.Draft.prototype.load = function(fn, scope) {
	Ext.Ajax.request({
				url : String.format('{0}/sys.do?method=getDraft', __cxtPath),
				scope : this,
				params : this.buildParams(),
				success : function(response, opt) {
					var result = Ext.decode(response.responseText);
					if (result.success) {
						Ext.apply(this, result.attributes.draft);
						this.content = this.content
								? Ext.decode(this.content)
								: {};
						fn.call(scope || window, this, result);
					} else {
						Ext.Msg.alert(result.messageTitle, result.message);
					}
				},
				failure : Ext.emptyFn
			});
};
/**
 * 生成参数
 * 
 * @param {Object|String}
 *            content
 * @return {Object} 参数
 */
com.bitc.xmis.sys.Draft.prototype.buildParams = function(content) {
	var c = content || this.content;
	if (c && !Ext.isString(c)) {
		c = Ext.encode(c);
	}
	return {
		id : this.id,
		model : this.model,
		appId : this.appId,
		text : this.text,
		url : this.url,
		userID : this.userID,
		content : c
	}
};
/**
 * 删除此条草稿
 * 
 * @param {Function}
 *            fn 回调函数。参数：
 *            <ul>
 *            <li><b>draft</b>:com.bitc.xmis.sys.Draft 此条草稿</li>
 *            <li><b>result</b>:Object
 *            服务器的响应，由Ext.decode(response.responseText)得来</li>
 *            </ul>
 * @param {Object}
 *            scope 回调函数的执行域
 */
com.bitc.xmis.sys.Draft.prototype.remove = function(fn, scope) {
	if (Ext.isEmpty(this.id)) {
		return;
	}
	Ext.Ajax.request({
				url : String.format('{0}/sys.do?method=delDraft', __cxtPath),
				scope : this,
				params : this.buildParams(),
				success : function(response, opt) {
					var result = Ext.decode(response.responseText);
					if (result.success) {
						if (fn) {
							fn.call(scope || window, this, result);
						}
					} else {
						Ext.Msg.alert(result.messageTitle, result.message);
					}
				},
				failure : Ext.emptyFn
			});
};
/**
 * @class com.bitc.xmis.sys.DraftList
 * @extends Ext.list.ListView 个人草稿列表
 */
com.bitc.xmis.sys.DraftList = Ext.extend(Ext.list.ListView, {
			/**
			 * @cfg {Number} listSize 列表显示的条数，如果要显示所有，设置为0，默认10
			 */
			listSize : 10,
			hideHeaders : true,
			emptyText : '草稿箱里什么也没有',
			initComponent : function() {
				this.store = new Ext.data.Store({
							url : String.format('{0}/sys.do?method=getDrafts',
									__cxtPath),
							baseParams : {
								userID : __emp.userID,
								start : 0,
								limit : this.listSize
							},
							reader : new Ext.data.JsonReader({
										root : 'rows'
									}, com.bitc.xmis.sys.DraftRecord),
							sortInfo : {
								field : 'lastModified',
								direction : 'ASC'
							},
							autoLoad : true
						});
				var actions = [{
					cls : 'delete',
					scope : this,
					handler : function(listview, record, value) {
						Ext.Msg.confirm('确认', '您确定要删除这条记录吗？删除后将无法恢复。',
								function(buttonId) {
									if (buttonId == 'yes') {
										var draft = new com.bitc.xmis.sys.Draft();
										draft.id = record.get('id');
										draft.remove(function() {
													this.reload();
												}, this);
									}
								}, this);
					},
					qtip : '删除草稿'
				}];
				var actioncol = new Ext.ux.list.ActionColumn({
							width : .2,
							header : '日期',
							dataIndex : 'lastModified',
							tpl : '{[fm.date(values.lastModified,"m-d")]}',
							getActionsFun : function() {
								return actions;
							}
						});
				this.columns = [{
							header : '标题',
							dataIndex : 'text'
						}, actioncol];
				this.plugins = actioncol;
				com.bitc.xmis.sys.DraftList.superclass.initComponent.call(this);
			},
			// private
			onRender : function(a, b) {
				this.on('click', function(listview, index) {
							var r = this.getStore().getAt(index);
							var url = r.get('url');
							url = url + (url.indexOf('?') == -1 ? +'?' : '&');
							url = url + 'draftId=' + r.get('id');
							xMis.turn(r.get('model') + '-' + r.get('appId'), r
											.get('text'), url);
						}, this);
				com.bitc.xmis.sys.DraftList.superclass.onRender
						.call(this, a, b);
			},
			/**
			 * 重新加载列表
			 * 
			 * @param {Object}
			 *            options 加载时使用的参数
			 */
			reload : function(options) {
				this.getStore().reload(options);
			},
			/**
			 * 加载所有的草稿
			 */
			loadAll : function() {
				this.getStore().reload({
							limit : 0
						});
			}
		});