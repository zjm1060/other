Ext.define('bitc.wrenv.inputview.inputZhiliuPanel', {
    extend: 'Ext.form.Panel',
    border: false,
    water_body_id: null,
    defaults: {
        anchor: '100%',
        labelWidth: 80
    },
    initComponent: function () {
        var zhiliuStore = this.zhiliuStore = Ext.create('Ext.data.Store', {
            fields: ['heliuName', 'dmAndFileName'],
            proxy: {
                type: 'ajax',
                url: __path + '/inputview/getZhiliuListData.do',
                reader: {
                    type: 'json',
                    root: 'rows'
                },
                extraParams: {water_body_id: this.water_body_id}
            },
            scope: this,
            autoLoad: true
        });
        this.items = [{
            xtype: 'fieldcontainer',
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
                xtype: 'button',
                hideLabel: true,
                anchor: '20%',
                //iconCls: 'add',
                text: '添加',
                scope: this,
                handler: function () {
                    //var items = this.items;
                    //var count = items.length;
                    //var item = items.getAt(count-1);
                    //if(count>1){
                    //    item.items.getAt(3).hide();
                    //}
                    this.add({
                        xtype: 'fieldcontainer',
                        fieldLabel: '支流',
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
                            xtype: 'combo',
                            store: zhiliuStore,
                            editable: false,
                            valueField: 'dmAndFileName',
                            displayField: 'heliuName',
                            submitValue: false,
                            allowBlank: false,
                            triggerAction: 'all',
                            hideLabel: true,
                            //name: 'zhiliu_item_'+count,
                            width: 100
                        }, {
                            xtype: 'textfield',
                            //name: 'zhiliu_BAA_'+count,
                            allowBlank: false,
                            submitValue: false,
                            disabled:true,
                            fieldLabel: '支流流域面积',
                            value: 0,
                            hideLabel: false,
                            width: 200
                        }, {
                            xtype: 'textfield',
                            //name: 'zhiliu_BAASA_'+count,
                            allowBlank: false,
                            submitValue: false,
                            disabled:true,
                            fieldLabel: '支流系数',
                            value: 0,
                            hideLabel: false,
                            width: 200
                        }, {
                            xtype: 'button',
                            iconCls: 'delete',
                            scope: this,
                            handler: function (btn, e) {
                                var me = this;
                                Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                                    if(e =='yes'){
                                        me.remove(btn.up().id);
                                    }
                                });
                            }
                        }]
                    });
                }
            }]
        }, {
            xtype: 'fieldcontainer',
            fieldLabel: '支流',
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
                xtype: 'combo',
                store: zhiliuStore,
                editable: false,
                valueField: 'dmAndFileName',
                displayField: 'heliuName',
                value:"157,Nans.txt",
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhiliu_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流流域面积',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAASA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流系数',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'delete',
                scope: this,
                handler: function (btn, e) {
                    var me = this;
                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                        if(e =='yes'){
                            me.remove(btn.up().id);
                        }
                    });
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '支流',
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
                xtype: 'combo',
                store: zhiliuStore,
                editable: false,
                valueField: 'dmAndFileName',
                displayField: 'heliuName',
                value:"155,Liuh.txt",
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhiliu_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流流域面积',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAASA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流系数',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'delete',
                scope: this,
                handler: function (btn, e) {
                    var me = this;
                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                        if(e =='yes'){
                            me.remove(btn.up().id);
                        }
                    });
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '支流',
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
                xtype: 'combo',
                store: zhiliuStore,
                editable: false,
                valueField: 'dmAndFileName',
                displayField: 'heliuName',
                value:"132,Beis.txt",
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhiliu_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流流域面积',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAASA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流系数',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'delete',
                scope: this,
                handler: function (btn, e) {
                    var me = this;
                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                        if(e =='yes'){
                            me.remove(btn.up().id);
                        }
                    });
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '支流',
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
                xtype: 'combo',
                store: zhiliuStore,
                editable: false,
                valueField: 'dmAndFileName',
                displayField: 'heliuName',
                value:"119,Xink.txt",
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhiliu_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流流域面积',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAASA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流系数',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'delete',
                scope: this,
                handler: function (btn, e) {
                    var me = this;
                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                        if(e =='yes'){
                            me.remove(btn.up().id);
                        }
                    });
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '支流',
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
                xtype: 'combo',
                store: zhiliuStore,
                editable: false,
                valueField: 'dmAndFileName',
                displayField: 'heliuName',
                value:"112,Tang.txt",
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhiliu_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流流域面积',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAASA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流系数',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'delete',
                scope: this,
                handler: function (btn, e) {
                    var me = this;
                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                        if(e =='yes'){
                            me.remove(btn.up().id);
                        }
                    });
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '支流',
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
                xtype: 'combo',
                store: zhiliuStore,
                editable: false,
                valueField: 'dmAndFileName',
                displayField: 'heliuName',
                value:"91,Lanh.txt",
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhiliu_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流流域面积',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAASA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流系数',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'delete',
                scope: this,
                handler: function (btn) {
                    var me = this;
                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                        if(e =='yes'){
                            me.remove(btn.up().id);
                        }
                    });
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '支流',
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
                xtype: 'combo',
                store: zhiliuStore,
                editable: false,
                valueField: 'dmAndFileName',
                displayField: 'heliuName',
                value:"87,Xihe.txt",
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhiliu_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流流域面积',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAASA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流系数',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'delete',
                scope: this,
                handler: function (btn, e) {
                    var me = this;
                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                        if(e =='yes'){
                            me.remove(btn.up().id);
                        }
                    });
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '支流',
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
                xtype: 'combo',
                store: zhiliuStore,
                editable: false,
                valueField: 'dmAndFileName',
                displayField: 'heliuName',
                value:"55,Wolo.txt",
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhiliu_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流流域面积',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAASA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流系数',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'delete',
                scope: this,
                handler: function (btn, e) {
                    var me = this;
                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                        if(e =='yes'){
                            me.remove(btn.up().id);
                        }
                    });
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '支流',
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
                xtype: 'combo',
                store: zhiliuStore,
                editable: false,
                valueField: 'dmAndFileName',
                displayField: 'heliuName',
                value:"47,Xjia.txt",
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhiliu_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流流域面积',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAASA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流系数',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'delete',
                scope: this,
                handler: function (btn, e) {
                    var me = this;
                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                        if(e =='yes'){
                            me.remove(btn.up().id);
                        }
                    });
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '支流',
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
                xtype: 'combo',
                store: zhiliuStore,
                editable: false,
                valueField: 'dmAndFileName',
                displayField: 'heliuName',
                value:"39,Wuda.txt",
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhiliu_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流流域面积',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAASA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流系数',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'delete',
                scope: this,
                handler: function (btn, e) {
                    var me = this;
                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                        if(e =='yes'){
                            me.remove(btn.up().id);
                        }
                    });
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '支流',
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
                xtype: 'combo',
                store: zhiliuStore,
                editable: false,
                valueField: 'dmAndFileName',
                displayField: 'heliuName',
                value:"7,XTan.txt",
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhiliu_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流流域面积',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'textfield',
                //name: 'zhiliu_BAASA_1',
                allowBlank: false,
                submitValue: false,
                disabled:true,
                fieldLabel: '支流系数',
                value: 0,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'delete',
                scope: this,
                handler: function (btn, e) {
                    var me = this;
                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                        if(e =='yes'){
                            me.remove(btn.up().id);
                        }
                    });
                }
            }]
        }];
        this.callParent(arguments);
    },
    setZlValue: function (data) {
        if (data && data.ZHILIU_DM_LIST) {
            var zhilius = data.ZHILIU_DM_LIST.split(',');
            var zhiliuNames = data.ZHILIU_NAME_LIST.split(',');
            var zhiliuCof = data.ZHILIU_COF.split(';');
            this.removeAll();
            this.add({
                xtype: 'fieldcontainer',
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
                    xtype: 'button',
                    hideLabel: true,
                    anchor: '20%',
                    text: '添加',
                    scope: this,
                    handler: function () {
                        this.add({
                            xtype: 'fieldcontainer',
                            fieldLabel: '支流',
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
                                xtype: 'combo',
                                store: this.zhiliuStore,
                                editable: false,
                                valueField: 'dmAndFileName',
                                displayField: 'heliuName',
                                submitValue: false,
                                allowBlank: false,
                                triggerAction: 'all',
                                hideLabel: true,
                                width: 100
                            }, {
                                xtype: 'textfield',
                                allowBlank: false,
                                submitValue: false,
                                disabled:true,
                                fieldLabel: '支流流域面积',
                                value: 0,
                                hideLabel: false,
                                width: 200
                            }, {
                                xtype: 'textfield',
                                allowBlank: false,
                                submitValue: false,
                                disabled:true,
                                fieldLabel: '支流系数',
                                value: 0,
                                hideLabel: false,
                                width: 200
                            }, {
                                xtype: 'button',
                                iconCls: 'delete',
                                scope: this,
                                handler: function (btn, e) {
                                    var me = this;
                                    Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                                        if(e =='yes'){
                                            me.remove(btn.up().id);
                                        }
                                    });
                                }
                            }]
                        });
                    }
                }]
            });
            for (var i = 0; i < zhilius.length; i++) {
                var cof = zhiliuCof[i].split(',');
                this.add({
                    xtype: 'fieldcontainer',
                    fieldLabel: '支流',
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
                        xtype: 'combo',
                        store: this.zhiliuStore,
                        editable: false,
                        valueField: 'dmAndFileName',
                        displayField: 'heliuName',
                        submitValue: false,
                        allowBlank: false,
                        triggerAction: 'all',
                        hideLabel: true,
                        width: 100,
                        listeners: {
                            afterrender: function (c, o) {
                                c.setValue(zhilius.shift()+','+zhiliuNames.shift());
                            }
                        }
                    }, {
                        xtype: 'textfield',
                        allowBlank: false,
                        submitValue: false,
                        disabled:true,
                        fieldLabel: '支流流域面积',
                        value: cof[0],
                        hideLabel: false,
                        width: 200,

                    }, {
                        xtype: 'textfield',
                        //name: 'zhiliu_BAASA_'+count,
                        allowBlank: false,
                        submitValue: false,
                        disabled:true,
                        fieldLabel: '支流系数',
                        value: cof[1],
                        hideLabel: false,
                        width: 200
                    }, {
                        xtype: 'button',
                        iconCls: 'delete',
                        scope: this,
                        handler: function (btn, e) {
                            var me = this;
                            Ext.MessageBox.confirm("提示","确定要删除此项配置？",function(e){
                                if(e =='yes'){
                                    me.remove(btn.up().id);
                                }
                            });
                        }
                    }]
                });
            }
        }
    }
});
