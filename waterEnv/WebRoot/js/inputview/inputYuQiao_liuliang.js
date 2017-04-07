Ext.define("bitc.wrenv.inputview.inputYuQiao_liuliang", {
    extend: 'Ext.tab.Panel',
    layout: 'column',
    //tabPosition: 'left',
    autoScroll: true,
    border: false,
    title: '流量边界参数',
    initComponent: function () {
        var wenduStore = this.wenduStore = Ext.create('Ext.data.ArrayStore', {
            fields: ['value'],
            data: [['0'], ['1']]
        });
        var wuranwuStore = this.wuranwuStore = Ext.create('Ext.data.ArrayStore', {
            fields: ['value'],
            data: [['0'], ['1']]
        });
        this.tbar = [{
            margin: '10 10 0 10',
            xtype: 'button',
            text: '添加序列',
            scope: this,
            handler: function () {
                var me = this;
                this.fireEvent('xuliechange', me.items.length + 1);
                var index = this.items.length + 1;
                this.add(Ext.create('Ext.form.Panel', {
                    autoScroll: true,
                    layout: 'column',
                    title: '序列' + index,
                    items: [{
                        xtype: 'fieldcontainer',
                        columnWidth: 0.3,
                        defaults: {
                            margin: '5 5 5 10',
                            labelWidth: 150,
                            width: 250,
                            submitValue: false
                        },
                        items: [{
                            xtype: 'textfield',
                            fieldLabel: '源汇项位置的I索引号',
                            labelWidth: 170,
                            allowBlank: false,
                            name: 'C24_IQS'
                        }, {
                            xtype: 'textfield',
                            fieldLabel: '源汇项位置的J索引号',
                            labelWidth: 170,
                            allowBlank: false,
                            name: 'C24_JQS'
                        }, {
                            xtype: 'combo',
                            editable: false,
                            allowBlank: false,
                            labelWidth: 170,
                            name: 'C24_ICSER2',
                            fieldLabel: '温度时间序列的编号',
                            store: wenduStore,
                            queryMode: 'local',
                            displayField: 'value',
                            valueField: 'value'
                        }, {
                            xtype: 'combo',
                            editable: false,
                            allowBlank: false,
                            labelWidth: 170,
                            name: 'C24_ICSER3',
                            fieldLabel: '污染物时间序列的编号',
                            store: wuranwuStore,
                            queryMode: 'local',
                            displayField: 'value',
                            valueField: 'value'
                        }, {
                            xtype: 'filefield',
                            margin: '5 0 5 10',
                            labelWidth: 100,
                            width: 300,
                            name: 'inputFile',
                            fieldLabel: '上传数据',
                            buttonText: '文件选择'
                        }, {
                            xtype: 'button',
                            width: 90,
                            margin: '5 0 5 10',
                            text: '数据保存',
                            scope: this,
                            handler: this.onSave
                        }, {
                            xtype: 'button',
                            width: 90,
                            margin: '5 0 5 10',
                            text: '删除',
                            scope: this,
                            handler: this.onDelete
                        }]
                    }, Ext.create('Ext.grid.Panel', {
                        margin: '10 0 0 10',
                        columnWidth: 0.4,
                        store: Ext.create('Ext.data.Store', {
                            fields: ['day', 'liuliang'],
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
                                text: '流量', dataIndex: 'liuliang', flex: 1, editor: {
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
                        autoScroll: true
                    })],
                    listeners: {
                        scope: this,
                        close: function (panel, option) {
                            debugger;
                            var me = this;
                            this.fireEvent('xuliechange', me.items.length - 1);
                            var flag = true;
                            for (var i = 0; i < this.items.length; i++) {
                                if (panel == this.items.getAt(i)) {
                                    flag = false;
                                } else if (flag) {
                                    this.items.getAt(i).setTitle('序列' + (i + 1));
                                } else {
                                    this.items.getAt(i).setTitle('序列' + i);
                                }
                            }
                        }
                    }
                }));
                Ext.Ajax.request({
                    url: __path + '/inputview/addKVData.do',
                    method: 'POST',
                    params: {
                        planId: this.planId,
                        data: JSON.stringify({'C23_NQSIJ': me.items.length + '', 'C23_NQSER': me.items.length + ''})
                    },
                    success: function (response) {
                        var result = Ext.decode(response.responseText);
                        if (!result.success) {
                            Ext.Msg.alert('提示', '出现异常！');
                        }
                    },
                    failure: function () {
                        callback.call(scope || window, false);
                    }
                });
            }
        }];
        var panel = Ext.create('Ext.form.Panel', {
            autoScroll: true,
            title: '序列1',
            layout: 'column',
            items: [{
                xtype: 'fieldcontainer',
                defaults: {
                    margin: '5 5 5 10',
                    labelWidth: 150,
                    width: 250,
                    submitValue: false
                },
                columnWidth: 0.3,
                items: [{
                    xtype: 'textfield',
                    fieldLabel: '源汇项位置的I索引号',
                    labelWidth: 170,
                    allowBlank: false,
                    name: 'C24_IQS'
                }, {
                    xtype: 'textfield',
                    fieldLabel: '源汇项位置的J索引号',
                    labelWidth: 170,
                    allowBlank: false,
                    name: 'C24_JQS'
                }, {
                    xtype: 'combo',
                    editable: false,
                    allowBlank: false,
                    labelWidth: 170,
                    name: 'C24_ICSER2',
                    fieldLabel: '温度时间序列的编号',
                    store: wenduStore,
                    queryMode: 'local',
                    displayField: 'value',
                    valueField: 'value'
                }, {
                    xtype: 'combo',
                    editable: false,
                    allowBlank: false,
                    labelWidth: 170,
                    name: 'C24_ICSER3',
                    fieldLabel: '污染物时间序列的编号',
                    store: wuranwuStore,
                    queryMode: 'local',
                    displayField: 'value',
                    valueField: 'value'
                }, {
                    xtype: 'filefield',
                    margin: '5 0 5 10',
                    labelWidth: 100,
                    width: 300,
                    name: 'inputFile',
                    fieldLabel: '上传数据',
                    buttonText: '文件选择'
                }, {
                    xtype: 'button',
                    width: 90,
                    margin: '5 0 5 10',
                    text: '数据保存',
                    scope: this,
                    handler: this.onSave
                }, {
                    xtype: 'button',
                    width: 90,
                    margin: '5 0 5 10',
                    text: '删除',
                    scope: this,
                    handler: this.onDelete
                }]
            }, Ext.create('Ext.grid.Panel', {
                margin: '10 0 0 10',
                columnWidth: 0.4,
                store: Ext.create('Ext.data.Store', {
                    fields: ['day', 'liuliang'],
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
                        text: '流量', dataIndex: 'liuliang', flex: 1, editor: {
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
                autoScroll: true
            })],
            listeners: {
                scope: this,
                close: function (panel, option) {
                    var me = this;
                    this.fireEvent('xuliechange', me.items.length - 1);
                    var flag = true;
                    for (var i = 0; i < this.items.length; i++) {
                        if (panel == this.items.getAt(i)) {
                            flag = false;
                        } else if (flag) {
                            this.items.getAt(i).setTitle('序列' + (i + 1));
                        } else {
                            this.items.getAt(i).setTitle('序列' + i);
                        }
                    }
                }
            }
        });
        this.items = [panel];
        this.callParent(arguments);
    },
    wenduStoreReload: function (data) {
        this.items.each(function (e) {
            e.getForm().findField('C24_ICSER2').setValue(null);
        });
        this.wenduStore.loadData(data);
    },
    wuranwuStoreReload: function (data) {
        this.items.each(function (e) {
            e.getForm().findField('C24_ICSER3').setValue(null);
        });
        this.wuranwuStore.loadData(data);
    },
    onSave: function () {
        this.setXulieCount();
        var baseForm = this.getActiveTab().getForm();
        var index = this.items.indexOf(this.getActiveTab()) + 1;
        if (baseForm.isValid()) {
            var C24_IQS = baseForm.findField('C24_IQS').getValue();
            var C24_JQS = baseForm.findField('C24_JQS').getValue();
            var C24_ICSER2 = baseForm.findField('C24_ICSER2').getValue();
            var C24_ICSER3 = baseForm.findField('C24_ICSER3').getValue();
            var data = {};
            data['I_CODE'] = C24_IQS + '';
            data['J_CODE'] = C24_JQS + '';
            data['LL_XULIE'] = index + '';
            data['WD_XULIE'] = C24_ICSER2 + '';
            data['WRW_XULIE'] = C24_ICSER3 + '';
            var gridData = [];
            this.getActiveTab().items.getAt(1).getStore().each(function (record) {
                gridData.push([
                    record.get('day'),
                    record.get('liuliang')
                ]);
            });

            baseForm.submit({
                url: __path + '/inputview/addLiuLiangData.do',
                method: 'post',
                params: {
                    planId: this.planId,
                    index: index,
                    data: JSON.stringify(data),
                    gridData: JSON.stringify(gridData)
                },
                waitMsg: '正在保存，请稍候...',
                scope: this,
                success: function (form, action) {
                    if (action.result.success) {
                        this.requestDataFromDB();
                        Ext.Msg.alert('提示', '保存成功');
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
        } else {
            Ext.Msg.alert('提示', '请检查输入项是否正确！');
        }
    },
    onDelete: function () {
        var me = this;
        var baseForm = this.getActiveTab();
        var index = this.items.indexOf(baseForm) + 1;
        var data = {};
        data['C23_NQSIJ'] = (this.items.length - 1) + '';
        data['C23_NQSER'] = (this.items.length - 1) + '';
        Ext.MessageBox.confirm("提示", "确定要删除此项配置？", function (e) {
            if (e == 'yes') {
                Ext.Ajax.request({
                    scope: this,
                    url: __path + '/inputview/deleteYuQiaoInputData.do',
                    params: {
                        planId: me.planId,
                        fileId: '22AC330DDC47456BB895782AD9D96CC6',
                        data: JSON.stringify(data),
                        index: index
                    },
                    success: function (response) {
                        var data = Ext.decode(response.responseText);
                        if (data.success) {
                            me.getActiveTab().close();
                        }
                    }
                });
            }
        });
    },
    requestDataFromDB: function () {
        var index = this.items.indexOf(this.getActiveTab()) + 1;
        Ext.Ajax.request({
            url: __path + '/inputview/getYuQiaoXulieMappingData.do',
            method: 'POST',
            async: false,
            scope: this,
            params: {
                planId: this.planId
            },
            success: function (response) {
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    var data = result.attributes.data;
                    if (data) {
                        if(data.length>0){
                            var res = data[0];
                            var wenducount = res['wendu_xulie'];
                            var wenduxulie = [];
                            for(var i=0;i<=wenducount;i++){
                                wenduxulie.push([i]);
                            }
                            this.wenduStore.loadData(wenduxulie);
                            var wuranwucount = res['wuranwu_xulie'];
                            var wuranwuxulie = [];
                            for(var i=0;i<=wuranwucount;i++){
                                wuranwuxulie.push([i]);
                            }
                            this.wuranwuStore.loadData(wuranwuxulie);
                        }
                        for (var i = 1; i < data.length; i++) {
                            var panel = this.items.getAt(i-1);
                            if (!panel) {
                                var index = this.items.length + 1;
                                panel = Ext.create('Ext.form.Panel', {
                                    autoScroll: true,
                                    layout: 'column',
                                    title: '序列' + index,
                                    items: [{
                                        xtype: 'fieldcontainer',
                                        columnWidth: 0.3,
                                        defaults: {
                                            margin: '5 5 5 10',
                                            labelWidth: 150,
                                            width: 250,
                                            submitValue: false
                                        },
                                        items: [{
                                            xtype: 'textfield',
                                            fieldLabel: '源汇项位置的I索引号',
                                            labelWidth: 170,
                                            allowBlank: false,
                                            name: 'C24_IQS'
                                        }, {
                                            xtype: 'textfield',
                                            fieldLabel: '源汇项位置的J索引号',
                                            labelWidth: 170,
                                            allowBlank: false,
                                            name: 'C24_JQS'
                                        }, {
                                            xtype: 'combo',
                                            editable: false,
                                            allowBlank: false,
                                            labelWidth: 170,
                                            name: 'C24_ICSER2',
                                            fieldLabel: '温度时间序列的编号',
                                            store: this.wenduStore,
                                            queryMode: 'local',
                                            displayField: 'value',
                                            valueField: 'value'
                                        }, {
                                            xtype: 'combo',
                                            editable: false,
                                            allowBlank: false,
                                            labelWidth: 170,
                                            name: 'C24_ICSER3',
                                            fieldLabel: '污染物时间序列的编号',
                                            store: this.wuranwuStore,
                                            queryMode: 'local',
                                            displayField: 'value',
                                            valueField: 'value'
                                        }, {
                                            xtype: 'filefield',
                                            margin: '5 0 5 10',
                                            labelWidth: 100,
                                            width: 300,
                                            name: 'inputFile',
                                            fieldLabel: '上传数据',
                                            buttonText: '文件选择'
                                        }, {
                                            xtype: 'button',
                                            width: 90,
                                            margin: '5 0 5 10',
                                            text: '数据保存',
                                            scope: this,
                                            handler: this.onSave
                                        }, {
                                            xtype: 'button',
                                            width: 90,
                                            margin: '5 0 5 10',
                                            text: '删除',
                                            scope: this,
                                            handler: this.onDelete
                                        }]
                                    }, Ext.create('Ext.grid.Panel', {
                                        margin: '10 0 0 10',
                                        columnWidth: 0.4,
                                        store: Ext.create('Ext.data.Store', {
                                            fields: ['day', 'liuliang'],
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
                                                text: '流量', dataIndex: 'liuliang', flex: 1, editor: {
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
                                        autoScroll: true
                                    })],
                                    listeners: {
                                        scope: this,
                                        close: function (panel, option) {
                                            debugger;
                                            var me = this;
                                            this.fireEvent('xuliechange', me.items.length - 1);
                                            var flag = true;
                                            for (var i = 0; i < this.items.length; i++) {
                                                if (panel == this.items.getAt(i)) {
                                                    flag = false;
                                                } else if (flag) {
                                                    this.items.getAt(i).setTitle('序列' + (i + 1));
                                                } else {
                                                    this.items.getAt(i).setTitle('序列' + i);
                                                }
                                            }
                                        }
                                    }
                                });
                                this.add(panel);
                            }
                            if(data[i]['i_code']){
                                panel.getForm().findField('C24_IQS').setValue(data[i]['i_code']);
                            }
                            if(data[i]['j_code']) {
                                panel.getForm().findField('C24_JQS').setValue(data[i]['j_code']);
                            }
                            if(data[i]['ICSER2']) {
                                panel.getForm().findField('C24_ICSER2').setValue(data[i]['ICSER2']);
                            }
                            if(data[i]['ICSER3']) {
                                panel.getForm().findField('C24_ICSER3').setValue(data[i]['ICSER3']);
                            }
                        }
                    }
                } else {
                    Ext.Msg.alert('提示', '获取失败，请重试！');
                }
            }
        });
        Ext.Ajax.request({
            url: __path + '/inputview/getYuQiaoLiuliangFileData.do',
            method: 'POST',
            scope: this,
            params: {
                planId: this.planId
            },
            success: function (response) {
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    var data = result.attributes.data;
                    if (data) {
                        for (var i = 0; i < data.length; i++) {
                            var panel = this.items.getAt(i);
                            if (!panel) {
                                var index = this.items.length + 1;
                                panel = Ext.create('Ext.form.Panel', {
                                    autoScroll : true,
                                    layout: 'column',
                                    title: '序列' + index,
                                    items: [{
                                        xtype: 'fieldcontainer',
                                        columnWidth: 0.3,
                                        defaults: {
                                            margin: '5 5 5 10',
                                            labelWidth: 150,
                                            width: 250,
                                            submitValue: false
                                        },
                                        items: [{
                                            xtype: 'textfield',
                                            fieldLabel: '源汇项位置的I索引号',
                                            labelWidth: 170,
                                            allowBlank: false,
                                            name: 'C24_IQS'
                                        }, {
                                            xtype: 'textfield',
                                            fieldLabel: '源汇项位置的J索引号',
                                            labelWidth: 170,
                                            allowBlank: false,
                                            name: 'C24_JQS'
                                        }, {
                                            xtype: 'combo',
                                            editable: false,
                                            allowBlank: false,
                                            labelWidth: 170,
                                            name: 'C24_ICSER2',
                                            fieldLabel: '温度时间序列的编号',
                                            store: this.wenduStore,
                                            queryMode: 'local',
                                            displayField: 'value',
                                            valueField: 'value'
                                        }, {
                                            xtype: 'combo',
                                            editable: false,
                                            allowBlank: false,
                                            labelWidth: 170,
                                            name: 'C24_ICSER3',
                                            fieldLabel: '污染物时间序列的编号',
                                            store: this.wuranwuStore,
                                            queryMode: 'local',
                                            displayField: 'value',
                                            valueField: 'value'
                                        }, {
                                            xtype: 'filefield',
                                            margin: '5 0 5 10',
                                            labelWidth: 100,
                                            width: 300,
                                            name: 'inputFile',
                                            fieldLabel: '上传数据',
                                            buttonText: '文件选择'
                                        }, {
                                            xtype: 'button',
                                            width: 90,
                                            margin: '5 0 5 10',
                                            text: '数据保存',
                                            scope: this,
                                            handler: this.onSave
                                        }, {
                                            xtype: 'button',
                                            width: 90,
                                            margin: '5 0 5 10',
                                            text: '删除',
                                            scope: this,
                                            handler: this.onDelete
                                        }]
                                    }, Ext.create('Ext.grid.Panel', {
                                        margin: '10 0 0 10',
                                        columnWidth: 0.4,
                                        store: Ext.create('Ext.data.Store', {
                                            fields: ['day', 'liuliang'],
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
                                                text: '流量', dataIndex: 'liuliang', flex: 1, editor: {
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
                                        autoScroll: true
                                    })],
                                    listeners: {
                                        scope: this,
                                        close: function (panel, option) {
                                            debugger;
                                            var me = this;
                                            this.fireEvent('xuliechange', me.items.length - 1);
                                            var flag = true;
                                            for (var i = 0; i < this.items.length; i++) {
                                                if (panel == this.items.getAt(i)) {
                                                    flag = false;
                                                } else if (flag) {
                                                    this.items.getAt(i).setTitle('序列' + (i + 1));
                                                } else {
                                                    this.items.getAt(i).setTitle('序列' + i);
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
    afterFirstLayout: function () {
        debugger;
        if (this.planId) {
            this.requestDataFromDB();
        }
        this.callParent(arguments);
    }
});
