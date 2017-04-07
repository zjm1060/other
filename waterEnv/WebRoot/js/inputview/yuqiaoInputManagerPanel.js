Ext.define('bitc.wrenv.inputview.yuqiaoInputManagerPanel', {
    extend: 'Ext.panel.Panel',
    border: false,
    layout: 'border',
    water_body_id:null,
    water_body_name:null,
    currentUser:null,
    initComponent: function () {
        var me = this;
        var planEdit = Ext.create('bitc.wrenv.inputview.yuqiaoPlanEditPanel', {
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
                    if(this.items.length == 1){
                        me.add(Ext.create('bitc.wrenv.inputview.inputYuQiaoManager', {
                            region: 'center',
                            planId:plan_id
                        }));
                    }
                }
            }
        });
        this.items = [planEdit];
        this.callParent(arguments);
    }
});