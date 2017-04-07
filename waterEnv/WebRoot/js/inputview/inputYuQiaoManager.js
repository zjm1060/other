Ext.define('bitc.wrenv.inputview.inputYuQiaoManager',{
    extend: 'Ext.tab.Panel',
    autoScroll: true,
    border: false,
    planId:null,
    initComponent: function () {
        var zhukong = Ext.create('bitc.wrenv.inputview.inputYuQiao_zhukong',{planId:this.planId});
        var qixiang = Ext.create('bitc.wrenv.inputview.inputYuQiao_qixiang',{planId:this.planId});
        var fengchang = Ext.create('bitc.wrenv.inputview.inputYuQiao_fengchang',{planId:this.planId});
        var liuliang = Ext.create('bitc.wrenv.inputview.inputYuQiao_liuliang',{planId:this.planId});
        var wendubianjie = Ext.create('bitc.wrenv.inputview.inputYuQiao_wendu',{
            planId:this.planId,
            listeners:{
                'xuliechange':function(data){
                    var store = [];
                    for(var i=0;i<=data;i++){
                        store.push([i+'']);
                    }
                    liuliang.wenduStoreReload(store);
                }
            }
        });
        var wuranwu = Ext.create('bitc.wrenv.inputview.inputYuQiao_wuranwu',{
            planId:this.planId,
            listeners:{
                'xuliechange':function(data){
                    var store = [];
                    for(var i=0;i<=data;i++){
                        store.push([i+'']);
                    }
                    liuliang.wuranwuStoreReload(store);
                }
            }
        });

        this.items=[zhukong,qixiang,fengchang,wendubianjie,wuranwu,liuliang];
        this.callParent(arguments);
    }
});
