Ext.define('bitc.report.plugins.ColumnPlugin', {
    init: function (grid) {
        grid.on('itemdblclick', function (view, record) {
            var pk = record.get('ID');
            var fieldValues = [];
            Ext.each(['ID', 'DNAME', 'HIDDEN', 'NAME', 'REPORTID', 'TYPE', 'SORT', 'CONFIG', 'EDITOR'], function (item) {
                fieldValues.push({
                    rowKeyValue: pk,
                    name: item,
                    value: record.get(item)
                });
            });
            var data = {
                id: 'browseFormColumn' + record.get("ID"),
                text: '修改列配置-'+record.get('DNAME'),
                action: 'bitc.report.FormView',
                config: {
                    formToken: 'reportColumn',
                    fieldValues: fieldValues
                }
            };
            addWorkTab(data);
        });
    }
});