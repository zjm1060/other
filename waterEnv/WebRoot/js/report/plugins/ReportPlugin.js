Ext.define('bitc.report.plugins.ReportPlugin', {
    init: function (grid) {
        grid.on('itemdblclick', this.itemdblclick);
        var newWin = this.newWin = new Ext.Window({
            width: 500,
            height: 400,
            modal: true,
            title: '添加报表',
            layout: 'fit',
            items: [
                Ext.create('bitc.report.ReportEditFormPanel', {
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
        var newReportWin = this.newReportWin = new Ext.Window({
            width: 800,
            height: 600,
            modal: true,
            title: '添加新报表',
            layout: 'fit',
            items: [
                Ext.create('bitc.report.NewReportEditPanel', {
                    listeners: {
                        savesuccess: function () {
                            newReportWin.hide();
                            grid.getStore().reload();
                        }
                    }
                })
            ],
            closeAction: 'hide'
        });
        var buttons = [{
            text: '添加报表',
            handler: function () {
                newWin.show();
            }
        }, {
            text: '添加新报表',
            handler: function () {
                newReportWin.show();
            }
        },{
            text: '删除报表',
            handler: function () {
                debugger;
                var record = grid.getSelectionModel().getLastSelected();
                if (!record) {
                    return;
                }
                Ext.Msg.confirm('提示', '确定要删除该报表吗？', function (btn) {
                    if (btn != 'yes') {
                        return;
                    }
                    Ext.Ajax.request({
                        url: __path + '/report/delete.do',
                        params: {
                            reportId: record.get("ID")
                        },
                        success: function () {
                            grid.getStore().reload();
                        }
                    });
                });
            }
        }];
        if (grid.down('toolbar') == null) {
            grid.addDocked({
                xtype: 'toolbar',
                dock: 'top',
                items: []
            });
        }
        grid.down('toolbar').insert(0,buttons);

    },
    itemdblclick: function (view, record) {
        var data = {
            id: 'editReport-' + record.get("ID"),
            text: '报表修改-' + record.get('TITLE'),
            action: 'bitc.report.ReportManage',
            config: {
                reportId: record.get("ID"),
                reportToken: record.get("NAME")
            }
        };
        addWorkTab(data);
    },
    beforeDestroy: function () {
        Ext.destroy(this.newWin);
        this.callParent(arguments);
    }
});