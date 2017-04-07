Ext.define('bitc.wrenv.inputview.inputDayinDuanMianPanel', {
    extend: 'Ext.form.Panel',
    border: false,
    defaults: {
        anchor: '98%',
        labelWidth: 80
    },
    initComponent: function () {
        var hdStore = this.hdStore = Ext.create('Ext.data.Store', {
            fields: ['name', 'value'],
            proxy: {
                type: 'ajax',
                url: __path + '/inputview/getHedaoListData.do',
                reader: {
                    type: 'json',
                    root: 'rows'
                }
            },
            autoLoad: true
        });
        var dmStore = this.dmStore =  Ext.create('Ext.data.Store', {
            fields: ['name', 'value'],
            proxy: {
                type: 'ajax',
                extraParams: {
                    hedao: 1
                },
                url: __path + '/inputview/getDuanMianListData.do',
                reader: {
                    type: 'json',
                    root: 'rows'
                }
            },
            autoLoad: true
        });
        this.items = [{
            xtype: 'fieldcontainer',
            isContext: false,
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
                //iconCls: 'add',
                text: '添加',
                scope: this,
                handler: function () {
                    this.add({
                        xtype: 'fieldcontainer',
                        isContext: true,
                        fieldLabel: '河道',
                        defaults: {
                            xtype: 'combo'
                        },
                        layout: {
                            type: 'hbox',
                            hideLabel: false,
                            defaultMargins: {
                                top: 0,
                                right: 5,
                                bottom: 5,
                                left: 0
                            }
                        },
                        items: [{
                            width: 200,
                            editable: false,
                            //name: 'sz_nspr',
                            submitValue: false,
                            valueField: 'value',
                            displayField: 'name',
                            allowBlank: false,
                            triggerAction: 'all',
                            store: hdStore,
                            listeners: {
                                change: function (comb, newValue, oldValue) {
                                    comb.up().items.getAt(1).getStore().proxy.extraParams['hedao'] = newValue;
                                    comb.up().items.getAt(1).getStore().reload();
                                    comb.up().items.getAt(1).setValue('');
                                }
                            }
                        }, {
                            width: 280,
                            editable: false,
                            fieldLabel: '断面',
                            //name: 'sz_nspr',
                            submitValue: false,
                            valueField: 'value',
                            displayField: 'name',
                            allowBlank: false,
                            triggerAction: 'all',
                            store:dmStore
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
            isContext: true,
            fieldLabel: '河道',
            defaults: {
                xtype: 'combo'
            },
            layout: {
                type: 'hbox',
                hideLabel: false,
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 0
                }
            },
            items: [{
                width: 200,
                editable: false,
                //name: 'sz_nspr',
                submitValue: false,
                valueField: 'value',
                displayField: 'name',
                allowBlank: false,
                triggerAction: 'all',
                store: hdStore,
                listeners: {
                    change: function (comb, newValue, oldValue) {
                        comb.up().items.getAt(1).getStore().proxy.extraParams['hedao'] = newValue;
                        comb.up().items.getAt(1).getStore().reload();
                        comb.up().items.getAt(1).setValue('');
                    }
                }
            }, {
                width: 280,
                editable: false,
                fieldLabel: '断面',
                //name: 'sz_nspr',
                submitValue: false,
                valueField: 'value',
                displayField: 'name',
                allowBlank: false,
                triggerAction: 'all',
                store: dmStore
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
    setHdValue: function (data) {
        var me = this;
        this.removeAll();
        this.add({
            xtype: 'fieldcontainer',
            isContext: false,
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
                text: '添加',
                scope: this,
                handler: function () {
                    this.add({
                        xtype: 'fieldcontainer',
                        isContext: true,
                        fieldLabel: '河道',
                        defaults: {
                            xtype: 'combo'
                        },
                        layout: {
                            type: 'hbox',
                            hideLabel: false,
                            defaultMargins: {
                                top: 0,
                                right: 5,
                                bottom: 5,
                                left: 0
                            }
                        },
                        items: [{
                            width: 200,
                            editable: false,
                            submitValue: false,
                            valueField: 'value',
                            displayField: 'name',
                            allowBlank: false,
                            triggerAction: 'all',
                            store: me.hdStore,
                            listeners: {
                                change: function (comb, newValue, oldValue) {
                                    comb.up().items.getAt(1).getStore().proxy.extraParams['hedao'] = newValue;
                                    comb.up().items.getAt(1).getStore().reload();
                                    comb.up().items.getAt(1).setValue('');
                                }
                            }
                        }, {
                            width: 280,
                            editable: false,
                            fieldLabel: '断面',
                            submitValue: false,
                            valueField: 'value',
                            displayField: 'name',
                            allowBlank: false,
                            triggerAction: 'all',
                            store: me.dmStore
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
        if (data) {
            var heduans = data.split(';');
            var duanmian = [];
            var hedao = [];
            for (var i = 0; i < heduans.length; i++) {
                duanmian.push(heduans[i].split(',')[0]);
                hedao.push(heduans[i].split(',')[1]);
            }
            for (var i = 0; i < heduans.length; i++) {
                this.add({
                    xtype: 'fieldcontainer',
                    isContext: true,
                    fieldLabel: '河道',
                    defaults: {
                        xtype: 'combo'
                    },
                    layout: {
                        type: 'hbox',
                        hideLabel: false,
                        defaultMargins: {
                            top: 0,
                            right: 5,
                            bottom: 5,
                            left: 0
                        }
                    },
                    items: [{
                        width: 200,
                        editable: false,
                        submitValue: false,
                        valueField: 'value',
                        displayField: 'name',
                        allowBlank: false,
                        triggerAction: 'all',
                        store: me.hdStore,
                        listeners: {
                            change: function (comb, newValue, oldValue) {
                                comb.up().items.getAt(1).getStore().proxy.extraParams['hedao'] = newValue;
                                comb.up().items.getAt(1).getStore().reload();
                                comb.up().items.getAt(1).setValue('');
                            },
                            afterrender: function (c, o) {
                                c.setValue(duanmian.shift());
                            }
                        }
                    }, {
                        width: 280,
                        editable: false,
                        fieldLabel: '断面',
                        submitValue: false,
                        valueField: 'value',
                        displayField: 'name',
                        allowBlank: false,
                        triggerAction: 'all',
                        store: me.dmStore,
                        listeners: {
                            afterrender: function (c, o) {
                                c.setValue(hedao.shift());
                            }
                        }
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
