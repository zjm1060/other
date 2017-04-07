Ext.define('bitc.report.FormView', {
    extend: 'Ext.form.Panel',
    requires: ['bitc.report.widget.CombWidget', 'bitc.report.widget.DeleteButton'],
    region: 'center',
    autoScroll: true,
    formToken: null,
    formTopHidden :true,
    fieldConfig: {},
    fieldValues: [],
    fieldDefaults: {
        labelAlign: 'left',
        labelWidth: 120,
        msgTarget: 'side'
    },
    initComponent: function () {
        Ext.Ajax.request({
            url: __path + '/form/meta.do',
            params: {
                formToken: this.formToken
            },
            method: 'POST',
            success: function (response) {
                var me = this;
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    this.initFields(result.attributes.meta);
                }
            },
            scope: this
        });
        this.dockedItems = [];
        this.addEvents('savesuccess', 'formready', 'beforesubmit');
        this.callParent(arguments);
    },
    initFields: function (meta) {
        var me = this;
        var fields = [];
        var form = meta;
        Ext.each(form.fields, function (item, index) {
            var fieldcfg = {
                xtype: item.xtype,
                submitValue: true,
                padding: '10 10 10 10',
                fieldLabel: item.fieldLabel,
                value: null,
                anchor: '90%',
                rowKeyValue: null,
                dbName: form.dbName
            };
            if (item.xtype == 'datefield') {
                if (item.sqlType == 'date') {
                    fieldcfg['submitFormat'] = 'Y-m-d';
                } else {
                    fieldcfg['submitFormat'] = 'Y-m-d H:i:s';
                }
            }
            if (item.xtype != 'radiogroup') {
                fieldcfg['name'] = item.name;
            } else {
                fieldcfg['radioName'] = item.name;
            }
            if ((item.xtype == 'combo' || item.xtype == 'combobox') && item.extraConfig && item.extraConfig.comboData) {
                Ext.apply(fieldcfg, {
                    store: Ext.create('Ext.data.ArrayStore', {
                        id: 0,
                        fields: [
                            'valueField', 'displayField'
                        ],
                        data: item.extraConfig.comboData
                    }),
                    triggerAction: 'all',
                    queryMode: 'local',
                    displayField: 'displayField',
                    valueField: 'valueField'
                });
            }
            if (item.extraConfig) {
                Ext.apply(fieldcfg, item.extraConfig);
            }
            if (me.fieldConfig && me.fieldConfig[item.name]) {
                Ext.apply(fieldcfg, me.fieldConfig[item.name]);
            }
            //暂时全可以为空
            //fieldcfg['allowBlank'] = true;
            fields.push(fieldcfg);
        });
        this.add(fields);
        this.setValues(this.fieldValues);
        this.addDocked({
            xtype: 'toolbar',
            dock: 'top',
            items: [{
                text: '保存',
                //xtype: 'button',
                handler: function () {
                    if (this.isValid()) {
                        if (this.fireEvent('beforesubmit')) {
                            Ext.Ajax.request({
                                url: __path + '/form/save.do',
                                method: 'POST',
                                params: this.getParams(fields),
                                success: function (response) {
                                    var result = Ext.decode(response.responseText);
                                    if (result.success) {
                                        Ext.Msg.alert("提示", "保存成功！");
                                        this.fireEvent('savesuccess');
                                    } else {
                                        Ext.Msg.alert("提示", "保存失败！");
                                    }
                                },
                                scope: this
                            });
                        }
                    }else{
                        Ext.Msg.alert('提示','请按提示修改后重试！');
                    }
                },
                scope: this
            }, '->', {
                text: '修改该表单',
                hidden : this.formTopHidden,
                handler: function () {
                    var pk = form.id;
                    var fieldValues = [{
                        rowKeyValue: pk,
                        name: 'ID',
                        value: form['id']
                    }, {
                        rowKeyValue: pk,
                        name: 'NAME',
                        value: form['name']
                    }, {
                        rowKeyValue: pk,
                        name: 'CONFIG',
                        value: form['config']
                    }, {
                        rowKeyValue: pk,
                        name: 'DBNAME',
                        value: form['dbName']
                    }, {
                        rowKeyValue: pk,
                        name: 'TITLE',
                        value: form['title']
                    }];
                    var data = {
                        id: 'editForm' + pk,
                        text: '修改表单-' + form['title'],
                        action: 'bitc.report.FormManage',
                        config: {
                            formId: pk,
                            formToken: form['name'],
                            formValues: fieldValues
                        }
                    };
                    addWorkTab(data);
                }
            }]
        });
        this.doLayout();
        this.fireEvent('formready', this);
    },
    getParams: function (fields) {
        var form = this.getForm();
        var params = {"formToken": this.formToken};
        Ext.each(fields, function (item) {
            if (item.xtype == 'radiogroup') {
                params[item.radioName + '_submitValue'] = item.submitValue;
                params[item.radioName] = this.down('radiogroup[radioName="' + item.radioName + '"]').getValue()[0];
                params[item.radioName + '_rowKeyValue'] = form.findField(item.radioName).rowKeyValue;
            } else if (item.xtype == 'datefield') {
                var field = form.findField(item.name);
                params[item.name + '_submitValue'] = item.submitValue;
                params[item.name] = Ext.util.Format.date(field.getValue(), field.submitFormat);
                params[item.name + '_rowKeyValue'] = field.rowKeyValue;
            } else {
                params[item.name + '_submitValue'] = item.submitValue;
                params[item.name] = form.findField(item.name).getValue();
                params[item.name + '_rowKeyValue'] = form.findField(item.name).rowKeyValue;
            }
        }, this);
        return params;
    },
    /**
     *
     * @param values [{name:'xxx',value:'xxx',rowKeyValue:'xxx'}]
     */
    setValues: function (values) {
        Ext.each(values, function (item) {
            var field = this.getForm().findField(item.name);
            if (field) {
                field.setValue(item.value);
                field.rowKeyValue = item.rowKeyValue;
            }
        }, this);
    }
});