Ext.ns('Ext.ux');
/**
 * @class Ext.ux.InklingSearchField
 * @extends Ext.ux.form.SearchField
 *          通用多条件模糊查询组件。常用放置在Grid的TopToolbar中。需要后台数据来源配合使用。
 *          此组件会在查询的时候为Store增加两个baseParam:
 *          <div class="sub-desc"><ul>
 *          <li><b>inklings</b>:要查询的字段 </li>
 *          <li><b>inkling</b>:用户输入的条件</li>
 *          </ul></div>
 *          后台详见StandardQuery相关内容
 */
Ext.ux.InklingSearchField = Ext.extend(Ext.ux.form.SearchField, {
			/**
			 * @cfg {Ext.data.Store} store 查询关联的Store
			 * @requires
			 */
			/**
			 * @cfg {Object} tooltip Tooltip配置对象，包含两个属性：
			 *      <div class="sub-desc"><ul>
			 *      <li><b>title</b>:标题</li>
			 *      <li><b>text</b>:提示内容</li>
			 *      </ul>
			 *      </div>
			 */
			// private
			initComponent : function() {
				Ext.ux.InklingSearchField.superclass.initComponent.call(this);
				this.addEvents('beforesearch');
			},
			// private
			onRender : function(ct, position) {
				Ext.ux.InklingSearchField.superclass.onRender.call(this, ct,
						position);
				if (this.tooltip && this.tooltip.text)
					new Ext.ToolTip({
								target : this.id,
								trackMouse : false,
								draggable : false,
								maxWidth : 200,
								minWidth : 100,
								title : this.tooltip.title,
								html : this.tooltip.text
							});
			},
			width : 120,
			hasSearch : false,
			emptyText : '请输入关键词',
			/**
			 * @cfg {String} paramName 查询值的参数名，默认为inkling，请不要随意更改此配置
			 */
			paramName : 'inkling',
			/**
			 * @cfg {Array} inklings
			 *      要进行查询匹配的列（属性）。此参数为一个数组，其中的元素为Object，每个元素应该包含以下参数： 
			 *      <div class="sub-desc">
			 *      <ul>
			 *      <li><b>name</b>：{String}
			 *      列名或者属性名。后台为JDBC查询时使用列名，为Hibernate查询时使用属性名</li>
			 *      <li><b>op</b>：{String}
			 *      匹配时使用的运算符。可选：'=','&gt;','&lt;','&gt;=','&lt;=','&gt;&lt;','like'</li>
			 *      <li><b>type</b>：{String}
			 *      该字段或属性的类型。可选：'string','integer','float','double','date','datetime','boolean'</li>
			 *      </ul>
			 *      </div>
			 * @requires
			 */
			inklings : [],
			onTrigger2Click : function() {
				var params = '';
				Ext.iterate(this.inklings, function(item, index, allItems) {
							params += item.name + ',' + item.op + ','
									+ item.type;
							if (index < allItems.length - 1) {
								params += '|';
							}
						}, this);
				this.store.baseParams = this.store.baseParams || {};
				this.store.baseParams['inklings'] = params;
				this.fireEvent('beforesearch', this, this.store);
				Ext.ux.InklingSearchField.superclass.onTrigger2Click.call(this);
			},
			onTrigger1Click : function() {
				this.store.baseParams = this.store.baseParams || {};
				delete this.store.baseParams['inklings'];
				Ext.ux.InklingSearchField.superclass.onTrigger1Click.call(this);
			}
		});