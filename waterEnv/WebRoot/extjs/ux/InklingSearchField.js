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
Ext.define('Ext.ux.InklingSearchField', {
    extend: 'Ext.ux.form.SearchField',
    alias: 'widget.searchfield',
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
    initComponent: function () {
        this.callParent(arguments);
    },

    // private
    onRender: function (ct, position) {
        this.callParent(arguments);
        if (this.tooltip && this.tooltip.text)
            new Ext.ToolTip({
                target: this.id,
                trackMouse: false,
                draggable: false,
                maxWidth: 200,
                minWidth: 100,
                title: this.tooltip.title,
                html: this.tooltip.text
            });
    },
    width: 120,
    hasSearch: false,
    emptyText: '请输入关键词',
    inklings: [],
    onTrigger2Click: function () {
        var value = this.getValue();
        var params = ' (';

        Ext.iterate(this.inklings, function (item, index, allItems) {
            var param = '';
            if (item.op == 'like') {
                param = " '%" + value + "%'";
            } else {
                param = value;
            }
            params += item.name + ' ' + item.op + ' ' + param;
            if (index < allItems.length - 1) {
                params += ' or ';
            }
        }, this);
        params = params+') ';
        this.reportView.reload(params , true);
        this.triggerCell.item(0).setDisplayed(true);
    },
    onTrigger1Click: function () {
        this.setValue('');
        this.reportView.reload();
        this.triggerCell.item(0).setDisplayed(false);
    }
});