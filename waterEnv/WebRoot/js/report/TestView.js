/**
 * Created by BomWu on 2015/11/24 0024.
 */
Ext.define('bitc.report.TestView', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView', {
            reportToken: 'repTest',
            reportCondition: null,
            //parameterValues:Ext.encode(['GGG']),
            listeners: {
                gridready: this.init,
                scope: this
            },
            region: 'center'
        });
        this.items = [gridview];
        this.callParent(arguments);
    },
    init: function (panel, grid) {
        var form = Ext.create('bitc.report.FormView', {
            formToken: 'test',
            listeners: {
                savesuccess: function () {
                    newWin.hide();
                    grid.getStore().reload();
                }
            }
        });
        var newWin = new Ext.Window({
            layout: 'fit',
            width: 500,
            height: 500,
            items: [form],
            closeAction: 'hide'
        });
        grid.down('toolbar').insert(0, [{
            text: '添加',
            handler: function () {
                newWin.show();
            }
        }, {
            xtype: 'deletebutton',
            beforeDelete: function () {
                return grid.getSelectionModel().getCount() > 0;
            },
            deleteSuccess: function () {
                grid.getStore().reload();
            },
            getDeleteRows: function () {
                var record = grid.getSelectionModel().getLastSelected();
                return {
                    dbName: '_self',
                    formFields: [{
                        tableName: 'rep_test',
                        rowKey: 'id',
                        rowKeyValue: record.get('ID')
                    }]
                };
            }
        }]);
    }
});
