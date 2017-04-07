Ext.define("bitc.wrenv.inputview.inputYuQiao_qixiang", {
    extend: 'Ext.form.Panel',
    layout: 'column',
    autoScroll: true,
    defaults: {
        anchor: '98%',
        labelWidth: 80
    },
    title: '气象边界参数',
    initComponent: function () {
        var gridstore = this.gridstore = Ext.create('Ext.data.Store', {
            fields: ['day', 'aser_l_c02', 'aser_l_c03', 'aser_l_c04', 'aser_l_c05', 'aser_l_c06', 'aser_l_c07', 'aser_l_c08'],
            data: {'items': []},
            proxy: {
                type: 'memory',
                reader: {
                    type: 'json',
                    root: 'items'
                }
            }
        });
        var grid = this.grid = Ext.create('Ext.grid.Panel', {
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
                }
                },
                {
                    text: '气压(hpa)', dataIndex: 'aser_l_c02', flex: 1, editor: {
                    xtype: 'textfield',
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
                },
                {
                    text: '气温(°C)', dataIndex: 'aser_l_c03', flex: 1, editor: {
                    xtype: 'textfield',
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
                },
                {
                    text: '相对湿度', dataIndex: 'aser_l_c04', flex: 1, editor: {
                    xtype: 'textfield',
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
                },
                {
                    text: '降雨(m/d)', dataIndex: 'aser_l_c05', flex: 1, editor: {
                    xtype: 'textfield',
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
                },
                {
                    text: '蒸发量(m/d)', dataIndex: 'aser_l_c06', flex: 1, editor: {
                    xtype: 'textfield',
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
                },
                {
                    text: '太阳辐射(w/m2)', dataIndex: 'aser_l_c07', flex: 1, editor: {
                    xtype: 'textfield',
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
                },
                {
                    text: '云层覆盖率', dataIndex: 'aser_l_c08', flex: 1, editor: {
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
            columnWidth: 0.6
        });
        var formPanel= this.formPanel = Ext.create('Ext.form.Panel', {
            autoScroll: true,
            border: false,
            columnWidth: 0.35,
            anchor: '98%',
            defaults: {
                margin: '5 5 5 10',
                labelWidth: 150,
                width: 250,
                submitValue:false
            },
            items: [{
                xtype: 'combo',
                editable: false,
                allowBlank: false,
                labelWidth: 110,
                name: 'aser_01',
                fieldLabel: '太阳辐射分配方法',
                store: Ext.create('Ext.data.Store', {
                    fields: ['abbr', 'name'],
                    data: [{'abbr': '0', 'name': '辐射进入河床'}, {'abbr': '1', 'name': '全部分配给表层'}]
                }),
                queryMode: 'local',
                displayField: 'name',
                valueField: 'abbr'
            }, {
                xtype: 'textfield',
                fieldLabel: '蒸发转化系数',
                name: 'aser_02',
                value: '-1'
            }, {
                xtype: 'textfield',
                fieldLabel: '对流转化系数',
                name: 'aser_03',
                value: '-1'
            }, {
                xtype: 'textfield',
                fieldLabel: '太阳长波辐射衰减系数',
                name: 'aser_04',
                value: '3'
            }, {
                xtype: 'textfield',
                fieldLabel: '太阳短波辐射衰减系数',
                name: 'aser_05',
                value: '0'
            }, {
                xtype: 'textfield',
                fieldLabel: '长波衰减的小树部分',
                name: 'aser_06',
                value: '1'
            }, {
                xtype: 'textfield',
                fieldLabel: '河床温度层的厚度',
                name: 'aser_07',
                value: '1'
            }, {
                xtype: 'textfield',
                fieldLabel: '初始河床温度',
                name: 'aser_08',
                value: '1'
            }, {
                xtype: 'textfield',
                fieldLabel: '河床和水体底层的转化系数1',
                labelWidth: 170,
                name: 'aser_09',
                value: '0.001'
            }, {
                xtype: 'textfield',
                fieldLabel: '河床和水体底层的转化系数2',
                labelWidth: 170,
                name: 'aser_10',
                value: '1'
            }, {
                xtype: 'filefield',
                labelWidth: 100,
                width: 300,
                name: 'inputFile',
                fieldLabel: '上传数据',
                buttonText: '文件选择'
            },{
                xtype: 'button',
                width: 100,
                margin: '10 10 0 10',
                text: '保存数据',
                scope: this,
                handler:this.onSave
            }]
        });
        this.items = [formPanel, grid];
        this.callParent(arguments);
    },
    onSave: function () {
        var baseForm = this.formPanel.getForm();
            if (baseForm.isValid()) {
                var aser_01 = baseForm.findField('aser_01').getValue();
                var aser_02 = baseForm.findField('aser_02').getValue();
                var aser_03 = baseForm.findField('aser_03').getValue();
                var aser_04 = baseForm.findField('aser_04').getValue();
                var aser_05 = baseForm.findField('aser_05').getValue();
                var aser_06 = baseForm.findField('aser_06').getValue();
                var aser_07 = baseForm.findField('aser_07').getValue();
                var aser_08 = baseForm.findField('aser_08').getValue();
                var aser_09 = baseForm.findField('aser_09').getValue();
                var aser_10 = baseForm.findField('aser_10').getValue();
                var data = {};
                data['aser_01'] = aser_01+'';
                data['aser_02'] = aser_02+'';
                data['aser_03'] = aser_03+'';
                data['aser_04'] = aser_04+'';
                data['aser_05'] = aser_05+'';
                data['aser_06'] = aser_06+'';
                data['aser_07'] = aser_07+'';
                data['aser_08'] = aser_08+'';
                data['aser_09'] = aser_09+'';
                data['aser_10'] = aser_10+'';
                var gridData = [];
                this.grid.getStore().each(function (record) {
                    gridData.push([
                        record.get('day'),
                        record.get('aser_l_c02'),
                        record.get('aser_l_c03'),
                        record.get('aser_l_c04'),
                        record.get('aser_l_c05'),
                        record.get('aser_l_c06'),
                        record.get('aser_l_c07'),
                        record.get('aser_l_c08')
                    ]);
            });

                baseForm.submit({
                    url: __path + '/inputview/addQiXiangData.do',
                    method: 'post',
                    params: {
                        planId:this.planId,
                        data:JSON.stringify(data),
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
        var keys =['aser_01','aser_02','aser_03','aser_04','aser_05','aser_06','aser_07','aser_08','aser_09','aser_10'];
        Ext.Ajax.request({
            url:__path+'/inputview/getYuQiaoInputData.do',
            method:'POST',
            scope:this,
            params:{
                planId:this.planId,
                keys:keys.join('#')
            },
            success:function(response){
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    var data = result.attributes.data;
                    if(data){
                        for(var i=0;i<keys.length;i++){
                            if(data[keys[i]]){
                                this.formPanel.getForm().findField(keys[i]).setValue(data[keys[i]]);
                            }
                        }
                    }
                } else {
                    Ext.Msg.alert('提示','获取失败，请重试！');
                }
            }
        });
        Ext.Ajax.request({
            url:__path+'/inputview/getYuQiaoQiXiangFileData.do',
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
