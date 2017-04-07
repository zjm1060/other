Ext.define('bitc.wrenv.inputview.inputManagerPanel', {
    extend: 'Ext.panel.Panel',
    border: false,
    layout: 'border',
    water_body_id:null,
    water_body_name:null,
    currentUser:null,
    initComponent: function () {
        var me = this;
        var planEdit = Ext.create('bitc.wrenv.inputview.planEditPanel', {
            water_body_id: this.water_body_id,
            water_body_name: this.water_body_name,
            plan_id:this.plan_id,
            currentUser:this.currentUser,
            frame: true,
            height: 250,
            collapsible: true,
            title: '创建方案',
            region: 'north',
            listeners: {
                scope:this,
                savesuccess:function(plan_id){
                    this.items.getAt(0).getForm().findField('plan_id').setValue(plan_id);
                    var year = this.items.getAt(0).getForm().findField('run_year').getValue();
                    //me.remove(me.inputEdit.id);
                    if(this.items.length == 1){
                        me.add(Ext.create('bitc.wrenv.inputview.inputEditPanel', {
                            region: 'center',
                            plan_id:plan_id,
                            water_body_id:me.water_body_id
                        }));
                        me.items.getAt(1).bianjieReload(year);
                    }
                },
                yearchange:function(newValue){
                    if(this.items.length == 2) {
                        me.items.getAt(1).bianjieReload(newValue);
                    }
                }
            }
        });
        //var inputEdit= this.inputEdit = Ext.create('bitc.wrenv.inputview.inputEditPanel', {
        //    region: 'center'
        //});

        this.items = [planEdit];
        this.callParent(arguments);
    }
});