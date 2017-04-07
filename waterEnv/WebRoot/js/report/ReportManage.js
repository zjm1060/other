Ext.define('bitc.report.ReportManage', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    reportId: null,
    reportToken: null,
    initComponent: function () {
        var reportForm = this.reportForm = Ext.create('bitc.report.ReportEditFormPanel', {
            title: '报表',
            region: 'north'
        });
        var grid = this.grid = Ext.create('bitc.report.ReportView', {
            title: '报表列',
            region: 'center',
            reportToken: 'repColumn',
            reportCondition: Ext.String.format("reportid='{0}'", this.reportId)
        });
        reportForm.load({
            url: __path + '/report/load.do',
            params: {
                reportId: this.reportId
            },
            success: function (a, b) {
                reportForm.getForm().setValues({
                    isPaging: b.result.data.isPaging ? 'true' : 'false'
                })
            }
        });
        this.items = [reportForm, grid];
        this.callParent(arguments);
    }
});