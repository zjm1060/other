Ext.define('bitc.report.ReportFormEditPanel', {
    extend: 'Ext.form.Panel',
    url: __path + '/form/new.do',
    frame: true,
    width: '400',
    defaultType: 'textfield',
    padding: 20,
    initComponent: function () {
        this.items = [new Ext.form.ComboBox({
            allowBlank: false,
            name: 'dbName',
            fieldLabel: '数据库',
            displayField: 'dname',
            valueField: 'name',
            triggerAction: 'all',
            emptyText: '选择已注册的数据库',
            editable: false,
            store: new Ext.data.JsonStore({
                fields: ['name', 'dname'],
                proxy: {
                    type: 'ajax',
                    url: __path + '/report/dbs.do'
                },
                autoLoad: true
            })
        }), {
            fieldLabel: '表单标识',
            name: 'name',
            emptyText: '输入报表的唯一标识',
            allowBlank: false
        }, {
            fieldLabel: '表单名称',
            allowBlank: false,
            name: 'title'
        }, {
            fieldLabel: '数据表名称',
            name: 'tableName',
            allowBlank: false
        }, {
            fieldLabel: '记录主键',
            name: 'rowKey',
            emptyText: '输入数据的主键',
            allowBlank: false
        }];
        this.buttons = [{
            text: '提交',
            handler: function () {
                this.getForm().submit({
                    waitMsg: '正在处理……',
                    waitTitle: '请稍候',
                    success: function (form, action) {
                        Ext.Msg.alert('提示', '保存成功');
                        this.fireEvent('savesuccess');
                    },
                    failure: function (form, action) {
                        //Ext.Msg.alert('提示','保存失败');
                    }
                });
            },
            scope: this
        }];
        this.addEvents('savesuccess');
        this.callParent(arguments);
    }
});