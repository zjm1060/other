Ext.define('bitc.report.plugins.ReportManagePlugin',{
    init: function (grid) {
        grid.on('itemdblclick', this.itemdblclick);

    },
    itemdblclick: function (view, record) {
        var pk = record.get('ID');
        var fieldValues = [];
        Ext.each(['ID','NAME','DBNAME','SQL','TITLE','PLUGINS'],function(item){
            fieldValues.push({
                rowKeyValue: pk,
                name: item,
                value:record.get(item)
            });
        });

        var data = {
            id: 'browseRepColumn' + record.get("ID"),
            text: record.get('TITLE') + '-报表管理',
            action: 'bitc.report.ReportManage',
            config: {
                reportToken: 'repColumn',
                reportCondition: "reportid='" + record.get("ID") + "'",
                fieldValues:fieldValues,
                formToken: 'rep_report_form'
            }
        };
        addWorkTab(data);
    }
});