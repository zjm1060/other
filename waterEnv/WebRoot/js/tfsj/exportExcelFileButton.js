/**
 * Created by Administrator on 2016/9/19.
 */
Ext.define('bitc.wrenv.tfsj.exportExcelFileButton', {
    alias: 'widget.exportExcel',
    extend : 'Ext.Button',
    iconCls: 'xls',
    text: this.text || '导出数据',
    initComponent: function () {
        this.menu = [{
            text: '导出本页数据',
            handler: function () {
                this.onExport(true);
            },
            scope: this
        }, {
            text: '导出所有数据',
            handler: function () {
                this.onExport(false);
            },
            scope: this
        }];
        this.callParent(arguments);
    },
    onExport: function (currentPage) {
        var grid = this.up('gridpanel');
        Ext.MessageBox.show({
            title: '请稍候',
            msg: '正在导出...',
            width: 300,
            wait: true,
            waitConfig: {
                interval: 200
            },
            closable: false
        });
        var store = grid.store;
        var storeUrl = store.proxy.url;
        var url = __path + storeUrl;
        var columns = grid.columns;
        debugger;
        var cms = new Array();
        var fields = {
            items: store.model.getFields(),
            get: function (name) {
                for (var i = 0; i < this.items.length; i++) {
                    var item = this.items[i];
                    if (item.name == name) {
                        return item;
                    }
                }
                return null;
            }
        };
        for (var i = 0; i < columns.length; i++) {
            var col = columns[i];
            if (col.hidden === true || col.xtype == 'rownumberer') {// 列是否可见
                continue;
            }
            if (col.dataIndex || col.dataIndex == '') {
                var c = {
                    dataIndex: col.dataIndex
                };
                cms.push(c);
            }
        }
        var params = {
            notreportexporttoexcelrequest : true,
            exp_name: this.exportName || (new Date().getTime()),
            start: (store.currentPage-1)*store.pageSize,
            limit: store.pageSize,
            exp_cms: Ext.encode(cms)
        };
        if (store.proxy.extraParams) {
            Ext.apply(params, store.proxy.extraParams);
        }
        if (!currentPage) {
            params['start'] = 0;
            params['limit'] = 0;
        }
        // Create form panel. It contains a basic form that we need for the file download.
        var form = Ext.create('Ext.form.Panel', {
            standardSubmit: true,
            url: url,
            method: 'post'
        });
        // Call the submit to begin the file download.
        form.submit({
            target: '_blank', // Avoids leaving the page.
            params: params
        });
        Ext.defer(function () {
            Ext.destroy(form);
            form = null;
            Ext.Msg.hide();
        }, 1000);
    }
});
