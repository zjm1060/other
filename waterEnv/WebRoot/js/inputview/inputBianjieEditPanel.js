Ext.define('bitc.wrenv.inputview.inputBianjieEditPanel', {
    extend: 'Ext.form.Panel',
    border: false,
    water_body_id: null,
    plan_id: null,
    defaults: {
        anchor: '100%',
        labelWidth: 80
    },
    layout: 'fit',
    initComponent: function () {
        var me = this;
        var zhiliuStore = Ext.create('Ext.data.Store', {
            fields: ['fileName', 'dmName'],
            proxy: {
                type: 'ajax',
                url: __path + '/inputview/getBianjieListData.do',
                reader: {
                    type: 'json',
                    root: 'rows'
                },
                extraParams: {water_body_id: this.water_body_id, plan_id: this.plan_id}
            },
            scope: this,
            autoLoad: true
        });
        var panel = this.panel = Ext.create('Ext.form.Panel', {
            border: false,
            //region:'center',
            items: [{
                xtype: 'combo',
                store: zhiliuStore,
                fieldLabel: '支流',
                editable: false,
                valueField: 'fileName',
                displayField: 'dmName',
                anchor: '80%',
                //submitValue:false,
                allowBlank: false,
                triggerAction: 'all',
                name: 'bianjie_name',
                listeners: {
                    scope: this,
                    collapse: function (field, opt) {
                        if (field.getRawValue() == '唐马寨' || field.getRawValue() == '观音阁') {
                            field.setValue(null);
                        }
                    },
                    expand:function(field,opt){
                        field.getStore().reload();
                    }
                }
            }, {
                xtype: 'combo',
                name: 'bianjie_type',
                anchor: '80%',
                fieldLabel: '边界类型',
                value: '流量',
                editable: false,
                valueField: 'value',
                displayField: 'value',
                allowBlank: false,
                triggerAction: 'all',
                store: Ext.create('Ext.data.ArrayStore', {
                    fields: ['value'],
                    data: [['流量'], ['水位'], ['流量水位过程线']]
                }),
                listeners: {
                    scope: this,
                    select: function (combo, records, eOpts) {
                        if(combo.getValue() != '流量'){
                            combo.setValue('流量');
                        }
                    }
                }
            }, {
                xtype: 'filefield',
                anchor: '80%',
                name: 'bianjie_file',
                fieldLabel: '上传数据',
                buttonText: '文件选择'
            }, {
                xtype: 'button',
                width: 100,
                margin: '0 10 0 10',
                text: '查看',
                scope: this,
                handler: me.showData
            }, {
                xtype: 'button',
                width: 100,
                margin: '0 10 0 10',
                text: '保存数据',
                scope: this,
                handler: me.uploadData
            }]
        });
        this.items = [panel];
        this.callParent(arguments);
    },
    showData: function () {
        var field = this.panel.getForm().findField('bianjie_name');
        this.fireEvent('showgriddata', field.getRawValue(), field.getValue());
    },
    uploadData: function () {
        this.fireEvent('uploadfiledata');
    }
});
