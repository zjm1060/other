Ext.define('bitc.wrenv.inputview.inputEditManager', {
    extend: 'Ext.panel.Panel',
    border: false,
    plan_id:null,
    layout:'border',
    initComponent: function () {
        Ext.Ajax.request({
            scope:this,
            url: __path+'/inputview/getUpdatePlanData.do',
            async: false,
            params: {
                plan_id: this.plan_id
            },
            success: function(response){
                var text = Ext.decode(response.responseText);
                var data = text.attributes.data;
                if(data){
                    if(data[0]){
                        var plnaInfo = data[0];
                       this.planEdit = Ext.create('bitc.wrenv.inputview.planEditPanel', {
                            water_body_id: plnaInfo.run_obj_id,
                            water_body_name: plnaInfo.run_obj_name,
                            plan_id:plnaInfo.plan_id,
                            currentUser:plnaInfo.plan_owner,
                            frame: true,
                            height: 250,
                            collapsible: true,
                            title: '创建方案',
                            region: 'north'
                        });
                        this.planEdit.setPlanValue(plnaInfo);
                       this.inputEdit = Ext.create('bitc.wrenv.inputview.inputEditPanel', {
                           plan_id:plnaInfo.plan_id,
                           water_body_id: plnaInfo.run_obj_id,
                            region: 'center'
                        });
                        this.inputEdit.setHeWangDiXing(data[1]);
                        this.inputEdit.setSdlValue(data[2]);
                        this.inputEdit.setSzpjCgValue(data[3]);
                        this.inputEdit.setSzpjSsValue(data[4]);
                        this.inputEdit.setSzpjCgOrSs(data[5]);
                    }
                }
            }
        });
        this.items = [this.planEdit, this.inputEdit ];
        this.callParent(arguments);
    }
});