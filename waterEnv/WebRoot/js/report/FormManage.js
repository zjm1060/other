Ext.define('bitc.report.FormManage', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    formId: null,
    formToken: null,
    formValues: null,
    initComponent: function () {
        var form = this.form = Ext.create('bitc.report.FormView', {
            region: 'north',
            formToken: 'editForm',
            fieldValues: this.formValues
        });
        var me = this;
        var fieldGrid = this.fieldGrid = Ext.create('bitc.report.ReportView', {
            title: '字段明细',
            region: 'center',
            reportToken: 'repField',
            reportCondition: Ext.String.format("formid='{0}'", this.formId),
            listeners: {
                'gridready': function (view, grid) {
                    me.initFormEvents(grid);
                }
            }
        });
        this.items = [fieldGrid, form];
        this.callParent(arguments);
    },
    initFormEvents: function (grid) {

        var form = Ext.create('bitc.report.FormView', {
            formToken: 'formField',
            listeners: {
                savesuccess: function () {
                    newWin.hide();
                    grid.getStore().reload();
                }
            }
        });
        var newWin = new Ext.Window({
            title: '添加字段',
            layout: 'fit',
            width: 500,
            height: 600,
            items: [form],
            closeAction: 'hide'
        });
        grid.on('itemdblclick', function (view, record) {
            var pk = record.get('ID');
            var fieldValues = [];
            Ext.each(['ID', 'CONFIG', 'FIELDLABEL', 'FORMID', 'NAME', 'ROWKEY', 'SORT', 'TABLENAME', 'XTYPE',
                'SQLTYPE', 'INSERTVALUEGENERATOR', 'UPDATEVALUEGENERATOR'], function (item) {
                fieldValues.push({
                    rowKeyValue: pk,
                    name: item,
                    value: record.get(item)
                });
            });
            newWin.setTitle('修改字段');
            newWin.show();
            form.setValues(fieldValues);
        });
        grid.down('toolbar').insert(0, [{
            text: '添加字段',
            scope: this,
            handler: function () {
                var fieldValues = [];
                Ext.each(['ID', 'CONFIG', 'FIELDLABEL', 'NAME', 'ROWKEY', 'SORT', 'TABLENAME', 'XTYPE',
                    'SQLTYPE', 'INSERTVALUEGENERATOR', 'UPDATEVALUEGENERATOR'], function (item) {
                    fieldValues.push({
                        rowKeyValue: null,
                        name: item,
                        value: null
                    });
                });
                fieldValues.push({
                    rowKeyValue: null,
                    name: 'FORMID',
                    value: this.formId
                });
                newWin.setTitle('添加字段');
                newWin.show();
                form.setValues(fieldValues);
            }
        }, {
            xtype: 'button',
            text: '复制',
            handler: function () {
                if (grid.getSelectionModel().getCount() < 1) {
                    return;
                }
                var record = grid.getSelectionModel().getLastSelected();
                var fieldValues = [];
                Ext.each(['ID', 'CONFIG', 'FIELDLABEL', 'FORMID', 'NAME', 'ROWKEY', 'SORT', 'TABLENAME', 'XTYPE',
                    'SQLTYPE', 'INSERTVALUEGENERATOR', 'UPDATEVALUEGENERATOR'], function (item) {
                    fieldValues.push({
                        rowKeyValue: null,
                        name: item,
                        value: record.get(item)
                    });
                });
                newWin.setTitle('复制字段');
                newWin.show();
                form.setValues(fieldValues);
            }
        }, {
            xtype: 'deletebutton',
            beforeDelete: function () {
                return grid.getSelectionModel().getCount() > 0;
            },
            getDeleteRows: function () {
                var record = grid.getSelectionModel().getLastSelected();
                return {
                    dbName: '_self',
                    formFields: [{
                        tableName: 'rep_field',
                        rowKey: 'id',
                        rowKeyValue: record.get('ID')
                    }]
                };
            }
        }]);
    }
});