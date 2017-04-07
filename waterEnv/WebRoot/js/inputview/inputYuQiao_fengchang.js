Ext.define("bitc.wrenv.inputview.inputYuQiao_fengchang", {
    extend: 'Ext.form.Panel',
    autoScroll: true,
    title:'风场边界参数',
    layout:'column',
    defaults: {
        anchor: '98%',
        labelWidth: 80
    },
    initComponent: function () {
        var gridstore = this.gridstore = Ext.create('Ext.data.Store', {
            fields: ['day', 'wser_l_c02', 'wser_l_c03'],
            data: {'items': []},
            proxy: {
                type: 'memory',
                reader: {
                    type: 'json',
                    root: 'items'
                }
            }
        });
        var grid = this.grid = Ext.create('Ext.grid.Panel',{
            autoScroll: true,
            margin: '10 0 0 10',
            store: gridstore,
            tbar:[{
                xtype:'button',
                text:'添加',
                scope:this,
                handler:function(){
                    this.grid.getStore().insert(0,{});
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
                    text: '风速(m/s)', dataIndex: 'wser_l_c02', flex: 1, editor: {
                    xtype: 'textfield',
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
                }, {
                    text: '风向(°)', dataIndex: 'wser_l_c03', flex: 1, editor: {
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
        });
        var panel = Ext.create('Ext.form.Panel', {
            autoScroll: true,
            border: false,
            columnWidth: 0.35,
            height:300,
            anchor: '98%',
            defaults: {
                margin: '5 5 5 10',
                labelWidth: 150,
                width: 250,
                submitValue:false
            },
            items: [{
                xtype: 'filefield',
                margin: '50 10 10 10',
                labelWidth: 100,
                width:300,
                name: 'inputFile',
                fieldLabel: '上传数据',
                buttonText: '文件选择'
            },{
                xtype:'fieldcontainer',
                defaults: {
                    xtype: 'button',
                    width:100
                },
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
                    margin: '10 10 0 10',
                    text: '保存数据',
                    scope: this,
                    handler: this.onSave
                }]
            }]
        });
        this.items=[panel,grid];
        this.callParent(arguments);
    },
    onSave: function () {
        var baseForm = this.getForm();
        if (baseForm.isValid()) {
            var gridData = [];
            this.grid.getStore().each(function (record) {
                gridData.push([
                    record.get('day'),
                    record.get('wser_l_c02'),
                    record.get('wser_l_c03')
                ]);
            });
            baseForm.submit({
                url: __path + '/inputview/addFengChangData.do',
                method: 'post',
                params: {
                    planId:this.planId,
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
    requestDataFromDB:function(){
        Ext.Ajax.request({
            url:__path+'/inputview/getYuQiaoFengchangFileData.do',
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
                        this.grid.getStore().loadData(data);
                    }
                }
            }
        })
    },
    afterFirstLayout: function() {
        if(this.planId){
            this.requestDataFromDB();
        }
        this.callParent(arguments);
    }
});
