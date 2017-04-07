Ext.define('bitc.wrenv.inputview.inputSzpjEditPanel', {
    extend: 'Ext.form.Panel',
    border: false,
    title: '水质参数',
    plan_id:null,
    defaults: {
        anchor: '100%',
        labelWidth: 80
    },
    initComponent: function () {
        var changgui = this.changgui=  Ext.create('bitc.wrenv.inputview.inputSzpjChangguiEditPanel',{plan_id:this.plan_id});
        var shunshi = this.shunshi=  Ext.create('bitc.wrenv.inputview.inputSzpjShunshiEditPanel',{hidden:true,plan_id:this.plan_id});
        this.items=[{
            xtype: 'fieldset',
            layout: 'anchor',
            padding: '5 5 5 5',
            bodyPadding: 10,
            title:'基本参数',
            anchor: '98%',
            items:[{
                xtype: 'radiogroup',
                fieldLabel: '水质计算类型',
                labelWidth: 110,
                name: 'model_sub_type_group',
                allowBlank: false,
                submitValue:false,
                anchor: '33%',
                items: [{boxLabel: '常规', name: 'model_sub_type', submitValue:false,inputValue: '1', checked: true}, {
                    boxLabel: '瞬时源',
                    submitValue:false,
                    name: 'model_sub_type',
                    inputValue: '2'
                }],
                listeners:{
                    scope:this,
                    change:function( radio, newValue, oldValue, eOpts ){
                        if(newValue.length == 1){
                            if(newValue[0] == 1){
                                this.changgui.show();
                                this.shunshi.hide();
                            }else{
                                this.changgui.hide();
                                this.shunshi.show();
                            }
                        }
                    }
                }
            }]
        },changgui,shunshi];
        this.callParent(arguments);
    }
});
