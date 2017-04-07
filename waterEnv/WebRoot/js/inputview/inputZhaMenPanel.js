Ext.define('bitc.wrenv.inputview.inputZhaMenPanel', {
    extend: 'Ext.form.Panel',
    border: false,
    defaults: {
        anchor: '100%',
        labelWidth: 80
    },
    initComponent: function () {
        var zhamenStore = this.zhamenStore = Ext.create('Ext.data.Store', {
            fields: ['name', 'dmId'],
            proxy: {
                type: 'ajax',
                url: __path + '/inputview/getZhaMenNameAndDmId.do',
                reader: {
                    type: 'json',
                    root: 'rows'
                },
            },
            autoLoad: true
        });
        var HQPanels = [];
        Ext.Ajax.request({
            url: __path + "/inputview/getZhaMenListDate.do",
            method: "post",
            async:false,
            success: function (response) {
                var resp = Ext.decode(response.responseText);
                var data = resp.attributes.data;
                for (var i = 0; i < data.length; i++) {
                    HQPanels.push(Ext.create('bitc.wrenv.inputview.HQShowPanel', {
                        hidden: true,
                        zhamenName: data[i].name,
                        data: data[i].shuiwei,
                        xData: data[i].liuliang
                    }));
                }
            },
            scope: this
        });
        var win = this.win = Ext.create('Ext.window.Window', {
            title: 'HQ曲线图',
            height: 600,
            width: 700,
            layout: 'fit',
            items: HQPanels
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
                        fieldLabel: '闸门',
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
                            store: zhamenStore,
                            editable: false,
                            valueField: 'dmId',
                            displayField: 'name',
                            allowBlank: false,
                            submitValue: false,
                            triggerAction: 'all',
                            hideLabel: true,
                            //name: 'zhamen_item_'+count,
                            width: 100
                        }, {
                            xtype: 'textfield',
                            //name: 'zhamen_conf_'+count,
                            allowBlank: false,
                            submitValue: false,
                            value: 0,
                            hidden:true,
                            fieldLabel: '闸门系数',
                            hideLabel: false,
                            width: 200
                        }, {
                            xtype: 'button',
                            iconCls: 'linechart',
                            listeners: {
                                scope: this,
                                mouseover: function (item, e, eOpts) {
                                    debugger;
                                    var name = item.up().down('combo').getRawValue();
                                    this.win.items.each(function (item) {
                                        if (item.zhamenName == name) {
                                            item.show();
                                        }
                                    });
                                    if (name) {
                                        this.win.show();
                                    }
                                },
                                mouseout: function (item, e, eOpts) {
                                    var name = item.up().down('combo').getRawValue();
                                    this.win.items.each(function (item) {
                                        if (item.zhamenName == name) {
                                            item.hide();
                                        }
                                    });
                                    if (name) {
                                        this.win.hide();
                                    }
                                }
                            }
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
                                //if(items.length>1){
                                //    var item = items.getAt(count-1);
                                //    item.items.getAt(3).show();
                                //}
                            }
                        }]
                    });
                }
            }]
        }, {
            xtype: 'fieldcontainer',
            fieldLabel: '闸门',
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
                store: zhamenStore,
                editable: false,
                valueField: 'dmId',
                displayField: 'name',
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                value:"8",
                //name: 'zhamen_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhamen_conf_1',
                allowBlank: false,
                value: 0,
                hidden:true,
                fieldLabel: '闸门系数',
                submitValue: false,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'linechart',
                listeners: {
                    scope: this,
                    mouseover: function (item, e, eOpts) {
                        debugger;
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.show();
                            }
                        });
                        if (name) {
                            this.win.show();
                        }
                    },
                    mouseout: function (item, e, eOpts) {
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.hide();
                            }
                        });
                        if (name) {
                            this.win.hide();
                        }
                    }
                }
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
                    //if(items.length>1){
                    //    var item = items.getAt(count-1);
                    //    item.items.getAt(3).show();
                    //}
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '闸门',
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
                store: zhamenStore,
                editable: false,
                valueField: 'dmId',
                displayField: 'name',
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhamen_item_1',
                value:"56",
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhamen_conf_1',
                allowBlank: false,
                value: 0,
                hidden:true,
                fieldLabel: '闸门系数',
                submitValue: false,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'linechart',
                listeners: {
                    scope: this,
                    mouseover: function (item, e, eOpts) {
                        debugger;
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.show();
                            }
                        });
                        if (name) {
                            this.win.show();
                        }
                    },
                    mouseout: function (item, e, eOpts) {
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.hide();
                            }
                        });
                        if (name) {
                            this.win.hide();
                        }
                    }
                }
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
                    //if(items.length>1){
                    //    var item = items.getAt(count-1);
                    //    item.items.getAt(3).show();
                    //}
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '闸门',
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
                store: zhamenStore,
                editable: false,
                valueField: 'dmId',
                displayField: 'name',
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhamen_item_1',
                value:"58",
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhamen_conf_1',
                allowBlank: false,
                value: 0,
                hidden:true,
                fieldLabel: '闸门系数',
                submitValue: false,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'linechart',
                listeners: {
                    scope: this,
                    mouseover: function (item, e, eOpts) {
                        debugger;
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.show();
                            }
                        });
                        if (name) {
                            this.win.show();
                        }
                    },
                    mouseout: function (item, e, eOpts) {
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.hide();
                            }
                        });
                        if (name) {
                            this.win.hide();
                        }
                    }
                }
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
                    //if(items.length>1){
                    //    var item = items.getAt(count-1);
                    //    item.items.getAt(3).show();
                    //}
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '闸门',
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
                store: zhamenStore,
                editable: false,
                valueField: 'dmId',
                displayField: 'name',
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhamen_item_1',
                value:"60",
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhamen_conf_1',
                allowBlank: false,
                value: 0,
                hidden:true,
                fieldLabel: '闸门系数',
                submitValue: false,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'linechart',
                listeners: {
                    scope: this,
                    mouseover: function (item, e, eOpts) {
                        debugger;
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.show();
                            }
                        });
                        if (name) {
                            this.win.show();
                        }
                    },
                    mouseout: function (item, e, eOpts) {
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.hide();
                            }
                        });
                        if (name) {
                            this.win.hide();
                        }
                    }
                }
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
                    //if(items.length>1){
                    //    var item = items.getAt(count-1);
                    //    item.items.getAt(3).show();
                    //}
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '闸门',
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
                store: zhamenStore,
                editable: false,
                valueField: 'dmId',
                displayField: 'name',
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhamen_item_1',
                value:"61",
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhamen_conf_1',
                allowBlank: false,
                value: 0,
                hidden:true,
                fieldLabel: '闸门系数',
                submitValue: false,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'linechart',
                listeners: {
                    scope: this,
                    mouseover: function (item, e, eOpts) {
                        debugger;
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.show();
                            }
                        });
                        if (name) {
                            this.win.show();
                        }
                    },
                    mouseout: function (item, e, eOpts) {
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.hide();
                            }
                        });
                        if (name) {
                            this.win.hide();
                        }
                    }
                }
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
                    //if(items.length>1){
                    //    var item = items.getAt(count-1);
                    //    item.items.getAt(3).show();
                    //}
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '闸门',
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
                store: zhamenStore,
                editable: false,
                valueField: 'dmId',
                displayField: 'name',
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhamen_item_1',
                value:"65",
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhamen_conf_1',
                allowBlank: false,
                value: 0,
                hidden:true,
                fieldLabel: '闸门系数',
                submitValue: false,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'linechart',
                listeners: {
                    scope: this,
                    mouseover: function (item, e, eOpts) {
                        debugger;
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.show();
                            }
                        });
                        if (name) {
                            this.win.show();
                        }
                    },
                    mouseout: function (item, e, eOpts) {
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.hide();
                            }
                        });
                        if (name) {
                            this.win.hide();
                        }
                    }
                }
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
                    //if(items.length>1){
                    //    var item = items.getAt(count-1);
                    //    item.items.getAt(3).show();
                    //}
                }
            }]
        },{
            xtype: 'fieldcontainer',
            fieldLabel: '闸门',
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
                store: zhamenStore,
                editable: false,
                valueField: 'dmId',
                displayField: 'name',
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                //name: 'zhamen_item_1',
                value:"66",
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'zhamen_conf_1',
                allowBlank: false,
                value: 0,
                hidden:true,
                fieldLabel: '闸门系数',
                submitValue: false,
                hideLabel: false,
                width: 200
            }, {
                xtype: 'button',
                iconCls: 'linechart',
                listeners: {
                    scope: this,
                    mouseover: function (item, e, eOpts) {
                        debugger;
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.show();
                            }
                        });
                        if (name) {
                            this.win.show();
                        }
                    },
                    mouseout: function (item, e, eOpts) {
                        var name = item.up().down('combo').getRawValue();
                        this.win.items.each(function (item) {
                            if (item.zhamenName == name) {
                                item.hide();
                            }
                        });
                        if (name) {
                            this.win.hide();
                        }
                    }
                }
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
                    //if(items.length>1){
                    //    var item = items.getAt(count-1);
                    //    item.items.getAt(3).show();
                    //}
                }
            }]
        }]

        this.callParent(arguments);
    },
    setZmValue: function (data) {
        if (data && data.ZHAMEN_LIST) {
            var zhamens = data.ZHAMEN_LIST.split(';');
            var zhanmen = [];
            var cof = [];
            for (var i = 0; i < zhamens.length; i++) {
                var data = zhamens[i].split(',');
                zhanmen.push(data[0]);
                cof.push(data[1]);
            }
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
                            fieldLabel: '闸门',
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
                                store: this.zhamenStore,
                                editable: false,
                                valueField: 'dmId',
                                displayField: 'name',
                                allowBlank: false,
                                submitValue: false,
                                triggerAction: 'all',
                                hideLabel: true,
                                width: 100
                            }, {
                                xtype: 'textfield',
                                allowBlank: false,
                                submitValue: false,
                                value: 0,
                                hidden:true,
                                fieldLabel: '闸门系数',
                                hideLabel: false,
                                width: 200
                            }, {
                                xtype: 'button',
                                iconCls: 'linechart',
                                listeners: {
                                    scope: this,
                                    mouseover: function (item, e, eOpts) {
                                        var name = item.up().down('combo').getRawValue();
                                        this.win.items.each(function (item) {
                                            if (item.zhamenName == name) {
                                                item.show();
                                            }
                                        });
                                        if (name) {
                                            this.win.show();
                                        }
                                    },
                                    mouseout: function (item, e, eOpts) {
                                        var name = item.up().down('combo').getRawValue();
                                        this.win.items.each(function (item) {
                                            if (item.zhamenName == name) {
                                                item.hide();
                                            }
                                        });
                                        if (name) {
                                            this.win.hide();
                                        }
                                    }
                                }
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
                        });
                    }
                }]
            });
            for (var i = 0; i < zhamens.length; i++) {
                this.add({
                    xtype: 'fieldcontainer',
                    fieldLabel: '闸门',
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
                        store: this.zhamenStore,
                        editable: false,
                        valueField: 'dmId',
                        displayField: 'name',
                        allowBlank: false,
                        submitValue: false,
                        triggerAction: 'all',
                        hideLabel: true,
                        width: 100,
                        listeners: {
                            afterrender: function (c, o) {
                                c.setValue(zhanmen.shift());
                            }
                        }
                    }, {
                        xtype: 'textfield',
                        allowBlank: false,
                        submitValue: false,
                        value: cof[i],
                        hidden:true,
                        fieldLabel: '闸门系数',
                        hideLabel: false,
                        width: 200
                    }, {
                        xtype: 'button',
                        iconCls: 'linechart',
                        listeners: {
                            scope: this,
                            mouseover: function (item, e, eOpts) {
                                debugger;
                                var name = item.up().down('combo').getRawValue();
                                this.win.items.each(function (item) {
                                    if (item.zhamenName == name) {
                                        item.show();
                                    }
                                });
                                if (name) {
                                    this.win.show();
                                }
                            },
                            mouseout: function (item, e, eOpts) {
                                var name = item.up().down('combo').getRawValue();
                                this.win.items.each(function (item) {
                                    if (item.zhamenName == name) {
                                        item.hide();
                                    }
                                });
                                if (name) {
                                    this.win.hide();
                                }
                            }
                        }
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
                });
            }
        }
    }
});
