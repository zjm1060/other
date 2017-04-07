Ext.define("bitc.wrenv.inputview.inputYuQiao_wuranwu", {
    extend: 'Ext.tab.Panel',
    autoScroll: true,
    //tabPosition: 'left',
    border: false,
    title:'污染物边界参数',
    initComponent: function () {
        var me = this;
        this.tbar=[{
            margin: '10 10 0 10',
            xtype:'button',
            text:'添加序列',
            scope:this,
            handler:function(){
                me.fireEvent('xuliechange',me.items.length+1);
                var index = me.items.length+1;
                this.add(Ext.create('Ext.form.Panel',{
                    autoScroll: true,
                    title:'序列'+index,
                    items:[{
                        xtype:'fieldcontainer',
                        layout: {
                            type: 'hbox',
                            defaultMargins: {
                                top: 0,
                                right: 5,
                                bottom: 5,
                                left: 10
                            }
                        },
                        items:[{
                            xtype: 'filefield',
                            margin: '5 0 5 10',
                            labelWidth: 100,
                            width:300,
                            name: 'inputFile',
                            fieldLabel: '上传数据',
                            buttonText: '文件选择'
                        },{
                            xtype:'button',
                            width:90,
                            margin: '5 0 5 10',
                            text:'数据保存',
                            scope:this,
                            handler:this.onSave
                        },{
                            xtype:'button',
                            width:90,
                            margin: '5 0 5 10',
                            text:'删除',
                            scope:this,
                            handler:this.onDelete
                        }]
                    },Ext.create('Ext.grid.Panel', {
                        store: Ext.create('Ext.data.Store', {
                            fields: ['day', 'wuranwu'],
                            data: {'items': []},
                            proxy: {
                                type: 'memory',
                                reader: {
                                    type: 'json',
                                    root: 'items'
                                }
                            }
                        }),
                        tbar:[{
                            xtype:'button',
                            text:'添加',
                            handler:function(){
                                this.up('grid').getStore().insert(0,{});
                            }
                        }],
                        columns: [
                            {xtype: 'rownumberer', width: 40},
                            {text: '天', dataIndex: 'day', width: 80, editor: {
                                xtype: 'textfield',
                                saveBtnText: '保存',
                                cancelBtnText: "取消",
                                allowBlank: false
                            }},
                            {
                                text: '污染物浓度(mg/l)', dataIndex: 'wuranwu', flex: 1, editor: {
                                xtype: 'textfield',
                                saveBtnText: '保存',
                                cancelBtnText: "取消",
                                allowBlank: false
                            }
                            }
                        ],
                        height: 550,
                        plugins: [
                            Ext.create('Ext.grid.plugin.CellEditing', {
                                clicksToEdit: 1
                            })
                        ],
                        autoScroll: true,
                        columnWidth: 0.4
                    })],
                    listeners:{
                        scope:this,
                        close:function(panel,option){
                            me.fireEvent('xuliechange',me.items.length-1);
                            var flag = true;
                            for(var i=0;i<me.items.length;i++){
                                if(panel == me.items.getAt(i)){
                                    flag = false;
                                }else if(flag){
                                    me.items.getAt(i).setTitle('序列'+(i+1));
                                }else{
                                    me.items.getAt(i).setTitle('序列'+i);
                                }
                            }
                        }
                    }
                }));
                Ext.Ajax.request({
                    url:__path+'/inputview/addKVData.do',
                    method: 'POST',
                    params: {
                        planId:this.planId,
                        data: JSON.stringify({'C22_NCSER3':me.items.length+''})
                    },
                    success: function (response) {
                        var result = Ext.decode(response.responseText);
                        if (!result.success) {
                            Ext.Msg.alert('提示','出现异常！');
                        }
                    },
                    failure: function () {
                        callback.call(scope || window, false);
                    }
                });
            }
        }];
        var panel = Ext.create('Ext.form.Panel',{
            autoScroll: true,
            title:'序列1',
            items:[{
                xtype:'fieldcontainer',
                layout: {
                    type: 'hbox',
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 10
                    }
                },
                items:[{
                    xtype: 'filefield',
                    margin: '5 0 5 10',
                    labelWidth: 100,
                    width:300,
                    name: 'inputFile',
                    fieldLabel: '上传数据',
                    buttonText: '文件选择'
                },{
                    xtype:'button',
                    width:90,
                    margin: '5 0 5 10',
                    text:'数据保存',
                    scope:this,
                    handler:this.onSave
                },{
                    xtype:'button',
                    width:90,
                    margin: '5 0 5 10',
                    text:'删除',
                    scope:this,
                    handler:this.onDelete
                }]
            },Ext.create('Ext.grid.Panel', {
                store: Ext.create('Ext.data.Store', {
                    fields: ['day', 'wuranwu'],
                    data: {'items': []},
                    proxy: {
                        type: 'memory',
                        reader: {
                            type: 'json',
                            root: 'items'
                        }
                    }
                }),
                tbar:[{
                    xtype:'button',
                    text:'添加',
                    handler:function(){
                        this.up('grid').getStore().insert(0,{});
                    }
                }],
                columns: [
                    {xtype: 'rownumberer', width: 40},
                    {text: '天', dataIndex: 'day', width: 80, editor: {
                        xtype: 'textfield',
                        saveBtnText: '保存',
                        cancelBtnText: "取消",
                        allowBlank: false
                    }},
                    {
                        text: '污染物浓度(mg/l)', dataIndex: 'wuranwu', flex: 1, editor: {
                        xtype: 'textfield',
                        saveBtnText: '保存',
                        cancelBtnText: "取消",
                        allowBlank: false
                    }
                    }
                ],
                height: 550,
                plugins: [
                    Ext.create('Ext.grid.plugin.CellEditing', {
                        clicksToEdit: 1
                    })
                ],
                autoScroll: true,
                columnWidth: 0.4
            })],
            listeners:{
                scope:this,
                close:function(panel,option){
                    me.fireEvent('xuliechange',me.items.length-1);
                    var flag = true;
                    for(var i=0;i<me.items.length;i++){
                        if(panel == me.items.getAt(i)){
                            flag = false;
                        }else if(flag){
                            me.items.getAt(i).setTitle('序列'+(i+1));
                        }else{
                            me.items.getAt(i).setTitle('序列'+i);
                        }
                    }
                }
            }
        });
        this.items=[panel];
        this.callParent(arguments);
    },
    onSave:function(){
        this.setXulieCount();
        var baseForm = this.getActiveTab();
        var index =  this.items.indexOf(baseForm)+1;
        if (baseForm.isValid()) {
            var gridData = [];
            baseForm.items.getAt(1).getStore().each(function (record) {
                gridData.push([
                    record.get('day'),
                    record.get('wuranwu')
                ]);
            });
            baseForm.submit({
                url: __path + '/inputview/addWuRanWuData.do',
                method: 'post',
                params: {
                    planId:this.planId,
                    index:index,
                    gridData: JSON.stringify(gridData)
                },
                waitMsg: '正在保存，请稍候...',
                scope: this,
                success: function (form, action) {
                    if (action.result.success) {
                        this.requestDataFromDB();
                        Ext.Msg.alert('提示','保存成功');
                    } else {
                        Ext.Msg.alert('错误', action.result
                            ? action.result.message
                            : '保存时服务器出现错误，请与管理员联系。');
                    }
                    this.fireEvent('savesuccess', this, action.result);
                },
                failure: function (form, action) {
                    Ext.Msg.alert('错误', action.result
                        ? action.result.message
                        : '保存时服务器出现错误，请与管理员联系。');
                }
            });
        }else{
            Ext.Msg.alert('提示','请检查输入项是否正确！');
        }
    },
    onDelete:function(){
        var me = this;
        var baseForm = this.getActiveTab();
        var index =  this.items.indexOf(baseForm)+1;
        var data={};
        data['C22_NCSER3']= (this.items.length-1)+'';
        Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
            if(e =='yes'){
                Ext.Ajax.request({
                    scope:this,
                    url: __path+'/inputview/deleteYuQiaoInputData.do',
                    params: {
                        planId: me.planId,
                        fileId:'79D02002047041818B2D97D5C6ECE619',
                        data:JSON.stringify(data),
                        index:index
                    },
                    success:function(response){
                        var data = Ext.decode(response.responseText);
                        if(data.success){
                            me.getActiveTab().close();
                        }
                    }
                });
            }
        });
    },
    requestDataFromDB:function(){

        Ext.Ajax.request({
            url:__path+'/inputview/getYuQiaoWuranwuFileData.do',
            method:'POST',
            scope:this,
            params:{
                planId:this.planId
            },
            success:function(response){
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    var data = result.attributes.data;
                    if(data){
                        for(var i=0;i<data.length;i++){
                            var panel = this.items.getAt(i);
                            if(!panel){
                                var index = this.items.length+1;
                                panel = Ext.create('Ext.form.Panel',{
                                    autoScroll: true,
                                    title:'序列'+index,
                                    items:[{
                                        xtype:'fieldcontainer',
                                        layout: {
                                            type: 'hbox',
                                            defaultMargins: {
                                                top: 0,
                                                right: 5,
                                                bottom: 5,
                                                left: 10
                                            }
                                        },
                                        items:[{
                                            xtype: 'filefield',
                                            margin: '5 0 5 10',
                                            labelWidth: 100,
                                            width:300,
                                            name: 'inputFile',
                                            fieldLabel: '上传数据',
                                            buttonText: '文件选择'
                                        },{
                                            xtype:'button',
                                            width:90,
                                            margin: '5 0 5 10',
                                            text:'数据保存',
                                            scope:this,
                                            handler:this.onSave
                                        },{
                                            xtype:'button',
                                            width:90,
                                            margin: '5 0 5 10',
                                            text:'删除',
                                            scope:this,
                                            handler:this.onDelete
                                        }]
                                    },Ext.create('Ext.grid.Panel', {
                                        store: Ext.create('Ext.data.Store', {
                                            fields: ['day', 'wuranwu'],
                                            data: {'items': []},
                                            proxy: {
                                                type: 'memory',
                                                reader: {
                                                    type: 'json',
                                                    root: 'items'
                                                }
                                            }
                                        }),
                                        tbar:[{
                                            xtype:'button',
                                            text:'添加',
                                            handler:function(){
                                                this.up('grid').getStore().insert(0,{});
                                            }
                                        }],
                                        columns: [
                                            {xtype: 'rownumberer', width: 40},
                                            {text: '天', dataIndex: 'day', width: 80, editor: {
                                                xtype: 'textfield',
                                                saveBtnText: '保存',
                                                cancelBtnText: "取消",
                                                allowBlank: false
                                            }},
                                            {
                                                text: '污染物浓度(mg/l)', dataIndex: 'wuranwu', flex: 1, editor: {
                                                xtype: 'textfield',
                                                saveBtnText: '保存',
                                                cancelBtnText: "取消",
                                                allowBlank: false
                                            }
                                            }
                                        ],
                                        height: 550,
                                        plugins: [
                                            Ext.create('Ext.grid.plugin.CellEditing', {
                                                clicksToEdit: 1
                                            })
                                        ],
                                        autoScroll: true,
                                        columnWidth: 0.4
                                    })],
                                    listeners:{
                                        scope:this,
                                        close:function(panel,option){
                                            me.fireEvent('xuliechange',me.items.length-1);
                                            var flag = true;
                                            for(var i=0;i<me.items.length;i++){
                                                if(panel == me.items.getAt(i)){
                                                    flag = false;
                                                }else if(flag){
                                                    me.items.getAt(i).setTitle('序列'+(i+1));
                                                }else{
                                                    me.items.getAt(i).setTitle('序列'+i);
                                                }
                                            }
                                        }
                                    }
                                });
                                this.add(panel);
                            }
                            panel.items.getAt(1).getStore().loadData(data[i])

                        }
                    }
                }
            }
        })
    },
    setXulieCount:function(){
        var me = this;
        if (this.planId && me.items.length ==1) {
            Ext.Ajax.request({
                url:__path+'/inputview/addKVData.do',
                method: 'POST',
                params: {
                    planId:this.planId,
                    data: JSON.stringify({'C22_NCSER2':me.items.length+''})
                },
                success: function (response) {
                    var result = Ext.decode(response.responseText);
                    if (!result.success) {
                        Ext.Msg.alert('提示','出现异常！');
                    }
                },
                failure: function () {
                    callback.call(scope || window, false);
                }
            });
        }
    },
    afterFirstLayout: function() {
        if(this.planId){
            this.requestDataFromDB();
        }
        this.callParent(arguments);
    }
});
