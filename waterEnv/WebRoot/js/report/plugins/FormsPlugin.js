Ext.define('bitc.report.plugins.FormsPlugin', {
    init: function (grid) {
        var newWin = this.newWin = new Ext.Window({
            width: 500,
            height: 400,
            modal: true,
            title: '添加报表',
            layout: 'fit',
            items: [
                Ext.create('bitc.report.ReportFormEditPanel', {
                    listeners: {
                        savesuccess: function () {
                            newWin.hide();
                            grid.getStore().reload();
                        }
                    }
                })
            ],
            closeAction: 'hide'
        });
        grid.on('itemdblclick', this.itemdblclick);
        grid.down('toolbar').insert(0, [{
            text: '新建',
            handler: function () {
                newWin.show();
            }
        }]);
    },
    itemdblclick: function (view, record) {
        var fieldValues = [];
        var pk = record.get('ID');
        Ext.each(['ID', 'NAME', 'CONFIG', 'DBNAME', 'TITLE','LISTENER'], function (item) {
            debugger;
            fieldValues.push({
                rowKeyValue: pk,
                name: item,
                value: record.get(item)
            });
        });
        var data = {
            id: 'formManage' + record.get("ID"),
            text: '修改表单-' + record.get('TITLE'),
            action: 'bitc.report.FormManage',
            config: {
                formId: record.get("ID"),
                formToken: record.get("NAME"),
                formValues: fieldValues
            }
        };
        addWorkTab(data);
    }
});