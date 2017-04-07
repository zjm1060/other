Ext.define('bitc.wrenv.inputview.inputHedaoPanel', {
    extend: 'Ext.form.Panel',
    border: false,
    defaults: {
        anchor: '100%',
        labelWidth: 80
    },
    initComponent: function () {
        this.items= [{
            xtype: 'fieldcontainer',
            fieldLabel: '糙率',
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
                name: 'ICL',
                allowBlank: false,
                submitValue:false,
                hideLabel: true,
                value:'2',
                width: 100
            },  {
                xtype: 'textfield',
                allowBlank: false,
                submitValue:false,
                width: 100,
                value:0.025,
                name: 'DNL'
            }]
        }, {
            xtype: 'radiogroup',
            fieldLabel: '河道形状损失系数',
            labelWidth: 110,
            name: 'IFEGROUP',
            allowBlank: false,
            submitValue:false,
            hideLabel: false,
            readOnly:true,
            anchor: '33%',
            items: [{boxLabel: '不考虑', name: 'IFE',submitValue:false, inputValue: '0', checked: true}, {
                boxLabel: '考虑',
                name: 'IFE',
                submitValue:false,
                inputValue: '1'
            }]
        }, {
            xtype: 'radiogroup',
            labelWidth: 130,
            allowBlank: false,
            name: 'IFFPGROUP',
            submitValue:false,
            fieldLabel: '是否仅为复式断面',
            hideLabel: false,
            anchor: '33%',
            items: [{boxLabel: '是', name: 'IFFP',submitValue:false, inputValue: '1'}, {
                boxLabel: '否',
                submitValue:false,
                name: 'IFFP',
                inputValue: '0' ,
                checked: true
            }]
        }, {
            xtype: 'radiogroup',
            labelWidth: 130,
            name: 'IFHNGROUP',
            allowBlank: false,
            submitValue:false,
            fieldLabel: '低高水糙率是否不同',
            hideLabel: false,
            anchor: '33%',
            items: [{boxLabel: '不考虑', name: 'IFHN',submitValue:false, inputValue: '0', checked: true}, {
                boxLabel: '考虑',
                submitValue:false,
                name: 'IFHN',
                inputValue: '1'
            }]
        }, {
            xtype: 'fieldcontainer',
            //fieldLabel: 'ITEIKO',
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
                name: 'ITEIKO',
                hidden:true,
                allowBlank: false,
                submitValue:false,
                hideLabel: true,
                value:0,
                width: 100
            }, {
                name: 'BAA',
                allowBlank: false,
                submitValue:false,
                labelWidth: 180,
                fieldLabel: '主流流域面积（km²）',
                hideLabel: false,
                value:6.4,
                width: 280
            }, {
                name: 'BAASA',
                allowBlank: false,
                submitValue:false,
                fieldLabel: '主流系数',
                value:0.0,
                hideLabel: false,
                width: 200
            }]
        }];
        this.callParent(arguments);
    },
    setHdValue:function (data) {
        if(data['BAABAASA']){
            var BAABAASA =  data['BAABAASA'].split(',');
            this.getForm().findField('BAA').setValue(BAABAASA[0]);
            this.getForm().findField('BAASA').setValue(BAABAASA[1]);
        }

        if(data['ICLDNL']){
            var ICLDNL = data['ICLDNL'].split(',');
            this.getForm().findField('ICL').setValue(ICLDNL[0]);
            this.getForm().findField('DNL').setValue(ICLDNL[1]);

        }
        this.getForm().findField('IFEGROUP').setValue(data['IFE']);
        
        this.getForm().findField('IFFPGROUP').setValue(data['IFFP']);
        
        this.getForm().findField('IFHNGROUP').setValue(data['IFHN']);
        
        //this.getForm().findField('ITEIKO').setValue(data['ITEIKO']);
    }
});
