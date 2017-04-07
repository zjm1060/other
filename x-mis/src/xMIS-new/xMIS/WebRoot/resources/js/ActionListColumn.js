Ext.ns('Ext.ux.list');
/**
 * 
 * @class Ext.ux.list.ActionColumn
 * @extends Ext.list.Column 动作列
 * @xtype lvactioncolumn
 * @cfg {Function} getActionsFun 该函数应该返回一个数组包含要显示的Action。 该函数的参数：
 *      <ul>
 *      <li><b>data</b>: Object record.data </li>
 *      </ul>
 *      <div class="sub-desc">数组中Action对象的配置：
 *      <ul>
 *      <li><b>cls</b>:String 附加的class，一般为其设置图标</li>
 *      <li><b>qtip</b>:String 提示信息</li>
 *      <li><b>handler</b>:Funciton 点击此Action触发的动作，此函数有三个参数：
 *      <ul>
 *      <li><b>listview</b>:ListView 此ListView</li>
 *      <li><b>record</b>:Record 触发动作的当前记录，即ListView Store中的Record </li>
 *      <li><b>value</b>:Object 当前列dataIndex对应的数据 </li>
 *      </ul>
 *      </li>
 *      </ul>
 *      </div> <br/>例：
 * 
 * <pre><code>
 *  
 *      function(data){
 *      	return [{
 *      		cls:'',
 *      		handler:function(listview, record,value){
 *      		},
 *      		scope:this,
 *      		qtip:''
 *      	}]
 *     	}
 * </code></pre>
 * 
 * @cfg {Object} scope the scope of getActionsFun
 * @constructor
 * @param {Object}
 *            config The config object
 */
Ext.ux.list.ActionColumn = Ext.extend(Ext.list.Column, {
	/**
	 * @cfg {Number} actionWidth 每个Action的宽度(default 20)
	 */
	actionWidth : 20,
	constructor : function(c) {
		var tpl = '<div class="ux-cell-value">'
				+ '<tpl if="\'left\'!==align">{value}</tpl>'
				+ '<div class="ux-cell-actions<tpl if="\'left\'===align"> ux-cell-actions-left</tpl>" style="width:{width}px">'
				+ '{actions}' + '</div>'
				+ '<tpl if="\'left\'===align">{value}</tpl>' + '</div>';
		var data = {
			align : c.align || 'right',
			value : c.tpl ? c.tpl : '{' + c.dataIndex + '}',
			width : '{[this.getWidth(values)]}',
			actions : '{[this.getActions(values)]}'
		};
		var xt = new Ext.XTemplate(tpl);
		var column = this;
		var getActionsFun = c.getActionsFun;
		var scope = c.scope;
		c.tpl = new Ext.XTemplate(xt.apply(data), {
			getWidth : function(values) {
				var actions = getActionsFun.call(scope || window, values);
				if (actions) {
					return column.actionWidth * actions.length;
				}
				return 0;
			},
			getActions : function(values) {
				var t = new Ext.XTemplate(
						'<tpl for="actions">',
						'<div class="ux-cell-action {cls}" ext:qtip="{qtip}" style="{style}">&#160;</div>',
						'</tpl>');
				var actions = getActionsFun.call(scope || window, values);
				if (actions && actions.length > 0) {
					return t.apply({
								'actions' : actions
							});
				} else {
					return '';
				}
			}
		});
		Ext.ux.list.ActionColumn.superclass.constructor.call(this, c);
	},
	init : function(listview) {
		this.listview = listview;
		listview.on('beforeclick', this.onClick, this);

	},
	onClick : function(lv, idx, node, e) {
		var t = e.getTarget('div.ux-cell-action');
		var record, value, action;
		if (t) {
			record = this.listview.getStore().getAt(idx);
			dataIndex = this.dataIndex;
			value = record.get(dataIndex);
			action = t.className.replace(/ux-cell-action /, '');
		}

		// check if we've collected all necessary variables
		if (record && action) {
			var actions = this.getActionsFun.call(this.scope || window,
					record.data);
			Ext.iterate(actions, function(act) {
						if (act.cls == action) {
							if (act.handler) {
								act.handler.call(act.scope || window,
										this.listview, record, value);
							}
							return false;// end the loop
						}
					}, this);
			return false;// end the event
		}

	}
});
Ext.reg('lvactioncolumn', Ext.ux.list.ActionColumn);
