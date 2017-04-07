Ext.define('bitc.wrenv.inputview.inputDuanmianPanel', {
    extend: 'Ext.form.Panel',
    border: false,
    defaults: {
        anchor: '100%',
        labelWidth: 100
    },
    initComponent: function () {
        this.items = [{
            xtype: 'fieldcontainer',
            fieldLabel: '断面个数',
            defaults: {
                xtype: 'textfield'
            },
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 0
                }
            },
            items: [{
                name: 'IDAN',
                allowBlank: false,
                submitValue:false,
                hideLabel: true,
                value:158,
                width: 150
            }, {
                name: 'IGORY',
                allowBlank: false,
                submitValue:false,
                fieldLabel: '支流域个数',
                value:0,
                hideLabel: false,
                width: 250
            }]
        }, {
            xtype: 'fieldcontainer',
            fieldLabel: '首节点号',
            defaults: {
                xtype: 'textfield'
            },
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 0
                }
            },
            items: [{
                allowBlank: false,
                submitValue:false,
                value:1,
                anchor: '50%',
                width: 150,
                name: 'NJ'
            },{
                name: 'ISHI',
                allowBlank: false,
                submitValue:false,
                fieldLabel: '末节点号',
                hideLabel: false,
                value:158,
                width: 250
            }]
        }, {
            xtype: 'textfield',
            fieldLabel: '首断面号',
            hidden:true,
            name: 'IE',
            allowBlank: false,
            submitValue:false,
            value:158,
            labelWidth:100,
            width: 250
        }];
        this.callParent(arguments);
    },
    setDmValue:function (data) {
        this.getForm().findField('NJ').setValue(data['NJ']);
        this.getForm().findField('IGORY').setValue(data['IGORY']);
        this.getForm().findField('IDAN').setValue(data['IDAN']);
        this.getForm().findField('ISHI').setValue(data['ISHI']);
        //this.getForm().findField('IE').setValue(data['IE']);
    }
});
