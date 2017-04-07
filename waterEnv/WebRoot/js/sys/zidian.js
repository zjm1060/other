Ext.define('bitc.wrenv.sys.zidian', {
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping'],
    layout : 'border',
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView', {
            reportToken: 'zidian',
            rowNumberShow:false,
            defaultGridWidth : false,
            region: 'center',
            gridConfig: {
                groupField: '字典名',
                features: [{
                    ftype: 'grouping',
                    groupHeaderTpl: ['{rows:this.getName}', {
                        getName: function (rows) {
                            return rows[0].get('字典名');
                        }
                    }],
                    hideGroupedHeader: true,
                    startCollapsed: true
                }]
            }
        });
        this.items = [gridview];
        this.callParent(arguments);
    }
});