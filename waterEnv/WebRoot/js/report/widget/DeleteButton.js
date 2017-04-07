/**
 * Created by BomWu on 2015/11/24 0024.
 */
Ext.define('bitc.report.widget.DeleteButton', {
    alias: 'widget.deletebutton',
    extend: 'Ext.button.Button',
    text: '删除',
    initComponent: function () {
        var me=this;
        this.handler = function () {
            Ext.Msg.confirm('提示', '确认要删除吗？', function (btn) {
                if (btn == 'yes') {
                    me.deleteIt();
                }
            });
        };
        this.callParent(arguments);
    },
    /**
     * @cfg {Function} getDeleteRows
     * 该方法用于回调，返回需要删除的行
     * 比如：
     *  <code><pre>
     *      {
     *          dbName:'_self',
     *          formFields:[{
     *              table:'A',
     *              rowKey:'a',
     *              rowKeyValue:'1'
     *          }]
     *      }
     *  </pre></code>
     */
    getDeleteRows: Ext.emptyFn,
    /**
     * @cfg {Function} deleteSuccess
     * 删除成功后的回调函数
     */
    deleteSuccess: Ext.emptyFn,
    /**
     * @cfg {Function} beforeDelete
     * @return 回返false，阻止删除
     */
    beforeDelete: Ext.emptyFn,
    deleteIt: function () {
        if (!this.beforeDelete()) {
            return;
        }
        Ext.Ajax.request({
            url: __path + '/form/delete.do',
            method: 'POST',
            jsonData: this.getDeleteRows(),
            success: function (response) {
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    Ext.Msg.alert("提示", "删除成功！");
                    this.deleteSuccess.call(this.scope)
                } else {
                    Ext.Msg.alert("提示", "删除失败！");
                }
            },
            scope: this
        });
    }
});