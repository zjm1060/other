Ext.define('bitc.wrenv.inputview.inputShuikuPanel', {
    extend: 'Ext.form.Panel',
    border: false,
    water_body_id: null,
    defaults: {
        anchor: '100%',
        labelWidth: 80
    },
    initComponent: function () {
        var shuikuStore = this.shuikuStore = Ext.create('Ext.data.Store', {
            fields: ['shuiku', 'dmId'],
            proxy: {
                type: 'ajax',
                url: __path + '/inputview/getShuiKuListDate.do',
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
                    //    item.items.getAt(2).hide();
                    //}
                    this.add({
                        xtype: 'fieldcontainer',
                        fieldLabel: '水库',
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
                            store: this.shuikuStore,
                            editable: false,
                            valueField: 'dmId',
                            displayField: 'shuiku',
                            submitValue: false,
                            allowBlank: false,
                            triggerAction: 'all',
                            hideLabel: true,
                            width: 100
                        }, {
                            xtype: 'textfield',
                            allowBlank: false,
                            submitValue: false,
                            fieldLabel: '控制水位(m)',
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
            fieldLabel: '水库',
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
                store: shuikuStore,
                editable: false,
                valueField: 'dmId',
                displayField: 'shuiku',
                allowBlank: false,
                submitValue: false,
                triggerAction: 'all',
                hideLabel: true,
                value:89,
                //name: 'shuiku_item_1',
                width: 100
            }, {
                xtype: 'textfield',
                //name: 'shuiku_sw_1',
                allowBlank: false,
                submitValue: false,
                fieldLabel: '控制水位(m)',
                value: 86.2,
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
    setSkValue: function (data) {
        if (data && data.SHUIKU_LIST) {
            var shuikus = data.SHUIKU_LIST.split(';');
            var shuiku = [];
            var cof = [];
            for (var i = 0; i < shuikus.length; i++) {
                var data = shuikus[i].split(',');
                shuiku.push(data[0]);
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
                            fieldLabel: '水库',
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
                                store: this.shuikuStore,
                                editable: false,
                                valueField: 'dmId',
                                displayField: 'shuiku',
                                submitValue: false,
                                allowBlank: false,
                                triggerAction: 'all',
                                hideLabel: true,
                                width: 100
                            }, {
                                xtype: 'textfield',
                                allowBlank: false,
                                submitValue: false,
                                fieldLabel: '控制水位(m)',
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
            for (var i = 0; i < shuikus.length; i++) {
                this.add({
                    xtype: 'fieldcontainer',
                    fieldLabel: '水库',
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
                        store: this.shuikuStore,
                        editable: false,
                        valueField: 'dmId',
                        displayField: 'shuiku',
                        submitValue: false,
                        allowBlank: false,
                        triggerAction: 'all',
                        hideLabel: true,
                        width: 100,
                        listeners: {
                            afterrender: function (c, o) {
                                c.setValue(parseInt(shuiku.shift()));
                            }
                        }
                    }, {
                        xtype: 'textfield',
                        allowBlank: false,
                        submitValue: false,
                        fieldLabel: '控制水位(m)',
                        value: cof[i],
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
